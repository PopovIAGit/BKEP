/*======================================================================
Имя файла:          core_TorqueObserver.c
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		16.04.2014
Описание:
модуль управления задвижкой
======================================================================*/

#include "core.h"
#include "peref.h"

// фиксированные точки для снятия момента
Int VoltArray[CUB_COUNT1] 	= {160, 190,  220,  250};

Int CurrArray[CUB_COUNT2] 		= {400, 800, 1200, 1600, 2000, 3000};		// добавил 3000 PIA 14.11.17
Int CurrArray50000[CUB_COUNT2] 	= {400, 600, 800, 1200, 1600, 2000}; // для эпц 50000
Int CurrArray35000[CUB_COUNT2] 	= {400, 500, 600, 800, 1200, 1600}; // для эпц 35000


Int AnUIArray100A50[CUB_COUNT2] 	= { 50,  55,   65,   69,   73}; //для эпц100а50
Int AnUIArray[CUB_COUNT2] 			= {25, 35, 50, 65, 70,  80};	// добавил 70 	PIA 14.11.17
Int AnUIArray100A25[CUB_COUNT2] 	= {50,  61,   69,   74,   80};  // для эпцр 100 а25

Int AnUIArrayEPZR2000[CUB_COUNT2]	= {40,50,55,65,70,75};

void CubInit(TCubStr *p, TCubConfig *Cfg)	//инициализация куба
{
	register TCubPoint *Pt;
	register Uns i, j, Tmp;

	p->InputX = Cfg->X_Value;//среднее напряжение
	p->InputY = Cfg->Y_Value;//средний ток в процентах от номинального

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
    int16 A0, A1, B0, B1;

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

    	p->PtR  = B0 + (int16)((float32)(p->Input.X - A0) * (B1 - B0) / (A1 - A0));

    	// расчет точки T
    	A0 = p->Points[p->Num1][p->Num2+1].X;
    	A1 = p->Points[p->Num1+1][p->Num2+1].X;
    	B0 = p->Points[p->Num1][p->Num2+1].Z;
    	B1 = p->Points[p->Num1+1][p->Num2+1].Z;

    	p->PtT  = B0 + (int16)((float32)(p->Input.X - A0) * (B1 - B0) / (A1 - A0));

    	// последний расчет
    	A0 = p->Points[p->Num1][p->Num2].Y;
    	A1 = p->Points[p->Num1][p->Num2+1].Y;
    	B0 = p->PtR;
    	B1 = p->PtT;

    	p->Output  = B0 + (int16)((float32)(p->Input.Y - A0) * (B1 - B0) / (A1 - A0));
}

// Инициализация
void Core_TorqueInit(TTorqObs *p)
{
	if (g_Ram.ramGroupC.DriveType == dt50000_F48)
	{
		//CurrArray = CurrArray50000;//{400, 600, 800, 1200, 1600};
		memcpy(CurrArray, CurrArray50000, sizeof(CurrArray));
	}

	if (g_Ram.ramGroupC.DriveType == dt35000_F48 || g_Ram.ramGroupC.DriveType == dt15000_D10_T )
		{
			//CurrArray = CurrArray50000;//{400, 600, 800, 1200, 1600};
			memcpy(CurrArray, CurrArray35000, sizeof(CurrArray));
		}

	if (g_Ram.ramGroupC.DriveType == dt100_A25_S || g_Ram.ramGroupC.DriveType == dt100_A25)
	{
		//AnUIArray =  AnUIArray100A25;//{50,  61,   69,   74,   80};
		memcpy(AnUIArray, AnUIArray100A25, sizeof(AnUIArray));
	}

	if (g_Ram.ramGroupC.DriveType == dt100_A50 || g_Ram.ramGroupC.DriveType == dt100_A50_S)
	{
		//AnUIArray = AnUIArray100A50;//{ 50,  55,   65,   69,   73};
		memcpy(AnUIArray, AnUIArray100A50, sizeof(AnUIArray));
	}
	if (g_Ram.ramGroupC.DriveType == dt2000_EPZR)
	{
		memcpy(AnUIArray, AnUIArrayEPZR2000, sizeof(AnUIArray));
	}

	p->TqCurr.X_Value = (Int *)&g_Ram.ramGroupH.Umid;// - VoltTmp;
	p->TqCurr.X_Array = VoltArray;
	p->TqCurr.Y_Value = (Int *)&g_Ram.ramGroupH.Imidpr;
	p->TqCurr.Y_Array = CurrArray;

	// вторая поверхность U и AngFi - малые токи
	p->TqAngUI.X_Value = (Int *)&g_Ram.ramGroupH.Umid;
	p->TqAngUI.X_Array = VoltArray;
	p->TqAngUI.Y_Value = (Int *)&g_Ram.ramGroupA.AngleUI;
	p->TqAngUI.Y_Array = AnUIArray;

	// Забираем средний ток
	p->Imidpr 	    = &g_Ram.ramGroupH.Imidpr;
	p->TorqueSetPr 	= &g_Core.MotorControl.TorqueSetPr;
	p->TransCurr 	= &g_Ram.ramGroupH.TransCurr;

	// Инициализируем поверхности
	CubInit(&p->Cub1, &p->TqCurr);		 // инициализация структуры куба для расчета момента
	CubInit(&p->Cub2, &p->TqAngUI);

	// Инициализируем фильтр момента
	peref_ApFilter3Init(&p->Trqfltr, (Uns)Prd18kHZ, 1);
}

