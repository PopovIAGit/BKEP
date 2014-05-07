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

//------------------- Глобальные переменные -------------------------------------
//------------------- Протатипы функций -----------------------------------------
void Peref_SensObserverInit(TSensObserver *p);
void SetOffsets(TSensObserver *p);
void Peref_SensObserverUpdate(TSensObserver *p);


#endif
