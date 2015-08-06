
#include "config.h"
#include "comm.h"
#include "g_Ram.h"

Byte testLeds=0;

#define BKP_SCI_ID				SCIA
//#define BKP_SCI_BAUD			192
#define BKP_SCI_PARITY			0

#define OUTSIDE_FRAME	0
#define BEGIN_FRAME		1
#define LINK_ESCAPE		2
#define INSIDE_FRAME	3

#define BOF				0xC0
#define XBOF				0xFF
#define EOF				0xC1
#define CE					0x7D
#define TRANS				0x20

#define INIT_FCS			0xFFFF
#define GOOD_FCS			0x0000
#define GENER_FCS		0xA001

#define FCS_CALC(crc, c)	((crc >> 8) ^ fcsTable[(crc ^ c) & 0xFF])

#define TIMER_SET(ptimer, interval)	*(ptimer) = interval
#define TIMER_RESET(ptimer)				*(ptimer) = 0

#define CONN_SCALE				(Uint16)(1.000 * 18000)

static unsigned int fcsGenFlag = 0;
static unsigned int fcsTable[256];
unsigned int HZ=0;

static void gen_FcsTable(void);

static void async_unwrap_bof(TMbBBHandle Port, char byte);
static void async_unwrap_ce(TMbBBHandle Port, char byte);
static void async_unwrap_eof(TMbBBHandle Port, char byte);
static void async_unwrap_other(TMbBBHandle Port, char byte);
static void async_wrap_char(TMbBBHandle Port);
static char timerExpired(unsigned int *Timer);
static void BkpConnTrEnable(char Level);

void SciMasterConnBetweenBlockInit(TMbBBHandle Port)
{
	memset(&Port->RxPacket, 0, sizeof(TScPacket));
	memset(&Port->TxPacket, 0, sizeof(TScPacket));
	memset(&Port->Frame,    0, sizeof(TScFrame));
	memset(&Port->Stat,     0, sizeof(TScStat));

	Port->Params.UartID      = BKP_SCI_ID;
	//Port->Params.BrrValue    = SCI_BRR(BKP_SCI_BAUD);
	Port->Params.BrrValue    = BKP_SCI_BAUD+1;
	Port->Params.Parity      = BKP_SCI_PARITY;
	Port->Params.Mode        = 1;
	Port->Params.TimeoutPre  = 5;
	Port->Params.TimeoutPost = 5;
	Port->Params.TimeoutConn = CONN_SCALE/100;
	Port->Params.RetryCount  = 3;
	Port->Frame.TxDelayTimeout = CONN_SCALE/250;
	Port->Params.TrEnable    = BkpConnTrEnable;

	Port->TxPacket.Data[0]   = 0x78;
	Port->TxPacket.Data[1]   = 0x56;
	Port->TxPacket.Data[2]   = 0x34;
	Port->TxPacket.Data[3]   = 0x12;

	if(!fcsGenFlag) {gen_FcsTable(); fcsGenFlag = 1;}

	Port->Params.TrEnable(0);

	SCI_init(Port->Params.UartID, Port->Params.BrrValue,
		Port->Params.Parity, 8);

	if(Port->Params.Mode)
	{
		Port->TxPacket.Flag = 1;
	}
	else
	{
		TIMER_SET(&Port->Frame.TimerPost, Port->Params.TimeoutPost);
		TIMER_SET(&Port->Frame.TimerConn, Port->Params.TimeoutConn);
	}
}

static void BkpConnTrEnable(char Level)
{
	RS485_DIR_BKD = Level;
}

