/*======================================================================
Имя файла:          core.c
Автор:
Версия файла:
Дата изменения:
Описание: Работа ЯДРА
======================================================================*/

#include "core.h"
#include "peref.h"
#include "g_Structs.h"
#include "comm.h"
#include "stat.h"
#include "stat_fm25v10.h"

TFM25V10 Eeprom1;
TFM25V10 Eeprom2;

Uns LVS_flag  	 = 0;
Uns ShnModeTimer = 0;

TCore	g_Core;

//выбор микросхем
__inline void Eeprom1CsSet(Byte Lev)  {SC_EEPROM1 = !Lev;}
__inline void Eeprom2CsSet(Byte Lev)  {SC_EEPROM2 = !Lev;}

static void MoveMode (void);
static void StopMode(void);
static void PlugBreakMode(void);
static void ShnControlMode(void);
static void StartMode(void);
static void ShnControlErrCheck(void);

//---------------------------------------------------
void Core_Init(TCore *p)
{
	// Инициализация епром
	Eeprom1.CsFunc = &Eeprom1CsSet;
	FM25V10_Init(&Eeprom1);
	Eeprom2.CsFunc = &Eeprom2CsSet;
	FM25V10_Init(&Eeprom2);

	// Значения параметров присваиваются
	Core_MenuInit(&p->menu);

	Core_ValveDriveInit(&p->VlvDrvCtrl);	// Управление задвижкой
	Core_TorqueInit(&p->TorqObs);			// Расчет моментов
	//Core_CommandsInit(&p->commands);		// Получение команд, настройка калибровки
	Core_ProtectionsInit(&p->Protections);	// Защиты

	p->Status.bit.Stop = 1;					// При включение выставляем стоп
}

// Функция задания момента в зависимости от положения и направления движения
void Core_DefineCtrlParams(TCore *p) // 50 hz
{
	Int MaxZone, CloseZone, OpenZone;	// максимальный размер зоны, зона открытия, зона закрытия

	if(g_Ram.ramGroupB.ValveType == vtShiber)		// Если задвижка шиберная
	{
		g_Ram.ramGroupB.BreakMode = vtNone;			// всегда работаем без уплотнения
	}

	MaxZone = (g_Ram.ramGroupA.FullWay >> 1) - 1;	// определяем максимальный размер зон открыто/закрыто

	CloseZone = g_Ram.ramGroupB.CloseZone;
	if (CloseZone > MaxZone) CloseZone = MaxZone; // если заданя больше максимума задаем максимум

	OpenZone = g_Ram.ramGroupB.OpenZone;
	if (OpenZone > MaxZone) OpenZone = MaxZone;   // если заданя больше максимума задаем максимум
	OpenZone = g_Ram.ramGroupA.FullWay - OpenZone;

	if(!p->MotorControl.RequestDir) p->MotorControl.TorqueSet = 0; // Если не задано необходиоме направление вращения то задние момента 0
	else if(p->MotorControl.RequestDir > 0)	 // Выставляем задание на момент в зависимости от направления и калибровки
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

	// пересчитываем задание на момент
	p->MotorControl.TorqueSetPr = (Uns)(((LgUns)p->MotorControl.TorqueSet * 100)/p->TorqObs.TorqueMax);
}

// Остановка по калибровке
void Core_CalibStop (TCore *p)
{
	Bool StopFlag = False; // внутенний флаг остановки
	LgInt Position = p->VlvDrvCtrl.Valve.Position;

	if(p->VlvDrvCtrl.Valve.Position == POS_UNDEF) //Если целевое положение не определено то уходим
	{
		p->MotorControl.TargetPos = POS_UNDEF;
		return;
	}
	p->MotorControl.TargetPos = (g_Peref.Position.LinePos - Position);

	if(p->Status.bit.Stop) return;

	//if((p->MotorControl.RequestDir < 0) && (p->MotorControl.TargetPos <= 0)) StopFlag = True;
	//if((p->MotorControl.RequestDir > 0) && (p->MotorControl.TargetPos >= 0)) StopFlag = True;

	if((p->MotorControl.RequestDir < 0) && (p->MotorControl.TargetPos <= g_Ram.ramGroupC.BreakZone)) StopFlag = True;
	if((p->MotorControl.RequestDir > 0) && (p->MotorControl.TargetPos >= -g_Ram.ramGroupC.BreakZone)) StopFlag = True;

	if (StopFlag)	// Если пора останавливаться
	{
		if(p->VlvDrvCtrl.Valve.BreakFlag) p->MotorControl.OverWayFlag = 1;	//
		else	// Если
			{
				p->MotorControl.CalibStop = 1;
				Core_ValveDriveStop(&p->VlvDrvCtrl);
				p->VlvDrvCtrl.EvLog.Value = CMD_STOP;
			}
	}
}

