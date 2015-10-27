/*======================================================================
��� �����:          core_ProtectionI2T.h
�����:				����� �.�.
������ �����:		1.00
���� ���������:		28.04.2014
��������:
������������ ���� ������ �����-������� ������
======================================================================*/

#ifndef CORE_PROTECTION_I2T_
#define CORE_PROTECTION_I2T_

#include "config.h"


//	������ ������� ��������
#define I2T_CONV(curr, currNom, numerator, denominator, subtra) \
	(Uns)(_IQ15div (numerator, _IQ15div(curr, currNom) - denominator) + subtra)

#define K_LOW_I		1.4	// ����������� ��������� ���� � ������������ ����, ��� �������
							// ���������� ���������� ������������ ������ ������������ ������
#define K_HIGH_I	7		// ����������� ��������� ���� � ������������ ����, ��� �������
							// ������ ������� ������������ ������ ������������ ������ �� ������������

// ��������� ��������� ������
typedef union {
	Uns all;
	struct
	{
		Uns Enable	:1;		// 0	���������� ������ (0-����, 1-���.)
		Uns Num		:4;		// 1-4	����� ���� � �������� ������
		Uns Hyst	:12;	// 4-15 �������� �����������
	} bit;
} TAlarmI2TCfg;
//--------------------------------------------------------
// ��������� ������� ������
typedef struct {
	TAlarmI2TCfg	Cfg;			// ������������ ������
	Uns			*InputCurrentMid;	// ����: ������� ��� (rms)
	Uns    		*Output;			// �����: ������ LoadAlarms 0-��� 1-������
	Uns			*NomCurrent;		// ����������� ��� � ����
	Uns			minHighCurrent;		// ����������� �������� ����, ��� ������� �����-������� ������ ����������� ���������
	Uns			maxLowCurrent;	 	// ������������ �������� ����, ��� ������� �����-������� ������ �� �������
	Uns			Timeout;			// ����-��� ������������ (� ������� IQ15)
	Uns			Scale;				// ������� ������� ������� (� ������� IQ15)
	Uns			Timer;				// ������ ������������ (� ������� IQ15)
} TAlarmI2T;

void Core_ProtectionI2TInit(TAlarmI2T *);
void Core_ProtectionI2TUpdate(TAlarmI2T *);

#endif
