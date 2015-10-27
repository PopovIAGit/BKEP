/*======================================================================
Имя файла:          peref_SensObserver.h
Автор:
Версия файла:
Дата изменения:
Описание:
Модуль обработки датчиков
Входы: сигналы с АЦП в Uns
Выходы: сигналы в IQ24
======================================================================*/

#ifndef PEREF_SENS_OBSERVER_
#define PEREF_SENS_OBSERVER_

//----------- Подключение заголовочных файлов -----------------------------

#include "config.h"
//--------------------- Константы-------------------------------------------
//--------------------- Макросы --------------------------------------------
//#define ADC_CONV(Value, Gain, Offset) 	(((LgInt)(Value - Offset) * Gain) >> 13)
#define ADC_CONV(Value, Gain, Offset) 		(0.00001 * (int16)((Value) - (Offset)) * (Gain))
//#define ADC_CONV(Value, Gain, Offset) 	((LgInt)(Value - Offset) * Gain)
//-------------------- Структуры ------------------------------------------
// Параметры датчиков
typedef struct {
	Uns				*p_UR_Mpy;		// Мультипликатор для напряжения R
	Uns 			*p_US_Mpy;		// Мультипликатор для напряжения S
	Uns 			*p_UT_Mpy;		// Мультипликатор для напряжения T
	Uns				*p_UR_Offset;	// Сдвиг для напряжения R
	Uns 			*p_US_Offset;	// Сдвиг для напряжения S
	Uns 			*p_UT_Offset;	// Сдвиг для напряжения T
	Uns				*p_IU_Mpy;		// Мультипликатор для тока U
	Uns 			*p_IV_Mpy;		// Мультипликатор для тока V
	Uns 			*p_IW_Mpy;		// Мультипликатор для тока W
	Uns				*p_IU_Offset;	// Сдвиг для тока U
	Uns 			*p_IV_Offset;	// Сдвиг для тока V
	Uns 			*p_IW_Offset;	// Сдвиг для тока W
} TSensorsParams;

// Параметры сигналов Телесуправления
typedef struct {
	Uns				*p_UOpen_Mpy;			// Мультипликатор для напряжения сигнала Open
	Uns				*p_UOpen_Offset;		// Сдвиг для напряжения сигнала Open
	Uns				*p_UClose_Mpy;			// Мультипликатор для напряжения сигнала Close
	Uns				*p_UClose_Offset;		// Сдвиг для напряжения сигнала Close
	Uns				*p_UStopOpen_Mpy;		// Мультипликатор для напряжения сигнала StopOpen
	Uns				*p_UStopOpen_Offset;	// Сдвиг для напряжения сигнала StopOpen
	Uns				*p_UStopClose_Mpy;		// Мультипликатор для напряжения сигнала StopClose
	Uns				*p_UStopClose_Offset;	// Сдвиг для напряжения сигнала StopClose
	Uns				*p_UMu_Mpy;				// Мультипликатор для напряжения сигнала MU
	Uns				*p_UMu_Offset;			// Сдвиг для напряжения сигнала MU
	Uns				*p_UDu_Mpy;				// Мультипликатор для напряжения сигнала DU
	Uns				*p_UDu_Offset;			// Сдвиг для напряжения сигнала DU
	Uns				*p_UResetAlarm_Mpy;		// Мультипликатор для напряжения сигнала ResetAlarm
	Uns				*p_UResetAlarm_Offset;	// Сдвиг для напряжения сигнала ResetAlarm
} TDigSinalParams;

// Режим подстройки оффсетов
typedef enum{
	mManual     = 0,				// Режим ручной настройки
	mAutoOffset = 1,				// Режим автонайтроки offset
	mTest		= 2					// Режим тест
} TOffsetMode;

// Основная структура настройки датчиков тока и напряжения
typedef struct {
	TSensorsParams		parSensors;		// Параметр: параметры датчиков
	TOffsetMode		    pOffsetMode;	// Параметр: способ вычисления сдвига (Offset)
	float32				URinp;			// Вход: напряжение R
	float32 			USinp;			// Вход: напряжение S
	float32 			UTinp;			// Вход: напряжение T
	float32				IUinp;			// Вход: ток U
	float32 			IVinp;			// Вход: ток V
	float32	 			IWinp;			// Вход: ток W
	float32				URout;			// Выход: напряжение R
	float32 			USout;			// Выход: напряжение S
	float32 			UTout;			// Выход: напряжение T
	float32				IUout;			// Выход: ток U
	float32 			IVout;			// Выход: ток V
	float32 			IWout;			// Выход: ток W
} TSensObserver;

// Основная структура настройки датчиков тока и напряжения
typedef struct {
	TDigSinalParams		parSensors;		// Параметр: параметры датчиков
	TOffsetMode		    pOffsetMode;	// Параметр: способ вычисления сдвига (Offset)

	float32				UOpenInp;		// Вход: напряжение
	float32 			UCloseInp;		// Вход: напряжение
	float32 			UStopOpenInp;	// Вход: напряжение
	float32 			UStopCloseInp;	// Вход: напряжение
	float32 			UMuInp;			// Вход: напряжение
	float32 			UDuInp;			// Вход: напряжение
	float32 			UResetAlarmInp;	// Вход: напряжение

	float32				UOpenOut;		// Вход: напряжение
	float32 			UCloseOut;		// Вход: напряжение
	float32 			UStopOpenOut;	// Вход: напряжение
	float32 			UStopCloseOut;	// Вход: напряжение
	float32 			UMuOut;			// Вход: напряжение
	float32 			UDuOut;			// Вход: напряжение
	float32 			UResetAlarmOut;	// Вход: напряжение

} TSensTuObserver;

//------------------- Глобальные переменные -------------------------------------
//------------------- Протатипы функций -----------------------------------------
void Peref_SensObserverInit(TSensObserver *p);
void SetOffsets(TSensObserver *p);
void Peref_SensObserverUpdate(TSensObserver *p);
void Peref_SensTuObserverUpdate(TSensTuObserver *p);

void Peref_SensTuObserverInit(TSensTuObserver *p);


#endif
