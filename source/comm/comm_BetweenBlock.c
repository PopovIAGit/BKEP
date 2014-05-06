
#include "config.h"
#include "comm.h"
#include "g_Ram.h"


//#define BKP_SCI_ID			SCIA
//#define SCI_BAUD			SCI_BRR(1152)

//#define SCI_BREAK			0x20

//#define tx_ext_enable()		RS485_DIR_BKD = 0
//#define tx_ext_disable()	RS485_DIR_BKD = 1
//#define is_tx_disable()		(RS485_DIR_BKD == 1)

//struct sci_master_dev dev;

//unsigned long test_counter = 0x12345678;
/*Uns conn_enable = 1;
Uns max_time = 0;
Uns conn_err = 0;*/
//
static Uns CalcCRC(Uns *Buffer, Uns Length);

void SciMasterConnBetweenBlockCommTimer(TMbBBHandle bPort)
{
	if(bPort->TxTimer > 0)
	{
		bPort->TxTimer--;
		if(!bPort->TxTimer)
		{
			BKP_SCI_TX_DISABLE();
			memset(bPort->Buffer, 0, BKP_MAX_SCI_BUF_LEN);
			bPort->RxLength = 0;
			bPort->RxTimer = BKP_SCI_RX_TIMEOUT;
			SCI_rx_enable(BKP_SCI_ID);
		}
	}

	if(bPort->RxTimer > 0)
	{
		bPort->RxTimer--;
		if(!bPort->RxTimer)
		{
			SCI_rx_disable(BKP_SCI_ID);
			bPort->RxFlag = 1;
		}
	}

	if (bPort->ConnTimer < bPort->ConnTimeout) bPort->ConnTimer++;
	else bPort->IsConnected = 0;

	/*if(bPort->TxTimer > 0)
	{
		bPort->TxTimer--;
		if(!bPort->TxTimer)
		{
			BKP_SCI_TX_DISABLE();
			memset(bPort->Buffer, 0, BKP_MAX_SCI_BUF_LEN);
			bPort->RxLength = 0;
			bPort->RxTimer = BKP_SCI_RX_TIMEOUT;
			SCI_rx_enable(BKP_SCI_ID);
		}
	}

	if(bPort->RxTimer > 0)
	{
		bPort->RxTimer--;
		if(!bPort->RxTimer)
		{
			SCI_rx_disable(BKP_SCI_ID);
			bPort->RxFlag = 1;
		}
	}*/
}

void SciMasterConnBetweenBlockInit(TMbBBHandle bPort)
{
	//bPort->timeout = 1000/5;//???
	memset(&bPort, 0, sizeof(TMbBBHandle));
	SCI_init(BKP_SCI_ID, BKP_SCI_BAUD, 0, 8);
	//SCI_init(BKP_SCI_ID, SCI_BRR(BKP_SCI_BAUD), 0, 8, 2);
	bPort->ConnTimeout = BKP_CONN_TIMEOUT;

	//SCI_init(SCI_ID, SCI_BAUD, 0, 8, 2);
	//bPort->conn_enable = 1;
}

