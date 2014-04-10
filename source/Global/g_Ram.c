/*======================================================================
Имя файла:          g_Ram.c
Автор:
Версия файла:
Дата изменения:
Описание:
Обновление переменной g_Ram
======================================================================*/

#include "g_Ram.h"
#include "core.h"
//#include "drive.h"
//#include "comm.h"
//#include "peref.h"
//#include "stat.h"

//extern LgUns KakByMetkiEncodera;

Uns startTimer = 20, startcomplite = 0;

Uns predOnDU=0;
Uns predOnMU=0;
Uns prevCountRunReg=0;
Uns predTaskPOS_PR=0;

Uns predNoMoveTime1=0;
Uns predNoMoveTime2=0;
Uns predNoMoveTime3=0;
//Uns predB31_Reg=0;
Uns predBreakCloseTorque=0;
Uns predTaskZoneStopReg=0;
Uns TaskTorqueStartCloseReg=0;
Uns TaskTorqueRunCloseReg=0;
Uns TaskTorqueUporCloseReg=0;
Uns TaskTorqueStartOpenReg=0;
Uns TaskTorqueRunOpenReg=0;
Uns TaskTorqueUporOpenReg=0;
Uns predStartCloseTorque=0;
Uns predRunCloseTorque=0;
Uns predUporCloseTorque=0;
Uns predStartOpenTorque=0;
Uns predRunOpenTorque=0;
Uns predUporOpenTorque=0;
Uns predTaskZoneStartReg=0;
Uns MAX_TORQUE=0;

//---------------------------------------------------
void g_Ram_Init(TRam *p)
{

}
//---------------------------------------------------
void g_Ram_Update(TRam *p)
{
	
}
//---------------------------------------------------
void g_Ram_SetTorque(void)
{

}
// Обновление значение по требованию
void RefreshParams(Uns addr)
{

}
//---------------------------------------------------
Int MinMax3IntValue (Int val1, Int val2, Int val3)
{
	Int minMax = 0;
	if (((val1 <= val2)&&(val1 >= val3))||((val1 >= val2)&&(val1 <= val3)))
		minMax = val1;
	if (((val2 <= val1)&&(val2 >= val3))||((val2 >= val1)&&(val2 <= val3)))
		minMax = val2;
	if (((val3 <= val1)&&(val3 >= val2))||((val3 >= val1)&&(val3 <= val2)))
		minMax = val3;
	return minMax;
}
//---------------------------------------------------
Int Max3Int (Int val1, Int val2, Int val3)
{
	Int Max = 0;
	if ((val1 >= val2)&&(val1 >= val3))
		Max = val1;
	if ((val2 >= val1)&&(val2 >= val3))
		Max = val2;
	if ((val3 >= val2)&&(val3 >= val1))
		Max = val3;
	return Max;
}
//--------------------------------------------------------

