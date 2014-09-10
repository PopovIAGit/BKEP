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

Bool RtcStart    = False;

//---------------------------------------------------
void Peref_Init(TPeref *p) // ??? инит фильтров унести в переодическое обновление
{
	//----для  Телеуправления---------------------------------------------------------------------------
	memset(&p->InDigSignal, 0, sizeof(TSinSignalObserver));


	Peref_SensTuObserverInit(&p->InDigSignalObserver);// Инициализируем обработку синусойды

	Peref_SinObserverInitFloat(&p->InDigSignal.sigOpen,		Prd18kHZ);
	Peref_SinObserverInitFloat(&p->InDigSignal.sigClose,	Prd18kHZ);
	Peref_SinObserverInitFloat(&p->InDigSignal.sigStop,		Prd18kHZ);
	Peref_SinObserverInitFloat(&p->InDigSignal.sigMU,		Prd18kHZ);
	Peref_SinObserverInitFloat(&p->InDigSignal.sigResetAlarm,Prd18kHZ);
	Peref_SinObserverInitFloat(&p->InDigSignal.sigReadyTU,	Prd18kHZ);
	Peref_SinObserverInitFloat(&p->InDigSignal.sigDU,		Prd18kHZ);

	//--------токи и напряжения------------------------------------------------------------------------

	Peref_SensObserverInit(&p->sensObserver);// Инициализируем обработку синусойды

	memset(&p->sinObserver, 0, sizeof(TSinObserver));

	Peref_SinObserverInitFloat(&p->sinObserver.UR,Prd18kHZ);
	Peref_SinObserverInitFloat(&p->sinObserver.US,Prd18kHZ);
	Peref_SinObserverInitFloat(&p->sinObserver.UT,Prd18kHZ);
	Peref_SinObserverInitFloat(&p->sinObserver.IU,Prd18kHZ);
	Peref_SinObserverInitFloat(&p->sinObserver.IV,Prd18kHZ);
	Peref_SinObserverInitFloat(&p->sinObserver.IW,Prd18kHZ);

	p->phaseOrder.UR = &p->sinObserver.UR;	 // Привязываем
	p->phaseOrder.US = &p->sinObserver.US;
	p->phaseOrder.UT = &p->sinObserver.UT;

	p->phaseOrder.Timeout = 5;
	p->Umid = 0;
	p->Imid = 0;
	p->AngleUI = 0;

	Peref_CalibInit(&p->Position);
	ContactorInit(&p->ContactorControl);
	// ----Сигнализация лампочками-----------------------
	Peref_LedsInit(&p->leds, Prd10HZ);
	ADT75_Init(&p->TSens);
	MCP4276_Init(&p->Dac);
	DS3231_Init(&p->Rtc);
	p->Rtc.Data =(char *) &p->RtcData;

}
//---------------------------------------------------
void Peref_18kHzCalc(TPeref *p) // 18 кГц
{
	//-------------------- Фильтруем АЦП-------------------------------

	// TU
	// забираем отмасштабированный сигнал с АЦП на вход фильтра 1-ого порядка
	p->UfltrOpen.Input	 	 = p->InDigSignalObserver.UOpenOut;
	p->UfltrClose.Input		 = p->InDigSignalObserver.UCloseOut;
	p->UfltrStop.Input		 = p->InDigSignalObserver.UStopOut;
	p->UfltrMu.Input		 = p->InDigSignalObserver.UMuOut;
	p->UfltrResetAlarm.Input = p->InDigSignalObserver.UResetAlarmOut;
	p->UfltrReadyTU.Input	 = p->InDigSignalObserver.UReadyTuOut;
	p->UfltrDU.Input		 = p->InDigSignalObserver.UDuOut;

	//фильтруем входной сигнал ТУ
	peref_ApFilter1Calc(&p->UfltrOpen);
	peref_ApFilter1Calc(&p->UfltrClose);
	peref_ApFilter1Calc(&p->UfltrStop);
	peref_ApFilter1Calc(&p->UfltrMu);
	peref_ApFilter1Calc(&p->UfltrResetAlarm);
	peref_ApFilter1Calc(&p->UfltrReadyTU);
	peref_ApFilter1Calc(&p->UfltrDU);

	//передаём фильтрованный сигнал на обработку RMS
	p->InDigSignal.sigOpen.Input 		= p->UfltrOpen.Output;
	p->InDigSignal.sigClose.Input 		= p->UfltrClose.Output;
	p->InDigSignal.sigStop.Input 		= p->UfltrStop.Output;
	p->InDigSignal.sigMU.Input 			= p->UfltrMu.Output;
	p->InDigSignal.sigResetAlarm.Input 	= p->UfltrResetAlarm.Output;
	p->InDigSignal.sigReadyTU.Input 	= p->UfltrReadyTU.Output;
	p->InDigSignal.sigDU.Input 			= p->UfltrDU.Output;

	//функция расчёта RMS для сигналов ТУ
	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigOpen);
	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigClose);
	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigStop);
	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigMU);
	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigResetAlarm);
	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigReadyTU);
	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigDU);
	///////////////////////////////////////////////////////////////////////////////////

	//RST UVW
	//----------------------------------------------------------------------------
	//---отмасштабированные данные с ацп на вход фильтра 1-ого порядка
	p->URfltr.Input = p->sensObserver.URout;
	p->USfltr.Input = p->sensObserver.USout;
	p->UTfltr.Input = p->sensObserver.UTout;
	p->IUfltr.Input = p->sensObserver.IUout;
	p->IVfltr.Input = p->sensObserver.IVout;
	p->IWfltr.Input = p->sensObserver.IWout;

	peref_ApFilter1Calc(&p->URfltr);
	peref_ApFilter1Calc(&p->USfltr);
	peref_ApFilter1Calc(&p->UTfltr);
	peref_ApFilter1Calc(&p->IUfltr);
	peref_ApFilter1Calc(&p->IVfltr);
	peref_ApFilter1Calc(&p->IWfltr);

	//--------------- RMS угол полярность -----------------------------

	p->sinObserver.UR.Input = p->URfltr.Output;
	p->sinObserver.US.Input = p->USfltr.Output;
	p->sinObserver.UT.Input = p->UTfltr.Output;
	p->sinObserver.IU.Input = p->IUfltr.Output;
	p->sinObserver.IV.Input = p->IVfltr.Output;
	p->sinObserver.IW.Input = p->IWfltr.Output;

	Peref_SinObserverUpdateFloat(&p->sinObserver.UR);
	Peref_SinObserverUpdateFloat(&p->sinObserver.US);
	Peref_SinObserverUpdateFloat(&p->sinObserver.UT);
	Peref_SinObserverUpdateFloat(&p->sinObserver.IU);
	Peref_SinObserverUpdateFloat(&p->sinObserver.IV);
	Peref_SinObserverUpdateFloat(&p->sinObserver.IW);

	Peref_PhaseOrderUpdate(&p->phaseOrder);

	if (!p->sinObserver.IV.CurAngle)	p->Phifltr.Input = p->sinObserver.US.CurAngle;

	//-------------------------------------------------------------
}

