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
	p->TypeVoltSignal 				 = (Uns *)&g_Ram.ramGroupB.InputType;
	p->TypeLogicSignal		    	 = (Uns *)&g_Ram.ramGroupB.TuInvert.all;

	// Необходимо взять enableLevel и disableLevel из g_Ram
	p->dinStopOpen.outputReg 			 = &p->Inputs.all;
	p->dinStopOpen.enableLevel220 		 = (Int *)&g_Ram.ramGroupC.LevelOn220;
	p->dinStopOpen.disableLevel220 		 = (Int *)&g_Ram.ramGroupC.LevelOff220;
	p->dinStopOpen.enableLevel24 		 = (Int *)&g_Ram.ramGroupC.LevelOn24;
	p->dinStopOpen.disableLevel24 		 = (Int *)&g_Ram.ramGroupC.LevelOff24;
	p->dinStopOpen.timeOut 				 = g_Ram.ramGroupB.TuTime * Prd50HZ/10;
	p->dinStopOpen.timer 				 = 0;

	p->dinOpen.outputReg        	 = &p->Inputs.all;
	p->dinOpen.enableLevel220   	 = (Int *)&g_Ram.ramGroupC.LevelOn220;
	p->dinOpen.disableLevel220  	 = (Int *)&g_Ram.ramGroupC.LevelOff220;
	p->dinOpen.enableLevel24    	 = (Int *)&g_Ram.ramGroupC.LevelOn24;
	p->dinOpen.disableLevel24   	 = (Int *)&g_Ram.ramGroupC.LevelOff24;
	p->dinOpen.timeOut 				 = g_Ram.ramGroupB.TuTime * Prd50HZ/10;
	p->dinOpen.timer 				 = 0;

	p->dinClose.outputReg 			 = &p->Inputs.all;
	p->dinClose.enableLevel220  	 = (Int *)&g_Ram.ramGroupC.LevelOn220;
	p->dinClose.disableLevel220 	 = (Int *)&g_Ram.ramGroupC.LevelOff220;
	p->dinClose.enableLevel24   	 = (Int *)&g_Ram.ramGroupC.LevelOn24;
	p->dinClose.disableLevel24  	 = (Int *)&g_Ram.ramGroupC.LevelOff24;
	p->dinClose.timeOut 			 = g_Ram.ramGroupB.TuTime * Prd50HZ/10;
	p->dinClose.timer 				 = 0;

	p->dinMu.outputReg 				 = &p->Inputs.all;
	p->dinMu.enableLevel220  		 = (Int *)&g_Ram.ramGroupC.LevelOn220;
	p->dinMu.disableLevel220 		 = (Int *)&g_Ram.ramGroupC.LevelOff220;
	p->dinMu.enableLevel24   		 = (Int *)&g_Ram.ramGroupC.LevelOn24;
	p->dinMu.disableLevel24  		 = (Int *)&g_Ram.ramGroupC.LevelOff24;
	p->dinMu.timeOut 				 = g_Ram.ramGroupB.TuTime * Prd50HZ/10;
	p->dinMu.timer 					 = 0;

	p->dinResetAlarm.outputReg 		 = &p->Inputs.all;
	p->dinResetAlarm.enableLevel220  = (Int *)&g_Ram.ramGroupC.LevelOn220;
	p->dinResetAlarm.disableLevel220 = (Int *)&g_Ram.ramGroupC.LevelOff220;
	p->dinResetAlarm.enableLevel24   = (Int *)&g_Ram.ramGroupC.LevelOn24;
	p->dinResetAlarm.disableLevel24  = (Int *)&g_Ram.ramGroupC.LevelOff24;
	p->dinResetAlarm.timeOut 		 = g_Ram.ramGroupB.TuTime * Prd50HZ/10;
	p->dinResetAlarm.timer 			 = 0;

	p->dinStopClose.outputReg 		 = &p->Inputs.all;
	p->dinStopClose.enableLevel220   = (Int *)&g_Ram.ramGroupC.LevelOn220;
	p->dinStopClose.disableLevel220  = (Int *)&g_Ram.ramGroupC.LevelOff220;
	p->dinStopClose.enableLevel24    = (Int *)&g_Ram.ramGroupC.LevelOn24;
	p->dinStopClose.disableLevel24   = (Int *)&g_Ram.ramGroupC.LevelOff24;
	p->dinStopClose.timeOut 		 = g_Ram.ramGroupB.TuTime * Prd50HZ/10;
	p->dinStopClose.timer 			 = 0;

	p->dinDu.outputReg 				 = &p->Inputs.all;
	p->dinDu.enableLevel220  		 = (Int *)&g_Ram.ramGroupC.LevelOn220;
	p->dinDu.disableLevel220 		 = (Int *)&g_Ram.ramGroupC.LevelOff220;
	p->dinDu.enableLevel24   		 = (Int *)&g_Ram.ramGroupC.LevelOn24;
	p->dinDu.disableLevel24  		 = (Int *)&g_Ram.ramGroupC.LevelOff24;
	p->dinDu.timeOut 				 = g_Ram.ramGroupB.TuTime * Prd50HZ/10;
	p->dinDu.timer 					 = 0;
}

