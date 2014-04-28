/*======================================================================
��� �����:          g_InterruptUpdate.h
�����:
������ �����:
���� ���������:
��������: 
	������������ ���� ��� ����������� ������ ������� ����� ���
������� ��������� �� ������� ����������: 18���, 2���, 200��, 50��, 10 ��.
��� ������� ������ ������, ������ �� - �� �������������.
������������ ���������� ����� (�������) ��� ������ ������ ����������:
2��� - �� ����� 4-� �����
200�� - �� �����  20-�� ����� 
50 �� - �� �����  80-�� �����
10 �� - �� �����  200-� �����
======================================================================*/

#include "config.h"	

#include "core.h"
#include "comm.h"
#include "g_Ram.h"
//#include "drive.h"
#include "peref.h"
#include "stat.h"
#include "csl/csl_dlog.h"

//--------���������� ��������------------------------------------------------

// ������ �������������� ������� � ��������� �� �������,
// ��������� ������� � ��������� �� �������� �������
#define PrdElemInit(Name, Argument) \
	{ (TPrdFunc)(Name), (void *)(Argument) }

// ������ �������� ���������� ��������� �������
#define TaskCount(List)	(sizeof(List) / sizeof(TPeriodicalFunction))

#define TASK_COUNT_MAX_2kHZ  10		//������������ ���-�� ����� ��� ������� 2 ���
#define TASK_COUNT_MAX_200HZ 20		//������������ ���-�� ����� ��� ������� 200 ��
#define TASK_COUNT_MAX_50HZ  80		//������������ ���-�� ����� ��� ������� 50 ��
#define TASK_COUNT_MAX_10HZ  200	//������������ ���-�� ����� ��� ������� 10 ��

//---------�����������--�����--������----------------------------------------

typedef void (*TPrdFunc)(void *Data);	// ����������� ���� "��������� �� �������"

typedef struct _TPrdElem				// ����������� ���� ������ ���������, ������� �������� ����:
{
	TPrdFunc Func;						// - ��������� �� �������;
    void *Data;							// - ��������� �� �������� �������
} TPeriodicalFunction;					// ���������� ��������� ���������������� ����

//---------���������---�������-----------------------------------------------

        void task1				(void);				//������������� �������
        void task1_10Hz			(void);		//������������� �������, ����������� �� ������� 10 ��
        void task1_18Hz			(void);		//������������� �������, ����������� �� ������� 18 ��
extern  void InterruptInit		(void);		//������� ������������� ����������
extern  void InterruptUpdate	(void); 		//������� ��������� ����������

//-------����������--����������----------------------------------------------

int     count18kHz,             //������� ���������� �������� 18 ���
        count2kHz,              //������� ���������� �������� 2  ���
        count200Hz,             //������� ���������� �������� 200 ��
        count50Hz,              //������� ���������� �������� 10  ��

        tskCnt2kHz,             //������� ����������� ����� 2 ���. �� �� ����� ����������� ������
        tskCnt200Hz,            //������� ����������� ����� 200��. �� �� ����� ����������� ������
        tskCnt50Hz,             //������� ����������� ����� 50 ��. �� �� ����� ����������� ������
        tskCnt10Hz,             //������� ����������� ����� 10 ��. �� �� ����� ����������� ������

        tskMaxCnt18kHz,			//���������� ����� ��� ������� 18 ���
        tskMaxCnt2kHz,          //���������� ����� ��� ������� 2  ���
        tskMaxCnt200Hz,         //���������� ����� ��� ������� 200 ��
        tskMaxCnt50Hz,          //���������� ����� ��� ������� 50  ��
        tskMaxCnt10Hz;          //���������� ����� ��� ������� 10  ��

LgUns  X, X10, X18;

// ������������������ ���������� �� ������� 18 ���

// ================================ 18 ��� ==================================

TPeriodicalFunction Task18kHz[] =         
{
	PrdElemInit(Peref_18kHzCalc,						&g_Peref),
	PrdElemInit(MonitorUpdate,							NULL),
	//PrdElemInit(AvagoEncoderAEAT84ADCalc,				&g_Peref.position),
	PrdElemInit(task1,	NULL)
};

//���� ��������� ������� ��� ��������������� ������

// ================================ 2 ��� ==================================

