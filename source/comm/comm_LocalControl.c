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
	p->btn3Param.timeout = BTN_STOP_ENABLE_TIME;
	p->btn4Param.timeout = BTN_STOP_ENABLE_TIME;

	p->btn1Param.timer = 0;
	p->btn2Param.timer = 0;
	p->btn3Param.timer = 0;
	p->btn4Param.timer = 0;

	p->inputBCP_Data = (&g_Ram.ramGroupC.HallBlock);
}

// Драйвер обработки ручек БКП
Uns Comm_LocalButtonUpdate(TCommMPU *p)
{
	Uns result;

	if (!p->inputBCP_Data->all
			|| ((p->inputBCP_Data->all & hmClose)
					&& (p->inputBCP_Data->all & hmOpen)))
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
		if (!(p->inputBCP_Data->bit.StopDU && p->inputBCP_Data->bit.StopMU))
		{
			if (p->inputBCP_Data->bit.Open)
			{
				if (++p->btn1Param.timer > p->btn1Param.timeout)
				{
					p->btn1Param.timer = 0;
					result |= BTN_OPEN_BIT;
				}
			}
			if (p->inputBCP_Data->bit.Close)
			{
				if (++p->btn2Param.timer > p->btn2Param.timeout)
				{
					p->btn2Param.timer = 0;
					result |= BTN_CLOSE_BIT;
				}
			}
		}
		else if (p->inputBCP_Data->bit.StopDU && p->inputBCP_Data->bit.StopMU)
		{
			if (++p->btn3Param.timer > p->btn3Param.timeout)
			{
				p->btn3Param.timer = 0;
				result |= BTN_STOP;
			}
		}
	}
	return result;
}

