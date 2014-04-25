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

//#include "core_Protections.h"
#include "core_Menu.h"
#include "g_Ram.h"
#include "g_InitHardware.h"
#include "csl\csl_memory.h"
#include "stat_fm25v10.h"
//#include "core_Commands.h"
//#include "core_MotorControl.h"
//#include "core_HeatMotor.h"

// Параметры для останова
typedef enum {
	brakeOff 	= 0,	// Не тормозить
	brakeOn		= 1		// Тормозить
} TBrake;
//--------------------------------------------------------
//параметры 
typedef struct {
	Uns		timerMaxTorque;		// таймер для Максимальной нагрузки
	Uns 	timeOutMaxTorque;	// предел таймера для максимальной нагрузки
	Uns 	timerNoMove;		// таймер для отсутствия движения (в УПОРе и в Движении)
	Uns 	timeOutNoMove;		// предел таймера для отсутствия движения (в Движении)
	Uns		timerBraking;		// таймер при торможении
	Uns		timeOutBraking;		// предел таймера для торможения
	Uns		brakingLevel;		// уровень торможения
	Uns		brakingEnable;		// торможение вкл/откл
	Uns		uporTimer;			// ?????? ??? ?????
	Uns		uporEnable;			// Упор вкл/откл
	Uns		speed2uporTimer;	//
	Uns   	DynBrakeComplite;	//Бит завершения динамического торможения
	Uns		braking;			//Притормаживание. [0, 1]
	Uns		speed_ctrl;			//Режим управления скоростью
	Int		startTimer;			//При пуске не выводим аварии сети
} TCoreParams;
//--------------------------------------------------------
//================== ЯДРО ============================
//--------------------------------------------------------

typedef struct {
	// ---
	//TCoreStatus 		status;			// Статус работы
	// ---
	//TCoreProtections	protections;	// Защиты
	// ---
	//TCoreParams			params;			// Параметры
	// ---
	TCoreMenu			menu;			// Меню
	// ---
	/*TTorque				torque;			// Расчет момента
	// ---
	TCoreCommands		commands;		// Команды
	// ---
	THeatMotor			heatMotor;		// Нагрев двигателя*/
	Uns					timerHeatMotor;	// Таймер нагрева двигателя
} TCore;

//--------------------------------------------------------

void Core_Init(TCore *);
void Core_CmdUpdate(TCore *);
void Core_StatusUpdate(TCore *);
__inline void Core_StopHeatMotor(TCore *);
Bool HeatMotorControl(TCore *);
void StopByCommand(TCore *, Int);
void StopByFault(TCore *);
void StartByCommand(TCore *, Int, Uns);

extern TCore g_Core;
extern volatile Uns setTorque;
#endif // CORE_

