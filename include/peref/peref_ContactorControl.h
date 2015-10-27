/*======================================================================
Имя файла:          peref_ContactorControl.h
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		16.04.2014
Описание:
Модуль Управления контакторами
======================================================================*/

#ifndef PEREF_CONTACTOR_CONTROL_
#define PEREF_CONTACTOR_CONTROL_

//----------- Подключение заголовочных файлов ------------------------------
#include "config.h"
//--------------------- Константы-------------------------------------------
#define CONT_ERR_TIME	(0.200 * Prd50HZ) // Время задержки на включение выключение контактора
//--------------------- Макросы --------------------------------------------
#define SET_CONT_CLOSE(V)		CONTACTOR_1_CONTROL = V; asm(" RPT #9 || NOP")
#define SET_CONT_OPEN(V)		CONTACTOR_2_CONTROL = V; asm(" RPT #9 || NOP")

//-------------------- Структуры -------------------------------------------

typedef struct
{
	Bool ContactorError;
	Uns  ContactorCancelFlag;
	Uns  ContactorErrTimer;

	TContactorGroup *ContactorGroup;
} TContactorControl;
//------------------- Глобальные переменные --------------------------------
//------------------- Протатипы функций ------------------------------------

void ContactorInit(TContactorControl *);
void ContactorControlUpdate(TContactorControl *);
void ContactorErrClear(TContactorControl *);

#endif
