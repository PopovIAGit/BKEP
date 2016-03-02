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
Uns LocalPoss = 0;


__inline void peref_74HC595CsSet(Byte Lev) {CS_RELE = !Lev;}

Bool RtcStart    = False;

//---------------------------------------------------
void Peref_Init(TPeref *p) // ??? инит фильтров унести в переодическое обновление
{
    //----для  Телеуправления---------------------------------------------------------------------------
    memset(&p->InDigSignal, 0, sizeof(TSinSignalObserver));

    Peref_SensTuObserverInit(&p->InDigSignalObserver); // Инициализируем обработку синусойды

    Peref_SinObserverInitFloat(&p->InDigSignal.sigOpen, Prd18kHZ);
    Peref_SinObserverInitFloat(&p->InDigSignal.sigClose, Prd18kHZ);
    Peref_SinObserverInitFloat(&p->InDigSignal.sigStopOpen, Prd18kHZ);
    Peref_SinObserverInitFloat(&p->InDigSignal.sigMU, Prd18kHZ);
    Peref_SinObserverInitFloat(&p->InDigSignal.sigResetAlarm, Prd18kHZ);
    Peref_SinObserverInitFloat(&p->InDigSignal.sigStopClose, Prd18kHZ);
    Peref_SinObserverInitFloat(&p->InDigSignal.sigDU, Prd18kHZ);

    //--------токи и напряжения------------------------------------------------------------------------

    Peref_SensObserverInit(&p->sensObserver); // Инициализируем обработку синусойды

    memset(&p->sinObserver, 0, sizeof(TSinObserver));

    Peref_SinObserverInitFloat(&p->sinObserver.UR, Prd18kHZ);
    Peref_SinObserverInitFloat(&p->sinObserver.US, Prd18kHZ);
    Peref_SinObserverInitFloat(&p->sinObserver.UT, Prd18kHZ);
    Peref_SinObserverInitFloat(&p->sinObserver.IU, Prd18kHZ);
    Peref_SinObserverInitFloat(&p->sinObserver.IV, Prd18kHZ);
    Peref_SinObserverInitFloat(&p->sinObserver.IW, Prd18kHZ);

    p->phaseOrder.UR = &p->sinObserver.UR;	 // Привязываем
    p->phaseOrder.US = &p->sinObserver.US;
    p->phaseOrder.UT = &p->sinObserver.UT;

    p->phaseOrder.Timeout = 5;
    p->Umid = 0;
    p->Imid = 0;
    p->AngleUI = 0;
    p->TU_Offset_Calib_timer = 0;
    Peref_CalibInit(&p->Position);
    ContactorInit(&p->ContactorControl);
    // ----Сигнализация лампочками-----------------------
    Peref_LedsInit(&p->leds, Prd10HZ);
    ADT75_Init(&p->TSens);
    MCP4276_Init(&p->Dac);
    DS3231_Init(&p->Rtc);
    DisplDrvInit(&p->Display);
    p->Rtc.Data = (char *) &p->RtcData;
    //-------Драйвер сдвигового регистра для ТС------
    p->ShiftReg.CsFunc = &peref_74HC595CsSet;
    Peref_74HC595Init(&p->ShiftReg);

}
//---------------------------------------------------
void Peref_18kHzCalc(TPeref *p) // 18 кГц
{
    //-------------------- Фильтруем АЦП-------------------------------

    // TU
    // забираем отмасштабированный сигнал с АЦП на вход фильтра 1-ого порядка
    switch(p->NumCalcTU_18kHz)
    {
    case 0:
    	p->UfltrOpen.Input 		= p->InDigSignalObserver.UOpenOut;
    	peref_ApFilter1Calc(&p->UfltrOpen);
    	p->InDigSignal.sigOpen.Input = p->UfltrOpen.Output;
    	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigOpen);
    	p->NumCalcTU_18kHz=1;
    	break;
    case 1:
    	p->UfltrClose.Input 	= p->InDigSignalObserver.UCloseOut;
    	peref_ApFilter1Calc(&p->UfltrClose);
    	p->InDigSignal.sigClose.Input = p->UfltrClose.Output;
    	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigClose);
    	p->NumCalcTU_18kHz=2;
    	break;
    case 2:
    	p->UfltrStopOpen.Input 	= p->InDigSignalObserver.UStopOpenOut;
    	peref_ApFilter1Calc(&p->UfltrStopOpen);
    	p->InDigSignal.sigStopOpen.Input = p->UfltrStopOpen.Output;
    	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigStopOpen);
    	p->NumCalcTU_18kHz=3;
    	break;
    case 3:
    	p->UfltrMu.Input 		= p->InDigSignalObserver.UMuOut;
        peref_ApFilter1Calc(&p->UfltrMu);
        p->InDigSignal.sigMU.Input = p->UfltrMu.Output;
        Peref_SinObserverUpdateFloat(&p->InDigSignal.sigMU);
		p->NumCalcTU_18kHz=4;
		break;
    case 4:
    	p->UfltrResetAlarm.Input= p->InDigSignalObserver.UResetAlarmOut;
    	peref_ApFilter1Calc(&p->UfltrResetAlarm);
    	p->InDigSignal.sigResetAlarm.Input = p->UfltrResetAlarm.Output;
    	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigResetAlarm);
		p->NumCalcTU_18kHz=5;
		break;
    case 5:
    	p->UfltrStopClose.Input = p->InDigSignalObserver.UStopCloseOut;
    	peref_ApFilter1Calc(&p->UfltrStopClose);
    	p->InDigSignal.sigStopClose.Input = p->UfltrStopClose.Output;
    	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigStopClose);
		p->NumCalcTU_18kHz=6;
		break;
    case 6:
    	p->UfltrDU.Input 		= p->InDigSignalObserver.UDuOut;
    	peref_ApFilter1Calc(&p->UfltrDU);
    	p->InDigSignal.sigDU.Input = p->UfltrDU.Output;
    	Peref_SinObserverUpdateFloat(&p->InDigSignal.sigDU);
		p->NumCalcTU_18kHz=0;
		break;

    	default: p->NumCalcTU_18kHz=0; break;
    }

	/*
	p->UfltrOpen.Input 		= p->InDigSignalObserver.UOpenOut;
    p->UfltrClose.Input 	= p->InDigSignalObserver.UCloseOut;
    p->UfltrStopOpen.Input 	= p->InDigSignalObserver.UStopOpenOut;
    p->UfltrMu.Input 		= p->InDigSignalObserver.UMuOut;
    p->UfltrResetAlarm.Input= p->InDigSignalObserver.UResetAlarmOut;
    p->UfltrStopClose.Input = p->InDigSignalObserver.UStopCloseOut;
    p->UfltrDU.Input 		= p->InDigSignalObserver.UDuOut;

    //фильтруем входной сигнал ТУ
    peref_ApFilter1Calc(&p->UfltrOpen);
    peref_ApFilter1Calc(&p->UfltrClose);
    peref_ApFilter1Calc(&p->UfltrStopOpen);
    peref_ApFilter1Calc(&p->UfltrMu);
    peref_ApFilter1Calc(&p->UfltrResetAlarm);
    peref_ApFilter1Calc(&p->UfltrStopClose);
    peref_ApFilter1Calc(&p->UfltrDU);

    //передаём фильтрованный сигнал на обработку RMS
    p->InDigSignal.sigOpen.Input = p->UfltrOpen.Output;
    p->InDigSignal.sigClose.Input = p->UfltrClose.Output;
    p->InDigSignal.sigStopOpen.Input = p->UfltrStopOpen.Output;
    p->InDigSignal.sigMU.Input = p->UfltrMu.Output;
    p->InDigSignal.sigResetAlarm.Input = p->UfltrResetAlarm.Output;
    p->InDigSignal.sigStopClose.Input = p->UfltrStopClose.Output;
    p->InDigSignal.sigDU.Input = p->UfltrDU.Output;

    //функция расчёта RMS для сигналов ТУ
    Peref_SinObserverUpdateFloat(&p->InDigSignal.sigOpen);
    Peref_SinObserverUpdateFloat(&p->InDigSignal.sigClose);
    Peref_SinObserverUpdateFloat(&p->InDigSignal.sigStopOpen);
    Peref_SinObserverUpdateFloat(&p->InDigSignal.sigMU);
    Peref_SinObserverUpdateFloat(&p->InDigSignal.sigResetAlarm);
    Peref_SinObserverUpdateFloat(&p->InDigSignal.sigStopClose);
    Peref_SinObserverUpdateFloat(&p->InDigSignal.sigDU);
    ///////////////////////////////////////////////////////////////////////////////////
*/
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




    if (!p->sinObserver.IV.CurAngle)
	{
	    p->Phifltr.Input = p->sinObserver.US.CurAngle;
	}

    //-------------------------------------------------------------
}

