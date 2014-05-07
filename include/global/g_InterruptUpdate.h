/*======================================================================
Имя файла:          g_InterruptUpdate.h
Автор:
Версия файла:
Дата изменения:
Описанте: 
	Заголовочный файл для организации работы функций блока БКД
Функции раскиданы по группам прерываний: 18кГц, 2кГц, 200Гц, 50Гц, 10 Гц.
Эти частоты заданы жестко, менять их - не рекомендуется.
Максимальное количество задач (функций) для каждой группы прерываний:
2кГц - не более 4-х задач
200Гц - не более  20-ти задач 
50 Гц - не более  80-ти задач
10 Гц - не более  200-т задач
======================================================================*/

#include "config.h"	

#include "core.h"
#include "comm.h"
#include "g_Ram.h"
//#include "drive.h"
#include "peref.h"
#include "stat.h"
#include "csl/csl_dlog.h"

//--------ОБЪЯВЛЕНИЕ МАКРОСОВ------------------------------------------------

// Макрос преобразования функции в указатель на функцию,
// аргумента функции в указатель на аргумент функции
#define PrdElemInit(Name, Argument) \
	{ (TPrdFunc)(Name), (void *)(Argument) }

// Макрос подсчета количества элементов массива
#define TaskCount(List)	(sizeof(List) / sizeof(TPeriodicalFunction))

#define TASK_COUNT_MAX_2kHZ  10		//Максимальное кол-во задач для частоты 2 кГц
#define TASK_COUNT_MAX_200HZ 20		//Максимальное кол-во задач для частоты 200 Гц
#define TASK_COUNT_MAX_50HZ  80		//Максимальное кол-во задач для частоты 50 Гц
#define TASK_COUNT_MAX_10HZ  200	//Максимальное кол-во задач для частоты 10 Гц

//---------ОПРЕДЕЛЕНИЕ--ТИПОВ--ДАННЫХ----------------------------------------

typedef void (*TPrdFunc)(void *Data);	// Определение типа "Указатель на функцию"

typedef struct _TPrdElem				// Определение типа данных структуры, которая содержит поля:
{
	TPrdFunc Func;						// - указатель на функцию;
    void *Data;							// - указатель на аргумент функции
} TPeriodicalFunction;					// Объявление структуры соответствующего типа

//---------ПРОТОТИПЫ---ФУНКЦИЙ-----------------------------------------------

        void task1				(void);				//Периодическая функция
        void task1_10Hz			(void);		//Периодическая функция, выполняемая на частоте 10 Гц
        void task1_18Hz			(void);		//Периодическая функция, выполняемая на частоте 18 Гц
extern  void InterruptInit		(void);		//Функция инициализации прерываний
extern  void InterruptUpdate	(void); 		//Функция обработки прерываний

//-------ОБЪЯВЛЕНИЕ--ПЕРЕМЕННЫХ----------------------------------------------

int     count18kHz,             //Счетчик прерываний частотой 18 кГц
        count2kHz,              //Счетчик прерываний частотой 2  кГц
        count200Hz,             //Счетчик прерываний частотой 200 Гц
        count50Hz,              //Счетчик прерываний частотой 10  Гц

        tskCnt2kHz,             //Счетчик выполненных задач 2 кГц. Он же номер выполняемой задачи
        tskCnt200Hz,            //Счетчик выполненных задач 200Гц. Он же номер выполняемой задачи
        tskCnt50Hz,             //Счетчик выполненных задач 50 Гц. Он же номер выполняемой задачи
        tskCnt10Hz,             //Счетчик выполненных задач 10 Гц. Он же номер выполняемой задачи

        tskMaxCnt18kHz,			//Количество задач для частоты 18 кГц
        tskMaxCnt2kHz,          //Количество задач для частоты 2  кГц
        tskMaxCnt200Hz,         //Количество задач для частоты 200 Гц
        tskMaxCnt50Hz,          //Количество задач для частоты 50  Гц
        tskMaxCnt10Hz;          //Количество задач для частоты 10  Гц

LgUns  X, X10, X18;

// Высокоприоритетные прерывания на частоте 18 кГц

// ================================ 18 кГц ==================================

TPeriodicalFunction Task18kHz[] =         
{
	PrdElemInit(Peref_18kHzCalc,						&g_Peref),
	PrdElemInit(MonitorUpdate,							NULL),
	//PrdElemInit(AvagoEncoderAEAT84ADCalc,				&g_Peref.position),
	PrdElemInit(task1,	NULL)
};

