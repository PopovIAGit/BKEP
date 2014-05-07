/*======================================================================
Имя файла:          modbus_event.h
Автор:
Версия файла:
Дата изменения:
Описание:
Драйвер коммуникации по протоколу ModBus RTU
======================================================================*/

#ifndef MODBUS_EVENT_
#define MODBUS_EVENT_

#include "comm_ModBusRtu.h"

static void SendMasterResponse(TMbPort *hPort);
__inline void CrcPack(TMbPort *);
//-------------------------------------------------------------------------------
__inline void BreakFrameEvent(TMbPort *hPort)
{
	if (hPort->Params.HardWareType==UART_TYPE) SCI_rx_disable(hPort->Params.ChannelID);
	else if (hPort->Params.HardWareType==MCBSP_TYPE) McBsp_rx_disable(hPort->Params.ChannelID);
}

//-------------------------------------------------------------------------------
__inline void NewFrameEvent(TMbPort *hPort)
{
	hPort->Frame.NewMessage = true;
	hPort->Frame.RxLength   = hPort->Frame.Data - hPort->Frame.Buf;
	hPort->Frame.Data       = hPort->Frame.Buf;
}

//-------------------------------------------------------------------------------
__inline void PreambleEvent(TMbPort *hPort)
{
	hPort->Frame.Data = hPort->Frame.Buf;

	if (hPort->Params.HardWareType==UART_TYPE) SCI_transmit(hPort->Params.ChannelID, *hPort->Frame.Data++);
	else if (hPort->Params.HardWareType==MCBSP_TYPE) McBsp_transmit(hPort->Params.ChannelID, *hPort->Frame.Data++, 0);
}

//-------------------------------------------------------------------------------
__inline void PostambleEvent(TMbPort *hPort)
{
	hPort->Frame.Data = hPort->Frame.Buf;
	hPort->Params.TrEnable(0);
	//GpioDataRegs.GPADAT.bit.GPIO30=1;???

	if (hPort->Params.HardWareType==UART_TYPE){
		SCI_tx_disable(hPort->Params.ChannelID);
		SCI_rx_enable(hPort->Params.ChannelID);
	}
	else if (hPort->Params.HardWareType==MCBSP_TYPE) {
		McBsp_tx_disable(hPort->Params.ChannelID);
		McBsp_rx_enable(hPort->Params.ChannelID);
	}
}

//-------------------------------------------------------------------------------
__inline void ConnTimeoutEvent(TMbPort *hPort)
{
	#if defined(_MASTER_)
	if (IsMaster())
	{
		if (!hPort->Packet.Exception)
		{
			hPort->Stat.SlaveNoRespCount++;
			hPort->Frame.RetryCounter++;
			if (hPort->Frame.RetryCounter < hPort->Params.RetryCount)
			{
				hPort->Frame.WaitResponse = false;
				return;	
			}
			hPort->Packet.Exception = EX_NO_CONNECTION;
		}
		SendMasterResponse(hPort);
	}
	#endif
	
	#if defined(_SLAVE_)
	if (IsSlave()) hPort->Packet.Exception = EX_NO_CONNECTION;
	#endif
}

//-------------------------------------------------------------------------------
#if defined(_SLAVE_)
__inline void AcknoledgeEvent(TMbPort *hPort)
{
	hPort->Packet.Acknoledge = false;
}
#endif

//-------------------------------------------------------------------------------
static void SendFrame(TMbPort *hPort)
{
	CrcPack(hPort);
	hPort->Params.TrEnable(1);
	//GpioDataRegs.GPADAT.bit.GPIO30=0;???

	if (hPort->Params.HardWareType==UART_TYPE) SCI_tx_enable(hPort->Params.ChannelID);
	else if (hPort->Params.HardWareType==MCBSP_TYPE) McBsp_tx_enable(hPort->Params.ChannelID);

	StartTimer(&hPort->Frame.TimerPre);
	hPort->Stat.TxMsgCount++;
}

//-------------------------------------------------------------------------------
static void SendMasterResponse(TMbPort *hPort)
{
	hPort->Packet.Response    = hPort->Packet.Request;
	hPort->Packet.Request     = 0;
	hPort->Frame.RetryCounter = 0;
	hPort->Frame.WaitResponse = false;
}

#endif

