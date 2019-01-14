/*======================================================================
��� �����:          core.c
�����:
������ �����:
���� ���������:
��������: ������ ����
======================================================================*/

#include "core.h"
#include "peref.h"
#include "g_Structs.h"
#include "comm.h"
#include "stat.h"
#include "stat_fm25v10.h"

TFM25V10 Eeprom1;
TFM25V10 Eeprom2;

TCore	g_Core;

//����� ���������
__inline void Eeprom1CsSet(Byte Lev)  {SC_EEPROM1 = !Lev;}
__inline void Eeprom2CsSet(Byte Lev)  {SC_EEPROM2 = !Lev;}

static void MoveMode (void);
static void StopMode(void);
static void PlugBreakMode(void);
static void ShnStartMode(void);
static void ShnStopMode(void);
static void StartMode(void);
static void ShnControlErrCheck(void);

//---------------------------------------------------
void Core_Init(TCore *p)
{
	// ������������� �����
	Eeprom1.CsFunc = &Eeprom1CsSet;
	FM25V10_Init(&Eeprom1);
	Eeprom2.CsFunc = &Eeprom2CsSet;
	FM25V10_Init(&Eeprom2);

	// �������� ���������� �������������
	Core_MenuInit(&p->menu);

	Core_ValveDriveInit(&p->VlvDrvCtrl);	// ���������� ���������
	Core_TorqueInit(&p->TorqObs);			// ������ ��������
	//Core_CommandsInit(&p->commands);		// ��������� ������, ��������� ����������
	Core_ProtectionsInit(&p->Protections);	// ������
	Core_DisplayFaultsInit(&p->DisplayFaults);

	p->Status.bit.Stop 			= 1;					// ��� ��������� ���������� ����
	g_Ram.ramGroupH.ContGroup 		= cgStop;
}

// ������� ������� ������� � ����������� �� ��������� � ����������� ��������
void Core_DefineCtrlParams(TCore *p) // 50 hz
{
	Int MaxZone, CloseZone, OpenZone;	// ������������ ������ ����, ���� ��������, ���� ��������

	if(g_Ram.ramGroupB.ValveType == vtShiber)		// ���� �������� ��������
	{
		g_Ram.ramGroupB.BreakMode = vtNone;			// ������ �������� ��� ����������
	}

	MaxZone = (g_Ram.ramGroupA.FullWay >> 1) - 1;	// ���������� ������������ ������ ��� �������/�������

	CloseZone = g_Ram.ramGroupB.CloseZone;
	if (CloseZone > MaxZone) CloseZone = MaxZone; // ���� ������ ������ ��������� ������ ��������

	OpenZone = g_Ram.ramGroupB.OpenZone;
	if (OpenZone > MaxZone) OpenZone = MaxZone;   // ���� ������ ������ ��������� ������ ��������
	OpenZone = g_Ram.ramGroupA.FullWay - OpenZone;

	if(!p->MotorControl.RequestDir) p->MotorControl.TorqueSet = 0; // ���� �� ������ ����������� ����������� �������� �� ������ ������� 0
	else if(p->MotorControl.RequestDir > 0)	 // ���������� ������� �� ������ � ����������� �� ����������� � ����������
	{
		p->MotorControl.TorqueSet = g_Ram.ramGroupB.MoveOpenTorque;
		p->MotorControl.BreakFlag = 0;

		if(g_Ram.ramGroupH.CalibState == csCalib)
		{
			if(g_Ram.ramGroupA.CurWay <= CloseZone)
			{
				p->MotorControl.TorqueSet = g_Ram.ramGroupB.StartCloseTorque;
				p->MotorControl.BreakFlag = 0;
			}
			else if (g_Ram.ramGroupA.CurWay >= OpenZone)
			{
				p->MotorControl.TorqueSet = g_Ram.ramGroupB.BreakOpenTorque;
				p->MotorControl.BreakFlag = 1;
			}
		}
	}
	else
	{
		p->MotorControl.TorqueSet = g_Ram.ramGroupB.MoveCloseTorque;
		p->MotorControl.BreakFlag = 0;

		if(g_Ram.ramGroupH.CalibState == csCalib)
		{
			if(g_Ram.ramGroupA.CurWay <= CloseZone)
			{
				p->MotorControl.TorqueSet = g_Ram.ramGroupB.BreakCloseTorque;
				p->MotorControl.BreakFlag = 1;
			}
			else if (g_Ram.ramGroupA.CurWay >= OpenZone)
			{
				p->MotorControl.TorqueSet = g_Ram.ramGroupB.StartOpenTorque;
				p->MotorControl.BreakFlag = 0;
			}
		}
	}

	// ������������� ������� �� ������
	p->MotorControl.TorqueSetPr = (Uns)(((LgUns)p->MotorControl.TorqueSet * 100)/p->TorqObs.TorqueMax);

}

