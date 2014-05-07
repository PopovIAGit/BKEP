/*======================================================================
��� �����:          core_Protections.h
�����:				����� �.�.
������ �����:		1.00
���� ���������:		25.04.2014
��������:
������ ����� ��������������
======================================================================*/

#include "core.h"
#include "peref.h"

void Core_ProtectionsInit(TCoreProtections *p)
{
	//---------������ �� ����������---------------------------------------
	//---------���������� ���������� (������)---------------------------------------

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

	//---------���������� ���������� (�������������)---------------------------------------

	p->underDefVoltageR.Cfg.all 	= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,UvR_bit,20);
	p->underDefVoltageS.Cfg.all 	= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,UvS_bit,20);
	p->underDefVoltageT.Cfg.all 	= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,UvT_bit,20);

	p->underDefVoltageR.Input 		= (Int *)&g_Ram.ramGroupH.Ur;
	p->underDefVoltageS.Input 		= (Int *)&g_Ram.ramGroupH.Us;
	p->underDefVoltageT.Input 		= (Int *)&g_Ram.ramGroupH.Ut;

	p->underDefVoltageR.Output 		= (Uns *)&p->outDefects.Net.all;
	p->underDefVoltageS.Output 		= (Uns *)&p->outDefects.Net.all;
	p->underDefVoltageT.Output 	 	= (Uns *)&p->outDefects.Net.all;

	p->underDefVoltageR.EnableLevel = (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underDefVoltageS.EnableLevel = (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underDefVoltageT.EnableLevel = (Int *)&g_Ram.ramGroupC.UvLevel;

	p->underDefVoltageR.DisableLevel= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underDefVoltageS.DisableLevel= (Int *)&g_Ram.ramGroupC.UvLevel;
	p->underDefVoltageT.DisableLevel= (Int *)&g_Ram.ramGroupC.UvLevel;

	p->underDefVoltageR.Timeout		= &g_Ram.ramGroupC.UvDTime;
	p->underDefVoltageS.Timeout		= &g_Ram.ramGroupC.UvDTime;
	p->underDefVoltageT.Timeout		= &g_Ram.ramGroupC.UvDTime;

	p->underDefVoltageR.Scale 		= PROTECT_SCALE;
	p->underDefVoltageS.Scale 		= PROTECT_SCALE;
	p->underDefVoltageT.Scale 		= PROTECT_SCALE;

	//---------���������� ���������� (������)------------------------------------------
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

	//---------���������� ���������� (�������������)------------------------------------------
	p->overDefVoltageR.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvR_bit,20);
	p->overDefVoltageS.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvS_bit,20);
	p->overDefVoltageT.Cfg.all 		= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvT_bit,20);

	p->overDefVoltageR.Input 		= (Int *)&g_Ram.ramGroupH.Ur;
	p->overDefVoltageS.Input 		= (Int *)&g_Ram.ramGroupH.Us;
	p->overDefVoltageT.Input 		= (Int *)&g_Ram.ramGroupH.Ut;

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

	//---------���������� ���������� �� 47% (������) ------------------------------------------
	p->overMax_VoltageR.Cfg.all 	= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvR_max_bit,20);
	p->overMax_VoltageS.Cfg.all 	= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvS_max_bit,20);
	p->overMax_VoltageT.Cfg.all 	= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,OvT_max_bit,20);

	p->overMax_VoltageR.Input 		= (Int *)&g_Ram.ramGroupH.Ur;
	p->overMax_VoltageS.Input 		= (Int *)&g_Ram.ramGroupH.Us;
	p->overMax_VoltageT.Input 		= (Int *)&g_Ram.ramGroupH.Ut;

	p->overMax_VoltageR.Output 		= (Uns *)&p->outFaults.Net.all;
	p->overMax_VoltageS.Output 		= (Uns *)&p->outFaults.Net.all;
	p->overMax_VoltageT.Output 	 	= (Uns *)&p->outFaults.Net.all;

	p->overMax_VoltageR.EnableLevel = (Int *)&g_Ram.ramGroupC.OvLevel_max;
	p->overMax_VoltageS.EnableLevel = (Int *)&g_Ram.ramGroupC.OvLevel_max;
	p->overMax_VoltageT.EnableLevel = (Int *)&g_Ram.ramGroupC.OvLevel_max;

	p->overMax_VoltageR.DisableLevel= (Int *)&g_Ram.ramGroupC.OvLevel_max;
	p->overMax_VoltageS.DisableLevel= (Int *)&g_Ram.ramGroupC.OvLevel_max;
	p->overMax_VoltageT.DisableLevel= (Int *)&g_Ram.ramGroupC.OvLevel_max;

	p->overMax_VoltageR.Timeout		= &g_Ram.ramGroupC.OvTime_max;
	p->overMax_VoltageS.Timeout		= &g_Ram.ramGroupC.OvTime_max;
	p->overMax_VoltageT.Timeout		= &g_Ram.ramGroupC.OvTime_max;

	p->overMax_VoltageR.Scale 		= PROTECT_SCALE;
	p->overMax_VoltageS.Scale 		= PROTECT_SCALE;
	p->overMax_VoltageT.Scale 		= PROTECT_SCALE;

	//---------����� ��� ���������� (R S T  ������)-----------------------
	p->breakVoltR.Cfg.all			= PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL,BvR_bit,20);
	p->breakVoltS.Cfg.all 			= PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL,BvS_bit,20);
	p->breakVoltT.Cfg.all 			= PRT_CFG_SET(CAN_NOT_BE_RESETED, INP_LESS_LEVEL,BvT_bit,20);

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

	//---------���������� ���������� (�������������)-----------------------------------
	p->voltSkew.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,VSk_bit,20);
	p->voltSkew.Input				= (Int *)&g_Ram.ramGroupH.VSkValue;
	p->voltSkew.Output				= (Uns *)&p->outDefects.Net.all;
	p->voltSkew.EnableLevel			= (Int *)&g_Ram.ramGroupC.VSkLevel;
	p->voltSkew.DisableLevel		= (Int *)&g_Ram.ramGroupC.VSkLevel;
	p->voltSkew.Timeout				= &g_Ram.ramGroupC.VSkTime;
	p->voltSkew.Scale				= PROTECT_SCALE;

	//---------������ �� ����---------------------------------------------
	//---------����� ��� �� ���� (U V W �������������)---------------------------------
	p->breakCurrU.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,BvR_bit,HYST_OFF);
	p->breakCurrV.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,BvS_bit,HYST_OFF);
	p->breakCurrW.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,BvT_bit,HYST_OFF);

	p->breakCurrU.Input 			= (Int *)&g_Ram.ramGroupH.Iu;
	p->breakCurrV.Input 			= (Int *)&g_Ram.ramGroupH.Iv;
	p->breakCurrW.Input 			= (Int *)&g_Ram.ramGroupH.Iw;

	p->breakCurrU.Output 			= (Uns *)&p->outDefects.Load.all;
	p->breakCurrV.Output 			= (Uns *)&p->outDefects.Load.all;
	p->breakCurrW.Output 	 		= (Uns *)&p->outDefects.Load.all;

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

	//---------�����-������� ���������� (������)-------------------------------------
	p->I2t.Cfg.bit.Hyst 			= HYST_OFF;
	p->I2t.Cfg.bit.Num 				= I2t_bit;
	p->I2t.InputCurrentMid 			= &g_Ram.ramGroupH.Imid;
	p->I2t.NomCurrent 				= &g_Ram.ramGroupC.Inom;
	p->I2t.Output 					= (Uns *)&p->outFaults.Load.all;
	p->I2t.Scale 					= PROTECT_SCALE;

	//--------���������� ����� (�������������)----------------------------------------------
	p->voltSkew.Cfg.all 			= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,ISkew_bit,HYST_OFF);
	p->voltSkew.Input				= (Int *)&g_Ram.ramGroupH.ISkewValue;
	p->voltSkew.Output				= (Uns *)&p->outDefects.Load.all;
	p->voltSkew.EnableLevel			= (Int *)&g_Ram.ramGroupC.ISkewLevel;
	p->voltSkew.DisableLevel		= (Int *)&g_Ram.ramGroupC.ISkewLevel;
	p->voltSkew.Timeout				= &g_Ram.ramGroupC.ISkewTime;
	p->voltSkew.Scale				= PROTECT_SCALE;

	//------�������� ���������----------------------------------------------
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

	p->ShC_U.Output					= &p->outFaults.Load.all;
	p->ShC_V.Output					= &p->outFaults.Load.all;
	p->ShC_W.Output					= &p->outFaults.Load.all;

	//------����������� ����������------------------------------------------
	//------�������� ����� ���----------------------------------------------
	p->overHeatBCD.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,Th_BCD_bit,1);
	p->overHeatBCD.Input			= &g_Ram.ramGroupA.Temper;
	p->overHeatBCD.Output			= &p->outDefects.Dev.all;
	p->overHeatBCD.EnableLevel		= &g_Ram.ramGroupC.TemperHigh;
	p->overHeatBCD.DisableLevel		= &g_Ram.ramGroupC.TemperHigh;
	p->overHeatBCD.Timeout			= 0;
	p->overHeatBCD.Scale			= PROTECT_SCALE;

	//------�������������� ����� ���----------------------------------------
	p->underColdBCD.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,Tl_BCD_bit,1);
	p->underColdBCD.Input			= &g_Ram.ramGroupA.Temper;
	p->underColdBCD.Output			= &p->outDefects.Dev.all;
	p->underColdBCD.EnableLevel		= &g_Ram.ramGroupC.TemperLow;
	p->underColdBCD.DisableLevel	= &g_Ram.ramGroupC.TemperLow;
	p->underColdBCD.Timeout			= 0;
	p->underColdBCD.Scale			= PROTECT_SCALE;

	//------�������� ����� ���----------------------------------------------
	p->overHeatBCP.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_GREATER_LEVEL,Th_BCP_bit,1);
	p->overHeatBCP.Input			= &g_Ram.ramGroupA.TemperBKP;
	p->overHeatBCP.Output			= &p->outFaults.Dev.all;
	p->overHeatBCP.EnableLevel		= &g_Ram.ramGroupC.TemperHigh;
	p->overHeatBCP.DisableLevel		= &g_Ram.ramGroupC.TemperHigh;
	p->overHeatBCP.Timeout			= 0;
	p->overHeatBCP.Scale			= PROTECT_SCALE;

	//------�������������� ����� ���----------------------------------------
	p->underColdBCP.Cfg.all			= PRT_CFG_SET(CAN_BE_RESETED, INP_LESS_LEVEL,Tl_BCP_bit,1);
	p->underColdBCP.Input			= &g_Ram.ramGroupA.TemperBKP;
	p->underColdBCP.Output			= &p->outDefects.Dev.all;
	p->underColdBCP.EnableLevel		= &g_Ram.ramGroupC.TemperLow;
	p->underColdBCP.DisableLevel	= &g_Ram.ramGroupC.TemperLow;
	p->underColdBCP.Timeout			= 0;
	p->underColdBCP.Scale			= PROTECT_SCALE;

}

