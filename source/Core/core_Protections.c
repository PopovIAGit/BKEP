/*======================================================================
 Èìÿ ôàéëà:          core_Protections.h
 Àâòîð:				Ïîïîâ È.À.
 Âåðñèÿ ôàéëà:		1.01
 Äàòà èçìåíåíèÿ:		10.06.2015
 Îïèñàíèå:
 ìîäóëü çàùèò ýëåêòðîïðèâîäà
 ======================================================================*/

#include "core.h"
#include "peref.h"

Int Temper110 = 110;
Int Temper90  = 90;
Int TemperM10 = -10;
Int TemperM40 = -40;

Uns tmpTime=0;
Uns FlagDisconect = 0;

//ToDo äîáàâèòü íåò äâèæåíèÿ êàê àâàðèþ âñòðîåííîãî äàò÷èêà ïîëîæåíèÿ

void Core_ProtectionsInit(TCoreProtections *p)
{
	//---------ÇÀÙÈÒÛ ÏÐÎÖÅÑÑÀ--------------------------------------------------------
	//---------Ìóôòà--------------------------------------------------------

	// Ìóôòà âûñòàâëÿåòñÿ â core.c

	//---------Íåò Äâèæåííèÿ-----------------------------------------------

	p->NoMove.Cfg.all = PRT_CFG_SET(CAN_BE_MUFTA, INP_LESS_LEVEL, NoMove_bit, HYST_OFF);
	p->NoMove.Input = (Int *) &g_Ram.ramGroupA.Position;
	p->NoMove.Output = &p->NoMoveFlag;
	p->NoMove.EnableLevel = (Int *) &g_Ram.ramGroupC.MuffZone;
	p->NoMove.DisableLevel = (Int *) &g_Ram.ramGroupC.MuffZone;
	p->NoMove.Timeout = &g_Ram.ramGroupB.NoMoveTime;
	p->NoMove.Scale = PROTECT_SCALE;
	p->NoMove.Signal = 0;

	//---------ÇÀÙÈÒÛ ÏÎ ÍÀÏÐßÆÅÍÈÞ---------------------------------------
	//---------Ïîíèæåííîå íàïðÿæåíèå (àâàðèÿ)---------------------------------------

	p->underVoltageR.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, UvR_bit, 20);
	p->underVoltageS.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, UvS_bit, 20);
	p->underVoltageT.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, UvT_bit, 20);

	p->underVoltageR.Input = (Int *) &g_Ram.ramGroupA.Ur;
	p->underVoltageS.Input = (Int *) &g_Ram.ramGroupA.Us;
	p->underVoltageT.Input = (Int *) &g_Ram.ramGroupA.Ut;

	p->underVoltageR.Output = (Uns *) &p->outFaults.Net.all;
	p->underVoltageS.Output = (Uns *) &p->outFaults.Net.all;
	p->underVoltageT.Output = (Uns *) &p->outFaults.Net.all;

	p->underVoltageR.EnableLevel = (Int *) &g_Ram.ramGroupC.UvLevel;
	p->underVoltageS.EnableLevel = (Int *) &g_Ram.ramGroupC.UvLevel;
	p->underVoltageT.EnableLevel = (Int *) &g_Ram.ramGroupC.UvLevel;

	p->underVoltageR.DisableLevel = (Int *) &g_Ram.ramGroupC.UvLevel;
	p->underVoltageS.DisableLevel = (Int *) &g_Ram.ramGroupC.UvLevel;
	p->underVoltageT.DisableLevel = (Int *) &g_Ram.ramGroupC.UvLevel;

	p->underVoltageR.Timeout = &g_Ram.ramGroupC.UvTime;
	p->underVoltageS.Timeout = &g_Ram.ramGroupC.UvTime;
	p->underVoltageT.Timeout = &g_Ram.ramGroupC.UvTime;

	p->underVoltageR.Scale = PROTECT_SCALE;
	p->underVoltageS.Scale = PROTECT_SCALE;
	p->underVoltageT.Scale = PROTECT_SCALE;

	//---------Ïîíèæåííîå íàïðÿæåíèå (íåèñïðàâíîñòü)---------------------------------------

	p->underDefVoltageR.Cfg.all = PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL, UvR_bit, 20);
	p->underDefVoltageS.Cfg.all = PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL, UvS_bit, 20);
	p->underDefVoltageT.Cfg.all = PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL, UvT_bit, 20);

	p->underDefVoltageR.Input = (Int *) &g_Ram.ramGroupA.Ur;
	p->underDefVoltageS.Input = (Int *) &g_Ram.ramGroupA.Us;
	p->underDefVoltageT.Input = (Int *) &g_Ram.ramGroupA.Ut;

	p->underDefVoltageR.Output = (Uns *) &p->outDefects.Net.all;
	p->underDefVoltageS.Output = (Uns *) &p->outDefects.Net.all;
	p->underDefVoltageT.Output = (Uns *) &p->outDefects.Net.all;

	p->underDefVoltageR.EnableLevel = (Int *) &g_Ram.ramGroupC.UvLevel;
	p->underDefVoltageS.EnableLevel = (Int *) &g_Ram.ramGroupC.UvLevel;
	p->underDefVoltageT.EnableLevel = (Int *) &g_Ram.ramGroupC.UvLevel;

	p->underDefVoltageR.DisableLevel = (Int *) &g_Ram.ramGroupC.UvLevel;
	p->underDefVoltageS.DisableLevel = (Int *) &g_Ram.ramGroupC.UvLevel;
	p->underDefVoltageT.DisableLevel = (Int *) &g_Ram.ramGroupC.UvLevel;

	p->underDefVoltageR.Timeout = &g_Ram.ramGroupC.UvDTime;
	p->underDefVoltageS.Timeout = &g_Ram.ramGroupC.UvDTime;
	p->underDefVoltageT.Timeout = &g_Ram.ramGroupC.UvDTime;

	p->underDefVoltageR.Scale = PROTECT_SCALE;
	p->underDefVoltageS.Scale = PROTECT_SCALE;
	p->underDefVoltageT.Scale = PROTECT_SCALE;

	//---------Ïîâûøåííîå íàïðÿæåíèå (àâàðèÿ)------------------------------------------
	p->overVoltageR.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, OvR_bit, 20);
	p->overVoltageS.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, OvS_bit, 20);
	p->overVoltageT.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, OvT_bit, 20);

	p->overVoltageR.Input = (Int *) &g_Ram.ramGroupA.Ur;
	p->overVoltageS.Input = (Int *) &g_Ram.ramGroupA.Us;
	p->overVoltageT.Input = (Int *) &g_Ram.ramGroupA.Ut;

	p->overVoltageR.Output = (Uns *) &p->outFaults.Net.all;
	p->overVoltageS.Output = (Uns *) &p->outFaults.Net.all;
	p->overVoltageT.Output = (Uns *) &p->outFaults.Net.all;

	p->overVoltageR.EnableLevel = (Int *) &g_Ram.ramGroupC.OvLevel;
	p->overVoltageS.EnableLevel = (Int *) &g_Ram.ramGroupC.OvLevel;
	p->overVoltageT.EnableLevel = (Int *) &g_Ram.ramGroupC.OvLevel;

	p->overVoltageR.DisableLevel = (Int *) &g_Ram.ramGroupC.OvLevel;
	p->overVoltageS.DisableLevel = (Int *) &g_Ram.ramGroupC.OvLevel;
	p->overVoltageT.DisableLevel = (Int *) &g_Ram.ramGroupC.OvLevel;

	p->overVoltageR.Timeout = &g_Ram.ramGroupC.OvTime;
	p->overVoltageS.Timeout = &g_Ram.ramGroupC.OvTime;
	p->overVoltageT.Timeout = &g_Ram.ramGroupC.OvTime;

	p->overVoltageR.Scale = PROTECT_SCALE;
	p->overVoltageS.Scale = PROTECT_SCALE;
	p->overVoltageT.Scale = PROTECT_SCALE;

	//---------Ïîâûøåííîå íàïðÿæåíèå (íåèñïðàâíîñòü)------------------------------------------
	p->overDefVoltageR.Cfg.all = PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL, OvR_bit, 20);
	p->overDefVoltageS.Cfg.all = PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL, OvS_bit, 20);
	p->overDefVoltageT.Cfg.all = PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL, OvT_bit, 20);

	p->overDefVoltageR.Input = (Int *) &g_Ram.ramGroupA.Ur;
	p->overDefVoltageS.Input = (Int *) &g_Ram.ramGroupA.Us;
	p->overDefVoltageT.Input = (Int *) &g_Ram.ramGroupA.Ut;

	p->overDefVoltageR.Output = (Uns *) &p->outDefects.Net.all;
	p->overDefVoltageS.Output = (Uns *) &p->outDefects.Net.all;
	p->overDefVoltageT.Output = (Uns *) &p->outDefects.Net.all;

	p->overDefVoltageR.EnableLevel = (Int *) &g_Ram.ramGroupC.OvLevel;
	p->overDefVoltageS.EnableLevel = (Int *) &g_Ram.ramGroupC.OvLevel;
	p->overDefVoltageT.EnableLevel = (Int *) &g_Ram.ramGroupC.OvLevel;

	p->overDefVoltageR.DisableLevel = (Int *) &g_Ram.ramGroupC.OvLevel;
	p->overDefVoltageS.DisableLevel = (Int *) &g_Ram.ramGroupC.OvLevel;
	p->overDefVoltageT.DisableLevel = (Int *) &g_Ram.ramGroupC.OvLevel;

	p->overDefVoltageR.Timeout = &g_Ram.ramGroupC.OvDTime;
	p->overDefVoltageS.Timeout = &g_Ram.ramGroupC.OvDTime;
	p->overDefVoltageT.Timeout = &g_Ram.ramGroupC.OvDTime;

	p->overDefVoltageR.Scale = PROTECT_SCALE;
	p->overDefVoltageS.Scale = PROTECT_SCALE;
	p->overDefVoltageT.Scale = PROTECT_SCALE;

	//---------Ïîâûøåííîå íàïðÿæåíèå íà 47% (ÀÂÀÐÈß) ------------------------------------------
	p->overMax_VoltageR.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, OvR_max_bit, 20);
	p->overMax_VoltageS.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, OvS_max_bit, 20);
	p->overMax_VoltageT.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, OvT_max_bit, 20);

	p->overMax_VoltageR.Input = (Int *) &g_Ram.ramGroupA.Ur;
	p->overMax_VoltageS.Input = (Int *) &g_Ram.ramGroupA.Us;
	p->overMax_VoltageT.Input = (Int *) &g_Ram.ramGroupA.Ut;

	p->overMax_VoltageR.Output = (Uns *) &p->outFaults.Net.all;
	p->overMax_VoltageS.Output = (Uns *) &p->outFaults.Net.all;
	p->overMax_VoltageT.Output = (Uns *) &p->outFaults.Net.all;

	p->overMax_VoltageR.EnableLevel = (Int *) &g_Ram.ramGroupC.OvLevel_max;
	p->overMax_VoltageS.EnableLevel = (Int *) &g_Ram.ramGroupC.OvLevel_max;
	p->overMax_VoltageT.EnableLevel = (Int *) &g_Ram.ramGroupC.OvLevel_max;

	p->overMax_VoltageR.DisableLevel = (Int *) &g_Ram.ramGroupC.OvLevel_max;
	p->overMax_VoltageS.DisableLevel = (Int *) &g_Ram.ramGroupC.OvLevel_max;
	p->overMax_VoltageT.DisableLevel = (Int *) &g_Ram.ramGroupC.OvLevel_max;

	p->overMax_VoltageR.Timeout = &g_Ram.ramGroupC.OvTime_max;
	p->overMax_VoltageS.Timeout = &g_Ram.ramGroupC.OvTime_max;
	p->overMax_VoltageT.Timeout = &g_Ram.ramGroupC.OvTime_max;

	p->overMax_VoltageR.Scale = PROTECT_SCALE;
	p->overMax_VoltageS.Scale = PROTECT_SCALE;
	p->overMax_VoltageT.Scale = PROTECT_SCALE;

	//---------Îáðûâ ôàç íàïðÿæåíèÿ (R S T  àâàðèÿ)-----------------------
	p->breakVoltR.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, BvR_bit, 20);
	p->breakVoltS.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, BvS_bit, 20);
	p->breakVoltT.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL, BvT_bit, 20);

	p->breakVoltR.Input = (Int *) &g_Ram.ramGroupA.Ur;
	p->breakVoltS.Input = (Int *) &g_Ram.ramGroupA.Us;
	p->breakVoltT.Input = (Int *) &g_Ram.ramGroupA.Ut;

	p->breakVoltR.Output = (Uns *) &p->outFaults.Net.all;
	p->breakVoltS.Output = (Uns *) &p->outFaults.Net.all;
	p->breakVoltT.Output = (Uns *) &p->outFaults.Net.all;

	p->breakVoltR.EnableLevel = (Int *) &g_Ram.ramGroupC.BvLevel;
	p->breakVoltS.EnableLevel = (Int *) &g_Ram.ramGroupC.BvLevel;
	p->breakVoltT.EnableLevel = (Int *) &g_Ram.ramGroupC.BvLevel;

	p->breakVoltR.DisableLevel = (Int *) &g_Ram.ramGroupC.BvLevel;
	p->breakVoltS.DisableLevel = (Int *) &g_Ram.ramGroupC.BvLevel;
	p->breakVoltT.DisableLevel = (Int *) &g_Ram.ramGroupC.BvLevel;

	p->breakVoltR.Timeout = &g_Ram.ramGroupC.BvTime;
	p->breakVoltS.Timeout = &g_Ram.ramGroupC.BvTime;
	p->breakVoltT.Timeout = &g_Ram.ramGroupC.BvTime;

	p->breakVoltR.Scale = PROTECT_SCALE;
	p->breakVoltS.Scale = PROTECT_SCALE;
	p->breakVoltT.Scale = PROTECT_SCALE;
	//---------ÇÀÙÈÒÛ ÏÎ ÒÎÊÓ---------------------------------------------
	//---------Îáðûâ ôàç ïî òîêó (U V W íåèñïðàâíîñòü)---------------------------------
	p->breakCurrU.Cfg.all = PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL, PhlU_bit, HYST_OFF);
	p->breakCurrV.Cfg.all = PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL, PhlV_bit, HYST_OFF);
	p->breakCurrW.Cfg.all = PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL, PhlW_bit, HYST_OFF);

	p->breakCurrU.Input = (Int *) &g_Ram.ramGroupH.IuPr;
	p->breakCurrV.Input = (Int *) &g_Ram.ramGroupH.IvPr;
	p->breakCurrW.Input = (Int *) &g_Ram.ramGroupH.IwPr;

	//p->breakCurrU.Output 			= (Uns *)&p->outDefects.Load.all;
	//p->breakCurrV.Output 			= (Uns *)&p->outDefects.Load.all;
	//p->breakCurrW.Output 	 		= (Uns *)&p->outDefects.Load.all;
	p->breakCurrU.Output = (Uns *) &p->registerBrCurr;
	p->breakCurrV.Output = (Uns *) &p->registerBrCurr;
	p->breakCurrW.Output = (Uns *) &p->registerBrCurr;

	p->breakCurrU.EnableLevel = (Int *) &g_Ram.ramGroupC.PhlLevel;
	p->breakCurrV.EnableLevel = (Int *) &g_Ram.ramGroupC.PhlLevel;
	p->breakCurrW.EnableLevel = (Int *) &g_Ram.ramGroupC.PhlLevel;

	p->breakCurrU.DisableLevel = (Int *) &g_Ram.ramGroupC.PhlLevel;
	p->breakCurrV.DisableLevel = (Int *) &g_Ram.ramGroupC.PhlLevel;
	p->breakCurrW.DisableLevel = (Int *) &g_Ram.ramGroupC.PhlLevel;

	p->breakCurrU.Timeout = &g_Ram.ramGroupC.PhlTime;
	p->breakCurrV.Timeout = &g_Ram.ramGroupC.PhlTime;
	p->breakCurrW.Timeout = &g_Ram.ramGroupC.PhlTime;

	p->breakCurrU.Scale = PROTECT_SCALE;
	p->breakCurrV.Scale = PROTECT_SCALE;
	p->breakCurrW.Scale = PROTECT_SCALE;

	//---------Âðåìÿ-òîêîâàÿ ïåðåãðóçêà (àâàðèÿ)-------------------------------------
	p->I2t.Cfg.bit.Hyst = HYST_OFF;
	p->I2t.Cfg.bit.Num = I2t_bit;
	p->I2t.InputCurrentMid = &g_Ram.ramGroupH.Imid;
	p->I2t.NomCurrent = &g_Ram.ramGroupC.Inom;
	p->I2t.Output = (Uns *) &p->outFaults.Load.all;
	p->I2t.Scale = 50;//PROTECT_SCALE;
	Core_ProtectionI2TInit(&p->I2t);

	//------Êîðîòêîå çàìûêàíèå----------------------------------------------
	p->ShC_U.Cfg.bit.Num = ShCU_bit;
	p->ShC_V.Cfg.bit.Num = ShCV_bit;
	p->ShC_W.Cfg.bit.Num = ShCW_bit;

	p->ShC_U.Cfg.bit.Hyst = HYST_OFF;
	p->ShC_V.Cfg.bit.Hyst = HYST_OFF;
	p->ShC_W.Cfg.bit.Hyst = HYST_OFF;

	p->ShC_U.InputCurrent = (Uns *) &ADC_IU;
	p->ShC_V.InputCurrent = (Uns *) &ADC_IV;
	p->ShC_W.InputCurrent = (Uns *) &ADC_IW;

	p->ShC_U.HighCurrentLevel = &g_Ram.ramGroupC.ShC_Up;
	p->ShC_V.HighCurrentLevel = &g_Ram.ramGroupC.ShC_Up;
	p->ShC_W.HighCurrentLevel = &g_Ram.ramGroupC.ShC_Up;

	p->ShC_U.LowCurrentLevel = &g_Ram.ramGroupC.ShC_Down;
	p->ShC_V.LowCurrentLevel = &g_Ram.ramGroupC.ShC_Down;
	p->ShC_W.LowCurrentLevel = &g_Ram.ramGroupC.ShC_Down;

	p->ShC_U.Output = &p->ShcTmpState;	//&p->outFaults.Load.all;
	p->ShC_V.Output = &p->ShcTmpState;	//&p->outFaults.Load.all;
	p->ShC_W.Output = &p->ShcTmpState;	//&p->outFaults.Load.all;

	//------ÄÈÀÃÍÎÑÒÈÊÀ ÓÑÒÐÎÉÑÒÂÀ------------------------------------------
	//------Ïåðåãðåâ áëîêà ÁÊÄ----------------------------------------------
	p->overHeatBCD.Cfg.all = PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL, Th_BCD_bit, 1);
	p->overHeatBCD.Input = &g_Ram.ramGroupA.Temper;
	p->overHeatBCD.Output = &p->outDefects.Dev.all;
	p->overHeatBCD.EnableLevel = &Temper90;	//90
	p->overHeatBCD.DisableLevel = &Temper90;
	p->overHeatBCD.Timeout = &g_Ram.ramGroupC.BvTime;
	p->overHeatBCD.Scale = PROTECT_SCALE;

	//------Ïåðåîõëàæäåíèå áëîêà ÁÊÄ----------------------------------------
	p->underColdBCD.Cfg.all = PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL, Tl_BCD_bit, 1);
	p->underColdBCD.Input = &g_Ram.ramGroupA.Temper;
	p->underColdBCD.Output = &p->outDefects.Dev.all;
	p->underColdBCD.EnableLevel = &TemperM10;
	p->underColdBCD.DisableLevel = &TemperM10;
	p->underColdBCD.Timeout = &g_Ram.ramGroupC.BvTime;
	p->underColdBCD.Scale = PROTECT_SCALE;

	//------Ïåðåãðåâ áëîêà ÁÊÏ----------------------------------------------
	p->overHeatBCP.Cfg.all = PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_GREATER_LEVEL, Th_BCP_bit, 1);
	p->overHeatBCP.Input = &g_Ram.ramGroupA.TemperBKP;
	p->overHeatBCP.Output = &p->outFaults.Dev.all;
	p->overHeatBCP.EnableLevel = &Temper110;	// 110
	p->overHeatBCP.DisableLevel = &Temper110;
	p->overHeatBCP.Timeout = &g_Ram.ramGroupC.BvTime;
	p->overHeatBCP.Scale = PROTECT_SCALE;

	//------Ïåðåîõëàæäåíèå áëîêà ÁÊÏ----------------------------------------
	p->underColdBCP.Cfg.all = PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL, Tl_BCP_bit, 1);
	p->underColdBCP.Input = &g_Ram.ramGroupA.TemperBKP;
	p->underColdBCP.Output = &p->outDefects.Dev.all;
	p->underColdBCP.EnableLevel = &TemperM40;
	p->underColdBCP.DisableLevel = &TemperM40;
	p->underColdBCP.Timeout = &g_Ram.ramGroupC.BvTime;
	p->underColdBCP.Scale = PROTECT_SCALE;

	p->FaultDelay = (Uns) (Prd10HZ * 2);
	p->registerBrCurr = 0;
	p->registerBrVolt = 0;

}