// ��������� �� ����������
void Core_CalibStop (TCore *p)
{
	Bool StopFlag = False; // ��������� ���� ���������
	LgInt Position = p->VlvDrvCtrl.Valve.Position;

	if(p->VlvDrvCtrl.Valve.Position == POS_UNDEF) //���� ������� ��������� �� ���������� �� ������
	{
		p->MotorControl.TargetPos = POS_UNDEF;
		return;
	}
	p->MotorControl.TargetPos = (g_Peref.Position.LinePos - Position);

	if(p->Status.bit.Stop) return;

	if((p->MotorControl.RequestDir < 0) && (p->MotorControl.TargetPos <= g_Ram.ramGroupC.BreakZone)) StopFlag = True;
	if((p->MotorControl.RequestDir > 0) && (p->MotorControl.TargetPos >= -g_Ram.ramGroupC.BreakZone)) StopFlag = True;

	if (StopFlag)	// ���� ���� ���������������
	{
		if(p->VlvDrvCtrl.Valve.BreakFlag) p->MotorControl.OverWayFlag = 1;	//
		else	// ����
			{
				p->MotorControl.CalibStop = 1;
				Core_ValveDriveStop(&p->VlvDrvCtrl);
				p->VlvDrvCtrl.EvLog.Value = CMD_STOP;
			}
	}
}

// ���������� �����������
void Core_CalibControl(TCore *p)
{
	g_Peref.Position.ResetFlag = !p->Status.bit.Stop;

	if(g_Peref.Position.CancelFlag)
	{
		p->VlvDrvCtrl.Mpu.CancelFlag = true;
		g_Peref.Position.CancelFlag = false;
	}

		p->Status.bit.Closed = p->Status.bit.Stop && ((g_Peref.Position.Zone & CLB_CLOSE) != 0);
		p->Status.bit.Opened = p->Status.bit.Stop && ((g_Peref.Position.Zone & CLB_OPEN)  != 0);

	if(g_Ram.ramGroupD.CalibReset != 0)
	{
		if (!p->Status.bit.Stop )
		{
			p->VlvDrvCtrl.Mpu.CancelFlag = true;
		}
		else
		{
			g_Ram.ramGroupD.TaskClose = trReset;
			g_Ram.ramGroupD.TaskOpen  = trReset;

			p->VlvDrvCtrl.EvLog.Value = CMD_RES_CLB;
		}
		g_Ram.ramGroupD.CalibReset = 0;
	}

	// ������� �� ����� �������� ������
	if(g_Ram.ramGroupD.CycleReset != 0 )
	{
		if (!p->Status.bit.Stop )
		{
			p->VlvDrvCtrl.Mpu.CancelFlag = true;
		}
		else
		{
			g_Ram.ramGroupH.CycleCnt = 0;
			g_Peref.Position.CycleData = 0;
			p->VlvDrvCtrl.EvLog.Value = CMD_RES_CYCLE;
		}
		g_Ram.ramGroupD.CycleReset = 0;
	}

	if (IsMemParReady())																	// ���� ���� ���������� � ������ ����������
	{
		if (g_Ram.ramGroupH.CycleCnt != p->PrevCycle)										// ���� ������� ������ ���������
		{
			WriteToEeprom(REG_CYCLE_CNT, &g_Ram.ramGroupH.CycleCnt, 1);						// �������� �������� ������� ������
			p->PrevCycle = g_Ram.ramGroupH.CycleCnt;										// ��������� ���������� ��������, ��� ����������� ��������
		}
		else if (g_Ram.ramGroupH.CalibState != g_Ram.ramGroupA.CalibState)					// ���� ��������� ���������� ����������
		{
			WriteToEeprom(REG_CALIB_STATE, &g_Ram.ramGroupH.CalibState, sizeof(ClbIndication));	// �� �������� ��������� ����������
			g_Ram.ramGroupA.CalibState = g_Ram.ramGroupH.CalibState;
		}
	}
}

