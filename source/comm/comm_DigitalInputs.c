/*======================================================================
��� �����:          DigitalInputs.c
�����:
������ �����:
���� ���������:
��������:
������ ��������� ������� �������� �������� �� ��������������. ������ �� �����.
======================================================================*/
//#include "comm_DigitalInputs.h"
#include "comm.h"

void DigitalInputInit(TDigitalInput *p, Uns delay, Uns incDiv, Uns decDiv)
{
	p->timeDelay = delay;
	p->deltOn	 = delay/incDiv;	// deltOn/deltOff ������� � TU_TIME �� g_Ram
	p->deltOff	 = delay/decDiv;
}

void DigitalInpUpdate(TDigitalInput *p)	// �� ���� ������� �������� ������ � ��
{
	// "���������" ������� ���������� �� �����
	p->inpOpen.inputBit 		= (p->input>>DIN_OPEN_BIT) & 0x0001;
	p->inpClose.inputBit 		= (p->input>>DIN_CLOSE_BIT) & 0x0001;
	p->inpStop.inputBit 		= (p->input>>DIN_STOP_BIT) & 0x0001;
	p->inpMu.inputBit 			= (p->input>>DIN_MU_BIT) & 0x0001;
	p->inpResetAlarm.inputBit 	= (p->input>>DIN_RESETALARM_BIT) & 0x0001;
	p->inpReadyTu.inputBit 		= (p->input>>DIN_READYTU_BIT) & 0x0001;
	p->inpDu.inputBit 			= (p->input>>DIN_DU_BIT) & 0x0001;

	// ����� ������� ��������� ������� ��� ������� ����
	DigitalInputCalc (&p->inpOpen, 	 		p->deltOn, p->deltOff, p->timeDelay); 	// ��������� ������������ � Output
	DigitalInputCalc (&p->inpClose, 		p->deltOn, p->deltOff, p->timeDelay);	// � ����� �� ��������: ����� ������� ������, � ���� ��������
	DigitalInputCalc (&p->inpStop, 	 		p->deltOn, p->deltOff, p->timeDelay);	// � ������� ������ - ������ ��������, ������� ������
	DigitalInputCalc (&p->inpMu, 	 		p->deltOn, p->deltOff, p->timeDelay); 	// ��������� ������������ � Output
	DigitalInputCalc (&p->inpResetAlarm, 	p->deltOn, p->deltOff, p->timeDelay); 	// ��������� ������������ � Output
	DigitalInputCalc (&p->inpReadyTu, 	 	p->deltOn, p->deltOff, p->timeDelay); 	// ��������� ������������ � Output
	DigitalInputCalc (&p->inpDu, 	 		p->deltOn, p->deltOff, p->timeDelay); 	// ��������� ������������ � Output

	// ��������� �����
	p->output  = (p->inpOpen.outputBit  	<<(DIN_OPEN_BIT))&DIN_OPEN_BIT;
	p->output |= (p->inpClose.outputBit 	<<(DIN_CLOSE_BIT))&DIN_CLOSE_BIT;
	p->output |= (p->inpStop.outputBit		<<(DIN_STOP_BIT))&DIN_STOP_BIT;
	p->output |= (p->inpMu.outputBit		<<(DIN_MU_BIT))&DIN_MU_BIT;
	p->output |= (p->inpResetAlarm.outputBit<<(DIN_RESETALARM_BIT))&DIN_RESETALARM_BIT;
	p->output |= (p->inpReadyTu.outputBit	<<(DIN_READYTU_BIT))&DIN_READYTU_BIT;
	p->output |= (p->inpDu.outputBit		<<(DIN_DU_BIT))&DIN_DU_BIT;

}

void DigitalInputCalc(TDigitalInputBit *p, Uns Increment, Uns Decrement, Uns TimeDelay)
{
	if (p->inputBit)   // ���� ������ �������
	{
		if (p->Timer < TimeDelay)  // ���� ������ �� ������ ������� ������������
			p->Timer += Increment; // ������ ������������� �� Increment
		else // ������ ������ �������� ������������
		{
			if (!p->outputBit) p->outputBit = 1;	// ����� - � "1"
			p->Timer = TimeDelay;
		}
	}
	else			// ���� ������ �� �������
	{
		if (p->Timer > 0) // ���� ������ ������ ����
			p->Timer -=  Decrement;	// ������ ����������� �� Decrement
		else // ������ ���� �� ����
		{
			if (p->outputBit) p->outputBit = 0;	// ����� - � "0"
			p->Timer = 0;
		}
	}
}
