/*======================================================================
Имя файла:          ApFilter1.c
Автор:              Попов И.А.
Версия файла:       01.00
Дата изменения:		04/04/14
======================================================================*/

#include "peref.h"


void peref_ApFilter1Init(APFILTER1 *p, Uns Freq, Float Tf)
{
	/*p->dt = _IQ15div(1LU, Freq);
	p->Tf = _IQ15mpy(p->dt, _IQ15(Tf));
	p->K1 = _IQdiv(p->dt, (p->dt + p->Tf));
	p->K2 = _IQdiv(p->Tf, (p->dt + p->Tf));*/

	p->dt = (Float) (1.0) / (Float) (Freq);
	p->Tf = p->dt * Tf;
	p->K1 = (p->dt) / (p->dt + p->Tf);
	p->K2 = (p->Tf) / (p->dt + p->Tf);
}

void peref_ApFilter1Reset(APFILTER1 *p)
{
	p->Output  = 0;
	p->PrevOut = 0;
}

void peref_ApFilter1Calc(APFILTER1 *p)
{
	/*p->Output  = _IQmpy(p->Input, p->K1) + _IQmpy(p->PrevOut, p->K2);
	p->PrevOut = p->Output;*/

	/*
	p->OutputIQ15  = _IQ15mpy(p->Input, p->K1) + _IQ15mpy(p->PrevOut, p->K2);
	p->PrevOut = p->OutputIQ15;
	p->OutputInt = (Int)_IQ15int(p->OutputIQ15);
	*/

	p->Output = (p->Input * p->K1) + (p->PrevOut * p->K2);
	p->PrevOut = p->Output;

}
