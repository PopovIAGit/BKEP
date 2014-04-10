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
 


//---------������� ������������� ����������----------------------------------
void InterruptInit    (void)  
{
    //��������� ��������� ����������
    count18kHz = 0;
    count2kHz  = 0;
    count200Hz = 0;
    count50Hz  = 0;
    //��������� ��������� ���������� �����
    tskCnt2kHz  = 0;
    tskCnt200Hz = 0;
    tskCnt50Hz  = 0;
    tskCnt10Hz  = 0;
    //����������� ���������� ����� ��� ������ 2���, 200��, 50��, 10��
    tskMaxCnt18kHz = TaskCount(Task18kHz);
    tskMaxCnt2kHz  = TaskCount(Task2kHz);
    tskMaxCnt200Hz = TaskCount(Task200Hz);
    tskMaxCnt50Hz  = TaskCount(Task50Hz);
    tskMaxCnt10Hz  = TaskCount(Task10Hz);
    //��������� ��������������� ���������� ������� (��� �������)
    X   = 0;
    X10  = 0;
    X18 = 0;
}

//---------������� ��������� ����������--------------------------------------
void InterruptUpdate(void)  //������� ��������� ����������
{
	int i;
	for (i=tskMaxCnt18kHz; i!=0; i--)
		Task18kHz[i-1].Func(Task18kHz[i-1].Data);
    count18kHz++;
    if (tskCnt2kHz<tskMaxCnt2kHz) // ���� �� ������������ ������� ��� ������� 2 ���?
    {
        Task2kHz[tskCnt2kHz].Func(Task2kHz[tskCnt2kHz].Data);
        tskCnt2kHz++;           
    }
    else if (tskCnt200Hz<tskMaxCnt200Hz) // ���� �� ������������ ������� ��� ������� 200 ��?
    {
        Task200Hz[tskCnt200Hz].Func(Task200Hz[tskCnt200Hz].Data);
        tskCnt200Hz++;
    }
    else  if (tskCnt50Hz<tskMaxCnt50Hz) // ���� �� ������������ ������� ��� ������� 50 ��?
    {
        Task50Hz[tskCnt50Hz].Func(Task50Hz[tskCnt50Hz].Data);
        tskCnt50Hz++;
    }
    else if (tskCnt10Hz<tskMaxCnt10Hz) // ���� �� ������������ ������� ��� ������� 10 ��?
    {
        Task10Hz[tskCnt10Hz].Func(Task10Hz[tskCnt10Hz].Data);
        tskCnt10Hz++;
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
//-----------��������  �������-----------------------------------------------
void task1(void) // ��� ������� (������ ��� �������������)
{
	X += 1;
}

void task1_18Hz(void) // ��� ������� (������ ��� �������������)
{

}

void task1_10Hz(void) // ������� ����������
{
	if (++X10 > 10)
	{
		//OUT_TOGGLE(GpioDataRegs.GPADAT.bit.GPIO7);
		GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;
		GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
		X10 = 0;
	}
}
//------------����� �����-----------------------------------------------

