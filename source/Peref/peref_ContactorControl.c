/*======================================================================
��� �����:          peref_ContactorControl.c
�����:				����� �.�.
������ �����:		1.00
���� ���������:		16.04.2014
��������:
������ ���������� ������������
======================================================================*/

#include "peref.h"

Uns ContTest = 0;
Uns Tmp = 0;

void ContactorInit(TContactorControl *p)
{
	p->ContactorGroup = ToPtr(&g_Ram.ramGroupH.ContGroup);

}

void ContactorControlUpdate(TContactorControl *p)
{
		if (g_Ram.ramGroupG.Mode == 1)
		{
			switch (g_Ram.ramGroupG.TestContactor)
			{
				case cgStop:
					{
						SET_CONT_CLOSE(1);
						SET_CONT_OPEN(1);
					}
					break;
				case cgOpen:
					{
						SET_CONT_CLOSE(1);
						DELAY_US(1);
						SET_CONT_OPEN(0);
						DELAY_US(1);
					}
					break;
				case cgClose:
					{
						SET_CONT_CLOSE(0);
						SET_CONT_OPEN(1);
					}
					break;
			}
		}
		else
		{
			switch (*p->ContactorGroup)
			{
			case cgStop:
				{
					SET_CONT_CLOSE(1);
					SET_CONT_OPEN(1);
				}
				break;
			case cgOpen:
				{
					SET_CONT_CLOSE(1);
					DELAY_US(1);
					SET_CONT_OPEN(0);
					DELAY_US(1);
				}
				break;
			case cgClose:
				{
					SET_CONT_CLOSE(0);
					SET_CONT_OPEN(1);
				}
				break;
			}
		}
}

// ����� ������ ���������� � ���������� �������
void ContactorErrClear(TContactorControl *p)
{
	p->ContactorErrTimer = 0;
	p->ContactorError = false;
}