// Управление калибровкой
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

	// Команда на сброс счетчика циклов
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

	if (IsMemParReady())																	// если есть готовность к записи параметров
	{
		if (g_Ram.ramGroupH.CycleCnt != p->PrevCycle)										// если счетчик циклов обновился
		{
			WriteToEeprom(REG_CYCLE_CNT, &g_Ram.ramGroupH.CycleCnt, 1);						// записали параметр счетчик циклов
			p->PrevCycle = g_Ram.ramGroupH.CycleCnt;										// запомнили записанный параметр, для последующей проверки
		}
		else if (g_Ram.ramGroupH.CalibState != g_Ram.ramGroupA.CalibState)					// если состояние калибровки изменилось
		{
			WriteToEeprom(REG_CALIB_STATE, &g_Ram.ramGroupH.CalibState, sizeof(ClbIndication));	// то записали состояние калибровки
			g_Ram.ramGroupA.CalibState = g_Ram.ramGroupH.CalibState;
		}
	}
}

// Действия выполняемые при стопе
void StopPowerControl(void)
{
	g_Core.Status.bit.Stop 		= 1;	// выставили статус стоп
	g_Core.Status.bit.Opening 	= 0;
	g_Core.Status.bit.Closing 	= 0;
	g_Core.Status.bit.Test 		= 0;

	if (g_Core.Status.bit.Fault)
	{
		g_Core.MotorControl.WorkMode = wmStop;  // Переходим в стейт машину стоп
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
	else g_Core.MotorControl.WorkMode = wmStop;

	g_Core.MotorControl.CalibStop = 0;
	g_Core.VlvDrvCtrl.StartDelay = (Uns)START_DELAY_TIME; // Выставляем задержку перед следующим пуском
	g_Core.TorqObs.ObsEnable = false;


	// сброс аварии при стопе для шнайдера
	/*if(g_Comm.Shn.SHN_StopFlag == 1)
	{
		g_Comm.Shn.SHN_StopFlag 				= 0;
		g_Comm.SHN_Regs.Control.all 			= 0;
		g_Comm.SHN_Regs.Control.bit.ResetFaults = 1;
		g_Comm.Shn.SHN_WriteFlag 				= 1;
	}*/
	/*if(g_Comm.Shn.SHN_StopFlag == 1 && !g_Core.MotorControl.CalibStop)
	{
		g_Comm.Shn.SHN_StopFlag 				= 0;
		g_Comm.SHN_Regs.Control.all 			= 0x200f;
		g_Comm.Shn.SHN_WriteFlag 				= 1;
	}*/
}

// Действия при пуске
void StartPowerControl(TValveCmd ControlWord)
{
	//Если КЗ то return
	if (g_Ram.ramGroupA.Faults.Load.all & LOAD_SHC_MASK) return;

	// сброс аварий необходимый для пуска
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
		//	g_Ram.ramGroupH.ContGroup = cgClose;
			break;
		case vcwOpen:
			g_Core.MotorControl.RequestDir = 1;
			g_Core.MotorControl.WorkMode = wmStart;
		//	g_Ram.ramGroupH.ContGroup = cgOpen;
			break;
		case vcwTestClose:
			g_Core.MotorControl.RequestDir = -1;
			g_Core.MotorControl.WorkMode = wmStart;
			g_Core.Status.bit.Test = 1;
			//g_Ram.ramGroupH.ContGroup = cgClose;
			break;
		case vcwTestOpen:
			g_Core.MotorControl.RequestDir = 1;
			g_Core.MotorControl.WorkMode = wmStart;
			g_Core.Status.bit.Test = 1;
		//	g_Ram.ramGroupH.ContGroup = cgOpen;
			break;
	}
}

