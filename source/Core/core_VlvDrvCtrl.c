/*======================================================================
Имя файла:          core_VlvDrvCtrl.c
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		16.04.2014
Описание:
модуль управления задвижкой
======================================================================*/

#include "core.h"

__inline void GetActiveControls(TCoreVlvDrvCtrl *);	// Получаем данные от куда обрабатываем команды
__inline void MpuControl(TCoreVlvDrvCtrl *);		// Обрабатываем местны команы
__inline void TeleControl(TCoreVlvDrvCtrl *);		// обрабатываем команды ТУ/Модбас
__inline void UnitControl(TCoreVlvDrvCtrl *);		// Выбераем действие в зависимости от полученной корманлды
__inline void DriveStop(TCoreVlvDrvCtrl *);			// Действия при команде стоп
__inline void DriveClose(TCoreVlvDrvCtrl *);		// ... при закрыть
__inline void DriveOpen(TCoreVlvDrvCtrl *);			// ... при открыть
__inline void UpdateComm(TCoreVlvDrvCtrl *);		// Обработка внутренней команды при обработке реверса
static   void ReverseDrive(TCoreVlvDrvCtrl *);		// Действия при реверсеы

Bool Flag = False;

void Core_ValveDriveInit(TCoreVlvDrvCtrl *p)
{
	p->Status 				= ToPtr(&g_Core.Status);
	p->ControlWord 			= &g_Ram.ramGroupD.ControlWord;
	p->MuDuSetup			= &g_Ram.ramGroupB.MuDuSetup;
	p->DuSource				= &g_Ram.ramGroupB.DuSource;
	p->ReverseType			= &g_Ram.ramGroupH.ReverseType;
	p->Mpu.Enable			= true;
	p->Mpu.BtnKey			= &g_Ram.ramGroupH.CmdButton;
	p->Mpu.ScuKey			= &g_Ram.ramGroupH.CmdKey;
	p->Mpu.CancelFlag		= false;
	p->Tu.Enable			= true;
	p->Tu.LocalFlag			= false;
	p->Tu.State				= &g_Ram.ramGroupH.TuState;
	p->Tu.Ready				= true;
	p->Tu.LockSeal			= &g_Ram.ramGroupB.TuLockSeal;
	p->Valve.PosRegEnable 	= true;
	p->Valve.BreakFlag		= false;
	p->Valve.Position		= POS_UNDEF;
	p->Valve.BreakDelta		= 0;
	p->Valve.BreakMode 		= &g_Ram.ramGroupB.BreakMode;
	p->Valve.CalibStates	= &g_Ram.ramGroupA.CalibState;
	p->Valve.CalibFullStep  = &g_Ram.ramGroupH.FullStep;
	p->EvLog.Source			= 0;
	p->EvLog.Value			= 0;
	p->Command				= vcwNone;
	p->MuDuInput			= 0;
	p->ActiveControls		= 0;
	p->StartDelay			= 0;
	p->IgnorComFlag			= 0;
	p->StartControl			= &StartPowerControl;
	p->StopControl			= &StopPowerControl;

}

void Core_ValveDriveStop(TCoreVlvDrvCtrl *p)
{
	p->Valve.BreakFlag = False;

	if (p->Status->bit.Fault || !(*p->Tu.LockSeal)) p->Tu.Ready = False;

	p->Valve.Position = POS_UNDEF;

	p->IgnorComFlag = 0;

	p->StopControl();
}

void Core_ValveDriveUpdate(TCoreVlvDrvCtrl *p)
{
	GetActiveControls(p);	// Получение данных по активированному типу управления (местное/дистанция)
	TeleControl(p);			// Подача команд дистанционного управления
	MpuControl(p);			// Подача команд местного управления
	UnitControl(p);			// Дейстаия в зависимости от команды (открыть/закрыть/стоп)
}

