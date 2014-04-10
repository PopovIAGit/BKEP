/*======================================================================
Имя файла:          comm_ModbusRtu.c
Автор:
Версия файла:
Дата изменения:
Описание:
Модуль ModBus
======================================================================*/

#define _MASTER_
#define _SLAVE_

#include "comm_ModbusRtu.h"
#include "comm_ModbusConfig.h"
#include "comm_ModbusCrc.h"
#include "comm_ModbusEvent.h"
#include "comm_ModbusFrame.h"
#include "comm_ModbusInterrupts.h"

// Локальные переменные
static Bool CrcTableGenFlag = false;
static Bool OpenFlags[4] = {false, false, false, false};

//#if defined(_MASTER_) && defined(_SLAVE_)
//#elif defined(_MASTER_) && !defined(_SLAVE_)
//#define IsMaster()	1
//#elif !defined(_MASTER_) && defined(_SLAVE_)
//#define IsSlave()	1
//#else
//#error "Неопределенный режим!"
//#endif

// Локальные функции
__inline void UpdateNewFrame(TMbPort *hPort);
#if defined(_MASTER_)
__inline void MasterRequest(TMbPort *hPort);
__inline void MasterConfirmation(TMbPort *hPort);
#endif
#if defined(_SLAVE_)
__inline void SlaveIndication(TMbPort *hPort);
__inline void SlaveResponse(TMbPort *hPort);
#endif



void ModBusInit(TMbPort *hPort)
{
	Bool *OpenFlag = &OpenFlags[hPort->Params.UartID];
	if (*OpenFlag == false)
	{
		memset(&hPort->Packet, 0, sizeof(TMbPacket));
		memset(&hPort->Frame,  0, sizeof(TMbFrame));
		*OpenFlag = true;
	}

	if (!CrcTableGenFlag)
	{
		GenerateCrcTable();
		CrcTableGenFlag = true;
	}
	
	ResetCommumication(hPort, TRUE);
}

void ModBusInvoke(TMbPort *hPort)
{
	if (hPort->Frame.NewMessage)
	{
		UpdateNewFrame(hPort);
		hPort->Frame.NewMessage = FALSE;
	}
	else
	{
		#if defined(_MASTER_)
		if (IsMaster()) MasterRequest(hPort);
		#endif
		#if defined(_SLAVE_)
		if (IsSlave())  SlaveResponse(hPort);
		#endif
	}
}

void ModBusTimings(TMbPort *hPort)
{
	if (!TimerPending(&hPort->Frame.Timer1_5))  BreakFrameEvent(hPort);
	if (!TimerPending(&hPort->Frame.Timer3_5))  NewFrameEvent(hPort);
	if (!TimerPending(&hPort->Frame.TimerPre))  PreambleEvent(hPort);
	if (!TimerPending(&hPort->Frame.TimerPost)) PostambleEvent(hPort);
	if (!TimerPending(&hPort->Frame.TimerConn)) ConnTimeoutEvent(hPort);
	#if defined(_SLAVE_)
	if (!TimerPending(&hPort->Frame.TimerAck))  AcknoledgeEvent(hPort);
	#endif
}

__inline void UpdateNewFrame(TMbPort *hPort)
{
	TMbFrame *Frame = &hPort->Frame;
	TMbStat  *Stat  = &hPort->Stat;
	Byte Status;
	
	Status = SCI_getstatus(hPort->Params.UartID);
	if (Status & SCI_RX_ERROR)
	{
		if (Status & SCI_PARITY_ERROR)  Stat->BusParityErrCount++;
		if (Status & SCI_OVERRUN_ERROR) Stat->BusOverrunErrCount++;
		if (Status & SCI_FRAME_ERROR)   Stat->BusFrameErrCount++;
		SCI_reset(hPort->Params.UartID);
		goto FRAMING_ERROR;
	}
	
	if (Frame->RxLength < 3)
	{
		Stat->BusFrameLenErrCount++;
		goto FRAMING_ERROR;
	}

	if (CalcFrameCrc(Frame->Buf, Frame->RxLength) != GOOD_CRC)
	{
		Stat->BusFrameCrcErrCount++;
		goto FRAMING_ERROR;
	}
	
	Stat->BusMsgCount++;
	
	#if defined(_MASTER_)
	if (IsMaster()) MasterConfirmation(hPort);
	#endif
	#if defined(_SLAVE_)
	if (IsSlave())  SlaveIndication(hPort);
	#endif
	return;
	
FRAMING_ERROR:
	Stat->BusErrCount++;
	SCI_rx_enable(hPort->Params.UartID);
}

