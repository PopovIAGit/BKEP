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
#include "g_Ram.h"
//--------------------- ���������-------------------------------------------
#define CONT_ERR_TIME	(0.200 * Prd50HZ) // ����� �������� �� ��������� ���������� ����������
//--------------------- ������� --------------------------------------------
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
void ContactorControl(TContactorControl *);
void ContactorErrClear(TContactorControl *);

#endif
