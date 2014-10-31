/*======================================================================
��� �����:          peref_LedsDrv.h
�����:
������ �����:
���� ���������:
��������:
������ ������������ ���������
======================================================================*/

#ifndef PEREF_LEDS_
#define PEREF_LEDS_

#include "config.h"

// ��������





#define LED_CLOSING_MASK	BIT0
#define LED_CONNECT_MASK	BIT1	// ��������� "��/��". 0-��, 1-��
#define LED_CLOSE_MASK		BIT2	// ��������� "�������/�����������". 1 - "�������", 1,0,1,0 - "�����������"
#define LED_FAULT_MASK		BIT3	// ��������� "������". 1 - ������ �� �����, 0 - ��� ������
#define LED_DEFECT_MASK		BIT4	// ��������� "����� ����������������". 0 - ����� �����. ����., 1 - ����� �����. ���.
#define LED_MUFTA_MASK		BIT5
#define LED_OPEN_MASK		BIT6	// ��������� "�������/�����������". 1 - "�������", 1,0,1,0 - "�����������"
#define LED_OPENING_MASK	BIT7
#define LED_MUDU_MASK		BIT8
#define LED_BLUETOOTH_MASK	BIT9	// ��������� "�������/�����������". 1 - "�������", 1,0,1,0 - "�����������"
			   
#define LEDS_WITHOUT_CNTR	0x1F	// ��������� ������ ����������

// ����� ��� ������������ ������ � display ��������
#define LEDS_MASK	0xE0	// 11100000	

typedef struct 
{
	Byte 	isBlink;		// 0 - �� ������, 1 - ������
	Uns 	blinkPRD;		// ������� ������� (��)
	Uns		blinkTimer;		// ������ �������
	Uns		timeOfBlink;	// �����, ��� ���������� �������� ��������� "������" (������� �� blinkPRD)
	Uns		status;			// ������� ������ ������ ����������
} TLedParams, *pLedParams;

typedef union {
	Uns all;
  	struct {
		Uns Closing:1;		// 0	��������� "�����������"
		Uns MuDu:1;		    // 8	��������� "��/��"
		Uns Close:1;		// 2	��������� "�������"
		Uns Fault:1;		// 3	��������� "������"
		Uns Defect:1;		// 4    ��������� "�������������"
		Uns Mufta:1;		// 5	��������� "�����"
		Uns Open:1;			// 6	��������� "�������"
		Uns Opening:1;		// 7	��������� "�����������"
		Uns	Connect:1;		// 1 	��������� "����"
		Uns Bluetooth:1;	// 9	��������� "������ Bluetooth"
     	Uns rsvd:6; 		// 10-15  ������
  } bit;
} TLedReg;

typedef struct
{
	TLedParams 	ledCntr;		// ��������� �� ������� ������� �����
	TLedParams 	ledOpen;		// ��������� "�������"
	TLedParams 	ledFault;		// ��������� "������"
	TLedParams 	ledDefect;		// ��������� "����� ����������������"
	TLedParams 	ledClose;		// ��������� "�������"
	TLedParams 	ledBluetooth;	// ��������� "��/��"
	TLedParams	ledConnect;		// ��������� "������ �����"
	TLedParams	ledMufta;
	TLedParams	ledOpening;
	TLedParams	ledClosing;
	TLedParams	ledMuDu;
	TLedReg		leds;
	Uns 		*pStatus;		// ��������� �� ������ ����
	Uns			leds_displayReg;
	Byte		spiId;          // ������������� SPI-����� (0-SPIA, 1-SPIB, ...)
	Uns			spiBaud;        // ������� ��������������� (����������� ��� ������������ ����)
	Byte		CSAddr;         // ����� ��� ���-�������
} TLeds, *pLeds;


void Peref_LedsInit(pLeds p, Uns);
void Peref_LedsUpdate(pLeds p);

#endif