#if defined(_MASTER_)
__inline void MasterRequest(TMbPort *hPort)
{
	TMbPacket *Packet = &hPort->Packet;

	if (!Packet->Request) return;
	if (hPort->Frame.WaitResponse) return;
	
	SCI_rx_disable(hPort->Params.UartID);
	
	Packet->Exception = 0;
	switch(Packet->Request)
	{
		case MB_READ_REGS:   ReadRegsRequest(hPort); break;
		case MB_WRITE_REG:   WriteRegRequest(hPort); break;
		case MB_DIAGNOSTICS:
			switch(Packet->SubRequest)
			{
				case MB_RET_QUERY_DATA:    ReturnQueryDataRequest(hPort);  break;
				case MB_RESTART_COMM:      RestartCommRequest(hPort);      break;
				case MB_RET_DIAGN_REG:     RetDiagnRegRequest(hPort);      break;
				case MB_FORCE_LISTEN:      ForceListenModeRequest(hPort);  break;
				case MB_CLEAR_DIAGN_REG:   ClearDiagnRegRequest(hPort);    break;
				case MB_RET_BUS_MSG:       RetBusMsgRequest(hPort);        break;
				case MB_RET_BUS_ERR:       RetBusErrRequest(hPort);        break;
				case MB_RET_BUS_EXCEPT:    RetBusExcRequest(hPort);        break;
				case MB_RET_SLAVE_MSG:     RetSlaveMsgRequest(hPort);      break;
				case MB_RET_SLAVE_NO_RESP: RetSlaveNoRespRequest(hPort);   break;
				case MB_RET_SLAVE_NAK:     RetSlaveNakRequest(hPort);      break;
				case MB_RET_SLAVE_BUSY:    RetSlaveBusyRequest(hPort);     break;
				case MB_RET_BUS_OVERRUN:   RetBusOverrunRequest(hPort);    break;
				case MB_CLEAR_OVERRUN:     ClearOverrunFlagRequest(hPort); break;
				default: Packet->Exception = EX_ILLEGAL_FUNCTION;
			}
		break;
		case MB_WRITE_REGS:  WriteRegsRequest(hPort);     break;
		case MB_REPORT_ID:   ReportSlaveIdRequest(hPort); break;
		default: Packet->Exception = EX_ILLEGAL_FUNCTION;
	}

	if (Packet->Exception || !hPort->Params.Slave)
		SendMasterResponse(hPort);
	else
	{
		hPort->Frame.WaitResponse = TRUE;
		StartTimer(&hPort->Frame.TimerConn);
		SendFrame(hPort);	
	}
}
#endif

#if defined(_SLAVE_)
__inline void SlaveIndication(TMbPort *hPort)
{
	TMbPacket *Packet = &hPort->Packet;
	Byte Slave, Func;
	
	Slave = hPort->Frame.Buf[0];
	if ((Slave != 0) && (Slave != hPort->Params.Slave))
	{
		SCI_rx_enable(hPort->Params.UartID);
		return;	
	}
	
	hPort->Stat.SlaveMsgCount++;
	if (!Slave || hPort->Frame.ListenMode)
		hPort->Stat.SlaveNoRespCount++;
	
	Func = hPort->Frame.Buf[1];
	Packet->Exception = 0;
	switch(Func)
	{
		case MB_READ_REGS:   ReadRegsIndication(hPort); break;
		case MB_WRITE_REG:   WriteRegIndication(hPort); break;
		case MB_DIAGNOSTICS:
			switch(WordUnPack(&hPort->Frame.Buf[2]))
			{
				case MB_RET_QUERY_DATA:    ReturnQueryDataIndication(hPort);  break;
				case MB_RESTART_COMM:      RestartCommIndication(hPort);      break;
				case MB_RET_DIAGN_REG:     RetDiagnRegIndication(hPort);      break;
				case MB_FORCE_LISTEN:      ForceListenModeIndication(hPort);  break;
				case MB_CLEAR_DIAGN_REG:   ClearDiagnRegIndication(hPort);    break;
				case MB_RET_BUS_MSG:       RetBusMsgIndication(hPort);        break;
				case MB_RET_BUS_ERR:       RetBusErrIndication(hPort);        break;
				case MB_RET_BUS_EXCEPT:    RetBusExcIndication(hPort);        break;
				case MB_RET_SLAVE_MSG:     RetSlaveMsgIndication(hPort);      break;
				case MB_RET_SLAVE_NO_RESP: RetSlaveNoRespIndication(hPort);   break;
				case MB_RET_SLAVE_NAK:     RetSlaveNakIndication(hPort);      break;
				case MB_RET_SLAVE_BUSY:    RetSlaveBusyIndication(hPort);     break;
				case MB_RET_BUS_OVERRUN:   RetBusOverrunIndication(hPort);    break;
				case MB_CLEAR_OVERRUN:     ClearOverrunFlagIndication(hPort); break;
				default: Packet->Exception = EX_ILLEGAL_FUNCTION;
			}
		break;
		case MB_WRITE_REGS:  WriteRegsIndication(hPort);     break;
		case MB_REPORT_ID:   ReportSlaveIdIndication(hPort); break;
		default: Packet->Exception = EX_ILLEGAL_FUNCTION;
	}
	
	if (Packet->Exception || (Func == MB_DIAGNOSTICS)) Packet->Response = Func;
	else Packet->Request = Func;
}
#endif

