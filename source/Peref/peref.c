/*======================================================================
Имя файла:          peref.c
Автор:
Версия файла:
Дата изменения:
Описание:
Модуль обработки переферии
======================================================================*/

#include "peref.h"
#include "g_Ram.h"
#include "comm.h"

#define SetCs()		SetCs2()
#define ClrCs()		ClrCs2()

//#define OFFSET_ISO 33580L
//#define GAIN_ISO 10000 

LgUns OFFSET_ISO = 32450;
Uns		GAIN_ISO = 10000;

// Линейная формула преобразования кодов с АЦП по датчику температуры
// в соответствующее сопротивление R(x) = b*x + c, где x - то, что пришло с АЦП,
// R - сопротивление, b и c - коэффициенты, которые задаются ниже:
#define koef_B		_IQ15(-9.6774)
#define koef_C		_IQ15(6600.0)
#define ADC_to_R(inpADC)	_IQ15int(_IQ15mpy(koef_B, _IQ15(inpADC) ) + koef_C) 	// Для случая 3 датчика

#define koef_B1		_IQ15(-10.204)
#define koef_C1		_IQ15(6928.57)
#define ADC_to_R1(inpADC1)	_IQ15int(_IQ15mpy(koef_B1, _IQ15(inpADC1) ) + koef_C1)	// Для случая 2 датчика

TPeref	g_Peref;
Uns		loadAngleSwitch = 0;

//---------------------------------------------------
void Peref_Init(TPeref *p)
{
	//g_Ram.ramGroup.POSITION 	= 0;
	//g_Ram.ramGroup.TEMP_SENS 	= 0;
	//g_Ram.ramGroup.SENSORS.all 	= 0;
	//g_Ram.ramGroup.COMMAND.all	= 0;

	// ----Сигнализация лампочками-----------------------
	//Peref_LedsInit(&p->leds, Prd10HZ);
	//ADT75_Init(&p->tempSensor);
	//обработка энкодера
	//Peref_EncoderInit(&p->position);
}
//---------------------------------------------------
void Peref_18kHzCalc(TPeref *p) // 18 кГц
{

}

//---------------------------------------------------
// Обработка фильтров
void Peref_50HzCalc(TPeref *p)	// 50 Гц
{
	//Peref_TenControl(p);
}

void Peref_10HzCalc(TPeref *p)	// 10 Гц
{

}

//---------------------------------------------------
void Peref_TenInit(TTen *p, Uns freq, Uns tenTime, Uns tenRate) 
{
	//p->timerTen = 0;
	//p->timerTenTimeOut = freq*tenTime;
	//p->tenEnableTime = (Uns)(p->timerTenTimeOut * tenRate)/100;
}
//---------------------------------------------------
void Peref_TenControl(void) // 10 Гц
{
	// условие для аварийного отключения или включения ТЭНа надо дописать
	//if (g_Ram.ramGroup.COMMAND.bit.ten==1) OFF_TEN=1;
	//else if (g_Ram.ramGroup.COMMAND.bit.ten==0) OFF_TEN=0;

	/*if (!g_Ram.ramGroup.TEN_ENABLE)		// Если ТЕН запрещен
	{
		if (!g_Comm.digitInterface.Outputs.bit.Dout5)		// Если тен включен
			g_Comm.digitInterface.Outputs.bit.Dout5 = 1; 	// то выключаем его
		if (!shiftReg2.data.bit.ten)		// Если тен включен
			shiftReg2.data.bit.ten = 1; 	// то выключаем его
		if (!shiftReg2.data.bit.ten_bubi)	// и выходим из функции
			shiftReg2.data.bit.ten_bubi = 1;
		return;								
	}

	if (g_Peref.tempSensor.error == true)
	{
		g_Comm.digitInterface.Outputs.bit.Dout5 = 0;
		shiftReg2.data.bit.ten = 0;
		shiftReg2.data.bit.ten_bubi = 0;
		return;
	}
	// Пока что решили не вычислять интенсивность ТЕНА
	//Peref_TenRateCalc(p);					// Вычисляем интенсивность ТЕН

	if (g_Ram.ramGroupA.TEMPER_PU <= g_Ram.ramGroupC.TEN_ON_LEVEL)		// Если ТЕН включен
	{
		g_Comm.digitInterface.Outputs.bit.Dout5 = 0;
		shiftReg2.data.bit.ten = 0;
		shiftReg2.data.bit.ten_bubi = 0;
	}
	else if (g_Ram.ramGroupA.TEMPER_PU >= g_Ram.ramGroupC.TEN_OFF_LEVEL)									// иначе (Если ТЕН выключен)
	{
		if (!g_Comm.digitInterface.Outputs.bit.Dout5)		// Если тен включен
			g_Comm.digitInterface.Outputs.bit.Dout5 = 1; 	// то выключаем его
		if (!shiftReg2.data.bit.ten)						// Если тен включен
			shiftReg2.data.bit.ten = 1; 					// то выключаем его
		if (!shiftReg2.data.bit.ten_bubi)					// Если тен включен
			shiftReg2.data.bit.ten_bubi = 1; 				// то выключаем его
	}*/
}
//---------------------------------------------------
void Peref_TenRateCalc(TTen *p)
{
	/*Int deltaT;
	static Uns tenRate, prevTenRate;

	deltaT = g_Ram.ramGroup.TEN_OFF_LEVEL - g_Ram.ramGroup.TEMPER_PU;
	
	if (deltaT <= 1)		// температура ниже 4 град
		tenRate = 30;
	else if (deltaT <= 5)	// температура ниже 0 град
		tenRate = 50;
	else if (deltaT <= 10)	// температура ниже -5 град
		tenRate = 80;
	else 					// температура ниже -10 град
		tenRate = 100;		

	if (tenRate != prevTenRate)
	{
		Peref_TenInit(p, Prd10HZ, g_Ram.ramGroup.TEN_PERIOD, tenRate);
		prevTenRate = tenRate;
	}*/
} 
//---------------------------------------------------
