/*======================================================================
Имя файла:          peref.h
Автор:
Версия файла:
Дата изменения:
Применяемость:
Описание:
======================================================================*/

#ifndef PEREF_
#define PEREF_

//----------- Подключение заголовочных файлов -----------------------------
#include "std.h"
#include "config.h"
#include "g_Ram.h"
#include "peref_ApFilter1.h"  		// PIA 03.04.14
#include "peref_ApFilter3.h"  		// PIA 04.04.14
#include "peref_SensObserver.h" 	// PIA 07.04.14
#include "peref_SinObserver.h"		// PIA 08.04.14
#include "peref_Calibs.h"			// PIA 14.04.14
#include "peref_ContactorControl.h"	// PIA 17.04.14
//#include "peref_DisplayDrv.h"
//#include "peref_LedsDrv.h"
//#include "peref_TempObserver.h"
//#include "peref_Clock.h"
//#include "peref_SDModDrv.h"
//#include "peref_EncoderDrv.h"
//#include "peref_TempDrv.h"
//#include "peref_Dac7513Drv.h"
//
#ifdef __cplusplus
extern "C" {
#endif

//-------------------- Структуры -------------------------------------------
// Структура для работы с фильтрами переферии
typedef struct {
	// Фильтры U
	APFILTER3  URfltr;
	APFILTER3  USfltr;
	APFILTER3  UTfltr;
	// Фильтры I
	APFILTER3  IUfltr;
	APFILTER3  IVfltr;
	APFILTER3  IWfltr;
	// ------
	TSensObserver	sensObserver;		// Масштабирование сигналов с датчиков
	TSinObserver	sinObserver;		// Вычисление RMS
	TPhaseOrder		phaseOrder; 		// Чередование фаз сети
	//------
	APFILTER1 Phifltr;					// Фильтр угола фи
	APFILTER1 Umfltr;					// Фильтр среднего напряжения
	APFILTER3 Imfltr;					// Фильтр среднего тока

	TPerefPosition Position;			// Калибровка датчика положения и расчет скорости
	TContactorControl ContactorControl;	// Управление контакторами

	Uns Umid;
	Uns Imid;
	Uns AngleUI;
} TPeref;

//------------------- Протатипы функций ------------------------------------
void Peref_Init(TPeref *);
void Peref_18kHzCalc(TPeref *);
void Peref_50HzCalc (TPeref *);
void Peref_10HzCalc (TPeref *);

//------------------- Глобальные переменные --------------------------------
extern	TPeref	g_Peref;

#ifdef __cplusplus
}
#endif // extern "C"

#endif