// �������� ����������� ��� �����
void StopPowerControl(void)
{
	g_Core.Status.bit.Stop 		= 1;	// ��������� ������ ����
	g_Core.Status.bit.Opening 	= 0;
	g_Core.Status.bit.Closing 	= 0;
	g_Core.Status.bit.Test 		= 0;

	if (g_Core.Status.bit.Fault)
	{
		g_Core.MotorControl.WorkMode = wmStop;  // ��������� � ����� ������ ����
	}
	else if (g_Core.MotorControl.CalibStop)
	{
		switch(g_Ram.ramGroupB.StopMethod)
		{
			case smSlowDown: g_Core.MotorControl.WorkMode = wmStop; break;
			case smReverse:  g_Core.MotorControl.WorkMode = wmPlugBreak; break;
			case smDynBreak: g_Core.MotorControl.WorkMode = wmShnStop; break;
		}
	}
	else
		{
			if(g_Ram.ramGroupB.StopMethod == smDynBreak)
			{
				g_Ram.ramGroupATS.Control1.all = 0x4000;
			}
			 g_Core.MotorControl.WorkMode = wmStop;
		}

	g_Core.MotorControl.CalibStop = 0;
	g_Core.VlvDrvCtrl.StartDelay = (Uns)START_DELAY_TIME; // ���������� �������� ����� ��������� ������
	g_Core.TorqObs.ObsEnable = false;
	g_Core.Protections.SoftStarterTimer = 0;
	g_Core.Protections.SoftStarterTimer2 = 0;
	g_Core.Protections.MoveOnFlag 	= 0;
	g_Core.Protections.SoftStarterFlag = 0;
	g_Core.Protections.SoftStarterConnTimer = 0;
	g_Core.Protections.VoltErrFlag = 0;
}

// �������� ��� �����
void StartPowerControl(TValveCmd ControlWord)
{
	//���� �� �� return
	if (g_Ram.ramGroupA.Faults.Load.all & LOAD_SHC_MASK) return;

	// ����� ������ ����������� ��� �����
	Core_ProtectionsReset(&g_Core.Protections);

	if(g_Core.Protections.outFaults.Dev.all || g_Core.Protections.outFaults.Load.all ||g_Core.Protections.outFaults.Net.all ||g_Core.Protections.outFaults.Proc.all)
	    return;

	g_Core.TorqObs.ObsEnable 				= true;
	g_Core.MotorControl.PlugBreakStep 		= 0;
	g_Core.Status.bit.Stop 					= 0;
	g_Core.MotorControl.TorqueSet 			= 0xFFFF;
	g_Core.MotorControl.MufTimer 			= 0;
	g_Core.MotorControl.ShnControlErrTimer 	= 0;
	g_Core.MotorControl.ShnControlErr 		= 0;
	g_Core.MotorControl.PlugBreakTimer 		= 0;
	g_Comm.Shn.SHN_StopFlag 				= 1;

	switch (ControlWord)
	{
		case vcwClose:
			g_Core.MotorControl.RequestDir = -1;
			g_Core.MotorControl.WorkMode = wmStart;
			break;
		case vcwOpen:
			g_Core.MotorControl.RequestDir = 1;
			g_Core.MotorControl.WorkMode = wmStart;
			break;
		case vcwTestClose:
			g_Core.MotorControl.RequestDir = -1;
			g_Core.MotorControl.WorkMode = wmStart;
			g_Core.Status.bit.Test = 1;
			break;
		case vcwTestOpen:
			g_Core.MotorControl.RequestDir = 1;
			g_Core.MotorControl.WorkMode = wmStart;
			g_Core.Status.bit.Test = 1;
			break;
	}
}

