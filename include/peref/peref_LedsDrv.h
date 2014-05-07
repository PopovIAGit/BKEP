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




#define LED_OPEN_MASK		BIT0	// ��������� "�������/�����������". 1 - "�������", 1,0,1,0 - "�����������"
#define LED_MUFTA_MASK		BIT1
#define LED_DEFECT_MASK		BIT2	// ��������� "����� ����������������". 0 - ����� �����. ����., 1 - ����� �����. ���.
#define LED_FAULT_MASK		BIT3	// ��������� "������". 1 - ������ �� �����, 0 - ��� ������
#define LED_CLOSE_MASK		BIT4	// ��������� "�������/�����������". 1 - "�������", 1,0,1,0 - "�����������"
#define LED_BLUETOOTH_MASK	BIT5	// ��������� "�������/�����������". 1 - "�������", 1,0,1,0 - "�����������"
#define LED_CONNECT_MASK	BIT6	// ��������� "��/��". 0-��, 1-��

			   
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
		Uns Open:1;			// 0	��������� "�������"
		Uns Mufta:1;		// 1	��������� "�����"
		Uns Defect:1;		// 2    ��������� "�������������"
		Uns Fault:1;		// 3	��������� "������"
		Uns Close:1;		// 4	��������� "�������"
		Uns Bluetooth:1;	// 5	��������� "������ Bluetooth"
		Uns	Connect:1;		// 6 	��������� "����"
     	Uns rsvd:9; 		// 7-15  ������
  } bit;
} TLedReg;

typedef struct
{
	TLedParams 	ledCntr;		// ��������� �� ������� ������� �����
	TLedParams 	ledOpen;		// ��������� "�������"
	TLedParams 	ledFault;		// ��������� "������"
	TLedParams 	ledDefect;		// ��������� "����� ����������������"
	TLedParams 	ledClose;		// ��������� "�������"
	TLedParams 	ledBluetooth;		// ��������� "��/��"
	TLedParams	ledConnect;		// ��������� "������ �����"
	TLedParams	ledMufta;
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