// ������� ���������/���������� �����
void Core_ProtectionsEnable(TCoreProtections *p)
{
	static Byte State = 0;
	Bool Enable;

	switch (++State)
	{
	case 1: //Muffta
		break;
	case 2:	// ������ �� ���������
		Enable = (g_Ram.ramGroupC.Uv != pmOff);						// �������� ����������
		p->underVoltageR.Cfg.bit.Enable 	= Enable;
		p->underVoltageS.Cfg.bit.Enable 	= Enable;
		p->underVoltageT.Cfg.bit.Enable 	= Enable;
		p->underDefVoltageR.Cfg.bit.Enable 	= Enable;
		p->underDefVoltageS.Cfg.bit.Enable 	= Enable;
		p->underDefVoltageT.Cfg.bit.Enable 	= Enable;

		Enable = (g_Ram.ramGroupC.Ov != pmOff);						// ��������� ����������
		p->overVoltageR.Cfg.bit.Enable 		= Enable;
		p->overVoltageS.Cfg.bit.Enable 		= Enable;
		p->overVoltageT.Cfg.bit.Enable 		= Enable;
		p->overDefVoltageR.Cfg.bit.Enable 	= Enable;
		p->overDefVoltageS.Cfg.bit.Enable 	= Enable;
		p->overDefVoltageT.Cfg.bit.Enable 	= Enable;
		p->overMax_VoltageR.Cfg.bit.Enable 	= Enable;
		p->overMax_VoltageS.Cfg.bit.Enable 	= Enable;
		p->overMax_VoltageT.Cfg.bit.Enable 	= Enable;

		Enable = (g_Ram.ramGroupC.Bv != pmOff);						// ����� ��� ����������
		p->breakVoltR.Cfg.bit.Enable 		= Enable;
		p->breakVoltS.Cfg.bit.Enable 		= Enable;
		p->breakVoltT.Cfg.bit.Enable 		= Enable;

		p->voltSkew.Cfg.bit.Enable = (g_Ram.ramGroupC.VSk != pmOff);//�������� ����������
		break;
	case 3: // ������ �� ����
		Enable = (g_Ram.ramGroupC.Phl != pmOff) && (!g_Core.Status.bit.Stop);					// ����� �������� ��� (����.)
		p->breakCurrU.Cfg.bit.Enable 		= Enable;
		p->breakCurrU.Cfg.bit.Enable 		= Enable;
		p->breakCurrU.Cfg.bit.Enable 		= Enable;

		Enable = (g_Ram.ramGroupC.ShC != pmOff) && (!g_Core.Status.bit.Stop);					// �������� ���������
		p->ShC_U.Cfg.bit.Enable 			= Enable;
		p->ShC_V.Cfg.bit.Enable 			= Enable;
		p->ShC_W.Cfg.bit.Enable 			= Enable;

		p->currSkew.Cfg.bit.Enable = (g_Ram.ramGroupC.ISkew != pmOff) && (!g_Core.Status.bit.Stop);// �������� �����

		p->I2t.Cfg.bit.Enable = (g_Ram.ramGroupC.I2t != pmOff) && (!g_Core.Status.bit.Stop);		// ���
		break;
	case 4: // ������ ����������
		Enable = g_Ram.ramGroupC.TemperTrack != pmOff;
		p->overHeatBCD.Cfg.bit.Enable 		= Enable;
		p->overHeatBCP.Cfg.bit.Enable 		= Enable;
		p->underColdBCD.Cfg.bit.Enable 		= Enable;
		p->underColdBCP.Cfg.bit.Enable 		= Enable;

		State = 0;	// ��������� � �������
		break;
	}

}

