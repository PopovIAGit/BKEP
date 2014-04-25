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
//#include "g_Ram.h"
//#include "comm.h"
//#include "stat.h"

#define IsAlarmsExist (	p->protections.processAlarmsExist|	\
						p->protections.netAlarmsExist|		\
						p->protections.loadAlarmsExist|		\
						p->protections.deviceAlarmsExist )
TCore	g_Core;

//---------------------------------------------------
void Core_Init(TCore *p)
{
	//Core_MenuInit(&p->menu);
	Core_ValveDriveInit(&p->VlvDrvCtrl);	// ���������� ���������
	Core_TorqueInit(&p->TorqObs);			// ������ ��������
	//Core_CommandsInit(&p->commands);		// ��������� ������, ��������� ����������
	//Core_ProtectionsInit(&p->protections);// ������

	p->Status.bit.Stop = 1;					// ��� ��������� ���������� ����
}

// ������� ������� ���������� ������� � ����������� �� ��������� � ����������� ��������
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
			else if (g_Ram.ramGroupA.CurWay >= CloseZone)
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
			else if (g_Ram.ramGroupA.CurWay >= CloseZone)
			{
				p->MotorControl.TorqueSet = g_Ram.ramGroupB.StartOpenTorque;
				p->MotorControl.BreakFlag = 0;
			}
		}
	}

	// ������������� ������� �� ������
	p->MotorControl.TorqueSetPr = (p->MotorControl.TorqueSet * 100)/p->TorqObs.TorqueMax;
}

