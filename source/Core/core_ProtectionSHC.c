/*======================================================================
Имя файла:          core_ProtectionI2T.c
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		28.04.2014
Описание:
Модуль защиты от короткого замыкания
======================================================================*/

#include "core.h"

void Core_ProtecionSHC_Update(TAlarmSHC *p)
{
	register Uns BitMask;

	BitMask = (1UL << p->Cfg.bit.Num); // Единицу типа Unsigned сдвигаем влево на номер бита в регистре аварий. Получаем маску аварии.

	if (!p->Cfg.bit.Enable)
		return;

	if (!(*p->Output & BitMask))
	{
		if ((*p->InputCurrent > *p->HighCurrentLevel) || (*p->InputCurrent < *p->LowCurrentLevel))
		{
			*p->Output |= BitMask;
		}
	}
}