//
__inline void GetActiveControls(TCoreVlvDrvCtrl *p)
{

		Uns  DigState = 0;

		p->ActiveControls = 0;

		switch(*p->MuDuSetup)
		{
			case mdOff:    p->Status->bit.MuDu = 0; Flag = True; break;
			case mdSelect: p->Status->bit.MuDu = p->MuDuInput; Flag = false; break;
			case mdMuOnly: p->Status->bit.MuDu = 1; Flag = false;  break;
			case mdDuOnly: p->Status->bit.MuDu = 0; Flag = false;  break;
			default:       return;
		}

		if (Flag || p->Status->bit.MuDu)
		{
			p->ActiveControls |= (CMD_SRC_PDU|CMD_SRC_MPU);
			if (p->Tu.LocalFlag) p->ActiveControls |= CMD_SRC_DIGITAL;
		}

		if (Flag || !p->Status->bit.MuDu)
		{
			DigState = p->Tu.LocalFlag ? 0 : CMD_SRC_DIGITAL;
			switch(*p->DuSource)
			{
				case mdsAll:     p->ActiveControls |= (DigState|CMD_SRC_SERIAL); break;
				case mdsDigital: p->ActiveControls |= DigState; break;
				case mdsSerial:  p->ActiveControls |= CMD_SRC_SERIAL; break;
			}
		}
}

// обработка команд с МПУ
__inline void MpuControl(TCoreVlvDrvCtrl *p)
{
	Uns Key = 0;
			Uns  Active = 0;

		p->Mpu.Enable = p->Status->bit.MuDu | Flag;

		if(!p->Mpu.Enable) return;				// выключено выходим

		if (*p->Mpu.BtnKey)						// Пришла команда с ручек
		{
			Key = *p->Mpu.BtnKey;				// Запомнили пришедшую команду
			Active = (p->ActiveControls & CMD_SRC_MPU); // Запомнили активирован ли режим мпу
			p->EvLog.Source = CMD_SRC_MPU;				// Источник команды для журнала
			*p->Mpu.BtnKey = 0;					// Сбросили команду
		}

		if (*p->Mpu.ScuKey)						//Аналогично
		{
			Key = *p->Mpu.ScuKey;
			Active = (p->ActiveControls & CMD_SRC_PDU);
			p->EvLog.Source = CMD_SRC_PDU;
			*p->Mpu.ScuKey = 0;
		}

		switch (Key)							// Обработка пришедшей команды
		{
			case KEY_STOP:						// если стоп
				*p->ControlWord = vcwStop;		// Подаем команду на остановку, вне зависимости от активации режима мпу
				break;
			case KEY_CLOSE:
				if (Active) *p->ControlWord = vcwClose;	// Если редим мпу активирован то выполяем команду
				else p->Mpu.CancelFlag = True;			// Если режим мпу не активен то подаем флаг отмены команды
				break;
			case KEY_OPEN:
				if (Active) *p->ControlWord = vcwOpen;
				else p->Mpu.CancelFlag = True;
				break;
		}
}

__inline void TeleControl(TCoreVlvDrvCtrl *p)
{
	TValveCmd TuControl = vcwNone;
	Bool Ready;

	p->Tu.Enable = !p->Status->bit.MuDu;

	if (!p->Tu.Enable) return;

	if (!(p->ActiveControls & CMD_SRC_DIGITAL))
	{
		*p->Tu.State &= ~(TU_STOP|TU_CLOSE|TU_OPEN);
		p->Tu.Ready = True;
		return;
	}

	Ready = p->Status->bit.Stop && p->Tu.Ready;

	switch(*p->Tu.State & (TU_CLOSE|TU_OPEN))
	{
		case TU_CLOSE: if (Ready) TuControl = vcwClose; break;
		case TU_OPEN:	if (Ready) TuControl = vcwOpen;  break;
		case (TU_CLOSE|TU_OPEN):  TuControl = vcwStop;  break;
		default:
			p->Tu.Ready = True;
	}

	if (*p->Tu.State & TU_STOP) TuControl = vcwStop;

	if (TuControl != vcwNone)
	{
		*p->ControlWord = TuControl;
		p->EvLog.Source = CMD_SRC_DIGITAL;
	}
	*p->Tu.State &= ~(TU_STOP|TU_CLOSE|TU_OPEN);
}

__inline void UnitControl(TCoreVlvDrvCtrl *p)
{
		UpdateComm(p);

		if (p->StartDelay > 0) p->StartDelay--;
		else if (*p->ControlWord)
		{
			switch(*p->ControlWord)
			{
				case vcwStop:      DriveStop(p);  	break;
				case vcwClose:
				case vcwTestClose: DriveClose(p); 	break;
				case vcwOpen:
				case vcwTestOpen:  DriveOpen(p);  	break;
				//case vcwDemo:        				break;
				//case vcwTestEng:
				default: break;//p->StartControl(*p->ControlWord);
			}
			*p->ControlWord = vcwNone;
		}
}

