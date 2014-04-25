/*======================================================================
Имя файла:          add_Monitor.c
Автор:				Попов И.А.
Версия файла:		1.0
Дата изменения:		10.04.14
Применяемость:
Описание:			Графический монитор реального времени
======================================================================*/

#include "csl\csl.h"
#include "config.h"
#include "peref.h"
#include "comm.h"

TMonitor Monitor;		// 	Простой монитор
TDataLog Dlog;			//	Dlog монитор

Int DlogCh1 = 0;
Int DlogCh2 = 0;
Uns MonSelect = 1;

void MonitorInit(void)
{
	memset(&Monitor, 0, sizeof(TMonitor));
	memset(&Dlog, 0, sizeof(TDataLog));

	Monitor.program = 1;
	Monitor.mon_index = 0;
	Monitor.dot = 0;
	Monitor.dot_max = 10;

	//---------------------

	Dlog.Mode       = DLOG_STOP;
	Dlog.DotSpace	= 10;
	Dlog.CntrMax    = 0x200;
	Dlog.Level      = 1;
	Dlog.Delay      = 0;
	Dlog.OutOfTrig  = 1;
	Dlog.Data1Ptr   = &DlogCh1;
	Dlog.Data2Ptr   = &DlogCh2;
	Dlog.Graph1Ptr  = (Int *)0x3FBC00;
	Dlog.Graph2Ptr  = (Int *)0x3FBE00;
}

void MonitorUpdate(void){

	if (!Monitor.dot) switch (Monitor.program)
	{
		 case 1:
			 	 	 Monitor.buffer1[Monitor.mon_index] = g_Peref.IWfltr.Input;
			 	 	 Monitor.buffer2[Monitor.mon_index] = g_Peref.IWfltr.Output;		break;
		 case 2:
					 Monitor.buffer1[Monitor.mon_index] = g_Peref.sensObserver.IWinp;
					 Monitor.buffer2[Monitor.mon_index] = g_Peref.sensObserver.IWout;		break;
		 case 3:
					 Monitor.buffer1[Monitor.mon_index] = g_Peref.sinObserver.IW.Input;
					 Monitor.buffer2[Monitor.mon_index] = g_Peref.sinObserver.IW.Output;		break;
		 case 4:
					 Monitor.buffer1[Monitor.mon_index] = g_Peref.sinObserver.IW.Polarity;
					 Monitor.buffer2[Monitor.mon_index] = g_Peref.sinObserver.IW.CurAngle;		break;

	}

	if (++Monitor.dot > Monitor.dot_max)
	{
		if (++Monitor.mon_index > 255) Monitor.mon_index = 0;
		Monitor.dot = 0;
	}
}

void DlogUpdate(void)
{
	Dlog.Mode     = DLOG_CONT;

	switch (MonSelect)
		{
			//
		 	case 1: DlogCh1 = (Int) g_Peref.sensObserver.URinp;
		 			DlogCh2 = (Int)	g_Peref.sensObserver.URout;

		 	default: return;
		}

	DLOG_update(&Dlog);
}

void DLOG_update(TDataLog *p)
{
	if (p->Mode == DLOG_STOP)
	{
		p->Counter = 0;
		p->Prescaller = 0;
		p->Timer = 0;
		return;
	}

	if (p->OutOfTrig || p->Trigger)
	{
		p->Timer = 0;
		if (++p->Prescaller >= p->DotSpace)
		{
			p->Prescaller = 0;

			p->Graph1Ptr[p->Counter] = *p->Data1Ptr;
			p->Graph2Ptr[p->Counter] = *p->Data2Ptr;

			if (++p->Counter >= p->CntrMax)
			{
				p->Counter = 0;
				p->Trigger = 0;
				if (p->Mode != DLOG_CONT) p->Mode = DLOG_STOP;
			}
		}
	}
	else if (p->Timer < p->Delay) p->Timer++;
	else
	{
		if ((*p->TriggerPtr >= p->Level) && (p->DataPrev < p->Level)) p->Trigger = 1;
		p->DataPrev = *p->TriggerPtr;
	}
}
