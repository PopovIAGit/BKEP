/*======================================================================
Имя файла:          modbus_frame.h
Автор:
Версия файла:
Дата изменения:
Описание:
Драйвер коммуникации по протоколу ModBus RTU
======================================================================*/

#ifndef MODBUS_FRAME_
#define MODBUS_FRAME_

#include "comm_ModBusRtu.h"

static void ResetCommumication(TMbPort *, Bool);
__inline void CrcPack(TMbPort *);

//-------------------------------------------------------------------------------
__inline void WordPack(Byte *Buf, Uns Data)
{
	Buf[0] = (Byte)(Data >> 8);
	Buf[1] = (Byte)(Data & 0xFF);
}

__inline Uns WordUnPack(Byte *Buf)
{
	return (((Uns)Buf[0] << 8) | (Uns)Buf[1]);
}

static void DataPack(Byte *Buf, Uns *Data, Byte Count)
{
 	do {
		*Buf++ = (Byte)(*Data >> 8);
		*Buf++ = (Byte)(*Data & 0xFF);
		Data++;
	} while (--Count);
}

static void DataUnPack(Byte *Buf, Uns *Data, Byte Count)
{
 	do {
		*Data  = (Uns)(*Buf++) << 8;
		*Data |= (Uns)(*Buf++);
		Data++;
	} while (--Count);
}

__inline void CrcPack(TMbPort *hPort)
{
	Uns Crc = CalcFrameCrc((hPort->Frame.Buf), hPort->Frame.TxLength);
	hPort->Frame.Buf[hPort->Frame.TxLength++] = (Byte)(Crc & 0xFF);
	hPort->Frame.Buf[hPort->Frame.TxLength++] = (Byte)(Crc >> 8);
	if ( (hPort->Params.Mode==MB_SLAVE)&&(hPort->Params.HardWareType == MCBSP_TYPE) )
	{
		if (hPort->Frame.Buf[1]!=16 && hPort->Frame.Buf[1]!=17) hPort->Frame.Buf[7]=0;
	}
	
}

//-------------------------------------------------------------------------------
static void DiagnosticRequest(TMbPort *hPort, Uns SubFunc, Bool Flag)
{
	Byte *Buf = hPort->Frame.Buf;
	Buf[0] = hPort->Params.Slave;
	Buf[1] = MB_DIAGNOSTICS;
	WordPack(&Buf[2], SubFunc);
	if (!Flag)
	{
		WordPack(&Buf[4], 0x0000);
		hPort->Frame.TxLength = 6;
	}
	else
	{
		DataPack(&Buf[4], hPort->Packet.Data, hPort->Packet.Count);
		hPort->Frame.TxLength = (hPort->Packet.Count << 1) + 4;
	}
}

static void DiagnosticIndication(TMbPort *hPort, Uns Data)
{
	if ((hPort->Frame.RxLength != 8) || (WordUnPack(&hPort->Frame.Buf[4]) != 0x0000))
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	WordPack(&hPort->Frame.Buf[4], Data);
	hPort->Frame.TxLength = 6;
}

static void DiagnosticConfirmation(TMbPort *hPort, Uns Count)
{
	if (hPort->Frame.RxLength != hPort->Frame.TxLength)
	{
		hPort->Packet.Exception = FR_ERR_BAD_RESPONSE_LENGTH;
		return;
	}
	if (WordUnPack(&hPort->Frame.Buf[2]) != hPort->Packet.SubRequest)
	{
		hPort->Packet.Exception = FR_ERR_BAD_RESPONSE_FUNC;
		return;
	}
	DataUnPack(&hPort->Frame.Buf[4], hPort->Packet.Data, Count);
}

__inline void EchoResponse(TMbPort *hPort)
{
	hPort->Frame.TxLength = hPort->Frame.RxLength - 2;
}

//-------------------------------------------------------------------------------
__inline void ReadRegsRequest(TMbPort *hPort)
{
	Byte *Buf = hPort->Frame.Buf;
	if (!hPort->Packet.Count || (hPort->Packet.Count > 210))
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_NUM;
		return;
	}
	Buf[0] = hPort->Params.Slave;
	Buf[1] = MB_READ_REGS;
	WordPack(&Buf[2], hPort->Packet.Addr);
	WordPack(&Buf[4], hPort->Packet.Count);
	hPort->Frame.TxLength = 6;
}

