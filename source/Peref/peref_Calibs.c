/*======================================================================
Имя файла:          peref_Calibs.c
Автор:              Попов И.А.
Версия файла:       01.00
Дата изменения:		14/04/14
======================================================================*/

#include "peref.h"

#define MpyDivK(V1,V2,K,Prec)	(int16)((int32)((float32)(V1) * (V2) / (K))  >> (Prec))
#define DivKQ1(V1,V2,K,Prec) 	(int16)((int32)((float32)(V1) * (K)  / (V2)) >> (Prec))

LgUns TmpRevolution = 0;
LgUns DataTmp;

void Peref_CalibInit(TPerefPosition *p)
{
	p->ResetFlag 	= false;
	p->CancelFlag 	= false;
	p->GearRatio 	= g_Ram.ramGroupC.GearRatio;
	p->Zone 		= 0;
	p->RevMax 		= REV_MAX;
	p->LinePos		= 0;
	p->FullStep		= 0;
	p->BasePos		= 0;
	p->CycleData	= 0;
	p->MuffTime		= MUFF_CLB_TIME;
	p->MuffTimer	= 0;
	p->Reserved		= 0;
	p->GearInv		= CalcClbGearInv(&g_Peref.Position);
	p->FstepInv		= 0;
	p->PosSensPow	= &g_Ram.ramGroupC.PosSensPow;
	p->PositionAcc	= &g_Ram.ramGroupH.PositionAccTemp;
	p->RodType		= ToPtr(&g_Ram.ramGroupB.RodType);
	p->AbsPosition	= ToPtr(&g_Ram.ramGroupH.Position);	// ??? Заменить на данные с энкодера
	p->PositionPr	= &g_Ram.ramGroupA.PositionPr;
	p->FullWay		= &g_Ram.ramGroupA.FullWay;
	p->CurWay		= &g_Ram.ramGroupA.CurWay;
	p->CycleCnt		= &g_Ram.ramGroupH.CycleCnt;
	p->Command		= ToPtr(&g_Ram.ramGroupD.TaskClose);
	p->Indication	= ToPtr(&g_Ram.ramGroupH.CalibState);

}



Uint32 CalcClbGearInv(TPerefPosition *p)
{
	#if defined(__TMS320C28X__)
	return 0UL;
	#else
	return ((16384000UL >> *p->PosSensPow) / (LgUns)p->GearRatio + 1);
	#endif
}

Uint32 CalcClbAbsRev(TPerefPosition *p, Uint16 GearRev)
{
	return MpyDivK(GearRev, p->GearRatio, 1000, *p->PosSensPow);
}

int16 CalcClbGearRev(TPerefPosition *p, int32 AbsRev)
{
	return DivKQ1(AbsRev, p->GearRatio, 1000, *p->PosSensPow);
}

Uint32 CalcClbPercentToAbs(TPerefPosition *p, Uint16 Percent)
{
	return MpyDivK(p->FullStep, Percent, 1000, 0);
}


