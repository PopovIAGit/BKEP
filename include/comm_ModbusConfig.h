/*======================================================================
Имя файла:          modbus_config.h
Автор:              
Версия файла:       01.00
Описание:
Модуль конфигурации таймеров и сброса связи
======================================================================*/

#ifndef MODBUS_CONFIG_
#define MODBUS_CONFIG_

#include "comm_ModbusRtu.h"

//-------------------------------------------------------------------------------
static void ResetCommumication(TMbPort *hPort, Bool ClearEventLog)
{
	TMbParams *Params = &hPort->Params;
	TMbFrame *Frame = &hPort->Frame;
	Uns Tout1_5, Tout3_5, Scale = (Uns)Params->Scale;
	
	SCI_init(Params->UartID, Params->UartBaud, Params->Parity, 8);
	
	Params->TrEnable(0);
	
	StopTimer(&Frame->Timer1_5);
	StopTimer(&Frame->Timer3_5);
	StopTimer(&Frame->TimerPre);
	StopTimer(&Frame->TimerPost);
	#if defined(_SLAVE_)
	StopTimer(&Frame->TimerAck);
	#endif

	hPort->Packet.Request    = 0;
	hPort->Packet.Response   = 0;
	hPort->Packet.Exception  = EX_NO_CONNECTION;
	hPort->Packet.Acknoledge = false;
	
	Frame->ListenMode   = false;
	Frame->RetryCounter = 0;
	Frame->WaitResponse = false;
	Frame->NewMessage   = false;

	if (Params->BaudRate <= 192)
	{
		Tout1_5 = (165 * Scale) / Params->BaudRate; // 1.5 * 11 * 10 = 165
		Tout3_5 = (385 * Scale) / Params->BaudRate; // 3.5 * 11 * 10 = 385
	}
	else
	{
		Tout1_5 = (3 * Scale) >> 2; // 0.750
		Tout3_5 = (7 * Scale) >> 2; // 1.750
	}
	
	SetTimeout(&Frame->TimerPre,  Params->TxDelay * Scale);
	SetTimeout(&Frame->TimerPost, Tout3_5);
	SetTimeout(&Frame->TimerConn, Params->ConnTimeout * Scale);
	#if defined(_SLAVE_)
	SetTimeout(&Frame->TimerAck,  Params->AckTimeout * Scale);
	#endif
	
	Tout1_5 = Tout1_5 + Params->RxDelay;
	if (Tout3_5 < Tout1_5) Tout3_5 = Tout1_5 + 1;

	SetTimeout(&Frame->Timer1_5,  Tout1_5);
	SetTimeout(&Frame->Timer3_5,  Tout3_5);
	
	memset(&hPort->Stat, 0, sizeof(TMbStat));
	
	#if defined(_SLAVE_)
	if (IsSlave()) StartTimer(&Frame->TimerPost);
	#endif
}

#endif