// Стэйт машина
void Core_ControlMode(TCore *p) // 50 Гц
{
    p->Status.bit.Mufta = p->Protections.outFaults.Proc.bit.Mufta;

    switch(p->MotorControl.WorkMode)
    {
    case wmStop:		StopMode();	break;
	case wmPlugBreak:	PlugBreakMode();	break;
	case wmShnStop:		ShnControlMode();break;
	case wmStart:		StartMode();break;
	case wmShnStart:	ShnControlMode();break;
	case wmMove:		MoveMode(); break;
//	case wmSpeedTest:	break; ???
    }
}

static void StopMode(void)
{
	/*if(g_Comm.Shn.SHN_StopFlag == 1)
	{
		g_Comm.SHN_Regs.Control.all = 0;
		g_Comm.SHN_Regs.Control.bit.ResetFaults = 1;
		g_Comm.Shn.SHN_WriteFlag = 1;
	}*/

	g_Core.MotorControl.RequestDir  = 0;		// сбрасываем необходимое направение движения
	g_Core.MotorControl.ShnControlErrTimer = 0;
	g_Ram.ramGroupA.Torque 			= 0;		// отображаем момент
	g_Ram.ramGroupH.ContGroup 		= cgStop; 	// Подали команду на стоп контакторам
}

static void MoveMode(void)
{
	g_Ram.ramGroupA.Torque = g_Core.TorqObs.Indication; // отображаем текущий момент

	if (CONTACTOR_1_STATUS && g_Core.MotorControl.RequestDir < 0)  g_Core.Status.bit.Closing = 1;
	if (CONTACTOR_2_STATUS && g_Core.MotorControl.RequestDir > 0)  g_Core.Status.bit.Opening = 1;

	if(g_Core.TorqObs.Indication < g_Core.MotorControl.TorqueSet)
		g_Core.MotorControl.MufTimer = 0;
	else if (++g_Core.MotorControl.MufTimer >= (5 * g_Ram.ramGroupB.MuffTimer))
		g_Core.Protections.MuffFlag = 1;	// выставляем муфту если в течении секунды момент больше заданного

}

static void PlugBreakMode(void)
{
	g_Ram.ramGroupA.Torque = 0;				// отображаем момент

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
		g_Ram.ramGroupH.ContGroup 	   	   = (g_Core.MotorControl.RequestDir > 0) ? cgClose : cgOpen;
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
	/*if ((g_Ram.ramGroupB.StopMethod == smDynBreak)
			&& (g_Comm.SHN_Regs.Status.bit.Malfunction==1))
	{
		ShnControlErrCheck();

		if (!g_Comm.Shn.SHN_WriteFlag)
		{
			g_Comm.SHN_Regs.Control.all = 0;
			g_Comm.SHN_Regs.Control.bit.ResetFaults = 1;
			g_Comm.Shn.SHN_WriteFlag = 1;
			g_Comm.Shn.SHN_ReadFlag = 1;
			g_Comm.Shn.SHN_Busy = 0;
		}
		return;
	}*/

	if (g_Ram.ramGroupB.StopMethod == smDynBreak)
	{
		g_Core.MotorControl.WorkMode = wmShnStart;
		g_Core.MotorControl.ShnControlStep = 2;
	}
	else
	{
		g_Core.MotorControl.WorkMode = wmMove;
	}

	if(g_Core.MotorControl.RequestDir == -1) g_Ram.ramGroupH.ContGroup = cgClose;
	if(g_Core.MotorControl.RequestDir == 1)	 g_Ram.ramGroupH.ContGroup = cgOpen;
}

