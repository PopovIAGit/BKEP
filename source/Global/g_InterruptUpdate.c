/*======================================================================
��� �����:          g_InterruptUpdate.�
�����:
������ �����:
���� ���������:
��������: 
	� ���� ����� ���������� ������� ��������� ����������. ��� �������
������ ���������� � �������� 18 ���, ����� ��� ���������� ���������� ����������
� �������� 2 ���, 200 ��, 50 ��, � 10 ��. ��� ������� �������� �� ���������
�� ����� "�������� ������ ���������� 3.1.vsd".
	���� ��������� ��������: ������������ ���������� ���, ����� �� ������ ����� 
� 18 ��� ����������� ������������� ���������� �����. ��������, �� ������ ����� 
� 18 ��� ����� ����������� ������ ������ 18 ��� + ���� ������ �� ��������� 
����� 2 ���, 200 ��, 50 �� � 10 ��. 
	��� �� ������ ������������ �������� ���������� ����������.
======================================================================*/
#include "g_InterruptUpdate.h"

// ����������� �������� �� ������������ �������� ���������
// ���� ���������� �������� ����� ��� ���������� �������
// ��������� ����������� ��������� ����������, �� ���������
// ��������� �� ������ ��� ����������
STATIC_ASSERT(TaskCount(Task2kHz) < TASK_COUNT_MAX_2kHZ);
STATIC_ASSERT(TaskCount(Task200Hz)< TASK_COUNT_MAX_200HZ);
STATIC_ASSERT(TaskCount(Task50Hz) < TASK_COUNT_MAX_50HZ);
STATIC_ASSERT(TaskCount(Task10Hz) < TASK_COUNT_MAX_10HZ);
 
//-------����������--����������----------------------------------------------

unsigned    count18kHz = 0,         //������� ���������� �������� 18 ���
        	count2kHz  = 0,         //������� ���������� �������� 2  ���
        	count200Hz = 0,         //������� ���������� �������� 200 ��
        	count50Hz  = 0,         //������� ���������� �������� 10  ��

        	tskCnt2kHz  = 0,        //������� ����������� ����� 2 ���. �� �� ����� ����������� ������
        	tskCnt200Hz = 0,        //������� ����������� ����� 200��. �� �� ����� ����������� ������
        	tskCnt50Hz  = 0,        //������� ����������� ����� 50 ��. �� �� ����� ����������� ������
        	tskCnt10Hz  = 0,        //������� ����������� ����� 10 ��. �� �� ����� ����������� ������
        	CpuTime18kHz = 0,
        	CpuMaxTime = SYSCLK/Prd18kHZ;

unsigned        tskMaxCnt18kHz	= TaskCount(Task18kHz),			//���������� ����� ��� ������� 18 ���
				tskMaxCnt2kHz	= TaskCount(Task2kHz),          //���������� ����� ��� ������� 2  ���
				tskMaxCnt200Hz	= TaskCount(Task200Hz),         //���������� ����� ��� ������� 200 ��
				tskMaxCnt50Hz	= TaskCount(Task50Hz),          //���������� ����� ��� ������� 50  ��
				tskMaxCnt10Hz	= TaskCount(Task10Hz);          //���������� ����� ��� ������� 10  ��

