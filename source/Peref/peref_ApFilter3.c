/*======================================================================
Имя файла:          peref_ApFilter3.c
Автор:              Попов И.А.
Версия файла:       01.00
Дата изменения:		04/04/14
======================================================================*/

#include "peref.h"


void peref_ApFilter3Init(APFILTER3 *p, Uns Freq, Float Tf)
{
	/*p->dt = _IQdiv(1LU, Freq);
	p->Tf = _IQmpy(p->dt, _IQ15(Tf));
	p->K1 = _IQdiv(p->dt, (p->dt + p->Tf));
	p->K2 = _IQdiv(p->Tf, (p->dt + p->Tf));*/

	p->dt =  (Float)(1.0) / (Float) (Freq);
	p->Tf =  p->dt * Tf;
	p->K1 = (p->dt) / (p->dt + p->Tf);
	p->K2 = (p->Tf) / (p->dt + p->Tf);

}

void peref_ApFilter3Reset(APFILTER3 *p)
{
	p->Output = 0;
	p->PrevOut = 0;

	p->Output1 = 0;
	p->PrevOut1 = 0;

	p->Output2 = 0;
	p->PrevOut2 = 0;
}

void peref_ApFilter3Calc(APFILTER3 *p)
{
	/*p->Output1 = _IQmpy(p->Input,p->K1) + _IQmpy(p->PrevOut1,p->K2);
	p->PrevOut1 = p->Output1;

	p->Output2 = _IQmpy(p->Output1,p->K1) + _IQmpy(p->PrevOut2,p->K2);
	p->PrevOut2 = p->Output2;

	p->Output = _IQmpy(p->Output2,p->K1) + _IQmpy(p->PrevOut,p->K2);
	p->PrevOut = p->Output;*/

	p->Output1 = (p->Input * p->K1) + (p->PrevOut1 * p->K2);
	p->PrevOut1 = p->Output1;

	p->Output2 = (p->Output1 * p->K1) + (p->PrevOut2 * p->K2);
	p->PrevOut2 = p->Output2;

	p->Output = (p->Output2 * p->K1) + (p->PrevOut * p->K2);
	p->PrevOut = p->Output;
}
