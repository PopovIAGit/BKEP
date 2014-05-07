/*======================================================================
Имя файла:          csl_dlog.h
Автор:
Версия файла:
Дата изменения:
Описание:
Графический монитор реального времени
======================================================================*/

#ifndef CSL_DLOG_
#define CSL_DLOG_

#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DLOG_STOP	0
#define DLOG_CONT	1
#define DLOG_ONCE	2

#define MON_CHAN_SIZE	0x100

typedef struct _TDataLog {
	Uns   Mode;
	Uns   CntrMax;
	Int   Level;
	Uns   DotSpace;
	Uns   Delay;   
	Uns   Counter;
	Uns   Prescaller;
	Uns	Timer;
	Uns   OutOfTrig;
	Int   Trigger;
	Int   DataPrev;
	Int  *TriggerPtr;
	Int  *Data1Ptr;
	Int  *Data2Ptr;
	Int  *Graph1Ptr;
	Int  *Graph2Ptr;
} TDataLog;

typedef struct {

	Uns program;
	Uns mon_index;
	Uns dot;
	Uns dot_max;
	Float MonBuffer[MON_CHAN_SIZE*2];

}TMonitor;

void MonitorInit(void);
void DlogUpdate(void);
void MonitorUpdate(void);

void DLOG_update(TDataLog *);

#ifdef __cplusplus
}
#endif // extern "C"

#endif /* CSL_DLOG_ */

