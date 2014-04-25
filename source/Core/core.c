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
	Core_ValveDriveInit(&p->VlvDrvCtrl);	// Управление задвижкой
	Core_TorqueInit(&p->TorqObs);			// Расчет моментов
	//Core_CommandsInit(&p->commands);		// Получение команд, настройка калибровки
	//Core_ProtectionsInit(&p->protections);// Защиты

	p->Status.bit.Stop = 1;					// При включение выставляем стоп
}

// Функция задания параметров момента в зависимости от положения и направления движения
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
			g_Ram.ramGroupD.TaskClose = trReset;

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

	/*Добавить Запись параметров калибровки и числа циклов

	if (IsMemParReady())													// если есть готовность к записи параметров
	{
		if (GrH->CycleCnt != PrevCycle)										// если счетчик циклов обновился
		{
			WritePar(REG_CYCLE_CNT, &GrH->CycleCnt, 1);						// записали параметр счетчик циклов
			PrevCycle = GrH->CycleCnt;										// запомнили записанный параметр, для последующей проверки
		}
		else if (GrH->CalibState != GrA->CalibState)						// если состояние калибровки изменилось
		{
			WritePar(REG_CALIB_STATE, &GrH->CalibState, sizeof(ClbIndication));// то записали состояние калибровки
			GrA->CalibState = GrH->CalibState;								// запоминаем записанный параметр, для последующей проверки
		}
	}
	  */
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
	//	ProtectionsReset();

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
		break;
	}
}

//---------------ОБРАБОТКА СТАТУСОВ---------------------------
void Core_StatusUpdate(TCore *p) // 50 Гц
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
		g_Ram.ramGroupA.TORQUE = 0;	// в СТОПЕ момент обнуляем
		SifuControl  = 180;
		/////
		if (TestRun) TestOut = AngleInterp(TestStart, TestEnd, TestTime); 
	}
	else
	{  // НЕ в СТОПЕ момент рассчитываем в соответствии	с корректировками из группы C1
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
		
			// в УПОРЕ момент выводим в соответствии с заданием
		if (p->status.bit.upor)	
			g_Ram.ramGroupA.TORQUE = setTorque;
		if (p->status.bit.testOpenClose) 
			g_Ram.ramGroupA.TORQUE = 0;
	}
	p->status.bit.muDu 	= !IsLockMPU();
//--------ВЫСТАВЛЕНИЕ СТАТУСА ТЕН-------------------------------
	//p->status.bit.ten = !g_Comm.localControl.inputData.bit.ten;
	if (g_Ram.ramGroupB.TEN_ENABLE)	// Если ТЕН разрешен
	{
		if (p->status.bit.ten) // Если ТЕН включен
		{
			if (g_Ram.ramGroupA.TEMPER_PU >= g_Ram.ramGroupC.TEN_OFF_LEVEL)
				p->status.bit.ten = 0;
		}
		else					// Если ТЕН выключен
		{
			if (g_Ram.ramGroupA.TEMPER_PU <= g_Ram.ramGroupC.TEN_ON_LEVEL)
				p->status.bit.ten = 1;
		}
	}
	else							// Иначе (если ТЕН запрещен)
		p->status.bit.ten = 0;
//--------ВЫСТАВЛЕНИЕ СТАТУСА ДЕЖУРНЫЙ РЕЖИМ---------
	p->status.bit.sleepMode = p->menu.SleepActive;
//--------ВЫСТАВЛЕНИЕ СТАТУСА АВАРИЯ--------------------------
	p->status.bit.fault = (IsAlarmsExist > 0) ? 1 : 0;

	//--------ВЫСТАВЛЕНИЕ СТАТУСОВ ЗАКРЫТО И ОТКРЫТО---------
	p->status.bit.closed = g_Peref.position.encoderStatus.bit.closed;
	p->status.bit.opened = g_Peref.position.encoderStatus.bit.opened;

// - - - - - - - - - - - СТОП - - - - - - - - - - - - - - - - 
	if (p->status.bit.stop)								//если мы в СТОПЕ
	{
		p->params.timerNoMove = 0;
		p->params.uporTimer = 0;
		p->params.timerMaxTorque = 0;
		p->params.speed2uporTimer = 0;
	}
	//  Если есть аварии, останавливающие движение, делаем СТОП,
	else if (IsAlarmsExist & (LedTsStartStop|LedTsNotStartStop))
	{
		StopByFault(p);
	}
