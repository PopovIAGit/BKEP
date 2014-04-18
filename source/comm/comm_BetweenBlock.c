
#include "config.h"
#include "comm.h"
#include "g_Ram.h"

#define SCI_ID				SCIA
#define SCI_BAUD			SCI_BRR(1152)

#define SCI_BREAK			0x20

#define tx_ext_enable()		RS485_DIR_BKD = 0
#define tx_ext_disable()	RS485_DIR_BKD = 1
#define is_tx_disable()		(RS485_DIR_BKD == 1)

//struct sci_master_dev dev;

unsigned long test_counter = 0x12345678;
/*Uns conn_enable = 1;
Uns max_time = 0;
Uns conn_err = 0;*/
//
static Uns CalcCRC(Byte *buf, Uns len);

void SciMasterConnBetweenBlockInit(TMbBBHandle bPort)
{
	bPort->timeout = 1000/5;//???
	SCI_init(SCI_ID, SCI_BAUD, 0, 8);
	//SCI_init(SCI_ID, SCI_BAUD, 0, 8, 2);
	bPort->conn_enable = 1;
}

void SciMasterConnBetweenBlockUpdate(TMbBBHandle bPort)
{
	Uns crc;
	
	if(!bPort->wait_resp) {
		if(!bPort->conn_enable) return;
		//conn_enable = 0;

		bPort->buf[0] = (Byte)(test_counter >> 0)  & 0xFF;
		bPort->buf[1] = (Byte)(test_counter >> 8)  & 0xFF;
		bPort->buf[2] = (Byte)(test_counter >> 16) & 0xFF;
		bPort->buf[3] = (Byte)(test_counter >> 24) & 0xFF;

		crc = CalcCRC(bPort->buf, MAX_SCI_BUF_LEN-2);
		bPort->buf[MAX_SCI_BUF_LEN-1] = crc >> 8;
		bPort->buf[MAX_SCI_BUF_LEN-2] = crc & 0xFF;
		
		bPort->wait_resp = 1;
		bPort->timer = 0;
		bPort->rx_len = 0;
		bPort->tx_len = 1;
		tx_ext_enable();
		SCI_tx_enable(SCI_ID);
		SCI_transmit(SCI_ID, bPort->buf[0]);
	}
	else if(is_tx_disable()) {
		if(++bPort->timer >= bPort->timeout) {
			bPort->conn_err++;
			SCI_rx_disable(SCI_ID);
			bPort->wait_resp = 0;
		}
		else if(bPort->rx_len == MAX_SCI_BUF_LEN) {
			SCI_rx_disable(SCI_ID);
			bPort->wait_resp = 0;
			
			crc  = (Uns)bPort->buf[MAX_SCI_BUF_LEN-1] << 8;
			crc |= (Uns)bPort->buf[MAX_SCI_BUF_LEN-2];
			if(crc != CalcCRC(bPort->buf, MAX_SCI_BUF_LEN-2))
				bPort->conn_err++;
			else {
				test_counter  = ((Uns)bPort->buf[3]) << 24;
				test_counter |= ((Uns)bPort->buf[2]) << 16;
				test_counter |= ((Uns)(bPort->buf[1])) << 8;
				test_counter |= ((Uns)bPort->buf[0]);
			}

			if(bPort->max_time < bPort->timer) bPort->max_time = bPort->timer;
		}
	}

	if(bPort->postable) {
		tx_ext_disable();
		memset(bPort->buf, 0, sizeof(bPort->buf));
		SCI_rx_enable(SCI_ID);
		bPort->postable = 0;
	}
}

void SciMasterConnBetweenBlockRxHandler(TMbBBHandle bPort)
{
	Byte byte = SCI_recieve(SCI_ID);
	
	if(SCI_getstatus(SCI_ID) & SCI_BREAK) {
		SCI_reset(SCI_ID);
		return;
	}
	
	if(bPort->rx_len < MAX_SCI_BUF_LEN) {
		bPort->buf[bPort->rx_len++] = byte;
	}
}

void SciMasterConnBetweenBlockTxHandler(TMbBBHandle bPort)
{
	if(bPort->tx_len < MAX_SCI_BUF_LEN) {
		SCI_transmit(SCI_ID, bPort->buf[bPort->tx_len++]);
	}
	else {
		SCI_tx_disable(SCI_ID);
		bPort->postable = 1;
	}
}

static Uns CalcCRC(Byte *buf, Uns len)
{
	Uns crc = 0;
  while(len--) crc = crc + (Uns)(*buf++);
  return ~crc;
}


