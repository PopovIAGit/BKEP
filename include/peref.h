/*======================================================================
Имя файла:          peref.h
Автор:
Версия файла:
Дата изменения:
Применяемость:      Совместно с библиотекой IQmath
Описание:
======================================================================*/

#ifndef PEREF_
#define PEREF_

#include "std.h"
#include "config.h"
#include "g_Ram.h"
#include "peref_ApFilter1.h"  	// PIA 03.04.14
#include "peref_ApFilter3.h"  	// PIA 04.04.14
#include "peref_SensObserver.h" // PIA 07.04.14
#include "peref_SinObserver.h"	// PIA 08.04.14
//#include "peref_DisplayDrv.h"
//#include "peref_Position.h"
//#include "peref_LedsDrv.h"
//#include "peref_TempObserver.h"
//#include "peref_Clock.h"
//#include "peref_SDModDrv.h"
//#include "peref_EncoderDrv.h"
//#include "peref_TempDrv.h"
//#include "peref_Dac7513Drv.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
	Uns all;
	struct {
   		Uns ten:1;			// 0     Тен Вкл.Выкл
		Uns testIso:1;		// 1     Тест изоляции
		Uns dsp_enb_bf:1;	// 2     микросхема Васи (формирующая логика)
		Uns ten_bubi:1;		// 3	 Управление ТЭНом на БУБИ
		Uns reserv:12;		// 4-15	 Резерв
	} bit;
} TShiftReg2Data;

typedef struct
{
	Uns					input;			// Вход: Uns
	LgUns				buffer[20];		// Кольцевой буффер
	Uns					output;			// Выход
} TSensor;

// Структура для управления нагревательным элементом (ТЕН)
typedef struct
{
	Uns					timerTen;		// Таймер
	Uns					timerTenTimeOut;// Период цикла работы тена
	Uns					tenEnableTime;	// Промежуток включения ТЕНА
} TTen;

//----------------------------------------------
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
	TSensObserver	sensObserver;	// Масштабирование сигналов с датчиков
	TSinObserver	sinObserver;	// Вычисление RMS
	TPhaseOrder		phaseOrder; 	// Чередование фаз сети

	//------
	APFILTER1 Phifltr;				// Фильтр угола фи
	APFILTER1 Umfltr;				// Фильтр среднего напряжения
	APFILTER1 Imfltr;				// Фильтр среднего тока
} TPeref;

void Peref_Init(TPeref *);
void Peref_18kHzCalc(TPeref *);
void Peref_50HzCalc (TPeref *);
void Peref_10HzCalc (TPeref *);
void Peref_TenInit(TTen *, Uns, Uns, Uns);
void Peref_TenControl(void);
void Peref_TenRateCalc(TTen *);
Uns CircleBufferCalc (TSensor *);

extern	TPeref	g_Peref;

#ifdef __cplusplus
}
#endif // extern "C"

#endif
