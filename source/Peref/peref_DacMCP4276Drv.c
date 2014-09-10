/*======================================================================
Имя файла:          peref_Dac7513Drv.c
Автор:              PIA
Версия файла:      	1.0
Дата изменения:		25.07.14
Описание: Драйвер 12-битного ЦАП (микросхема DAC7513)
======================================================================*/

#include "peref.h"

void MCP4276_Init(MCP4726 *p)
{
	p->Slave = 0x60;
}

void MCP4726_Update(MCP4726 *p)
{
	if(!p->Busy)
	{
		if(I2cMsg.Busy) return;

		I2cMsg.Slave     = p->Slave;
		I2cMsg.TxBytes   = 2;
		I2cMsg.RxBytes   = 0;
		I2cMsg.Buffer[0] = (p->Data >> 8) & 0x0F;
		I2cMsg.Buffer[1] = p->Data & 0xFF;
		I2cMsg.Status    = I2C_MSGSTAT_SEND_WITHSTOP;

		p->Busy     = 1;
		I2cMsg.Busy = 1;
	}
	else if(I2cMsg.Status == I2C_MSGSTAT_INACTIVE) //I2C_MSGSTAT_NACK)
	{
		p->Error      = 0;
		p->Busy       = 0;
		I2cMsg.Busy   = 0;
		I2cMsg.Status = I2C_MSGSTAT_INACTIVE;
	}
	else if(I2cMsg.Status == I2C_MSGSTAT_NACK) //I2C_MSGSTAT_INACTIVE)
	{
		p->Error      = 0;
		p->Busy     = 0;
		I2cMsg.Busy = 0;
	}
}








