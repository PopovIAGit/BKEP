/*
 * peref_74HC595.c
 *
 *  Created on: 15 окт. 2015 г.
 *      Author: PopovIA
 */


#include "peref.h"
#define RELE_ON_TIME 10

void Peref_74HC595Init(TPeref_74hc595 *p)
{
	p->SpiId 	 = SPIA;
	p->SpiBaud	 = SPI_BRR(4000);
	p->Data 	 = 0xAAAA; 	// Значение соответсвует выключению всех реле
	p->EmptyData = 0xAA;	// Отключение не использованной части каскада из 3х микросхем

	p->CsFunc(0);

	SPI_init(p->SpiId,SPI_MASTER,0,p->SpiBaud,8);
}

#if NEW_RAZ
void Peref_74HC595UpdateTs(TPeref_74hc595 *p, TOutputReg data)
#else
void Peref_74HC595Update(TPeref_74hc595 *p, TOutputReg data)
#endif
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

#if NEW_RAZ
void Peref_74HC595UpdateLed(TPeref_74hc595 *p, TLedReg *data)
{
	//заполняем буфер для передачи
	p->ShiftReg.bit.D_Out1  = data->bit.Mpz;
	p->ShiftReg.bit.D_Out2  = data->bit.Close;
	p->ShiftReg.bit.D_Out3  = data->bit.Fault;
	p->ShiftReg.bit.D_Out4  = data->bit.Defect;
	p->ShiftReg.bit.D_Out5  = data->bit.MuDu;
	p->ShiftReg.bit.D_Out6  = data->bit.Mufta;
	p->ShiftReg.bit.D_Out7  = data->bit.Open;
	p->ShiftReg.bit.D_Out8  = data->bit.Mpo;
	p->ShiftReg.bit.D_Out9  = data->bit.Connect;
    p->ShiftReg.bit.D_Out10 = data->bit.Bluetooth;

	// отправляем данные
	p->Data  = p->ShiftReg.all;

    p->CsFunc(1);
    SPI_send(p->SpiId, ((p->Data>>8) & 0xFF));
    SPI_send(p->SpiId, (p->Data & 0xFF));
    p->CsFunc(0);
}

void Peref_74HC595UpdateDispRS(TPeref_74hc595 *p, Uns set)
{
	p->ShiftReg.bit.D_Out9 = set;
//	p->Data = p->ShiftReg.all;

  /*  p->CsFunc(1);
    SPI_send(p->SpiId, ((p->Data>>8) & 0xFF));
    SPI_send(p->SpiId, (p->Data & 0xFF));
    p->CsFunc(0);*/
}

void Peref_74HC595UpdateDispRW(TPeref_74hc595 *p, Uns set)
{
	p->ShiftReg.bit.D_Out10 = set;
//	p->Data = p->ShiftReg.all;

/*    p->CsFunc(1);
    SPI_send(p->SpiId, ((p->Data>>8) & 0xFF));
    SPI_send(p->SpiId, (p->Data & 0xFF));
    p->CsFunc(0);*/
}

void Peref_74HC595UpdateDispEN(TPeref_74hc595 *p, Uns set)
{
	p->ShiftReg.bit.D_Out11 = set;
//	p->Data = p->ShiftReg.all;

  /*  p->CsFunc(1);
    SPI_send(p->SpiId, ((p->Data>>8) & 0xFF));
    SPI_send(p->SpiId, (p->Data & 0xFF));
    p->CsFunc(0);*/
}

void Peref_74HC595UpdateDispSendData(TPeref_74hc595 *p, Byte Data)
{
	p->ShiftReg.bit.D_Out1 = Data & 0x1;
	p->ShiftReg.bit.D_Out2 = Data>>1 & 0x1;
	p->ShiftReg.bit.D_Out3 = Data>>2 & 0x1;
	p->ShiftReg.bit.D_Out4 = Data>>3 & 0x1;
	p->ShiftReg.bit.D_Out5 = Data>>4 & 0x1;
	p->ShiftReg.bit.D_Out6 = Data>>5 & 0x1;
	p->ShiftReg.bit.D_Out7 = Data>>6 & 0x1;
	p->ShiftReg.bit.D_Out8 = Data>>7 & 0x1;


//	p->Data = p->ShiftReg.all;

 /*   p->CsFunc(1);
    SPI_send(p->SpiId, ((p->Data>>8) & 0xFF));
    SPI_send(p->SpiId, (p->Data & 0xFF));
    p->CsFunc(0);*/
}

void Peref_74HC595UpdateDisp(TPeref_74hc595 *p)
{
	p->Data = p->ShiftReg.all;

    p->CsFunc(1);
    SPI_send(p->SpiId, ((p->Data>>8) & 0xFF));
    SPI_send(p->SpiId, (p->Data & 0xFF));
    p->CsFunc(0);
}
#endif

