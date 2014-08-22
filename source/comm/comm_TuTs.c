/*======================================================================
Имя файла:          communication_TU_TS.c
Автор:
Версия файла:
Дата изменения:
Описание:
Модуль обработки сигналов телеуправления и телесигнализации
======================================================================*/
#include "comm.h"
#include "peref.h"
#include "g_Ram.h"

#define ADC_conv_DIN(value, gain, offset)\
		( ( (LgInt)value - (LgInt)offset)* gain) 

void Comm_TuTsInit (TDigitalInterface *p)
{
	p->TypeVoltSignal 			= (Uns *)&g_Ram.ramGroupB.InputType;
	p->TypeLogicSignal		    = (Uns *)&g_Ram.ramGroupB.TuInvert.all;

	// Необходимо взять enableLevel и disableLevel из g_Ram
	p->dinStop.outputReg 		= &p->Inputs.all;
	p->dinStop.enableLevel220 	= (Int *)&g_Ram.ramGroupB.LevelOnStop220;
	p->dinStop.disableLevel220 	= (Int *)&g_Ram.ramGroupB.LevelOffStop220;
	p->dinStop.enableLevel24 	= (Int *)&g_Ram.ramGroupB.LevelOnStop24;
	p->dinStop.disableLevel24 	= (Int *)&g_Ram.ramGroupB.LevelOffStop24;
	p->dinStop.timeOut 			= 3;
	p->dinStop.timer 			= 0;


	p->dinOpen.outputReg        = &p->Inputs.all;
	p->dinOpen.enableLevel220   = (Int *)&g_Ram.ramGroupB.LevelOnOpen220;
	p->dinOpen.disableLevel220  = (Int *)&g_Ram.ramGroupB.LevelOffOpen220;
	p->dinOpen.enableLevel24    = (Int *)&g_Ram.ramGroupB.LevelOnOpen24;
	p->dinOpen.disableLevel24   = (Int *)&g_Ram.ramGroupB.LevelOffOpen24;
	p->dinOpen.timeOut 			= 3;
	p->dinOpen.timer 			= 0;

	p->dinClose.outputReg 		= &p->Inputs.all;
	p->dinClose.enableLevel220  = (Int *)&g_Ram.ramGroupB.LevelOnClose220;
	p->dinClose.disableLevel220 = (Int *)&g_Ram.ramGroupB.LevelOffClose220;
	p->dinClose.enableLevel24   = (Int *)&g_Ram.ramGroupB.LevelOnClose24;
	p->dinClose.disableLevel24  = (Int *)&g_Ram.ramGroupB.LevelOffClose24;
	p->dinClose.timeOut 		= 3;
	p->dinClose.timer 			= 0;

	p->dinMu.outputReg 			= &p->Inputs.all;
	p->dinMu.enableLevel220  	= (Int *)&g_Ram.ramGroupB.LevelOnMU220;
	p->dinMu.disableLevel220 	= (Int *)&g_Ram.ramGroupB.LevelOffMU220;
	p->dinMu.enableLevel24   	= (Int *)&g_Ram.ramGroupB.LevelOnMU24;
	p->dinMu.disableLevel24  	= (Int *)&g_Ram.ramGroupB.LevelOffMU24;
	p->dinMu.timeOut 			= 3;
	p->dinMu.timer 				= 0;

	p->dinResetAlarm.outputReg 		 = &p->Inputs.all;
	p->dinResetAlarm.enableLevel220  = (Int *)&g_Ram.ramGroupB.LevelOnResetAlarm220;
	p->dinResetAlarm.disableLevel220 = (Int *)&g_Ram.ramGroupB.LevelOffResetAlarm220;
	p->dinResetAlarm.enableLevel24   = (Int *)&g_Ram.ramGroupB.LevelOnResetAlarm24;
	p->dinResetAlarm.disableLevel24  = (Int *)&g_Ram.ramGroupB.LevelOffResetAlarm24;
	p->dinResetAlarm.timeOut 		 = 3;
	p->dinResetAlarm.timer 			 = 0;

	p->dinPredReady.outputReg 		 = &p->Inputs.all;
	p->dinPredReady.enableLevel220   = (Int *)&g_Ram.ramGroupB.LevelOnReadyTU220;
	p->dinPredReady.disableLevel220  = (Int *)&g_Ram.ramGroupB.LevelOffReadyTU220;
	p->dinPredReady.enableLevel24    = (Int *)&g_Ram.ramGroupB.LevelOnReadyTU24;
	p->dinPredReady.disableLevel24   = (Int *)&g_Ram.ramGroupB.LevelOffReadyTU24;
	p->dinPredReady.timeOut 		 = 3;
	p->dinPredReady.timer 			 = 0;

	p->dinDu.outputReg 			= &p->Inputs.all;
	p->dinDu.enableLevel220  	= (Int *)&g_Ram.ramGroupB.LevelOnDU220;
	p->dinDu.disableLevel220 	= (Int *)&g_Ram.ramGroupB.LevelOffDU220;
	p->dinDu.enableLevel24   	= (Int *)&g_Ram.ramGroupB.LevelOnDU24;
	p->dinDu.disableLevel24  	= (Int *)&g_Ram.ramGroupB.LevelOffDU24;
	p->dinDu.timeOut 			= 3;
	p->dinDu.timer 				= 0;

}

