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
#include "g_Ram.h"

void DigitalInputInit(TDigitalInput *p, Uns delay, Uns incDiv, Uns decDiv)
{
	p->timeDelay = delay;
	p->deltOn	 = delay/incDiv;	// deltOn/deltOff ������� � TU_TIME �� g_Ram
	p->deltOff	 = delay/decDiv;
}

void DigitalInpUpdate(TDigitalInput *p)	// �� ���� ������� �������� ������ � ��
{
	p->TypeLogicSignal = (Uns *)&g_Ram.ramGroupB.TuInvert.all;

    // "���������" ������� ���������� �� �����
	p->inpOpen.inputBit 		= (p->input>>DIN_OPEN_BIT) & 0x0001;
	p->inpClose.inputBit 		= (p->input>>DIN_CLOSE_BIT) & 0x0001;
	p->inpStopOpen.inputBit 	= (p->input>>DIN_STOP_OPEN_BIT) & 0x0001;
	p->inpMu.inputBit 			= (p->input>>DIN_MU_BIT) & 0x0001;
	p->inpResetAlarm.inputBit 	= (p->input>>DIN_RESETALARM_BIT) & 0x0001;
	p->inpStopClose.inputBit 	= (p->input>>DIN_STOP_CLOSE_BIT) & 0x0001;
	p->inpDu.inputBit 			= (p->input>>DIN_DU_BIT) & 0x0001;

	// ���������� ��� �������� �������
	p->inpOpen.signalType 		= (*p->TypeLogicSignal >> DIN_OPEN_BIT)&0x0001;
	p->inpClose.signalType 		= (*p->TypeLogicSignal >> DIN_CLOSE_BIT)&0x0001;
	p->inpStopOpen.signalType 	= (*p->TypeLogicSignal >> DIN_STOP_OPEN_BIT)&0x0001;
	p->inpStopClose.signalType 	= (*p->TypeLogicSignal >> DIN_STOP_CLOSE_BIT)&0x0001;
	p->inpMu.signalType 		= (*p->TypeLogicSignal >> DIN_MU_BIT)&0x0001;
	p->inpDu.signalType 		= (*p->TypeLogicSignal >> DIN_DU_BIT)&0x0001;
	p->inpResetAlarm.signalType 	= (*p->TypeLogicSignal >> DIN_RESETALARM_BIT)&0x0001;

	// ����� ������� ��������� ������� ��� ������� ����
	DigitalInputCalc (&p->inpOpen, 	 		p->deltOn, p->deltOff, p->timeDelay); 	// ��������� ������������ � Output
	DigitalInputCalc (&p->inpClose, 		p->deltOn, p->deltOff, p->timeDelay);	// � ����� �� ��������: ����� ������� ������, � ���� ��������
	DigitalInputCalc (&p->inpStopOpen, 	 	p->deltOn, p->deltOff, p->timeDelay);	// � ������� ������ - ������ ��������, ������� ������
	DigitalInputCalc (&p->inpMu, 	 		p->deltOn, p->deltOff, p->timeDelay); 	// ��������� ������������ � Output
	DigitalInputCalc (&p->inpResetAlarm, 	p->deltOn, p->deltOff, p->timeDelay); 	// ��������� ������������ � Output
	DigitalInputCalc (&p->inpStopClose, 	p->deltOn, p->deltOff, p->timeDelay); 	// ��������� ������������ � Output
	DigitalInputCalc (&p->inpDu, 	 		p->deltOn, p->deltOff, p->timeDelay); 	// ��������� ������������ � Output

	// ��������� �����
	p->output  = (p->inpOpen.outputBit  	<<(DIN_OPEN_BIT))&(1<<DIN_OPEN_BIT);
	p->output |= (p->inpClose.outputBit 	<<(DIN_CLOSE_BIT))&(1<<DIN_CLOSE_BIT);
	p->output |= (p->inpStopOpen.outputBit	<<(DIN_STOP_OPEN_BIT))&(1<<DIN_STOP_OPEN_BIT);
	p->output |= (p->inpStopClose.outputBit	<<(DIN_STOP_CLOSE_BIT))&(1<<DIN_STOP_CLOSE_BIT);
	p->output |= (p->inpMu.outputBit		<<(DIN_MU_BIT))&(1<<DIN_MU_BIT);
	p->output |= (p->inpDu.outputBit		<<(DIN_DU_BIT))&(1<<DIN_DU_BIT);
	p->output |= (p->inpResetAlarm.outputBit<<(DIN_RESETALARM_BIT))&(1<<DIN_RESETALARM_BIT);

}

void DigitalInputCalc(TDigitalInputBit *p, Uns Increment, Uns Decrement, Uns TimeDelay)
{
	if (!(p->signalType))
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
	else if(p->signalType)
	    {
		 if (!p->inputBit)   // ���� ������ �������
			{
				if (p->Timer < TimeDelay)  // ���� ������ �� ������ ������� ������������
					p->Timer +=Increment ; // ������ ������������� �� Increment
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

}