// Ôóíêöèÿ âêëþ÷åíèÿ/âûêëþ÷åíèÿ çàùèò
void Core_ProtectionsEnable(TCoreProtections *p)
{
	static Byte State = 0;
	Bool Enable;

	if (p->FaultDelay > 0)
		return;

	switch (++State)
	{
	case 1:  //Muffta
		if (g_Ram.ramGroupC.BKP91 == 0)
		p->NoMove.Cfg.bit.Enable = ((g_Core.MotorControl.WorkMode & wmMove) != 0);
		else
		{
			if (g_Ram.ramGroupA.PositionPr == 9999) p->NoMove.Cfg.bit.Enable = 0;

			else p->NoMove.Cfg.bit.Enable = ((g_Core.MotorControl.WorkMode & wmMove) != 0) && (stepMode.stepModeStatus != smsInPause);
		}
		break;
	case 2:  // Çàùèòû ïî íàðÿæåíèþ
		Enable = (g_Ram.ramGroupC.Uv != pmOff) && (!p->outDefects.Dev.bit.LowPower) && (!p->outFaults.Net.bit.BvR)&& (!p->outFaults.Net.bit.BvS)&& (!p->outFaults.Net.bit.BvT);						// Ïîíèæåèå íàïðÿæåíèÿ
		p->underVoltageR.Cfg.bit.Enable = Enable;
		p->underVoltageS.Cfg.bit.Enable = Enable;
		p->underVoltageT.Cfg.bit.Enable = Enable;
		p->underDefVoltageR.Cfg.bit.Enable = Enable;
		p->underDefVoltageS.Cfg.bit.Enable = Enable;
		p->underDefVoltageT.Cfg.bit.Enable = Enable;

		Enable = (g_Ram.ramGroupC.Ov != pmOff);						// Ïîâûøåíèå íàïðÿæåíèÿ
		p->overVoltageR.Cfg.bit.Enable = Enable;
		p->overVoltageS.Cfg.bit.Enable = Enable;
		p->overVoltageT.Cfg.bit.Enable = Enable;
		p->overDefVoltageR.Cfg.bit.Enable = Enable;
		p->overDefVoltageS.Cfg.bit.Enable = Enable;
		p->overDefVoltageT.Cfg.bit.Enable = Enable;
		p->overMax_VoltageR.Cfg.bit.Enable = Enable;
		p->overMax_VoltageS.Cfg.bit.Enable = Enable;
		p->overMax_VoltageT.Cfg.bit.Enable = Enable;

		Enable = (g_Ram.ramGroupC.Bv != pmOff) && (!p->outDefects.Dev.bit.LowPower);						// Îáðûâ ôàç íàïðÿæåíèÿ
		p->breakVoltR.Cfg.bit.Enable = Enable;
		p->breakVoltS.Cfg.bit.Enable = Enable;
		p->breakVoltT.Cfg.bit.Enable = Enable;

		break;
	case 3:  // Çàùèòû ïî òîêó
		Enable = (g_Ram.ramGroupC.Phl != pmOff) && (!g_Core.Status.bit.Stop) && (stepMode.stepModeStatus != smsInPause);	// Îáðûâ âûõîäíûõ ôàç (äâèã.)
		p->breakCurrU.Cfg.bit.Enable = Enable;
		p->breakCurrV.Cfg.bit.Enable = Enable;
		p->breakCurrW.Cfg.bit.Enable = Enable;

		//Enable = (g_Ram.ramGroupC.ShC != pmOff) && (!g_Core.Status.bit.Stop);					// Êîðîòêîå çàìûêàíèå
		Enable = (g_Ram.ramGroupC.ShC != pmOff);
		p->ShC_U.Cfg.bit.Enable = Enable;
		p->ShC_V.Cfg.bit.Enable = Enable;
		p->ShC_W.Cfg.bit.Enable = Enable;

		p->I2t.Cfg.bit.Enable = (g_Ram.ramGroupC.I2t != pmOff) /*&& (!g_Core.Status.bit.Stop)*/;		// ÂÒÇ
		break;
	case 4:  // Çàùèòû óñòðîéñòâà
		Enable = g_Ram.ramGroupC.TemperTrack != pmOff;
		p->overHeatBCD.Cfg.bit.Enable = Enable;
		p->overHeatBCP.Cfg.bit.Enable = Enable;
		p->underColdBCD.Cfg.bit.Enable = Enable;
		p->underColdBCP.Cfg.bit.Enable = Enable;

		State = 0;	// âåðíóëèñü ê èñòîêàì
		break;
	}
}