// ��������� ������ �������� � ����������
void Core_DevProc_FaultIndic(TCoreProtections *p)
{
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
		p->outDefects.Dev.bit.Memory1 = 0;//g_Eeprom1.Error;
		p->outDefects.Dev.bit.Memory2 = 0;//g_Eeprom2.Error;
		p->outDefects.Dev.bit.PosSens = 0;//??? �������� ��� ������� ��������
		p->outDefects.Dev.bit.Rtc 	 = 0;
		p->outDefects.Dev.bit.TSens 	 = 0;
	}
}

void EngPhOrdPrt(TCoreProtections *p)
{
	static Bool  Flag = False;          // ���� ��� ��������� �������� ������������ ����������� ����
	static LgUns StartPos;
	static Uns   Timer = 0;
	static Int   EngPhOrdValue = 0;
	LgInt  Delta;

	if ((g_Ram.ramGroupC.PhOrd == pmOff) || (g_Core.Status.bit.Stop))
	{
		Flag = false;
		StartPos = g_Ram.ramGroupC.Position;
		Timer = 0;
		return;
	}

	if (Timer < ((Uns)PROTECT_SCALE * g_Ram.ramGroupC.PhOrdTime)) Timer++;
	else if (!Flag)
	{
		Delta = g_Ram.ramGroupC.Position - StartPos;

		if (Delta >  ((REV_MAX+1)/2)) Delta -= (REV_MAX+1);
		if (Delta < -((REV_MAX+1)/2)) Delta += (REV_MAX+1);

		EngPhOrdValue = 0;
		if(Delta >=  ((LgInt) g_Ram.ramGroupC.PhOrdZone)) EngPhOrdValue = 1;
		if(Delta <= -((LgInt) g_Ram.ramGroupC.PhOrdZone)) EngPhOrdValue = -1;

		if(g_Ram.ramGroupB.RodType == rdtInverse) EngPhOrdValue = -EngPhOrdValue;

		if((EngPhOrdValue != 0) && (EngPhOrdValue != g_Core.MotorControl.RequestDir))
			p->outFaults.Proc.bit.PhOrd = 1;

		Flag = true;
	}
}

