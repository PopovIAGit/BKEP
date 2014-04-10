/*======================================================================
Имя файла:          comm.h
Автор:
Версия файла:
Дата изменения:
Описание:
Описание структур и функций модуля коммуникации
========================================================================*/

#ifndef COMM_
#define COMM_

#include "std.h"
//#include "comm_DigitalInputs.h"
//#include "comm_LocalControl.h"
#include "comm_ModbusRtu.h"
#include "comm_SerialComm.h"
#include "comm_BetweenBlock.h"


#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------
//================== КОММУНИКАЦИИ ========================
//--------------------------------------------------------

typedef struct {
	//TCommMPU			localControl;	// Пульт местного управления
	// ---

	// ---
	//Uns					outputCmdReg;	// Выход: суммарный регистр команд
	// ---
	Uns					outputCmdSrc;	// Выход: источник команд
	TMbBBPort  			mbBkp;				// ModBus - БКП
	TMbPort  			mbAsu;				// ModBus - АСУ
	TMbPort  			mbShn;				// ModBus - УПП
	TMbPort  			mbBt;				// ModBus - Bluetooth
} TComm;
//--------------------------------------------------------

void Comm_Init(TComm *);
void Comm_Update(TComm *);
void UpdateTUTS_Test(TComm *p);
Uns DigitCmdModeUpdate (Uns *);

extern TComm	g_Comm;

#ifdef __cplusplus
}
#endif // extern "C"

#endif