// ����� ������
void Core_ControlMode(TCore *p) // 50 ��
{
    p->Status.bit.Mufta = p->Protections.outFaults.Proc.bit.Mufta;

	// ���������� ��������� ����������� ����������� ��� ���

    if(p->Status.bit.Stop)
    {
    	if(!InRange(g_Peref.sensObserver.IUout,-0.5, 0.5))
    	{
    		if(g_Peref.sensObserver.IUout > 0) g_Ram.ramGroupC.IU_Offset++;
    		if(g_Peref.sensObserver.IUout < 0) g_Ram.ramGroupC.IU_Offset--;
    	}
    	if(!InRange(g_Peref.sensObserver.IVout,-0.5, 0.5))
    	{
    		if(g_Peref.sensObserver.IVout > 0) g_Ram.ramGroupC.IV_Offset++;
    		if(g_Peref.sensObserver.IVout < 0) g_Ram.ramGroupC.IV_Offset--;
    	}
    	if(!InRange(g_Peref.sensObserver.IWout,-0.5, 0.5))
    	{
    		if(g_Peref.sensObserver.IWout > 0) g_Ram.ramGroupC.IW_Offset++;
    		if(g_Peref.sensObserver.IWout < 0) g_Ram.ramGroupC.IW_Offset--;
    	}

    }

    switch(p->MotorControl.WorkMode)
    {
    case wmStop:		StopMode();	break;
	case wmPlugBreak:	PlugBreakMode();	break;
	case wmShnStop:		ShnStopMode();break;
	case wmStart:		StartMode();break;
	case wmShnStart:	ShnStartMode();break;
	case wmMove:		MoveMode(); break;
//	case wmSpeedTest:	break; ???
    }
}

static void StopMode(void)
{
	if (g_Ram.ramGroupB.StopMethod == smDynBreak && g_Core.Status.bit.Fault == 0)
	{
		if (g_Ram.ramGroupATS.State1.bit.ReadyToSwitchOn == 0)
		{
			g_Ram.ramGroupATS.Control1.bit.DecelStopReq = 0;
			g_Core.MotorControl.RequestDir  = 0;		// ���������� ����������� ���������� ��������
			g_Core.MotorControl.ShnControlErrTimer = 0;
			g_Ram.ramGroupA.Torque 			= 0;		// ���������� ������
			g_Ram.ramGroupH.ContGroup 		= cgStop; 	// ������ ������� �� ���� �����������

		}
	}
	else
	{
		g_Core.MotorControl.RequestDir  = 0;		// ���������� ����������� ���������� ��������
		g_Core.MotorControl.ShnControlErrTimer = 0;
		g_Ram.ramGroupA.Torque 			= 0;		// ���������� ������
		g_Ram.ramGroupH.ContGroup 		= cgStop; 	// ������ ������� �� ���� �����������
	}

	g_Core.Protections.MoveOnFlag 	= 0;
	g_Core.MotorControl.accelTimer          = 0;		// ������ �������
}

static void MoveMode(void)
{
//	if (g_Ram.ramGroupC.DriveType == 1)
//	{
		//ToDo ������ ��� �������� � ����������� ����.
		if (g_Ram.ramGroupA.Faults.Net.bit.UvR
				|| g_Ram.ramGroupA.Faults.Net.bit.UvS
				|| g_Ram.ramGroupA.Faults.Net.bit.UvT
				|| g_Ram.ramGroupA.Faults.Net.bit.OvR
				|| g_Ram.ramGroupA.Faults.Net.bit.OvS
				|| g_Ram.ramGroupA.Faults.Net.bit.OvT
				|| g_Ram.ramGroupA.Faults.Load.bit.PhlU
				|| g_Ram.ramGroupA.Faults.Load.bit.PhlU
				|| g_Ram.ramGroupA.Faults.Load.bit.PhlV
				|| g_Ram.ramGroupA.Faults.Load.bit.PhlW)
		{
			g_Ram.ramGroupA.Torque = g_Core.MotorControl.TorqueSet - 1;
		}
		else
		{
			g_Ram.ramGroupA.Torque = g_Core.TorqObs.Indication; // ���������� ������� ������
		}
//	}
//	else
//		g_Ram.ramGroupA.Torque = g_Core.TorqObs.Indication; // ���������� ������� ������


		//ToDo ������ ���������� � ����������� �� ���� ����� + �������� ������ �� ���������� �������� �����!!!
	if(g_Peref.phaseOrder.Direction == 1)
	{
	    if (CONTACTOR_1_STATUS && g_Core.MotorControl.RequestDir > 0)  g_Core.Status.bit.Opening = 1;
	    if (CONTACTOR_2_STATUS && g_Core.MotorControl.RequestDir < 0)  g_Core.Status.bit.Closing = 1;
	}
	else if(g_Peref.phaseOrder.Direction == -1)
	{
	    if (CONTACTOR_2_STATUS && g_Core.MotorControl.RequestDir > 0)  g_Core.Status.bit.Opening = 1;
	    if (CONTACTOR_1_STATUS && g_Core.MotorControl.RequestDir < 0)  g_Core.Status.bit.Closing = 1;
	}


	g_Core.Protections.MuffFlag = g_Core.Protections.MuffFlag200Hz;

	if (g_Core.Protections.SoftStarterFlag == 1 && g_Ram.ramGroupB.StopMethod == smDynBreak)
	{
		g_Core.MotorControl.WorkMode = wmShnStart;
		g_Core.Protections.SoftStarterFlag = 0;
	}
}