// ��������� �� ����������
void Core_CalibStop (TCore *p)
{
	Bool StopFlag = False; // ��������� ���� ���������

	if(p->VlvDrvCtrl.Valve.Position == POS_UNDEF) //���� ������� ��������� �� ���������� �� ������
	{
		p->MotorControl.TargetPos = POS_UNDEF;
		return;
	}
	p->MotorControl.TargetPos = g_Peref.Position.LinePos - p->VlvDrvCtrl.Valve.Position;

	if(p->Status.bit.Stop) return;

	if((p->MotorControl.RequestDir < 0) && (p->MotorControl.TargetPos <= 0)) StopFlag = True;
	if((p->MotorControl.RequestDir > 0) && (p->MotorControl.TargetPos >= 0)) StopFlag = True;

	if (StopFlag)	// ���� ���� ���������������
	{
		if(p->VlvDrvCtrl.Valve.BreakFlag) p->MotorControl.OverWayFlag = 1;	//
		else	// ����
			{
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
			g_Ram.ramGroupD.TaskClose = trReset;

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
	}

	/*�������� ������ ���������� ���������� � ����� ������

	if (IsMemParReady())													// ���� ���� ���������� � ������ ����������
	{
		if (GrH->CycleCnt != PrevCycle)										// ���� ������� ������ ���������
		{
			WritePar(REG_CYCLE_CNT, &GrH->CycleCnt, 1);						// �������� �������� ������� ������
			PrevCycle = GrH->CycleCnt;										// ��������� ���������� ��������, ��� ����������� ��������
		}
		else if (GrH->CalibState != GrA->CalibState)						// ���� ��������� ���������� ����������
		{
			WritePar(REG_CALIB_STATE, &GrH->CalibState, sizeof(ClbIndication));// �� �������� ��������� ����������
			GrA->CalibState = GrH->CalibState;								// ���������� ���������� ��������, ��� ����������� ��������
		}
	}
	  */
}

// �������� ����������� ��� �����
void StopPowerControl(void)
{
	g_Core.Status.bit.Stop 		= 1;	// ��������� ������ ����
	g_Core.Status.bit.Opening 	= 0;
	g_Core.Status.bit.Closing 	= 0;
	g_Core.Status.bit.Test 		= 0;

	g_Ram.ramGroupH.ContGroup 	 = cgStop;	// ������ ������� �� ���� �����������
	g_Core.MotorControl.WorkMode = wmStop;  // ��������� � ����� ������ ����
	g_Core.VlvDrvCtrl.StartDelay = (Uns) START_DELAY_TIME; // ���������� �������� ����� ��������� ������
}

// �������� ��� �����
void StartPowerControl(TValveCmd ControlWord)
{
	//���� �� �� return

	switch (ControlWord)
	{
		case vcwClose:
			g_Core.MotorControl.RequestDir = -1;
			g_Core.MotorControl.WorkMode = wmMove;
			g_Ram.ramGroupH.ContGroup = cgClose;
			break;
		case vcwOpen:
			g_Core.MotorControl.RequestDir = 1;
			g_Core.MotorControl.WorkMode = wmMove;
			g_Ram.ramGroupH.ContGroup = cgOpen;
			break;
		case vcwTestClose:
			g_Core.MotorControl.RequestDir = -1;
			g_Core.MotorControl.WorkMode = wmMove;
			g_Core.Status.bit.Test = 1;
			g_Ram.ramGroupH.ContGroup = cgOpen;
			break;
		case vcwTestOpen:
			g_Core.MotorControl.RequestDir = 1;
			g_Core.MotorControl.WorkMode = wmMove;
			g_Core.Status.bit.Test = 1;
			g_Ram.ramGroupH.ContGroup = cgOpen;
			break;
	}

	// ����� ������ ����������� ��� �����
	//	ProtectionsReset();

	g_Core.Status.bit.Stop 			= 0;
	g_Core.MotorControl.TorqueSet 	= 0xFFFF;
	if(g_Core.MotorControl.RequestDir < 0) g_Core.Status.bit.Closing = 1;
	if(g_Core.MotorControl.RequestDir > 0) g_Core.Status.bit.Opening = 1;

}

// ����� ������, �������� ������ ������ � �������������
void Core_ControlMode(TCore *p)
{
	switch(p->MotorControl.WorkMode)
	{
	case wmStop:
		p->MotorControl.RequestDir = 0;		// ���������� ����������� ���������� ��������
		g_Ram.ramGroupA.Torque = 0;			// ���������� ������
		break;
	case wmMove:
		g_Ram.ramGroupA.Torque = p->TorqObs.Indication; // ���������� ������� ������
		break;
	}
}

//---------------��������� ��������---------------------------
void Core_StatusUpdate(TCore *p) // 50 ��
{
	/*
//	static LgUns prevPosition;
	static Uns t_TimerDisplayReset = 0;
	extern Uns NoCurrentPause;

	p->torque.current		= _IQmpy(_IQ(0.512), g_Peref.filter3IUrms.OutputIQ15);
	p->torque.loadAngle		= (LgInt)167772 * g_Ram.ramGroupA.LOAD_ANGLE_MID;
	p->torque.targetTorque	= (LgInt)setTorque * 167;
	
	//p->torque.current		= _IQmpy(_IQ(0.512), Tmp1_LgIntQ15);
	//p->torque.loadAngle		= (LgInt)167772 * Tmp2_Q0;
	//p->torque.targetTorque	= (LgInt)Tmp3_Q0 * 167;
	
	Core_TorqueCalc(&p->torque);
	if (p->status.bit.stop)
	{ 
		g_Ram.ramGroupA.TORQUE = 0;	// � ����� ������ ��������
		SifuControl  = 180;
		/////
		if (TestRun) TestOut = AngleInterp(TestStart, TestEnd, TestTime); 
	}
	else
	{  // �� � ����� ������ ������������ � ������������	� ��������������� �� ������ C1
		if (setTorque < p->torque.uporLowValue)
		{
			g_Ram.ramGroupA.TORQUE = (Uns)(p->torque.torque/167) + g_Ram.ramGroupC.CORR_MOVE1*40 - 2000;
			SifuControl  = (Uns)(p->torque.sifuAngle/167777) + g_Ram.ramGroupC.CORR_UPOR1/2 - 25;
		}
		else if (setTorque > p->torque.uporMidValue)
		{
		  	g_Ram.ramGroupA.TORQUE = (Uns)(p->torque.torque/167) + g_Ram.ramGroupC.CORR_MOVE3*40 - 2000;
			SifuControl  = (Uns)(p->torque.sifuAngle/167777) + g_Ram.ramGroupC.CORR_UPOR3/2 - 25;
		}
		else
		{
		 	g_Ram.ramGroupA.TORQUE = (Uns)(p->torque.torque/167) + g_Ram.ramGroupC.CORR_MOVE2*40 - 2000;
			SifuControl  = (Uns)(p->torque.sifuAngle/167777) + g_Ram.ramGroupC.CORR_UPOR2/2 - 25;
		}
		
		if ( (g_Ram.ramGroupA.U_MID < (220 - g_Ram.ramGroupC.CORR_MOVE4))|(g_Ram.ramGroupA.U_MID > (220 + g_Ram.ramGroupC.CORR_MOVE4)) )
			g_Ram.ramGroupA.TORQUE = g_Ram.ramGroupA.TORQUE/3;
		
		if ((p->protections.outAlarms.netAlarms.all & NETALARM_MASK)||
			(p->protections.outAlarms.loadAlarms.all & LOADALARM_MASK))
				 g_Ram.ramGroupA.TORQUE = 0;
		
			// � ����� ������ ������� � ������������ � ��������
		if (p->status.bit.upor)	
			g_Ram.ramGroupA.TORQUE = setTorque;
		if (p->status.bit.testOpenClose) 
			g_Ram.ramGroupA.TORQUE = 0;
	}
	p->status.bit.muDu 	= !IsLockMPU();
//--------����������� ������� ���-------------------------------
	//p->status.bit.ten = !g_Comm.localControl.inputData.bit.ten;
	if (g_Ram.ramGroupB.TEN_ENABLE)	// ���� ��� ��������
	{
		if (p->status.bit.ten) // ���� ��� �������
		{
			if (g_Ram.ramGroupA.TEMPER_PU >= g_Ram.ramGroupC.TEN_OFF_LEVEL)
				p->status.bit.ten = 0;
		}
		else					// ���� ��� ��������
		{
			if (g_Ram.ramGroupA.TEMPER_PU <= g_Ram.ramGroupC.TEN_ON_LEVEL)
				p->status.bit.ten = 1;
		}
	}
	else							// ����� (���� ��� ��������)
		p->status.bit.ten = 0;
//--------����������� ������� �������� �����---------
	p->status.bit.sleepMode = p->menu.SleepActive;
//--------����������� ������� ������--------------------------
	p->status.bit.fault = (IsAlarmsExist > 0) ? 1 : 0;

	//--------����������� �������� ������� � �������---------
	p->status.bit.closed = g_Peref.position.encoderStatus.bit.closed;
	p->status.bit.opened = g_Peref.position.encoderStatus.bit.opened;

// - - - - - - - - - - - ���� - - - - - - - - - - - - - - - - 
	if (p->status.bit.stop)								//���� �� � �����
	{
		p->params.timerNoMove = 0;
		p->params.uporTimer = 0;
		p->params.timerMaxTorque = 0;
		p->params.speed2uporTimer = 0;
	}
	//  ���� ���� ������, ��������������� ��������, ������ ����,
	else if (IsAlarmsExist & (LedTsStartStop|LedTsNotStartStop))
	{
		StopByFault(p);
	}
// - - - - - - - - - - - ���� - - - - - - - - - - - - - - - - 
	else if (p->status.bit.upor)							//���� �� � �������� � � �����
	{
		if (g_Ram.ramGroupC.SIFU_TEST) 
			g_Sifu.SetAngle = g_Ram.ramGroupC.SET_ANGLE;
		else if (p->status.bit.testOpenClose) 
			g_Sifu.SetAngle = 5;
		else 
			g_Sifu.SetAngle = SifuControl;
			
		if (++p->params.speed2uporTimer >= g_Ram.ramGroupC.TIME_ZAZOR*5) 
		{
			p->params.speed2uporTimer = g_Ram.ramGroupC.TIME_ZAZOR*5;
			if (g_Ram.ramGroupA.SPEED_P > g_Ram.ramGroupB.SELECT_SPEED)		// ��� ����. ��������
			{	   															// �� ������ � ��������
				p->status.bit.upor = 0;	
				p->params.timerNoMove = 0;
				p->params.uporTimer = 0;
				p->params.speed2uporTimer = 0;						
			}
			else
			{
				if (++p->params.uporTimer >= g_Ram.ramGroupB.UPOR_TIMEOUT*5)
				{
					p->protections.outAlarms.loadAlarms.bit.mufta = 1;
					StopByFault(p);
					p->params.timerNoMove = 0;
					p->params.uporTimer = 0;
				}
			}
		}
	}
// - - - - - - - - - ���������� - - - - - - - - - - - - - - - 
	else if (p->status.bit.braking)			//���� �� ��������� � ����������
	{
	p->params.timerBraking++; // ������ ������ ������ ����������
	
	switch (g_Ram.ramGroupC.BRAKING_MODE)
	{
	case 11: // ������ ���������������� �� �������
		{
		if (p->params.timerBraking <= g_Ram.ramGroupC.REVERS_TIME * 5) // ����������������
			{
			if ((g_Sifu.Direction == dmForward) && (!fl_DirectionIsSetted) )
				{
				g_Sifu.Direction = dmBackward;
				fl_DirectionIsSetted = true;
				}
			else if ( (g_Sifu.Direction == dmBackward) && (!fl_DirectionIsSetted) )
					{
					g_Sifu.Direction = dmForward;
					fl_DirectionIsSetted = true;
					}
			if (g_Ram.ramGroupC.REV_START_LEV <= g_Ram.ramGroupC.REV_END_LEV)
				g_Sifu.OpenAngle = g_Ram.ramGroupC.REV_END_LEV;
			else
				{
				g_Sifu.OpenAngle = AngleInterp(g_Ram.ramGroupC.REV_START_LEV, g_Ram.ramGroupC.REV_END_LEV, g_Ram.ramGroupC.REVERS_TIME); 
				}
			}
		else // �����
			{
			p->params.timerBraking = 0;
			p->params.DynBrakeComplite = 1;
			StopByCommand(p, brakeOff);
			}
		} break;

	case 12: // ������ ���������������� �� ��������
		{
		if ((g_Peref.position.speedRPM > g_Ram.ramGroupC.REV_2_DYNBRAKE_LEV)&&(p->params.timerBraking <= (g_Ram.ramGroupC.REVERS_TIME + g_Ram.ramGroupC.DYNBRAKE_TIME)*5))
			{
			if ((g_Sifu.Direction == dmForward) && (!fl_DirectionIsSetted) )
				{
				g_Sifu.Direction = dmBackward;
				fl_DirectionIsSetted = true;
				}
			else if ( (g_Sifu.Direction == dmBackward) && (!fl_DirectionIsSetted) )
					{
					g_Sifu.Direction = dmForward;
					fl_DirectionIsSetted = true;
					}
			if (g_Ram.ramGroupC.REV_START_LEV <= g_Ram.ramGroupC.REV_END_LEV)
				g_Sifu.OpenAngle = g_Ram.ramGroupC.REV_END_LEV;
			else
				{
				g_Sifu.OpenAngle = AngleInterp(g_Ram.ramGroupC.REV_START_LEV, g_Ram.ramGroupC.REV_END_LEV, g_Ram.ramGroupC.REVERS_TIME);
				}
			}
		else // �����
			{
			p->params.timerBraking = 0;
			p->params.DynBrakeComplite = 1;
			StopByCommand(p, brakeOff);
			}
		} break;

	case 21: // ������ ������������ ������. �� �������
		{
		if (p->params.timerBraking <= g_Ram.ramGroupC.DYNBRAKE_TIME * 5) // ����������������
			{
			g_Sifu.Direction = dmDynBraking;
			if (g_Ram.ramGroupC.DYNBRAKE_START_LEV <= g_Ram.ramGroupC.DYNBRAKE_END_LEV)
				g_Sifu.dynBrakeLevel = g_Ram.ramGroupC.DYNBRAKE_END_LEV;
			else
				{
				g_Sifu.dynBrakeLevel = AngleInterp(g_Ram.ramGroupC.DYNBRAKE_START_LEV, g_Ram.ramGroupC.DYNBRAKE_END_LEV, g_Ram.ramGroupC.DYNBRAKE_TIME); 
				}
			}
		else // �����
			{
			p->params.timerBraking = 0;
			p->params.DynBrakeComplite = 1;
			StopByCommand(p, brakeOff);
			}
		} break;

	case 22: // ������ ������������ ������. �� ��������
		{
		if ((g_Peref.position.speedRPM > g_Ram.ramGroupC.REV_2_DYNBRAKE_LEV)&&(p->params.timerBraking <= (g_Ram.ramGroupC.REVERS_TIME + g_Ram.ramGroupC.DYNBRAKE_TIME)*5))
			{
			g_Sifu.Direction = dmDynBraking;
			if (g_Ram.ramGroupC.DYNBRAKE_START_LEV <= g_Ram.ramGroupC.DYNBRAKE_END_LEV)
				g_Sifu.dynBrakeLevel = g_Ram.ramGroupC.DYNBRAKE_END_LEV;
			else
				{
				g_Sifu.dynBrakeLevel = AngleInterp(g_Ram.ramGroupC.DYNBRAKE_START_LEV, g_Ram.ramGroupC.DYNBRAKE_END_LEV, g_Ram.ramGroupC.DYNBRAKE_TIME); 
				}
			}
		else // �����
			{
			p->params.timerBraking = 0;
			p->params.DynBrakeComplite = 1;
			StopByCommand(p, brakeOff);
			}

		} break;

	case 31: // ���������������� + ������������ ������. �� �������
		{
		if (p->params.timerBraking <= g_Ram.ramGroupC.REVERS_TIME * 5) // ����������������
			{
			if ((g_Sifu.Direction == dmForward) && (!fl_DirectionIsSetted) )
				{
				g_Sifu.Direction = dmBackward;
				fl_DirectionIsSetted = true;
				}
			else if ( (g_Sifu.Direction == dmBackward) && (!fl_DirectionIsSetted) )
					{
					g_Sifu.Direction = dmForward;
					fl_DirectionIsSetted = true;
					}
			if (g_Ram.ramGroupC.REV_START_LEV <= g_Ram.ramGroupC.REV_END_LEV)
				g_Sifu.OpenAngle = g_Ram.ramGroupC.REV_END_LEV;
			else
				{
				g_Sifu.OpenAngle = AngleInterp(g_Ram.ramGroupC.REV_START_LEV, g_Ram.ramGroupC.REV_END_LEV, g_Ram.ramGroupC.REVERS_TIME); 
				}
			}
			else if (p->params.timerBraking <= (g_Ram.ramGroupC.REVERS_TIME + g_Ram.ramGroupC.DYNBRAKE_TIME)*5) // ������������
				 	{
					g_Sifu.Direction = dmDynBraking;
					if (g_Ram.ramGroupC.DYNBRAKE_START_LEV <= g_Ram.ramGroupC.DYNBRAKE_END_LEV)
						g_Sifu.dynBrakeLevel = g_Ram.ramGroupC.DYNBRAKE_END_LEV;
					else
						{
						g_Sifu.dynBrakeLevel = AngleInterp(g_Ram.ramGroupC.DYNBRAKE_START_LEV, g_Ram.ramGroupC.DYNBRAKE_END_LEV, g_Ram.ramGroupC.DYNBRAKE_TIME); 
						}
					}
				 else // �����
					{
					p->params.timerBraking = 0;
					p->params.DynBrakeComplite = 1;
					StopByCommand(p, brakeOff);
					}
		} break;

	case 32: // ���������������� + ������������ ������. �� ������ ��������
		{
		if ((g_Peref.position.speedRPM > g_Ram.ramGroupC.REV_2_DYNBRAKE_LEV)&&(p->params.timerBraking <= (g_Ram.ramGroupC.REVERS_TIME + g_Ram.ramGroupC.DYNBRAKE_TIME)*5))
			{
			if ((g_Sifu.Direction == dmForward) && (!fl_DirectionIsSetted) )
				{
				g_Sifu.Direction = dmBackward;
				fl_DirectionIsSetted = true;
				}
			else if ( (g_Sifu.Direction == dmBackward) && (!fl_DirectionIsSetted) )
					{
					g_Sifu.Direction = dmForward;
					fl_DirectionIsSetted = true;
					}
			if (g_Ram.ramGroupC.REV_START_LEV <= g_Ram.ramGroupC.REV_END_LEV)
				g_Sifu.OpenAngle = g_Ram.ramGroupC.REV_END_LEV;
			else
				{
				g_Sifu.OpenAngle = AngleInterp(g_Ram.ramGroupC.REV_START_LEV, g_Ram.ramGroupC.REV_END_LEV, g_Ram.ramGroupC.REVERS_TIME);
				}
			}
			else if (p->params.timerBraking <= (g_Ram.ramGroupC.REVERS_TIME + g_Ram.ramGroupC.DYNBRAKE_TIME)*5)
				 	{
					g_Sifu.Direction = dmDynBraking;
					if (g_Ram.ramGroupC.DYNBRAKE_START_LEV <= g_Ram.ramGroupC.DYNBRAKE_END_LEV)
						g_Sifu.dynBrakeLevel = g_Ram.ramGroupC.DYNBRAKE_END_LEV;
					else
						{
						g_Sifu.dynBrakeLevel = AngleInterp(g_Ram.ramGroupC.DYNBRAKE_START_LEV, g_Ram.ramGroupC.DYNBRAKE_END_LEV, g_Ram.ramGroupC.DYNBRAKE_TIME);
						}
					}
				 else // �����
					{
					p->params.timerBraking = 0;
					p->params.DynBrakeComplite = 1;
					StopByCommand(p, brakeOff);
					}
		} break;

	default:
		{	// ������������ ��������� => ����� ������ ����������
		p->params.timerBraking = 0;
		p->params.DynBrakeComplite = 1;
		StopByCommand(p, brakeOff);
		} break; 
	}


	}
// - - - - - - - - - - �������� - - - - - - - - - - - - - - - 
	else if (p->status.bit.closing|p->status.bit.opening)	//���� �� � ��������
	{
		if (g_Ram.ramGroupC.SIFU_TEST) 
			g_Sifu.SetAngle = g_Ram.ramGroupC.SET_ANGLE;
		else 
		{
			if (p->params.speed_ctrl) g_Sifu.SetAngle = 111;
			else g_Sifu.SetAngle = 5;
		}

		if (p->status.bit.testOpenClose)	// ���� ��������� � �������� ������
			return;							// �������. �� ������� �� �� ���������, �� �� ������, �� �� ��������, �� �� ����������� ���
		if (g_Ram.ramGroupA.TORQUE >= setTorque) //�������� �������
		{
			if (++p->params.timerMaxTorque >= g_Ram.ramGroupB.UPOR_TIMEOUT*5) 
			{
				if (g_Ram.ramGroupB.UPOR_MODE)
				{
					p->status.bit.upor = 1;					//�� ����� �������� - � ����
					p->params.speed2uporTimer = 0;
				}
				else 
				{
					p->protections.outAlarms.loadAlarms.bit.mufta = 1;
					StopByFault(p);
					p->params.timerNoMove = 0;
					p->params.uporTimer = 0;
				}
			p->params.timerMaxTorque = 0;
			}
		} 
		else p->params.timerMaxTorque = 0;
		
		if ((p->status.bit.closing)&&(p->status.bit.closed)) 
		{
			if ((g_Ram.ramGroupB.BRAKING_ENABLE == 1)||(g_Ram.ramGroupB.BRAKING_ENABLE == 2))
				StopByCommand(p, brakeOn);
			else
				StopByCommand(p, brakeOff);	
		}
		if ((p->status.bit.opening)&&(p->status.bit.opened)) 
		{
			if ((g_Ram.ramGroupB.BRAKING_ENABLE == 1)||(g_Ram.ramGroupB.BRAKING_ENABLE == 2))
				StopByCommand(p, brakeOn);
			else
				StopByCommand(p, brakeOff);
		}
	}
//-------------- ����� ������� ��� �����. ������������ �� � ������ ������--------
	if ( (g_Ram.ramGroupA.TEMPER_PU < 0)&&(!p->status.bit.sleepMode) )
	{
		if (++t_TimerDisplayReset > 500)	// 10 c
		{
			DisplayReset(&g_Peref.display, g_Ram.ramGroupB.INDICATOR_TYPE);
			t_TimerDisplayReset = 0;
		}
	}
//-------------- ������ ��������� -----------------------------
	if (!HeatMotorControl(p)) Core_StopHeatMotor(p);

//--------����������� ������� ������ ���������---------
	p->status.bit.heatMotor = p->heatMotor.status.bit.active;
}
//--------------------------------------------------------

}*/


}