void Peref_Calibration(TPerefPosition *p)
{
	LgUns Data;

	// Задание/сброс Закрыто
	if(p->Command->TaskClose != 0)
	{
		switch(p->Command->TaskClose)
		{
			case 1: // Выставляем
				if(!CheckStatus(p, CLB_CLOSE)) break;
				p->Indication->ClosePos = *p->AbsPosition;
				p->Indication->Status |= CLB_CLOSE;
				break;
			case 2: // Снимаем
				if (!CheckStatus(p, 0)) break;
				p->Indication->ClosePos = 0;
				p->Indication->Status &= CLB_OPEN;
				break;
		}
		p->Command->TaskClose = 0;
	}

	//Задание/сброс Открыто
	if(p->Command->TaskOpen != 0)
	{
		switch(p->Command->TaskOpen)
		{
		case 1:	//Выставляем
			if(!CheckStatus(p, CLB_OPEN)) break;
			p->Indication->OpenPos = *p->AbsPosition;
			p->Indication->Status |= CLB_OPEN;
			break;
		case 2: // Снимаем
			if (!CheckStatus(p, 0)) break;
			p->Indication->OpenPos = 0;
		    p->Indication->Status &= CLB_CLOSE;
			break;
		}
		p->Command->TaskOpen = 0;
	}

	// задание количества оборотов на Открытие
	if (p->Command->RevOpen > 0)
	{
		if (CheckStatus(p, CLB_FLAG))
		{
			p->Indication->ClosePos = *p->AbsPosition;
			Data = CalcClbAbsRev(p, p->Command->RevOpen);
		//	Data = (((LgUns)p->GearRatio * (LgUns)p->Command->RevOpen) << *p->PosSensPow)/10;
			if (*p->RodType) Data = p->Indication->ClosePos - Data;
			else Data = p->Indication->ClosePos + Data;
			p->Indication->OpenPos = Data & p->RevMax;
			p->Indication->Status = CLB_FLAG;
		}
		p->Command->RevOpen = 0;
	}

	// задание количества оборотов на Закрытие
	if (p->Command->RevClose > 0)
	{
		if (CheckStatus(p, CLB_FLAG))
		{
			p->Indication->OpenPos = *p->AbsPosition;
			Data = CalcClbAbsRev(p, p->Command->RevClose);
			//Data = (((LgUns)p->GearRatio * (LgUns)p->Command->RevClose) << *p->PosSensPow)/10;
			if (*p->RodType) Data = p->Indication->OpenPos + Data;
			else Data = p->Indication->OpenPos - Data;
			p->Indication->ClosePos = Data & p->RevMax;
			p->Indication->Status = CLB_FLAG;
		}
		p->Command->RevClose = 0;
	}
}

void Peref_CalibUpdate(TPerefPosition *p)
{
	ClbIndication *Indic = p->Indication;
	LgUns Position, ClosePos, OpenPos;

	if (*p->RodType)									// если выбран обратный тип штока
	{
		Position = p->RevMax - *p->AbsPosition + 1;		// текущая позиция ровна максимальному количеству
		ClosePos = p->RevMax - Indic->ClosePos + 1;		// положение закрыто  = макс пололжение - запомненное положение закрыто
		OpenPos  = p->RevMax - Indic->OpenPos  + 1;		// аналогтчно
	}
	else	// если выбран обычный шток
	{
		Position = *p->AbsPosition;			// запоминаем текущее положение
		ClosePos = Indic->ClosePos;			// запоминаем положение закрыто
		OpenPos  = Indic->OpenPos;			// запоминаем положение открыто
	}
	p->Zone = 0;

	if(Indic->Status == CLB_FLAG)
	{
		if (!p->FullStep)//Если путь еще не расчитан
		{
			// Полный ход в метках энкодера с ограничением на макс. количество меток энкодера
			p->FullStep = (OpenPos - ClosePos) & p->RevMax;
			*p->FullWay = (Uint16)CalcClbGearRev(p, (int32)p->FullStep);
			//*p->FullWay = (Uns)(((p->FullStep * 10) >> *p->PosSensPow)/ p->GearRatio);
			if((*p->FullWay >> 1) <= *p->PositionAcc)
			{
				p->FullStep = 0;
			}
			if(!p->FullStep)
			{
				p->Zone = CLB_FLAG;
			}
			else
			{
				p->BasePos = (OpenPos + ClosePos) >> 1;
				if(OpenPos >= ClosePos)
				{
					p->BasePos = p->BasePos + (p->RevMax >> 1);
				}
				p->BasePos = p->BasePos & p->RevMax;
			}
		}
		else
		{
			p->LinePos = Position - ClosePos;
			if (ClosePos > p->BasePos)
			{
				if (Position < p->BasePos)
					p->LinePos = p->LinePos + p->RevMax + 1;
			}
			else
			{
				if (Position > p->BasePos)
					p->LinePos = p->LinePos - p->RevMax - 1;
			}

			*p->CurWay = CalcClbGearRev(p, p->LinePos);
			//*p->CurWay = ((p->LinePos * 10) >> *p->PosSensPow)/ p->GearRatio;
			if (*p->CurWay <= (Int)*p->PositionAcc) p->Zone |= CLB_CLOSE;
			if (*p->CurWay >= ((Int)*p->FullWay - (Int)*p->PositionAcc)) p->Zone |= CLB_OPEN;

			*p->PositionPr = DivKQ1(p->LinePos, p->FullStep, 1000, 0);
			//*p->PositionPr = (p->LinePos*1000UL)/p->FullStep;
			if ((p->Zone & CLB_CLOSE) && (*p->PositionPr > 0))    *p->PositionPr = 0;
			if ((p->Zone & CLB_OPEN)  && (*p->PositionPr < 1000)) *p->PositionPr = 1000;
		}
	}
	else
	{
		*p->PositionPr 	= 9999;
		*p->FullWay 	= 9999;
		 p->FullStep 	= 0;

		if (!Indic->Status) {*p->CurWay = 9999; return;}
		else if (Indic->Status & CLB_CLOSE) p->LinePos = (Position - ClosePos) & p->RevMax;
		else if (Indic->Status & CLB_OPEN)  p->LinePos = (OpenPos - Position)  & p->RevMax;

		*p->CurWay = CalcClbGearRev(p, p->LinePos);
		//*p->CurWay = ((p->LinePos * 10) >> *p->PosSensPow)/ p->GearRatio;
		if (*p->CurWay <= (Int)*p->PositionAcc) p->Zone |= Indic->Status;
	}
}


