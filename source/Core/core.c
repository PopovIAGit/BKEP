/*======================================================================
��� �����:          core.c
�����:
������ �����:
���� ���������:
��������: ������ ����
======================================================================*/

#include "core.h"
//#include "peref.h"
#include "g_Structs.h"
#include "g_Ram.h"
//#include "comm.h"
//#include "stat.h"

#define IsAlarmsExist (	p->protections.processAlarmsExist|	\
						p->protections.netAlarmsExist|		\
						p->protections.loadAlarmsExist|		\
						p->protections.deviceAlarmsExist )

#define 	NETALARM_MASK	0x1FF
#define 	LOADALARM_MASK	0x7

TCore	g_Core;

//---------------------------------------------------
void Core_Init(TCore *p)
{
	// �������� ���������� �������������
	Core_MenuInit(&p->menu);
	//Core_CommandsInit(&p->commands);
	//Core_ProtectionsInit(&p->protections); // ������������ ������
	//Core_MotorControlInit(&g_Sifu);// ������������
	//Core_HeatMotorMotorInit(&p->heatMotor);
	//Core_TorqueInit(&p->torque);

}
//-------���� �� �������--------------------------------------------
void StopByCommand(TCore *p, Int brake)
{	
	
}
//-------���� �� ������---------------------------------------------
void StopByFault(TCore *p)
{

}
//------����� �� �������-----------------------------
void StartByCommand(TCore *p, Int direction, Uns command)
{

}
//------------��������� ������----------------------- // 50 ��
void Core_CmdUpdate(TCore *p)	//�������� ������ �-��, � ����������� �� �������� �������
{

}
//---------------��������� ��������---------------------------
void Core_StatusUpdate(TCore *p) // 50 ��
{
	
}
//--------------------------------------------------------




