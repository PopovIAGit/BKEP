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
#include "stat_fm25v10.h"

TFM25V10 Eeprom1;
TFM25V10 Eeprom2;

#define IsAlarmsExist (	p->protections.processAlarmsExist|	\
						p->protections.netAlarmsExist|		\
						p->protections.loadAlarmsExist|		\
						p->protections.deviceAlarmsExist )

#define 	NETALARM_MASK	0x1FF
#define 	LOADALARM_MASK	0x7

TCore	g_Core;

//����� ���������
__inline void Eeprom1CsSet(Byte Lev)  {SC_EEPROM1 = !Lev;}
__inline void Eeprom2CsSet(Byte Lev)  {SC_EEPROM2 = !Lev;}

//---------------------------------------------------
void Core_Init(TCore *p)
{
	// ������������� �����
	Eeprom1.CsFunc = &Eeprom1CsSet;
	FM25V10_Init(&Eeprom1);
	Eeprom2.CsFunc = &Eeprom2CsSet;
	FM25V10_Init(&Eeprom2);

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