__inline void ReadRegsIndication(TMbPort *hPort)
{
	if (hPort->Frame.RxLength != 8)
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	hPort->Packet.Addr  = WordUnPack(&hPort->Frame.Buf[2]);
	hPort->Packet.Count = WordUnPack(&hPort->Frame.Buf[4]);
	if (!hPort->Packet.Count || (hPort->Packet.Count > 210))
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
}

__inline void ReadRegsResponse(TMbPort *hPort)
{
	Byte *Buf = hPort->Frame.Buf;
	Buf[2] = hPort->Packet.Count << 1;
	DataPack(&Buf[3], hPort->Packet.Data, hPort->Packet.Count);
	hPort->Frame.TxLength = (hPort->Packet.Count << 1) + 3;
	/*Byte *Buf = hPort->Frame.Buf;
	//Buf[2] = hPort->Packet.Count << 1;
	DataPack(&Buf[2], hPort->Packet.Data, hPort->Packet.Count);
	hPort->Frame.TxLength = (hPort->Packet.Count << 1) + 3;*/
}

__inline void ReadRegsConfirmation(TMbPort *hPort)
{
	Byte *Buf = hPort->Frame.Buf;
	if (hPort->Frame.RxLength != (Buf[2] + 5))
	{
		hPort->Packet.Exception = FR_ERR_BAD_RESPONSE_LENGTH;
		return;
	}
	if (Buf[2] != (hPort->Packet.Count << 1))
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_NUM;
		return;
	}
	DataUnPack(&Buf[3], hPort->Packet.Data, hPort->Packet.Count);

}

//-------------------------------------------------------------------------------
__inline void WriteRegRequest(TMbPort *hPort)
{
	Byte *Buf = hPort->Frame.Buf;
	Buf[0] = hPort->Params.Slave;
	Buf[1] = MB_WRITE_REG;
	WordPack(&Buf[2], hPort->Packet.Addr);
	WordPack(&Buf[4], hPort->Packet.Data[0]);
	hPort->Frame.TxLength = 6;
}

__inline void WriteRegIndication(TMbPort *hPort)
{
	if (hPort->Frame.RxLength != 8)
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	hPort->Packet.Addr    = WordUnPack(&hPort->Frame.Buf[2]);
	hPort->Packet.Data[0] = WordUnPack(&hPort->Frame.Buf[4]);
	hPort->Packet.Count   = 1;
}

__inline void WriteRegResponse(TMbPort *hPort)
{
	EchoResponse(hPort);
}

__inline void WriteRegConfirmation(TMbPort *hPort)
{
	if (hPort->Frame.RxLength != 8)
	{
		hPort->Packet.Exception = FR_ERR_BAD_RESPONSE_LENGTH;
		return;
	}
	if (WordUnPack(&hPort->Frame.Buf[2]) != hPort->Packet.Addr)
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_ADDRESS;
		return;
	}
	if (WordUnPack(&hPort->Frame.Buf[4]) != hPort->Packet.Data[0])
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_VALUE;
	}
}

//-------------------------------------------------------------------------------
__inline void ReturnQueryDataRequest(TMbPort *hPort)
{
	if (!hPort->Packet.Count || (hPort->Packet.Count > 210))
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_NUM;
		return;
	}
	DiagnosticRequest(hPort, MB_RET_QUERY_DATA, TRUE);
}

__inline void ReturnQueryDataIndication(TMbPort *hPort)
{
	EchoResponse(hPort);
}

__inline void ReturnQueryDataConfirmation(TMbPort *hPort)
{
	DiagnosticConfirmation(hPort, hPort->Packet.Count);
}

//-------------------------------------------------------------------------------
__inline void RestartCommRequest(TMbPort *hPort)
{
	if ((hPort->Packet.Data[0] != 0x0000) && (hPort->Packet.Data[0] != 0xFF00))
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_VALUE;
		return;
	}
	hPort->Packet.Count = 1;
	DiagnosticRequest(hPort, MB_RESTART_COMM, TRUE);
}

__inline void RestartCommIndication(TMbPort *hPort)
{
	Uns Data;
	if (hPort->Frame.RxLength != 8)
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	Data = WordUnPack(&hPort->Frame.Buf[4]);
	if ((Data != 0x0000) && (Data != 0xFF00))
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	ResetCommumication(hPort, Data != 0);
	EchoResponse(hPort);
}

