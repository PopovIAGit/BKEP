/*======================================================================
Имя файла:          core_ProtectionsAlarm.c
Автор:
Версия файла:		1.00
Дата изменения:		25.04.2014
Описание:
модуль реализации алгоритма защит
======================================================================*/

#include "core.h"

void Core_ProtectionsAlarmInit(TAlarmElem *p)
{

}


void Core_ProtectionsAlarmUpdate(TAlarmElem *p)
{
	register Uns BitMask, Tmp;

	BitMask = (1UL << p->Cfg.bit.Num); // Единицу типа Unsigned сдвигаем влево на номер бита в регистре аварий. Получаем маску аварии.

	if(!p->Cfg.bit.Enable)	// Если защита выключена
	{
		if (p->Cfg.bit.CanBeReseted)	*p->Output &= ~BitMask;

			 p->Timer = 0;
			 p->Signal = *p->Input;
		return;
	}

	if(p->Cfg.bit.CanBeReseted == CAN_BE_MUFTA)
	{
		if ( abs(*p->Input - p->Signal) >= (Uns)*p->EnableLevel)
		{
			p->Timer = 0;
			p->Signal = *p->Input; //ToLong(p->Input);
		}
		else
		{
			if (p->Timer < (*p->Timeout * p->Scale))
				p->Timer++;
			else
				*p->Output |= BitMask;
		}
	   return;
	}

	if (!(*p->Output & BitMask))	// Если аварии нет (*p->Output & BitMask дает "0")
		{
			// Смотрим на Level("активный уровень").
			// Он показывает, каким является предельное допустимое значение сигнала, максимальным или минимальным
			switch(p->Cfg.bit.Level)
			{
				// Если Level = 0, то p->Level - нижний допустимый предел значения сигнала
				case 0: Tmp = (*p->Input <= *p->EnableLevel); break;	// проверка, упал ли ниже входной сигнал минимального предела
				// Если Level = 1, то p->Level - верхний допустимый предел сигнала
				case 1: Tmp = (*p->Input >= *p->EnableLevel); break;	// проверка, превысил ли входной сигнал максимальный предел
			}
			if (!Tmp)	// Если все в норме, т.е. сигнал не привысил или упал ниже пределов
			{
				if (p->Timer > 0) p->Timer--;	// если таймер не равен "0", уменьшаем таймер
			}
			else 	// иначе - сигнал превысил или упал ниже допустимого предела
			if (++p->Timer >= (*p->Timeout * p->Scale))	// Увеличиваем таймер, и если таймер достиг времени срабатывания аварии
			{
				*p->Output |= BitMask;	// выставляем АВАРИЮ
				p->Timer = 0;
			}
		}
	else // если авария уже есть (*p->Output & BitMask дает "1")
		if (p->Cfg.bit.CanBeReseted == CAN_BE_RESETED) // Если "режим работы"=1, то есть аварию можно "снять",
		{
			// Смотрим на Level("активный уровень").
			// Он показывает, каким является предельное допустимое значение сигнала, максимальным или минимальным
			switch(p->Cfg.bit.Level)
			{
				// Если Level = 0, то p->Level - нижний допустимый предел значения сигнала
				case 0: Tmp = (*p->Input >= (*p->DisableLevel + (Int)p->Cfg.bit.Hyst)); break;// проверка, превысил ли входной сигнал минимальный предел
				// Если Level = 1, то p->Level - верхний допустимый предел сигнала
				case 1: Tmp = (*p->Input <= (*p->DisableLevel - (Int)p->Cfg.bit.Hyst)); break;// проверка, упал ли ниже входной сигнал максимального предела
			}
			if (Tmp)	// Если входное значение восстановилось до номального значения
			{
				 *p->Output &= ~BitMask;	// снимаем АВАРИЮ
			}
		}
}
