/*======================================================================
Имя файла:          comm_DigitalInputs.h
Автор:
Версия файла:
Дата изменения:
Описание:
Вход - Tu_STAT; Выход - tu_status
======================================================================*/
#ifndef DIGITAL_INPUT_
#define DIGITAL_INPUT_

#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ENABLE_DELAY 8000	// Величина, при достижении которой считается, что сигнал пришел
//#define ICREMENT_DIV 25		// Относительная велечина прироста таймера
//#define DECREMENT_DIV 10	// Относительная велечина уменьшения таймера
#define LO_DELTA 25		// Относительная велечина прироста/уменьшения таймера
#define HI_DELTA 40		// Относительная велечина прироста/уменьшения таймера

typedef struct 
{
	unsigned	inputBit;			// входной сигнал 
	unsigned	outputBit;			// выходной сигнал 
	int    		Timer;				// Таймер
	unsigned	signalType;			// тип сигнала (Инверсный/не инверсный)

} TDigitalInputBit;

typedef struct
{
	Uns				 	input;			// Вход: подготовленное состояние дискретного входа в соответствии с уровнем и инрерсией
	Uns					output;			// Выход: 
	TDigitalInputBit	inpOpen;		// Открыть
	TDigitalInputBit	inpClose;		// Закрыть
	TDigitalInputBit	inpStop;		// Стоп
	TDigitalInputBit	inpMu;			//
	TDigitalInputBit	inpResetAlarm;	//
	TDigitalInputBit	inpReadyTu;		//
	TDigitalInputBit	inpDu;			//
	Uns 				timeDelay;		// Величина "срабатывания" сигнала
	Uns					deltOn;			// Инкремент 
	Uns 				deltOff;		// Декремент
	Uns				*TypeLogicSignal;// тип сигнала по логике нормально розомкнутый/замкнутый - 0/1
} TDigitalInput;

void DigitalInputInit (TDigitalInput *, Uns, Uns, Uns);
void DigitalInpUpdate(TDigitalInput *);
void DigitalInputCalc (TDigitalInputBit *, Uns, Uns, Uns);

#ifdef __cplusplus
}
#endif // extern "C"

#endif	//DIGITAL_INPUT_

