/*======================================================================
Имя файла:          core_Protections.h
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		25.04.2014
Описание:
модуль защит электропривода
======================================================================*/

#include "core.h"


void Core_ProtectionsInit(TCoreProtections *p)
{
	//---------ЗАЩИТЫ ПО НАПРЯЖЕНИЮ---------------------------------------
	//---------Пониженное напряжение---------------------------------------
	p->underVoltageR.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,UvR_bit,20);
	p->underVoltageS.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,UvS_bit,20);
	p->underVoltageT.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,UvT_bit,20);

	p->underVoltageR.Input 			= (Int *)&g_Ram.ramGroupH.Ur;
	p->underVoltageS.Input 			= (Int *)&g_Ram.ramGroupH.Us;
	p->underVoltageT.Input 			= (Int *)&g_Ram.ramGroupH.Ut;

	p->underVoltageR.Output 		= (Uns *)&p->outFaults.Net.all;
	p->underVoltageS.Output 		= (Uns *)&p->outFaults.Net.all;
	p->underVoltageT.Output 	 	= (Uns *)&p->outFaults.Net.all;

	p->underVoltageR.EnableLevel  	= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underVoltageS.EnableLevel  	= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underVoltageT.EnableLevel  	= (Int *)&g_Ram.ramGroupC.UvLevel;

	p->underVoltageR.DisableLevel 	= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underVoltageS.DisableLevel 	= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underVoltageT.DisableLevel 	= (Int *)&g_Ram.ramGroupC.UvLevel;

	p->underVoltageR.Timeout		= &g_Ram.ramGroupC.UvTime;
	p->underVoltageS.Timeout		= &g_Ram.ramGroupC.UvTime;
	p->underVoltageT.Timeout		= &g_Ram.ramGroupC.UvTime;

	p->underVoltageR.Scale 			= PROTECT_SCALE;
	p->underVoltageS.Scale 			= PROTECT_SCALE;
	p->underVoltageT.Scale 			= PROTECT_SCALE;

	//---------Повышенное напряжение------------------------------------------
	p->overVoltageR.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvR_bit,20);
	p->overVoltageS.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvS_bit,20);
	p->overVoltageT.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvT_bit,20);

	p->overVoltageR.Input 			= (Int *)&g_Ram.ramGroupH.Ur;
	p->overVoltageS.Input 			= (Int *)&g_Ram.ramGroupH.Us;
	p->overVoltageT.Input 			= (Int *)&g_Ram.ramGroupH.Ut;

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

	//---------Повышенное напряжение на 47% ------------------------------------------
	p->overVoltageR.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvR_max_bit,20);
	p->overVoltageS.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvS_max_bit,20);
	p->overVoltageT.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvT_max_bit,20);

	p->overVoltageR.Input 			= (Int *)&g_Ram.ramGroupH.Ur;
	p->overVoltageS.Input 			= (Int *)&g_Ram.ramGroupH.Us;
	p->overVoltageT.Input 			= (Int *)&g_Ram.ramGroupH.Ut;

	p->overVoltageR.Output 			= (Uns *)&p->outFaults.Net.all;
	p->overVoltageS.Output 			= (Uns *)&p->outFaults.Net.all;
	p->overVoltageT.Output 	 		= (Uns *)&p->outFaults.Net.all;

	p->overVoltageR.EnableLevel  	= (Int *)&g_Ram.ramGroupC.OvLevel_max;
	p->overVoltageS.EnableLevel  	= (Int *)&g_Ram.ramGroupC.OvLevel_max;
	p->overVoltageT.EnableLevel  	= (Int *)&g_Ram.ramGroupC.OvLevel_max;

	p->overVoltageR.DisableLevel 	= (Int *)&g_Ram.ramGroupC.OvLevel_max;
	p->overVoltageS.DisableLevel 	= (Int *)&g_Ram.ramGroupC.OvLevel_max;
	p->overVoltageT.DisableLevel 	= (Int *)&g_Ram.ramGroupC.OvLevel_max;

	p->overVoltageR.Timeout			= &g_Ram.ramGroupC.OvTime_max;
	p->overVoltageS.Timeout			= &g_Ram.ramGroupC.OvTime_max;
	p->overVoltageT.Timeout			= &g_Ram.ramGroupC.OvTime_max;

	p->overVoltageR.Scale 			= PROTECT_SCALE;
	p->overVoltageS.Scale 			= PROTECT_SCALE;
	p->overVoltageT.Scale 			= PROTECT_SCALE;

	//---------Обрыв фаз напряжения (R S T)-----------------------
	p->breakVoltR.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,BvR_bit,20);
	p->breakVoltS.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,BvS_bit,20);
	p->breakVoltT.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,BvT_bit,20);

	p->breakVoltR.Input 			= (Int *)&g_Ram.ramGroupH.Ur;
	p->breakVoltS.Input 			= (Int *)&g_Ram.ramGroupH.Us;
	p->breakVoltT.Input 			= (Int *)&g_Ram.ramGroupH.Ut;

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

	//---------Ассиметрия напряжений-----------------------------------
	p->voltSkew.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,VSk_bit,20);
	p->voltSkew.Input				= (Int *)&g_Ram.ramGroupH.VSkValue;
	p->voltSkew.Output				= (Uns *)&p->outFaults.Net.all;
	p->voltSkew.EnableLevel			= (Int *)&g_Ram.ramGroupC.VSkLevel;
	p->voltSkew.DisableLevel		= (Int *)&g_Ram.ramGroupC.VSkLevel;
	p->voltSkew.Timeout				= &g_Ram.ramGroupC.VSkTime;
	p->voltSkew.Scale				= PROTECT_SCALE;

	//---------ЗАЩИТЫ ПО ТОКУ---------------------------------------------
	//---------Обрыв фаз по току (U V W)---------------------------------
	p->breakCurrU.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,BvR_bit,0);
	p->breakCurrV.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,BvS_bit,0);
	p->breakCurrW.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,BvT_bit,0);

	p->breakCurrU.Input 			= (Int *)&g_Ram.ramGroupH.Iu;
	p->breakCurrV.Input 			= (Int *)&g_Ram.ramGroupH.Iv;
	p->breakCurrW.Input 			= (Int *)&g_Ram.ramGroupH.Iw;

	p->breakCurrU.Output 			= (Uns *)&p->outFaults.Load.all;
	p->breakCurrV.Output 			= (Uns *)&p->outFaults.Load.all;
	p->breakCurrW.Output 	 		= (Uns *)&p->outFaults.Load.all;

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

	//---------Время-токовая перегрузка-------------------------------------
	p->I2t.Cfg.bit.Hyst 			= 0;
	p->I2t.Cfg.bit.Num 				= I2t_bit;
	p->I2t.InputCurrentMid 			= &g_Ram.ramGroupH.Imid;
	p->I2t.NomCurrent 				= &g_Ram.ramGroupC.Inom;
	p->I2t.Output 					= (Uns *)&p->outFaults.Load.all;
	p->I2t.Scale 					= PROTECT_SCALE;

	//--------Ассиметрия токов----------------------------------------------
	p->voltSkew.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,ISkew_bit,0);
	p->voltSkew.Input				= (Int *)&g_Ram.ramGroupH.ISkewValue;
	p->voltSkew.Output				= (Uns *)&p->outFaults.Load.all;
	p->voltSkew.EnableLevel			= (Int *)&g_Ram.ramGroupC.ISkewLevel;
	p->voltSkew.DisableLevel		= (Int *)&g_Ram.ramGroupC.ISkewLevel;
	p->voltSkew.Timeout				= &g_Ram.ramGroupC.ISkewTime;
	p->voltSkew.Scale				= PROTECT_SCALE;

	//------Короткое замыкание----------------------------------------------
	p->ShC_U.Cfg.bit.Num 			= ShCU_bit;
	p->ShC_V.Cfg.bit.Num 			= ShCV_bit;
	p->ShC_W.Cfg.bit.Num 			= ShCW_bit;

	p->ShC_U.Cfg.bit.Hyst 			= 0;
	p->ShC_V.Cfg.bit.Hyst 			= 0;
	p->ShC_W.Cfg.bit.Hyst 			= 0;

	p->ShC_U.InputCurrent			= (Uns *)&ADC_IU;
	p->ShC_V.InputCurrent			= (Uns *)&ADC_IV;
	p->ShC_W.InputCurrent			= (Uns *)&ADC_IW;

	p->ShC_U.HighCurrentLevel		= &g_Ram.ramGroupC.ShC_Up;
	p->ShC_V.HighCurrentLevel		= &g_Ram.ramGroupC.ShC_Up;
	p->ShC_W.HighCurrentLevel		= &g_Ram.ramGroupC.ShC_Up;

	p->ShC_U.LowCurrentLevel		= &g_Ram.ramGroupC.ShC_Down;
	p->ShC_V.LowCurrentLevel		= &g_Ram.ramGroupC.ShC_Down;
	p->ShC_W.LowCurrentLevel		= &g_Ram.ramGroupC.ShC_Down;

	p->ShC_U.Output					= &p->outFaults.Load.all;
	p->ShC_V.Output					= &p->outFaults.Load.all;
	p->ShC_W.Output					= &p->outFaults.Load.all;

	//------ДИАГНОСТИКА УСТРОЙСТВА------------------------------------------
	//------Перегрев блока БКД----------------------------------------------
	p->overHeatBCD.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,Th_BCD_bit,0);
	p->overHeatBCD.Input			= &g_Ram.ramGroupA.Temper;
	p->overHeatBCD.Output			= &p->outFaults.Dev.all;
	p->overHeatBCD.EnableLevel		= &g_Ram.ramGroupC.TemperHigh;
	p->overHeatBCD.DisableLevel		= &g_Ram.ramGroupC.TemperHigh;
	p->overHeatBCD.Timeout			= 0;
	p->overHeatBCD.Scale			= PROTECT_SCALE;

	//------Переохлаждение блока БКД----------------------------------------
	p->overHeatBCD.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,Tl_BCD_bit,0);
	p->overHeatBCD.Input			= &g_Ram.ramGroupA.Temper;
	p->overHeatBCD.Output			= &p->outFaults.Dev.all;
	p->overHeatBCD.EnableLevel		= &g_Ram.ramGroupC.TemperLow;
	p->overHeatBCD.DisableLevel		= &g_Ram.ramGroupC.TemperLow;
	p->overHeatBCD.Timeout			= 0;
	p->overHeatBCD.Scale			= PROTECT_SCALE;

	//------Перегрев блока БКП----------------------------------------------
	p->overHeatBCD.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,Th_BCP_bit,0);
	p->overHeatBCD.Input			= &g_Ram.ramGroupA.Temper;
	p->overHeatBCD.Output			= &p->outFaults.Dev.all;
	p->overHeatBCD.EnableLevel		= &g_Ram.ramGroupC.TemperHigh;
	p->overHeatBCD.DisableLevel		= &g_Ram.ramGroupC.TemperHigh;
	p->overHeatBCD.Timeout			= 0;
	p->overHeatBCD.Scale			= PROTECT_SCALE;

	//------Переохлаждение блока БКП----------------------------------------
	p->overHeatBCD.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,Tl_BCP_bit,0);
	p->overHeatBCD.Input			= &g_Ram.ramGroupA.Temper;
	p->overHeatBCD.Output			= &p->outFaults.Dev.all;
	p->overHeatBCD.EnableLevel		= &g_Ram.ramGroupC.TemperLow;
	p->overHeatBCD.DisableLevel		= &g_Ram.ramGroupC.TemperLow;
	p->overHeatBCD.Timeout			= 0;
	p->overHeatBCD.Scale			= PROTECT_SCALE;

}

void Core_ProtectionsReset(TCoreProtections *p)
{
	return;
}
