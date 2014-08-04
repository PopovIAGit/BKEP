/*======================================================================
Имя файла:          peref_SinObserver.h
Автор:				Попов И.А.
Версия файла:		1.0
Дата изменения:		04.04.14
Описание: Обработка синусойды тока и напряжения
Вход  - Синусоидальные значения тока и напряжения
Выход - RMS значения I и U, полярность, угол фаз
======================================================================*/

#ifndef PEREF_SIN_OBSERVER_
#define PEREF_SIN_OBSERVER_

//----------- Подключение заголовочных файлов -----------------------------
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif
//--------------------- Константы------------------------------------------
#define DIRECT_RST	0
#define REVERS_TSR  1

#define PHO_ANGLE	30	//
#define MIN_ANGLE	120
#define MAX_ANGLE	250
#define BASE_ANGLE	360
#define RMS_SCALE	(LgUns)(1.11*32768/BASE_ANGLE)

//--------------------- Макросы -------------------------------------------
//--------------------  Структуры ------------------------------------------
typedef struct
{
  Int 	Input;			// 	Входное мгновенное значение
  Uns 	Output;			//	Выходное действующее значение
  Uns 	Sign;			//	Знак входного сигнала
  Int 	Polarity;		//	Полярность
  Uns 	CurAngle;		//	Угол фазы сигнала
  Uns   StepAngle;		// 	Шаг изменения угла
  Uns   Counter;		// 	Счетчик для более точного расчета интеграла
  LgUns Sum;			//	Интеграл мгновенных значений
} TSinPhaseObserver;

typedef struct
{
  Float Input;			// 	Входное мгновенное значение
  Float Output;			//	Выходное действующее значение
  Uns 	Sign;			//	Знак входного сигнала
  Int 	Polarity;		//	Полярность
  Uns 	CurAngle;		//	Угол фазы сигнала
  Uns   StepAngle;		// 	Шаг изменения угла
  Uns   Counter;		// 	Счетчик для более точного расчета интеграла
  Float Sum;			//	Интеграл мгновенных значений
} TSinPhaseObserverFloat;

typedef struct
{
	Int Direction;
	Int TmpDir;
	Uns Timeout;
	Uns Timer;
	TSinPhaseObserverFloat 	*UR;
	TSinPhaseObserverFloat	*US;
	TSinPhaseObserverFloat	*UT;
} TPhaseOrder;

typedef struct
{
	TSinPhaseObserverFloat UR;
	TSinPhaseObserverFloat US;
	TSinPhaseObserverFloat UT;
	TSinPhaseObserverFloat IU;
	TSinPhaseObserverFloat IV;
	TSinPhaseObserverFloat IW;

}TSinObserver;

typedef struct
{
	TSinPhaseObserverFloat	sigOpen;
	TSinPhaseObserverFloat	sigClose;
	TSinPhaseObserverFloat	sigStop;
	TSinPhaseObserverFloat	sigMU;
	TSinPhaseObserverFloat	sigResetAlarm;
	TSinPhaseObserverFloat	sigReadyTU;
	TSinPhaseObserverFloat	sigDU;
}TSinSignalObserver;

//--------------------  Глобальные переменные ------------------------------
//--------------------  Протатипы функций ----------------------------------

void Peref_SinObserverInit(TSinPhaseObserver *p, Uns Freq);
void Peref_SinObserverUpdate(TSinPhaseObserver *p);
void Peref_PhaseOrderUpdate(TPhaseOrder *p);

void Peref_SinObserverInitFloat(TSinPhaseObserverFloat *p, Uns Freq);
void Peref_SinObserverUpdateFloat(TSinPhaseObserverFloat *p);



#ifdef __cplusplus
}
#endif // extern "C"

#endif
