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
	LgInt Input;   // Входной сигнал
	LgInt Output;  // Выходной сигнал
	LgInt Output1; // Входной сигнал
	LgInt PrevOut1;// Выходной сигнал
	LgInt Output2; // Входной сигнал
	LgInt PrevOut2;// Выходной сигнал
	Float dt;      // Постоянная времени семплирования
	Float Tf;      // Постоянная времени фильтра
	Float K1;      // Коэффициент фильтра
	Float K2;      // Коэффициент фильтра
	LgInt PrevOut; // Предыдущее значение выходного сигнала
} APFILTER3;

void peref_ApFilter3Init(APFILTER3 *p, Uns Freq, Uns Tf);
void peref_ApFilter3Calc(APFILTER3 *);
void peref_ApFilter3Reset(APFILTER3 *);

#ifdef __cplusplus
}
#endif // extern "C"

#endif
