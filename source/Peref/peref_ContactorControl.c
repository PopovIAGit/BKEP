/*======================================================================
Имя файла:          peref_ContactorControl.c
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		16.04.2014
Описание:
Модуль Управления контакторами
======================================================================*/

#include "peref.h"

Uns ContTest = 0;
Uns Tmp = 0;

void ContactorInit(TContactorControl *p)
{
	p->ContactorGroup = ToPtr(&g_Ram.ramGroupH.ContGroup);

}

void ContactorControlUpdate(TContactorControl *p)
{

//	switch (*p->ContactorGroup)
	switch (ContTest)
	{
		case cgStop:
		{
				SET_CONT_CLOSE(0);
				SET_CONT_OPEN(0);

		/*	if (CONTACTOR_1_STATUS || CONTACTOR_2_STATUS)		// Если сигнал с контактора не уходит за 0,2 секунды после подачи команды стоп
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
			}*/
		}
			break;
		case cgOpen:
		{
			SET_CONT_CLOSE(0);
			SET_CONT_OPEN(1);
		/*
			if (CONTACTOR_2_STATUS || p->ContactorError) 		//Если уже активирован встречный контактор или авария контактора то уходим в стоп
			{
				p->ContactorGroup = cgStop;
				break;
			}
			else
			{
				SET_CONT_CLOSE(0);
				SET_CONT_OPEN(1);

				if (!CONTACTOR_1_STATUS)						// Если обратная связь не пришла или отключилась, ждем и уходим в стоп
				{
					if(++p->ContactorErrTimer >= CONT_ERR_TIME)
					{
						p->ContactorError = true;
						p->ContactorErrTimer = 0;
						p->ContactorGroup = cgStop;
						Tmp++;
					}
				}
				else p->ContactorErrTimer = 0;
			}*/
		}
			break;
		case cgClose:
		{
			SET_CONT_CLOSE(1);
			SET_CONT_OPEN(0);						// Если все хорошо, включаем контактор

			/*if (CONTACTOR_1_STATUS || p->ContactorError) 		//Если уже активирован встречный контактор или авария контакторов то уходим в стоп
			{
				p->ContactorGroup = cgStop;
				break;
			}
			else
			{
				SET_CONT_CLOSE(1);
				SET_CONT_OPEN(0);						// Если все хорошо, включаем контактор

				if (!CONTACTOR_2_STATUS)
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
			*/
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