void Comm_TuTsUpdate (TDigitalInterface *p)	//200 Гц
{
	Uns  ts_all=0;

	// ---------------------- ТЕЛЕСИГНАЛИЗАЦИЯ-------------------------------
	p->Outputs.bit.Opened  = g_Ram.ramGroupA.Status.bit.Opened;	    // 0	Открыто
	p->Outputs.bit.Closed  = g_Ram.ramGroupA.Status.bit.Closed;	    // 1	Закрыто
	p->Outputs.bit.Mufta   = g_Ram.ramGroupA.Status.bit.Mufta;	    // 2	Муфта
	p->Outputs.bit.Fault   = g_Ram.ramGroupA.Status.bit.Fault;	    // 3	Авария
	p->Outputs.bit.Defect  = g_Ram.ramGroupA.Status.bit.Defect;	    // 4	Дефект
	p->Outputs.bit.Opening = g_Ram.ramGroupA.Status.bit.Opening;	// 5	Открывается
	p->Outputs.bit.Closing = g_Ram.ramGroupA.Status.bit.Closing;	// 6	Закрывается
	p->Outputs.bit.MUDU    = g_Ram.ramGroupA.Status.bit.MuDu;		// 7	МУ/ДУ
	// ----------------------ИНВЕРСИЯ ТЕЛЕСИГНАЛИЗАЦИИ-------------------------------

	//инверсия ТС
	g_Ram.ramGroupA.StateTs.all = p->Outputs.all ^g_Ram.ramGroupB.TsInvert.all;
	ts_all = p->Outputs.all ^g_Ram.ramGroupB.TsInvert.all;

	// ----------------------ВЫВОД ТЕЛЕСИГНАЛИЗАЦИИ----------------------------------
	TS_1 = (ts_all>>0) & 0x01;
	TS_2 = (ts_all>>1) & 0x01;
	TS_3 = (ts_all>>2) & 0x01;
	TS_4 = (ts_all>>3) & 0x01;
	TS_5 = (ts_all>>4) & 0x01;
	TS_6 = (ts_all>>5) & 0x01;
	TS_7 = (ts_all>>6) & 0x01;
	TS_8 = (ts_all>>7) & 0x01;
	TS_9 = (ts_all>>8) & 0x01;
	TS_10= (ts_all>>9) & 0x01;

	//в группу H добавить
	//g_Ram.ramGroupH.ADC_OPEN=g_Peref.AdcOpen;
	//g_Ram.ramGroupH.ADC_CLOSE=g_Peref.AdcClose;
	//g_Ram.ramGroupH.ADC_STOP=g_Peref.AdcStop;
	//g_Ram.ramGroupH.ADC_TEST_BLOCK=g_Peref.AdcTestBlock;

	if (!(*p->TypeLogicSignal & OPEN_BIT))
		DIN_Update_On(&p->dinOpen, p->TypeVoltSignal, OPEN_BIT);
	else
		DIN_Update_Off(&p->dinOpen, p->TypeVoltSignal, OPEN_BIT);

	if (!(*p->TypeLogicSignal&CLOSE_BIT)) DIN_Update_On(&p->dinClose, p->TypeVoltSignal, CLOSE_BIT);
	else DIN_Update_Off(&p->dinClose, p->TypeVoltSignal, CLOSE_BIT);

	if (!(*p->TypeLogicSignal&STOP_BIT)) DIN_Update_On(&p->dinStop, p->TypeVoltSignal, STOP_BIT);
	else DIN_Update_Off(&p->dinStop, p->TypeVoltSignal, STOP_BIT);

	if (!(*p->TypeLogicSignal&MU_BIT)) DIN_Update_On(&p->dinMu, p->TypeVoltSignal, MU_BIT);
	else DIN_Update_Off(&p->dinMu, p->TypeVoltSignal, MU_BIT);

	if (!(*p->TypeLogicSignal&RESETALARM_BIT)) DIN_Update_On(&p->dinResetAlarm, p->TypeVoltSignal, RESETALARM_BIT);
	else DIN_Update_Off(&p->dinResetAlarm, p->TypeVoltSignal, RESETALARM_BIT);

	if (!(*p->TypeLogicSignal&READYTU_BIT)) DIN_Update_On(&p->dinPredReady, p->TypeVoltSignal, READYTU_BIT);
	else DIN_Update_Off(&p->dinPredReady, p->TypeVoltSignal, READYTU_BIT);

	if (!(*p->TypeLogicSignal&DU_BIT)) DIN_Update_On(&p->dinDu, p->TypeVoltSignal, DU_BIT);
	else DIN_Update_Off(&p->dinDu, p->TypeVoltSignal, DU_BIT);

}
//-----------------------------------------------------
void DIN_Update_On (TDIN *p, Uns *Type,Uns bit)
{
	int disableLevel=0;
	int enableLevel=0;

	if (*Type==it24)
	{
		disableLevel = *p->disableLevel24;
		enableLevel  = *p->enableLevel24;
	}
	else
	if (*Type==it220)
	{
		disableLevel = *p->disableLevel220;
		enableLevel  = *p->enableLevel220;
	}

	if (*p->outputReg & bit)	// Если бит активен
	{

		if (p->inputDIN < disableLevel)//p->disableLevel
			*p->outputReg &= ~bit;
	}
	else					// Иначе (бит не активен)
	{
		if (p->inputDIN > enableLevel )// если уровень сигнала первысил порог
		{	
			if (p->timer++ >= p->timeOut)	
			{
				*p->outputReg |= bit;			// выставляем бит
				p->timer = 0;
			}
		}
		else
			p->timer = 0;
	}
}
//-----------------------------------------------------
void DIN_Update_Off (TDIN *p, Uns *Type,Uns bit)
{
	int disableLevel=0;
	int enableLevel=0;

	if (*Type==it24)
	{
		disableLevel = *p->disableLevel24;
		enableLevel  = *p->enableLevel24;
	}
	else
	if (*Type==it220)
	{
		disableLevel = *p->disableLevel220;
		enableLevel  = *p->enableLevel220;
	}

	if (*p->outputReg & bit)	// Если бит активен
	{
		if (p->inputDIN < disableLevel)// если уровень сигнала ниже порога
		{	
			if (p->timer++ >= p->timeOut)	
			{
				*p->outputReg &= ~bit;	// Снимаем бит
				p->timer = 0;
			}
		}
		else
			p->timer = 0;
	}
	else					// Иначе (бит не активен)
	{
		if (p->inputDIN > enableLevel )//p->disableLevel
			*p->outputReg |= bit;			// выставляем бит
	}
}
//-----------------------------------------------------
/*void DIN_Update (TDIN *p,Uns bit)
{
	if (*p->outputReg & bit)	// Если бит активен
	{
		if (p->inputDIN < *p->disableLevel)//p->disableLevel
			*p->outputReg &= ~bit;
	}
	else					// Иначе (бит не активен)
	{
		if (p->inputDIN > *p->enableLevel )// если уровень сигнала первысил порог
		{
			if (p->timer++ >= p->timeOut)
			{
				*p->outputReg |= bit;			// выставляем бит
				p->timer = 0;
			}
		}
		else
			p->timer = 0;
	}
}
//-----------------------------------------------------
void DIN_Update_Stop (TDIN *p,Uns bit)
{
	if (*p->outputReg & bit)	// Если бит активен
	{
		if (p->inputDIN < *p->disableLevel)// если уровень сигнала ниже порога
		{
			if (p->timer++ >= p->timeOut)
			{
				*p->outputReg &= ~bit;	// Снимаем бит
				p->timer = 0;
			}
		}
		else
			p->timer = 0;
	}
	else					// Иначе (бит не активен)
	{
		if (p->inputDIN > *p->enableLevel )//p->disableLevel
			*p->outputReg |= bit;			// выставляем бит
	}
}*/

//----------------------------------------------------




