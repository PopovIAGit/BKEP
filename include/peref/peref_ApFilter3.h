/*======================================================================
Имя файла:          ApFilter3.h
Автор:              ПоповИ.А.
Версия файла:       01.00
Дата изменения:		04/04/14
Применяемость:      Совместно с библиотекой IQmath
Описание:
Апериодический фильтр третьего порядка
======================================================================*/

#ifndef APFILTER3_
#define APFILTER3_

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// Структура для работы с фильтром
typedef struct APFILTER3 {
	Float Input;   // Входной сигнал lgint
	Float Output;  // Выходной сигнал
	Float Output1; // Входной сигнал
	Float PrevOut1;// Выходной сигнал
	Float Output2; // Входной сигнал
	Float PrevOut2;// Выходной сигнал lgint
	Float dt;      // Постоянная времени семплирования
	Float Tf;      // Постоянная времени фильтра
	Float K1;      // Коэффициент фильтра
	Float K2;      // Коэффициент фильтра
	Float PrevOut; // Предыдущее значение выходного сигнала
} APFILTER3;

void peref_ApFilter3Init(APFILTER3 *p, Uns Freq, Float Tf);
void peref_ApFilter3Calc(APFILTER3 *);
void peref_ApFilter3Reset(APFILTER3 *);

#ifdef __cplusplus
}
#endif // extern "C"

#endif
