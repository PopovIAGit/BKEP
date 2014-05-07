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

//	switch (*p->ContactorGroup)
	switch (ContTest)
	{
		case cgStop:
		{
				SET_CONT_CLOSE(0);
				SET_CONT_OPEN(0);

		/*	if (CONTACTOR_1_STATUS || CONTACTOR_2_STATUS)		// ���� ������ � ���������� �� ������ �� 0,2 ������� ����� ������ ������� ����
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
			}*/
		}
			break;
		case cgOpen:
		{
			SET_CONT_CLOSE(0);
			SET_CONT_OPEN(1);
		/*
			if (CONTACTOR_2_STATUS || p->ContactorError) 		//���� ��� ����������� ��������� ��������� ��� ������ ���������� �� ������ � ����
			{
				p->ContactorGroup = cgStop;
				break;
			}
			else
			{
				SET_CONT_CLOSE(0);
				SET_CONT_OPEN(1);

				if (!CONTACTOR_1_STATUS)						// ���� �������� ����� �� ������ ��� �����������, ���� � ������ � ����
				{
					if(++p->ContactorErrTimer >= CONT_ERR_TIME)
					{
						p->ContactorError = true;
						p->ContactorErrTimer = 0;
						p->ContactorGroup = cgStop;
						Tmp++;
					}
				}
				else p->ContactorErrTimer = 0;
			}*/
		}
			break;
		case cgClose:
		{
			SET_CONT_CLOSE(1);
			SET_CONT_OPEN(0);						// ���� ��� ������, �������� ���������

			/*if (CONTACTOR_1_STATUS || p->ContactorError) 		//���� ��� ����������� ��������� ��������� ��� ������ ����������� �� ������ � ����
			{
				p->ContactorGroup = cgStop;
				break;
			}
			else
			{
				SET_CONT_CLOSE(1);
				SET_CONT_OPEN(0);						// ���� ��� ������, �������� ���������

				if (!CONTACTOR_2_STATUS)
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
			*/
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
