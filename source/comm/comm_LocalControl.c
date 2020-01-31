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

	if (g_Ram.ramGroupC.HallBlock.bit.StateCalib==1 && (!g_Peref.Position.CancelFlag))
	{
		if (p->inputBCP_Data->bit.Open)
		{
			if (++p->btn1Param.timer > p->btn1Param.timeout*10)
			{
				p->btn1Param.timer = 0;
				if (g_Ram.ramGroupA.Faults.Proc.bit.NoOpen==1) g_Ram.ramGroupD.TaskOpen = trTask; //задать
				else g_Ram.ramGroupD.TaskOpen = trReset; //сбросить
			}
		}
		if (p->inputBCP_Data->bit.Close)
		{
			if (++p->btn2Param.timer > p->btn2Param.timeout*10)
			{
				p->btn2Param.timer = 0;
				if (g_Ram.ramGroupA.Faults.Proc.bit.NoClose==1) g_Ram.ramGroupD.TaskClose = trTask;//задать
				else g_Ram.ramGroupD.TaskClose = trReset; //сбросить
			}
		}

		result = 0;
		return result;
	}

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
		if (!(p->inputBCP_Data->bit.StopDU || p->inputBCP_Data->bit.StopMU))
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
		else if (p->inputBCP_Data->bit.StopDU || p->inputBCP_Data->bit.StopMU)
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

// SDV ЦПА
#define DELAY_TIMEOUT 150 	// 150 = 3 сек на 50 Гц

#define KVO_KVZ_OFF_DEFAULT { \
		&g_Ram.ramGroupA.StateTu.all, \
		&g_Ram.ramGroupC.TimeBtnStopKVOKVZ, \
		DELAY_TIMEOUT, \
		0, 0, 0 }

TKVOKVZoff KvoKvzOff = KVO_KVZ_OFF_DEFAULT;

// Функция формирования флага по размыканию КВО и КВЗ при стопе
// Вход - факт поворота ручки стоп и факт прохождения команды СТОП по ТУ
// Выход - флаг размыкания КВО и КВЗ
Bool OffKVOKVZ_Control (TKVOKVZoff *p, Uns btnStatus)	// 50 Hz
{
	if (g_Ram.ramGroupA.Faults.Dev.bit.LowPower)		// Нет питания? Гасим КВО и КВЗ
	{
		return 1;
	}

	if (g_Comm.bkpNotConnected)							// БКП еще не вышел на связь? Гасим КВО и КВЗ
	{
		return 1;
	}

	if (!g_Ram.ramGroupB.KvoKvzOffOnStop)				// Если разрыв КВО и КВЗ при стопе выключен - дальше не идем
	{
		return 0;
	}

	if (p->delayFlag)							// Если висит флаг задержки
	{											// то просто отсчитваем таймер и игнорируем состояние ручек управления и ТУ
		p->offFlag = false;
		if (p->timer++ >= p->delayTimeout)		// Когда таймер истек просто обнуляем флаг задержки
		{
			p->timer = 0;
			p->delayFlag = false;
		}
	}
	else if (!p->offFlag) 						// Если флаг размыкания КВО и КВЗ снят
	{											// то смотрим на состояние кнопок и ТУ
		if ((btnStatus & BTN_STOP)||	// Если нажата кнопка СТОП на МПУ то выставляем флаг разрывани КВО и КВЗ
			(*p->pTuState & (TU_STOP_OPEN | TU_STOP_CLOSE)) ) // Если пришел СТОП по ТУ - аналогично
		{
			// Выставляем флаг по разрыву КВО и КВЗ только после того, как пускатели разомкнулись
			if (CONTACTOR_1_CONTROL && CONTACTOR_2_CONTROL)
			{
				p->offFlag = true;
			}
		}
	}
	else										// если флаг выключения КВО и КВЗ активен
	{
		if(p->timer++ >= *p->pOnTimeout * 5)	// Отсчитываем таймер.
		{										// по окончанию таймера, проверяем, ушел ли стоп
			if ((btnStatus & BTN_STOP)||
				(*p->pTuState & (TU_STOP_OPEN | TU_STOP_CLOSE)) )
			{									// если СТОП не ушел, то удерживаем таймер
				p->timer = *p->pOnTimeout;
			}
			else								// если СТОП ушел, то снимаем все флаги
			{
				p->timer = 0;					// снимаем флаг разрыва КВО и КВЗ
				p->offFlag = false;				// и выставляем флаг задержки, чтобы выждать паузу, прежде чем опять смотреть на состояние ТУ и Кнопок
				p->delayFlag = true;
			}
		}
	}

	return p->offFlag;
}

