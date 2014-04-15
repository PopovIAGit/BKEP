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

	//настраиваем один и тот же физический канал для драйвера Bluetooth и Modbus
	g_Comm.Bluetooth.HardWareType = g_Comm.mbBt.Params.HardWareType;	// физика
	g_Comm.Bluetooth.ChannelID    = g_Comm.mbBt.Params.ChannelID;		// индекс канала
	g_Comm.Bluetooth.BaudRate	  = g_Comm.mbBt.Params.BaudRate;		//
	g_Comm.Bluetooth.UartBaud	  = g_Comm.mbBt.Params.UartBaud;		//
	g_Comm.Bluetooth.Mode	  	  = g_Comm.mbBt.Params.Mode;			//
	g_Comm.Bluetooth.Parity	   	  = g_Comm.mbBt.Params.Parity;			//


	SerialCommInit(&g_Comm.mbAsu);
	SerialCommInit(&g_Comm.mbShn);
	SerialCommInit(&g_Comm.mbBt);


}
//---------------------------------------------------
void Comm_Update(TComm *p)
{
	//	КОМАНДЫ С МПУ !!!
	//Comm_LocalControlUpdate(&p->localControl); // Ф-я обр-ки сигналов с ПДУ
	ModBusUpdate(&g_Comm.mbAsu); // slave канал связи с верхним уровнем АСУ
	ModBusUpdate(&g_Comm.mbShn); // master канал связи с устройством плавного пуска

	SciMasterConnBetweenBlockUpdate(&g_Comm.mbBkp);// master канал связи с

	BluetoothWTUpdate(&g_Comm.Bluetooth); //драйвер Bluetooth
	ModBusUpdate(&g_Comm.mbBt);  // slave
	//ImUpdate(&Im);

	//ModBusUpdate(&g_Comm.mbBt);  // slave
	//ModBusUpdate(&g_Comm.mbBkp); // master
	//SerialCommUpdate(&Mb);
}

//---------------------------------------------------