#define RestartCommConfirmation(hPort)		DiagnosticConfirmation(hPort, 0)

//-------------------------------------------------------------------------------
#define RetDiagnRegRequest(hPort)				DiagnosticRequest(hPort, MB_RET_DIAGN_REG, FALSE)
#define RetDiagnRegIndication(hPort)			DiagnosticIndication(hPort, hPort->Stat.DiagnReg)
#define RetDiagnRegConfirmation(hPort)		DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define ForceListenModeRequest(hPort)			DiagnosticRequest(hPort, MB_FORCE_LISTEN, FALSE)

__inline void ForceListenModeIndication(TMbPort *hPort)
{
	DiagnosticIndication(hPort, 0x0000);
	if (!hPort->Packet.Exception) hPort->Frame.ListenMode = TRUE;
}

#define ForceListenModeConfirmation(hPort)	DiagnosticConfirmation(hPort, 0)

//-------------------------------------------------------------------------------
#define ClearDiagnRegRequest(hPort)			DiagnosticRequest(hPort, MB_CLEAR_DIAGN_REG, FALSE)

__inline void ClearDiagnRegIndication(TMbPort *hPort)
{
	DiagnosticIndication(hPort, 0x0000);
	if (!hPort->Packet.Exception) memset(&hPort->Stat, 0, sizeof(TMbStat));
}

#define ClearDiagnRegConfirmation(hPort)		DiagnosticConfirmation(hPort, 0)

//-------------------------------------------------------------------------------
#define RetBusMsgRequest(hPort)					DiagnosticRequest(hPort, MB_RET_BUS_MSG, FALSE)
#define RetBusMsgIndication(hPort)				DiagnosticIndication(hPort, hPort->Stat.BusMsgCount)
#define RetBusMsgConfirmation(hPort)			DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetBusErrRequest(hPort)					DiagnosticRequest(hPort, MB_RET_BUS_ERR, FALSE)
#define RetBusErrIndication(hPort)				DiagnosticIndication(hPort, hPort->Stat.BusErrCount)
#define RetBusErrConfirmation(hPort)			DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetBusExcRequest(hPort)					DiagnosticRequest(hPort, MB_RET_BUS_EXCEPT, FALSE)
#define RetBusExcIndication(hPort)				DiagnosticIndication(hPort, hPort->Stat.BusExcCount)
#define RetBusExcConfirmation(hPort)			DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetSlaveMsgRequest(hPort)				DiagnosticRequest(hPort, MB_RET_SLAVE_MSG, FALSE)
#define RetSlaveMsgIndication(hPort)			DiagnosticIndication(hPort, hPort->Stat.SlaveMsgCount)
#define RetSlaveMsgConfirmation(hPort)		DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetSlaveNoRespRequest(hPort)			DiagnosticRequest(hPort, MB_RET_SLAVE_NO_RESP, FALSE)
#define RetSlaveNoRespIndication(hPort)		DiagnosticIndication(hPort, hPort->Stat.SlaveNoRespCount)
#define RetSlaveNoRespConfirmation(hPort)	DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetSlaveNakRequest(hPort)				DiagnosticRequest(hPort, MB_RET_SLAVE_NAK, FALSE)
#define RetSlaveNakIndication(hPort)			DiagnosticIndication(hPort, hPort->Stat.SlaveNakCount)
#define RetSlaveNakConfirmation(hPort)		DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetSlaveBusyRequest(hPort)				DiagnosticRequest(hPort, MB_RET_SLAVE_BUSY, FALSE)
#define RetSlaveBusyIndication(hPort)			DiagnosticIndication(hPort, hPort->Stat.SlaveBusyCount)
#define RetSlaveBusyConfirmation(hPort)		DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define RetBusOverrunRequest(hPort)			DiagnosticRequest(hPort, MB_RET_BUS_OVERRUN, FALSE)
#define RetBusOverrunIndication(hPort)		DiagnosticIndication(hPort, hPort->Stat.BusOverrunErrCount)
#define RetBusOverrunConfirmation(hPort)		DiagnosticConfirmation(hPort, 1)

//-------------------------------------------------------------------------------
#define ClearOverrunFlagRequest(hPort)		DiagnosticRequest(hPort, MB_CLEAR_OVERRUN, FALSE)

__inline void ClearOverrunFlagIndication(TMbPort *hPort)
{
	DiagnosticIndication(hPort, 0x0000);
	if (!hPort->Packet.Exception) hPort->Stat.BusOverrunErrCount = 0;
}

