/*======================================================================
Имя файла:          core_ProtectionI2T.c
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		28.04.2014
Описание:
Модуль время-токовой защиты
======================================================================*/

#include "core.h"

void Core_ProtectionI2TInit(TAlarmI2T *p)
{
	//p->minHighCurrent = (Uns)_IQ15int( _IQmpy (_IQ15(*p->NomCurrent), _IQ(K_HIGH_I)) );
	//p->maxLowCurrent  = (Uns)_IQ15int( _IQmpy (_IQ15(*p->NomCurrent), _IQ(K_LOW_I)) );

	p->minHighCurrent = *p->NomCurrent * K_HIGH_I;
	p->maxLowCurrent  = *p->NomCurrent * K_LOW_I;
}

void Core_ProtectionI2TUpdate(TAlarmI2T *p)
{
	register unsigned BitMask;

		BitMask = (1U << p->Cfg.bit.Num);	// Единицу типа Unsigned сдвигаем влево на номер бита в регистре аварий. Получаем маску аварии.

		if (!p->Cfg.bit.Enable)	// Если "разрешение работы"=0, то есть "выкл", то
		{
			*p->Output &= ~BitMask;	// Умножаем "выход" на инвентированную маску, то есть, снимаем АВАРИЮ
			p->Timer = 0;
			return;
		}

		if (!(*p->Output & BitMask))	// Если аварии нет (*p->Output & BitMask дает "0")
		{
		/*	if (*p->InputCurrentMid > p->minHighCurrent )	// если ток превысил величину, при которой время-токовая защита срабатывает моментально
			{
				*p->Output |= BitMask;	// выставляем аварию немедленно
				p->Timer = 0;
			}
			else*/
			if (*p->InputCurrentMid < p->maxLowCurrent)	// если ток ниже величины, при которой время-токовая защита не ведется
			{
				p->Timer = 0;// отключаем накопление
			}
			else
			{
				p->Timeout =  I2T_CONV( *p->InputCurrentMid, *p->NomCurrent, _IQ15(50), _IQ15(1.4), _IQ15(60));
				if (++p->Timer >= (p->Timeout * p->Scale)) //  таймер  достиг времени срабатывания аварии
				{
					*p->Output |= BitMask;	// выставляем АВАРИЮ
					p->Timer = 0;
				}
			}
		}
}
