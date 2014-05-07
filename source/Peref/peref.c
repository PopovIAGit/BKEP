/*======================================================================
Имя файла:          peref.c
Автор:				Попов И.А.
Версия файла:		1.0
Дата изменения:		07.04.2014
Описание:
Модуль обработки переферии
======================================================================*/

#include "peref.h"

// Линейная формула преобразования кодов с АЦП по датчику температуры
// в соответствующее сопротивление R(x) = b*x + c, где x - то, что пришло с АЦП,
// R - сопротивление, b и c - коэффициенты, которые задаются ниже:
#define koef_B		_IQ15(-9.6774)
#define koef_C		_IQ15(6600.0)
#define ADC_to_R(inpADC)	_IQ15int(_IQ15mpy(koef_B, _IQ15(inpADC) ) + koef_C) 	// Для случая 3 датчика

#define koef_B1		_IQ15(-10.204)
#define koef_C1		_IQ15(6928.57)
#define ADC_to_R1(inpADC1)	_IQ15int(_IQ15mpy(koef_B1, _IQ15(inpADC1) ) + koef_C1)	// Для случая 2 датчика

TPeref	g_Peref;

//---------------------------------------------------
void Peref_Init(TPeref *p) // ??? инит фильтров унести в переодическое обновление
{
  /*peref_ApFilter3Init(&p->URfltr, (LgUns)Prd18kHZ, g_Ram.ramGroupC.CoefVoltFltr);		// Инициализируем фильтры
	peref_ApFilter3Init(&p->USfltr, (LgUns)Prd18kHZ, g_Ram.ramGroupC.CoefVoltFltr);
	peref_ApFilter3Init(&p->UTfltr, (LgUns)Prd18kHZ, g_Ram.ramGroupC.CoefVoltFltr);
	peref_ApFilter3Init(&p->IUfltr, (LgUns)Prd18kHZ, g_Ram.ramGroupC.CoefCurrFltr);
	peref_ApFilter3Init(&p->IVfltr, (LgUns)Prd18kHZ, g_Ram.ramGroupC.CoefCurrFltr);
	peref_ApFilter3Init(&p->IWfltr, (LgUns)Prd18kHZ, g_Ram.ramGroupC.CoefCurrFltr);*/

	peref_ApFilter3Init(&p->URfltr, (Uns)Prd18kHZ, 0.02);		// Инициализируем фильтры
	peref_ApFilter3Init(&p->USfltr, (Uns)Prd18kHZ, 0.02);
	peref_ApFilter3Init(&p->UTfltr, (Uns)Prd18kHZ, 0.02);
	peref_ApFilter3Init(&p->IUfltr, (Uns)Prd18kHZ, 0.02);
	peref_ApFilter3Init(&p->IVfltr, (Uns)Prd18kHZ, 0.02);
	peref_ApFilter3Init(&p->IWfltr, (Uns)Prd18kHZ, 0.02);

	Peref_SensObserverInit(&p->sensObserver);// Инициализируем обработку синусойды

	memset(&p->sinObserver, 0, sizeof(TSinObserver));

	Peref_SinObserverInit(&p->sinObserver.UR,Prd18kHZ);
	Peref_SinObserverInit(&p->sinObserver.US,Prd18kHZ);
	Peref_SinObserverInit(&p->sinObserver.UT,Prd18kHZ);
	Peref_SinObserverInit(&p->sinObserver.IU,Prd18kHZ);
	Peref_SinObserverInit(&p->sinObserver.IV,Prd18kHZ);
	Peref_SinObserverInit(&p->sinObserver.IW,Prd18kHZ);

	p->phaseOrder.UR = &p->sinObserver.UR;	 // Привязываем
	p->phaseOrder.US = &p->sinObserver.US;
	p->phaseOrder.UT = &p->sinObserver.UT;

	p->phaseOrder.Timeout = 5;
	p->Umid = 0;
	p->Imid = 0;
	p->AngleUI = 0;

	peref_ApFilter1Init(&p->Phifltr, Prd200HZ, 0.05);
	peref_ApFilter1Init(&p->Umfltr, Prd200HZ, 0.05);
	peref_ApFilter3Init(&p->Imfltr, Prd200HZ, 0.05);

	Peref_CalibInit(&p->Position);
	ContactorInit(&p->ContactorControl);
	// ----Сигнализация лампочками-----------------------
	Peref_LedsInit(&p->leds, Prd10HZ);
}
//---------------------------------------------------
void Peref_18kHzCalc(TPeref *p) // 18 кГц
{
	//-------------------- Фильтруем АЦП-------------------------------

/*	p->URfltr.Input = ADC_UR;
	p->USfltr.Input = ADC_US;
	p->UTfltr.Input = ADC_UT;
	p->IUfltr.Input = ADC_IU;
	p->IVfltr.Input = ADC_IV;
	p->IWfltr.Input = ADC_IW;


	peref_ApFilter3Calc(&p->URfltr);
	peref_ApFilter3Calc(&p->USfltr);
	peref_ApFilter3Calc(&p->UTfltr);
	peref_ApFilter3Calc(&p->IUfltr);
	peref_ApFilter3Calc(&p->IVfltr);
	peref_ApFilter3Calc(&p->IWfltr);*/

	//-------------- Обработка синусойды ------------------------------

/*	p->sensObserver.URinp = p->URfltr.Output;
	p->sensObserver.USinp = p->USfltr.Output;
	p->sensObserver.UTinp = p->UTfltr.Output;
	p->sensObserver.IUinp = p->IUfltr.Output;
	p->sensObserver.IVinp = p->IVfltr.Output;
	p->sensObserver.IWinp = p->IWfltr.Output;*/

		p->sensObserver.URinp = ADC_UR;
		p->sensObserver.USinp = ADC_US;
		p->sensObserver.UTinp = ADC_UT;
		p->sensObserver.IUinp = ADC_IU;
		p->sensObserver.IVinp = ADC_IV;
		p->sensObserver.IWinp = ADC_IW;



	Peref_SensObserverUpdate(&p->sensObserver);

	//--------------- RMS угол полярность -----------------------------

	p->sinObserver.UR.Input = p->sensObserver.URout;
	p->sinObserver.US.Input = p->sensObserver.USout;
	p->sinObserver.UT.Input = p->sensObserver.UTout;
	p->sinObserver.IU.Input = p->sensObserver.IUout;
	p->sinObserver.IV.Input = p->sensObserver.IVout;
	p->sinObserver.IW.Input = p->sensObserver.IWout;

	Peref_SinObserverUpdate(&p->sinObserver.UR);
	Peref_SinObserverUpdate(&p->sinObserver.US);
	Peref_SinObserverUpdate(&p->sinObserver.UT);
	Peref_SinObserverUpdate(&p->sinObserver.IU);
	Peref_SinObserverUpdate(&p->sinObserver.IV);
	Peref_SinObserverUpdate(&p->sinObserver.IW);

	Peref_PhaseOrderUpdate(&p->phaseOrder);

	if (!p->sinObserver.IV.CurAngle)	p->Phifltr.Input = p->sinObserver.US.CurAngle;
}

//---------------------------------------------------
void Peref_50HzCalc(TPeref *p)	// 50 Гц
{
	peref_ApFilter1Calc(&p->Phifltr);
	if (!g_Core.Status.bit.Stop)
	p->AngleUI = _IQtoIQ16(p->Phifltr.Output);

	p->Umfltr.Input = _IQ16toIQ(Mid3UnsValue(p->sinObserver.UR.Output, p->sinObserver.US.Output, p->sinObserver.UT.Output));
	peref_ApFilter1Calc(&p->Umfltr);
	p->Umid = _IQtoIQ16(p->Umfltr.Output);

	p->Imfltr.Input = _IQ16toIQ(Mid3UnsValue(p->sinObserver.IU.Output, p->sinObserver.IV.Output, p->sinObserver.IW.Output));
	peref_ApFilter3Calc(&p->Imfltr);
	p->Imid = _IQtoIQ16(p->Imfltr.Output);

}

void Peref_10HzCalc(TPeref *p)	// 10 Гц
{

}

//---------------------------------------------------
