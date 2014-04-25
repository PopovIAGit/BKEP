/*======================================================================
Имя файла:          peref_ContactorControl.c
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		16.04.2014
Описание:
Модуль Управления контакторами
======================================================================*/

#include "peref.h"


void ContactorInit(TContactorControl *p)
{
	p->ContactorGroup = ToPtr(&g_Ram.ramGroupH.ContGroup);
}

void ContactorControlUpdate(TContactorControl *p)
{
	switch (*p->ContactorGroup)
	{
		case cgStop:
		{
			CONTACTOR_1_CONTROL = 0;
			CONTACTOR_2_CONTROL = 0;

			if (CONTACTOR_1_ENABLE || CONTACTOR_2_ENABLE)		// Если сигнал с контактора не уходит за 0,2 секунды после подачи команды стоп
			{
				if (++p->ContactorErrTimer >= CONT_ERR_TIME)
				{
					p->ContactorErrTimer = CONT_ERR_TIME;		// Остаемся в данном состоянии
					p->ContactorError = true;					// Выставляем аварию
				}
			}
			else
			{
				p->ContactorErrTimer = 0;						// Если сигналы с контакторов снялись то сбрасываем таймер, авария при этом не уходит
			}
		}
			break;
		case cgOpen:
		{
			if (CONTACTOR_2_ENABLE || p->ContactorError) 		//Если уже активирован встречный контактор или авария контактора то уходим в стоп
			{
				p->ContactorGroup = cgStop;
				break;
			}
			else
			{
				CONTACTOR_1_CONTROL = 1;						// Если все хорошо, включаем контактор
				CONTACTOR_2_CONTROL = 0;

				if (!CONTACTOR_1_ENABLE)						// Если обратная связь не пришла или отключилась, ждем и уходим в стоп
				{
					if(++p->ContactorErrTimer >= CONT_ERR_TIME)
					{
						p->ContactorError = true;
						p->ContactorErrTimer = 0;
						p->ContactorGroup = cgStop;
					}
				}
				else p->ContactorErrTimer = 0;
			}
		}
			break;
		case cgClose:
		{
			if (CONTACTOR_1_ENABLE || p->ContactorError) 		//Если уже активирован встречный контактор или авария контакторов то уходим в стоп
			{
				p->ContactorGroup = cgStop;
				break;
			}
			else
			{
				CONTACTOR_1_CONTROL = 0;
				CONTACTOR_2_CONTROL = 1;						// Если все хорошо, включаем контактор

				if (!CONTACTOR_2_ENABLE)
				{
					if(++p->ContactorErrTimer >= CONT_ERR_TIME)
					{
						p->ContactorError = true;
						p->ContactorErrTimer = 0;
						p->ContactorGroup = cgStop;
					}
				}
				else p->ContactorErrTimer = 0;
			}

		}
			break;
	}
}

// Сброс аварии контактора с обнулением таймера
void ContactorErrClear(TContactorControl *p)
{
	p->ContactorErrTimer = 0;
	p->ContactorError = false;
}
