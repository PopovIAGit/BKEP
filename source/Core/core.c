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
//#include "comm.h"
#include "stat.h"
#include "stat_fm25v10.h"

TFM25V10 Eeprom1;
TFM25V10 Eeprom2;

TCore	g_Core;

//выбор микросхем
__inline void Eeprom1CsSet(Byte Lev)  {SC_EEPROM1 = !Lev;}
__inline void Eeprom2CsSet(Byte Lev)  {SC_EEPROM2 = !Lev;}

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
	p->MotorControl.TorqueSetPr = (p->MotorControl.TorqueSet * 100)/p->TorqObs.TorqueMax;
}

// Остановка по калибровке
void Core_CalibStop (TCore *p)
{
	Bool StopFlag = False; // внутенний флаг остановки

	if(p->VlvDrvCtrl.Valve.Position == POS_UNDEF) //Если целевое положение не определено то уходим
	{
		p->MotorControl.TargetPos = POS_UNDEF;
		return;
	}
	p->MotorControl.TargetPos = g_Peref.Position.LinePos - p->VlvDrvCtrl.Valve.Position;

	if(p->Status.bit.Stop) return;

	if((p->MotorControl.RequestDir < 0) && (p->MotorControl.TargetPos <= 0)) StopFlag = True;
	if((p->MotorControl.RequestDir > 0) && (p->MotorControl.TargetPos >= 0)) StopFlag = True;

	if (StopFlag)	// Если пора останавливаться
	{
		if(p->VlvDrvCtrl.Valve.BreakFlag) p->MotorControl.OverWayFlag = 1;	//
		else	// Если
			{
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
	}

	if (IsMemParReady())																	// если есть готовность к записи параметров
	{
		if (g_Ram.ramGroupH.CycleCnt != p->PrevCycle)										// если счетчик циклов обновился
		{
			WriteToEeprom(REG_CYCLE_CNT, &g_Ram.ramGroupH.CycleCnt, 1);						// записали параметр счетчик циклов
			p->PrevCycle = g_Ram.ramGroupH.CycleCnt;										// запомнили записанный параметр, для последующей проверки
			g_Ram.ramGroupA.CycleCnt = g_Ram.ramGroupH.CycleCnt;
		}
		else if (g_Ram.ramGroupH.CalibState != g_Ram.ramGroupA.CalibState)					// если состояние калибровки изменилось
		{
			WriteToEeprom(REG_CALIB_STATE, &g_Ram.ramGroupH.CalibState, sizeof(ClbIndication));	// то записали состояние калибровки
			g_Ram.ramGroupA.CalibState = g_Ram.ramGroupH.CalibState;						// запоминаем записанный параметр, для последующей проверки
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

	g_Ram.ramGroupH.ContGroup 	 = cgStop;	// Подали команду на стоп контакторам
	g_Core.MotorControl.WorkMode = wmStop;  // Переходим в стейт машину стоп
	g_Core.VlvDrvCtrl.StartDelay = (Uns) START_DELAY_TIME; // Выставляем задержку перед следующим пуском
}

// Действия при пуске
void StartPowerControl(TValveCmd ControlWord)
{
	//Если КЗ то return
	if (g_Core.Protections.ShC_U ||g_Core.Protections.ShC_V  ||g_Core.Protections.ShC_W ) return;

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

	// сброс аварий необходимый для пуска
		Core_ProtectionsReset(&g_Core.Protections);

	g_Core.Status.bit.Stop 			= 0;
	g_Core.MotorControl.TorqueSet 	= 0xFFFF;
	if(g_Core.MotorControl.RequestDir < 0) g_Core.Status.bit.Closing = 1;
	if(g_Core.MotorControl.RequestDir > 0) g_Core.Status.bit.Opening = 1;

}

// Стэйт машина, Добавить логику работы с СофтСтартером
void Core_ControlMode(TCore *p)
{
	switch(p->MotorControl.WorkMode)
	{
	case wmStop:
		p->MotorControl.RequestDir = 0;		// сбрасываем необходимое направение движения
		g_Ram.ramGroupA.Torque = 0;			// отображаем момент
		break;
	case wmMove:
		g_Ram.ramGroupA.Torque = p->TorqObs.Indication; // отображаем текущий момент
		if(p->TorqObs.Indication < p->MotorControl.TorqueSet) p->MotorControl.MufTimer = 0;
		else if (++p->MotorControl.MufTimer >= MOVE_STATE_TIME) p->Protections.outFaults.Proc.bit.Mufta = 1;	// выставляем муфту если в течении секунды момент больше заданного
		break;
	case wmPlugBreak:

		break;
	}
}