//---------------------------------------------------
void Peref_50HzCalc(TPeref *p)	// 50 Гц
{
	//фильтруем TU RST UVW фильтром 3-его порядка

	p->UR3fltr.Input = p->sinObserver.UR.Output;
	p->US3fltr.Input = p->sinObserver.US.Output;
	p->UT3fltr.Input = p->sinObserver.UT.Output;

	p->IU3fltr.Input = p->sinObserver.IU.Output;
	p->IV3fltr.Input = p->sinObserver.IV.Output;
	p->IW3fltr.Input = p->sinObserver.IW.Output;

	peref_ApFilter3Calc(&p->UR3fltr);
	peref_ApFilter3Calc(&p->US3fltr);
	peref_ApFilter3Calc(&p->UT3fltr);
	peref_ApFilter3Calc(&p->IU3fltr);
	peref_ApFilter3Calc(&p->IV3fltr);
	peref_ApFilter3Calc(&p->IW3fltr);

	p->U3fltrOpen.Input 		= p->InDigSignal.sigOpen.Output;
	p->U3fltrClose.Input 		= p->InDigSignal.sigClose.Output;
	p->U3fltrStop.Input 		= p->InDigSignal.sigStop.Output;
	p->U3fltrMu.Input 			= p->InDigSignal.sigMU.Output;
	p->U3fltrResetAlarm.Input 	= p->InDigSignal.sigResetAlarm.Output;;
	p->U3fltrReadyTU.Input 		= p->InDigSignal.sigReadyTU.Output;
	p->U3fltrDU.Input 			= p->InDigSignal.sigDU.Output;

	peref_ApFilter3Calc(&p->U3fltrOpen);
	peref_ApFilter3Calc(&p->U3fltrClose);
	peref_ApFilter3Calc(&p->U3fltrStop);
	peref_ApFilter3Calc(&p->U3fltrMu);
	peref_ApFilter3Calc(&p->U3fltrDU);
	peref_ApFilter3Calc(&p->U3fltrReadyTU);
	peref_ApFilter3Calc(&p->U3fltrResetAlarm);

	g_Comm.digitInterface.dinOpen.inputDIN 			= p->U3fltrOpen.Output;
	g_Comm.digitInterface.dinClose.inputDIN 		= p->U3fltrClose.Output;
	g_Comm.digitInterface.dinStop.inputDIN 			= p->U3fltrStop.Output;
	g_Comm.digitInterface.dinMu.inputDIN 			= p->U3fltrMu.Output;
	g_Comm.digitInterface.dinDu.inputDIN 			= p->U3fltrDU.Output;
	g_Comm.digitInterface.dinPredReady.inputDIN 	= p->U3fltrReadyTU.Output;
	g_Comm.digitInterface.dinResetAlarm.inputDIN 	= p->U3fltrResetAlarm.Output;

	peref_ApFilter1Calc(&p->Phifltr);
	if (!g_Core.Status.bit.Stop)
	p->AngleUI = _IQtoIQ16(p->Phifltr.Output);

	p->Umfltr.Input = Mid3ValueUns(p->sinObserver.UR.Output, p->sinObserver.US.Output, p->sinObserver.UT.Output);
	peref_ApFilter1Calc(&p->Umfltr);
	p->Umid = _IQtoIQ16(p->Umfltr.Output);

	p->Imfltr.Input = Mid3ValueUns(p->sinObserver.IU.Output, p->sinObserver.IV.Output, p->sinObserver.IW.Output);
	peref_ApFilter3Calc(&p->Imfltr);
	p->Imid = _IQtoIQ16(p->Imfltr.Output);

	I2CDevUpdate(p);
}

