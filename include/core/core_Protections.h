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
	((CanBeReseted<<1)|(Level<<2)|(Bit<<3)|(Hyst<<7))
//-------------------- Структуры -------------------------------------------

typedef struct {
	// Входы --------------------------------------------------
	// -------- Диагностика процесса ------------
	// -------- Диагностика сети ----------------
		TAlarmElem 			underVoltageR;	// Понижение напряжения R
		TAlarmElem 			underVoltageS;	// Понижение напряжения S
		TAlarmElem 			underVoltageT;	// Понижение напряжения T
		TAlarmElem 			overVoltageR;	// Превышение напряжения R
		TAlarmElem 			overVoltageS;	// Превышение напряжения S
		TAlarmElem 			overVoltageT;	// Превышение напряжения T
		TAlarmElem			voltSkew;		// Асимметрия напряжения входной сети
		TAlarmElem 			breakVoltR;		// Обрыв фазы R
		TAlarmElem 			breakVoltS;		// Обрыв фазы S
		TAlarmElem 			breakVoltT;		// Обрыв фазы T
	// -------- Диагностика нагруски ------------
		TAlarmElem			breakCurrU;		// Обрыв фазы U
		TAlarmElem			breakCurrV;		// Обрыв фазы V
		TAlarmElem			breakCurrW;		// Обрыв фазы W
		TAlarmElem			currSkew;		// Ассиметрия тока
		TAlarmI2T 			I2t;			// Время-токовая перегрузка
		TAlarmSHC			ShC_U;			// Короткое замыкание фазы U
		TAlarmSHC			ShC_V;			// Короткое замыкание фазы V
		TAlarmSHC			ShC_W;			// Короткое замыкание фазы W
	// -------- Диагностика устройства ----------
		TAlarmElem			overHeatBCP;	// Перегрев	БКП
		TAlarmElem			underColdBCP; 	// Переохлаждение БКП
		TAlarmElem			overHeatBCD;	// Перегрев БКД
		TAlarmElem			underColdBCD; 	// Переохлаждение БКД
	// Выходы ---------------------------------------------------------
		TFltUnion			outFaults;

}TCoreProtections;

//------------------- Глобальные переменные --------------------------------
//------------------- Протатипы функций ------------------------------------

void Core_ProtectionsInit(TCoreProtections *);
void Core_ProtectionsUpdate(TCoreProtections *);
void Core_ProtectionsReset(TCoreProtections *);
void Core_ProtectionsClear(TCoreProtections *);
void Core_ProtectionsEnable(TCoreProtections *);
Bool IsFaultExist(TPrtMode Mode);
Bool IsDefectExist(TPrtMode Mode);
void Core_FaultIndication(TCoreProtections *);

#endif
