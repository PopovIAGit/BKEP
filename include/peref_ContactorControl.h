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
#include "g_Ram.h"
//--------------------- Константы-------------------------------------------
#define CONT_ERR_TIME	(0.200 * Prd50HZ) // Время задержки на включение выключение контактора
//--------------------- Макросы --------------------------------------------
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
void ContactorControl(TContactorControl *);
void ContactorErrClear(TContactorControl *);

#endif