void Protections_MuffFlag(void)
{
	if(g_Core.Status.bit.Stop)				// ��������� ������ �� ����� � �����
	{
		g_Core.MotorControl.MufTimer = 0;
		return;
	}

	if (g_Ram.ramGroupC.DriveType == dt50000_F48 	//���� ��� ������� - ����50000
	    && g_Core.MotorControl.accelTimer < 1200)	//� ������ ������� ��� �� �������� (400 = 6 ��� * 200 hz)
	{
		g_Core.MotorControl.accelTimer++;			// ������� �� �������, ����� �� �������
		return;
	}

	if (g_Ram.ramGroupA.Torque > g_Core.MotorControl.TorqueSet)
	{
		if (g_Core.MotorControl.MufTimer < (80 * g_Ram.ramGroupB.MuffTimer))
		{
			g_Core.MotorControl.MufTimer += 4;
		}

		if (g_Core.MotorControl.MufTimer >= (80 * g_Ram.ramGroupB.MuffTimer))
		{
			g_Core.Protections.MuffFlag200Hz = 1;	//  1 ���������� ����� ���� � ������� ������� ������ ������ ���������
		}
	}
	else
	{
		if (g_Core.MotorControl.MufTimer) g_Core.MotorControl.MufTimer--;
	}

}

static void PlugBreakMode(void)
{
	g_Ram.ramGroupA.Torque = 0;				// ���������� ������

	if (g_Core.MotorControl.PlugBreakTimer > 0) g_Core.MotorControl.PlugBreakTimer--;
	switch (g_Core.MotorControl.PlugBreakStep)
	{
	case 0:
		g_Ram.ramGroupH.ContGroup 	   	   = cgStop;
		g_Core.MotorControl.PlugBreakTimer = 5 * g_Ram.ramGroupC.BrakePause;
		g_Core.MotorControl.PlugBreakStep  = 1;
		break;
	case 1:
		if (g_Core.MotorControl.PlugBreakTimer > 0)	break;
		g_Ram.ramGroupH.ContGroup 	   	   = (g_Core.MotorControl.RequestDir > 0) ? cgOpen : cgClose;
		g_Core.MotorControl.PlugBreakTimer = 5 * g_Ram.ramGroupC.BrakeTime;
		g_Core.MotorControl.PlugBreakStep  = 2;
		break;
	case 2:
		if (g_Core.MotorControl.PlugBreakTimer > 0)	break;
		g_Core.MotorControl.WorkMode = wmStop;
	}
}

static void StartMode(void)
{
	if (g_Ram.ramGroupB.StopMethod == smDynBreak)
	{
		g_Core.MotorControl.WorkMode = wmShnStart;
		g_Core.MotorControl.ShnControlStepStart = 0;
		g_Core.MotorControl.ShnControlStepStop = 0;
	}
	else
	{
		g_Core.MotorControl.WorkMode = wmMove;
	}
	if (g_Peref.phaseOrder.Direction == -1)
	{
		if(g_Core.MotorControl.RequestDir == -1) g_Ram.ramGroupH.ContGroup = cgClose;
		if(g_Core.MotorControl.RequestDir == 1)	 g_Ram.ramGroupH.ContGroup = cgOpen;
	}
	else if (g_Peref.phaseOrder.Direction == 1)
	{
		if(g_Core.MotorControl.RequestDir == -1) g_Ram.ramGroupH.ContGroup = cgOpen;
		if(g_Core.MotorControl.RequestDir == 1)	 g_Ram.ramGroupH.ContGroup = cgClose;
	}
}

