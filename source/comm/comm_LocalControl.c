/*======================================================================
��� �����:          comm_LocalControl.c
�����:
������ �����:      	1.0
���� ���������:		12.05.2014
��������:
������ ��������� �������� � ������ �������� ������ ����������,
�������������� ���� �������� � ��������������� �������
======================================================================*/


#include "comm.h"
#include "core_Menu.h"
#include "g_Ram.h"

void Comm_LocalControlInit(TCommMPU *p)
{
	p->btn1Param.timeout = BTN_ENABLE_TIME;
	p->btn2Param.timeout = BTN_ENABLE_TIME;
	p->btn3Param.timeout = BTN_STOP_ENABLE_TIME;
	p->btn4Param.timeout = BTN_STOP_ENABLE_TIME;

	p->btn1Param.timer = 0;
	p->btn2Param.timer = 0;
	p->btn3Param.timer = 0;
	p->btn4Param.timer = 0;

	p->inputBCP_Data = (&g_Ram.ramGroupC.HallBlock);
}

// ������� ��������� ����� ���
Uns Comm_LocalButtonUpdate(TCommMPU *p)
{
	Uns result;

	if (g_Ram.ramGroupC.HallBlock.bit.StateCalib==1 && (!g_Peref.Position.CancelFlag))
	{
		if (p->inputBCP_Data->bit.Open)
		{
			if (++p->btn1Param.timer > p->btn1Param.timeout*10)
			{
				p->btn1Param.timer = 0;
				if (g_Ram.ramGroupA.Faults.Proc.bit.NoOpen==1) g_Ram.ramGroupD.TaskOpen = trTask; //������
				else g_Ram.ramGroupD.TaskOpen = trReset; //��������
			}
		}
		if (p->inputBCP_Data->bit.Close)
		{
			if (++p->btn2Param.timer > p->btn2Param.timeout*10)
			{
				p->btn2Param.timer = 0;
				if (g_Ram.ramGroupA.Faults.Proc.bit.NoClose==1) g_Ram.ramGroupD.TaskClose = trTask;//������
				else g_Ram.ramGroupD.TaskClose = trReset; //��������
			}
		}

		result = 0;
		return result;
	}

	if (!p->inputBCP_Data->all
			|| ((p->inputBCP_Data->all & hmClose)
					&& (p->inputBCP_Data->all & hmOpen)))
	{
		p->btn1Param.timer = 0;
		p->btn2Param.timer = 0;
		p->btn3Param.timer = 0;
		p->btn4Param.timer = 0;

		result = 0;
		return result;
	}
	else
	{
		if (!(p->inputBCP_Data->bit.StopDU || p->inputBCP_Data->bit.StopMU))
		{
			if (p->inputBCP_Data->bit.Open)
			{
				if (++p->btn1Param.timer > p->btn1Param.timeout)
				{
					p->btn1Param.timer = 0;
					result |= BTN_OPEN_BIT;
				}
			}
			if (p->inputBCP_Data->bit.Close)
			{
				if (++p->btn2Param.timer > p->btn2Param.timeout)
				{
					p->btn2Param.timer = 0;
					result |= BTN_CLOSE_BIT;
				}
			}
		}
		else if (p->inputBCP_Data->bit.StopDU || p->inputBCP_Data->bit.StopMU)
		{
			if (++p->btn3Param.timer > p->btn3Param.timeout)
			{
				p->btn3Param.timer = 0;
				result |= BTN_STOP;
			}
		}
	}
	return result;
}

// SDV ���
#define DELAY_TIMEOUT 150 	// 150 = 3 ��� �� 50 ��

#define KVO_KVZ_OFF_DEFAULT { \
		&g_Ram.ramGroupA.StateTu.all, \
		&g_Ram.ramGroupC.TimeBtnStopKVOKVZ, \
		DELAY_TIMEOUT, \
		0, 0, 0 }

TKVOKVZoff KvoKvzOff = KVO_KVZ_OFF_DEFAULT;

// ������� ������������ ����� �� ���������� ��� � ��� ��� �����
// ���� - ���� �������� ����� ���� � ���� ����������� ������� ���� �� ��
// ����� - ���� ���������� ��� � ���
Bool OffKVOKVZ_Control (TKVOKVZoff *p, Uns btnStatus)	// 50 Hz
{
	if (g_Ram.ramGroupA.Faults.Dev.bit.LowPower)		// ��� �������? ����� ��� � ���
	{
		return 1;
	}

	if (g_Comm.bkpNotConnected)							// ��� ��� �� ����� �� �����? ����� ��� � ���
	{
		return 1;
	}

	if (!g_Ram.ramGroupB.KvoKvzOffOnStop)				// ���� ������ ��� � ��� ��� ����� �������� - ������ �� ����
	{
		return 0;
	}

	if (p->delayFlag)							// ���� ����� ���� ��������
	{											// �� ������ ���������� ������ � ���������� ��������� ����� ���������� � ��
		p->offFlag = false;
		if (p->timer++ >= p->delayTimeout)		// ����� ������ ����� ������ �������� ���� ��������
		{
			p->timer = 0;
			p->delayFlag = false;
		}
	}
	else if (!p->offFlag) 						// ���� ���� ���������� ��� � ��� ����
	{											// �� ������� �� ��������� ������ � ��
		if ((btnStatus & BTN_STOP)||	// ���� ������ ������ ���� �� ��� �� ���������� ���� ��������� ��� � ���
			(*p->pTuState & (TU_STOP_OPEN | TU_STOP_CLOSE)) ) // ���� ������ ���� �� �� - ����������
		{
			// ���������� ���� �� ������� ��� � ��� ������ ����� ����, ��� ��������� ������������
			if (CONTACTOR_1_CONTROL && CONTACTOR_2_CONTROL)
			{
				p->offFlag = true;
			}
		}
	}
	else										// ���� ���� ���������� ��� � ��� �������
	{
		if(p->timer++ >= *p->pOnTimeout * 5)	// ����������� ������.
		{										// �� ��������� �������, ���������, ���� �� ����
			if ((btnStatus & BTN_STOP)||
				(*p->pTuState & (TU_STOP_OPEN | TU_STOP_CLOSE)) )
			{									// ���� ���� �� ����, �� ���������� ������
				p->timer = *p->pOnTimeout;
			}
			else								// ���� ���� ����, �� ������� ��� �����
			{
				p->timer = 0;					// ������� ���� ������� ��� � ���
				p->offFlag = false;				// � ���������� ���� ��������, ����� ������� �����, ������ ��� ����� �������� �� ��������� �� � ������
				p->delayFlag = true;
			}
		}
	}

	return p->offFlag;
}

