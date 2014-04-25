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

//#include "core_Protections.h"
#include "core_Menu.h"
#include "g_Ram.h"
#include "g_InitHardware.h"
#include "csl\csl_memory.h"
#include "stat_fm25v10.h"
//#include "core_Commands.h"
//#include "core_MotorControl.h"
//#include "core_HeatMotor.h"

// ��������� ��� ��������
typedef enum {
	brakeOff 	= 0,	// �� ���������
	brakeOn		= 1		// ���������
} TBrake;
//--------------------------------------------------------
//��������� 
typedef struct {
	Uns		timerMaxTorque;		// ������ ��� ������������ ��������
	Uns 	timeOutMaxTorque;	// ������ ������� ��� ������������ ��������
	Uns 	timerNoMove;		// ������ ��� ���������� �������� (� ����� � � ��������)
	Uns 	timeOutNoMove;		// ������ ������� ��� ���������� �������� (� ��������)
	Uns		timerBraking;		// ������ ��� ����������
	Uns		timeOutBraking;		// ������ ������� ��� ����������
	Uns		brakingLevel;		// ������� ����������
	Uns		brakingEnable;		// ���������� ���/����
	Uns		uporTimer;			// ?????? ??? ?????
	Uns		uporEnable;			// ���� ���/����
	Uns		speed2uporTimer;	//
	Uns   	DynBrakeComplite;	//��� ���������� ������������� ����������
	Uns		braking;			//���������������. [0, 1]
	Uns		speed_ctrl;			//����� ���������� ���������
	Int		startTimer;			//��� ����� �� ������� ������ ����
} TCoreParams;
//--------------------------------------------------------
//================== ���� ============================
//--------------------------------------------------------

typedef struct {
	// ---
	//TCoreStatus 		status;			// ������ ������
	// ---
	//TCoreProtections	protections;	// ������
	// ---
	//TCoreParams			params;			// ���������
	// ---
	TCoreMenu			menu;			// ����
	// ---
	/*TTorque				torque;			// ������ �������
	// ---
	TCoreCommands		commands;		// �������
	// ---
	THeatMotor			heatMotor;		// ������ ���������*/
	Uns					timerHeatMotor;	// ������ ������� ���������
} TCore;

//--------------------------------------------------------

void Core_Init(TCore *);
void Core_CmdUpdate(TCore *);
void Core_StatusUpdate(TCore *);
__inline void Core_StopHeatMotor(TCore *);
Bool HeatMotorControl(TCore *);
void StopByCommand(TCore *, Int);
void StopByFault(TCore *);
void StartByCommand(TCore *, Int, Uns);

extern TCore g_Core;
extern volatile Uns setTorque;
#endif // CORE_