static void ShnStopMode(void)
{
	switch (g_Core.MotorControl.ShnControlStepStop)
		{
		case 0:

			g_Ram.ramGroupA.Status.bit.Stop = 1;

			g_Ram.ramGroupATS.Control1.all = 0x200F;
			g_Core.MotorControl.DinBreakTimer = g_Ram.ramGroupC.StopShnTime * BREAK_SCALE;
			g_Core.MotorControl.ShnControlStepStop = 1;
			break;

		case 1:
			if(g_Core.MotorControl.DinBreakTimer > 0) g_Core.MotorControl.DinBreakTimer--;
			if (g_Core.MotorControl.DinBreakTimer == 0)
			{
				g_Ram.ramGroupATS.Control1.all = 0x4000;
				g_Core.MotorControl.ShnControlStepStop = 2;
			}
			break;
		case 2:
			if (g_Ram.ramGroupATS.State1.bit.ReadyToSwitchOn==0)
			{
				g_Core.MotorControl.WorkMode = wmStop;
				g_Ram.ramGroupATS.Control1.all = 0;
				g_Core.MotorControl.ShnControlStepStop = 0;
			}
			break;



		}
}

static void ShnStartMode(void)
{
	switch (g_Core.MotorControl.ShnControlStepStart)
	{
	case 0:
		if (g_Ram.ramGroupATS.State1.bit.ReadyToSwitchOn == 0)
		{
			g_Ram.ramGroupATS.Control1.all = 0;
			g_Core.MotorControl.ShnControlStepStart = 1;
		}
		else
		{
			g_Ram.ramGroupATS.Control1.all = 0xb;
			g_Core.MotorControl.ShnControlStepStart = 1;
		}
		break;
	case 1:
			g_Ram.ramGroupATS.Control1.bit.ResetFaults = 1;
			if (g_Ram.ramGroupATS.Control1.bit.ResetFaults == 1 && g_Ram.ramGroupATS.State1.bit.Malfunction == 0)
				{
					g_Ram.ramGroupATS.Control1.bit.ResetFaults = 0;
					g_Core.MotorControl.ShnControlStepStart = 2;
				}
			break;
	case 2:
		if(g_Ram.ramGroupATS.State1.bit.ReadyToSwitchOn == 0)
		{
			g_Ram.ramGroupATS.Control1.all = 0x06;
			g_Core.MotorControl.ShnControlStepStart = 3;
		}
		else
		{
			g_Ram.ramGroupATS.Control1.all = 0;
			g_Core.MotorControl.ShnControlStepStart = 0;
		}
		break;
	case 3:
		if (g_Ram.ramGroupATS.State1.bit.ReadyToSwitchOn)
		{
			g_Ram.ramGroupATS.Control1.all = 0x07;
			g_Core.MotorControl.ShnControlStepStart = 4;
		}
		break;
	case 4:
		if (g_Ram.ramGroupATS.State1.bit.SwichedOn)
		{
			g_Ram.ramGroupATS.Control1.all = 0x0F;
			g_Core.MotorControl.WorkMode = wmMove;
			g_Core.MotorControl.ShnControlStepStart = 0;
		}
		break;
	}
//	g_Core.MotorControl.ShnControlStepStart++;
}

