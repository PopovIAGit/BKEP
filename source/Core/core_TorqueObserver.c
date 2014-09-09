/*======================================================================
»м€ файла:          core_TorqueObserver.c
јвтор:				ѕопов ».ј.
¬ерси€ файла:		1.00
ƒата изменени€:		16.04.2014
ќписание:
модуль управлени€ задвижкой
======================================================================*/

#include "core.h"

// фиксированные точки дл€ сн€ти€ момента
Int VoltArray[CUB_COUNT1] = {160, 190,  220,  250};
Int CurrArray[CUB_COUNT2] = {400, 800, 1200, 1600, 2000};
Int AnUIArray[CUB_COUNT2] = { 25,  35,   50,   65,   80};


void CubInit(TCubStr *p, TCubConfig *Cfg)	//инициализаци€ куба
{
	register TCubPoint *Pt;
	register Uns i, j, Tmp;

	p->InputX = Cfg->X_Value;
	p->InputY = Cfg->Y_Value;

	p->Min.X  = 0x7FFF;
	p->Max.X  = 0x8000;
	p->Min.Y  = 0x7FFF;
	p->Max.Y  = 0x8000;

	for (i=0; i < CUB_COUNT1; i++)
	{
		Tmp = Cfg->X_Array[i];
		for (j=0; j < CUB_COUNT2; j++)
		{
			Pt = &p->Points[i][j];
			Pt->X = Tmp;
			Pt->Y = Cfg->Y_Array[j];

			if (Pt->X < p->Min.X) p->Min.X = Pt->X;
			if (Pt->X > p->Max.X) p->Max.X = Pt->X;
			if (Pt->Y < p->Min.Y) p->Min.Y = Pt->Y;
			if (Pt->Y > p->Max.Y) p->Max.Y = Pt->Y;
		}
	}
}


void CubRefresh(TCubStr *p, TCubArray *Array)	//
{
	register TCubPoint *Pt;
	register Uns i, j;

	for (i=0; i < CUB_COUNT1; i++)
	{
		for (j=0; j < CUB_COUNT2; j++)
		{
			Pt = &p->Points[i][j];
			Pt->Z = Array->Data[i][j];
		}
	}
}

void CubCalc(TCubStr *p)
{
	Int A0, A1, B0, B1;

	// контроль входных данных на случай если они выпали за пределы куба
	p->Input.X = *p->InputX;
	p->Input.Y = *p->InputY;

	if (p->Input.X < p->Min.X) p->Input.X = p->Min.X;
	if (p->Input.X > p->Max.X) p->Input.X = p->Max.X;
	if (p->Input.Y < p->Min.Y) p->Input.Y = p->Min.Y;
	if (p->Input.Y > p->Max.Y) p->Input.Y = p->Max.Y;

	// поиск квадрата
	for (p->Num1=0; p->Num1 < CUB_COUNT1-1; p->Num1++)
		if (p->Input.X < p->Points[p->Num1+1][0].X) break;
	for (p->Num2=0; p->Num2 < CUB_COUNT2-1; p->Num2++)
		if (p->Input.Y < p->Points[p->Num1][p->Num2+1].Y) break;

	// расчет точки R
	A0 = p->Points[p->Num1][p->Num2].X;
	A1 = p->Points[p->Num1+1][p->Num2].X;
	B0 = p->Points[p->Num1][p->Num2].Z;
	B1 = p->Points[p->Num1+1][p->Num2].Z;

	p->PtR  = B0 + _IQ1div((LgInt)(p->Input.X - A0) * (B1 - B0), (A1 - A0) << 1);

	// расчет точки T
	A0 = p->Points[p->Num1][p->Num2+1].X;
	A1 = p->Points[p->Num1+1][p->Num2+1].X;
	B0 = p->Points[p->Num1][p->Num2+1].Z;
	B1 = p->Points[p->Num1+1][p->Num2+1].Z;

	p->PtT  = B0 + _IQ1div((LgInt)(p->Input.X - A0) * (B1 - B0), (A1 - A0) << 1);

	// последний расчет
	A0 = p->Points[p->Num1][p->Num2].Y;
	A1 = p->Points[p->Num1][p->Num2+1].Y;
	B0 = p->PtR;
	B1 = p->PtT;

	p->Output = B0 + _IQ1div((LgInt)(p->Input.Y - A0) * (B1 - B0), (A1 - A0) << 1);
}

// »нициализаци€
void Core_TorqueInit(TTorqObs *p)
{
	// перва€ поверхность U и I - большие токи
	p->TqCurr.X_Value = (Int *)&g_Ram.ramGroupH.Umid;
	p->TqCurr.X_Array = VoltArray;
	p->TqCurr.Y_Value = (Int *)&g_Ram.ramGroupH.Imidpr;
	p->TqCurr.Y_Array = CurrArray;

	// втора€ поверхность U и AngFi - малые токи
	p->TqAngUI.X_Value = (Int *)&g_Ram.ramGroupH.Umid;
	p->TqAngUI.X_Array = VoltArray;
	p->TqAngUI.Y_Value = (Int *)&g_Ram.ramGroupC.AngleUI;
	p->TqAngUI.Y_Array = AnUIArray;

	// «абераем средний ток
	p->Imidpr	 	= &g_Ram.ramGroupH.Imidpr;
	p->TorqueSetPr  = &g_Core.MotorControl.TorqueSetPr;

	// »нициализируем поверхности
	CubInit(&p->Cub1, &p->TqCurr);		 // инициализаци€ структуры куба дл€ расчета момента
	CubInit(&p->Cub2, &p->TqAngUI);

	// »нициализируем фильтр момента
	peref_ApFilter3Init(&p->Trqfltr, (LgUns)Prd18kHZ, 1);
}

// –асчет момента
void Core_TorqueCalc(TTorqObs *p)
{
		register TCubStr *Cub;	//
		register Int Tmp;
		register Int Add;		// ручной контроль индикации момента

		if (!p->ObsEnable) {p->Indication = 0; return;} //если выключен расчет момента то индикаци€ 0 и закрываем тиристоры и выходим

		Cub = (p->Imidpr >= p->TransCurr) ? &p->Cub1 : &p->Cub2;  // выбераем по какому кубу работаем дл€ маленьких или больших токов

		CubCalc(Cub);	// считаем выбранный куб

		if 		(*p->TorqueSetPr < 40) 	Add = p->Corr40Trq;		// ƒобавил PIA 09.10.2012
		else if (*p->TorqueSetPr < 60)	Add = p->Corr60Trq;		// корректировка индикации момента дл€ +- изменени€ времени перехода на поверхность упора
		else if (*p->TorqueSetPr < 80)	Add = p->Corr80Trq;
		else if (*p->TorqueSetPr < 110)	Add = p->Corr110Trq;

		p->Trqfltr.Input = _IQ16toIQ(Cub->Output); // фильтруем значение момента
		peref_ApFilter3Calc(&p->Trqfltr);
		Tmp =(_IQtoIQ16(p->Trqfltr.Output)) + Add;

		if (Tmp < TORQ_MIN_PR) Tmp = TORQ_MIN_PR;	// провер€ем на вхождение в зону от
		if (Tmp > TORQ_MAX_PR) Tmp = TORQ_MAX_PR;   // 10 до 110 %

		p->Indication = (Tmp * p->TorqueMax) / 100;//PU0ToValue(Tmp, p->TorqueMax);// переводим проценты в Ќм относительно максимального ћ
}

