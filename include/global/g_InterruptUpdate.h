/*======================================================================
»м€ файла:          g_InterruptUpdate.h
јвтор:
¬ерси€ файла:
ƒата изменени€:
ќписанте: 
	«аголовочный файл дл€ организации работы функций блока Ѕ ƒ
‘ункции раскиданы по группам прерываний: 18к√ц, 2к√ц, 200√ц, 50√ц, 10 √ц.
Ёти частоты заданы жестко, мен€ть их - не рекомендуетс€.
ћаксимальное количество задач (функций) дл€ каждой группы прерываний:
2к√ц - не более 4-х задач
200√ц - не более  20-ти задач 
50 √ц - не более  80-ти задач
10 √ц - не более  200-т задач
======================================================================*/

#include "config.h"	

#include "core.h"
#include "comm.h"
#include "g_Ram.h"
//#include "drive.h"
#include "peref.h"
#include "stat.h"
#include "csl/csl_dlog.h"

//--------ќЅЏя¬Ћ≈Ќ»≈ ћј –ќ—ќ¬------------------------------------------------

// ћакрос преобразовани€ функции в указатель на функцию,
// аргумента функции в указатель на аргумент функции
#define PrdElemInit(Name, Argument) \
	{ (TPrdFunc)(Name), (void *)(Argument), 0 }

// ћакрос подсчета количества элементов массива
#define TaskCount(List)	(sizeof(List) / sizeof(TPeriodicalFunction))

#define TASK_COUNT_MAX_2kHZ  10		//ћаксимальное кол-во задач дл€ частоты 2 к√ц
#define TASK_COUNT_MAX_200HZ 20		//ћаксимальное кол-во задач дл€ частоты 200 √ц
#define TASK_COUNT_MAX_50HZ  80		//ћаксимальное кол-во задач дл€ частоты 50 √ц
#define TASK_COUNT_MAX_10HZ  200	//ћаксимальное кол-во задач дл€ частоты 10 √ц

//---------ќѕ–≈ƒ≈Ћ≈Ќ»≈--“»ѕќ¬--ƒјЌЌџ’----------------------------------------

typedef void (*TPrdFunc)(void *Data);	// ќпределение типа "”казатель на функцию"

typedef struct _TPrdElem				// ќпределение типа данных структуры, котора€ содержит пол€:
{
	TPrdFunc Func;						// - указатель на функцию;
    void *Data;							// - указатель на аргумент функции
    Uns CpuTime;						// - процессорное врем€ выполнени€
} TPeriodicalFunction;					// ќбъ€вление структуры соответствующего типа

//---------ѕ–ќ“ќ“»ѕџ---‘”Ќ ÷»…-----------------------------------------------
extern  void InterruptUpdate	(void); 		//‘ункци€ обработки прерываний

// ¬ысокоприоритетные прерывани€ на частоте 18 к√ц

// ================================ 18 к√ц ==================================

TPeriodicalFunction Task18kHz[] =         
{
	PrdElemInit(Peref_SensObserverUpdate,				&g_Peref.sensObserver),
	PrdElemInit(Peref_SensTuObserverUpdate,				&g_Peref.InDigSignalObserver),
	PrdElemInit(Peref_18kHzCalc,						&g_Peref),
	PrdElemInit(SciMasterConnBetweenBlockCommTimer,		&g_Comm.mbBkp),	//на 18к√ц
	PrdElemInit(Core_Protections18kHzUpdate,			&g_Core.Protections),
//	PrdElemInit(MonitorUpdate,							Null),
};

//—юда вставл€ем функции дл€ соответствующей группы

// ================================ 2 к√ц ==================================

TPeriodicalFunction Task2kHz[] =          //Ќе более 8-х задач
{
	PrdElemInit(SerialCommTimings,						&g_Comm.mbAsu),	//на 2 к√ц
	PrdElemInit(SerialCommTimings,						&g_Comm.mbShn),
	PrdElemInit(SerialCommTimings,						&g_Comm.mbBt),	//на 2 к√ц
	PrdElemInit(FM25V10_Update,							&Eeprom1),		//на 2 к√ц
	PrdElemInit(FM25V10_Update,							&Eeprom2),		//на 2 к√ц


};
// ================================ 200 √ц ==================================

