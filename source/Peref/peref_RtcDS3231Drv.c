
#include "peref.h"


void DS3231_Init(DS3231 *p)
{
	p->Slave = 0x68;
}

void DS3231_Update(DS3231 *p)
{
	int i, Data, Mode;

	if(!p->Busy)
	{
		if(I2cMsg.Busy) return;

		if(p->Flag)
		{
			I2cMsg.Slave     = p->Slave;
			I2cMsg.TxBytes   = 8;
			I2cMsg.RxBytes   = 0;
			I2cMsg.Buffer[0] = 0;
			for(i = 0; i < 7; i++)
			{
				Data = p->Data[i];
				if(i != 2) {Mode = 0; Data &= 0x7F;}
				else {Mode = Data & 0x60; Data &= 0x1F;}
				I2cMsg.Buffer[i+1] = Mode | (((Data / 10) << 4) | (Data % 10));
			}
			I2cMsg.Status = I2C_MSGSTAT_SEND_WITHSTOP;
		}
		else
		{
			I2cMsg.Slave     = p->Slave;
			I2cMsg.TxBytes   = 1;
			I2cMsg.RxBytes   = 7;
			I2cMsg.Buffer[0] = 0;
			I2cMsg.Status    = I2C_MSGSTAT_SEND_NOSTOP;
		}

		p->Busy     = 1;
		I2cMsg.Busy = 1;
	}
	else if(I2cMsg.Status == I2C_MSGSTAT_INACTIVE)//I2C_MSGSTAT_NACK)
	{
		if (p->Flag)
		{
		    p->Flag = 0;
		//    p->Error = 1;
		}
		else for(i = 0; i < 7; i++)
		{
			p->Error = 0;
			Data = I2cMsg.Buffer[i];
			if(i != 2) {Mode = 0; Data &= 0x7F;}
			else if (!(Data & 0x40)) {Mode = 0; Data &= 0x3F;}
			else {Mode = Data & 0x60; Data &= 0x1F;}
			p->Data[i] = Mode | (((Data >> 4) * 10) + (Data & 0x0F));
		}
		p->Busy       = 0;
		I2cMsg.Busy   = 0;
		I2cMsg.Status = I2C_MSGSTAT_INACTIVE;
	}
	else if(I2cMsg.Status == I2C_MSGSTAT_NACK)//I2C_MSGSTAT_INACTIVE)
	{
		p->Flag     = 0;
		p->Busy     = 0;
		I2cMsg.Busy = 0;
	}
}