// ����� ����� ��� �����
void Core_ProtectionsReset(TCoreProtections *p)
{
	g_Core.MotorControl.OverWayFlag = 0;		// �������� �������� ����������
	//Muff = 0;
	g_Core.Status.all &= ~STATUS_RESET_MASK;	// ��������� ������ �� ������ ����� � ��������������

	p->outDefects.Load.bit.ISkew = 0;
	p->outDefects.Load.bit.PhlU = 0;
	p->outDefects.Load.bit.PhlV = 0;
	p->outDefects.Load.bit.PhlW = 0;

	p->outDefects.Proc.bit.Overway = 0;
	p->outDefects.Proc.bit.NoMove = 0;
}

// ����� ���� �����
void Core_ProtectionsClear(TCoreProtections *p)
{
	g_Core.MotorControl.OverWayFlag = 0;		// �������� �������� ����������
	//Muff = 0;									// �������� �����

	g_Core.Status.all &= ~STATUS_RESET_MASK;	// ��������� ������ �� ������ ����� � ��������������

	p->outFaults.Dev.all  = 0;					// �������� ��� ������
	p->outFaults.Net.all  = 0;
	p->outFaults.Load.all = 0;
	p->outFaults.Proc.all = 0;

	p->outDefects.Dev.all  = 0;					// �������� ��� �������������
	p->outDefects.Net.all  = 0;
	p->outDefects.Load.all = 0;
	p->outDefects.Proc.all = 0;

	/* ??? ����� ������ ������ �������� � ��
	Encoder.Error  = False;
	Eeprom1.Error  = False;
	Eeprom2.Error  = False;
	TempSens.Error = False;

	IsShcReset = true; ��������� ���� ������ ��
	  */
}

void Core_ProtectionsUpdate(TCoreProtections *p)
{

}