TPeriodicalFunction Task200Hz[] =       	//не более  20-ти задач
{
	PrdElemInit(Peref_CalibUpdate, 						&g_Peref.Position),
	PrdElemInit(Core_LowPowerControl, 					&g_Core),
	PrdElemInit(DataBufferPre,							&g_Stat),
	PrdElemInit(LogEvControl,							&g_Stat),
	PrdElemInit(LogCmdControl,							&g_Stat),
	PrdElemInit(LogParamMbExec,							&g_Stat),
	PrdElemInit(LogParamControl,						&g_Stat),
	PrdElemInit(ImTimer,				                &g_Stat.Im),// на 200√
	PrdElemInit(DisplDrvUpdate,             			&g_Peref.Display),
};

// ================================ 50 √ц ==================================

TPeriodicalFunction Task50Hz[] =        //не более  80-ти задач
{
	PrdElemInit(Peref_50HzCalc,					&g_Peref),
	PrdElemInit(Peref_SpeedCalc, 				&g_Peref.Position),
	PrdElemInit(ContactorControlUpdate, 		&g_Peref.ContactorControl),
	PrdElemInit(Core_DefineCtrlParams, 			&g_Core),
	PrdElemInit(Core_ControlMode, 				&g_Core),
	PrdElemInit(Core_CalibControl, 				&g_Core),
	PrdElemInit(Core_CalibStop, 				&g_Core),
	PrdElemInit(Core_TorqueCalc, 				&g_Core.TorqObs),
	PrdElemInit(Core_ValveDriveUpdate, 			&g_Core.VlvDrvCtrl),
	PrdElemInit(Core_ProtectionsEnable,		    &g_Core.Protections),
	PrdElemInit(EngPhOrdPrt,					&g_Core.Protections),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.NoMove),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overVoltageR),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overVoltageS),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overVoltageT),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overMax_VoltageR),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overMax_VoltageS),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overMax_VoltageT),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overDefVoltageR),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overDefVoltageS),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overDefVoltageT),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.underVoltageR),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.underVoltageS),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.underVoltageT),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.underDefVoltageR),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.underDefVoltageS),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.underDefVoltageT),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.breakVoltR),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.breakVoltS),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.breakVoltT),
	PrdElemInit(Core_ProtectionI2TUpdate,		&g_Core.Protections.I2t),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.breakCurrU),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.breakCurrV),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.breakCurrW),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overHeatBCD),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.overHeatBCP),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.underColdBCD),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.Protections.underColdBCP),
	PrdElemInit(Comm_50HzCalc,					&g_Comm),
	PrdElemInit(GetCurrentCmd,					&g_Stat),
	PrdElemInit(Core_MenuDisplay,				&g_Core.menu),
	PrdElemInit(Core_Protections50HZUpdate,		&g_Core.Protections),
	PrdElemInit(Comm_TuTsUpdate,				&g_Comm.digitInterface),
	PrdElemInit(Comm_CommandUpdate,				&g_Comm),

};

// ================================ 10 √ц ==================================

TPeriodicalFunction Task10Hz[] =        //не более  200-т задач
{
    PrdElemInit(Peref_Calibration, 			&g_Peref.Position),
	PrdElemInit(CalcClbCycle, 				&g_Peref.Position),
	PrdElemInit(g_Ram_Update,				&g_Ram),
	PrdElemInit(Peref_SpeedCalc,			&g_Peref.Position),
	PrdElemInit(Peref_LedsUpdate,			&g_Peref.leds),
	PrdElemInit(Peref_10HzCalc,				&g_Peref),
	PrdElemInit(BluetoothTimer,				&g_Comm.Bluetooth),	//на 10 √ц
	PrdElemInit(RTC_Control,				NULL),				//на 10 √ц
	PrdElemInit(BluetoothActivation,		&g_Comm.Bluetooth),	//на 10 √ц
	PrdElemInit(ImTimerIndex,				&g_Stat.Im),		//на 10 √ц
	PrdElemInit(Core_MuDuControl,			&g_Core),			//на 10 √ц
	PrdElemInit(Core_DevProc_FaultIndic,	&g_Core.Protections),
	PrdElemInit(Core_OnOff_TEN,             &g_Core.Temper),
	PrdElemInit(TekModbusParamsUpdate,		NULL),				//на 10 √ц
	PrdElemInit(Core_DisplayFaultsUpdate,			&g_Core.DisplayFaults),				//на 10 √ц

};
//------------ онец файла-----------------------------------------------



