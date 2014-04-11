/*======================================================================
��� �����:          comm.c
�����:
������ �����:
���� ���������:
��������:
�������� ������ ��������� ������������
======================================================================*/

#include "comm.h"

TComm	g_Comm;
//---------------------------------------------------
void Comm_Init(TComm *p)
{
	// ����� �������� ����������:
	//Comm_LocalControlInit (&p->localControl);

	SciMasterConnBetweenBlockInit(&g_Comm.mbBkp);

	InitChanelAsuModbus(&g_Comm.mbAsu);
	InitChanelShnModbus(&g_Comm.mbShn);
	InitChanelBtModus(&g_Comm.mbBt);

	SerialCommInit(&g_Comm.mbAsu);
	SerialCommInit(&g_Comm.mbShn);
	SerialCommInit(&g_Comm.mbBt);


}
//---------------------------------------------------
void Comm_Update(TComm *p)
{
	//	������� � ��� !!!
	//Comm_LocalControlUpdate(&p->localControl); // �-� ���-�� �������� � ���
}

//---------------------------------------------------
