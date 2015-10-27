/*======================================================================
Имя файла:          core_Protections.h
Автор:				Попов И.А.
Версия файла:		1.01
Дата изменения:		10.06.2015
Описание:
модуль защит электропривода
======================================================================*/

#include "core.h"
#include "peref.h"


void Core_ProtectionsInit(TCoreProtections *p)
{
	//---------ЗАЩИТЫ ПРОЦЕССА--------------------------------------------------------
	//---------Муфта--------------------------------------------------------

		// Муфта выставляется в core.c

	//---------Нет Движенния-----------------------------------------------

	p->NoMove.Cfg.all			= PRT_CFG_SET(CAN_BE_MUFTA, INP_LESS_LEVEL,NoMove_bit,HYST_OFF);
	p->NoMove.Input				= (Int *)&g_Ram.ramGroupA.Position;
	p->NoMove.Output			= &p->NoMoveFlag;
	p->NoMove.EnableLevel		= (Int *)&g_Ram.ramGroupC.MuffZone;
	p->NoMove.DisableLevel		= (Int *)&g_Ram.ramGroupC.MuffZone;
	p->NoMove.Timeout			= &g_Ram.ramGroupB.NoMoveTime;
	p->NoMove.Scale				= PROTECT_SCALE;
	p->NoMove.Signal			= 0;

	//---------ЗАЩИТЫ ПО НАПРЯЖЕНИЮ---------------------------------------
	//---------Пониженное напряжение (авария)---------------------------------------

	p->underVoltageR.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,UvR_bit,20);
	p->underVoltageS.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,UvS_bit,20);
	p->underVoltageT.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,UvT_bit,20);

	p->underVoltageR.Input 			= (Int *)&g_Ram.ramGroupA.Ur;
	p->underVoltageS.Input 			= (Int *)&g_Ram.ramGroupA.Us;
	p->underVoltageT.Input 			= (Int *)&g_Ram.ramGroupA.Ut;

	p->underVoltageR.Output 		= (Uns *)&p->outFaults.Net.all;
	p->underVoltageS.Output 		= (Uns *)&p->outFaults.Net.all;
	p->underVoltageT.Output 	 	= (Uns *)&p->outFaults.Net.all;

	p->underVoltageR.EnableLevel  		= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underVoltageS.EnableLevel  		= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underVoltageT.EnableLevel  		= (Int *)&g_Ram.ramGroupC.UvLevel;

	p->underVoltageR.DisableLevel 		= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underVoltageS.DisableLevel 		= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underVoltageT.DisableLevel 		= (Int *)&g_Ram.ramGroupC.UvLevel;

	p->underVoltageR.Timeout		= &g_Ram.ramGroupC.UvTime;
	p->underVoltageS.Timeout		= &g_Ram.ramGroupC.UvTime;
	p->underVoltageT.Timeout		= &g_Ram.ramGroupC.UvTime;

	p->underVoltageR.Scale 			= PROTECT_SCALE;
	p->underVoltageS.Scale 			= PROTECT_SCALE;
	p->underVoltageT.Scale 			= PROTECT_SCALE;

	//---------Пониженное напряжение (неисправность)---------------------------------------

	p->underDefVoltageR.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,UvR_bit,20);
	p->underDefVoltageS.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,UvS_bit,20);
	p->underDefVoltageT.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,UvT_bit,20);

	p->underDefVoltageR.Input 		= (Int *)&g_Ram.ramGroupA.Ur;
	p->underDefVoltageS.Input 		= (Int *)&g_Ram.ramGroupA.Us;
	p->underDefVoltageT.Input 		= (Int *)&g_Ram.ramGroupA.Ut;

	p->underDefVoltageR.Output 		= (Uns *)&p->outDefects.Net.all;
	p->underDefVoltageS.Output 		= (Uns *)&p->outDefects.Net.all;
	p->underDefVoltageT.Output 	 	= (Uns *)&p->outDefects.Net.all;

	p->underDefVoltageR.EnableLevel 	= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underDefVoltageS.EnableLevel 	= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underDefVoltageT.EnableLevel 	= (Int *)&g_Ram.ramGroupC.UvLevel;

	p->underDefVoltageR.DisableLevel	= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underDefVoltageS.DisableLevel	= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underDefVoltageT.DisableLevel	= (Int *)&g_Ram.ramGroupC.UvLevel;

	p->underDefVoltageR.Timeout		= &g_Ram.ramGroupC.UvDTime;
	p->underDefVoltageS.Timeout		= &g_Ram.ramGroupC.UvDTime;
	p->underDefVoltageT.Timeout		= &g_Ram.ramGroupC.UvDTime;

	p->underDefVoltageR.Scale 		= PROTECT_SCALE;
	p->underDefVoltageS.Scale 		= PROTECT_SCALE;
	p->underDefVoltageT.Scale 		= PROTECT_SCALE;

	//---------Повышенное напряжение (авария)------------------------------------------
	p->overVoltageR.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvR_bit,20);
	p->overVoltageS.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvS_bit,20);
	p->overVoltageT.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvT_bit,20);

	p->overVoltageR.Input 			= (Int *)&g_Ram.ramGroupA.Ur;
	p->overVoltageS.Input 			= (Int *)&g_Ram.ramGroupA.Us;
	p->overVoltageT.Input 			= (Int *)&g_Ram.ramGroupA.Ut;

	p->overVoltageR.Output 			= (Uns *)&p->outFaults.Net.all;
	p->overVoltageS.Output 			= (Uns *)&p->outFaults.Net.all;
	p->overVoltageT.Output 	 		= (Uns *)&p->outFaults.Net.all;

	p->overVoltageR.EnableLevel  	= (Int *)&g_Ram.ramGroupC.OvLevel;
	p->overVoltageS.EnableLevel  	= (Int *)&g_Ram.ramGroupC.OvLevel;
	p->overVoltageT.EnableLevel  	= (Int *)&g_Ram.ramGroupC.OvLevel;

	p->overVoltageR.DisableLevel 	= (Int *)&g_Ram.ramGroupC.OvLevel;
	p->overVoltageS.DisableLevel 	= (Int *)&g_Ram.ramGroupC.OvLevel;
	p->overVoltageT.DisableLevel 	= (Int *)&g_Ram.ramGroupC.OvLevel;

	p->overVoltageR.Timeout			= &g_Ram.ramGroupC.OvTime;
	p->overVoltageS.Timeout			= &g_Ram.ramGroupC.OvTime;
	p->overVoltageT.Timeout			= &g_Ram.ramGroupC.OvTime;

	p->overVoltageR.Scale 			= PROTECT_SCALE;
	p->overVoltageS.Scale 			= PROTECT_SCALE;
	p->overVoltageT.Scale 			= PROTECT_SCALE;

	//---------Повышенное напряжение (неисправность)------------------------------------------
	p->overDefVoltageR.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvR_bit,20);
	p->overDefVoltageS.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvS_bit,20);
	p->overDefVoltageT.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvT_bit,20);

	p->overDefVoltageR.Input 		= (Int *)&g_Ram.ramGroupA.Ur;
	p->overDefVoltageS.Input 		= (Int *)&g_Ram.ramGroupA.Us;
	p->overDefVoltageT.Input 		= (Int *)&g_Ram.ramGroupA.Ut;

	p->overDefVoltageR.Output 		= (Uns *)&p->outDefects.Net.all;
	p->overDefVoltageS.Output 		= (Uns *)&p->outDefects.Net.all;
	p->overDefVoltageT.Output 	 	= (Uns *)&p->outDefects.Net.all;

	p->overDefVoltageR.EnableLevel  = (Int *)&g_Ram.ramGroupC.OvLevel;
	p->overDefVoltageS.EnableLevel  = (Int *)&g_Ram.ramGroupC.OvLevel;
	p->overDefVoltageT.EnableLevel  = (Int *)&g_Ram.ramGroupC.OvLevel;

	p->overDefVoltageR.DisableLevel = (Int *)&g_Ram.ramGroupC.OvLevel;
	p->overDefVoltageS.DisableLevel = (Int *)&g_Ram.ramGroupC.OvLevel;
	p->overDefVoltageT.DisableLevel = (Int *)&g_Ram.ramGroupC.OvLevel;

	p->overDefVoltageR.Timeout		= &g_Ram.ramGroupC.OvDTime;
	p->overDefVoltageS.Timeout		= &g_Ram.ramGroupC.OvDTime;
	p->overDefVoltageT.Timeout		= &g_Ram.ramGroupC.OvDTime;

	p->overDefVoltageR.Scale 		= PROTECT_SCALE;
	p->overDefVoltageS.Scale 		= PROTECT_SCALE;
	p->overDefVoltageT.Scale 		= PROTECT_SCALE;

	//---------Повышенное напряжение на 47% (АВАРИЯ) ------------------------------------------
	p->overMax_VoltageR.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvR_max_bit,20);
	p->overMax_VoltageS.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvS_max_bit,20);
	p->overMax_VoltageT.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvT_max_bit,20);

	p->overMax_VoltageR.Input 		= (Int *)&g_Ram.ramGroupA.Ur;
	p->overMax_VoltageS.Input 		= (Int *)&g_Ram.ramGroupA.Us;
	p->overMax_VoltageT.Input 		= (Int *)&g_Ram.ramGroupH.Ut;

	p->overMax_VoltageR.Output 		= (Uns *)&p->outFaults.Net.all;
	p->overMax_VoltageS.Output 		= (Uns *)&p->outFaults.Net.all;
	p->overMax_VoltageT.Output 	 	= (Uns *)&p->outFaults.Net.all;

	p->overMax_VoltageR.EnableLevel	= (Int *)&g_Ram.ramGroupC.OvLevel_max;
	p->overMax_VoltageS.EnableLevel	= (Int *)&g_Ram.ramGroupC.OvLevel_max;
	p->overMax_VoltageT.EnableLevel	= (Int *)&g_Ram.ramGroupC.OvLevel_max;

	p->overMax_VoltageR.DisableLevel	= (Int *)&g_Ram.ramGroupC.OvLevel_max;
	p->overMax_VoltageS.DisableLevel	= (Int *)&g_Ram.ramGroupC.OvLevel_max;
	p->overMax_VoltageT.DisableLevel	= (Int *)&g_Ram.ramGroupC.OvLevel_max;

	p->overMax_VoltageR.Timeout		= &g_Ram.ramGroupC.OvTime_max;
	p->overMax_VoltageS.Timeout		= &g_Ram.ramGroupC.OvTime_max;
	p->overMax_VoltageT.Timeout		= &g_Ram.ramGroupC.OvTime_max;

	p->overMax_VoltageR.Scale 		= PROTECT_SCALE;
	p->overMax_VoltageS.Scale 		= PROTECT_SCALE;
	p->overMax_VoltageT.Scale 		= PROTECT_SCALE;

	//---------Обрыв фаз напряжения (R S T  авария)-----------------------
	p->breakVoltR.Cfg.all			= PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL,BvR_bit,20);
	p->breakVoltS.Cfg.all 			= PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL,BvS_bit,20);
	p->breakVoltT.Cfg.all 			= PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL,BvT_bit,20);

	p->breakVoltR.Input 			= (Int *)&g_Ram.ramGroupA.Ur;
	p->breakVoltS.Input 			= (Int *)&g_Ram.ramGroupA.Us;
	p->breakVoltT.Input 			= (Int *)&g_Ram.ramGroupA.Ut;

	p->breakVoltR.Output 			= (Uns *)&p->outFaults.Net.all;
	p->breakVoltS.Output 			= (Uns *)&p->outFaults.Net.all;
	p->breakVoltT.Output 	 		= (Uns *)&p->outFaults.Net.all;

	p->breakVoltR.EnableLevel  		= (Int *)&g_Ram.ramGroupC.BvLevel;
	p->breakVoltS.EnableLevel  		= (Int *)&g_Ram.ramGroupC.BvLevel;
	p->breakVoltT.EnableLevel  		= (Int *)&g_Ram.ramGroupC.BvLevel;

	p->breakVoltR.DisableLevel 		= (Int *)&g_Ram.ramGroupC.BvLevel;
	p->breakVoltS.DisableLevel 		= (Int *)&g_Ram.ramGroupC.BvLevel;
	p->breakVoltT.DisableLevel 		= (Int *)&g_Ram.ramGroupC.BvLevel;

	p->breakVoltR.Timeout			= &g_Ram.ramGroupC.BvTime;
	p->breakVoltS.Timeout			= &g_Ram.ramGroupC.BvTime;
	p->breakVoltT.Timeout			= &g_Ram.ramGroupC.BvTime;

	p->breakVoltR.Scale 			= PROTECT_SCALE;
	p->breakVoltS.Scale 			= PROTECT_SCALE;
	p->breakVoltT.Scale 			= PROTECT_SCALE;

	//---------Ассиметрия напряжений (неисправность)-----------------------------------
	p->voltSkew.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,VSk_bit,20);
	p->voltSkew.Input			= (Int *)&g_Ram.ramGroupH.VSkValue;
	p->voltSkew.Output			= (Uns *)&p->outDefects.Net.all;
	p->voltSkew.EnableLevel			= (Int *)&g_Ram.ramGroupC.VSkLevel;
	p->voltSkew.DisableLevel		= (Int *)&g_Ram.ramGroupC.VSkLevel;
	p->voltSkew.Timeout			= &g_Ram.ramGroupC.VSkTime;
	p->voltSkew.Scale			= PROTECT_SCALE;

	//---------ЗАЩИТЫ ПО ТОКУ---------------------------------------------
	//---------Обрыв фаз по току (U V W неисправность)---------------------------------
	p->breakCurrU.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,PhlU_bit,HYST_OFF);
	p->breakCurrV.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,PhlV_bit,HYST_OFF);
	p->breakCurrW.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,PhlW_bit,HYST_OFF);

	p->breakCurrU.Input 			= (Int *)&g_Ram.ramGroupH.IuPr;
	p->breakCurrV.Input 			= (Int *)&g_Ram.ramGroupH.IvPr;
	p->breakCurrW.Input 			= (Int *)&g_Ram.ramGroupH.IwPr;

	//p->breakCurrU.Output 			= (Uns *)&p->outDefects.Load.all;
	//p->breakCurrV.Output 			= (Uns *)&p->outDefects.Load.all;
	//p->breakCurrW.Output 	 		= (Uns *)&p->outDefects.Load.all;
	p->breakCurrU.Output 			= (Uns *)&p->registerBrCurr;
	p->breakCurrV.Output 			= (Uns *)&p->registerBrCurr;
	p->breakCurrW.Output 	 		= (Uns *)&p->registerBrCurr;

	p->breakCurrU.EnableLevel  		= (Int *)&g_Ram.ramGroupC.PhlLevel;
	p->breakCurrV.EnableLevel  		= (Int *)&g_Ram.ramGroupC.PhlLevel;
	p->breakCurrW.EnableLevel  		= (Int *)&g_Ram.ramGroupC.PhlLevel;

	p->breakCurrU.DisableLevel 		= (Int *)&g_Ram.ramGroupC.PhlLevel;
	p->breakCurrV.DisableLevel 		= (Int *)&g_Ram.ramGroupC.PhlLevel;
	p->breakCurrW.DisableLevel 		= (Int *)&g_Ram.ramGroupC.PhlLevel;

	p->breakCurrU.Timeout			= &g_Ram.ramGroupC.PhlTime;
	p->breakCurrV.Timeout			= &g_Ram.ramGroupC.PhlTime;
	p->breakCurrW.Timeout			= &g_Ram.ramGroupC.PhlTime;

	p->breakCurrU.Scale 			= PROTECT_SCALE;
	p->breakCurrV.Scale 			= PROTECT_SCALE;
	p->breakCurrW.Scale 			= PROTECT_SCALE;

	//---------Время-токовая перегрузка (авария)-------------------------------------
	p->I2t.Cfg.bit.Hyst 			= HYST_OFF;
	p->I2t.Cfg.bit.Num 				= I2t_bit;
	p->I2t.InputCurrentMid 			= &g_Ram.ramGroupH.Imid;
	p->I2t.NomCurrent 				= &g_Ram.ramGroupC.Inom;
	p->I2t.Output 					= (Uns *)&p->outFaults.Load.all;
	p->I2t.Scale 					= PROTECT_SCALE;
	Core_ProtectionI2TInit(&p->I2t);

	//--------Ассиметрия токов (неисправность)----------------------------------------------
	p->voltSkew.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,ISkew_bit,HYST_OFF);
	p->voltSkew.Input				= (Int *)&g_Ram.ramGroupH.ISkewValue;
	p->voltSkew.Output				= (Uns *)&p->outDefects.Load.all;
	p->voltSkew.EnableLevel			= (Int *)&g_Ram.ramGroupC.ISkewLevel;
	p->voltSkew.DisableLevel		= (Int *)&g_Ram.ramGroupC.ISkewLevel;
	p->voltSkew.Timeout				= &g_Ram.ramGroupC.ISkewTime;
	p->voltSkew.Scale				= PROTECT_SCALE;

	//------Короткое замыкание----------------------------------------------
	p->ShC_U.Cfg.bit.Num 			= ShCU_bit;
	p->ShC_V.Cfg.bit.Num 			= ShCV_bit;
	p->ShC_W.Cfg.bit.Num 			= ShCW_bit;

	p->ShC_U.Cfg.bit.Hyst 			= HYST_OFF;
	p->ShC_V.Cfg.bit.Hyst 			= HYST_OFF;
	p->ShC_W.Cfg.bit.Hyst 			= HYST_OFF;

	p->ShC_U.InputCurrent			= (Uns *)&ADC_IU;
	p->ShC_V.InputCurrent			= (Uns *)&ADC_IV;
	p->ShC_W.InputCurrent			= (Uns *)&ADC_IW;

	p->ShC_U.HighCurrentLevel		= &g_Ram.ramGroupC.ShC_Up;
	p->ShC_V.HighCurrentLevel		= &g_Ram.ramGroupC.ShC_Up;
	p->ShC_W.HighCurrentLevel		= &g_Ram.ramGroupC.ShC_Up;

	p->ShC_U.LowCurrentLevel		= &g_Ram.ramGroupC.ShC_Down;
	p->ShC_V.LowCurrentLevel		= &g_Ram.ramGroupC.ShC_Down;
	p->ShC_W.LowCurrentLevel		= &g_Ram.ramGroupC.ShC_Down;

	p->ShC_U.Output					= &p->ShcTmpState;//&p->outFaults.Load.all;
	p->ShC_V.Output					= &p->ShcTmpState;//&p->outFaults.Load.all;
	p->ShC_W.Output					= &p->ShcTmpState;//&p->outFaults.Load.all;

	//------ДИАГНОСТИКА УСТРОЙСТВА------------------------------------------
	//------Перегрев блока БКД----------------------------------------------
	p->overHeatBCD.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,Th_BCD_bit,1);
	p->overHeatBCD.Input			= &g_Ram.ramGroupA.Temper;
	p->overHeatBCD.Output			= &p->outDefects.Dev.all;
	p->overHeatBCD.EnableLevel		= &g_Ram.ramGroupC.TemperHigh;
	p->overHeatBCD.DisableLevel		= &g_Ram.ramGroupC.TemperHigh;
	p->overHeatBCD.Timeout			= &g_Ram.ramGroupC.BvTime;
	p->overHeatBCD.Scale			= PROTECT_SCALE;

	//------Переохлаждение блока БКД----------------------------------------
	p->underColdBCD.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,Tl_BCD_bit,1);
	p->underColdBCD.Input			= &g_Ram.ramGroupA.Temper;
	p->underColdBCD.Output			= &p->outDefects.Dev.all;
	p->underColdBCD.EnableLevel		= &g_Ram.ramGroupC.TemperLow;
	p->underColdBCD.DisableLevel	= &g_Ram.ramGroupC.TemperLow;
	p->underColdBCD.Timeout			= &g_Ram.ramGroupC.BvTime;
	p->underColdBCD.Scale			= PROTECT_SCALE;

	//------Перегрев блока БКП----------------------------------------------
	p->overHeatBCP.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,Th_BCP_bit,1);
	p->overHeatBCP.Input			= &g_Ram.ramGroupA.TemperBKP;
	p->overHeatBCP.Output			= &p->outFaults.Dev.all;
	p->overHeatBCP.EnableLevel		= &g_Ram.ramGroupC.TemperHigh;
	p->overHeatBCP.DisableLevel		= &g_Ram.ramGroupC.TemperHigh;
	p->overHeatBCP.Timeout			= &g_Ram.ramGroupC.BvTime;
	p->overHeatBCP.Scale			= PROTECT_SCALE;

	//------Переохлаждение блока БКП----------------------------------------
	p->underColdBCP.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,Tl_BCP_bit,1);
	p->underColdBCP.Input			= &g_Ram.ramGroupA.TemperBKP;
	p->underColdBCP.Output			= &p->outDefects.Dev.all;
	p->underColdBCP.EnableLevel		= &g_Ram.ramGroupC.TemperLow;
	p->underColdBCP.DisableLevel		= &g_Ram.ramGroupC.TemperLow;
	p->underColdBCP.Timeout			= &g_Ram.ramGroupC.BvTime;
	p->underColdBCP.Scale			= PROTECT_SCALE;

	p->FaultDelay = (Uns)(Prd10HZ * 2);
	p->registerBrCurr = 0;
	p->registerBrVolt = 0;
}