void Core_LowPowerControl(TCore *p)
{
    Uns ShCState = 0;

    if (p->Protections.FaultDelay > 0)
	return;

	// ������� ���������� �����----------------------------------------------------
	if ((g_Ram.ramGroupA.Ur < 60) && (g_Ram.ramGroupA.Us < 60)
			&& (g_Ram.ramGroupA.Ut < 60))
	{
		p->Protections.outDefects.Dev.bit.LowPower = 1;
		p->DisplayFaults.DisplFault = 999;
	}
	else
	{
		p->Protections.outDefects.Dev.bit.LowPower = 0;
	}



	ShCState = p->Protections.ShcTmpState & LOAD_SHC_MASK; //p->Protections.outFaults.Load.all & LOAD_SHC_MASK;

	// ����� ������� ������������ ��
	if (ShCState && !p->Protections.outDefects.Dev.bit.LowPower)
	    {
		if (p->ShcResetTimer++ >= (2 * Prd200HZ))
		    {
			p->ShcResetTimer = 0;
			ShCState = 0;
			p->Protections.ShcTmpState = 0;
		    }
	    }

	// ������ ��----------------------------------------------------------------------
	if (ShCState && !g_Ram.ramGroupH.ScFaults && p->Protections.outDefects.Dev.bit.LowPower)
	{
		if (IsMemParReady())
		{
			g_Ram.ramGroupH.ScFaults = ShCState;
			WriteToEeprom(REG_SHC_FAULT, &g_Ram.ramGroupH.ScFaults, 1);			// �� �������� ��������� ��
		}
		p->ShcResetTimer = 0;
	}
	if (g_Ram.ramGroupH.ScFaults)
	{
		p->Protections.outFaults.Load.all |= g_Ram.ramGroupH.ScFaults;
	}
	if (p->Protections.ShcReset && IsMemParReady())
	{
		p->Protections.ShcReset = false;
		g_Ram.ramGroupH.ScFaults = 0;
		WriteToEeprom(REG_SHC_FAULT, &g_Ram.ramGroupH.ScFaults, 1);
		p->Protections.outFaults.Load.bit.ShCU = 0;
		p->Protections.outFaults.Load.bit.ShCV = 0;
		p->Protections.outFaults.Load.bit.ShCW = 0;
	}

	// 3 sec -----------------------------------------------------------------------
	if (g_Ram.ramGroupB.Sec3Mode)
	{
	    if (p->Protections.outDefects.Dev.bit.LowPower)	// ���� �������	�������
	    {
		    p->Sec3Timer++;
		    p->PowerLostFlag = 1;
		    g_Ram.ramGroupD.ControlWord = vcwStop;
	    }
	    else if (p->PowerLostFlag && (!p->Protections.outDefects.Dev.bit.LowPower))	// ���� ������� ���������
	    {
		    if (p->Sec3Timer < (4 * Prd200HZ))
		    {
			    g_Ram.ramGroupD.ControlWord = (TValveCmd)p->SaveDirection;
		    }
		    else
			{
			    p->SaveDirection = 0;
			}
		    p->Sec3Timer = 0;
		    p->PowerLostFlag = 0;
		    p->SaveDirection = 0;
	    }
	    else											// ���� ������� ��� �� �������
	    {
		    p->Sec3Timer = 0;
		    if (p->Status.bit.Closing) p->SaveDirection = vcwClose;
		    else if (p->Status.bit.Opening) p->SaveDirection = vcwOpen;
	    }
	}

}

void Core_MuDuControl(TCore *p)
{
	switch (g_Ram.ramGroupB.MuDuSetup)
	{
		case mdOff:
			p->Protections.outDefects.Proc.bit.MuDuDef = 0;
			p->VlvDrvCtrl.Tu.LocalFlag = false;

			break;
		case mdMuOnly:
			p->Protections.outDefects.Proc.bit.MuDuDef = 0;
			p->VlvDrvCtrl.Tu.LocalFlag = false;

			break;
		case mdDuOnly:
			p->Protections.outDefects.Proc.bit.MuDuDef = 0;
			p->VlvDrvCtrl.Tu.LocalFlag = false;

			break;
		case mdSelect:
			{
				if(g_Ram.ramGroupB.PlaceType == ptFire)
				{
					g_Ram.ramGroupB.MuDuSetup = mdOff;
					break;
				}

				if(!g_Ram.ramGroupA.StateTu.bit.Mu && !g_Ram.ramGroupA.StateTu.bit.Du)
					{
						if(p->MuDuDefTimer++ > (2 * Prd10HZ))
						{
							p->VlvDrvCtrl.MuDuInput = 0;
							p->Protections.outFaults.Proc.bit.MuDuDef = 1;
							p->MuDuDefTimer = 0;
							p->VlvDrvCtrl.Tu.LocalFlag = false;

						}
					}
					if(g_Ram.ramGroupA.StateTu.bit.Mu && !g_Ram.ramGroupA.StateTu.bit.Du)
					{
						p->VlvDrvCtrl.MuDuInput = 1;
						p->Protections.outDefects.Proc.bit.MuDuDef = 0;
						p->MuDuDefTimer = 0;
						if (g_Ram.ramGroupB.SwitcherMuDuMode == 1)
						{
							p->VlvDrvCtrl.Tu.LocalFlag = true;
						}
						else p->VlvDrvCtrl.Tu.LocalFlag = false;

					}
					if(!g_Ram.ramGroupA.StateTu.bit.Mu && g_Ram.ramGroupA.StateTu.bit.Du)
					{
						p->VlvDrvCtrl.MuDuInput = 0;
						p->Protections.outDefects.Proc.bit.MuDuDef = 0;
						p->MuDuDefTimer = 0;
						p->VlvDrvCtrl.Tu.LocalFlag = false;
					}
					if(g_Ram.ramGroupA.StateTu.bit.Mu && g_Ram.ramGroupA.StateTu.bit.Du)
					{
						if(p->MuDuDefTimer++ > (2 * Prd10HZ))
						{
							p->VlvDrvCtrl.MuDuInput = 0;
							p->Protections.outDefects.Proc.bit.MuDuDef = 1;
							p->MuDuDefTimer = 0;
							p->VlvDrvCtrl.Tu.LocalFlag = false;
						}
					}
			}
			break;
	}

	if (g_Ram.ramGroupD.PrtReset)
	{
		if(!p->Status.bit.Stop) p->VlvDrvCtrl.Mpu.CancelFlag = true;
		else {
			Core_ProtectionsClear(&p->Protections);
			if (g_Ram.ramGroupB.StopMethod == smDynBreak )
			{
				g_Ram.ramGroupATS.Control1.bit.ResetFaults = 1;
			}
			p->VlvDrvCtrl.EvLog.Value = CMD_RES_FAULT;
		}
		g_Ram.ramGroupD.PrtReset = 0;
	}

	if (g_Ram.ramGroupATS.Control1.bit.ResetFaults == 1 && g_Ram.ramGroupATS.State1.bit.Malfunction == 0)
	{
		g_Ram.ramGroupATS.Control1.bit.ResetFaults = 0;
	}

}