//---------------------------------------------------
void Peref_50HzCalc(TPeref *p)	// 50 Гц
{

	Uns Tu_Offset_adr = 0;
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

    // корректировка оффсетов дл ТУ 220/24

    if (p->TU_Offset_Calib_timer <= TU_OFFSET_CALIB_TIME && g_Ram.ramGroupC.TuOffsetCalib == 1)
	{
    	p->TU_Offset_Calib_timer++;

		if (!InRange(g_Peref.InDigSignal.sigOpen.Output, -0.1, 0.1))
		{
			if (g_Peref.InDigSignal.sigOpen.Input > 0)				g_Ram.ramGroupC.p_UOpen_Offset++;
			if (g_Peref.InDigSignal.sigOpen.Input < 0)				g_Ram.ramGroupC.p_UOpen_Offset--;
		}
		if (!InRange(g_Peref.InDigSignal.sigClose.Output, -0.1, 0.1))
		{
			if (g_Peref.InDigSignal.sigClose.Input > 0)				g_Ram.ramGroupC.p_UClose_Offset++;
			if (g_Peref.InDigSignal.sigClose.Input < 0)				g_Ram.ramGroupC.p_UClose_Offset--;
		}
		if (!InRange(g_Peref.InDigSignal.sigStopOpen.Output, -0.1, 0.1))
		{
			if (g_Peref.InDigSignal.sigStopOpen.Input > 0)			g_Ram.ramGroupC.p_UStopOpen_Offset++;
			if (g_Peref.InDigSignal.sigStopOpen.Input < 0)			g_Ram.ramGroupC.p_UStopOpen_Offset--;
		}
		if (!InRange(g_Peref.InDigSignal.sigStopClose.Output, -0.1, 0.1))
		{
			if (g_Peref.InDigSignal.sigStopClose.Input > 0)			g_Ram.ramGroupC.p_UStopClose_Offset++;
			if (g_Peref.InDigSignal.sigStopClose.Input < 0)			g_Ram.ramGroupC.p_UStopClose_Offset--;
		}
		if (!InRange(g_Peref.InDigSignal.sigMU.Output, -0.1, 0.1))
		{
			if (g_Peref.InDigSignal.sigMU.Input > 0)				g_Ram.ramGroupC.p_UMu_Offset++;
			if (g_Peref.InDigSignal.sigMU.Input < 0)				g_Ram.ramGroupC.p_UMu_Offset--;
		}
		if (!InRange(g_Peref.InDigSignal.sigDU.Output, -0.1, 0.1))
		{
			if (g_Peref.InDigSignal.sigDU.Input > 0)				g_Ram.ramGroupC.p_UDu_Offset++;
			if (g_Peref.InDigSignal.sigDU.Input < 0)				g_Ram.ramGroupC.p_UDu_Offset--;
		}
		if (!InRange(g_Peref.InDigSignal.sigResetAlarm.Output, -0.1, 0.1))
		{
			if (g_Peref.InDigSignal.sigResetAlarm.Input > 0)		g_Ram.ramGroupC.p_UResetAlarm_Offset++;
			if (g_Peref.InDigSignal.sigResetAlarm.Input < 0)		g_Ram.ramGroupC.p_UResetAlarm_Offset--;
		}
	}
    else if(p->TU_Offset_Calib_timer >= TU_OFFSET_CALIB_TIME && g_Ram.ramGroupC.TuOffsetCalib == 1)
    {
    	p->TU_Offset_Calib_timer = 0;
    	g_Ram.ramGroupC.TuOffsetCalib = 0;
    	Tu_Offset_adr = GetAdr(ramGroupC.p_UOpen_Mpy220);
    	if(IsMemParReady())
    	{
    		WriteToEeprom(Tu_Offset_adr, &g_Ram.ramGroupC.p_UOpen_Mpy220, 21);
    	}
    }


    /*switch(p->NumCalcTU_50Hz){
    	case 0:
    		p->U3fltrOpen.Input 		= p->InDigSignal.sigOpen.Output;
    		peref_ApFilter3Calc(&p->U3fltrOpen);
    		g_Comm.digitInterface.dinOpen.inputDIN = p->U3fltrOpen.Output;
    		p->NumCalcTU_50Hz=1;
    		break;
    	case 1:
			p->U3fltrClose.Input 		= p->InDigSignal.sigClose.Output;
			peref_ApFilter3Calc(&p->U3fltrClose);
			g_Comm.digitInterface.dinClose.inputDIN = p->U3fltrClose.Output;
			p->NumCalcTU_50Hz=2;
		break;
    	case 2:
			p->U3fltrStopOpen.Input 	= p->InDigSignal.sigStopOpen.Output;
			peref_ApFilter3Calc(&p->U3fltrStopOpen);
			g_Comm.digitInterface.dinStopOpen.inputDIN = p->U3fltrStopOpen.Output;
			p->NumCalcTU_50Hz=3;
		break;
    	case 3:
			p->U3fltrMu.Input 			= p->InDigSignal.sigMU.Output;
			peref_ApFilter3Calc(&p->U3fltrMu);
			g_Comm.digitInterface.dinMu.inputDIN = p->U3fltrMu.Output;
			p->NumCalcTU_50Hz=4;
		break;
    	case 4:
			p->U3fltrResetAlarm.Input 	= p->InDigSignal.sigResetAlarm.Output;
			peref_ApFilter3Calc(&p->U3fltrDU);
			g_Comm.digitInterface.dinDu.inputDIN = p->U3fltrDU.Output;
			p->NumCalcTU_50Hz=5;
		break;
    	case 5:
			p->U3fltrStopClose.Input 	= p->InDigSignal.sigStopClose.Output;
			peref_ApFilter3Calc(&p->U3fltrStopClose);
			g_Comm.digitInterface.dinStopClose.inputDIN = p->U3fltrStopClose.Output;
			p->NumCalcTU_50Hz=6;
		break;
    	case 6:
			p->U3fltrDU.Input 			= p->InDigSignal.sigDU.Output;
			peref_ApFilter3Calc(&p->U3fltrResetAlarm);
			g_Comm.digitInterface.dinResetAlarm.inputDIN = p->U3fltrResetAlarm.Output;
			p->NumCalcTU_50Hz=7;
		break;
    	case 7: p->NumCalcTU_50Hz=8; break;
    	case 8: p->NumCalcTU_50Hz=9; break;
    	case 9: p->NumCalcTU_50Hz=0; break;

    	default: p->NumCalcTU_50Hz=0; break;

    }*/
        		g_Comm.digitInterface.dinOpen.inputDIN = p->InDigSignal.sigOpen.Output;
    			g_Comm.digitInterface.dinClose.inputDIN = p->InDigSignal.sigClose.Output;
    			g_Comm.digitInterface.dinStopOpen.inputDIN = p->InDigSignal.sigStopOpen.Output;
    			g_Comm.digitInterface.dinMu.inputDIN = p->InDigSignal.sigMU.Output;
    			g_Comm.digitInterface.dinDu.inputDIN = p->InDigSignal.sigDU.Output;
    			g_Comm.digitInterface.dinStopClose.inputDIN = p->InDigSignal.sigStopClose.Output;
    			g_Comm.digitInterface.dinResetAlarm.inputDIN = p->InDigSignal.sigResetAlarm.Output;

    /*p->U3fltrOpen.Input 		= p->InDigSignal.sigOpen.Output;
    p->U3fltrClose.Input 		= p->InDigSignal.sigClose.Output;
    p->U3fltrStopOpen.Input 	= p->InDigSignal.sigStopOpen.Output;
    p->U3fltrMu.Input 			= p->InDigSignal.sigMU.Output;
    p->U3fltrResetAlarm.Input 	= p->InDigSignal.sigResetAlarm.Output;
    p->U3fltrStopClose.Input 	= p->InDigSignal.sigStopClose.Output;
    p->U3fltrDU.Input 			= p->InDigSignal.sigDU.Output;

    peref_ApFilter3Calc(&p->U3fltrOpen);
    peref_ApFilter3Calc(&p->U3fltrClose);
    peref_ApFilter3Calc(&p->U3fltrStopOpen);
    peref_ApFilter3Calc(&p->U3fltrMu);
    peref_ApFilter3Calc(&p->U3fltrDU);
    peref_ApFilter3Calc(&p->U3fltrStopClose);
    peref_ApFilter3Calc(&p->U3fltrResetAlarm);

    g_Comm.digitInterface.dinOpen.inputDIN = p->U3fltrOpen.Output;
    g_Comm.digitInterface.dinClose.inputDIN = p->U3fltrClose.Output;
    g_Comm.digitInterface.dinStopOpen.inputDIN = p->U3fltrStopOpen.Output;
    g_Comm.digitInterface.dinMu.inputDIN = p->U3fltrMu.Output;
    g_Comm.digitInterface.dinDu.inputDIN = p->U3fltrDU.Output;
    g_Comm.digitInterface.dinStopClose.inputDIN = p->U3fltrStopClose.Output;
    g_Comm.digitInterface.dinResetAlarm.inputDIN = p->U3fltrResetAlarm.Output;
	*/
    peref_ApFilter1Calc(&p->Phifltr);
    if (!g_Core.Status.bit.Stop)
	p->AngleUI = _IQtoIQ16(p->Phifltr.Output);

    p->Umfltr.Input = Mid3ValueUns(p->sinObserver.UR.Output, p->sinObserver.US.Output, p->sinObserver.UT.Output);
    peref_ApFilter1Calc(&p->Umfltr);
    p->Umid = _IQtoIQ16(p->Umfltr.Output);

   // p->Imfltr.Input = Mid3ValueUns(p->sinObserver.IU.Output, p->sinObserver.IV.Output, p->sinObserver.IW.Output);
    p->Imfltr.Input = ((p->sinObserver.IV.Output + p->sinObserver.IV.Output + p->sinObserver.IV.Output)/3.0);
    peref_ApFilter3Calc(&p->Imfltr);
    p->Imid = _IQtoIQ16(p->Imfltr.Output);

    I2CDevUpdate(p);
}

