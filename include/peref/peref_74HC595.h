/*
 * peref_74HC595.h
 *
 *  Created on: 15 окт. 2015 г.
 *      Author: PopovIA
 */

#ifndef PEREF_74HC595_H_
#define PEREF_74HC595_H_


//----------- Подключение заголовочных файлов -----------------------------
//--------------------- Константы-------------------------------------------
#define ON_TS_RELE	1
#define OFF_TS_RELE	0
//--------------------- Макросы --------------------------------------------
//-------------------- Структуры -------------------------------------------

// Структура параметров драйвера
typedef void (*TCsFunc)(Byte);

// Данная реализация расчитана на использование 2 микросхем из 3. 16 из 24

typedef union _TShiftReg {
	Uns all;
	struct {

		Uns D_Out1:1;			// 1-1
		Uns D_Out2:1;			// 1-2
     	Uns D_Out3:1;    		// 2-1
     	Uns D_Out4:1;			// 2-2
    	Uns D_Out5:1;			// 3-1
     	Uns D_Out6:1;			// 3 2
     	Uns D_Out7:1;			// 4 1
		Uns D_Out8:1;			// 4 2
		Uns D_Out9:1;			// 5 1
		Uns D_Out10:1;			// 5 2
     	Uns D_Out11:1;    		// 6 1
     	Uns D_Out12:1;			// 6 2
    	Uns D_Out13:1;			// 7 1
     	Uns D_Out14:1;			// 7 2
     	Uns D_Out15:1;			// 8 1
		Uns D_Out16:1;			// 8 2

	} bit;
} TShiftReg;

// Структура для работы с датчиком положения
typedef struct TPeref_74hc595 {

	Byte  		SpiId;					// Идентификатор SPI канала (SPIA, SPIB, ...)
	Uns   		SpiBaud;				// Частота синхроимпульсов
	TShiftReg  	ShiftReg;				// Текущий считанный байт
	Uns			Data;
	Uns			EmptyData;				// Отключение неиспользуемых реле в третьей микросхеме в каскаде

	TCsFunc CsFunc;						// Функция chip select'а

} TPeref_74hc595;

//------------------- Глобальные переменные --------------------------------
//------------------- Протатипы функций ------------------------------------

void Peref_74HC595Init(TPeref_74hc595 *);
void Peref_74HC595Update(TPeref_74hc595 *, TOutputReg);

#endif /* PEREF_74HC595_H_ */