void Comm_TuTsUpdate (TDigitalInterface *p)	//50 Гц
{
	static TOutputReg OutputRegTmp;
	static Uns TuEnbReleTimer;

	// ---------------------- ТЕЛЕСИГНАЛИЗАЦИЯ-------------------------------
	if (!(g_Ram.ramGroupA.Status.bit.Opened || g_Ram.ramGroupA.Status.bit.Closed))
	{
		p->Outputs.bit.Opened  = !g_Ram.ramGroupA.Status.bit.Opened;	// 0	Открыто
		p->Outputs.bit.Closed  = !g_Ram.ramGroupA.Status.bit.Closed;	// 1	Закрыто
	}
	else
	{
		p->Outputs.bit.Opened  = g_Ram.ramGroupA.Status.bit.Opened;	    // 0	Открыто
		p->Outputs.bit.Closed  = g_Ram.ramGroupA.Status.bit.Closed;	    // 1	Закрыто
	}

	p->Outputs.bit.Mufta   = g_Ram.ramGroupA.Status.bit.Mufta;	    	// 2	Муфта
	p->Outputs.bit.Fault   = g_Ram.ramGroupA.Status.bit.Fault;	    	// 3	Авария
	p->Outputs.bit.Defect  = g_Ram.ramGroupA.Status.bit.Defect;	    	// 4	Дефект
	p->Outputs.bit.Opening = g_Ram.ramGroupA.Status.bit.Opening;		// 5	Открывается
	p->Outputs.bit.Closing = g_Ram.ramGroupA.Status.bit.Closing;		// 6	Закрывается
	p->Outputs.bit.MUDU    = !g_Ram.ramGroupA.Status.bit.MuDu;			// 7	МУ/ДУ
	// ----------------------ИНВЕРСИЯ ТЕЛЕСИГНАЛИЗАЦИИ-------------------------------

	//инверсия ТС
	//g_Ram.ramGroupA.StateTs.all = p->Outputs.all ^ g_Ram.ramGroupB.TsInvert.all;
		OutputRegTmp.all = p->Outputs.all ^ g_Ram.ramGroupB.TsInvert.all;

			if(g_Ram.ramGroupG.Mode)
			{
				OutputRegTmp.all = g_Ram.ramGroupG.OutputReg.all;
			}

		if (OutputRegTmp.all != g_Ram.ramGroupA.StateTs.all)
		{
			ENB_RELE = 0;
			g_Ram.ramGroupA.StateTs.all = OutputRegTmp.all;
			Peref_74HC595Update(&g_Peref.ShiftReg, g_Ram.ramGroupA.StateTs);
			TuEnbReleTimer = (0.3 * Prd50HZ);
		}

		if(TuEnbReleTimer > 0) TuEnbReleTimer--;
		else if(TuEnbReleTimer == 0 && ENB_RELE == 0) ENB_RELE = 1;

	// ----------------------ВЫВОД ТЕЛЕСИГНАЛИЗАЦИИ----------------------------------
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

	if (!(*p->TypeLogicSignal&STOP_OPEN_BIT)) DIN_Update_On(&p->dinStopOpen, p->TypeVoltSignal, STOP_OPEN_BIT);
	else DIN_Update_Off(&p->dinStopOpen, p->TypeVoltSignal, STOP_OPEN_BIT);

	if (!(*p->TypeLogicSignal&MU_BIT)) DIN_Update_On(&p->dinMu, p->TypeVoltSignal, MU_BIT);
	else DIN_Update_Off(&p->dinMu, p->TypeVoltSignal, MU_BIT);

	if (!(*p->TypeLogicSignal&RESETALARM_BIT)) DIN_Update_On(&p->dinResetAlarm, p->TypeVoltSignal, RESETALARM_BIT);
	else DIN_Update_Off(&p->dinResetAlarm, p->TypeVoltSignal, RESETALARM_BIT);

	if (!(*p->TypeLogicSignal&STOP_CLOSE_BIT)) DIN_Update_On(&p->dinStopClose, p->TypeVoltSignal, STOP_CLOSE_BIT);
	else DIN_Update_Off(&p->dinStopClose, p->TypeVoltSignal, STOP_CLOSE_BIT);

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

		if (p->inputDIN <= disableLevel)//p->disableLevel
			*p->outputReg &= ~bit;
	}
	else					// Иначе (бит не активен)
	{
		if (p->inputDIN >= enableLevel )// если уровень сигнала первысил порог
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