//Сюда вставляем функции для соответствующей группы

// ================================ 2 кГц ==================================

TPeriodicalFunction Task2kHz[] =          //Не более 4-х задач
{
	//PrdElemInit(Core_ProtectionsRefresh,	&g_Core.protections),
	PrdElemInit(SerialCommTimings,						&g_Comm.mbAsu),
	//PrdElemInit(SerialCommTimings,						&g_Comm.mbShn),
	PrdElemInit(SerialCommTimings,						&g_Comm.mbBt),
	PrdElemInit(FM25V10_Update,							&Eeprom1),
	PrdElemInit(FM25V10_Update,							&Eeprom1),
	PrdElemInit(SciMasterConnBetweenBlockCommTimer,		&g_Comm.mbBkp),
	//PrdElemInit(SciMasterConnBetweenBlockUpdate,		&g_Comm.mbBkp),

};
// ================================ 200 Гц ==================================

TPeriodicalFunction Task200Hz[] =       	//не более  20-ти задач
{
//	PrdElemInit(Peref_DisplayUpdate, 					&g_Peref.display),
	PrdElemInit(Peref_CalibUpdate, 						&g_Peref.Position),
	PrdElemInit(DataBufferPre,							&g_Stat),
	PrdElemInit(LogEvControl,							&g_Stat),
	PrdElemInit(LogCmdControl,							&g_Stat),
	PrdElemInit(LogParamMbExec,							&g_Stat),
	PrdElemInit(LogParamControl,						&g_Stat),
};

// ================================ 50 Гц ==================================

TPeriodicalFunction Task50Hz[] =        //не более  80-ти задач
{
	PrdElemInit(Peref_50HzCalc,					&g_Peref),
	PrdElemInit(Peref_SpeedCalc, 				&g_Peref.Position),
	PrdElemInit(ContactorControlUpdate, 		&g_Peref.ContactorControl),
	PrdElemInit(Core_DefineCtrlParams, 			&g_Core),
	PrdElemInit(Core_ControlMode, 				&g_Core),
	PrdElemInit(Core_TorqueCalc, 				&g_Core.TorqObs),
	PrdElemInit(Core_ValveDriveUpdate, 			&g_Core.VlvDrvCtrl),
	PrdElemInit(Core_CalibControl, 				&g_Core),
	/*PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.protections.overHeatBlock),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.protections.underColdBlock),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.protections.overTemper1),
	PrdElemInit(EleSyEncoderCalc,				&g_Peref.position.encoder),*/
	//PrdElemInit(Comm_Update,					&g_Comm),
	/*PrdElemInit(Core_CommandControl,			&g_Core.commands),
	PrdElemInit(Core_CommandsUpdate,			&g_Core.commands),
	PrdElemInit(Core_CmdUpdate,					&g_Core),
	PrdElemInit(Peref_ClockControl,				&g_Peref.clock),
	PrdElemInit(Core_StatusUpdate,				&g_Core)*/
	PrdElemInit(GetCurrentCmd,							&g_Stat),
	PrdElemInit(Core_MenuDisplay,						&g_Core.menu),
};

// ================================ 10 Гц ==================================

TPeriodicalFunction Task10Hz[] =        //не более  200-т задач
{
	PrdElemInit(Peref_Calibration, 		&g_Peref.Position),
	PrdElemInit(CalcClbCycle, 			&g_Peref.Position),
	/*PrdElemInit(Core_ProtectionsUpdate,		&g_Core.protections),
	PrdElemInit(g_Ram_Update,				&g_Ram),
//	PrdElemInit(Peref_SpeedCalc,			&g_Peref.position),
	PrdElemInit(ADT7301_Update,				&g_Peref.tempSensor),*/
	//PrdElemInit(ADT75_Update,				NULL),
//	PrdElemInit(Drive_Update,				&g_Drive),
	//PrdElemInit(Peref_LedsUpdate,			&g_Peref.leds),
	//PrdElemInit(Peref_TenControl,			NULL),
	//PrdElemInit(Peref_10HzCalc,				&g_Peref),
	PrdElemInit(Peref_Calibration, 						&g_Peref.Position),
	PrdElemInit(BluetoothTimer,							&g_Comm.Bluetooth),
	PrdElemInit(task1_10Hz,								NULL)
};
//------------Конец файла-----------------------------------------------