// Функция включения/выключения защит
void Core_ProtectionsEnable(TCoreProtections *p)
{
	static Byte State = 0;
	Bool Enable;

	if (p->FaultDelay > 0) return;

	switch (++State)
	{
	case 1: //Muffta
		p->NoMove.Cfg.bit.Enable = ((g_Core.MotorControl.WorkMode & wmMove) != 0);
		break;
	case 2:	// Защиты по наряжению
		Enable = (g_Ram.ramGroupC.Uv != pmOff) && (!p->outDefects.Dev.bit.LowPower);						// Понижеие напряжения
		p->underVoltageR.Cfg.bit.Enable 	= Enable;
		p->underVoltageS.Cfg.bit.Enable 	= Enable;
		p->underVoltageT.Cfg.bit.Enable 	= Enable;
		p->underDefVoltageR.Cfg.bit.Enable 	= Enable;
		p->underDefVoltageS.Cfg.bit.Enable 	= Enable;
		p->underDefVoltageT.Cfg.bit.Enable 	= Enable;

		Enable = (g_Ram.ramGroupC.Ov != pmOff);						// Повышение напряжения
		p->overVoltageR.Cfg.bit.Enable 		= Enable;
		p->overVoltageS.Cfg.bit.Enable 		= Enable;
		p->overVoltageT.Cfg.bit.Enable 		= Enable;
		p->overDefVoltageR.Cfg.bit.Enable 	= Enable;
		p->overDefVoltageS.Cfg.bit.Enable 	= Enable;
		p->overDefVoltageT.Cfg.bit.Enable 	= Enable;
		p->overMax_VoltageR.Cfg.bit.Enable 	= Enable;
		p->overMax_VoltageS.Cfg.bit.Enable 	= Enable;
		p->overMax_VoltageT.Cfg.bit.Enable 	= Enable;

		Enable = (g_Ram.ramGroupC.Bv != pmOff) && (!p->outDefects.Dev.bit.LowPower);						// Обрыв фаз напряжения
		p->breakVoltR.Cfg.bit.Enable 		= Enable;
		p->breakVoltS.Cfg.bit.Enable 		= Enable;
		p->breakVoltT.Cfg.bit.Enable 		= Enable;

		p->voltSkew.Cfg.bit.Enable = (g_Ram.ramGroupC.VSk != pmOff);//Небаланс напряжений
		break;
	case 3: // Защиты по току
		Enable = (g_Ram.ramGroupC.Phl != pmOff) && (!g_Core.Status.bit.Stop);					// Обрыв выходных фаз (двиг.)
		p->breakCurrU.Cfg.bit.Enable 		= Enable;
		p->breakCurrV.Cfg.bit.Enable 		= Enable;
		p->breakCurrW.Cfg.bit.Enable 		= Enable;

		//Enable = (g_Ram.ramGroupC.ShC != pmOff) && (!g_Core.Status.bit.Stop);					// Короткое замыкание
		Enable = (g_Ram.ramGroupC.ShC != pmOff);
		p->ShC_U.Cfg.bit.Enable 			= Enable;
		p->ShC_V.Cfg.bit.Enable 			= Enable;
		p->ShC_W.Cfg.bit.Enable 			= Enable;

		p->currSkew.Cfg.bit.Enable = (g_Ram.ramGroupC.ISkew != pmOff) && (!g_Core.Status.bit.Stop);// Небаланс токов

		p->I2t.Cfg.bit.Enable = (g_Ram.ramGroupC.I2t != pmOff) && (!g_Core.Status.bit.Stop);		// ВТЗ
		break;
	case 4: // Защиты устройства
		Enable = g_Ram.ramGroupC.TemperTrack != pmOff;
		p->overHeatBCD.Cfg.bit.Enable 		= Enable;
		p->overHeatBCP.Cfg.bit.Enable 		= Enable;
		p->underColdBCD.Cfg.bit.Enable 		= Enable;
		p->underColdBCP.Cfg.bit.Enable 		= Enable;

		State = 0;	// вернулись к истокам
		break;
	}
}

