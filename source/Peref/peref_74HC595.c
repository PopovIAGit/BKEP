/*
 * peref_74HC595.c
 *
 *  Created on: 15 окт. 2015 г.
 *      Author: PopovIA
 */


#include "peref.h"

void Peref_74HC595Init(TPeref_74hc595 *p)
{
	p->SpiId 	 = SPIA;
	p->SpiBaud	 = SPI_BRR(4000);
	p->Data 	 = 0xAAAA; 	// Значение соответсвует выключению всех реле
	p->EmptyData = 0xAA;	// Отключение не использованной части каскада из 3х микросхем

	p->CsFunc(0);

	SPI_init(p->SpiId,SPI_MASTER,0,p->SpiBaud,8);
}

void Peref_74HC595Update(TPeref_74hc595 *p, TOutputReg data)
{
	if(data.bit.Opened)
	{
		p->ShiftReg.bit.D_Out1 = ON_TS_RELE;
		p->ShiftReg.bit.D_Out2 = OFF_TS_RELE;
	}
	else
	{
		p->ShiftReg.bit.D_Out1 = OFF_TS_RELE;
		p->ShiftReg.bit.D_Out2 = ON_TS_RELE;
	}
	if(data.bit.Closed)
	{
		p->ShiftReg.bit.D_Out3 = ON_TS_RELE;
		p->ShiftReg.bit.D_Out4 = OFF_TS_RELE;
	}
	else
	{
		p->ShiftReg.bit.D_Out3 = OFF_TS_RELE;
		p->ShiftReg.bit.D_Out4 = ON_TS_RELE;
	}
	if(data.bit.Opening)
	{
		p->ShiftReg.bit.D_Out5 = ON_TS_RELE;
		p->ShiftReg.bit.D_Out6 = OFF_TS_RELE;
	}
	else
	{
		p->ShiftReg.bit.D_Out5 = OFF_TS_RELE;
		p->ShiftReg.bit.D_Out6 = ON_TS_RELE;
	}
	if(data.bit.Closing)
	{
		p->ShiftReg.bit.D_Out7 = ON_TS_RELE;
		p->ShiftReg.bit.D_Out8 = OFF_TS_RELE;
	}
	else
	{
		p->ShiftReg.bit.D_Out7 = OFF_TS_RELE;
		p->ShiftReg.bit.D_Out8 = ON_TS_RELE;
	}
	if(data.bit.Defect)
	{
		p->ShiftReg.bit.D_Out9 = ON_TS_RELE;
		p->ShiftReg.bit.D_Out10 = OFF_TS_RELE;
	}
	else
	{
		p->ShiftReg.bit.D_Out9 = OFF_TS_RELE;
		p->ShiftReg.bit.D_Out10 = ON_TS_RELE;
	}
	if(data.bit.Mufta)
	{
		p->ShiftReg.bit.D_Out11 = ON_TS_RELE;
		p->ShiftReg.bit.D_Out12 = OFF_TS_RELE;
	}
	else
	{
		p->ShiftReg.bit.D_Out11 = OFF_TS_RELE;
		p->ShiftReg.bit.D_Out12 = ON_TS_RELE;
	}
	if(data.bit.Fault)
	{
		p->ShiftReg.bit.D_Out13 = ON_TS_RELE;
		p->ShiftReg.bit.D_Out14 = OFF_TS_RELE;
	}
	else
	{
		p->ShiftReg.bit.D_Out13 = OFF_TS_RELE;
		p->ShiftReg.bit.D_Out14 = ON_TS_RELE;
	}
	if(data.bit.MUDU)
	{
		p->ShiftReg.bit.D_Out15 = ON_TS_RELE;
		p->ShiftReg.bit.D_Out16 = OFF_TS_RELE;
	}
	else
	{
		p->ShiftReg.bit.D_Out15 = OFF_TS_RELE;
		p->ShiftReg.bit.D_Out16 = ON_TS_RELE;
	}

	p->Data = p->ShiftReg.all;

	p->CsFunc(1);
	SPI_send(p->SpiId, (p->EmptyData & 0xFF));
	SPI_send(p->SpiId, ((p->Data>>8) & 0xFF));
	SPI_send(p->SpiId, (p->Data & 0xFF));
	p->CsFunc(0);
}
