/*======================================================================
Имя файла:          rtc.h
Автор:
Версия файла:
Дата изменения:
Применяемость:
Описание:
Библиотека симуляции часов реального времени и 
функций управления временем и датой

========================================================================
История:
------------------------------------------------------------------------
	17/12/09	Версия 01.00
----------------------------------------------------------------------*/

#ifndef RTC_
#define RTC_

//#include "config.h"
//#include "peref_DS1305Drv.h"

#ifdef __cplusplus
extern "C" {
#endif

// =============  ОБНОВЛЕНИЕ ЧАСОВ ===========================
// Константы режима изменения часа (AM/PM)
#define RTC_AM_PM         0x20 // Текущий режим (0 - AM, 1 - PM)
#define RTC_AM_PM_MODE    0x40 // Наличие режима

// Структуры для работы с временем и датой
typedef union _TTimeVar {
  Uns all;
  struct {
     Uns Min:6;        // 0-5   Минута
     Uns Hour:10;      // 6-15  Час
  } bit;
} TTimeVar;

typedef union _TDateVar {
  Uns all;
  struct {
  	Uns Day:5;		// 0-4   День месяца
	Uns Month:4;	// 5-8   Месяц
  	Uns Year:7;		// 9-15  Год
  } bit;
} TDateVar;

typedef union {
  Uns      all;     // Код времени/даты
  TTimeVar Time;    // Структура запакованного времени
  TDateVar Date;    // Структура запакованной даты
} TTimeDate;

// Структуры для работы с часами реального времени
typedef struct RTC_Obj {
	Byte Sec;         // Секунда
	Byte Min;         // Минута
	Byte Hour;        // Час
	Byte WeekDay;     // День недели
	Byte Day;         // День месяца
	Byte Month;       // Месяц
	Byte Year;        // Год
	Byte PrevHour;    // Предыдущий час для корректировки
} RTC_Obj;

typedef struct RTC_Lite {
	Byte Sec;         // Секунда
	Byte Min;         // Минута
	Byte Hour;        // Час
	Byte Day;         // День
} RTC_Lite;

void Peref_RTCUpdate(void *);
void RTC_Tick(void *);
char RTC_DefMaxDay(char, char);
void RTC_GetTime(void *, TTimeVar *);
void RTC_GetDate(void *, TDateVar *);
void RTC_SetTime(void *, TTimeVar *, char);
void RTC_SetDate(void *, TDateVar *, char);
char RTC_TimeCorr(void *, int Corr);

#ifdef __cplusplus
}
#endif // extern "C"

#endif