void SciMasterConnBetweenBlockUpdate(TMbBBHandle Port)
{
	char sci_err = (SCI_getstatus(Port->Params.UartID) & SCI_RX_ERROR);
		if(!Port->Params.Mode && sci_err)
		{
			SCI_reset(Port->Params.UartID);
			SCI_init(BKP_SCI_ID, Port->Params.BrrValue, 0, 8);
		}

		if(timerExpired(&Port->Frame.TimerPre))
		{
			Port->Params.TrEnable(1);
			SCI_tx_enable(Port->Params.UartID);
			SCI_transmit(Port->Params.UartID, BOF);
			return;
		}

		if(timerExpired(&Port->Frame.TimerPost))
		{
			SCI_tx_disable(Port->Params.UartID);
			Port->Params.TrEnable(0);
			Port->RxPacket.State = OUTSIDE_FRAME;
			SCI_rx_enable(Port->Params.UartID);
			if(Port->Params.Mode) TIMER_SET(&Port->Frame.TimerConn, Port->Params.TimeoutConn);
			return;
		}

		if(timerExpired(&Port->Frame.TimerConn))
		{
			if(Port->Params.Mode)
			{
				Port->Stat.RxNoRespErrCount++;
				Port->Stat.RxErrCount++;
				if(Port->Frame.RetryCounter < Port->Params.RetryCount)
					Port->Frame.RetryCounter++;
				else Port->Frame.ConnFlag = 0;
				Port->TxPacket.Flag = 1;
			}
			else
			{
				Port->Frame.ConnFlag = 0;
			}
			return;
		}

		if(Port->Frame.TxDelayTimer > 0) Port->Frame.TxDelayTimer--;

		if(!Port->TxPacket.Flag) return;

		if(Port->Frame.TxDelayTimer) return;
		Port->Frame.TxDelayTimer = Port->Frame.TxDelayTimeout;

		Port->TxPacket.Flag = 0;

		Port->TxPacket.State = BEGIN_FRAME;
		Port->TxPacket.Len   = 0;
		Port->TxPacket.Fcs   = INIT_FCS;

		SCI_rx_disable(Port->Params.UartID);
		if(sci_err)
		{
			SCI_reset(Port->Params.UartID);
			SCI_init(BKP_SCI_ID, Port->Params.BrrValue, 0, 8);
		}
		TIMER_SET(&Port->Frame.TimerPre, Port->Params.TimeoutPre);
		Port->Stat.TxMsgCount++;

}

void SciMasterConnBetweenBlockCommTimer(TMbBBHandle bPort)
{
	Uns BkpEncPostion=0;
	Uns BkpEncErr=0;

	SciMasterConnBetweenBlockUpdate(bPort);

	bPort->TxPacket.Data[4] = g_Ram.ramGroupH.BkpIndication;// индикация светодиодов
	bPort->TxPacket.Data[5] = g_Core.Temper.OnOffTEN;		// управление теном

	if(!bPort->RxPacket.Flag) return;
	bPort->RxPacket.Flag = 0;

	g_Ram.ramGroupA.VersionPOBkp     = bPort->RxPacket.Data[0];
	BkpEncErr  			= (Uns)bPort->RxPacket.Data[4] << 8;
	BkpEncErr 		   |= (Uns)bPort->RxPacket.Data[3] << 0;
	BkpEncPostion      	= (Uns)bPort->RxPacket.Data[2] << 8;
	BkpEncPostion      |= (Uns)bPort->RxPacket.Data[1] << 0;
	g_Ram.ramGroupH.Position 		= BkpEncPostion;
	g_Ram.ramGroupC.HallBlock.all   = bPort->RxPacket.Data[5];
	//g_Ram.ramGroupA.TemperBKP       = (int16)bPort->RxPacket.Data[6];
	g_Ram.ramGroupH.BKP_Temper      = (int16)bPort->RxPacket.Data[6];
	if (g_Ram.ramGroupH.BKP_Temper > 128) g_Ram.ramGroupH.BKP_Temper -= 255;
	g_Core.Status.bit.Ten 			= bPort->RxPacket.Data[7];
	g_Ram.ramGroupA.RevErrValue		= BkpEncErr;

	bPort->TxPacket.Flag = 1;

}

void SciMasterConnBetweenBlockRxHandler(TMbBBHandle bPort)
{

	char byte;

	//if(!bPort->TxPacket.Flag) return;

	bPort->Stat.RxBytesCount++;

	byte = SCI_getstatus(bPort->Params.UartID);
	if(byte & SCI_BREAK) SCI_reset(bPort->Params.UartID);

	byte = SCI_recieve(bPort->Params.UartID);
	switch(byte) {
		case BOF: async_unwrap_bof(bPort, byte);   break;
		case EOF: async_unwrap_eof(bPort, byte);   break;
		case CE:  async_unwrap_ce(bPort, byte);    break;
		default:  async_unwrap_other(bPort, byte); break;
	}
}

void SciMasterConnBetweenBlockTxHandler(TMbBBHandle bPort)
{
	bPort->Stat.TxBytesCount++;
	async_wrap_char(bPort);
}

static void async_unwrap_bof(TMbBBHandle Port, char byte)
{
	switch(Port->RxPacket.State)
	{
		case LINK_ESCAPE:
		case INSIDE_FRAME:
			Port->Stat.RxErrCount++;
			Port->Stat.RxMissedErrCount++;
			break;
		case OUTSIDE_FRAME:
		case BEGIN_FRAME:
		default:
			break;
	}

	Port->RxPacket.State = BEGIN_FRAME;
	Port->RxPacket.Len   = 0;
	Port->RxPacket.Fcs   = INIT_FCS;
}