#if defined(_SLAVE_)
__inline void SlaveResponse(TMbPort *hPort)
{
	TMbPacket *Packet = &hPort->Packet;
	TMbFrame *Frame = &hPort->Frame;
	
	if (!Packet->Response) return;
	
	StartTimer(&Frame->TimerConn);

	if (!Frame->Buf[0] || Frame->ListenMode)
	{
		Packet->Response = 0;
		StartTimer(&Frame->TimerPost);
		return;	
	}
	
	if (Packet->Exception)
	{
		hPort->Stat.BusExcCount++;
		switch(Packet->Exception)
		{
			case EX_ACKNOWLEDGE:          Packet->Acknoledge = TRUE; StartTimer(&Frame->TimerAck); break;
			case EX_SLAVE_DEVICE_BUSY:    hPort->Stat.SlaveBusyCount++; break;
			case EX_NEGATIVE_ACKNOWLEDGE: hPort->Stat.SlaveNakCount++;  break;
		}
		Frame->Buf[1]   = Packet->Response | 0x80;
		Frame->Buf[2]   = Packet->Exception;
		Frame->TxLength = 3;
	}
	else switch(Packet->Response)
	{
		case MB_READ_REGS:  ReadRegsResponse(hPort);      break;
		case MB_WRITE_REG:  WriteRegResponse(hPort);      break;
		case MB_WRITE_REGS: WriteRegsResponse(hPort);     break;
		case MB_REPORT_ID:  ReportSlaveIdResponse(hPort); break;
		default: Packet->Response = 0; return;
	}
	Packet->Response = 0;
	
	SendFrame(hPort);
}
#endif

#if defined(_MASTER_)
__inline void MasterConfirmation(TMbPort *hPort)
{
	TMbPacket *Packet = &hPort->Packet;
	TMbFrame *Frame = &hPort->Frame;
	
  StopTimer(&Frame->TimerConn);

	if ((Frame->Buf[1] & 0x7F) != Packet->Request)
	{
		Packet->Exception = FR_ERR_BAD_RESPONSE_FUNC;
	}
	else if (Frame->Buf[1] & 0x80)
	{
		if (Frame->RxLength != 5) Packet->Exception = FR_ERR_BAD_RESPONSE_LENGTH;
		else
		{
			Packet->Exception = Frame->Buf[2];
			hPort->Stat.BusExcCount++;
			switch(Packet->Exception)
			{
				case EX_SLAVE_DEVICE_BUSY:    hPort->Stat.SlaveBusyCount++; break;
				case EX_NEGATIVE_ACKNOWLEDGE: hPort->Stat.SlaveNakCount++;  break;
			}
		}
	}
	else switch(Packet->Request)
	{
		case MB_READ_REGS:   ReadRegsConfirmation(hPort); break;
		case MB_WRITE_REG:   WriteRegConfirmation(hPort); break;
		case MB_DIAGNOSTICS:
			switch(Packet->SubRequest)
			{
				case MB_RET_QUERY_DATA:    ReturnQueryDataConfirmation(hPort);  break;
				case MB_RESTART_COMM:      RestartCommConfirmation(hPort);      break;
				case MB_RET_DIAGN_REG:     RetDiagnRegConfirmation(hPort);      break;
				case MB_FORCE_LISTEN:      ForceListenModeConfirmation(hPort);  break;
				case MB_CLEAR_DIAGN_REG:   ClearDiagnRegConfirmation(hPort);    break;
				case MB_RET_BUS_MSG:       RetBusMsgConfirmation(hPort);        break;
				case MB_RET_BUS_ERR:       RetBusErrConfirmation(hPort);        break;
				case MB_RET_BUS_EXCEPT:    RetBusExcConfirmation(hPort);        break;
				case MB_RET_SLAVE_MSG:     RetSlaveMsgConfirmation(hPort);      break;
				case MB_RET_SLAVE_NO_RESP: RetSlaveNoRespConfirmation(hPort);   break;
				case MB_RET_SLAVE_NAK:     RetSlaveNakConfirmation(hPort);      break;
				case MB_RET_SLAVE_BUSY:    RetSlaveBusyConfirmation(hPort);     break;
				case MB_RET_BUS_OVERRUN:   RetBusOverrunConfirmation(hPort);    break;
				case MB_CLEAR_OVERRUN:     ClearOverrunFlagConfirmation(hPort); break;
			}
		break;
		case MB_WRITE_REGS:  WriteRegsConfirmation(hPort);     break;
		case MB_REPORT_ID:   ReportSlaveIdConfirmation(hPort); break;
	}
	
	if (Packet->Exception) hPort->Stat.BusErrCount++;
	else
	{
		hPort->Stat.SlaveMsgCount++;
		SendMasterResponse(hPort);
	}
}
#endif



