// Индикация аварий процесса и устройства
void Core_DevProc_FaultIndic(TCoreProtections *p)
{

	if (p->FaultDelay > 0)
	{
		p->FaultDelay--;
		return;
	}

	p->outFaults.Proc.bit.Overway = g_Core.MotorControl.OverWayFlag;

	p->outDefects.Proc.all &= ~PROC_CALIB_MASK;
	if(g_Ram.ramGroupC.CalibIndic != pmOff)
	{
		p->outDefects.Proc.bit.NoClose = !(g_Ram.ramGroupH.CalibState & csClose);
		p->outDefects.Proc.bit.NoOpen  = !(g_Ram.ramGroupH.CalibState & csOpen);
		p->outDefects.Proc.bit.NoCalib = !(g_Ram.ramGroupH.CalibState & csCalib);
	}

	if(g_Ram.ramGroupC.ErrIndic != pmOff)
	{
		p->outDefects.Dev.bit.Memory1 = Eeprom1.Error;
		p->outDefects.Dev.bit.Memory2 = Eeprom2.Error;
		p->outDefects.Dev.bit.PosSens = 0;//??? добавить при наличии драйвера
		p->outDefects.Dev.bit.Rtc 	  = (Uns)g_Peref.Rtc.Error;
		p->outDefects.Dev.bit.TSens   = (Uns)g_Peref.TSens.Error;
		p->outDefects.Dev.bit.Dac     = (Uns)g_Peref.Dac.Error;

		if (g_Comm.Bluetooth.ModeProtocol!=2) p->outDefects.Dev.bit.NoBCP_Connect = !g_Comm.mbBkp.Frame.ConnFlag;
		else p->outDefects.Dev.bit.NoBCP_Connect = 0;

		//p->outDefects.Dev.bit.NoBCP_Connect = !g_Comm.mbBkp.Frame.ConnFlag;
	}
}

