/*======================================================================
Имя файла:          core.h
Автор:
Версия файла:
Дата изменения:
Описание:
Описание структур и функций ядра

Inputs 
Outputs
========================================================================*/

#ifndef CORE_
#define CORE_

//----------- Подключение заголовочных файлов ------------------------------

#include "core_Protections.h"
#include "core_Menu.h"
#include "core_VlvDrvCtrl.h"
#include "core_TorqueObserver.h"
#include "core_Drive.h"
#include "comm.h"
#include "g_Ram.h"
#include "g_InitHardware.h"
#include "csl\csl_memory.h"
#include "stat_fm25v10.h"
//#include "core_MotorControl.h"

//--------------------- Константы-------------------------------------------
#define CMD_DEFAULTS_USER	0x0010	// Пользовательские параметры по умолчанию
#define CMD_RES_CLB			0x0020	// Сброс калибровки датчика положения
#define CMD_RES_FAULT		0x0040	// Сброс защит
#define CMD_CLR_LOG			0x0080	// Очистка журнала
#define CMD_RES_CYCLE		0x0100	// Сброс счетчика циклов
#define CMD_PAR_CHANGE		0x0200	// Изменение параметров
#define CMD_DEFAULTS_FACT 	0x0400	// Заводские параметры по умолчанию

#define START_DELAY_TIME		(Uint16)(2.000 * Prd50HZ)		// Ограничение времени паузы между остановом и след. запуском
#define MOVE_STATE_TIME			(Uint16)(1.000 * Prd50HZ)		// Ограничение времени перехода в муфту
#define BREAK_SCALE				(Uint16)(0.010 * Prd50HZ)		//
//--------------------- Макросы --------------------------------------------
//-------------------- Структуры -------------------------------------------

//================== ЯДРО ============================

// Режимы работы при цифровом управлении приводом
typedef enum {
	wmStop       = 1,					// Режим стоп
	wmMove     	 = 2,					// Режим движения
	wmPlugBreak	 = 3					// Режим торможения
} TWorkMode;

// Структура для цифрового управления приводом
typedef struct _TDmControl {
	TWorkMode	WorkMode;				// Режим работы
	Int		 	RequestDir;				// Заданное направление вращения
	LgInt	 	TargetPos;				// Целевое положение
	Uns		 	TorqueSet;				// Задание момента
	Uns		 	TorqueSetPr;			// Задание момента в %
	Uns 		BreakFlag;				// Флаг определяющий возможность уплотнения
	Uns 		OverWayFlag;			// Флаг показывающий что уплотнение не достигнуто
	Uns 		MufTimer;				// Таймер срабатывания муфты в движении
	Uns 		CalibStop;				// Остановка по калибровке
	Uns 		PlugBreakTimer;			// Таймер используемый для торможения противовключением (пауза перед и само противовключение)
	Uns 		PlugBreakStep;			// Шаги противовключения (Пауза, торможение, выключение)
} TDmControl;

typedef struct {
	// ---
	TStatusReg 			Status;			// Статус работы
	TStatusReg 			TestStatus;		// переменная для проверки
	// ---
	TCoreProtections	Protections;	// Защиты
	// ---
	//TCoreParams		params;			// Параметры
	// ---
	TCoreMenu			menu;			// Меню
	// ---
	TTorqObs			TorqObs;		// Расчет момента
	// ---
	TCoreVlvDrvCtrl		VlvDrvCtrl;		// Управление задвижкой
	// ---
	TDmControl			MotorControl;	// Управление двигателем

	Uns 				PrevCycle;		// Предидущее значение счетчика циклов
	//---- 3 секунды------
	Uns 				Sec3Timer;		// Таймер для работы с 3 секундами
	Uns 				SaveDirection;	// Сохраненное направление вращения
	Uns 				PowerLostFlag;  // Флаг показывающий пропажу напряжения
} TCore;

//------------------- Глобальные переменные --------------------------------
//------------------- Протатипы функций ------------------------------------

void Core_Init(TCore *);
void Core_CmdUpdate(TCore *);
void Core_StatusUpdate(TCore *);			//

void StopPowerControl(void);					// действия при стопе
void StartPowerControl(TValveCmd ControlWord);  // Действия при старте

void Core_CalibControl(TCore *);			// Управление калибровкой
void Core_CalibStop(TCore *);				// стоп по калибровке
void Core_DefineCtrlParams(TCore *);		// Задача контролируемых параметров (Моменты - Движения, трогания, уплотнения. Определение текущей зоны и т.д)
void Core_ControlMode(TCore *);				// Стэйт машина
void Core_LowPowerControl(TCore *);			// Действия при отключении питания
void Core_MuDuControl(TCore *);				// управление режимами мустное дистанция

void Core_ProtectionsBreakRST(TCoreProtections *);

extern TCore g_Core;
extern volatile Uns setTorque;
#endif // CORE_