static Byte CheckStatus(TPerefPosition *p, Uns Condition)
{
	if ((p->Indication->Status & Condition) || p->ResetFlag)
	{
		p->CancelFlag = TRUE;
		return 0;
	}
	return 1;
}


void CalcClbCycle(TPerefPosition *p)
{
	if (!p->Indication->Status) {p->CycleData = 0; return;}

	if (p->Zone != 0)
		{
			if (!p->CycleData && p->Zone != 0x3) p->CycleData = p->Zone;

			if (p->Zone == (p->CycleData & CLB_FLAG))
			{
				if (p->CycleData & 0x40) return;
				p->CycleData |= 0x40;
				if (p->CycleData & 0x80)
				{
					*p->CycleCnt = *p->CycleCnt + 1;
					p->CycleData &= ~0x80;
				}
			}
			else
			{
				if (p->CycleData & 0x40)
				{
					p->CycleData &= ~0x40;
					p->CycleData |=  0x80;
				}
			}
		}
}


// Расчет скорости 50 Гц
void Peref_SpeedCalc(TPerefPosition *p)
{
		static   Uns   Timer = 0;
		static   LgUns RevPrev = -1UL;
		register LgInt Delta;

		Uns	Prd5Hz = 5,
				SP_CALC_TOUT = Prd50HZ / Prd5Hz;

	if (++Timer >= (Uns) SP_CALC_TOUT)
	{
		if (RevPrev == -1UL) // 1 итерация
			Delta = 0; // дельту в 0
		else
		{
			Delta = *p->AbsPosition - RevPrev; // дельта = новое значение - предидущие p->AbsPosition

			if (Delta > SP_POS_LIMIT)
				Delta -= (REV_MAX + 1); // если перешли через ноль сдвигаем на половину
			if (Delta < -SP_POS_LIMIT)
				Delta += (REV_MAX + 1);
		}
			RevPrev = *p->AbsPosition; // запоминаем предидущие значение ???  p->AbsPosition

		p->speedMPS = abs(Delta);

		p->speedRPM = (p->speedMPS * Prd5Hz * 60) >> *p->PosSensPow;

		Timer = 0;
	}

}