void EngPhOrdPrt(TCoreProtections *p)
{
    static Bool Flag = False;          // флаг для единичной проверки прав?е?льности чередования фазы
    static LgUns StartPos;
    static Uns Timer = 0;
    static Int EngPhOrdValue = 0;
    LgInt Delta;

    if ((g_Ram.ramGroupC.PhOrd == pmOff) || (g_Core.Status.bit.Stop))
	{
	    Flag = false;
	    StartPos = g_Ram.ramGroupA.Position;
	    Timer = 0;
	    return;
	}

    if (Timer < (g_Ram.ramGroupC.PhOrdTime * (Uns)PROTECT_SCALE))
	Timer++;
    else if (!Flag)
	{
	    Delta = g_Ram.ramGroupA.Position - StartPos;

	    if (Delta > ((REV_MAX + 1) / 2))
		Delta -= (REV_MAX + 1);
	    if (Delta < -((REV_MAX + 1) / 2))
		Delta += (REV_MAX + 1);

	    EngPhOrdValue = 0;
	    if (Delta >= ((LgInt) g_Ram.ramGroupC.PhOrdZone))
		EngPhOrdValue = 1;
	    if (Delta <= -((LgInt) g_Ram.ramGroupC.PhOrdZone))
		EngPhOrdValue = -1;

	    if (g_Ram.ramGroupB.RodType == rdtInverse)
		EngPhOrdValue = -EngPhOrdValue;

	    if ((EngPhOrdValue != 0) && (EngPhOrdValue != g_Core.MotorControl.RequestDir))
		p->outFaults.Proc.bit.PhOrd = 1;

	    Flag = true;
	}
}