void Peref_10HzCalc(TPeref *p)	// 10 Гц
{
	int16 PosPr = 0;
//-------- логика ЦАП -------------------------------------------------
	if (g_Ram.ramGroupG.Mode)	p->Dac.Data = g_Ram.ramGroupG.DacValue;
	else if(g_Ram.ramGroupH.CalibState != csCalib) p->Dac.Data = 0;
	else
	{
		PosPr = g_Ram.ramGroupA.PositionPr;
		if(PosPr < 0) PosPr = 0;
		if(PosPr > 1000) PosPr = 1000;
		p->Dac.Data = g_Ram.ramGroupC.Dac_Offset + (Uint16)(0.001 * (g_Ram.ramGroupC.Dac_Mpy - g_Ram.ramGroupC.Dac_Offset) * PosPr);
		if (PosPr==9999) p->Dac.Data = g_Ram.ramGroupC.Dac_Offset/4;//???
	}
	//---------------------------------------------------------------------------
}
//-----------------Обработка данных с микросхем по И2С---------------------------
void I2CDevUpdate(TPeref *p)
{
	static Uint16 Step = 0;

	I2C_update(&I2cMsg);

	switch(Step)
	{
		case 0: ADT75_Update(&p->TSens); if (!p->TSens.Busy) Step = 1; break;
		case 1: MCP4726_Update(&p->Dac); if (!p->Dac.Busy)   Step = 2; break;
		case 2: DS3231_Update(&p->Rtc);  if (!p->Rtc.Busy)   Step = 0; break;
	}

}

//---------------------------------------------------

void RTC_Control(void)
{
		static Uns PrevTime;
		static Uns PrevDate;
		TTimeVar *DevTime = &g_Ram.ramGroupB.DevTime;
		TDateVar *DevDate = &g_Ram.ramGroupB.DevDate;

		if(g_Peref.Rtc.Busy) return;

		if (RtcStart)
		{
			if (DevTime->all != PrevTime)
			{
				RTC_SetTime(&g_Peref.RtcData, DevTime, 0);
				g_Peref.Rtc.Flag = True;
			}
			if (DevDate->all != PrevDate)
			{
				RTC_SetDate(&g_Peref.RtcData, DevDate, 1);
				g_Peref.Rtc.Flag = True;
			}
			if (RTC_TimeCorr(&g_Peref.RtcData, g_Ram.ramGroupB.TimeCorrection))
			{
				g_Peref.Rtc.Flag = True;
			}
		}

		if (!g_Peref.Rtc.Flag)
		{
			RTC_GetTime(&g_Peref.RtcData, DevTime);
			RTC_GetDate(&g_Peref.RtcData, DevDate);
			g_Ram.ramGroupH.Seconds =(Uns) g_Peref.RtcData.Sec;
		}

		PrevTime = DevTime->all;
		PrevDate = DevDate->all;
		RtcStart = True;
}

//----------------------------------------------
