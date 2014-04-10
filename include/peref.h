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
//#include "peref_Filter1.h"
//#include "peref_Filter3.h"
//#include "peref_SensObserver.h"
//#include "peref_SinObserver.h"
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
	//TPerefPosition		position;		// Энкодер, калибровка и т.д.
	//TPerefEncoder		position;		// Энкодер, калибровка и т.д.
	// --- Лампочки
	//TLeds				leds;
	// --- ТЕН
	//TTen				ten;
	// ---
	//Uns					HallOpen;
	//Uns					HallClose;
	//Uns					HallStopMu;
	//Uns					HallStopDu;
	Int					tempSensor;
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