// - - - - - - - - - - - УПОР - - - - - - - - - - - - - - - - 
	else if (p->status.bit.upor)							//если мы в движении и в УПОРЕ
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
			if (g_Ram.ramGroupA.SPEED_P > g_Ram.ramGroupB.SELECT_SPEED)		// или тест. движение
			{	   															// то уходим в движение
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
// - - - - - - - - - ТОРМОЖЕНИЕ - - - - - - - - - - - - - - - 
	else if (p->status.bit.braking)			//если мы оказались в ТОРМОЖЕНИИ
	{
	p->params.timerBraking++; // таймер работы режима замедления
	
	switch (g_Ram.ramGroupC.BRAKING_MODE)
	{
	case 11: // только противовключение по времени
		{
		if (p->params.timerBraking <= g_Ram.ramGroupC.REVERS_TIME * 5) // противовключение
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
		else // выход
			{
			p->params.timerBraking = 0;
			p->params.DynBrakeComplite = 1;
			StopByCommand(p, brakeOff);
			}
		} break;

	case 12: // только противовключение по скорости
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
		else // выход
			{
			p->params.timerBraking = 0;
			p->params.DynBrakeComplite = 1;
			StopByCommand(p, brakeOff);
			}
		} break;

	case 21: // только динамическое тормож. по времени
		{
		if (p->params.timerBraking <= g_Ram.ramGroupC.DYNBRAKE_TIME * 5) // противовключение
			{
			g_Sifu.Direction = dmDynBraking;
			if (g_Ram.ramGroupC.DYNBRAKE_START_LEV <= g_Ram.ramGroupC.DYNBRAKE_END_LEV)
				g_Sifu.dynBrakeLevel = g_Ram.ramGroupC.DYNBRAKE_END_LEV;
			else
				{
				g_Sifu.dynBrakeLevel = AngleInterp(g_Ram.ramGroupC.DYNBRAKE_START_LEV, g_Ram.ramGroupC.DYNBRAKE_END_LEV, g_Ram.ramGroupC.DYNBRAKE_TIME); 
				}
			}
		else // выход
			{
			p->params.timerBraking = 0;
			p->params.DynBrakeComplite = 1;
			StopByCommand(p, brakeOff);
			}
		} break;

	case 22: // только динамическое тормож. по скорости
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
		else // выход
			{
			p->params.timerBraking = 0;
			p->params.DynBrakeComplite = 1;
			StopByCommand(p, brakeOff);
			}

		} break;

	case 31: // противовключение + динамическое тормож. по времени
		{
		if (p->params.timerBraking <= g_Ram.ramGroupC.REVERS_TIME * 5) // противовключение
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
			else if (p->params.timerBraking <= (g_Ram.ramGroupC.REVERS_TIME + g_Ram.ramGroupC.DYNBRAKE_TIME)*5) // динамическое
				 	{
					g_Sifu.Direction = dmDynBraking;
					if (g_Ram.ramGroupC.DYNBRAKE_START_LEV <= g_Ram.ramGroupC.DYNBRAKE_END_LEV)
						g_Sifu.dynBrakeLevel = g_Ram.ramGroupC.DYNBRAKE_END_LEV;
					else
						{
						g_Sifu.dynBrakeLevel = AngleInterp(g_Ram.ramGroupC.DYNBRAKE_START_LEV, g_Ram.ramGroupC.DYNBRAKE_END_LEV, g_Ram.ramGroupC.DYNBRAKE_TIME); 
						}
					}
				 else // выход
					{
					p->params.timerBraking = 0;
					p->params.DynBrakeComplite = 1;
					StopByCommand(p, brakeOff);
					}
		} break;

	case 32: // противовключение + динамическое тормож. по уровню скорости
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
				 else // выход
					{
					p->params.timerBraking = 0;
					p->params.DynBrakeComplite = 1;
					StopByCommand(p, brakeOff);
					}
		} break;

	default:
		{	// некорректные параметры => сброс режима торможения
		p->params.timerBraking = 0;
		p->params.DynBrakeComplite = 1;
		StopByCommand(p, brakeOff);
		} break; 
	}


	}
// - - - - - - - - - - ДВИЖЕНИЕ - - - - - - - - - - - - - - - 
	else if (p->status.bit.closing|p->status.bit.opening)	//если мы в ДВИЖЕНИИ
	{
		if (g_Ram.ramGroupC.SIFU_TEST) 
			g_Sifu.SetAngle = g_Ram.ramGroupC.SET_ANGLE;
		else 
		{
			if (p->params.speed_ctrl) g_Sifu.SetAngle = 111;
			else g_Sifu.SetAngle = 5;
		}

		if (p->status.bit.testOpenClose)	// если двигаемся в тестовом режиме
			return;							// Выходим. Не смотрим ни на положение, ни на момент, ни на скорость, ни на чередование фаз
		if (g_Ram.ramGroupA.TORQUE >= setTorque) //проверка момента
		{
			if (++p->params.timerMaxTorque >= g_Ram.ramGroupB.UPOR_TIMEOUT*5) 
			{
				if (g_Ram.ramGroupB.UPOR_MODE)
				{
					p->status.bit.upor = 1;					//во время движения - в упор
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
//-------------- Сброс дисплея при отриц. температурах не в спящем режиме--------
	if ( (g_Ram.ramGroupA.TEMPER_PU < 0)&&(!p->status.bit.sleepMode) )
	{
		if (++t_TimerDisplayReset > 500)	// 10 c
		{
			DisplayReset(&g_Peref.display, g_Ram.ramGroupB.INDICATOR_TYPE);
			t_TimerDisplayReset = 0;
		}
	}
//-------------- Нагрев Двигателя -----------------------------
	if (!HeatMotorControl(p)) Core_StopHeatMotor(p);

//--------ВЫСТАВЛЕНИЕ СТАТУСА НАГРЕВ ДВИГАТЕЛЯ---------
	p->status.bit.heatMotor = p->heatMotor.status.bit.active;
}
//--------------------------------------------------------

}*/


}