TPeriodicalFunction Task2kHz[] =          //�� ����� 4-� �����
{
	//PrdElemInit(Core_ProtectionsRefresh,	&g_Core.protections),
	PrdElemInit(SerialCommTimings,						&g_Comm.mbAsu),
	//PrdElemInit(SerialCommTimings,						&g_Comm.mbShn),
	PrdElemInit(SerialCommTimings,						&g_Comm.mbBt),
	PrdElemInit(FM25V10_Update,							&Eeprom1),
	PrdElemInit(FM25V10_Update,							&Eeprom1),
	PrdElemInit(SciMasterConnBetweenBlockCommTimer,		&g_Comm.mbBkp),
	//PrdElemInit(SciMasterConnBetweenBlockUpdate,		&g_Comm.mbBkp),

};
// ================================ 200 �� ==================================

TPeriodicalFunction Task200Hz[] =       	//�� �����  20-�� �����
{
//	PrdElemInit(Peref_DisplayUpdate, 					&g_Peref.display),
	PrdElemInit(Peref_CalibUpdate, 						&g_Peref.Position),
	PrdElemInit(DataBufferPre,							&g_Stat),
	PrdElemInit(LogEvControl,							&g_Stat),
	PrdElemInit(LogCmdControl,							&g_Stat),
	PrdElemInit(LogParamMbExec,							&g_Stat),
	PrdElemInit(LogParamControl,						&g_Stat),
};

// ================================ 50 �� ==================================

TPeriodicalFunction Task50Hz[] =        //�� �����  80-�� �����
{
	PrdElemInit(Peref_50HzCalc,					&g_Peref),
	PrdElemInit(Peref_SpeedCalc, 				&g_Peref.Position),
	PrdElemInit(ContactorControlUpdate, 		&g_Peref.ContactorControl),
	PrdElemInit(Core_DefineCtrlParams, 			&g_Core),
	PrdElemInit(Core_ControlMode, 				&g_Core),
	PrdElemInit(Core_TorqueCalc, 				&g_Core.TorqObs),
	PrdElemInit(Core_ValveDriveUpdate, 			&g_Core.VlvDrvCtrl),
	PrdElemInit(Core_CalibControl, 				&g_Core),
	/*PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.protections.overHeatBlock),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.protections.underColdBlock),
	PrdElemInit(Core_ProtectionsAlarmUpdate,	&g_Core.protections.overTemper1),
	PrdElemInit(EleSyEncoderCalc,				&g_Peref.position.encoder),*/
	//PrdElemInit(Comm_Update,					&g_Comm),
	/*PrdElemInit(Core_CommandControl,			&g_Core.commands),
	PrdElemInit(Core_CommandsUpdate,			&g_Core.commands),
	PrdElemInit(Core_CmdUpdate,					&g_Core),
	PrdElemInit(Peref_ClockControl,				&g_Peref.clock),
	PrdElemInit(Core_StatusUpdate,				&g_Core)*/
	PrdElemInit(GetCurrentCmd,							&g_Stat),
	PrdElemInit(Core_MenuDisplay,						&g_Core.menu),
};

// ================================ 10 �� ==================================

TPeriodicalFunction Task10Hz[] =        //�� �����  200-� �����
{
	PrdElemInit(Peref_Calibration, 		&g_Peref.Position),
	PrdElemInit(CalcClbCycle, 			&g_Peref.Position),
	/*PrdElemInit(Core_ProtectionsUpdate,		&g_Core.protections),
	PrdElemInit(g_Ram_Update,				&g_Ram),
//	PrdElemInit(Peref_SpeedCalc,			&g_Peref.position),
	PrdElemInit(ADT7301_Update,				&g_Peref.tempSensor),*/
	//PrdElemInit(ADT75_Update,				NULL),
//	PrdElemInit(Drive_Update,				&g_Drive),
	//PrdElemInit(Peref_LedsUpdate,			&g_Peref.leds),
	//PrdElemInit(Peref_TenControl,			NULL),
	//PrdElemInit(Peref_10HzCalc,				&g_Peref),
	PrdElemInit(Peref_Calibration, 						&g_Peref.Position),
	PrdElemInit(BluetoothTimer,							&g_Comm.Bluetooth),
	PrdElemInit(task1_10Hz,								NULL)
};
//------------����� �����-----------------------------------------------



