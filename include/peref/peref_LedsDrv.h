/*======================================================================
Имя файла:          peref_LedsDrv.h
Автор:
Версия файла:
Дата изменения:
Описание:
Модуль сигнализации индикации
======================================================================*/

#ifndef PEREF_LEDS_
#define PEREF_LEDS_

#include "config.h"

// Лампочка




#define LED_OPEN_MASK		BIT0	// Индикатор "ОТКРЫТО/ОТКРЫВАЕТСЯ". 1 - "ОТКРЫТО", 1,0,1,0 - "ОТКРЫВАЕТСЯ"
#define LED_MUFTA_MASK		BIT1
#define LED_DEFECT_MASK		BIT2	// Индикатор "Режим программирования". 0 - режим прогр. выкл., 1 - режим прогр. вкл.
#define LED_FAULT_MASK		BIT3	// Индикатор "АВАРИЯ". 1 - авария на блоке, 0 - нет аварий
#define LED_CLOSE_MASK		BIT4	// Индикатор "ОТКРЫТО/ОТКРЫВАЕТСЯ". 1 - "ЗАКРЫТО", 1,0,1,0 - "ЗАКРЫВАЕТСЯ"
#define LED_BLUETOOTH_MASK	BIT5	// Индикатор "ОТКРЫТО/ОТКРЫВАЕТСЯ". 1 - "ОТКРЫТО", 1,0,1,0 - "ОТКРЫВАЕТСЯ"
#define LED_CONNECT_MASK	BIT6	// Индикатор "МУ/ДУ". 0-МУ, 1-ДУ

			   
#define LEDS_WITHOUT_CNTR	0x1F	// Индикатор работы процессора

// Маска для формирования общего с display регистра
#define LEDS_MASK	0xE0	// 11100000	

typedef struct 
{
	Byte 	isBlink;		// 0 - не мигать, 1 - мигать
	Uns 	blinkPRD;		// Частота мигания (Гц)
	Uns		blinkTimer;		// Таймер мигания
	Uns		timeOfBlink;	// Время, при достижении которого светодиод "мигает" (зависит от blinkPRD)
	Uns		status;			// текущий статус работы светодиода
} TLedParams, *pLedParams;

typedef union {
	Uns all;
  	struct {
		Uns Open:1;			// 0	Светодиод "Открыто"
		Uns Mufta:1;		// 1	Светодиод "Муфта"
		Uns Defect:1;		// 2    Светодиод "Неисправность"
		Uns Fault:1;		// 3	Светодиод "Авария"
		Uns Close:1;		// 4	Светодиод "Закрыто"
		Uns Bluetooth:1;	// 5	Светодиод "Работа Bluetooth"
		Uns	Connect:1;		// 6 	Светодиод "Сеть"
     	Uns rsvd:9; 		// 7-15  Резерв
  } bit;
} TLedReg;

typedef struct
{
	TLedParams 	ledCntr;		// Светодиод на тыльной стороне платы
	TLedParams 	ledOpen;		// Светодиод "Открыто"
	TLedParams 	ledFault;		// Светодиод "Авария"
	TLedParams 	ledDefect;		// Светодиод "Режим программирования"
	TLedParams 	ledClose;		// Светодиод "Закрыто"
	TLedParams 	ledBluetooth;		// Светодиод "МУ/ДУ"
	TLedParams	ledConnect;		// Светодиод "Спящий режим"
	TLedParams	ledMufta;
	TLedReg		leds;
	Uns 		*pStatus;		// Указатель на статус ядра
	Uns			leds_displayReg;
	Byte		spiId;          // Идентификатор SPI-порта (0-SPIA, 1-SPIB, ...)
	Uns			spiBaud;        // Частота синхроимпульсов (расчитанная для конктретного чипа)
	Byte		CSAddr;         // Адрес для чип-селекта
} TLeds, *pLeds;


void Peref_LedsInit(pLeds p, Uns);
void Peref_LedsUpdate(pLeds p);

#endif