// Сброс защит при пуске
void Core_ProtectionsReset(TCoreProtections *p)
{
	g_Core.MotorControl.OverWayFlag = 0;		// Сбросили отсусвие уплотнения
	p->MuffFlag = 0;							// отчистили статус от аварии муфты и неисправености
	p->outFaults.Proc.bit.Mufta = 0;
	g_Core.Status.bit.Defect = 0;
	g_Core.Status.bit.Fault = 0;
	g_Core.Status.bit.Mufta = 0;
	p->NoMoveFlag=0;

	p->outDefects.Proc.bit.Mufta=0;
	p->outFaults.Proc.bit.Mufta=0;

	p->outDefects.Proc.bit.NoMove=0;
	p->outFaults.Proc.bit.NoMove=0;

	p->outDefects.Load.bit.ISkew = 0;
	p->outDefects.Load.bit.PhlU = 0;
	p->outDefects.Load.bit.PhlV = 0;
	p->outDefects.Load.bit.PhlW = 0;
	p->registerBrCurr = 0;

	p->outDefects.Proc.bit.Overway = 0;
	p->outDefects.Proc.bit.NoMove = 0;
	p->outFaults.Proc.bit.NoMove = 0;
}

// Сброс всех защит
void Core_ProtectionsClear(TCoreProtections *p)
{
	g_Core.MotorControl.OverWayFlag = 0;		// Сбросили отсусвие уплотнения
	p->MuffFlag = 0;								// сбросили муфту

	g_Core.Status.bit.Fault = 0;
	g_Core.Status.bit.Defect = 0;

	p->outFaults.Dev.all  = 0;					// сбросили все аварии
	p->outFaults.Net.all  = 0;
	p->outFaults.Load.all = 0;
	p->outFaults.Proc.all = 0;

	p->outDefects.Dev.all  = 0;					// сбросили все неисправности
	p->outDefects.Net.all  = 0;
	p->outDefects.Load.all = 0;
	p->registerBrCurr = 0;
	p->registerBrVolt = 0;
	p->outDefects.Proc.all = 0;

	/* ??? сброс ошибок памяти энкодера и ДТ
	Encoder.Error  = False;
	Eeprom1.Error  = False;
	Eeprom2.Error  = False;
	TempSens.Error = False;
	 */
	p->ShcTmpState = 0;
	p->ShcReset = true;

}

