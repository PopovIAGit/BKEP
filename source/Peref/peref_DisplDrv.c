/*
 * peref_DisplDrv.c
 *
 *  Created on: 13 окт. 2015 г.
 *      Author: PopovIA
 */

#include "peref.h"

Uns testData;

void DisplDrvInit(TPerefDisplay *p)
{
	p->data = 0;
	p->dataHi = 0;
	p->dataLow = 0;
	p->displTimer = 0;
}

void DisplDrvUpdate(TPerefDisplay *p)
{
	if (p->data < 100)
	{
		p->dataHi = p->data / 10;
		p->dataLow = p->data % 10;

		p->displTimer++;
		if (p->displTimer > 2)
			p->displTimer = 0;

		if (p->displTimer < 1)
		{
			BCD_FIX = 1;
			asm(" RPT #9 || NOP");

			DIG1 = 1;
			asm(" RPT #9 || NOP");

			DIG2 = 0;
			asm(" RPT #9 || NOP");

			BCD_A = (p->dataHi) & 0x1;
			asm(" RPT #9 || NOP");
			BCD_B = (p->dataHi >> 1) & 0x1;
			asm(" RPT #9 || NOP");
			BCD_C = (p->dataHi >> 2) & 0x1;
			asm(" RPT #9 || NOP");
			BCD_D = (p->dataHi >> 3) & 0x1;
			asm(" RPT #9 || NOP");
			BCD_FIX = 0;
			asm(" RPT #9 || NOP");
		}
		else
		{
			BCD_FIX = 1;
			asm(" RPT #9 || NOP");
			DIG1 = 0;
			asm(" RPT #9 || NOP");
			DIG2 = 1;
			asm(" RPT #9 || NOP");

			BCD_A = (p->dataLow) & 0x1;
			asm(" RPT #9 || NOP");
			BCD_B = (p->dataLow >> 1) & 0x1;
			asm(" RPT #9 || NOP");
			BCD_C = (p->dataLow >> 2) & 0x1;
			asm(" RPT #9 || NOP");
			BCD_D = (p->dataLow >> 3) & 0x1;
			asm(" RPT #9 || NOP");
			BCD_FIX = 0;
		}

	}
	else if (p->data == 999)
	{
		BCD_FIX = 1;
		asm(" RPT #9 || NOP");
		DIG1 = 1;
		asm(" RPT #9 || NOP");
		DIG2 = 1;
		asm(" RPT #9 || NOP");

		BCD_A = (1) & 0x1;
		asm(" RPT #9 || NOP");
		BCD_B = (1) & 0x1;
		asm(" RPT #9 || NOP");
		BCD_C = (1) & 0x1;
		asm(" RPT #9 || NOP");
		BCD_D = (1) & 0x1;
		asm(" RPT #9 || NOP");
		BCD_FIX = 0;
	}
}

