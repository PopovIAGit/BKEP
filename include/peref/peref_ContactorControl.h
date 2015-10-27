/*======================================================================
��� �����:          peref_ContactorControl.h
�����:				����� �.�.
������ �����:		1.00
���� ���������:		16.04.2014
��������:
������ ���������� ������������
======================================================================*/

#ifndef PEREF_CONTACTOR_CONTROL_
#define PEREF_CONTACTOR_CONTROL_

//----------- ����������� ������������ ������ ------------------------------
#include "config.h"
//--------------------- ���������-------------------------------------------
#define CONT_ERR_TIME	(0.200 * Prd50HZ) // ����� �������� �� ��������� ���������� ����������
//--------------------- ������� --------------------------------------------
#define SET_CONT_CLOSE(V)		CONTACTOR_1_CONTROL = V; asm(" RPT #9 || NOP")
#define SET_CONT_OPEN(V)		CONTACTOR_2_CONTROL = V; asm(" RPT #9 || NOP")

//-------------------- ��������� -------------------------------------------

typedef struct
{
	Bool ContactorError;
	Uns  ContactorCancelFlag;
	Uns  ContactorErrTimer;

	TContactorGroup *ContactorGroup;
} TContactorControl;
//------------------- ���������� ���������� --------------------------------
//------------------- ��������� ������� ------------------------------------

void ContactorInit(TContactorControl *);
void ContactorControlUpdate(TContactorControl *);
void ContactorErrClear(TContactorControl *);

#endif