__inline void DriveStop(TCoreVlvDrvCtrl *p)
{
	Core_ValveDriveStop(p);
	p->EvLog.Value = CMD_STOP;
}

__inline void DriveClose(TCoreVlvDrvCtrl *p)
{
	Bool FreeRun;

	if (p->Status->bit.Closed && (*p->ControlWord != vcwTestClose)) // SVS
	{
		p->IgnorComFlag = 1;										// Если уже закрыто то выставляем флаг игнорирования команды
	 	return;
	}
	if(p->Status->bit.Closing) return;								// Если идет закрытие то уходим
	if (p->Status->bit.Opening) {ReverseDrive(p); return;}

	p->Valve.BreakFlag = False;										//
	FreeRun = (*p->ControlWord == vcwTestClose) || !(*p->Valve.CalibStates == VLV_CLB_FLAG);		// Режим свободного движения - Если тестовое открытие или не откалиброванно
	if (FreeRun) p->Valve.Position = POS_UNDEF;						//
	else if (!((*p->Valve.BreakMode == vtClose) || (*p->Valve.BreakMode == vtBoth)))
	{
		p->Valve.Position = 0;				// Если не работаем в режиме уплотнения - целевое положение 0
	}
	else															// Если работаем в режиме уплотнения
	{
		p->Valve.BreakFlag = True;									// Выставили флаг работы в уплотнении
		p->Valve.Position  = -(LgInt)p->Valve.BreakDelta;			// Сместили целевое положение на допустимый ход выходного звена при уплотнении
	}
	p->EvLog.Value = CMD_CLOSE;	// Значение команды для журнала

	p->StartControl(*p->ControlWord); // Подали команду управления для начала работы
}

__inline void DriveOpen(TCoreVlvDrvCtrl *p)
{
	Bool FreeRun;

	if (p->Status->bit.Opened && (*p->ControlWord != vcwTestOpen)) // SVS
	{
		p->IgnorComFlag = 1;
	 	return;
	}
	if (p->Status->bit.Opening) return;
	if (p->Status->bit.Closing) {ReverseDrive(p); return;}

	p->Valve.BreakFlag = False;
	FreeRun = (*p->ControlWord == vcwTestOpen) || !(*p->Valve.CalibStates == VLV_CLB_FLAG);
	if (FreeRun) p->Valve.Position = POS_UNDEF;
	else if (!((*p->Valve.BreakMode == vtOpen) || (*p->Valve.BreakMode == vtBoth)))
	{
		p->Valve.Position = (LgInt)(*p->Valve.CalibFullStep);
	}
	else
	{
		p->Valve.BreakFlag = True;
		p->Valve.Position  = (LgInt)(*p->Valve.CalibFullStep) + (LgInt)p->Valve.BreakDelta;
	}
	p->EvLog.Value = CMD_OPEN;

	p->StartControl(*p->ControlWord);
}

__inline void UpdateComm(TCoreVlvDrvCtrl *p)		// Функция автоматической подачи команды при реверсе ()
{
	if (p->Command != vcwNone)					// Если пришла внутренняя
	{
		if (p->Command == vcwStop) *p->ControlWord = vcwStop;	// Если команда для реверса Стоп, подаем нормальную команду стоп
		else if (!p->Status->bit.Stop) return;					// Ждем пока остановимся
		else *p->ControlWord = p->Command;						// Подаем команду в противоположном направленииы
		p->Command = vcwNone;									// Сбрасываем внутреннюю команду
	}
}

void ReverseDrive(TCoreVlvDrvCtrl *p)				// Функция Реверса
{
	switch (*p->ReverseType)					// В зависимости от типа реверса
	{
		case rvtStop:  p->Command = vcwStop; break;	// Стоп при реверсе
		case rvtAuto: {
						p->Command = *p->ControlWord; Core_ValveDriveStop(p);	// Через стоп поехали в другую сторону
						break;
					  }
		case rvtNone: *p->ControlWord = vcwNone; break; // Игнорирование команды управления
	}
}




