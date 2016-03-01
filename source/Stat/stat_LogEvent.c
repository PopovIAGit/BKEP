/*
������ ��������
*/

#include "stat_LogEvent.h"

void LogEventUpdate(TLogEvent *p)
{
	Uns i, Temp, *Fltr, *FltrPrev, *pBuf = p->Data;

	if (!p->Enable)								
	{
		p->ExecFlag   = false;								
		p->WriteFlag  = false;
		memset(p->FaultsFiltr,     0, sizeof(p->FaultsFiltr));	
		memset(p->FaultsFiltrPrev, 0, sizeof(p->FaultsFiltrPrev));	
		return;						
	}
	
	for (i=0; i < p->FaultsCount; i++)
	{
		Fltr     = &p->FaultsFiltr[i];
		FltrPrev = &p->FaultsFiltrPrev[i];

		Temp   = p->FaultsState[i];
		*Fltr &= Temp;
		Temp  &= ~(*Fltr);
		if (Temp) *Fltr |= Temp;

		if ((*Fltr) ^ (*FltrPrev))
		{
			p->ExecFlag = True;
			//	p->DelayTimer = 0;
			*FltrPrev = *Fltr;
		}
	}
	
	if (p->ExecFlag)													// ��������� ���� ������������ ������
	{
		*pBuf++ = *p->Time;
		*pBuf++ = *p->Date;
		*pBuf++ = *p->Status;
		memcpy(pBuf, p->FaultsState, p->FaultsCount * sizeof(Uns));
		pBuf += p->FaultsCount;
		*pBuf++ = (Uns)(*p->PositionPr);
		*pBuf++ = *p->Torque;
		*pBuf++ = *p->Ur;	*pBuf++ = *p->Us;	*pBuf++ = *p->Ut;
		*pBuf++ = *p->Iu;	*pBuf++ = *p->Iv;	*pBuf++ = *p->Iw;
		*pBuf++ = (Uns)(*p->Temper);
		*pBuf++ = *p->Inputs;
		*pBuf++ = *p->Outputs;
		*pBuf++ = *p->Seconds;

		if (p->FirstEvent)
		{
			p->FirstEvent = false;
			p->ExecFlag   = false;
			p->WriteFlag  = false;
			return;
			*pBuf = 250;
		}
		else
			*pBuf = 0;
		
		p->WriteFlag = True;
		p->ExecFlag  = False;
		p->EventFlag = True;
	}
}


void LogCmdUpdate(TLogCmd *p)
{
	register Uns *pBuf = p->Data;

	if (!p->Enable)								// ������ ��������
	{
		p->ExecFlag   = false;					// ���������� �����
		p->WriteFlag  = false;
		p->CmdRegPrev = 0;						// �������� ���������� �������� �������� ������
		
		return;
	}

	if (p->CmdReg)					// ���� ������� ������� ������ �� ����� ����
	{
		p->ExecFlag = true;			// ��������� ������������ �������
	}

	if (p->ExecFlag)					// ���������� ������
	{
		*pBuf++ = *p->Time;				
		*pBuf++ = *p->Date;				
		*pBuf++ = *p->Seconds;
		*pBuf++ = p->CmdReg;			// ������� ������
			p->CmdReg = 0;				// �������� ������� ������
		*pBuf++ = *p->StatusReg;		// ��������� ������� �����
		*pBuf++ = *p->StatDigOut;		// ������� ��������� ���������� �������
		
		p->WriteFlag = true;
		p->ExecFlag  = false;
	}

}

void LogParamUpdate(TLogParam *p)
{
	register Uns *pBuf = p->Data;

	if (!p->Enable)									// ��������� ���������� �������
	{
		p->ExecFlag = false;						// ���������� ����� � ������� �� �������
		p->WriteFlag = false;
		
		return;
	}

	if (p->ExecFlag)						// �� ����� ��������� ������
	{
		*pBuf++ = *p->Time;				
		*pBuf++ = *p->Date;				
		*pBuf++ = (Uns)(*p->Seconds);
		*pBuf++ = p->Addr;					// ����� ���������
		*pBuf++ = p->NewValue;				// ����� �������� ���������
											// ������ �����������
		p->WriteFlag = true;				// ���������� ���� ���������� ������ ������� � ���
		p->ExecFlag = false;				// ���������� ���� ������������ �������
	}
}

//ma LogSIM
void LogSimUpdate(TLogSim *p)
{
	register Uns *pBuf = p->Data;

	if (!p->Enable)									// ��������� ���������� �������
	{
		p->ExecFlag = false;						// ���������� ����� � ������� �� �������
		p->WriteFlag = false;

		return;
	}

	if (p->ExecFlag)						// �� ����� ��������� ������
	{
		*pBuf++ = *p->Time;
		*pBuf++ = *p->Date;
		*pBuf++ = (Uns)(*p->Seconds);
		*pBuf++ = p->NewSimID[0];				// ����� �������� ��������� SIM
		*pBuf++ = p->NewSimID[1];				// ����� �������� ��������� SIM
		*pBuf++ = p->NewSimID[2];				// ����� �������� ��������� SIM
		*pBuf++ = p->NewSimID[3];				// ����� �������� ��������� SIM
		*pBuf++ = p->NewSimID[4];				// ����� �������� ��������� SIM
		*pBuf++ = p->NewSimID[5];				// ����� �������� ��������� SIM
		*pBuf++ = p->NewSimID[6];				// ����� �������� ��������� SIM
		*pBuf++ = p->NewSimID[7];				// ����� �������� ��������� SIM
		*pBuf++ = p->NewSimID[8];				// ����� �������� ��������� SIM
		*pBuf++ = p->NewSimID[9];				// ����� �������� ��������� SIM

											// ������ �����������
		p->WriteFlag = true;				// ���������� ���� ���������� ������ ������� � ���
		p->ExecFlag = false;				// ���������� ���� ������������ �������
	}
}