void SciMasterConnBetweenBlockUpdate(TMbBBHandle bPort, TBKPDataHandle Data)
{
	/*Uns Crc;

	if(!bPort->WaitResponse)
	{
		memset(bPort->Buffer, 0, BKP_MAX_SCI_BUF_LEN);

		bPort->Buffer[0] = 0x78;
		bPort->Buffer[1] = 0x56;
		bPort->Buffer[2] = 0x34;
		bPort->Buffer[3] = 0x12;
		bPort->Buffer[4] = g_Ram.ramGroupH.BkpIndication;

		Crc = CalcCRC(bPort->Buffer, BKP_MAX_SCI_BUF_LEN-2);
		bPort->Buffer[BKP_MAX_SCI_BUF_LEN-1] = Crc >> 8;
		bPort->Buffer[BKP_MAX_SCI_BUF_LEN-2] = Crc & 0xFF;

		bPort->WaitResponse = 1;
		bPort->TxLength = 1;

		BKP_SCI_TX_ENABLE();
		SCI_tx_enable(BKP_SCI_ID);
		SCI_transmit(BKP_SCI_ID, bPort->Buffer[0]);
	}
	else if(bPort->RxFlag)
	{
		bPort->RxFlag = 0;
		if(bPort->RxLength != BKP_MAX_SCI_BUF_LEN)
		{
			bPort->ConnErr++;
			bPort->IsConnected = 0;
		}
		else
		{
			Crc  = bPort->Buffer[BKP_MAX_SCI_BUF_LEN-1] << 8;
			Crc |= bPort->Buffer[BKP_MAX_SCI_BUF_LEN-2];
			if(Crc != CalcCRC(bPort->Buffer, BKP_MAX_SCI_BUF_LEN-2))
			{
				bPort->ConnErr++;
				bPort->IsConnected = 0;
			}
			else
			{
				bPort->IsConnected = 1;
				Data->BkpVersion     = bPort->Buffer[0]; //гр А
				Data->BkpEncPostion  = (LgUns)bPort->Buffer[4] << 24;//
				Data->BkpEncPostion |= (LgUns)bPort->Buffer[3] << 16;
				Data->BkpEncPostion |= (LgUns)bPort->Buffer[2] << 8;
				Data->BkpEncPostion |= (LgUns)bPort->Buffer[1] << 0;
				Data->BkpHallReg.all = bPort->Buffer[5];//
				Data->BkpTemper      = (Int)bPort->Buffer[6];
				Data->BkpOffTen      = bPort->Buffer[7];//???

				g_Ram.ramGroupA.VersionPOBkp = Data->BkpVersion;
				g_Ram.ramGroupC.Position	 = (Uns)Data->BkpEncPostion;
				g_Ram.ramGroupA.TemperBKP = Data->BkpTemper;
			}
		}
		bPort->WaitResponse = 0;
	}*/
	Uns Crc;

	if(!bPort->WaitResponse)
	{
		memset(bPort->Buffer, 0, BKP_MAX_SCI_BUF_LEN);

		bPort->Buffer[0] = 0x78;
		bPort->Buffer[1] = 0x56;
		bPort->Buffer[2] = 0x34;
		bPort->Buffer[3] = 0x12;
		bPort->Buffer[4] = g_Ram.ramGroupH.BkpIndication;//BkpLedsReg.all;

		Crc = CalcCRC(bPort->Buffer, BKP_MAX_SCI_BUF_LEN-2);
		bPort->Buffer[BKP_MAX_SCI_BUF_LEN-1] = Crc >> 8;
		bPort->Buffer[BKP_MAX_SCI_BUF_LEN-2] = Crc & 0xFF;

		bPort->WaitResponse = 1;
		bPort->TxLength = 1;

		BKP_SCI_TX_ENABLE();
		SCI_tx_enable(BKP_SCI_ID);
		SCI_transmit(BKP_SCI_ID, bPort->Buffer[0]);
	}
	else if(bPort->RxFlag)
	{
		bPort->RxFlag = 0;
		if(bPort->RxLength != BKP_MAX_SCI_BUF_LEN)
		{
			bPort->ConnErr++;
		}
		else
		{
			Crc  = bPort->Buffer[BKP_MAX_SCI_BUF_LEN-1] << 8;
			Crc |= bPort->Buffer[BKP_MAX_SCI_BUF_LEN-2];
			if(Crc != CalcCRC(bPort->Buffer, BKP_MAX_SCI_BUF_LEN-2))
			{
				bPort->ConnErr++;
			}
			else
			{
				bPort->IsConnected = 1;
				bPort->ConnTimer   = 0;
				Data->BkpVersion        = bPort->Buffer[0];
				Data->BkpEncPostion     = (Uint32)bPort->Buffer[4] << 24;
				Data->BkpEncPostion    |= (Uint32)bPort->Buffer[3] << 16;
				Data->BkpEncPostion    |= (Uint32)bPort->Buffer[2] << 8;
				Data->BkpEncPostion    |= (Uint32)bPort->Buffer[1] << 0;
				Data->BkpHallReg.all    = bPort->Buffer[5];
				Data->BkpTemper         = (Int)bPort->Buffer[6];
				Data->BkpOffTen         = bPort->Buffer[7];

				g_Ram.ramGroupA.VersionPOBkp = Data->BkpVersion;
				g_Ram.ramGroupC.Position	 = (Uns)Data->BkpEncPostion;
				g_Ram.ramGroupA.TemperBKP = Data->BkpTemper;
			}
		}
		bPort->WaitResponse = 0;
	}
}

