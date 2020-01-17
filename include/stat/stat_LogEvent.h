#ifndef _LOG_EV_H
#define _LOG_EV_H

#include "std.h"

#define NUMBER_OF_LOGS				4//ma LOgSim 3
#define LOG_EVENT_TYPE				0x01
#define LOG_CMD_TYPE				0x02
#define LOG_PARAMS_TYPE				0x03
#define LOG_SIM_TYPE				0x04

#define LOGS_CNT				4//ma LOgSim ���� 3									// ���������� �������� ����������

#define LOG_EV_START_ADDR		15000								// ��������� ����� ������� ������ �������� ������ �������
#define LOG_EV_DATA_CNT			20		//sizeof(TGroupE)			// ���������� ����� � 1 ������ �������� ������ �������
#define LOG_EV_CNT				500									// ���������� ������� � ������ �������
#define LOG_EV_DATA_CELL		1									// ���������� �������� � �������� ������ �������

#define LOG_EV_BUF_START_ADDR	0									// ��������� ����� ������ �������
#define LOG_EV_BUF_DATA_CNT		sizeof(TRamLogBuff)					// ���������� ����� � ����� ������ ������
#define LOG_EV_BUF_DATA_CELL	5									// ���������� ����� ������ � ����� ������

#define LOG_EV_BUF_CELL_COUNT	5									// ���������� �������� � ������ ������� �������

#define PREV_LEV_INDEX		0										//������� ��� ����������� ����������� ������ ��������
#define PREV_LCMD_INDEX		1
#define PREV_LPAR_INDEX		2
//ma LogSim
#define PREV_LSIM_INDEX		3

#define LOG_EV_RAM_DATA_ADR		GetAdr(ramGroupE)				// ����� ������ ������� � Ram
#define LOG_EV_RAM_DATA_LADR	(LOG_EV_RAM_DATA_ADR * LOG_EV_DATA_CNT)
#define LOG_EV_SIZE				(LOG_EV_CNT * LOG_EV_DATA_CNT * LOG_EV_DATA_CELL)

// 7500 �����
#define LOG_CMD_START_ADDR		2000								// ��������� ����� ������� ������
#define LOG_CMD_DATA_CNT		7 //���������� 1 ���� ���������								//���� 5 � ���������� ���������� ������� ����������								// ���������� ������������ � ������ �����
#define LOG_CMD_CNT				800								    // ������� ������� ������ (���������� �������)

// 2500 �����
#define LOG_PARAM_START_ADDR	11000								// ��������� ����� �������� ��������� ����������
#define LOG_PARAM_DATA_CNT		5									// ���������� ������������ ����� (���� ��������� �������: ����, ����� � �.�.)
#define LOG_PARAM_CNT			500									// ������� ������� ��������� ����������

// 10 ����� //ma LogSIM
#define LOG_SIM_START_ADDR		14000								// ��������� ����� ������� ������
#define LOG_SIM_DATA_CNT		13									// ���������� ������������ � ������ �����
#define LOG_SIM_CNT				10		   						    // ������� ������� ������ (���������� �������)


#define FAULTS_COUNT_DEF			4				// ���������� ��������� ��������
#define LOG_EV_BUF_DATA_COUNT		20				// ���������� ������� ������ ������� �������
#define LOG_CMD_BUF_DATA_COUNT		7//6				// ���������� ������� ������ ������� ������
#define LOG_PARAM_BUF_DATA_COUNT	5				// ���������� ������� ������ ������� ��������� ����������
//ma LogSIM
#define LOG_SIM_BUF_DATA_COUNT		13				// ���������� ������� ������ ������� SimID
#define MB_PARAM_BUF_COUNT			5

// ��������� ��� ������ � �������� �������
typedef struct
{
	Bool Enable;							// ���������� ������ ������� �������
	Bool ExecFlag;							// ���� ������������ ������ �������
	Bool WriteFlag;							// ���� ���������� ������ ������� � ������
	Bool EventFlag;							// ���� ������������� �������
	Bool FirstEvent;						// ���� ������� ������� ����� ��������� �������
	Uns  FaultsCount;						// ���������� ��������� ��������������
	Uns  FaultsState[FAULTS_COUNT_DEF];		// ����� ��������� ��������������
	Uns  FaultsFiltr[FAULTS_COUNT_DEF];		// ����� ���������� ��������� ��������������
	Uns  FaultsFiltrPrev[FAULTS_COUNT_DEF];	// ����� ���������� ��������� ��������������
	Uns *Time;
	Uns *Date;
	Uns *Status;
	Int *PositionPr;
	Uns *Torque;
	Uns *Ur;
	Uns *Us;
	Uns *Ut;
	Uns *Iu;
	Uns *Iv;
	Uns *Iw;
	Int *Temper;
	Uns *Inputs;
	Uns *Outputs;
	Uns *Seconds;
	Uns Data[LOG_EV_BUF_DATA_COUNT];		// ����� ������� �������
} TLogEvent, *pTLogEvent;



typedef struct
{
	Bool Enable;							// ���� ���������� ������ �������
	Bool ExecFlag;							// ���� ������������ ������ �������
	Bool WriteFlag;							// ���� ���������� ������ �������
	Uns *Time;								// ��������� �� �����
	Uns *Date;								// ��������� �� ����
	Uns *Seconds;							// ��������� �� �������
	Uns *StatusReg;							// ��������� �� ��������� ������� �����
	Uns CmdReg;								// ������� ������
	Uns CmdRegPrev;							// ���������� �������� �������� ������ 
	Uns *StatDigOut;							// ������� ��������� ���������� �������
	Uns *Position;
	Uns Data[LOG_CMD_BUF_DATA_COUNT];		// ����� ������� ������
} TLogCmd, *pTLogCmd;

typedef struct
{
	Bool Enable;							// ���� ���������� ������ �������
	Bool ExecFlag;							// ���� ������������ ������ �������
	Bool WriteFlag;							// ���� ���������� ������
	Uns *Time;								// ��������� �� �����
	Uns *Date;								// ��������� �� ����
	Uns *Seconds;							// ��������� �� �������
	Uns Addr;								// ����� ���������
	Uns NewValue;							// ����� �������� ���������
//	Bool MbChangeParam;
	Uns MbIndex;
	Uns Data[LOG_PARAM_BUF_DATA_COUNT];		// ����� �������
	Uns MbBuffer[MB_PARAM_BUF_COUNT];		// ����� ���������� ������
} TLogParam, *pTLogParam;

typedef struct
{
	Bool Enable;							// ���� ���������� ������ �������
	Bool ExecFlag;							// ���� ������������ ������ �������
	Bool WriteFlag;							// ���� ���������� ������ �������
	Uns *Time;								// ��������� �� �����
	Uns *Date;								// ��������� �� ����
	Uns *Seconds;							// ��������� �� �������
	Uns NewSimID[10];							// ������� ������
	Uns Data[LOG_SIM_BUF_DATA_COUNT];		// ����� ������� ������
} TLogSim, *pTLogSim;

void LogEventUpdate(TLogEvent *);					// ������� ������������ ������� �������
void LogCmdUpdate(TLogCmd *);				// ������� ������������ ������� ������
void LogParamUpdate(TLogParam *);			// ������� ������������ ������� ����������
//ma LogSIM
void LogSimUpdate(TLogSim *);			// ������� ������������ ������� SimID

#endif





