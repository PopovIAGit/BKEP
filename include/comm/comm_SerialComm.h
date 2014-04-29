/*======================================================================
��� �����:          comm_SerialComm.h
�����:
������ �����:
���� ���������:
��������:
������ ModBus
======================================================================*/

#ifndef SERIAL_COMM_
#define SERIAL_COMM_

// ����������� ������
//#include "comm_ModbusRtu.h"
#include "g_Structs.h"

// ����� ������ ��� ����� � �� � ���
#define BKP_SCI_ID			SCIA
#define ASU_SCI_ID			SCIB
#define SHN_SCI_ID			SCIC

#define BT_MCBSP_ID			MCBSPA

extern Bool MbBkpConnect;
extern Bool MbAsuConnect;
extern Bool MbShnConnect;
extern Bool MbBtConnect;

__inline void BkpMbSetTr(Byte Lev)  {RS485_DIR_BKD = !Lev;}
__inline void AsuMbSetTr(Byte Lev)  {RS485_DIR_ASU = !Lev;}
__inline void ShnMbSetTr(Byte Lev)  {RS485_DIR_SHN = !Lev;}
__inline void BtMbSetTr(Byte Lev)   {RS485_DIR_BT = !Lev;}

// ��������� �������
void SerialCommInit(TMbHandle);
void ModBusSlaveReset(TMbHandle);

void SerialCommTimings(TMbHandle);

void InitChanelBkpModbus(TMbHandle);
void InitChanelAsuModbus(TMbHandle);
void InitChanelShnModbus(TMbHandle);
void InitChanelBtModbus(TMbHandle);

void ModBusUpdate(TMbHandle);

#endif