// Èíäèêàöèÿ àâàðèé ïðîöåññà è óñòðîéñòâà
void Core_DevProc_FaultIndic(TCoreProtections *p)
{
	if (g_Ram.ramGroupC.BKP91)	p->NoMove.Input = (Int *) &g_Ram.ramGroupA.PositionPr;

	if (p->FaultDelay > 0)
	{
		p->FaultDelay--;
		return;
	}

	p->outFaults.Proc.bit.Overway = g_Core.MotorControl.OverWayFlag;

	p->outDefects.Proc.all &= ~PROC_CALIB_MASK;
	if (g_Ram.ramGroupC.CalibIndic != pmOff)
	{
		if (g_Ram.ramGroupC.BKP91)
		{
			p->outDefects.Proc.bit.NoClose = !(g_Ram.ramGroupH.CalibState & csClose);
			p->outDefects.Proc.bit.NoOpen = !(g_Ram.ramGroupH.CalibState & csOpen);
			p->outDefects.Proc.bit.NoCalib = !(g_Ram.ramGroupH.CalibState & csCalib);
		}
		else
		{
		p->outDefects.Proc.bit.NoClose = !(g_Ram.ramGroupH.CalibState & csClose);
		p->outDefects.Proc.bit.NoOpen = !(g_Ram.ramGroupH.CalibState & csOpen);
		p->outDefects.Proc.bit.NoCalib = !(g_Ram.ramGroupH.CalibState & csCalib);
		}
		}

	if (g_Ram.ramGroupC.ErrIndic != pmOff)
	{
		p->outDefects.Dev.bit.Memory1 = Eeprom1.Error;
		p->outDefects.Dev.bit.Memory2 = Eeprom2.Error;
		//p->outFaults.Dev.bit.PosSens = 0;	//??? äîáàâèòü ïðè íàëè÷èè äðàéâåðà
		p->outDefects.Dev.bit.Rtc = (Uns) g_Peref.Rtc.Error;
		p->outDefects.Dev.bit.TSens = (Uns) g_Peref.TSens.Error;
		p->outDefects.Dev.bit.Dac = (Uns) g_Peref.Dac.Error;

		// Îøèáêà íåò ñâÿçè ñ ÁÊÏ
		if(g_Comm.Bluetooth.ModeProtocol != 2 && p->outFaults.Dev.bit.NoBCP_Connect == 0)
		{
			if(g_Comm.mbBkp.Frame.ConnFlagCount == 0)
			{
				if (FlagDisconect == 0)
				{
					FlagDisconect = 1;
					g_Ram.ramGroupC.CounterDisconect++;
				}

				if (g_Ram.ramGroupA.Status.bit.Stop)
				{
					if(p->BCPConTimer++ >= g_Ram.ramGroupC.BCPConTime)
					{
						p->BCPConTimer = g_Ram.ramGroupC.BCPConTime;
						p->outFaults.Dev.bit.NoBCP_Connect = 1;
					}
				}
				else if (g_Ram.ramGroupA.Status.bit.Stop == 0)
				{
					if(p->BCPConTimer++ >= g_Ram.ramGroupC.BCPConTimeMove)
					{
						p->BCPConTimer = g_Ram.ramGroupC.BCPConTimeMove;
						p->outFaults.Dev.bit.NoBCP_Connect = 1;
					}
				}
			}
			else
			{
				FlagDisconect = 0;
				p->BCPConTimer = 0;
			}

		}


	    //p->outFaults.Dev.bit.NoBCP_Connect = !g_Comm.mbBkp.Frame.ConnFlag;
		//p->outDefects.Dev.bit.NoBCP_Connect = !g_Comm.mbBkp.Frame.ConnFlag;
	}
}