// Расчет момента
void Core_TorqueCalc(TTorqObs *p)
{
		register TCubStr *Cub;	//
		//register Int Tmp;
		register Int Add;		// ручной контроль индикации момента
		//p->ObsEnable=true;
		if (!p->ObsEnable) {p->Indication = 0; return;} //если выключен расчет момента то индикация 0 и закрываем тиристоры и выходим

		Cub = (*p->Imidpr >= *p->TransCurr) ? &p->Cub1 : &p->Cub2;  // выбераем по какому кубу работаем для маленьких или больших токов

		CubCalc(Cub);	// считаем выбранный куб

		if (g_Core.MotorControl.RequestDir == -1)
		{
		    if      (*p->TorqueSetPr < 30) 	Add = g_Ram.ramGroupC.CorrClose30Trq;		// Добавил PIA 09.10.2012
		    else if (*p->TorqueSetPr < 40)	Add = g_Ram.ramGroupC.CorrClose40Trq;
		    else if (*p->TorqueSetPr < 60)	Add = g_Ram.ramGroupC.CorrClose60Trq;		// корректировка индикации момента для +- изменения времени перехода на поверхность упора
		    else if (*p->TorqueSetPr < 80)	Add = g_Ram.ramGroupC.CorrClose80Trq;
		    else if (*p->TorqueSetPr < 110)	Add = g_Ram.ramGroupC.CorrClose110Trq;
		}
		else if (g_Core.MotorControl.RequestDir == 1)
		{
		    if 	    (*p->TorqueSetPr < 30) 	Add = g_Ram.ramGroupC.CorrOpen30Trq;		// Добавил PIA 09.10.2012
		    else if (*p->TorqueSetPr < 40)	Add = g_Ram.ramGroupC.CorrOpen40Trq;
		    else if (*p->TorqueSetPr < 60)	Add = g_Ram.ramGroupC.CorrOpen60Trq;		// корректировка индикации момента для +- изменения времени перехода на поверхность упора
		    else if (*p->TorqueSetPr < 80)	Add = g_Ram.ramGroupC.CorrOpen80Trq;
		    else if (*p->TorqueSetPr < 110)	Add = g_Ram.ramGroupC.CorrOpen110Trq;
		}
		else if (g_Core.MotorControl.RequestDir == 0) Add = 0;


		p->Trqfltr.Input = (Float)Cub->Output; // фильтруем значение момента
		peref_ApFilter3Calc(&p->Trqfltr);

		p->Tmp =((Uns)(p->Trqfltr.Output));

		if (p->Tmp < TORQ_MIN_PR) p->Tmp = TORQ_MIN_PR;	// проверяем на вхождение в зону от
		if (p->Tmp > TORQ_MAX_PR) p->Tmp = TORQ_MAX_PR;   // 10 до 110 %



		if (abs(Add) > p->Tmp) Add = 0;

		//PU0ToValue(Tmp, p->TorqueMax);// переводим проценты в Нм относительно максимального М
		p->Indication = ((Uns)((((LgUns)p->Tmp + Add) * p->TorqueMax) / 100));
}

void Core_VoltageDown(void)
{
	Float Ro 		= 53.1; 	//   /10
	Float Xl 		= 0.08;		//   /100
	Float Cur 		= g_Ram.ramGroupH.Imid / 10;
	Float CosFi 	= cos(((double)g_Ram.ramGroupA.AngleUI*3.14)/180);
	Float SinFi 	= sin(((double)g_Ram.ramGroupA.AngleUI*3.14)/180);
	Float Length 	= g_Ram.ramGroupB.ConductorLength;
	Float Section 	= g_Ram.ramGroupB.ConductorSection;
	Float Volt 		= 0;
	Float Ra 		= 0;

	Length = Length/1000;
	Section = Section/1000;
	Ra =  1/(Ro * Section);
	Volt = 1.732 * Cur * Length * ((Ra * CosFi) + (Xl* SinFi));
	//Volt = ((Ra * CosFi) + (Xl* SinFi))*100;
	//Volt = CosFi*100;
	g_Ram.ramGroupA.VoltageDown = Volt;

}

