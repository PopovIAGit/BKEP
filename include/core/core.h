/*======================================================================
��� �����:          core.h
�����:
������ �����:
���� ���������:
��������:
�������� �������� � ������� ����

Inputs 
Outputs
========================================================================*/

#ifndef CORE_
#define CORE_

//----------- ����������� ������������ ������ ------------------------------

#include "core_Protections.h"
#include "core_Menu.h"
#include "core_VlvDrvCtrl.h"
#include "core_TorqueObserver.h"
#include "g_Ram.h"
#include "g_InitHardware.h"
#include "csl\csl_memory.h"
#include "stat_fm25v10.h"
//#include "core_Commands.h"
//#include "core_MotorControl.h"
//#include "core_HeatMotor.h"

//--------------------- ���������-------------------------------------------
#define CMD_DEFAULTS_USER	0x0010	// ���������������� ��������� �� ���������
#define CMD_RES_CLB			0x0020	// ����� ���������� ������� ���������
#define CMD_RES_FAULT		0x0040	// ����� �����
#define CMD_CLR_LOG			0x0080	// ������� �������
#define CMD_RES_CYCLE		0x0100	// ����� �������� ������
#define CMD_PAR_CHANGE		0x0200	// ��������� ����������
#define CMD_DEFAULTS_FACT 	0x0400	// ��������� ��������� �� ���������

#define START_DELAY_TIME		(2.000 * Prd50HZ)		// ����������� ������� ����� ����� ��������� � ����. ��������
//--------------------- ������� --------------------------------------------
//-------------------- ��������� -------------------------------------------

//================== ���� ============================

// ������ ������ ��� �������� ���������� ��������
typedef enum {
	wmStop       = 1,					// ����� ����
	wmMove     	 = 2					// ����� ��������
} TWorkMode;

// ��������� ��� ��������� ���������� ��������
typedef struct _TDmControl {
	TWorkMode	WorkMode;				// ����� ������
	Int		 	RequestDir;				// �������� ����������� ��������
	LgInt	 	TargetPos;				// ������� ���������
	Uns		 	TorqueSet;				// ������� �������
	Uns		 	TorqueSetPr;			// ������� ������� � %
	Uns 		BreakFlag;				// ���� ������������ ����������� ����������
	Uns 		OverWayFlag;			// ���� ������������ ��� ���������� �� ����������
} TDmControl;

typedef struct {
	// ---
	TStatusReg 			Status;			// ������ ������
	TStatusReg 			TestStatus;		// ���������� ��� ��������
	// ---
	TCoreProtections	Protections;	// ������
	// ---
	//TCoreParams		params;			// ���������
	// ---
	TCoreMenu			menu;			// ����
	// ---
	TTorqObs			TorqObs;		// ������ �������
	// ---
	TCoreVlvDrvCtrl		VlvDrvCtrl;		// ���������� ���������
	// ---
	TDmControl			MotorControl;	// ���������� ����������
} TCore;

//------------------- ���������� ���������� --------------------------------
//------------------- ��������� ������� ------------------------------------

void Core_Init(TCore *);
void Core_CmdUpdate(TCore *);
void Core_StatusUpdate(TCore *);			//

void StopPowerControl(void);					// �������� ��� �����
void StartPowerControl(TValveCmd ControlWord);  // �������� ��� ������

void Core_CalibControl(TCore *);			// ���������� �����������
void Core_CalibStop(TCore *);				// ���� �� ����������
void Core_DefineCtrlParams(TCore *);		// ������ �������������� ���������� (������� - ��������, ��������, ����������. ����������� ������� ���� � �.�)
void Core_ControlMode(TCore *);				// ����� ������

extern TCore g_Core;
extern volatile Uns setTorque;
#endif // CORE_