void Core_OnOff_TEN(TCoreTemper *t)
{
	g_Ram.ramGroupA.TemperBKP = g_Ram.ramGroupH.BKP_Temper + g_Ram.ramGroupC.CorrTemper;
	if (g_Ram.ramGroupA.TemperBKP >= g_Ram.ramGroupC.TenOffValue) 		// ���� ����������� ������ ����������� ��������� - ��������� = 1
		t->OnOffTEN = TEN_OFF;
	else if (g_Ram.ramGroupA.TemperBKP <= g_Ram.ramGroupC.TenOnValue)	// ����� ���� ����������� ���� ����������� ��������� - �������� = 0
		t->OnOffTEN = TEN_ON ;
}

void Core_TechProgon(void)
{
    static Uns progonDelay = 0,			// �������� ����� ������, ����� ������ ��������� ������� �����
	    halfCycle = 0,			// �������� (������� -> ������� ��� ������� -> �������). ��� �������� ����� 1 �����
	    stopTimer = 0;			// ������ �������� ������ ������ �������, ���� ����
    static Byte isComandDone = false;		// ����, ������ �� �������. ������� ������ ���������� ������ 1 ��� �� ��������� ������� ��� �������

    if (g_Ram.ramGroupC.progonCycles == 0)
	return;

    if (g_Ram.ramGroupA.Status.bit.Fault || g_Ram.ramGroupA.CalibState != csCalib) 	// ��� ���������� ����� ��������� ������� �� ��������
    {
	g_Ram.ramGroupC.progonCycles = 0;
	g_Core.Status.bit.CycleMode = 0;
    }

    if (!g_Core.Status.bit.CycleMode)
    {
	if (g_Ram.ramGroupA.Status.bit.Closing || g_Ram.ramGroupA.Status.bit.Opening)
	    g_Core.Status.bit.CycleMode = 1;
    }
    else
    {
	if (g_Ram.ramGroupA.Status.bit.Closed || g_Ram.ramGroupA.Status.bit.Opened)
	{
	    stopTimer = 0;
	    if (!isComandDone)			// ���� ������� ��� �� ���� ������
	    {
		if (progonDelay++ > 50)	// 50 = 5 ��� �� 10 ��
		{
		    if (g_Ram.ramGroupC.progonCycles != 0)
		    {
			g_Ram.ramGroupD.ControlWord = g_Ram.ramGroupA.Status.bit.Closed ? vcwOpen : vcwClose;
		    }
		    isComandDone = true;
		    progonDelay = 0;
		    if (++halfCycle == 2) // ��� �������� = ���� ������ ����
		    {
			halfCycle = 0;
			g_Ram.ramGroupC.progonCycles--;
			if (g_Ram.ramGroupC.progonCycles == 0)
			{
			    progonDelay = 0;
			    g_Core.Status.bit.CycleMode = 0;
			}
		    }
		}
	    }
	}
	else
	{
	    isComandDone = false;
	    if (g_Ram.ramGroupA.Status.bit.Stop) // ���� ����������� ��� �������, �� �� ����� ������� ��������� ������� "����"
	    {
		if (stopTimer++ > 20)	// ������� ������ ������ ������� � ��������� � 2 �������
		{
		    g_Ram.ramGroupC.progonCycles = 0;
		    g_Core.Status.bit.CycleMode = 0;
		    stopTimer = 0;
		}
	    }
	}
    }
}