static void ShnControlMode(void)
{
	ShnControlErrCheck();

	if(g_Core.MotorControl.DinBreakTimer > 0) g_Core.MotorControl.DinBreakTimer--;

	if (g_Comm.Shn.SHN_WriteFlag) return;

	switch(g_Core.MotorControl.ShnControlStep)
	{
	case 0:
		g_Ram.ramGroupA.Status.bit.Stop = 1;
		//LVS_flag = 2;
	//	if(g_Core.MotorControl.CalibStop)
	//	{
			g_Comm.SHN_Regs.Control.all 	= 0x200F;
			g_Comm.mbShn.Packet.Request 	= 16;
			g_Comm.mbShn.Packet.Addr		= 400;
			g_Comm.mbShn.Packet.Response 	= 0;
			g_Comm.mbShn.Frame.WaitResponse = false;
			g_Comm.mbShn.Packet.Data[0] 	= 0x200F;
			g_Comm.Shn.SHN_Busy 			= 1;
			g_Comm.Shn.SHN_Mode=2;
		//}
		//else g_Comm.SHN_Regs.Control.all = 0;
		g_Comm.Shn.SHN_WriteFlag = 0;
		g_Core.MotorControl.DinBreakTimer = g_Ram.ramGroupC.StopShnTime * 5;//* BREAK_SCALE;
		g_Core.MotorControl.ShnControlStep = 1;
		return;
	case 1:
		g_Core.MotorControl.ShnControlErrTimer = 0;
		if(!g_Core.MotorControl.DinBreakTimer) g_Core.MotorControl.WorkMode = wmStop;
		return;
	//-------------------------------------------------------------
	case 2:
		LVS_flag = 0;
		//g_Comm.Shn.SHN_WriteFlag = 1;
		g_Comm.Shn.SHN_Mode=0;
		g_Comm.mbShn.Packet.Addr = ATS48_BIG_CONTROL_REG;
		g_Comm.mbShn.Packet.Count = 1;
		g_Comm.mbShn.Packet.Data[0] = 16384;

		//ShnModeTimer =
		g_Comm.SHN_Regs.Control.all = 0;
		break;
	case 3:
		g_Comm.Shn.SHN_Mode=1;
		//if (g_Comm.Shn.SHN_WriteFlag !=0) return;
		//g_Comm.Shn.SHN_Mode=0;
		//g_Comm.mbShn.Packet.Addr = ATS48_BIG_CONTROL_REG;
		//g_Comm.mbShn.Packet.Count = 1;
		g_Comm.SHN_Regs.Control.all = 0;
		break;
	case 4:
		//if (!(SHN_Data[0]&BIT14)) return;
		g_Comm.SHN_Regs.Control.all = 0;
		g_Comm.SHN_Regs.Control.bit.ResetFaults = 1;
		break;
	case 5:
		g_Comm.SHN_Regs.Control.all = 0;
		g_Comm.SHN_Regs.Control.bit.EnableVoltage = 1;
		g_Comm.SHN_Regs.Control.bit.DisableQuickStop = 1;
		break;
	case 6:
		//if(!g_Comm.SHN_Regs.Status.bit.ReadyToSwitchOn) return;
		g_Comm.SHN_Regs.Control.bit.SwitchOn = 1;
		break;
	case 7:
		//if(!g_Comm.SHN_Regs.Status.bit.SwichedOn) return;
		g_Comm.SHN_Regs.Control.bit.EnableOp = 1;
		break;
	case 8:
		//if(!g_Comm.SHN_Regs.Status.bit.OperationEnabled) return;
		g_Core.MotorControl.WorkMode = wmMove;
		g_Core.MotorControl.ShnControlStep = 0;
		return;
	}

	g_Comm.Shn.SHN_WriteFlag = 1;
	//g_Comm.Shn.SHN_Busy = 0;
	g_Core.MotorControl.ShnControlStep++;
	g_Core.MotorControl.ShnControlErrTimer = 0;
}

// Функция для остановки работу если упп не отзывается
static void ShnControlErrCheck(void)
{
	g_Core.MotorControl.ShnControlErrTimer++;
	if (g_Core.MotorControl.ShnControlErrTimer > 250)//SHN_CONTROL_ERR_TIME)
	{
		g_Core.MotorControl.WorkMode = wmStop;
		g_Core.MotorControl.ShnControlErr = 1;
	}
}


