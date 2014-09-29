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
//#include "config.h"
#include "comm_DigitalInputs.h"
#include "comm_LocalControl.h"
#include "comm_ModbusRtu.h"
#include "comm_SerialComm.h"
#include "comm_BetweenBlock.h"
#include "comm_BluetoothWT41.h"
#include "comm_TuTs.h"


#ifdef __cplusplus
extern "C" {
#endif

#define ICREMENT_DIV 25		// Относительная велечина прироста таймера
#define DECREMENT_DIV 10	// Относительная велечина уменьшения таймера
//--------------------------------------------------------
//================== КОММУНИКАЦИИ ========================
//--------------------------------------------------------

typedef struct {
	TCommMPU			localControl;	// Пульт местного управления
	//----
	TDigitalInput 		digitInput;		// Цифровой вход: телеуправлен
	// ---
	TDigitalInterface	digitInterface;	// Дискретный интерфейс: Телесигнализация и телеуправление
	// ---
	Uns					outputCmdReg;	// Выход: суммарный регистр команд
	// ---
	Uns					outputCmdSrc;	// Выход: источник команд
	// ---
	TMbBBPort  			mbBkp;			// ModBus - БКП
	TMbPort  			mbAsu;			// ModBus - АСУ
	TMbPort  			mbShn;			// ModBus - УПП
	TMbPort  			mbBt;			// ModBus - Bluetooth
	TBluetoothPort		Bluetooth;		// Bluetooth
	TBKPData			BkpData;		// структура обмена данными с БКП
} TComm;
//--------------------------------------------------------

void Comm_Init(TComm *);
void Comm_Update(TComm *);
void Comm_50HzCalc(TComm *);
void UpdateTUTS_Test(TComm *p);
Uns DigitCmdModeUpdate (Uns *);
void TekModbusParamsUpdate(void);

extern TComm	g_Comm;

#ifdef __cplusplus
}
#endif // extern "C"

#endif
