/*
Модуль журналов
*/

#include "stat_LogEvent.h"

void LogEventUpdate(TLogEvent *p)
{
	Uns i, Temp, *Fltr, *FltrPrev, *pBuf = p->Data;

	if (!p->Enable)								
	{
		p->ExecFlag   = false;								
		p->WriteFlag  = false;
		memset(p->FaultsFiltr,     0, sizeof(p->FaultsFiltr));	
		memset(p->FaultsFiltrPrev, 0, sizeof(p->FaultsFiltrPrev));	
		return;						
	}
	
	for (i=0; i < p->FaultsCount; i++)
	{
		Fltr     = &p->FaultsFiltr[i];
		FltrPrev = &p->FaultsFiltrPrev[i];

		Temp   = p->FaultsState[i];
		*Fltr &= Temp;
		Temp  &= ~(*Fltr);
		if (Temp) *Fltr |= Temp;

		if ((*Fltr) ^ (*FltrPrev))
		{
			p->ExecFlag = True;
			//	p->DelayTimer = 0;
			*FltrPrev = *Fltr;
		}
	}
	
	if (p->ExecFlag)													// Выставлен флаг формирования записи
	{
		*pBuf++ = *p->Time;
		*pBuf++ = *p->Date;
		*pBuf++ = *p->Status;
		memcpy(pBuf, p->FaultsState, p->FaultsCount * sizeof(Uns));
		pBuf += p->FaultsCount;
		*pBuf++ = (Uns)(*p->PositionPr);
		*pBuf++ = *p->Torque;
		*pBuf++ = *p->Ur;	*pBuf++ = *p->Us;	*pBuf++ = *p->Ut;
		*pBuf++ = *p->Iu;	*pBuf++ = *p->Iv;	*pBuf++ = *p->Iw;
		*pBuf++ = (Uns)(*p->Temper);
		*pBuf++ = *p->Inputs;
		*pBuf++ = *p->Outputs;
		*pBuf++ = *p->Seconds;

		if (p->FirstEvent)
		{
			p->FirstEvent = false;
			p->ExecFlag   = false;
			p->WriteFlag  = false;
			return;
			*pBuf = 250;
		}
		else
			*pBuf = 0;
		
		p->WriteFlag = True;
		p->ExecFlag  = False;
		p->EventFlag = True;
	}
}


void LogCmdUpdate(TLogCmd *p)
{
	register Uns *pBuf = p->Data;

	if (!p->Enable)								// Журнал выключен
	{
		p->ExecFlag   = false;					// Сбрасываем флаги
		p->WriteFlag  = false;
		p->CmdRegPrev = 0;						// Обнуляем предыдущее значение регистра команд
		
		return;
	}

	if (p->CmdReg)					// Если текущий регистр команд не равен нулю
	{
		p->ExecFlag = true;			// Разрешаем формирование журнала
	}

	if (p->ExecFlag)					// Формириуем журнал
	{
		*pBuf++ = *p->Time;				
		*pBuf++ = *p->Date;				
		*pBuf++ = *p->Seconds;
		*pBuf++ = p->CmdReg;			// Регистр команд
			p->CmdReg = 0;				// Обнулили регистр команд
		*pBuf++ = *p->StatusReg;		// Статусный регистр блока
		*pBuf++ = *p->StatDigOut;		// регистр состояния дискретных выходов
		
		p->WriteFlag = true;
		p->ExecFlag  = false;
	}

}

void LogParamUpdate(TLogParam *p)
{
	register Uns *pBuf = p->Data;

	if (!p->Enable)									// Проверяем выключение журнала
	{
		p->ExecFlag = false;						// Сбрасываем флаги и выходим из функции
		p->WriteFlag = false;
		
		return;
	}

	if (p->ExecFlag)						// По флагу формируем журнал
	{
		*pBuf++ = *p->Time;				
		*pBuf++ = *p->Date;				
		*pBuf++ = (Uns)(*p->Seconds);
		*pBuf++ = p->Addr;					// Адрес параметра
		*pBuf++ = p->NewValue;				// Новое значение параметра
											// Журнал сформирован
		p->WriteFlag = true;				// Выставляем флаг разрешения записи журнала в ПЗУ
		p->ExecFlag = false;				// Сбрасываем флаг формирования журнала
	}
}

//ma LogSIM
void LogSimUpdate(TLogSim *p)
{
	register Uns *pBuf = p->Data;

	if (!p->Enable)									// Проверяем выключение журнала
	{
		p->ExecFlag = false;						// Сбрасываем флаги и выходим из функции
		p->WriteFlag = false;

		return;
	}

	if (p->ExecFlag)						// По флагу формируем журнал
	{
		*pBuf++ = *p->Time;
		*pBuf++ = *p->Date;
		*pBuf++ = (Uns)(*p->Seconds);
		*pBuf++ = p->NewSimID[0];				// Новое значение параметра SIM
		*pBuf++ = p->NewSimID[1];				// Новое значение параметра SIM
		*pBuf++ = p->NewSimID[2];				// Новое значение параметра SIM
		*pBuf++ = p->NewSimID[3];				// Новое значение параметра SIM
		*pBuf++ = p->NewSimID[4];				// Новое значение параметра SIM
		*pBuf++ = p->NewSimID[5];				// Новое значение параметра SIM
		*pBuf++ = p->NewSimID[6];				// Новое значение параметра SIM
		*pBuf++ = p->NewSimID[7];				// Новое значение параметра SIM
		*pBuf++ = p->NewSimID[8];				// Новое значение параметра SIM
		*pBuf++ = p->NewSimID[9];				// Новое значение параметра SIM

											// Журнал сформирован
		p->WriteFlag = true;				// Выставляем флаг разрешения записи журнала в ПЗУ
		p->ExecFlag = false;				// Сбрасываем флаг формирования журнала
	}
}