//---------������� ��������� ����������--------------------------------------
void InterruptUpdate(void)  //������� ��������� ����������
{
	int i;
	CpuTimer1Regs.TIM.all = 0;
	for (i=tskMaxCnt18kHz; i!=0; i--)
	{
		Task18kHz[i-1].Func(Task18kHz[i-1].Data);
		Task18kHz[i-1].CpuTime = (Uint16)(-CpuTimer1Regs.TIM.all);
	}
	for (i = 0; i < tskMaxCnt18kHz - 1; i++) Task18kHz[i].CpuTime -= Task18kHz[i+1].CpuTime;
    count18kHz++;
	CpuTime18kHz = (Uint16)(-CpuTimer1Regs.TIM.all);
	CpuTimer1Regs.TIM.all = 0;
    if (tskCnt2kHz<tskMaxCnt2kHz) // ���� �� ������������ ������� ��� ������� 2 ���?
    {
        Task2kHz[tskCnt2kHz].Func(Task2kHz[tskCnt2kHz].Data);
		Task2kHz[tskCnt2kHz].CpuTime = (Uint16)(-CpuTimer1Regs.TIM.all);
		if (Task2kHz[tskCnt2kHz].CpuTime  + CpuTime18kHz > CpuMaxTime)
			g_Ram.ramGroupH.BadTask_2kHz |= 1<<tskCnt2kHz;
        tskCnt2kHz++;           
    }
    else if (tskCnt200Hz<tskMaxCnt200Hz) // ���� �� ������������ ������� ��� ������� 200 ��?
    {
        Task200Hz[tskCnt200Hz].Func(Task200Hz[tskCnt200Hz].Data);
		Task200Hz[tskCnt200Hz].CpuTime = (Uint16)(-CpuTimer1Regs.TIM.all);
		if (Task200Hz[tskCnt200Hz].CpuTime + CpuTime18kHz > CpuMaxTime) g_Ram.ramGroupH.BadTask_200Hz |= 1<<tskCnt200Hz;
        tskCnt200Hz++;
    }
    else  if (tskCnt50Hz<tskMaxCnt50Hz) // ���� �� ������������ ������� ��� ������� 50 ��?
    {
        Task50Hz[tskCnt50Hz].Func(Task50Hz[tskCnt50Hz].Data);
		Task50Hz[tskCnt50Hz].CpuTime = (Uint16)(-CpuTimer1Regs.TIM.all);
		if (Task50Hz[tskCnt50Hz].CpuTime + CpuTime18kHz > CpuMaxTime)
		{
			if (tskCnt50Hz <= 15) g_Ram.ramGroupH.BadTask_50Hz[0] |= 1<<tskCnt50Hz;
			else if (tskCnt50Hz <= 31) g_Ram.ramGroupH.BadTask_50Hz[1] |= 1<<(tskCnt50Hz-16);
			else g_Ram.ramGroupH.BadTask_50Hz[2] |= 1<<(tskCnt50Hz-32);
		}
        tskCnt50Hz++;
    }
    else if (tskCnt10Hz<tskMaxCnt10Hz) // ���� �� ������������ ������� ��� ������� 10 ��?
    {
        Task10Hz[tskCnt10Hz].Func(Task10Hz[tskCnt10Hz].Data);
		Task10Hz[tskCnt10Hz].CpuTime = (Uint16)(-CpuTimer1Regs.TIM.all);
		if (Task10Hz[tskCnt10Hz].CpuTime + CpuTime18kHz > CpuMaxTime) g_Ram.ramGroupH.BadTask_10Hz |= 1<<tskCnt10Hz;
        tskCnt10Hz++;
    	if (g_Ram.ramGroupH.TaskList == 0) g_Ram.ramGroupH.CpuTime = Task18kHz[g_Ram.ramGroupH.TaskNumber].CpuTime;
    	else if (g_Ram.ramGroupH.TaskList == 1) g_Ram.ramGroupH.CpuTime = Task2kHz[g_Ram.ramGroupH.TaskNumber].CpuTime;
    	else if (g_Ram.ramGroupH.TaskList == 2) g_Ram.ramGroupH.CpuTime = Task200Hz[g_Ram.ramGroupH.TaskNumber].CpuTime;
    	else if (g_Ram.ramGroupH.TaskList == 3) g_Ram.ramGroupH.CpuTime = Task50Hz[g_Ram.ramGroupH.TaskNumber].CpuTime;
    	else if (g_Ram.ramGroupH.TaskList == 4) g_Ram.ramGroupH.CpuTime = Task10Hz[g_Ram.ramGroupH.TaskNumber].CpuTime;
    }

    if (count18kHz == 9)
    {
        count18kHz = 0;
        count2kHz++;
        tskCnt2kHz = 0;
        if (count2kHz == 10)
        {
            count2kHz = 0;
            count200Hz++;
            tskCnt200Hz = 0;
            if (count200Hz == 4)
            {
                count200Hz=0;
                count50Hz++;
                tskCnt50Hz = 0;
                if (count50Hz == 5)
                {
                    count50Hz = 0;
                    tskCnt10Hz = 0;
                }
            }
        }
    }
}
//------------����� �����-----------------------------------------------

