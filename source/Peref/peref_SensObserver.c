/*======================================================================
Имя файла:          peref_SensObserver.c
Автор:				Попов И.А.
Версия файла:		1.0
Дата изменения:		04.04.14
Описание: Обработка датчиков тока и напряжения
Вход  - Фильтрованной значение с АЦП в
Выход - Приведенные значения I и U   в
======================================================================*/

#include "peref.h"

void Peref_SensObserverInit(TSensObserver *p)
{
	p->parSensors.p_UR_Mpy		= &g_Ram.ramGroupC.UR_Mpy;
	p->parSensors.p_US_Mpy 		= &g_Ram.ramGroupC.US_Mpy;
	p->parSensors.p_UT_Mpy 		= &g_Ram.ramGroupC.UT_Mpy;
	p->parSensors.p_UR_Offset 	= &g_Ram.ramGroupC.UR_Offset;
	p->parSensors.p_US_Offset 	= &g_Ram.ramGroupC.US_Offset;
	p->parSensors.p_UT_Offset 	= &g_Ram.ramGroupC.UT_Offset;
	p->parSensors.p_IU_Mpy 		= &g_Ram.ramGroupC.IU_Mpy;
	p->parSensors.p_IV_Mpy 		= &g_Ram.ramGroupC.IV_Mpy;
	p->parSensors.p_IW_Mpy 		= &g_Ram.ramGroupC.IW_Mpy;
	p->parSensors.p_IU_Offset 	= &g_Ram.ramGroupC.IU_Offset;
	p->parSensors.p_IV_Offset 	= &g_Ram.ramGroupC.IV_Offset;
	p->parSensors.p_IW_Offset	= &g_Ram.ramGroupC.IW_Offset;

	p->pOffsetMode = mAutoOffset;
}

void Peref_SensTuObserverInit(TSensTuObserver *p)
{
	p->parSensors.p_UOpen_Mpy			= &g_Ram.ramGroupH.UOpen_Mpy;
	p->parSensors.p_UOpen_Offset		= &g_Ram.ramGroupH.p_UOpen_Offset;

	p->parSensors.p_UClose_Mpy			= &g_Ram.ramGroupH.p_UClose_Mpy;
	p->parSensors.p_UClose_Offset		= &g_Ram.ramGroupH.p_UClose_Offset;

	p->parSensors.p_UStop_Mpy			= &g_Ram.ramGroupH.p_UStop_Mpy;
	p->parSensors.p_UStop_Offset		= &g_Ram.ramGroupH.p_UStop_Offset;

	p->parSensors.p_UMu_Mpy				= &g_Ram.ramGroupH.p_UMu_Mpy;
	p->parSensors.p_UStop_Offset		= &g_Ram.ramGroupH.p_UStop_Offset;

	p->parSensors.p_UStop_Mpy			= &g_Ram.ramGroupH.p_UStop_Mpy;
	p->parSensors.p_UMu_Offset			= &g_Ram.ramGroupH.p_UMu_Offset;

	p->parSensors.p_UResetAlarm_Mpy		= &g_Ram.ramGroupH.p_UResetAlarm_Mpy;
	p->parSensors.p_UResetAlarm_Offset	= &g_Ram.ramGroupH.p_UResetAlarm_Offset;

	p->parSensors.p_UReadyTu_Mpy		= &g_Ram.ramGroupH.p_UReadyTu_Mpy;
	p->parSensors.p_UReadyTu_Offset		= &g_Ram.ramGroupH.p_UReadyTu_Offset;

	p->parSensors.p_UDu_Mpy				= &g_Ram.ramGroupH.p_UDu_Mpy;
	p->parSensors.p_UDu_Offset			= &g_Ram.ramGroupH.p_UDu_Offset;

	p->pOffsetMode = mAutoOffset;
}

void SetOffsets(TSensObserver *p) //обработка оффсетов
{
	switch(p->pOffsetMode)
	{
	case mManual: break;
	case mAutoOffset:
		{

		}
		break;
	case mTest: break;

	}
}

void Peref_SensObserverUpdate(TSensObserver *p) // 18000 Гц
{
	// Преобразование напряжений
	p->URout = ADC_CONV(p->URinp, *p->parSensors.p_UR_Mpy, *p->parSensors.p_UR_Offset);
	p->USout = ADC_CONV(p->USinp, *p->parSensors.p_US_Mpy, *p->parSensors.p_US_Offset);
	p->UTout = ADC_CONV(p->UTinp, *p->parSensors.p_UT_Mpy, *p->parSensors.p_UT_Offset);

	// Преобразование токов
	p->IUout = ADC_CONV(p->IUinp, *p->parSensors.p_IU_Mpy, *p->parSensors.p_IU_Offset);
	p->IVout = ADC_CONV(p->IVinp, *p->parSensors.p_IV_Mpy, *p->parSensors.p_IV_Offset);
	p->IWout = ADC_CONV(p->IWinp, *p->parSensors.p_IW_Mpy, *p->parSensors.p_IW_Offset);

	// Проверили и поправили значения оффсетов
	SetOffsets(p);
}

void Peref_SensTuObserverUpdate(TSensTuObserver *p) // 18000 Гц
{
	// Преобразование напряжений
	p->UOpenOut  		= ADC_CONV(p->UOpenInp, 	  *p->parSensors.p_UOpen_Mpy, 		*p->parSensors.p_UOpen_Offset);
	p->UCloseOut 		= ADC_CONV(p->UCloseInp, 	  *p->parSensors.p_UClose_Mpy, 		*p->parSensors.p_UClose_Offset);
	p->UStopOut 		= ADC_CONV(p->UStopInp, 	  *p->parSensors.p_UStop_Mpy, 		*p->parSensors.p_UStop_Offset);
	p->UMuOut 			= ADC_CONV(p->UMuInp, 		  *p->parSensors.p_UMu_Mpy, 		*p->parSensors.p_UMu_Offset);
	p->UResetAlarmOut 	= ADC_CONV(p->UResetAlarmInp, *p->parSensors.p_UResetAlarm_Mpy, *p->parSensors.p_UResetAlarm_Offset);
	p->UReadyTuOut 		= ADC_CONV(p->UReadyTuInp,    *p->parSensors.p_UReadyTu_Mpy, 	*p->parSensors.p_UReadyTu_Offset);
	p->UDuOut		 	= ADC_CONV(p->UDuInp, 		  *p->parSensors.p_UDu_Mpy, 		*p->parSensors.p_UDu_Offset);

	// Проверили и поправили значения оффсетов
	//SetOffsets(p);
}


