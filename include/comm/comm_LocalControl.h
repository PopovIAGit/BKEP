/*======================================================================
Имя файла:          comm_LocalControl.h
Автор:
Версия файла:      	1.0
Дата изменения:		12.05.2014
Описание:
Заголовочный файл модуля обработки сигналов с кнопок МПУ
======================================================================*/
#ifndef COMM_MPU_
#define COMM_MPU_

//----------- Подключение заголовочных файлов ------------------------------
#include "std.h"
//--------------------- Константы-------------------------------------------

#define BTN_OPEN_BIT    BIT0
#define BTN_CLOSE_BIT   BIT1
#define BTN_STOPMU_BIT  BIT2
#define BTN_STOPDU_BIT  BIT3


//--------------------- Макросы --------------------------------------------
#define BTN_ENABLE_TIME			(0.200 * Prd50HZ)
//-------------------- Структуры -------------------------------------------

typedef union
{
	Uns all;
	struct
	{
		Uns btn1:1;		// 0 - кнопка 1 (левая ручка)
		Uns btn2:1;		// 1 - кнопка 2 (левая ручка)
		Uns btn3:1;		// 2 - кнопка 3 (правая ручка)
		Uns	btn4:1;		// 3 - кнопка 4 (правая ручка)
		Uns rsvd2:12;	// 4-15 - резерв
	} bit;
} TInputData;
// Состояние датчиков холла блока
typedef union _THallBlock {
	Uns all;
	struct {
		Uns Open:1;       // 0     Открыть
		Uns Close:1;      // 1     Закрыть
		Uns StopMU:1;     // 2     Стоп/МУ
		Uns StopDU:1;     // 3     Стоп/ДУ
		Uns Rsvd:12;      // 4-15  Резерв
	} bit;
} THallBlock;

/*
typedef union _THallBlock
{
	Uns all;
	struct
	{
		Uns Open:1;		// 0 - кнопка 1 (левая ручка)
		Uns Close:1;		// 1 - кнопка 2 (левая ручка)
		Uns StopMU:1;		// 2 - кнопка 3 (правая ручка)
		Uns	StopDU:1;		// 3 - кнопка 4 (правая ручка)
		Uns rsvd2:12;	// 4-15 - резерв
	} bit;
} THallBlock;*/

typedef struct {
	Uns					upLevel;		// Уровень срабатывания
	Uns 				timer;			// Таймер
	Uns 				timeout;		// Время срабатывания
} TButtonsParams;

// Маска кнопок управления
typedef union _TKeyInvert {
	Uns all;
	 struct {
		Uns Open:1;       	// 0     Открыть
		Uns Close:1;      	// 1     Закрыть
		Uns Stop:1;       	// 2     Стоп
		Uns Rsvd:13;      	// 3-15  Резерв
	 } bit;
} TKeyInvert;

// Структура для кнопок МПУ
typedef struct
{
	THallBlock			*inputBCP_Data;	 // Данные с БКП
	TButtonsParams		btn1Param;		 // Параметры кнопоки 1
	TButtonsParams		btn2Param;		 // Параметры кнопоки 2
	TButtonsParams		btn3Param;		 // Параметры кнопоки 3
	TButtonsParams		btn4Param;		 // Параметры кнопоки 4
	TButtonsParams		key1Param;		 // Параметры кнопоки ЩСУ 1
	TButtonsParams		key2Param;		 // Параметры кнопоки ЩСУ 2
	TButtonsParams		key3Param;		 // Параметры кнопоки ЩСУ 3
	TKeyInvert			*KeyLogicSignal; //  тип сигнала по логике нормально розомкнутый/замкнутый - 0/1
} TCommMPU;
//------------------- Глобальные переменные --------------------------------
//------------------- Протатипы функций ------------------------------------
void Comm_LocalControlInit(TCommMPU *);
void Comm_LocalControlUpdate(TCommMPU *);
Uns Comm_LocalKeyUpdate(TCommMPU *);
Uns Comm_LocalButtonUpdate(TCommMPU *);

#endif
