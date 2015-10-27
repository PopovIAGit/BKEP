/*======================================================================
��� �����:          comm_LocalControl.h
�����:
������ �����:      	1.1
���� ���������:		11.06.2015
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
#define BTN_STOP		(BTN_STOPDU_BIT|BTN_STOPMU_BIT)

#define BTN_RESET_ALARM GpioDataRegs.GPBDAT.bit.GPIO47


//--------------------- ������� --------------------------------------------
#define BTN_ENABLE_TIME			(0.300 * Prd50HZ)
#define BTN_STOP_ENABLE_TIME	(0.100 * Prd50HZ)
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

typedef struct {
	Uns					upLevel;		// ������� ������������
	Uns 				timer;			// ������
	Uns 				timeout;		// ����� ������������
} TButtonsParams;

// ��������� ��� ������ ���
typedef struct
{
	THallBlock			*inputBCP_Data;	 // ������ � ���
	TButtonsParams		btn1Param;		 // ��������� ������� 1
	TButtonsParams		btn2Param;		 // ��������� ������� 2
	TButtonsParams		btn3Param;		 // ��������� ������� 3
	TButtonsParams		btn4Param;		 // ��������� ������� 4

} TCommMPU;
//------------------- ���������� ���������� --------------------------------
//------------------- ��������� ������� ------------------------------------
void Comm_LocalControlInit(TCommMPU *);
void Comm_LocalControlUpdate(TCommMPU *);
Uns Comm_LocalButtonUpdate(TCommMPU *);

#endif
