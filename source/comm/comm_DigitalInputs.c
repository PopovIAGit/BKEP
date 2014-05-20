/*======================================================================
Имя файла:          DigitalInputs.c
Автор:
Версия файла:
Дата изменения:
Описание:
Модуль обработки входных цифровых сигналов по ТЕЛЕУПРАВЛЕНИЮ. Защита от помех.
======================================================================*/
//#include "comm_DigitalInputs.h"
#include "comm.h"

void DigitalInputInit(TDigitalInput *p, Uns delay, Uns incDiv, Uns decDiv)
{
	p->timeDelay = delay;
	p->deltOn	 = delay/incDiv;	// deltOn/deltOff связаны с TU_TIME из g_Ram
	p->deltOff	 = delay/decDiv;
}

void DigitalInpUpdate(TDigitalInput *p)	// На вход функции приходит сигнал с ТУ
{
	// "Разбираем" входную переменную по битам
	p->inpOpen.inputBit 		= (p->input>>DIN_OPEN_BIT) & 0x0001;
	p->inpClose.inputBit 		= (p->input>>DIN_CLOSE_BIT) & 0x0001;
	p->inpStop.inputBit 		= (p->input>>DIN_STOP_BIT) & 0x0001;
	p->inpMu.inputBit 			= (p->input>>DIN_MU_BIT) & 0x0001;
	p->inpResetAlarm.inputBit 	= (p->input>>DIN_RESETALARM_BIT) & 0x0001;
	p->inpReadyTu.inputBit 		= (p->input>>DIN_READYTU_BIT) & 0x0001;
	p->inpDu.inputBit 			= (p->input>>DIN_DU_BIT) & 0x0001;

	// Вызов функции обработки сигнала для каждого бита
	DigitalInputCalc (&p->inpOpen, 	 		p->deltOn, p->deltOff, p->timeDelay); 	// Результат записывается в Output
	DigitalInputCalc (&p->inpClose, 		p->deltOn, p->deltOff, p->timeDelay);	// в СТОПе всё наоборот: вверх считаем быстро, а вниз медленно
	DigitalInputCalc (&p->inpStop, 	 		p->deltOn, p->deltOff, p->timeDelay);	// В обычном случае - растет медленно, убывает быстро
	DigitalInputCalc (&p->inpMu, 	 		p->deltOn, p->deltOff, p->timeDelay); 	// Результат записывается в Output
	DigitalInputCalc (&p->inpResetAlarm, 	p->deltOn, p->deltOff, p->timeDelay); 	// Результат записывается в Output
	DigitalInputCalc (&p->inpReadyTu, 	 	p->deltOn, p->deltOff, p->timeDelay); 	// Результат записывается в Output
	DigitalInputCalc (&p->inpDu, 	 		p->deltOn, p->deltOff, p->timeDelay); 	// Результат записывается в Output

	// Формируем выход
	p->output  = (p->inpOpen.outputBit  	<<(DIN_OPEN_BIT))&DIN_OPEN_BIT;
	p->output |= (p->inpClose.outputBit 	<<(DIN_CLOSE_BIT))&DIN_CLOSE_BIT;
	p->output |= (p->inpStop.outputBit		<<(DIN_STOP_BIT))&DIN_STOP_BIT;
	p->output |= (p->inpMu.outputBit		<<(DIN_MU_BIT))&DIN_MU_BIT;
	p->output |= (p->inpResetAlarm.outputBit<<(DIN_RESETALARM_BIT))&DIN_RESETALARM_BIT;
	p->output |= (p->inpReadyTu.outputBit	<<(DIN_READYTU_BIT))&DIN_READYTU_BIT;
	p->output |= (p->inpDu.outputBit		<<(DIN_DU_BIT))&DIN_DU_BIT;

}

void DigitalInputCalc(TDigitalInputBit *p, Uns Increment, Uns Decrement, Uns TimeDelay)
{
	if (p->inputBit)   // Если сигнал активен
	{
		if (p->Timer < TimeDelay)  // Если таймер не достиг предела срабатывания
			p->Timer += Increment; // Таймер увеличивается на Increment
		else // Таймер достиг величины срабатывания
		{
			if (!p->outputBit) p->outputBit = 1;	// Выход - в "1"
			p->Timer = TimeDelay;
		}
	}
	else			// Если сигнал не активен
	{
		if (p->Timer > 0) // Если таймер больше нуля
			p->Timer -=  Decrement;	// Таймер уменьшается на Decrement
		else // Таймер упал до нуля
		{
			if (p->outputBit) p->outputBit = 0;	// Выход - в "0"
			p->Timer = 0;
		}
	}
}