static void async_unwrap_eof(TMbBBHandle Port, char byte)
{
	switch(Port->RxPacket.State)
	{
		case OUTSIDE_FRAME:
			Port->Stat.RxErrCount++;
			Port->Stat.RxMissedErrCount++;
			break;
		case BEGIN_FRAME:
		case LINK_ESCAPE:
		case INSIDE_FRAME:
		default:
			Port->RxPacket.State = OUTSIDE_FRAME;
			Port->Stat.RxMsgCount++;
			if(Port->RxPacket.Len != 10)
				Port->Stat.RxFrameErrLenCount++;
			else if(Port->RxPacket.Fcs != GOOD_FCS)
				Port->Stat.RxCrcErrCount++;
			else
			{
				TIMER_RESET(&Port->Frame.TimerConn);
				Port->Frame.ConnFlag = 1;
				Port->Frame.RetryCounter = 0;
				Port->Frame.MsgCount++;
				Port->RxPacket.Flag = 1;
				SCI_rx_disable(Port->Params.UartID);
				break;
			}
			Port->Stat.RxErrCount++;
			break;
	}
}

static void async_unwrap_ce(TMbBBHandle Port, char byte)
{
	switch(Port->RxPacket.State)
	{
		case OUTSIDE_FRAME:
			break;
		case LINK_ESCAPE:
			break;
		case BEGIN_FRAME:
		case INSIDE_FRAME:
		default:
			Port->RxPacket.State = LINK_ESCAPE;
			break;
	}
}

static void async_unwrap_other(TMbBBHandle Port, char byte)
{
	switch(Port->RxPacket.State)
	{
		case LINK_ESCAPE:
			 byte ^= TRANS;
		case BEGIN_FRAME:
			 Port->RxPacket.State = INSIDE_FRAME;
		case INSIDE_FRAME:
			if(Port->RxPacket.Len < 10)
			{
				Port->RxPacket.Data[Port->RxPacket.Len++] = byte;
				Port->RxPacket.Fcs = FCS_CALC(Port->RxPacket.Fcs, byte);
			}
			else
			{
				Port->Stat.RxErrCount++;
				Port->Stat.RxOverflowErrCount++;
				Port->RxPacket.State = OUTSIDE_FRAME;
			}
			break;
		case OUTSIDE_FRAME:
			break;
	}
}

static void async_wrap_char(TMbBBHandle Port)
{
	char byte;

	switch(Port->TxPacket.Len)
	{
		case 10:
			SCI_transmit(Port->Params.UartID, EOF);
			Port->TxPacket.Len++;
			Port->TxPacket.State = OUTSIDE_FRAME;
			return;
		case 8:
			Port->TxPacket.Data[10-2] = (Port->TxPacket.Fcs >> 0) & 0xFF;
			Port->TxPacket.Data[10-1] = (Port->TxPacket.Fcs >> 8) & 0xFF;
		default:
			byte = Port->TxPacket.Data[Port->TxPacket.Len];
			break;
	}

	switch(Port->TxPacket.State)
	{
		case BEGIN_FRAME:
			Port->TxPacket.State = INSIDE_FRAME;
		case INSIDE_FRAME:
			if((byte == BOF) || (byte == EOF) || (byte == CE))
			{
				byte = CE;
				Port->TxPacket.State = LINK_ESCAPE;
			}
			else
			{
				Port->TxPacket.Fcs = FCS_CALC(Port->TxPacket.Fcs, byte);
				Port->TxPacket.Len++;
			}
			break;
		case LINK_ESCAPE:
			Port->TxPacket.Fcs = FCS_CALC(Port->TxPacket.Fcs, byte);
			byte = byte ^ TRANS;
			Port->TxPacket.State = INSIDE_FRAME;
			Port->TxPacket.Len++;
			break;
		case OUTSIDE_FRAME:
			TIMER_SET(&Port->Frame.TimerPost, Port->Params.TimeoutPost);
			return;
	}

	SCI_transmit(Port->Params.UartID, byte);
}

static void gen_FcsTable(void)
{
	unsigned int i, j, v;
	for (i=0; i < 256; i++) {
		v = i;
		for (j=0; j < 8; j++) {
			if (v & 1) v = (v >> 1) ^ GENER_FCS;
			else v = (v >> 1);
		}
		fcsTable[i] = v;
	}
}

static char timerExpired(unsigned int *Timer)
{
	if (*Timer > 0)
	{
		*Timer = *Timer - 1;
		if (*Timer == 0) return 1;
	}
	return 0;
}




