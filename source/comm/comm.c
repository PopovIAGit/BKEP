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

	//����������� ���� � ��� �� ���������� ����� ��� �������� Bluetooth � Modbus
	g_Comm.Bluetooth.HardWareType = g_Comm.mbBt.Params.HardWareType;	// ������
	g_Comm.Bluetooth.ChannelID    = g_Comm.mbBt.Params.ChannelID;		// ������ ������
	g_Comm.Bluetooth.BaudRate	  = g_Comm.mbBt.Params.BaudRate;		//
	g_Comm.Bluetooth.UartBaud	  = g_Comm.mbBt.Params.UartBaud;		//
	g_Comm.Bluetooth.Mode	  	  = g_Comm.mbBt.Params.Mode;			//
	g_Comm.Bluetooth.Parity	   	  = g_Comm.mbBt.Params.Parity;			//


	SerialCommInit(&g_Comm.mbAsu);
	SerialCommInit(&g_Comm.mbShn);
	SerialCommInit(&g_Comm.mbBt);


}
//---------------------------------------------------
void Comm_Update(TComm *p)
{
	//	������� � ��� !!!
	//Comm_LocalControlUpdate(&p->localControl); // �-� ���-�� �������� � ���
	ModBusUpdate(&g_Comm.mbAsu); // slave ����� ����� � ������� ������� ���
	ModBusUpdate(&g_Comm.mbShn); // master ����� ����� � ����������� �������� �����

	SciMasterConnBetweenBlockUpdate(&g_Comm.mbBkp);// master ����� ����� �

	BluetoothWTUpdate(&g_Comm.Bluetooth); //������� Bluetooth
	ModBusUpdate(&g_Comm.mbBt);  // slave
	//ImUpdate(&Im);

	//ModBusUpdate(&g_Comm.mbBt);  // slave
	//ModBusUpdate(&g_Comm.mbBkp); // master
	//SerialCommUpdate(&Mb);
}

//---------------------------------------------------
