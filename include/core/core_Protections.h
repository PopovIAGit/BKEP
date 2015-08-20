/*======================================================================
Имя файла:          core_Protections.h
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		25.04.2014
Описание:
модуль защит электропривода
======================================================================*/

#ifndef CORE_PROTECTIONS_
#define CORE_PROTECTIONS_

//----------- Подключение заголовочных файлов ------------------------------
#include "config.h"
#include "core_ProtectionsAlarm.h"
#include "core_ProtectionI2T.h"
#include "core_ProtectionSHC.h"
#include "g_Structs.h"

//--------------------- Константы-------------------------------------------
#define PROTECT_SCALE (0.100 * Prd50HZ)

// Номера битов g_Structs.h -> TProcessReg

#define NoMove_bit			0
#define PhOrd_bit			1
#define NoClose_bit			2
#define	NoOpen_bit			3
#define NoCalib_bit			4
#define Overway_bit			5
#define Mufta_bit			6
#define MuDuDef_bit			7

// Номера битов g_Structs.h -> TNetReg
#define UvR_bit				0
#define UvS_bit				1
#define UvT_bit				2
#define OvR_bit				3
#define OvS_bit				4
#define OvT_bit				5
#define VSk_bit				7
#define BvR_bit				8
#define BvS_bit				9
#define BvT_bit				10
#define OvR_max_bit			11
#define OvS_max_bit			12
#define OvT_max_bit			13


// Номера битов g_Structs.h -> TLoadReg
#define PhlU_bit			0
#define PhlV_bit			1
#define PhlW_bit			2
#define I2t_bit				3
#define ShCU_bit			4
#define ShCV_bit			5
#define ShCW_bit			6
#define ISkew_bit			11

// Номера битов g_Structs.h -> TDeviceReg
#define PosSens_bit			0
#define Memory1_bit			1
#define Memory2_bit			2
#define Rtc_bit				3
#define TSens_bit			4
#define Th_BCP_bit			5
#define Tl_BCP_bit			6
#define Th_BCD_bit			7
#define Tl_BCD_bit			8

//--------------------- Макросы --------------------------------------------
#define PRT_CFG_SET(CanBeReseted, Level, Bit, Hyst) \
	((CanBeReseted<<1)|(Level<<3)|(Bit<<4)|(Hyst<<8))
//-------------------- Структуры -------------------------------------------

typedef struct {
	// Входы --------------------------------------------------
	// -------- Диагностика процесса ------------
		TAlarmElem			NoMove;				// Нет движения - авария
	// -------- Диагностика сети ----------------
		TAlarmElem 			underVoltageR;		// Понижение напряжения R  - авария
		TAlarmElem 			underVoltageS;		// Понижение напряжения S  - авария
		TAlarmElem 			underVoltageT;		// Понижение напряжения T  - авария
		TAlarmElem 			underDefVoltageR;	// Понижение напряжения R  - неисправность
		TAlarmElem 			underDefVoltageS;	// Понижение напряжения S  - неисправность
		TAlarmElem 			underDefVoltageT;	// Понижение напряжения T  - неисправность
		TAlarmElem 			overVoltageR;		// Превышение напряжения R - авария
		TAlarmElem 			overVoltageS;		// Превышение напряжения S - авария
		TAlarmElem 			overVoltageT;		// Превышение напряжения T - авария
		TAlarmElem 			overDefVoltageR;	// Превышение напряжения R - неисправность
		TAlarmElem 			overDefVoltageS;	// Превышение напряжения S - неисправность
		TAlarmElem 			overDefVoltageT;	// Превышение напряжения T - неисправность
		TAlarmElem 			overMax_VoltageR;	// Превышение напряжения R на 47% - авария
		TAlarmElem 			overMax_VoltageS;	// Превышение напряжения S на 47% - авария
		TAlarmElem 			overMax_VoltageT;	// Превышение напряжения T на 47% - авария
		TAlarmElem			voltSkew;			// Асимметрия напряжения входной сети - неисправность
		TAlarmElem 			breakVoltR;			// Обрыв фазы R - авария
		TAlarmElem 			breakVoltS;			// Обрыв фазы S - авария
		TAlarmElem 			breakVoltT;			// Обрыв фазы T - авария
	// -------- Диагностика нагруски ------------
		TAlarmElem			breakCurrU;			// Обрыв фазы U	- неисправность
		TAlarmElem			breakCurrV;			// Обрыв фазы V - неисправность
		TAlarmElem			breakCurrW;			// Обрыв фазы W - неисправность
		TAlarmElem			currSkew;			// Ассиметрия тока - неисправность
		TAlarmI2T 			I2t;				// Время-токовая перегрузка - авария
		TAlarmSHC			ShC_U;				// Короткое замыкание фазы U - авария
		TAlarmSHC			ShC_V;				// Короткое замыкание фазы V - авария
		TAlarmSHC			ShC_W;				// Короткое замыкание фазы W - авария
	// -------- Диагностика устройства ----------
		TAlarmElem			overHeatBCP;			// Перегрев	БКП - авария
		TAlarmElem			underColdBCP; 			// Переохлаждение БКП - неисправность
		TAlarmElem			overHeatBCD;			// Перегрев БКД - неисправность
		TAlarmElem			underColdBCD; 			// Переохлаждение БКД - неисправность
	// Выходы ---------------------------------------------------------
		TFltUnion			outFaults;			// Аварии
		TFltUnion			outDefects;			// Неисправности
		Uns					ShcTmpState;		// Промежуточная переменная для определения КЗ
		Uns					registerBrCurr;		// Регистр аварий обрыва фаз двигателя
		Uns					registerBrVolt;		// Регистр аварий обрыва фаз питания
		Uns 				MuffFlag;			// Флаг муфты
		Uns				NoMoveFlag;
		Bool				ShcReset;			// Флаг по которому сбрасывается КЗ
		Uns				FaultDelay;			// Пауза после инициализации для включения защит

}TCoreProtections;

//------------------- Глобальные переменные --------------------------------
//------------------- Протатипы функций ------------------------------------

void Core_ProtectionsInit(TCoreProtections *);		// Инициализация модуля защит
void Core_ProtectionsUpdate(TCoreProtections *);	// Реакция системы на срабатывание защит
void Core_ProtectionsReset(TCoreProtections *);		// Сброс защит при повторном пуске
void Core_ProtectionsClear(TCoreProtections *);		// Сброс всех защит
void Core_ProtectionsEnable(TCoreProtections *);	// Включение/Выключение защит
void Core_DevProc_FaultIndic(TCoreProtections *);	// Индикация ошибок устройства и технологического процесса
void EngPhOrdPrt(TCoreProtections *);				// неверное чередование фаз двигателя

#endif
