/*======================================================================
��� �����:          comm_DigitalInputs.h
�����:
������ �����:
���� ���������:
��������:
���� - Tu_STAT; ����� - tu_status
======================================================================*/
#ifndef DIGITAL_INPUT_
#define DIGITAL_INPUT_

#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ENABLE_DELAY 8000	// ��������, ��� ���������� ������� ���������, ��� ������ ������
//#define ICREMENT_DIV 25		// ������������� �������� �������� �������
//#define DECREMENT_DIV 10	// ������������� �������� ���������� �������
#define LO_DELTA 25		// ������������� �������� ��������/���������� �������
#define HI_DELTA 40		// ������������� �������� ��������/���������� �������

typedef struct 
{
	unsigned	inputBit;			// ������� ������ 
	unsigned	outputBit;			// �������� ������ 
	int    		Timer;				// ������
	unsigned	signalType;			// ��� ������� (���������/�� ���������)

} TDigitalInputBit;

typedef struct
{
	Uns				 	input;			// ����: �������������� ��������� ����������� ����� � ������������ � ������� � ���������
	Uns					output;			// �����: 
	TDigitalInputBit	inpOpen;		// �������
	TDigitalInputBit	inpClose;		// �������
	TDigitalInputBit	inpStop;		// ����
	TDigitalInputBit	inpMu;			//
	TDigitalInputBit	inpResetAlarm;	//
	TDigitalInputBit	inpReadyTu;		//
	TDigitalInputBit	inpDu;			//
	Uns 				timeDelay;		// �������� "������������" �������
	Uns					deltOn;			// ��������� 
	Uns 				deltOff;		// ���������
	Uns				*TypeLogicSignal;// ��� ������� �� ������ ��������� �����������/��������� - 0/1
} TDigitalInput;

void DigitalInputInit (TDigitalInput *, Uns, Uns, Uns);
void DigitalInpUpdate(TDigitalInput *);
void DigitalInputCalc (TDigitalInputBit *, Uns, Uns, Uns);

#ifdef __cplusplus
}
#endif // extern "C"

#endif	//DIGITAL_INPUT_

