

#include "peref.h"

void ADT75_Init(ADT75 *p)
{
	p->Slave = 0x48;
}

void ADT75_Update(ADT75 *p)
{
	if(!p->Busy)
	{
		if(I2cMsg.Busy) return;

		I2cMsg.Slave   = p->Slave;
		I2cMsg.TxBytes = 0;
		I2cMsg.RxBytes = 2;
		I2cMsg.Status  = I2C_MSGSTAT_RESTART;
		I2cMsg.Busy    = 1;
		p->Busy        = 1;
	}
	else if(I2cMsg.Status == I2C_MSGSTAT_NACK)
	{
		p->Busy       = 0;
		p->Error      = 1;
		I2cMsg.Busy   = 0;
		I2cMsg.Status = I2C_MSGSTAT_INACTIVE;
	}
	else if(I2cMsg.Status == I2C_MSGSTAT_INACTIVE)
	{
		p->Temper   = ((I2cMsg.Buffer[0] << 8) | (I2cMsg.Buffer[1] & 0xF0)) >> 8;
		p->Busy     = 0;
		I2cMsg.Busy = 0;
	}
}








