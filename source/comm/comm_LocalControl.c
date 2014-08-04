/*======================================================================
Имя файла:          comm_LocalControl.c
Автор:
Версия файла:      	1.0
Дата изменения:		12.05.2014
Описание:
Модуль обработки сигналов с кнопок местного пульта управления,
преобразование этих сигналов в соответствующие команды
======================================================================*/


#include "comm.h"
#include "core_Menu.h"
#include "g_Ram.h"

void Comm_LocalControlInit(TCommMPU *p)
{
	p->btn1Param.timeout = BTN_ENABLE_TIME;
	p->btn2Param.timeout = BTN_ENABLE_TIME;
	p->btn3Param.timeout = BTN_ENABLE_TIME;
	p->btn4Param.timeout = BTN_ENABLE_TIME;

	p->key1Param.timeout = BTN_ENABLE_TIME;
	p->key2Param.timeout = BTN_ENABLE_TIME;
	p->key3Param.timeout = BTN_ENABLE_TIME;

	p->btn1Param.timer = 0;
	p->btn2Param.timer = 0;
	p->btn3Param.timer = 0;
	p->btn4Param.timer = 0;

	p->key1Param.timer = 0;
	p->key2Param.timer = 0;
	p->key3Param.timer = 0;

	p->KeyLogicSignal = ToPtr(&g_Ram.ramGroupB.KeyInvert);
	p->inputBCP_Data  = ToPtr(&g_Ram.ramGroupC.HallBlock);
}

// Драйвер обработки кнопок ЩСУ
Uns Comm_LocalKeyUpdate(TCommMPU *p)
{
	static Uns result;

	if(!KEY_1 && KEY_2 && !KEY_3)
	{
		p->key1Param.timer = 0;
		p->key2Param.timer = 0;
		p->key3Param.timer = 0;

		result = 0;
		return result;
	}

	// Close
	if (!KEY_1 ^ p->KeyLogicSignal->bit.Close)
		p->key1Param.timer = 0;
	else if (p->key1Param.timer < p->key1Param.timeout)
		p->key1Param.timer++;
	else
		result = KEY_CLOSE;

	// Open
	if (!KEY_3 ^ p->KeyLogicSignal->bit.Open)
		p->key2Param.timer = 0;
	else if (p->key2Param.timer < p->key2Param.timeout)
		p->key2Param.timer++;
	else
		result = KEY_OPEN;

	// Stop
	if (!KEY_2 ^ p->KeyLogicSignal->bit.Stop)
		p->key3Param.timer = 0;
	else if (p->key3Param.timer < p->key3Param.timeout)
		p->key3Param.timer++;
	else
		result = KEY_STOP;

	return result;
}

// Драйвер обработки ручек БКП
Uns Comm_LocalButtonUpdate(TCommMPU *p)
{
	static Uns result;

	if (!p->inputBCP_Data->all || ((p->inputBCP_Data->all & hmClose) && (p->inputBCP_Data->all & hmOpen)))
	{
		p->btn1Param.timer = 0;
		p->btn2Param.timer = 0;
		p->btn3Param.timer = 0;
		p->btn4Param.timer = 0;

		result = 0;
		return result;
	}
	else
	{
		switch(p->inputBCP_Data->all)
		{
		case (1 << hmOpen):

			if (++p->btn1Param.timer > p->btn1Param.timeout)
			{
				p->btn1Param.timer = 0;
				result |=  BTN_OPEN_BIT;
			}

			break;
		case (1 << hmClose):

			if (++p->btn2Param.timer > p->btn2Param.timeout)
			{
				p->btn2Param.timer = 0;
				result |= BTN_CLOSE_BIT;
			}

			break;
		case (1 << hmStopMu):

			if (++p->btn4Param.timer > p->btn4Param.timeout)
			{
				p->btn4Param.timer = 0;
				result |= BTN_STOPMU_BIT;
			}
			break;
		case (1 << hmStopDu):

			if (++p->btn3Param.timer > p->btn3Param.timeout)
			{
				p->btn3Param.timer = 0;
				result |= BTN_STOPDU_BIT;
			}

			break;

		}
	}
	return result;
}