#define ClearOverrunFlagConfirmation(hPort)	DiagnosticConfirmation(hPort, 0)

//-------------------------------------------------------------------------------
__inline void WriteRegsRequest(TMbPort *hPort)
{
	Byte *Buf = hPort->Frame.Buf;
	if (!hPort->Packet.Count || (hPort->Packet.Count > 123))
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_NUM;
		return;
	}
	Buf[0] = hPort->Params.Slave;
	Buf[1] = MB_WRITE_REGS;
	WordPack(&Buf[2], hPort->Packet.Addr);
	WordPack(&Buf[4], hPort->Packet.Count);
	Buf[6] = hPort->Packet.Count << 1;
	DataPack(&Buf[7], hPort->Packet.Data, hPort->Packet.Count);
	hPort->Frame.TxLength = Buf[6] + 7;
}

__inline void WriteRegsIndication(TMbPort *hPort)
{
	Byte *Buf = hPort->Frame.Buf;
	hPort->Packet.Addr  = WordUnPack(&Buf[2]);
	hPort->Packet.Count = WordUnPack(&Buf[4]);
	if (!hPort->Packet.Count || (hPort->Packet.Count > 123))
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	if (Buf[6] != (hPort->Packet.Count << 1)) //что за проверка???  Блиииять надоели эти сраные куски кода!!!
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	if (hPort->Frame.RxLength != (Buf[6] + 9))
	{
		hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
		return;
	}
	DataUnPack(&Buf[7], hPort->Packet.Data, hPort->Packet.Count);
}

__inline void WriteRegsResponse(TMbPort *hPort)
{
	hPort->Frame.TxLength = 6;
}

__inline void WriteRegsConfirmation(TMbPort *hPort)
{
	if (hPort->Frame.RxLength != 8)
	{
		hPort->Packet.Exception = FR_ERR_BAD_RESPONSE_LENGTH;
		return;
	}
	if (WordUnPack(&hPort->Frame.Buf[2]) != hPort->Packet.Addr)
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_ADDRESS;
		return;
	}
	if (WordUnPack(&hPort->Frame.Buf[4]) != hPort->Packet.Count)
	{
		hPort->Packet.Exception = FR_ERR_BAD_DATA_NUM;
	}
}

//-------------------------------------------------------------------------------
__inline void ReportSlaveIdRequest(TMbPort *hPort)
{
	hPort->Frame.Buf[0] = hPort->Params.Slave;
	hPort->Frame.Buf[1] = MB_REPORT_ID;
	hPort->Frame.TxLength = 2;
}

__inline void ReportSlaveIdIndication(TMbPort *hPort)
{
	if (hPort->Frame.RxLength != 4) hPort->Packet.Exception = EX_ILLEGAL_DATA_VALUE;
}

__inline void ReportSlaveIdResponse(TMbPort *hPort)
{
	Byte i, *Buf = hPort->Frame.Buf;
	Buf[2] = 8;//hPort->Packet.Count;
	for (i=0; i < Buf[2]; i++) Buf[i+3] = hPort->Packet.Data[i];
	/*Buf[3] = (DEVICE_ID>>8)&0x00FF;
	Buf[4] = DEVICE_ID&0x00FF;
	Buf[5] = (DEVICE_YEAR>>8)&0x00FF;
	Buf[6] = DEVICE_YEAR&0x00FF;
	//
	Buf[7] = (g_Ram.ramGroupA.VersionPO>>8)&0x00FF;
	Buf[8] = g_Ram.ramGroupA.VersionPO&0x00FF;
	if (g_Ram.ramGroupH.Password1!=0) Buf[9] = 1; else Buf[9] = 0;
	if (g_Ram.ramGroupH.Password2!=0) Buf[10] = 1; else Buf[10] = 0;*/

	hPort->Frame.TxLength = Buf[2] + 3;

}

__inline void ReportSlaveIdConfirmation(TMbPort *hPort)
{
	Byte i, *Buf = hPort->Frame.Buf;
	if (hPort->Frame.RxLength != (Buf[2] + 5))
	{
		hPort->Packet.Exception = FR_ERR_BAD_RESPONSE_LENGTH;
		return;
	}
	for (i=0; i < Buf[2]; i++) hPort->Packet.Data[i] = Buf[i+3];
}

#endif