void Core_LowPowerControl(TCore *p)
{
    Uns ShCState = 0;

    if (p->Protections.FaultDelay > 0)
	return;

	// Событие выключения блока----------------------------------------------------
	if ((g_Ram.ramGroupA.Ur < 60) && (g_Ram.ramGroupA.Us < 60)
			&& (g_Ram.ramGroupA.Ut < 60))
	{
		p->Protections.outDefects.Dev.bit.LowPower = 1;
	}
	else p->Protections.outDefects.Dev.bit.LowPower = 0;


	ShCState = p->Protections.ShcTmpState & LOAD_SHC_MASK; //p->Protections.outFaults.Load.all & LOAD_SHC_MASK;

	// сброс ложного срабатывания КЗ
	if (ShCState && !p->Protections.outDefects.Dev.bit.LowPower)
	    {
		if (p->ShcResetTimer++ >= (2 * Prd200HZ))
		    {
			p->ShcResetTimer = 0;
			ShCState = 0;
			p->Protections.ShcTmpState = 0;
		    }
	    }

	// Запись КЗ----------------------------------------------------------------------
	if (ShCState && !g_Ram.ramGroupH.ScFaults && p->Protections.outDefects.Dev.bit.LowPower)
	{
		if (IsMemParReady())
		{
			g_Ram.ramGroupH.ScFaults = ShCState;
			WriteToEeprom(REG_SHC_FAULT, &g_Ram.ramGroupH.ScFaults, 1);			// то записали состояние КЗ
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
	    if (p->Protections.outDefects.Dev.bit.LowPower)	// Если питание	пропало
	    {
		    p->Sec3Timer++;
		    p->PowerLostFlag = 1;
		    g_Ram.ramGroupD.ControlWord = vcwStop;
	    }
	    else if (p->PowerLostFlag && (!p->Protections.outDefects.Dev.bit.LowPower))	// Если питание вернулось
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
	    else											// Если питание еще не пропало
	    {
		    p->Sec3Timer = 0;
		    if (p->Status.bit.Closing) p->SaveDirection = vcwClose;
		    else if (p->Status.bit.Opening) p->SaveDirection = vcwOpen;
	    }
	}
	//--------------------------------------------------------------------------
}

void Core_MuDuControl(TCore *p)
{
	switch (g_Ram.ramGroupB.MuDuSetup)
	{
		case mdOff:
			p->Protections.outDefects.Proc.bit.MuDuDef = 0;
			break;
		case mdMuOnly:
			p->Protections.outDefects.Proc.bit.MuDuDef = 0;
			break;
		case mdDuOnly:
			p->Protections.outDefects.Proc.bit.MuDuDef = 0;
			break;
		case mdSelect:
			{
				if(!g_Ram.ramGroupA.StateTu.bit.Mu && !g_Ram.ramGroupA.StateTu.bit.Du)
				{
					p->VlvDrvCtrl.MuDuInput = 0;
					p->Protections.outDefects.Proc.bit.MuDuDef = 1;
				}
				if(g_Ram.ramGroupA.StateTu.bit.Mu && !g_Ram.ramGroupA.StateTu.bit.Du)
				{
					p->VlvDrvCtrl.MuDuInput = 1;
					p->Protections.outDefects.Proc.bit.MuDuDef = 0;
				}
				if(!g_Ram.ramGroupA.StateTu.bit.Mu && g_Ram.ramGroupA.StateTu.bit.Du)
				{
					p->VlvDrvCtrl.MuDuInput = 0;
					p->Protections.outDefects.Proc.bit.MuDuDef = 0;
				}
				if(g_Ram.ramGroupA.StateTu.bit.Mu && g_Ram.ramGroupA.StateTu.bit.Du)
				{
					p->VlvDrvCtrl.MuDuInput = 0;
					p->Protections.outDefects.Proc.bit.MuDuDef = 1;
				}
			}
			break;

	}

	if (g_Ram.ramGroupD.PrtReset)
	{
		if(!p->Status.bit.Stop) p->VlvDrvCtrl.Mpu.CancelFlag = true;
		else {
			Core_ProtectionsClear(&p->Protections);
			p->VlvDrvCtrl.EvLog.Value = CMD_RES_FAULT;
		}
		g_Ram.ramGroupD.PrtReset = 0;
	}
}

void Core_OnOff_TEN(TCoreTemper *t)
{
	//t->CurrTemper = g_Ram.ramGroupA.TemperBKP + g_Ram.ramGroupC.CorrTemper;
	g_Ram.ramGroupA.TemperBKP = g_Ram.ramGroupH.BKP_Temper + g_Ram.ramGroupC.CorrTemper;
	if (g_Ram.ramGroupA.TemperBKP >= g_Ram.ramGroupC.TenOffValue)
		t->OnOffTEN=TEN_OFF;
	else if (g_Ram.ramGroupA.TemperBKP <= g_Ram.ramGroupC.TenOnValue)
		t->OnOffTEN=TEN_ON;
}
