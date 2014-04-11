/*======================================================================
Имя файла:          comm.c
Автор:
Версия файла:
Дата изменения:
Описание:
Основной модуль обработки коммуникаций
======================================================================*/

#include "comm.h"

TComm	g_Comm;
//---------------------------------------------------
void Comm_Init(TComm *p)
{
	// Пульт местного управления:
	//Comm_LocalControlInit (&p->localControl);

	SciMasterConnBetweenBlockInit(&g_Comm.mbBkp);

	InitChanelAsuModbus(&g_Comm.mbAsu);
	InitChanelShnModbus(&g_Comm.mbShn);
	InitChanelBtModus(&g_Comm.mbBt);

	SerialCommInit(&g_Comm.mbAsu);
	SerialCommInit(&g_Comm.mbShn);
	SerialCommInit(&g_Comm.mbBt);


}
//---------------------------------------------------
void Comm_Update(TComm *p)
{
	//	КОМАНДЫ С МПУ !!!
	//Comm_LocalControlUpdate(&p->localControl); // Ф-я обр-ки сигналов с ПДУ
}

//---------------------------------------------------
