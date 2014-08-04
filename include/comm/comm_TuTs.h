/*======================================================================
��� �����:          communication_TU_TS.h
�����:
������ �����:		1.0
���� ���������:
��������:
������������ ���� ������ ��������� �������� �������������� � ����������������
======================================================================*/
#ifndef _TU_TS
#define _TU_TS

//----------- ����������� ������������ ������ ------------------------------
#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif


#define DIN_OPEN_BIT		0
#define DIN_CLOSE_BIT		1
#define DIN_STOP_BIT		2
#define DIN_MU_BIT		 	3
#define DIN_RESETALARM_BIT 	4
#define DIN_READYTU_BIT 	5
#define DIN_DU_BIT 			6

#define OPEN_BIT			BIT0
#define CLOSE_BIT			BIT1
#define STOP_BIT			BIT2
#define MU_BIT		 		BIT3
#define RESETALARM_BIT 		BIT4
#define READYTU_BIT 		BIT5
#define DU_BIT 				BIT6

#define DOUT_OPEN_BIT		0
#define DOUT_CLOSE_BIT		1
#define DOUT_OPENING_BIT	2
#define DOUT_CLOSING_BIT	3
#define DOUT_DEFECT_BIT 	4
#define DOUT_MUFTA_BIT 		5
#define DOUT_ALARM_BIT 		6
#define DOUT_MUDU_BIT 		7


// ��� �������� �������
typedef enum {
	it24  =  0,		// 0 ��� ������� 24 �
	it220 = 1 		// 1 ��� ������� 220 �
}TInputType;

typedef struct {
	Int 	inputDIN;
	Uns 	*outputReg;
	Int		*enableLevel220;
	Int		*disableLevel220;
	Int		*enableLevel24;
	Int		*disableLevel24;
	Uns		timeOut;
	Uns		timer;
} TDIN;

// ��������� ���������� ������
typedef union _TInputReg {
  Uns all;
	struct {
		Uns Open:1;			// 0	����
		Uns Close:1;		// 1	�������
		Uns Stop:1;			// 2	�������
		Uns Mu:1;			// 3	������
		Uns ResetAlarm:1;	// 4	������
		Uns ReadyTu:1;		// 5	������
		Uns Du:1;			// 6	������
		Uns NoUse:9;		// 7-15	�� ������������
	} bit;
} TInputReg;


// ��������� ���������� ������� ��
typedef union _TOutputReg {
	Uns all;
	struct {
		Uns Opened:1;		// 0	�������
		Uns Closed:1;		// 1	�������
		Uns Opening:1;		// 2	�����������
		Uns Closing:1;		// 3	�����������
		Uns Defect:1;		// 4	�������
		Uns Mufta:1;		// 5	�����
		Uns Alarm:1;		// 6	������
		Uns MUDU:1;			// 7	��/��
		Uns Fault:1;		// 8	������������� 
		Uns Rsvd:7;			// 9-15  ������
	} bit;
} TOutputReg;

typedef struct TDigitalInterface
{
	TInputReg		Inputs;			// ��������� ���������� ������
	TOutputReg		Outputs;		// ��������� ���������� �������
	TDIN			dinStop;		// ���������� ���� "����"
	TDIN			dinClose;		// ���������� ���� "�������"
	TDIN			dinOpen;		// ���������� ���� "�������"
	TDIN			dinResetAlarm;	// ���������� ���� "����� ������"
	TDIN			dinPredReady;	// ���������� ���� "���� ����������� ��"
	TDIN			dinMu;			// ���������� ���� "��"
	TDIN			dinDu;			// ���������� ���� "��"
	Uns				*TypeVoltSignal;	// 24 / 220
	Uns				*TypeLogicSignal;// ��� ������� �� ������ ��������� �����������/��������� - 0/1
} TDigitalInterface;

// ����� ���������� ������
typedef union _TUInvert {
	Uns all;
	 struct {
		Uns Open:1;       	// 0     �������
		Uns Close:1;      	// 1     �������
		Uns Stop:1;       	// 2     ����
		Uns Mu:1;			// 3	 ��
		Uns Du:1;			// 4	 ��
		Uns Rsvd:11;      	// 5-15  ������
	 } bit;
} TUInvert;

// ����� ���������� �������
typedef union _TSInvert {
	Uns all;
	 struct {
	 	Uns Dout0:1;		// 0 - �������
		Uns Dout1:1;		// 1 - �������
		Uns Dout2:1;		// 2 - �����
		Uns Dout3:1;		// 3 - ������
		Uns Dout4:1;		// 4 - �����������/ ���� �������
		Uns Dout5:1;		// 5 - �����������/ ���
		Uns Dout6:1;		// 6 - ��/�� /	���
		Uns Dout7:1;		// 7 - �������/ ---
		Uns Rsvd:8;      	// 8-15  ������
	 } bit;
} TSInvert;


void Comm_TuTsInit (TDigitalInterface *);
void Comm_TuTsUpdate (TDigitalInterface *);
void DIN_Update_On (TDIN *p,Uns *Type,Uns bit);
void DIN_Update_Off (TDIN *p,Uns *Type,Uns bit);


#ifdef __cplusplus
}
#endif // extern "C"

#endif	//_TU_TS
