/*======================================================================
��� �����:          peref_ContactorControl.c
�����:				����� �.�.
������ �����:		1.00
���� ���������:		16.04.2014
��������:
������ ���������� ������������
======================================================================*/

#include "peref.h"


void ContactorInit(TContactorControl *p)
{
	p->ContactorGroup = ToPtr(&g_Ram.ramGroupH.ContGroup);
}

void ContactorControlUpdate(TContactorControl *p)
{
	switch (*p->ContactorGroup)
	{
		case cgStop:
		{
			CONTACTOR_1_CONTROL = 0;
			CONTACTOR_2_CONTROL = 0;

			if (CONTACTOR_1_ENABLE || CONTACTOR_2_ENABLE)		// ���� ������ � ���������� �� ������ �� 0,2 ������� ����� ������ ������� ����
			{
				if (++p->ContactorErrTimer >= CONT_ERR_TIME)
				{
					p->ContactorErrTimer = CONT_ERR_TIME;		// �������� � ������ ���������
					p->ContactorError = true;					// ���������� ������
				}
			}
			else
			{
				p->ContactorErrTimer = 0;						// ���� ������� � ����������� ������� �� ���������� ������, ������ ��� ���� �� ������
			}
		}
			break;
		case cgOpen:
		{
			if (CONTACTOR_2_ENABLE || p->ContactorError) 		//���� ��� ����������� ��������� ��������� ��� ������ ���������� �� ������ � ����
			{
				p->ContactorGroup = cgStop;
				break;
			}
			else
			{
				CONTACTOR_1_CONTROL = 1;						// ���� ��� ������, �������� ���������
				CONTACTOR_2_CONTROL = 0;

				if (!CONTACTOR_1_ENABLE)						// ���� �������� ����� �� ������ ��� �����������, ���� � ������ � ����
				{
					if(++p->ContactorErrTimer >= CONT_ERR_TIME)
					{
						p->ContactorError = true;
						p->ContactorErrTimer = 0;
						p->ContactorGroup = cgStop;
					}
				}
				else p->ContactorErrTimer = 0;
			}
		}
			break;
		case cgClose:
		{
			if (CONTACTOR_1_ENABLE || p->ContactorError) 		//���� ��� ����������� ��������� ��������� ��� ������ ����������� �� ������ � ����
			{
				p->ContactorGroup = cgStop;
				break;
			}
			else
			{
				CONTACTOR_1_CONTROL = 0;
				CONTACTOR_2_CONTROL = 1;						// ���� ��� ������, �������� ���������

				if (!CONTACTOR_2_ENABLE)
				{
					if(++p->ContactorErrTimer >= CONT_ERR_TIME)
					{
						p->ContactorError = true;
						p->ContactorErrTimer = 0;
						p->ContactorGroup = cgStop;
					}
				}
				else p->ContactorErrTimer = 0;
			}

		}
			break;
	}
}

// ����� ������ ���������� � ���������� �������
void ContactorErrClear(TContactorControl *p)
{
	p->ContactorErrTimer = 0;
	p->ContactorError = false;
}
