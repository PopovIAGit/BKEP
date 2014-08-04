/*======================================================================
Имя файла:          ApFilter1.h
Автор:              Саидов В.С.
Версия файла:       01.00
Дата изменения:		02/02/10
Применяемость:      Совместно с библиотекой IQmath
Описание:
Апериодический фильтр первого порядка
======================================================================*/

#ifndef APFILTER1_
#define APFILTER1_

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// Структура для работы с фильтром
typedef struct APFILTER1 {
	Float Input;   // Входной сигнал
	Float Output;  // Выходной сигнал
	Float PrevOut; // Выходной сигнал
	Float dt;      // Постоянная времени семплирования
	Float Tf;      // Постоянная времени фильтра
	Float K1;      // Коэффициент фильтра
	Float K2;      // Коэффициент фильтра
} APFILTER1;

void peref_ApFilter1Init(APFILTER1 *p, Uns Freq, Float Tf);
void peref_ApFilter1Calc(APFILTER1 *);
void peref_ApFilter1Reset(APFILTER1 *);

#ifdef __cplusplus
}
#endif // extern "C"

#endif