void SciMasterConnBetweenBlockRxHandler(TMbBBHandle bPort)
{
	/*Byte byte = SCI_recieve(SCI_ID);
	
	if(SCI_getstatus(SCI_ID) & SCI_BREAK) {
		SCI_reset(SCI_ID);
		return;
	}
	
	if(bPort->rx_len < MAX_SCI_BUF_LEN) {
		bPort->buf[bPort->rx_len++] = byte;
	}*/

	Uns Data = SCI_recieve(BKP_SCI_ID);
	bPort->RxByteCount++;
	if(SCI_getstatus(BKP_SCI_ID) & SCI_BREAK)
	{
		SCI_reset(BKP_SCI_ID);
		return;
	}
	if(bPort->RxLength < BKP_MAX_SCI_BUF_LEN)
	{
		bPort->Buffer[bPort->RxLength++] = Data;
		if(bPort->RxLength == BKP_MAX_SCI_BUF_LEN) bPort->RxTimer = 1;
		else bPort->RxTimer = BKP_SCI_RX_TIMEOUT;
	}

	/*Uns Data = SCI_recieve(BKP_SCI_ID);
	bPort->RxByteCount++;
	if(SCI_getstatus(BKP_SCI_ID) & SCI_BREAK)
	{
		SCI_reset(BKP_SCI_ID);
		return;
	}
	if(bPort->RxLength < BKP_MAX_SCI_BUF_LEN)
	{
		bPort->Buffer[bPort->RxLength++] = Data;
		if(bPort->RxLength == BKP_MAX_SCI_BUF_LEN) bPort->RxTimer = 1;
		else bPort->RxTimer = BKP_SCI_RX_TIMEOUT;
	}*/
}

void SciMasterConnBetweenBlockTxHandler(TMbBBHandle bPort)
{
	/*if(bPort->tx_len < MAX_SCI_BUF_LEN) {
		SCI_transmit(SCI_ID, bPort->buf[bPort->tx_len++]);
	}
	else {
		SCI_tx_disable(SCI_ID);
		bPort->postable = 1;
	}

	Port->TxByteCount++;
	if(Port->TxLength < BKP_MAX_SCI_BUF_LEN)
	{
		SCI_transmit(BKP_SCI_ID, Port->Buffer[Port->TxLength++]);
	}
	else
	{
		SCI_tx_disable(BKP_SCI_ID);
		Port->TxTimer = BKP_SCI_TX_TIMEOUT;
	}*/

	//-----------------------------
	bPort->TxByteCount++;
	if(bPort->TxLength < BKP_MAX_SCI_BUF_LEN)
	{
		SCI_transmit(BKP_SCI_ID, bPort->Buffer[bPort->TxLength++]);
	}
	else
	{
		SCI_tx_disable(BKP_SCI_ID);
		bPort->TxTimer = BKP_SCI_TX_TIMEOUT;
	}

	/*bPort->TxByteCount++;
	if(bPort->TxLength < BKP_MAX_SCI_BUF_LEN)
	{
		SCI_transmit(BKP_SCI_ID, bPort->Buffer[bPort->TxLength++]);
	}
	else
	{
		SCI_tx_disable(BKP_SCI_ID);
		SCI_rx_enable(BKP_SCI_ID);
		bPort->TxTimer = BKP_SCI_TX_TIMEOUT;
	}*/
}

static Uns CalcCRC(Uns *Buffer, Uns Length)
{
  Uns Crc = 0;
  while(Length--) Crc = Crc + (*Buffer++);
  return ~Crc;
}




