/*======================================================================
Имя файла:          core.c
Автор:
Версия файла:
Дата изменения:
Описание: Работа ЯДРА
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
	// Значения параметров присваиваются
	Core_MenuInit(&p->menu);
	//Core_CommandsInit(&p->commands);
	//Core_ProtectionsInit(&p->protections); // Иницилизация аварий
	//Core_MotorControlInit(&g_Sifu);// Иницилизация
	//Core_HeatMotorMotorInit(&p->heatMotor);
	//Core_TorqueInit(&p->torque);

}
//-------СТОП ПО КОМАНДЕ--------------------------------------------
void StopByCommand(TCore *p, Int brake)
{	
	
}
//-------СТОП ПО АВАРИИ---------------------------------------------
void StopByFault(TCore *p)
{

}
//------СТАРТ ПО КОМАНДЕ-----------------------------
void StartByCommand(TCore *p, Int direction, Uns command)
{

}
//------------ОБРАБОТКА КОМАНД----------------------- // 50 Гц
void Core_CmdUpdate(TCore *p)	//вызываем нужную ф-ию, в зависимости от текущего статуса
{

}
//---------------ОБРАБОТКА СТАТУСОВ---------------------------
void Core_StatusUpdate(TCore *p) // 50 Гц
{
	
}
//--------------------------------------------------------




