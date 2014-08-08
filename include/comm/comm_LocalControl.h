/*======================================================================
��� �����:          comm_LocalControl.h
�����:
������ �����:      	1.0
���� ���������:		12.05.2014
��������:
������������ ���� ������ ��������� �������� � ������ ���
======================================================================*/
#ifndef COMM_MPU_
#define COMM_MPU_

//----------- ����������� ������������ ������ ------------------------------
#include "std.h"
//--------------------- ���������-------------------------------------------

#define BTN_OPEN_BIT    BIT0
#define BTN_CLOSE_BIT   BIT1
#define BTN_STOPMU_BIT  BIT2
#define BTN_STOPDU_BIT  BIT3


//--------------------- ������� --------------------------------------------
#define BTN_ENABLE_TIME			(0.200 * Prd50HZ)
//-------------------- ��������� -------------------------------------------

typedef union
{
	Uns all;
	struct
	{
		Uns btn1:1;		// 0 - ������ 1 (����� �����)
		Uns btn2:1;		// 1 - ������ 2 (����� �����)
		Uns btn3:1;		// 2 - ������ 3 (������ �����)
		Uns	btn4:1;		// 3 - ������ 4 (������ �����)
		Uns rsvd2:12;	// 4-15 - ������
	} bit;
} TInputData;
// ��������� �������� ����� �����
typedef union _THallBlock {
	Uns all;
	struct {
		Uns Open:1;       // 0     �������
		Uns Close:1;      // 1     �������
		Uns StopMU:1;     // 2     ����/��
		Uns StopDU:1;     // 3     ����/��
		Uns Rsvd:12;      // 4-15  ������
	} bit;
} THallBlock;

/*
typedef union _THallBlock
{
	Uns all;
	struct
	{
		Uns Open:1;		// 0 - ������ 1 (����� �����)
		Uns Close:1;		// 1 - ������ 2 (����� �����)
		Uns StopMU:1;		// 2 - ������ 3 (������ �����)
		Uns	StopDU:1;		// 3 - ������ 4 (������ �����)
		Uns rsvd2:12;	// 4-15 - ������
	} bit;
} THallBlock;*/

typedef struct {
	Uns					upLevel;		// ������� ������������
	Uns 				timer;			// ������
	Uns 				timeout;		// ����� ������������
} TButtonsParams;

// ����� ������ ����������
typedef union _TKeyInvert {
	Uns all;
	 struct {
		Uns Open:1;       	// 0     �������
		Uns Close:1;      	// 1     �������
		Uns Stop:1;       	// 2     ����
		Uns Rsvd:13;      	// 3-15  ������
	 } bit;
} TKeyInvert;

// ��������� ��� ������ ���
typedef struct
{
	THallBlock			*inputBCP_Data;	 // ������ � ���
	TButtonsParams		btn1Param;		 // ��������� ������� 1
	TButtonsParams		btn2Param;		 // ��������� ������� 2
	TButtonsParams		btn3Param;		 // ��������� ������� 3
	TButtonsParams		btn4Param;		 // ��������� ������� 4
	TButtonsParams		key1Param;		 // ��������� ������� ��� 1
	TButtonsParams		key2Param;		 // ��������� ������� ��� 2
	TButtonsParams		key3Param;		 // ��������� ������� ��� 3
	TKeyInvert			*KeyLogicSignal; //  ��� ������� �� ������ ��������� �����������/��������� - 0/1
} TCommMPU;
//------------------- ���������� ���������� --------------------------------
//------------------- ��������� ������� ------------------------------------
void Comm_LocalControlInit(TCommMPU *);
void Comm_LocalControlUpdate(TCommMPU *);
Uns Comm_LocalKeyUpdate(TCommMPU *);
Uns Comm_LocalButtonUpdate(TCommMPU *);

#endif
