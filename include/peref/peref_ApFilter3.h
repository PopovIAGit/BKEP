/*======================================================================
��� �����:          ApFilter3.h
�����:              ������.�.
������ �����:       01.00
���� ���������:		04/04/14
�������������:      ��������� � ����������� IQmath
��������:
�������������� ������ �������� �������
======================================================================*/

#ifndef APFILTER3_
#define APFILTER3_

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// ��������� ��� ������ � ��������
typedef struct APFILTER3 {
	LgInt Input;   // ������� ������
	LgInt Output;  // �������� ������
	LgInt Output1; // ������� ������
	LgInt PrevOut1;// �������� ������
	LgInt Output2; // ������� ������
	LgInt PrevOut2;// �������� ������
	Float dt;      // ���������� ������� �������������
	Float Tf;      // ���������� ������� �������
	Float K1;      // ����������� �������
	Float K2;      // ����������� �������
	LgInt PrevOut; // ���������� �������� ��������� �������
} APFILTER3;

void peref_ApFilter3Init(APFILTER3 *p, Uns Freq, Uns Tf);
void peref_ApFilter3Calc(APFILTER3 *);
void peref_ApFilter3Reset(APFILTER3 *);

#ifdef __cplusplus
}
#endif // extern "C"

#endif