void EngPhOrdPrt(TCoreProtections *p)
{
	static Bool Flag = False;          // ôëàã äëÿ åäèíè÷íîé ïðîâåðêè ïðàâ?å?ëüíîñòè ÷åðåäîâàíèÿ ôàçû
	static LgUns StartPos;
	static Uns Timer = 0;
	static Int EngPhOrdValue = 0;
	LgInt data = 16384;//(REV_MAX + 1)/2;
	LgInt Delta;

	if ((g_Ram.ramGroupC.PhOrd == pmOff) || (g_Core.Status.bit.Stop))
	{
		Flag = false;
		StartPos = g_Ram.ramGroupA.Position;
		Timer = 0;
		return;
	}

	if (Timer < (g_Ram.ramGroupC.PhOrdTime * (Uns) PROTECT_SCALE))
		Timer++;
	else if (!Flag)
	{
		Delta = g_Ram.ramGroupA.Position - StartPos;

		if (Delta > data)
			Delta -= 32768; //(LgInt)(REV_MAX + 1);
		if (Delta < -data)
			Delta += 32768; //(LgInt)(REV_MAX + 1);

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

// Ñáðîñ çàùèò ïðè ïóñêå
void Core_ProtectionsReset(TCoreProtections *p)
{
	g_Core.MotorControl.OverWayFlag = 0;		// Ñáðîñèëè îòñóñâèå óïëîòíåíèÿ
//	p->MuffFlag = 0;							// îò÷èñòèëè ñòàòóñ îò àâàðèè ìóôòû è íåèñïðàâåíîñòè
//	p->outFaults.Proc.bit.Mufta = 0;
	g_Core.Status.bit.Defect = 0;
	//g_Core.Status.bit.Fault = 0;
	//g_Core.Status.bit.Mufta = 0;
	p->NoMoveFlag = 0;

//	p->outDefects.Proc.bit.Mufta=0;
//	p->outFaults.Proc.bit.Mufta=0;

	p->outDefects.Proc.bit.NoMove=0;
//	p->outFaults.Proc.bit.NoMove=0;

	p->outDefects.Load.bit.ISkew = 0;
	p->outDefects.Load.bit.PhlU = 0;
	p->outDefects.Load.bit.PhlV = 0;
	p->outDefects.Load.bit.PhlW = 0;
	p->registerBrCurr = 0;

	p->outDefects.Proc.bit.Overway = 0;
	p->outDefects.Proc.bit.FireContErr = 0;
	p->FireContErr_Timer = 0;
//	p->outDefects.Proc.bit.NoMove = 0;
//	p->outFaults.Proc.bit.NoMove = 0;
}

// Ñáðîñ âñåõ çàùèò
void Core_ProtectionsClear(TCoreProtections *p)
{
	Uns MuffAddr, TH_BCP_addr;

	tmpTime=0;
	g_Core.MotorControl.OverWayFlag = 0;		// Ñáðîñèëè îòñóñâèå óïëîòíåíèÿ

	g_Core.Protections.MuffFlag200Hz = 0;

	g_Core.DisplayFaults.DisplFaultUnion.Dev.all = 0;
	g_Core.DisplayFaults.DisplFaultUnion.Load.all = 0;
	g_Core.DisplayFaults.DisplFaultUnion.Net.all = 0;
	g_Core.DisplayFaults.DisplFaultUnion.Proc.all = 0;

	p->MuffFlag200Hz = 0;
	p->MuffFlag = 0;
	if (g_Ram.ramGroupH.MuffFault != 0 && IsMemParReady())
	{
		g_Ram.ramGroupH.MuffFault = 0;
		MuffAddr = REG_MUFF_FAULT;
		WriteToEeprom(MuffAddr, &g_Ram.ramGroupH.MuffFault, 1);
	}

	if(g_Ram.ramGroupH.TemperBCPFault == 1 && IsMemParReady())
	{
		g_Ram.ramGroupH.TemperBCPFault = 0;
		TH_BCP_addr = REG_TEMP_BCP_FAULT;
		WriteToEeprom(TH_BCP_addr, &g_Ram.ramGroupH.TemperBCPFault, 1);
	}

	g_Core.Status.bit.Fault = 0;
	g_Core.Status.bit.Defect = 0;
	p->outFaults.Dev.bit.NoBCP_Connect = 0;
	p->BCPConTimer = 0;

	p->outFaults.Dev.all = 0;					// ñáðîñèëè âñå àâàðèè
	p->outFaults.Net.all = 0;
	p->outFaults.Load.all = 0;
	p->outFaults.Proc.all = 0;

	p->outDefects.Dev.all = 0;					// ñáðîñèëè âñå íåèñïðàâíîñòè
	p->outDefects.Net.all = 0;
	p->outDefects.Load.all = 0;
	p->registerBrCurr = 0;
	p->registerBrVolt = 0;
	p->outDefects.Proc.all = 0;


	/* ??? ñáðîñ îøèáîê ïàìÿòè ýíêîäåðà è ÄÒ
	 Encoder.Error  = False;
	 Eeprom1.Error  = False;
	 Eeprom2.Error  = False;
	 TempSens.Error = False;
	 */
	p->ShcTmpState = 0;
	p->ShcReset = true;
	p->FireContErr_Timer = 0;

}

void Core_Protections50HZUpdate(TCoreProtections *p)
{
	static Uns prevStatus = 0;

	if (p->FaultDelay > 0)
		return;

	if (p->registerBrVolt)  // Åñëè èìåþòñÿ íåèñïðàâíîñòè ïî îáðûâó ôàç ïèòàíèÿ (â äâèæåíèè)
	{
		if (p->registerBrCurr)  // Åñëè âìåñòå ñ òåì èìåþòñÿ íåèñïðàâíîñòè ïî îáðûâó ôàç äâèãàòåëÿ
		{
			p->outDefects.Net.all = (p->outDefects.Net.all & ~NET_BV_MASK) | p->registerBrVolt;  // òî âûñòàâëÿåì îáðûâ ôàç ïèòàíèÿ
			p->outDefects.Load.all &= ~LOAD_PHL_MASK;
		}
		else
			// Åñëè îáðûâà ôàç äâèãàòåëÿ íå íàáëþäàåòñÿ
			p->outDefects.Net.all &= ~NET_BV_MASK;
	}
	else					// Åñëè íåèñïðàâíîñòü ïî îáðûâó ôàç îòñóòñòâóåò
	{
		p->outDefects.Load.all = (p->outDefects.Load.all & ~LOAD_PHL_MASK) | p->registerBrCurr;
		p->outDefects.Net.all &= ~NET_BV_MASK;
	}

	if (p->outFaults.Net.bit.BvR & p->outDefects.Net.bit.UvR)
		p->outDefects.Net.bit.UvR = 0;  // Åñëè ñðàáîòàëà àâàðèÿ ïî îáðûâó, íåèñïðàâíîñòü "ïîíèæåííîå íàïð." íå âûñòàâëÿåì
	if (p->outFaults.Net.bit.BvS & p->outDefects.Net.bit.UvS)
		p->outDefects.Net.bit.UvS = 0;
	if (p->outFaults.Net.bit.BvT & p->outDefects.Net.bit.UvT)
		p->outDefects.Net.bit.UvT = 0;

	if (prevStatus != g_Ram.ramGroupA.Status.bit.Stop) // Ïî ïåðåõîäó èç "äâèæåíèÿ" â "ñòîï" è îáðàòíî
	{
		prevStatus = g_Ram.ramGroupA.Status.bit.Stop; // ïåðåèíèöèàëèçèðóåì çàùèòó ïî îáðûâó ïèòàþùèõ ôàç êàê àâàðèþ èëè êàê íåèñïðàâíîñòü
		if (g_Ram.ramGroupA.Status.bit.Stop == 1)		// Åñëè â ÑÒÎÏå
		{
			if (g_Ram.ramGroupB.StopMethod != smDynBreak)
			{
				p->breakVoltR.Cfg.bit.CanBeReseted = CAN_NOT_BE_RESETED;
				p->breakVoltS.Cfg.bit.CanBeReseted = CAN_NOT_BE_RESETED;
				p->breakVoltT.Cfg.bit.CanBeReseted = CAN_NOT_BE_RESETED;

				p->breakVoltR.Output = (Uns *) &p->outFaults.Net.all;
				p->breakVoltS.Output = (Uns *) &p->outFaults.Net.all;
				p->breakVoltT.Output = (Uns *) &p->outFaults.Net.all;

				p->breakVoltR.EnableLevel = (Int *) &g_Ram.ramGroupC.BvLevel;
				p->breakVoltS.EnableLevel = (Int *) &g_Ram.ramGroupC.BvLevel;
				p->breakVoltT.EnableLevel = (Int *) &g_Ram.ramGroupC.BvLevel;

				if (p->outDefects.Net.all & NET_BV_MASK) // Åñëè èìååòñÿ íåèñïðàâíîñòü ïî îáðûâó, ïðåâðàùàåì íåèñïðàâíîñòü â àâàðèþ
				{
					p->outFaults.Net.all =	(p->outFaults.Net.all & ~ NET_BV_MASK) | (p->outDefects.Net.all & NET_BV_MASK);
					p->outDefects.Net.all &= ~NET_BV_MASK;
					p->registerBrVolt = 0;
				}
			}
			else
			{
				if (!p->outDefects.Proc.bit.SoftStarter)
				{
					p->breakVoltR.Cfg.bit.CanBeReseted = CAN_NOT_BE_RESETED;
					p->breakVoltS.Cfg.bit.CanBeReseted = CAN_NOT_BE_RESETED;
					p->breakVoltT.Cfg.bit.CanBeReseted = CAN_NOT_BE_RESETED;

					p->breakVoltR.Output = (Uns *) &p->outFaults.Net.all;
					p->breakVoltS.Output = (Uns *) &p->outFaults.Net.all;
					p->breakVoltT.Output = (Uns *) &p->outFaults.Net.all;

					p->breakVoltR.EnableLevel =	(Int *) &g_Ram.ramGroupC.BvLevel;
					p->breakVoltS.EnableLevel = (Int *) &g_Ram.ramGroupC.BvLevel;
					p->breakVoltT.EnableLevel =	(Int *) &g_Ram.ramGroupC.BvLevel;

					if (p->outDefects.Net.all & NET_BV_MASK) // Åñëè èìååòñÿ íåèñïðàâíîñòü ïî îáðûâó, ïðåâðàùàåì íåèñïðàâíîñòü â àâàðèþ
					{
						p->outFaults.Net.all = (p->outFaults.Net.all & ~ NET_BV_MASK)| (p->outDefects.Net.all & NET_BV_MASK);
						p->outDefects.Net.all &= ~NET_BV_MASK;
						p->registerBrVolt = 0;
					}
				}
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
			p->breakVoltR.Output = (Uns *) &p->registerBrVolt;
			p->breakVoltS.Output = (Uns *) &p->registerBrVolt;
			p->breakVoltT.Output = (Uns *) &p->registerBrVolt;

			p->breakVoltR.EnableLevel = (Int *) &g_Ram.ramGroupC.BvLevelMove;
			p->breakVoltS.EnableLevel = (Int *) &g_Ram.ramGroupC.BvLevelMove;
			p->breakVoltT.EnableLevel = (Int *) &g_Ram.ramGroupC.BvLevelMove;
		}
	}
}

void Core_Protections50HZUpdate2(TCoreProtections *p)
{
	Uns BCPDriveType = 0;

	if (p->FaultDelay > 0)
		return;

	//-------- Îøèáêà ÒÈÏ ÁÊÏ ------------------------

	if (g_Ram.ramGroupA.Faults.Dev.bit.NoBCP_Connect == 0  && g_Ram.ramGroupC.DriveType != 0 && !g_Comm.bkpNotConnected && !g_Ram.ramGroupC.BKP91)
	{
		p->BcpTypeDubl = g_Ram.ramGroupH.BkpType*2;

		if ((Uns)g_Ram.ramGroupC.DriveType == 27) // ÝÏÖ-10000 Ä.12 ñ Óôèìñêèì äâèæêîì(27-é ïî ñ÷åòó) - ýòî ÝÏÖ-10000
		{
			BCPDriveType = 10;
		}
		else if ((Uns)g_Ram.ramGroupC.DriveType == 28) // ÝÏÖ-10000 Ä.12 ñ Óôèìñêèì äâèæêîì(27-é ïî ñ÷åòó) - ýòî ÝÏÖ-10000
				{
					BCPDriveType = 8;
				}
		else if ((Uns)g_Ram.ramGroupC.DriveType > 15)
		{
			BCPDriveType = (Uns)g_Ram.ramGroupC.DriveType - 15;
		}
		else if((Uns)g_Ram.ramGroupC.DriveType <= 15)
		{
			BCPDriveType = (Uns)g_Ram.ramGroupC.DriveType;
		}

		if (BCPDriveType == 14) BCPDriveType = 15;

		if ((p->BcpTypeDubl != BCPDriveType) && ((p->BcpTypeDubl-1) != BCPDriveType))
		{
			if(p->BcpTypeTimer++ >= 5*Prd50HZ)
			{
				p->outFaults.Dev.bit.BCP_ErrorType = 1;
				p->BcpTypeTimer = 0;
			}
		}
		else p->BcpTypeTimer = 0;
	}
		//----------------------------------------
		//------------ Îøèáêà ÓÏÏ -------------------------------

		if (g_Ram.ramGroupB.StopMethod == smDynBreak)
		{
			if ((g_Ram.ramGroupATS.State1.all & 0xFF) == 0x27)
			{
				p->MoveOnFlag = 1; // ôëàã ÷òî çàïóñòèëèñü
			}

			// Åñëè ñ ÓÏÏ ïðèøåë áèò "àâàðèÿ" && Âèñèò ôëàã, ÷òî äâèæåíèå íà÷àòî && Ñâÿçü ñ ÓÏÏ ïðèñóòñòâóåò
			if(g_Ram.ramGroupATS.State1.bit.Malfunction == 1 && p->MoveOnFlag == 1 && g_Comm.mbShn.Stat.Status.bit.NoConnect == 0)
			{
				//
				if(p->outDefects.Load.all & LOAD_PHL_MASK && p->MoveOnFlag)
				{
					p->outDefects.Proc.bit.SoftStarter = 1;

					if (p->SoftStarterTimer2++ >= 20 * Prd50HZ)
					{
						p->outDefects.Proc.bit.SoftStarter = 0;
						p->outFaults.Proc.bit.SoftStarter = 1;
						p->SoftStarterTimer2 = 0;
					}
				}

				if ((p->outDefects.Net.all & NET_UV_MASK || p->registerBrVolt)&& p->MoveOnFlag)
				{
					p->outDefects.Proc.bit.SoftStarter = 1;
					p->VoltErrFlag = 1;
					if (p->SoftStarterTimer++ >= 20 * Prd50HZ)
					{
						p->outDefects.Proc.bit.SoftStarter = 0;
						p->outFaults.Proc.bit.SoftStarter = 1;
						p->SoftStarterTimer = 0;
					}
				}
				else if (((p->outDefects.Net.all & NET_UV_MASK) == 0 || p->registerBrVolt == 0)&& p->MoveOnFlag)
				{
					if(p->VoltErrFlag == 1 && g_Comm.mbShn.Stat.Status.bit.NoConnect == 0 && g_Ram.ramGroupA.Status.bit.Fault == 0 && p->MoveOnFlag == 1 && p->outDefects.Proc.bit.SoftStarter == 1)
					{
						p->outDefects.Proc.bit.SoftStarter = 0;
						p->SoftStarterTimer = 0;
						p->SoftStarterTimer2 = 0;
						p->SoftStarterFlag  = 1;
						p->VoltErrFlag = 0;
					}
				}
			}


			if (g_Comm.mbShn.Stat.Status.bit.NoConnect)
			{
				if ((p->outDefects.Net.all & NET_UV_MASK || p->registerBrVolt)&& p->MoveOnFlag)
				{
						p->outDefects.Proc.bit.SoftStarter = 1;
						if (p->SoftStarterTimer++ >= 20 * Prd50HZ)
						{
							p->outDefects.Proc.bit.SoftStarter = 0;
							p->outFaults.Proc.bit.SoftStarter = 1;
							p->SoftStarterTimer = 0;
						}
				}


				if(p->MoveOnFlag == 0)
				{
					if(p->outFaults.Proc.bit.SoftStarter == 0)
					{
						if(p->SoftStarterConnTimer++ >= (2 * Prd50HZ))
						{
							p->outFaults.Proc.bit.SoftStarter = 1;
							p->SoftStarterConnTimer = 0;
						}
					}

				}
			}
			else if(g_Comm.mbShn.Stat.Status.bit.NoConnect == 0 && g_Ram.ramGroupA.Status.bit.Fault == 0 && p->MoveOnFlag == 1 && p->outDefects.Proc.bit.SoftStarter == 1)
			{
				p->outDefects.Proc.bit.SoftStarter = 0;
				p->SoftStarterTimer = 0;
				p->SoftStarterTimer2 = 0;
				p->SoftStarterFlag  = 1;
				p->VoltErrFlag = 0;
			}
		}
		//----------------Çàìåíà áàòàðåéêè!!!----------------------

		if(g_Ram.ramGroupB.DevDate.bit.Year == 0 && g_Ram.ramGroupB.DevTime.bit.Hour == 0)
		{
			p->outDefects.Dev.bit.TimeNotSet = 1;
		}
		else
		{
			p->outDefects.Dev.bit.TimeNotSet = 0;
		}

	/*if (g_Ram.ramGroupB.DevDate.bit.Year != 0 && g_Ram.ramGroupH.HideDate.all == 0)
	{
		if (IsMemParReady())
		{
			BatteryLowHideDataReg = REG_BATTERY_FAULT;
			g_Ram.ramGroupH.HideDate.all = g_Ram.ramGroupB.DevDate.all;
			WriteToEeprom(BatteryLowHideDataReg, &g_Ram.ramGroupH.HideDate, 1);	// òî çàïèñàëè ñîñòîÿíèå ÊÇ
		}
	}

	if (g_Ram.ramGroupB.DevDate.bit.Year == 0 && g_Ram.ramGroupH.HideDate.all != 0)  // åñëè ÷àñû ñáðîñèëèñü íî çàïèñàííî - ñòèðàåì
	{
		if (IsMemParReady())
		{
			BatteryLowHideDataReg = REG_BATTERY_FAULT;
			g_Ram.ramGroupH.HideDate.all = 0;
			WriteToEeprom(BatteryLowHideDataReg, &g_Ram.ramGroupH.HideDate, 1);	// òî çàïèñàëè ñîñòîÿíèå ÊÇ
		}
	}

	if (g_Ram.ramGroupH.HideDate.all != 0 && (g_Ram.ramGroupB.DevDate.all >= (g_Ram.ramGroupH.HideDate.all + 1536)))// || g_Ram.ramGroupB.DevDate.bit.Year == 0)
	{
		p->outDefects.Dev.bit.BatteryLow = 1;
	}*/
}

void Core_Protections18kHzUpdate(TCoreProtections *p)
{
	Uns MuffEnable, MuffAddr, TH_BCP_addr;

	if (p->FaultDelay > 0)
		return;

	Core_ProtecionSHC_Update(&p->ShC_U);
	Core_ProtecionSHC_Update(&p->ShC_V);
	Core_ProtecionSHC_Update(&p->ShC_W);

	// Çàïèñü ÌÓÔÒÛ â ïàìÿòü!!!!!! Îî
	if (p->MuffFlag == 1 && g_Ram.ramGroupH.MuffFault == 0)
	{
		if (IsMemParReady())
		{
			g_Ram.ramGroupH.MuffFault = p->MuffFlag;
			p->MuffFlag = 0;
			MuffAddr = REG_MUFF_FAULT;
			WriteToEeprom(MuffAddr, &g_Ram.ramGroupH.MuffFault, 1);	// òî çàïèñàëè ñîñòîÿíèå ÊÇ
		}
	}

	p->outFaults.Proc.bit.Mufta = g_Ram.ramGroupH.MuffFault;

	// Çàïèñü òåìïåðàòóðû 110 ÁÊÏ â ïàìÿòü...
	if(g_Ram.ramGroupH.TemperBCPFault == 0 && p->outFaults.Dev.bit.Th_BCP == 1)
	{
		if (IsMemParReady())
		{
			g_Ram.ramGroupH.TemperBCPFault = p->outFaults.Dev.bit.Th_BCP;
			TH_BCP_addr = REG_TEMP_BCP_FAULT;
			WriteToEeprom(TH_BCP_addr, &g_Ram.ramGroupH.TemperBCPFault, 1);
		}
	}

	if(g_Ram.ramGroupH.TemperBCPFault == 1)
	{
		p->outFaults.Dev.bit.Th_BCP = g_Ram.ramGroupH.TemperBCPFault;
	}


	if (p->outDefects.Dev.all || p->outDefects.Load.all || p->outDefects.Net.all || p->outDefects.Proc.all)
	{
		if(!p->outDefects.Dev.bit.LowPower)
		g_Core.Status.bit.Defect = 1;
	}
	else
		g_Core.Status.bit.Defect = 0;
	if (p->outFaults.Dev.all || p->outFaults.Load.all || p->outFaults.Net.all || p->outFaults.Proc.all || g_Ram.ramGroupA.BCP9Reg.all)
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

			p->outDefects.Proc.bit.NoMove = MuffEnable;

			//Core_ValveDriveStop(&g_Core.VlvDrvCtrl);
			//g_Core.VlvDrvCtrl.EvLog.Value = CMD_DEFSTOP;
		}
		//ToDo Îáñóäèòü ðåàëèçàöèþ ðåæèìà ïîæàðêè
		if (g_Core.Status.bit.Fault && g_Ram.ramGroupB.PlaceType != ptFire)   	// Åñëè Âêëþ÷åíà ïîæàðêà òî ìû áóäåì ãîðåòü íî åõàòü....
		{
			Core_ValveDriveStop(&g_Core.VlvDrvCtrl);
			g_Core.VlvDrvCtrl.EvLog.Value = CMD_DEFSTOP;
		}
	}

}

// ToDo ïðîâåðèòü â ðåæèìå ïîæàðêè! + îòñòðîèòü óðîâíè äèñêðåòíîãî óïàðâëåíèÿ
void Core_ProtectionFireControl(void)
{
	if (g_Ram.ramGroupB.PlaceType != ptFire) return;

	if ((g_Ram.ramGroupA.StateTs.bit.Closing && !g_Ram.ramGroupA.StateTu.bit.Du)||(g_Ram.ramGroupA.StateTs.bit.Opening && !g_Ram.ramGroupA.StateTu.bit.Mu))
	{
		if((g_Core.Protections.FireContErr_Timer++ >= 1*Prd10HZ) && (g_Core.Protections.outDefects.Proc.bit.FireContErr == 0))
		{
			g_Core.Protections.outDefects.Proc.bit.FireContErr = 1;
			g_Core.Protections.FireContErr_Timer = 0;
		}
	}
}