void Core_ProtectionsUpdate(TCoreProtections *p)
{
	static Uns prevStatus = 0;
    Uns MuffEnable;

    if (p->FaultDelay > 0)
	return;

    if (p->registerBrVolt) // Если имеются неисправности по обрыву фаз питания (в движении)
    {
    	if (p->registerBrCurr)	// Если вместе с тем имеются неисправности по обрыву фаз двигателя
    	{
      		p->outDefects.Net.all =  (p->outDefects.Net.all & ~NET_BV_MASK) | p->registerBrVolt;// то выставляем обрыв фаз питания
      		p->outDefects.Load.all &=  ~LOAD_PHL_MASK;
    	}
    	else					// Если обрыва фаз двигателя не наблюдается
    		p->outDefects.Net.all &=  ~NET_BV_MASK;
    }
    else					// Если неисправность по обрыву фаз отсутствует
    {
    	p->outDefects.Load.all = (p->outDefects.Load.all & ~LOAD_PHL_MASK) | p->registerBrCurr;
    	p->outDefects.Net.all &=  ~NET_BV_MASK;
    }

	if (p->outFaults.Net.bit.BvR & p->outDefects.Net.bit.UvR) p->outDefects.Net.bit.UvR = 0; // Если сработала авария по обрыву, неисправность "пониженное напр." не выставляем
	if (p->outFaults.Net.bit.BvS & p->outDefects.Net.bit.UvS) p->outDefects.Net.bit.UvS = 0;
	if (p->outFaults.Net.bit.BvT & p->outDefects.Net.bit.UvT) p->outDefects.Net.bit.UvT = 0;
	if (prevStatus != g_Ram.ramGroupA.Status.bit.Stop)										// По переходу из "движения" в "стоп" и обратно
	{
		prevStatus = g_Ram.ramGroupA.Status.bit.Stop;										// переинициализируем защиту по обрыву питающих фаз как аварию или как неисправность
		if (g_Ram.ramGroupA.Status.bit.Stop == 1)		// Если в СТОПе
		{
			p->breakVoltR.Cfg.bit.CanBeReseted = CAN_NOT_BE_RESETED;
			p->breakVoltS.Cfg.bit.CanBeReseted = CAN_NOT_BE_RESETED;
			p->breakVoltT.Cfg.bit.CanBeReseted = CAN_NOT_BE_RESETED;

			p->breakVoltR.Output 			= (Uns *)&p->outFaults.Net.all;
			p->breakVoltS.Output 			= (Uns *)&p->outFaults.Net.all;
			p->breakVoltT.Output 	 		= (Uns *)&p->outFaults.Net.all;

			p->breakVoltR.EnableLevel  		= (Int *)&g_Ram.ramGroupC.BvLevel;
			p->breakVoltS.EnableLevel  		= (Int *)&g_Ram.ramGroupC.BvLevel;
			p->breakVoltT.EnableLevel  		= (Int *)&g_Ram.ramGroupC.BvLevel;

			if (p->outDefects.Net.all & NET_BV_MASK) // Если имеется неисправность по обрыву, превращаем неисправность в аварию
			{
				p->outFaults.Net.all = (p->outFaults.Net.all & ~ NET_BV_MASK) | (p->outDefects.Net.all & NET_BV_MASK);
				p->outDefects.Net.all &=  ~NET_BV_MASK;
				p->registerBrVolt = 0;
			}
		}
		else
		{
			p->breakVoltR.Cfg.bit.CanBeReseted = CAN_BE_RESETED;
			p->breakVoltS.Cfg.bit.CanBeReseted = CAN_BE_RESETED;
			p->breakVoltT.Cfg.bit.CanBeReseted = CAN_BE_RESETED;

			//p->breakVoltR.Output 			= (Uns *)&p->outDefects.Net.all;
			//p->breakVoltS.Output 			= (Uns *)&p->outDefects.Net.all;
			//p->breakVoltT.Output 	 		= (Uns *)&p->outDefects.Net.all;
			p->breakVoltR.Output 			= (Uns *)&p->registerBrVolt;
			p->breakVoltS.Output 			= (Uns *)&p->registerBrVolt;
			p->breakVoltT.Output 	 		= (Uns *)&p->registerBrVolt;

			p->breakVoltR.EnableLevel  		= (Int *)&g_Ram.ramGroupC.BvLevelMove;
			p->breakVoltS.EnableLevel  		= (Int *)&g_Ram.ramGroupC.BvLevelMove;
			p->breakVoltT.EnableLevel  		= (Int *)&g_Ram.ramGroupC.BvLevelMove;
		}
	}

    Core_ProtecionSHC_Update(&p->ShC_U);
    Core_ProtecionSHC_Update(&p->ShC_V);
    Core_ProtecionSHC_Update(&p->ShC_W);

    p->outFaults.Proc.bit.Mufta = p->MuffFlag;

    if (p->outDefects.Dev.all || p->outDefects.Load.all || p->outDefects.Net.all || p->outDefects.Proc.all)
	{
	    g_Core.Status.bit.Defect = 1;
	}
    else
	g_Core.Status.bit.Defect = 0;
    if (p->outFaults.Dev.all || p->outFaults.Load.all || p->outFaults.Net.all || p->outFaults.Proc.all)
	{
	    g_Core.Status.bit.Fault = 1;
	}
    else
	g_Core.Status.bit.Fault = 0;

    if (!g_Core.Status.bit.Stop)
	{
	    if (p->NoMoveFlag)
		{
		    if (!g_Core.MotorControl.RequestDir)
			MuffEnable = 1;
		    else if (g_Core.MotorControl.RequestDir > 0)
			MuffEnable = !g_Core.Status.bit.Opened;
		    else if (g_Core.MotorControl.RequestDir < 0)
			MuffEnable = !g_Core.Status.bit.Closed;

		    p->outFaults.Proc.bit.NoMove = MuffEnable;

		    Core_ValveDriveStop(&g_Core.VlvDrvCtrl);
		    g_Core.VlvDrvCtrl.EvLog.Value = CMD_DEFSTOP;
		}
	    if (g_Core.Status.bit.Fault)
		{
		    Core_ValveDriveStop(&g_Core.VlvDrvCtrl);
		    g_Core.VlvDrvCtrl.EvLog.Value = CMD_DEFSTOP;
		}
	}

}