void Peref_10HzCalc(TPeref *p)	// 10 Гц
{
  int16 PosPr = 0;

  if (g_Ram.ramGroupB.InputType==it24)
  {
		g_Peref.InDigSignalObserver.parSensors.p_UOpen_Mpy			= &g_Ram.ramGroupC.p_UOpen_Mpy24;
		g_Peref.InDigSignalObserver.parSensors.p_UClose_Mpy			= &g_Ram.ramGroupC.p_UClose_Mpy24;
		g_Peref.InDigSignalObserver.parSensors.p_UStopOpen_Mpy		= &g_Ram.ramGroupC.p_UStopOpen_Mpy24;
		g_Peref.InDigSignalObserver.parSensors.p_UStopClose_Mpy		= &g_Ram.ramGroupC.p_UStopClose_Mpy24;
		g_Peref.InDigSignalObserver.parSensors.p_UMu_Mpy			= &g_Ram.ramGroupC.p_UMu_Mpy24;
		g_Peref.InDigSignalObserver.parSensors.p_UDu_Mpy			= &g_Ram.ramGroupC.p_UDu_Mpy24;
		g_Peref.InDigSignalObserver.parSensors.p_UResetAlarm_Mpy	= &g_Ram.ramGroupC.p_UResetAlarm_Mpy24;

	} else
	{
		g_Peref.InDigSignalObserver.parSensors.p_UOpen_Mpy			= &g_Ram.ramGroupC.p_UOpen_Mpy220;
		g_Peref.InDigSignalObserver.parSensors.p_UClose_Mpy			= &g_Ram.ramGroupC.p_UClose_Mpy220;
		g_Peref.InDigSignalObserver.parSensors.p_UStopOpen_Mpy		= &g_Ram.ramGroupC.p_UStopOpen_Mpy220;
		g_Peref.InDigSignalObserver.parSensors.p_UStopClose_Mpy		= &g_Ram.ramGroupC.p_UStopClose_Mpy220;
		g_Peref.InDigSignalObserver.parSensors.p_UMu_Mpy			= &g_Ram.ramGroupC.p_UMu_Mpy220;
		g_Peref.InDigSignalObserver.parSensors.p_UDu_Mpy			= &g_Ram.ramGroupC.p_UDu_Mpy220;
		g_Peref.InDigSignalObserver.parSensors.p_UResetAlarm_Mpy	= &g_Ram.ramGroupC.p_UResetAlarm_Mpy220;

	}

 // if (p->Peref_StertDelayTimeout--) return; ???
//-------- логика ЦАП -------------------------------------------------
    if (g_Ram.ramGroupG.Mode)
    {
    	p->Dac.Data = g_Ram.ramGroupG.DacValue;
    	//ENABLE_DAC = 0;
    }
    else if (g_Ram.ramGroupH.CalibState != csCalib)
    	{
    	//	ENABLE_DAC = 1;
    		p->Dac.Data = 0;
    	}
    else
	{

    	//ENABLE_DAC = 0;
	    PosPr = g_Ram.ramGroupA.PositionPr;
	    if (PosPr < 0)
		PosPr = 0;
	    if (PosPr > 1000)
		PosPr = 1000;
	    p->Dac.Data = g_Ram.ramGroupC.Dac_Offset + (Uint16) (0.001 * (g_Ram.ramGroupC.Dac_Mpy - g_Ram.ramGroupC.Dac_Offset) * PosPr);
	    if (PosPr == 9999)
		p->Dac.Data = 0;//g_Ram.ramGroupC.Dac_Offset / 4;	//???
	}

    //------Настройка коэффициентов и усилителей датчиков тока в зависимости от типа привода----------------------------

    if(g_Ram.ramGroupC.DriveType < 15 && g_Ram.ramGroupC.DriveType != 0) CUR_SENSOR_GAIN = 0;
    if(g_Ram.ramGroupC.DriveType< 5 && g_Ram.ramGroupC.DriveType != 0) CUR_SENSOR_GAIN = 1;
/*
 *  dt100_A25 	= 1,
	dt100_A50 	= 2,
	dt400_B20 	= 3,
	dt400_B50 	= 4,
	dt800_V40 	= 5,
	dt1000_V20  = 6,
	dt4000_G9   = 7,
	dt4000_G18  = 8,
	dt10000_D6  = 9,
	dt10000_D12 = 10,
	dt10000_D10 = 11,
	dt15000_D10 = 12,
	dt20000_F40 = 13,
	dt35000_F48 = 14,
	dt50000_F48 = 15
 * */

    if(g_Ram.ramGroupC.DriveType != 0)
    {
    	if(g_Ram.ramGroupC.DriveType < dt4000_G9)
    	{
    		CUR_SENSOR_GAIN = 1;
    	}
    	else if (g_Ram.ramGroupC.DriveType < dt10000_D10)
    	{
    		CUR_SENSOR_GAIN = 0;
    	}
    	else if (g_Ram.ramGroupC.DriveType < dt35000_F48)
    	{
    		CUR_SENSOR_GAIN = 1;
    	}
    	else if (g_Ram.ramGroupC.DriveType <= dt50000_F48)
    	{
    		CUR_SENSOR_GAIN = 0;
    	}

    }
    else
    {
    	CUR_SENSOR_GAIN = g_Ram.ramGroupC.CurrentMpyType; // 0 - большие токи, 1 - малые токи
    }

}
//-----------------Обработка данных с микросхем по И2С---------------------------
void I2CDevUpdate(TPeref *p)
{
    static Uint16 Step = 0;

    I2C_update(&I2cMsg);

    switch (Step)
	{
	case 0:
	    ADT75_Update(&p->TSens);
	    if (!p->TSens.Busy)
		Step = 1;
	    break;
	case 1:
	    MCP4726_Update(&p->Dac);
	    if (!p->Dac.Busy)
		Step = 2;
	    break;
	case 2:
	    DS3231_Update(&p->Rtc);
	    if (!p->Rtc.Busy)
		Step = 0;
	    break;
	}

}

//---------------------------------------------------

void RTC_Control(void)
{
    static Uns PrevTime;
    static Uns PrevDate;
    TTimeVar *DevTime = &g_Ram.ramGroupB.DevTime;
    TDateVar *DevDate = &g_Ram.ramGroupB.DevDate;

    if (g_Peref.Rtc.Busy)
	return;

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
	    g_Ram.ramGroupH.Seconds = (Uns) g_Peref.RtcData.Sec;
	}

    PrevTime = DevTime->all;
    PrevDate = DevDate->all;
    RtcStart = True;
}

//----------------------------------------------
