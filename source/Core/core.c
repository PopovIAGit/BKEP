/*======================================================================
Имя файла:          core.c
Автор:
Версия файла:
Дата изменения:
Описание: Работа ЯДРА
======================================================================*/

#include "core.h"
//#include "peref.h"
#include "g_Structs.h"
#include "g_Ram.h"
//#include "comm.h"
//#include "stat.h"

#define IsAlarmsExist (	p->protections.processAlarmsExist|	\
						p->protections.netAlarmsExist|		\
						p->protections.loadAlarmsExist|		\
						p->protections.deviceAlarmsExist )

#define 	NETALARM_MASK	0x1FF
#define 	LOADALARM_MASK	0x7

TCore	g_Core;

/*
//LgInt Tmp1_LgIntQ15 = 0;
//Uns  Tmp2_Q0 = 0;
Uns		Tmp3_Q0 = 0;

Uns			SifuControl = 90;
//static Bool fl_DirectionIsSetted = false;

Uns TestRun = 0;
Uns TestOut = 0;
Uns TestStart = 140;
Uns TestEnd = 90;
Uns TestTime = 20; 

volatile Uns setTorque;

Uns AngleInterp(Uns StartValue, Uns EndValue, Uns Time);
Uns TimerInterp = 0;
extern	Uns		PilaR;	//Для теста
extern	Uns		PilaS;	//Для теста
extern	Uns		PilaT;	//Для теста
*/
//---------------------------------------------------
void Core_Init(TCore *p)
{
	/*// Значения параметров присваиваются
	Core_MenuInit(&p->menu);
	Core_CommandsInit(&p->commands);
	Core_ProtectionsInit(&p->protections);
	Core_MotorControlInit(&g_Sifu);
	Core_HeatMotorMotorInit(&p->heatMotor);
	Core_TorqueInit(&p->torque);

	p->status.bit.stop = 1;

//	p->params.uporEnable = 0; // Для теста
	p->params.timeOutMaxTorque = Prd50HZ * 2; // 2 секунды
	p->params.speed2uporTimer = 0;
	p->params.brakingEnable = 0; //Дин торм выкл
	p->params.DynBrakeComplite = 1;
	p->params.timerBraking = 0;
	p->params.braking = 0;
	p->params.startTimer = 0;
	//g_Sifu.dynBrakeLevelNEW = _IQ15(g_Ram.ramGroupB.LEVEL_DYN_BRAKE_BIG);*/
}
//-------СТОП ПО КОМАНДЕ--------------------------------------------
void StopByCommand(TCore *p, Int brake)
{	
	/*if ((brake == brakeOn)&&			// Если торможение включено
		(!p->status.bit.testOpenClose)&&// и электропривод не в режиме тестового движения
		(!p->status.bit.upor)&&	  		// и привод не находится в состоянии упора
		(!p->params.DynBrakeComplite))	// бит завершения динамического торможения		
	{									// то тормозим
		p->status.bit.braking = 1;		//уходим в торможение
		AlarmElem_Off(currSkew);
		AlarmElem_Off(breakCurrU);
		AlarmElem_Off(breakCurrV);
		AlarmElem_Off(breakCurrW);
		AlarmElem_Off(minSpeed);
	}
	else
	{
		g_Sifu.Direction = dmStop;
		g_Sifu.SetAngle = 180;
		g_Sifu.OpenAngle = 180;
		p->params.DynBrakeComplite = 1;
		g_Sifu.zazorTimer = 0;
		g_Sifu.regSifuOut.bit.sifu_ENB = 0;
		shiftReg2.data.bit.dsp_enb_bf = 1;	//микросхема Васи (OFF)
		p->status.bit.closing = 0;
		p->status.bit.opening = 0;
		p->status.bit.braking = 0;
		p->status.bit.upor = 0;
		if (p->status.bit.testOpenClose) 	// Если находились в тестовом режиме
		{
			p->status.bit.testOpenClose = 0;// Снимаем тестовый режим
			EnableDisableAlarmsForTests(&p->protections,false);					// Включить защиты, которые не учитывались при тестовом движении
		}
		if (g_Ram.ramGroupC.SIFU_TEST == 2)	// Искусственная пила
		{
			EnableDisableAlarmsForTests(&p->protections,false);	// Включить защиты, которые не учитывались при тестовом движении
		}

		// Записываем в журнал
		g_Stat.cmdValue = CMD_STOP;
	
		p->status.bit.stop = 1;

		fl_DirectionIsSetted = false;
		//g_Sifu.dynBrakeLevelNEW = _IQ15(g_Ram.ramGroupB.LEVEL_DYN_BRAKE_BIG);

		// Выключение аварий, пассивных в СТОПе
		AlarmElem_Off(overCurrentU);
		AlarmElem_Off(overCurrentV);
		AlarmElem_Off(overCurrentW);
		AlarmElem_Off(currSkew);
		AlarmElem_Off(breakCurrU);
		AlarmElem_Off(breakCurrV);
		AlarmElem_Off(breakCurrW);
		AlarmElem_Off(minSpeed);
	}*/
}
//-------СТОП ПО АВАРИИ---------------------------------------------
void StopByFault(TCore *p)
{
	/*g_Sifu.Direction = dmStop;				// СИФУ - вырубить все
	g_Sifu.regSifuOut.bit.sifu_ENB = 0;
	shiftReg2.data.bit.dsp_enb_bf = 1;	//микросхема Васи (OFF)
	g_Sifu.SetAngle = 180;
	g_Sifu.OpenAngle = 180;
	g_Sifu.zazorTimer = 0;
	p->status.bit.braking = 0;	
	p->status.bit.closing = 0;
	p->status.bit.opening = 0;
	p->status.bit.upor = 0;
	p->status.bit.stop = 1;
	fl_DirectionIsSetted = false;
	//g_Sifu.dynBrakeLevelNEW = _IQ15(g_Ram.ramGroupB.LEVEL_DYN_BRAKE_BIG);

	p->params.DynBrakeComplite = 1;	//Запрещен дин торм

	if (p->status.bit.testOpenClose) 		// Если находились в тестовом режиме
	{
		p->status.bit.testOpenClose = 0;	// Снимаем тестовый режим
		EnableDisableAlarmsForTests(&p->protections,false);	// Включить защиты, которые не учитывались при тестовом движении
	}
	if (g_Ram.ramGroupC.SIFU_TEST == 2)		// Искусственная пила
	{
		EnableDisableAlarmsForTests(&p->protections,false);	// Включить защиты, которые не учитывались при тестовом движении
	}
	// Записываем в журнал
	g_Stat.cmdValue = CMD_STOP;

	// Выключение аварий, пассивных в СТОПе
	AlarmElem_Off(overCurrentU);
	AlarmElem_Off(overCurrentV);
	AlarmElem_Off(overCurrentW);
	AlarmElem_Off(currSkew);
	AlarmElem_Off(breakCurrU);
	AlarmElem_Off(breakCurrV);
	AlarmElem_Off(breakCurrW);
	AlarmElem_Off(minSpeed);*/
}
//------СТАРТ ПО КОМАНДЕ-----------------------------
void StartByCommand(TCore *p, Int direction, Uns command)
{
	/*

	 //--------Управление СИФУ---------
	Core_ProtectionsReset(&p->protections,false); 					// Сброс защит
	Core_ProtectionsStopTestsMotorTestIso(&p->protections);			// СТОП Тест Двигателя
	// Если искусственная пила или тестовое движение, то не смотрим на аварии
	if ((g_Ram.ramGroupC.SIFU_TEST != 2)&&(command != CMD_OPEN_TEST)&&(command != CMD_CLOSE_TEST))
	{
		if(IsAlarmsExist & (LedTsNotStartStop|LedTsNotStartNotStop))	// Если есть аварии, запрещающие СТАРТ
			return;
	} else							// Искусственная пила
	{
		PilaR = 0;		//Для теста
		PilaS = 60;		//Для теста
		PilaT = 120;	//Для теста
		EnableDisableAlarmsForTests(&p->protections,true);
	}	
	// Сначала закрываем СИФУ	
	g_Sifu.regSifuOut.all = DISABLE_SIFU;
	GPIO_UPDATE();

	// Записываем в журнал
	g_Stat.cmdValue = command;

	if (command == CMD_OPEN)							// команда "Открыть"
	{
		if (direction == DIRECT_RST)					// Включить группу "ВПЕРЕД" = "ОТКРЫТЬ" c alfa = upor
		{
			if (!(g_Ram.ramGroupB.REVERS & 0x1))		// Если реверс выключен ...
				g_Sifu.Direction = dmForward;			// R S T
			else										// Если реверс включен ...
				g_Sifu.Direction = dmBackward;

			if (g_Ram.ramGroupC.SIFU_TEST)
				g_Sifu.SetAngle = g_Ram.ramGroupC.SET_ANGLE;
			else
				g_Sifu.SetAngle = SifuControl;

			g_Sifu.OpenAngle = 120;
			g_Sifu.regSifuOut.bit.sifu_ENB = 1;
			shiftReg2.data.bit.dsp_enb_bf = 0;	//микросхема Васи (ON)
		}
		else if (direction == REVERSE_SRT)				// Включить группу "НАЗАД" = "ОТКРЫТЬ" c alfa = upor
		{
			if (!(g_Ram.ramGroupB.REVERS & 0x1))		// Если реверс выключен ...
				g_Sifu.Direction = dmBackward;			// RT S TR
			else										// Если реверс включен ...
				g_Sifu.Direction = dmForward;

			if (g_Ram.ramGroupC.SIFU_TEST)
				g_Sifu.SetAngle = g_Ram.ramGroupC.SET_ANGLE;
			else
				g_Sifu.SetAngle = SifuControl;

			g_Sifu.OpenAngle = 120;
			g_Sifu.regSifuOut.bit.sifu_ENB = 1;
			shiftReg2.data.bit.dsp_enb_bf = 0;	//микросхема Васи (ON)
		}
		p->status.bit.opening = 1;
		g_Ram.ramGroupF.COUNT_RUN++;
	}
	else if (command == CMD_CLOSE)						// Команда "Закрыть"
	{
		if (direction == DIRECT_RST) 					// Включить группу "ВПЕРЕД" = "ЗАКРЫТЬ" c alfa = upor
		{
			if (!(g_Ram.ramGroupB.REVERS & 0x1))		// Если реверс выключен ...
				g_Sifu.Direction = dmBackward;			// R S T
			else										// Если реверс включен ...
				g_Sifu.Direction = dmForward;

			if (g_Ram.ramGroupC.SIFU_TEST)
				g_Sifu.SetAngle = g_Ram.ramGroupC.SET_ANGLE;
			else
				g_Sifu.SetAngle = SifuControl;

			g_Sifu.OpenAngle = 120;
			g_Sifu.regSifuOut.bit.sifu_ENB = 1;
			shiftReg2.data.bit.dsp_enb_bf = 0;	//микросхема Васи (ON)
		}
		else if (direction == REVERSE_SRT)				// Включить группу "НАЗАД" = "ЗАКРЫТЬ" c alfa = upor
		{
			if (!(g_Ram.ramGroupB.REVERS & 0x1))		// Если реверс выключен ...
				g_Sifu.Direction = dmForward;			// RT S TR
			else										// Если реверс включен ...
				g_Sifu.Direction = dmBackward;

			if (g_Ram.ramGroupC.SIFU_TEST)
				g_Sifu.SetAngle = g_Ram.ramGroupC.SET_ANGLE;
			else
				g_Sifu.SetAngle = SifuControl;

			g_Sifu.OpenAngle = 120;
			g_Sifu.regSifuOut.bit.sifu_ENB = 1;
			shiftReg2.data.bit.dsp_enb_bf = 0;	//микросхема Васи (ON)
		}
		p->status.bit.closing = 1;
		g_Ram.ramGroupF.COUNT_RUN++;
	}
	else if (command == CMD_OPEN_TEST)// команда "Открыть тест"
	{
		direction = 1;
		g_Sifu.Direction = dmForward;					// R S T
		
		if (g_Ram.ramGroupC.SIFU_TEST)
			g_Sifu.SetAngle = g_Ram.ramGroupC.SET_ANGLE;
		else
			g_Sifu.SetAngle = SifuControl;
		g_Sifu.OpenAngle = 120;
		g_Sifu.regSifuOut.bit.sifu_ENB = 1;
		shiftReg2.data.bit.dsp_enb_bf = 0;	//микросхема Васи (ON)
		p->status.bit.opening = 1;
		p->status.bit.testOpenClose = 1;				// Включить режим тестового движения, когда енкодер не вращается
		EnableDisableAlarmsForTests(&p->protections,true); 	//для теста
		//Убираем ранее выставленные аварии
		p->protections.processAlarmsExist = 0;
		p->protections.netAlarmsExist = 0;
		p->protections.loadAlarmsExist = 0;
		p->protections.deviceAlarmsExist = 0;
	}
	else if (command == CMD_CLOSE_TEST)// команда "Закрыть тест"
	{
		direction = 1;
		g_Sifu.Direction = dmBackward;					// R S T

		if (g_Ram.ramGroupC.SIFU_TEST)
			g_Sifu.SetAngle = g_Ram.ramGroupC.SET_ANGLE;
		else
			g_Sifu.SetAngle = SifuControl;
		g_Sifu.OpenAngle = 120;
		g_Sifu.regSifuOut.bit.sifu_ENB = 1;
		shiftReg2.data.bit.dsp_enb_bf = 0;	//микросхема Васи (ON)
		p->status.bit.closing = 1;
		p->status.bit.testOpenClose = 1;				// Включить режим тестового движения, когда енкодер не вращается
		EnableDisableAlarmsForTests(&p->protections,true); 	//для теста
		//Убираем ранее выставленные аварии
		p->protections.processAlarmsExist = 0;
		p->protections.netAlarmsExist = 0;
		p->protections.loadAlarmsExist = 0;
		p->protections.deviceAlarmsExist = 0;
	}

	p->status.bit.stop = 0;
	p->status.bit.upor = 1;				// Со старта всегда уходим в УПОР
	p->params.DynBrakeComplite = 0;		// Для того чтобы привод зашел в дин торм	
	p->params.startTimer = 3*2000;		// При пуске отсчитываем таймер и не выводим аварии сети				

	if (IsAlarm_On(overCurrMode))
	{
		AlarmElem_On(overCurrentU);
		AlarmElem_On(overCurrentV);
		AlarmElem_On(overCurrentW);
	}
	if (IsAlarm_On(breakCurrMode))
	{
		AlarmElem_On(breakCurrU);
		AlarmElem_On(breakCurrV);
		AlarmElem_On(breakCurrW);
	}

	if (IsAlarm_On(currSkewMode)) AlarmElem_On(currSkew);
	// Включаем аварию Нет движения
	if ((IsAlarm_On(noMoveMode))&&(!p->protections.outAlarms.processAlarms.bit.noCalib)) AlarmElem_On(minSpeed);
	*/
}
//------------ОБРАБОТКА КОМАНД----------------------- // 50 Гц
void Core_CmdUpdate(TCore *p)	//вызываем нужную ф-ию, в зависимости от текущего статуса
{
	/*
	// Задержка на подачу команд
	if (p->commands.ignoreTimer) 						// Если сейчас время игнорирования команд
	{		
		p->commands.ignoreTimer--;						// уменьшаем таймер на единицу,
		return; 								//выходим
	}
	if (p->commands.cmdCommand)
	{
		switch (p->commands.cmdCommand)
		{	 
			case CMD_STOP:
				if (!IsStopped()) 			//Команда "СТОП" если находимся не в стопе
				{
					if (g_Ram.ramGroupB.BRAKING_ENABLE == 2)	// Если разрешено торможение по команде
						StopByCommand(p, brakeOn);
					else
						StopByCommand(p, brakeOff);
				}
				break;

			case CMD_CLOSE:
				if (!(p->status.bit.closing||p->status.bit.closed||p->status.bit.braking)) //Команда "ЗАКРЫТЬ" если находимся не в закрытом состоянии или не закрываемся
				{
					if (p->status.bit.opening) 									// Если блок в состоянии "открывается"
					{
						if (g_Ram.ramGroupB.BRAKING_ENABLE == 2)	// Если разрешено торможение по команде
							StopByCommand(p, brakeOn);
						else
							StopByCommand(p, brakeOff);
					}
					else														// В остальных случаях 
						StartByCommand(p, g_Peref.phaseOrder.Direction, CMD_CLOSE); // даем "старт"
				}
				break;

			case CMD_OPEN:
				if (!(p->status.bit.opening||p->status.bit.opened||p->status.bit.braking)) //Команда "ОТКРЫТЬ" если находимся не в открытом состоянии или не открываемся
				{
					if (p->status.bit.closing) 									// Если блок в состоянии "закрывается"
					{
						if (g_Ram.ramGroupB.BRAKING_ENABLE == 2)	// Если разрешено торможение по команде
							StopByCommand(p, brakeOn);
						else
							StopByCommand(p, brakeOff);
					}
					else														// В остальных случаях 
						StartByCommand(p, g_Peref.phaseOrder.Direction, CMD_OPEN);  // даем "старт"
				}
				break;

			case CMD_CLOSE_TEST:			//поехали закрывать в тестовом режиме
				if (IsStopped()) 			//тест только из стопа
				{
					if (!p->status.bit.closed)
						StartByCommand(p, g_Peref.phaseOrder.Direction, CMD_CLOSE_TEST); // даем "старт"
				}
				break;

			case CMD_OPEN_TEST:			//поехали открывать в тестовом режиме
				if (IsStopped()) 			//тест только из стопа
				{
					if (!p->status.bit.opened)
						StartByCommand(p, g_Peref.phaseOrder.Direction, CMD_OPEN_TEST); // даем "старт"
				}
				break;
		}
		// Задержка на подачу команд
		p->commands.ignoreTimer = IGNORE_TIME;
		p->commands.cmdCommand = 0; 				//очищаем регистр комманд
	}*/
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
// Обработка нагрева двигателя
Bool HeatMotorControl(TCore *p)
{
	// Если в СТОПе и неактивны тесты двигателя
	if (IsStopped())
	{
		if (g_Ram.ramGroupC.HEAT_MOTOR_REG)
		{	// Если нет темп. аварий
			if (!IsTempAlarmsExist())
			{
				switch (g_Ram.ramGroupC.HEAT_MOTOR_REG)
				{
					case HM_CMD_START:
						if (!p->heatMotor.status.bit.active)
						{
							// Останавливаем Тесты Двигателя
							Core_ProtectionsStopTestsMotorTestIso(&p->protections);
							p->heatMotor.status.all = HM_ACTIVE;
							p->heatMotor.voltageLevel = 170 - (Uns)_IQmpy(_IQdiv(g_Ram.ramGroupC.HEAT_MOTOR_VOLT_LEVEL,100),170); // 170 - 0.1 (10%) * 170 = 153 градуса // / 100 в IQ24
							// Ограничение на угол открытия
							if (p->heatMotor.voltageLevel < 20) p->heatMotor.voltageLevel = 20;			//Не выходим за пределы
							else if (p->heatMotor.voltageLevel > 170) p->heatMotor.voltageLevel = 170;
							p->heatMotor.phaseNumber = 0;
							p->timerHeatMotor = 0;
						}
						break;
					case HM_CMD_STOP:
						return false;
				}
			}
			else p->commands.cancelFlag = true;

			g_Ram.ramGroupC.HEAT_MOTOR_REG = 0;
			// Обновление нижней строки дисплея
			p->menu.updateLowStr = true;
		}
		if (p->heatMotor.status.bit.active)
		{
			// 1 с за 50 Гц = 1.0 с * 5
			if ( (++p->timerHeatMotor > (g_Ram.ramGroupC.HEAT_MOTOR_TIME * 5))
				|| (g_Ram.ramGroupA.TEMP_MOTOR >= (Int)g_Ram.ramGroupC.HEAT_MOTOR_LEVEL) )
				return false;
		}
		return true;
	} 
	else return false;
	*/
}
//--------------------------------------------------------
// СТОП Нагрев Двигателя
/*__inline void Core_StopHeatMotor(TCore *p)
{
	p->heatMotor.voltage = 0;
	p->heatMotor.status.all = 0;
	p->timerHeatMotor = 0;
}*/
//--------------------------------------------------------


/*Uns AngleInterp(Uns StartValue, Uns EndValue, Uns Time)
{

static LgInt OutputQ15 = 0;

	//timeQ15 = (LgInt)Time * 3277;
	//nQ15 = _IQ15mpy((LgInt)Time * 3277, _IQ15(50.0));
	//FullDelta = StartValue - EndValue;
	//DeltaQ15 = _IQ15div(_IQ0toIQ15((LgInt)(StartValue - EndValue)), _IQ15mpy((LgInt)Time * 3277, _IQ15(50.0)));

	if (TimerInterp == 0) OutputQ15 = (LgInt)StartValue << 15;
	else OutputQ15 = OutputQ15 - _IQ15div(((LgInt)(StartValue - EndValue) << 15), _IQ15mpy((LgInt)Time * 3277, _IQ15(50.0)));

	TimerInterp++;
	
	if (OutputQ15 <= ((LgInt)EndValue << 15)) return EndValue;
	else if (OutputQ15 >= ((LgInt)StartValue << 15)) return StartValue;
		 else return (Uns)(OutputQ15 >> 15);

}*/



