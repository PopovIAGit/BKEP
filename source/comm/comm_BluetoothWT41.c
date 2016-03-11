/*#include "comm_BluetoothWT41.h"
#include "structs.h"
#include "interface.h"*/

#include "config.h"
#include "comm.h"
#include "g_Ram.h"
#include "stat.h"

#define BT_TIMER_SCALE			PRD3
#define BT_TIMER				1.00 * BT_TIMER_SCALE

Byte RxState = 0;
//для драйвера Bluetooth
char StrDev[] = {"SET BT NAME BKEP 000000\r\n"};
Uns TestCount=0;

//void InitChanelBt(TBluetoothHandle);
void SendOneString(TBluetoothHandle, char *String);
void SendTwoString(TBluetoothHandle, char *FirstString, char *SecondString);
void SendCommandOne(TBluetoothHandle, char *ComStr);
void SendCommandTwo(TBluetoothHandle, char *ComStr, char *AddStr);

void ClearInitValues(TBluetoothHandle);
void ClearValues(TBluetoothHandle);
__inline void RxCommandMode(TBluetoothHandle);
__inline void RxDataMode(TBluetoothHandle,TMbHandle);
Bool CheckString(TBluetoothHandle, char *Str);
Bool CheckCommError(TBluetoothHandle);

void EnableBtRx(void);
void EnableBtTx(void);
Uns ReceiveBtByte(void);
void TransmitBtByte(Uns Data);
void TransmitBtByteIM(Uns Data);
/*
void EnableBtRx(TBluetoothHandle);
void EnableBtTx(TBluetoothHandle);
Byte ReceiveBtByte(TBluetoothHandle);
void TransmitBtByte(TBluetoothHandle, Byte Data);
 */
Uns count=0;

void InitChanelBt(TBluetoothHandle bPort)
{

	Uns DevFactNum = g_Ram.ramGroupC.FactoryNumber;
	Uns DevProdYear = g_Ram.ramGroupC.ProductYear;

	//char *DevName="";
	//char *DevYear="";

	bPort->TxBusy	= false;
	bPort->State	= 0;
	bPort->CmdState = 0;
	bPort->Status	= 0;
	//bPort->Mode 	= BT_COMMAND_MODE;
	bPort->StrIndex	= 0;
	bPort->Timer	= 0;
	bPort->Period	= 10;//(1.00 * PRD3)

	bPort->IsConnected 	= false;
	bPort->Error		= false;
	bPort->WaitDelayAfterConnect = 0;

	bPort->EnableRx 	= EnableBtRx;
	bPort->EnableTx 	= EnableBtTx;
	bPort->ReceiveByte 	= ReceiveBtByte;
	bPort->TransmitByte = TransmitBtByte;
	bPort->TransmitByteIM = TransmitBtByteIM;

	memset(&bPort->RxBuffer[0],	0, sizeof(BT_RX_BUFFER_SIZE));

	#if BT_DBG
	bPort->RxBytesCount = 0;
	bPort->TxBytesCount = 0;
	#endif

	//bPort->DeviceNameString = &StrDev[0];
	//bPort->DeviceAuthCodeString = BT_AUTH_CODE_STRING;

	//---
	//memset(&PiData, 0, sizeof(TPiData));
	//PiData.DiscrOut = 0;

	if (DevFactNum > 9999)
		DevFactNum = 0;
	if (DevProdYear > 99)
		DevProdYear = 0;

	DecToStr(DevFactNum, &StrDev[22], 0, 3, False, False);
	DecToStr(DevProdYear, &StrDev[18], 0, 1, False, False);
	//DevName = &StrDev[0];
	//{"SET BT NAME BKE 000000\r\n"};

	bPort->DeviceNameString = &StrDev[0];
	bPort->DeviceAuthCodeString = BT_AUTH_CODE_STRING;

	//!!!Init здесь должна быть инициализация McBSP
	/*if (bPort->HardWareType==UART_TYPE)
	{
		//SCI_init(bPort->ChannelID, bPort->UartBaud, bPort->Parity, 8);
		//SciaRegs.SCICCR.bit.STOPBITS = 0;
	} else
	if (bPort->HardWareType==MCBSP_TYPE){
		//InitMcbspa();
	}*/
	bPort->EnableRx();

	// После включения Bluetooth запускаем таймер
	bPort->Timer = bPort->Period;

	//-------------------------------------------
	bPort->Enabled=false;
	bPort->Connect=false;
	bPort->TimerActive.Counter=0;
	bPort->TimerActive.Timeout=600*5;
	InitTimer(&bPort->TimerActive, bPort->TimerActive.Timeout);

	StartTimer(&bPort->TimerActive);
	RS485_DIR_BT = 0; //квл
	//GpioDataRegs.GPADAT.bit.GPIO27=0;

	bPort->TimerBlink.Counter=0;
	bPort->TimerBlink.Timeout=20;
	InitTimer(&bPort->TimerBlink, bPort->TimerBlink.Timeout);
}


void BluetoothActivation(TBluetoothHandle bPort)
{
	/*if (BUTTON_BLUE==1 && bPort->ModeProtocol!=0){
		bPort->ButtActivTimer++;
		if (bPort->ButtActivTimer>3)//300мс
		{
			GpioDataRegs.GPADAT.bit.GPIO27=1;
		}
	}*/
	if (BUTTON_BLUE==1 && bPort->ModeProtocol!=0){
		if (bPort->ButtActivTimer>3){
			bPort->Enabled=false;
			RS485_DIR_BT = 1;//выкл
			g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_MPU;
			g_Core.VlvDrvCtrl.EvLog.Value = CMD_OFF_BLT;
			StopTimer(&bPort->TimerActive);
			GpioDataRegs.GPADAT.bit.GPIO27=1;
			bPort->ModeProtocol = 0;
			bPort->State = 7;
			//McBsp_tx_disable(MCBSPA);
			//McBsp_rx_enable(MCBSPA);
			bPort->Function = 0;
			bPort->ButtActivTimer = 0;
			bPort->Mode = BT_COMMAND_MODE;
		}
	}

	if (BUTTON_BLUE==0 && bPort->ModeProtocol==0){
		bPort->ButtActivTimer++;
		if (bPort->ButtActivTimer>2)//300мс
		{
			GpioDataRegs.GPADAT.bit.GPIO27=0;
			bPort->Function = 1;
			if (bPort->ButtActivTimer>12) {
				bPort->Function = 2;

			}
		}
	}
	if (BUTTON_BLUE==1 && bPort->ModeProtocol==0){
		if (bPort->ButtActivTimer>0) bPort->ButtActivTimer--;
		if (bPort->Function ==1 ) bPort->Function = 3;
		if (bPort->Function ==2 ) bPort->Function = 4;
	}

	if (bPort->Function>2 && bPort->Function<5)
	{
		InitTimer(&bPort->TimerActive, bPort->TimerActive.Timeout);
		StartTimer(&bPort->TimerActive);
		bPort->Enabled=true;
		RS485_DIR_BT = 0;//вкл
		g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_MPU;
		g_Core.VlvDrvCtrl.EvLog.Value = CMD_ON_BLT;
		GpioDataRegs.GPADAT.bit.GPIO27=0;
		bPort->ButtActivTimer=0;
		bPort->ModeProtocol = 1; //Modbus
	}
	if (bPort->Function==3) bPort->Function=0;
	if (bPort->Function==4)
	{
		bPort->Function = 5;
		bPort->ButtActivTimer=0;
		bPort->ModeProtocol = 2; //IM
		bPort->BlinkConnect = true;
		StartTimer(&bPort->TimerBlink);
		g_Stat.Im.Index=0;
	}

	if (bPort->Function == 5 ) { g_Stat.Im.Index=0; bPort->BlinkConnect=true;}

	if (bPort->Enabled==true && bPort->BlinkConnect==false)
	{
		if (bPort->Connect==true)
		{
			if (BUTTON_BLUE==0 && bPort->ModeProtocol!=0)//
			{
				bPort->ButtActivTimer++;
				if (bPort->ButtActivTimer>3)//300мс
				{
					GpioDataRegs.GPADAT.bit.GPIO27=1;
				}
			}
		} else {
			if (BUTTON_BLUE==0 && bPort->ModeProtocol!=0)//
			{
				bPort->ButtActivTimer++;
				if (bPort->ButtActivTimer>3)//300мс
				{
					GpioDataRegs.GPADAT.bit.GPIO27=1;
				}
			}
			else if (bPort->ModeProtocol==1) GpioDataRegs.GPADAT.bit.GPIO27=0;
		}
	} else if (bPort->Enabled==true && bPort->BlinkConnect==true)
	{
		GpioDataRegs.GPATOGGLE.bit.GPIO27=1;
		if (!TimerPending(&bPort->TimerBlink))
		{
			StopTimer(&bPort->TimerBlink);
			bPort->BlinkConnect = false;
			bPort->Function = 0;
			g_Stat.Im.Index=0;

			if (bPort->ModeProtocol == 1) bPort->Mode = BT_DATA_MODE;
		}
	}

	if (bPort->TimerActive.Counter<(bPort->TimerActive.Timeout-1800) && bPort->Connect==true)
	{
		//TODO на время отладки отключаю
		bPort->Connect=false;
		StartTimer(&bPort->TimerActive);
		/*p->EvLog.Source = CMD_SRC_MPU;
		p->EvLog.Source = CMD_SRC_MPU;*/
	}

	if (!TimerPending(&bPort->TimerActive))
	{
		bPort->Enabled=false;
		RS485_DIR_BT = 1;//выкл
		StopTimer(&bPort->TimerActive);
		GpioDataRegs.GPADAT.bit.GPIO27=1;
		g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_BLOCK;
		g_Core.VlvDrvCtrl.EvLog.Value = CMD_OFF_BLT;
		bPort->ModeProtocol = 0;
		bPort->State = 7;
	}

	if (!TimerPending(&bPort->PredStateTimer))
	{
		if (bPort->PredState==bPort->State)
		{
			bPort->State=2;
			bPort->CmdState=0;
		}
	}
}

void PredStateTimerEnabled(TBluetoothHandle bPort)
{
	bPort->PredStateTimer.Counter=0;
	bPort->PredStateTimer.Timeout=20;
	InitTimer(&bPort->PredStateTimer, bPort->PredStateTimer.Timeout);
	StartTimer(&bPort->PredStateTimer);
}


void BluetoothWTUpdate(TBluetoothHandle bPort)
{

	if (bPort->State<7 && bPort->AssuredLaunchTimer>30)
	{
		bPort->AssuredLaunchTimer = 0;
		bPort->State=0;
		g_Core.Protections.outDefects.Dev.bit.BlueNoLaunch = 0;
	}
	switch (bPort->State)
	{
		// Иницилизация драйвера
		case 0:
			    InitChanelBt(bPort);
				bPort->State=1;
				break;
				
		// Обязательно должна быть пауза после инициализации
		case 1: if (!bPort->Timer)
				{ 
					ClearValues(bPort);	bPort->State=2;

					bPort->PredState=bPort->State;
					//bPort->PredStateTimer=
					PredStateTimerEnabled(bPort);
				}
				break;

		//
		case 2: if (bPort->Timer>1) break;
			    SendCommandOne(bPort, CMD_CONTROL_CONFIG);
				if (bPort->Status == BT_RECEIVE_COMPLETE)
				{ 
					ClearValues(bPort);	bPort->State=3;
					bPort->PredState=bPort->State;
					PredStateTimerEnabled(bPort);
				}
				break;

		case 3: SendCommandOne(bPort, CMD_CONTROL_ECHO);
				if (bPort->Status == BT_RECEIVE_COMPLETE)
				{ 
					ClearValues(bPort);	bPort->State=4;
					bPort->PredState=bPort->State;
					PredStateTimerEnabled(bPort);
				}
				break;
				
		case 4:	SendCommandOne(bPort, bPort->DeviceNameString);//CMD_CONTROL_NAME);//SendCommandTwo(bPort, CMD_CONTROL_NAME, bPort->DeviceNameString);
				if (bPort->Status == BT_RECEIVE_COMPLETE)
				{ 
					ClearValues(bPort);	bPort->State=5;
					bPort->PredState=bPort->State;
					PredStateTimerEnabled(bPort);
				}
				break;

		case 5: SendCommandOne(bPort, CMD_CONTROL_AUTH);//SendCommandTwo(bPort, CMD_CONTROL_AUTH, bPort->DeviceAuthCodeString);
				if (bPort->Status == BT_RECEIVE_COMPLETE)
				{ 
					ClearValues(bPort);	bPort->State=6;
					bPort->PredState=bPort->State;
					PredStateTimerEnabled(bPort);
				}
				break;
		case 6: SendCommandOne(bPort, CMD_CONTROL_BAUD);
				if (bPort->Status == BT_RECEIVE_COMPLETE)
				{ 
					ClearValues(bPort);	bPort->State=7;
					bPort->PredState=bPort->State;
					PredStateTimerEnabled(bPort);
				}
				break;
		// Режим ожидания соединения
		case 7:	bPort->BlinkConnect = false;
				bPort->PredState=0;
				g_Core.Protections.outDefects.Dev.bit.BlueNoLaunch = 0;
				if (bPort->Status == BT_RECEIVE_COMPLETE)
				{
					if (CheckString(bPort, "RI"))
					{ // Ожидаем соединение
		 				bPort->State=8;							// Должно прийти RING
						bPort->BlinkConnect = true;
				    }else
						ClearValues(bPort);						// Делаем сброс значений, если пришло что-то другое
				}											// Обычно приходит OK или PAIR
				break;
		case 8:
				StartTimer(&bPort->TimerBlink);
				bPort->Mode = BT_DATA_MODE;					// Переход в режим данных
				ClearValues(bPort);
				bPort->State=9;
				bPort->StrIndex=0;
				//if (bPort->ModeProtocol==2) bPort->WaitDelayAfterConnect = 40;
				break;
		case 9:
			    if (bPort->Mode == BT_COMMAND_MODE)			// Работаем в режиме данных,
				{										// ожидаем переход в режим команд
					ClearValues(bPort);
					bPort->State = 10;						// Переход в режим ожидания соединения
					// После включения Bluetooth запускаем таймер
					bPort->Timer = bPort->Period;
				}										
				break;				
				// Режим ожидания соединения
		case 10:	if (!bPort->Timer)
					{
						bPort->BlinkConnect = false;

						ClearValues(bPort);	bPort->State=7;
						McBsp_tx_disable(MCBSPA);
						McBsp_rx_enable(MCBSPA);
					}
				break;
	}

	bPort->IsConnected = (bPort->Mode == BT_DATA_MODE);
}

void ClearValues(TBluetoothHandle bPort)
{
	bPort->StrIndex = 0;
	bPort->CmdState = 0;
	bPort->Status = BT_IDLE;
}

// Разделил функции на две, т.к. strlen считает как-то долго
void SendCommandOne(TBluetoothHandle bPort, char *ComStr)
{
	switch(bPort->CmdState)
	{
		case 0: EnableBtTx();
				bPort->StrIndex = 0;
				bPort->CmdState = 1;
				bPort->Timer = 10;
				break;

		case 1: SendOneString(bPort, ComStr);

				if (bPort->Status == BT_TRANSMIT_COMPLETE)
				{
					bPort->CmdState = 2;
				} else if (!bPort->Timer) {
					bPort->State = 1;
				}
				break;

		case 2:
					EnableBtRx();
					bPort->StrIndex = 0;
					bPort->CmdState = 3;
				break;
				
		case 3:	if (bPort->Status == BT_RECEIVE_COMPLETE)		// Ждем завершение приема, т.к. при успешной команде модуль отвечает
					bPort->CmdState = 4;

				break;
	}
}


void SendCommandTwo(TBluetoothHandle bPort, char *ComStr, char *AddStr)
{
	switch(bPort->CmdState)
	{
		case 0: EnableBtTx();
				bPort->StrIndex = 0;
				bPort->CmdState = 1;
				break;

		case 1: SendTwoString(bPort, ComStr, AddStr);
				if (bPort->Status == BT_TRANSMIT_COMPLETE)
					bPort->CmdState = 2;
				break;

		case 2:
			    EnableBtRx();
				bPort->StrIndex = 0;
				bPort->CmdState = 3;
				break;
				
		case 3:	if (bPort->Status == BT_RECEIVE_COMPLETE)		// Ждем завершение приема, т.к. при успешной команде модуль отвечает
					bPort->CmdState = 4;
				break;
	}
}

void BluetoothRxHandler(TBluetoothHandle bPort, TMbHandle hPort)
{
	//Uns Data;

	// Обработчик прерывания зависит от текущего режима Bluetooth
	/*if (bPort->WaitDelayAfterConnect>2)
	{
		Data = ReceiveBtByte();
		return;
	}*/
	if 		(bPort->Mode == BT_COMMAND_MODE)	RxCommandMode(bPort);
	else if (bPort->Mode == BT_DATA_MODE)		RxDataMode(bPort, hPort);
}

//проверка статуса McBSP
Bool CheckCommError(TBluetoothHandle bPort)
{
	Byte Data = 0;
	Bool Error = false;

	/*if (bPort->HardWareType==UART_TYPE){
		Data = SCI_getstatus(bPort->ChannelID);
		if (Data & SCI_BREAK)
		{
			SCI_reset(bPort->ChannelID); Error = true;
		}

	} else if (bPort->HardWareType==MCBSP_TYPE){
		Data = McBsp_getstatus(bPort->ChannelID);
		if (Data & MCBSP_ERROR)
		{
			McBsp_reset(bPort->ChannelID); Error = true;
		}
	}*/

	Data = McBsp_getstatus(bPort->ChannelID);
	if (Data & MCBSP_ERROR)
	{
		McBsp_reset(bPort->ChannelID); Error = true;
	}

	return Error;
}


__inline void RxCommandMode(TBluetoothHandle bPort)
{
	Uns Data;

	//bPort->Error = CheckCommError(bPort);

	//if (bPort->Error)
	//	return;

	Data = ReceiveBtByte();

#if BT_DBG
	bPort->RxBytesCount++;
#endif

	if (bPort->StrIndex <= BT_RX_BUFFER_SIZE)
	{
		bPort->RxBuffer[bPort->StrIndex] = (char)(Data&0x00FF);
		bPort->StrIndex++;
		bPort->RxBuffer[bPort->StrIndex] = (char)(Data>>8)&0x00ff;
		bPort->StrIndex++;
	}

	// Самая маленькая посылка - 2 символа "ОК", иногда присылает сразу "\r\n", поэтому ждем первые символы
	// if ((p->StrIndex >= 2) && (Data == '\n'))
	if ((((Data)&0x00ff) == '\n') || (((Data>>8)&0x00ff) == '\n'))
	{
		bPort->Status = BT_RECEIVE_COMPLETE;
	}
	else
	{
		bPort->Status = BT_RECEIVE_BUSY;
	}

	//bPort->Status = (((Data)&0x00ff) == '\n') ? BT_RECEIVE_COMPLETE : BT_RECEIVE_BUSY;
	//bPort->Status = (((Data>>8)&0x00ff) == '\n') ? BT_RECEIVE_COMPLETE : BT_RECEIVE_BUSY;

}

__inline void RxDataMode(TBluetoothHandle bPort, TMbHandle hPort)
{
	Uns Data;
	Uns Data1;
	Uns Data2;
	TMbFrame *Frame = &hPort->Frame;

	//bPort->Error = CheckCommError(bPort);

	//if (bPort->Error)
	//	return;

	Data = ReceiveBtByte();
	bPort->Connect=true;

	if (bPort->Mode != BT_COMMAND_MODE && bPort->ModeProtocol ==1 )
	{
		if ((Frame->Data - Frame->Buf) < 256)
		{
			if (hPort->Params.HardWareType==MCBSP_TYPE)
			{
				*Frame->Data++ = (char)(Data&0x00FF);
				*Frame->Data++ = (char)(Data>>8)&0x00ff;
			}else
			{
				*Frame->Data++ = Data;
			}
		
		StartTimer(&Frame->Timer1_5);
		StartTimer(&Frame->Timer3_5);
		}
		

		hPort->Stat.RxBytesCount++;

		if (Frame->Buf[0]=='N' &&
			Frame->Buf[1]=='O' &&
			(Frame->Buf[2]==' ' || Frame->Buf[2]=='!'))
		{

			bPort->Mode = BT_COMMAND_MODE;
			bPort->Status = BT_RECEIVE_COMPLETE;
			Frame->Buf[0]=0;
			Frame->Buf[1]=0;
			Frame->Buf[2]=0;
			bPort->StrIndex=0;
			GpioDataRegs.GPADAT.bit.GPIO27=0;
			StartTimer(&Frame->TimerPost);
		}
	}

#if BT_DBG
	bPort->RxBytesCount++;
#endif


	// Прием данных для инф.модуля
	if (bPort->ModeProtocol == 2)
	{
		GpioDataRegs.GPATOGGLE.bit.GPIO27=1;
		Frame->Buf[0]=0;
		Frame->Buf[1]=0;
		Frame->Buf[2]=0;


		if (hPort->Params.HardWareType==MCBSP_TYPE)
		{
			Data1 = (char)(Data&0x00FF);
			Data2 = (char)(Data>>8)&0x00ff;

			switch (RxState)
			{
				case 0: if ((char)(Data1&0x00FF) != 'N')	{RxState=0;} else {RxState++;}
						if ((char)(Data2&0x00FF) != 'O')	{RxState=0;} else {RxState++;}  break;
				case 2: if ((char)(Data1&0x00FF) != ' ' || (char)(Data1&0x00FF) != '!')	{RxState=0;} else {RxState++;}	break;
				/*case 3: if (Data != 'C')	RxState = 0;	break;
				case 4: if (Data != 'A')	RxState = 0;	break;
				case 5: if (Data != 'R')	RxState = 0;	break;
				case 6: if (Data != 'R')	RxState = 0;	break;
				case 7: if (Data != 'I')	RxState = 0;	break;*/
			}

			// Прием данных для инф.модуля
			ImReceiveData(&g_Stat.Im, (char)(Data1&0x00FF));
			//ImReceiveData(&g_Stat.Im, (char)(Data2&0x00FF));//27.07  18-14 открыл коментарий

		}else
		{
			Data1 = Data;

			switch (RxState)
			{
				case 0: if ((char)(Data&0x00FF) != 'N')	RxState=0; else RxState++;	break;
				case 1: if ((char)(Data&0x00FF) != 'O')	RxState=0; else RxState++;	break;
				case 2: if ((char)(Data&0x00FF) != ' ' || (char)(Data&0x00FF) != '!')	RxState=0; else RxState++;	break;
				/*case 3: if (Data != 'C')	RxState = 0;	break;
				case 4: if (Data != 'A')	RxState = 0;	break;
				case 5: if (Data != 'R')	RxState = 0;	break;
				case 6: if (Data != 'R')	RxState = 0;	break;
				case 7: if (Data != 'I')	RxState = 0;	break;*/
			}
			// Прием данных для инф.модуля
			ImReceiveData(&g_Stat.Im, (char)(Data1&0x00FF));

		}

		if (RxState >= 2) //было 7 а теперь 10
		{
			bPort->Mode = BT_COMMAND_MODE;
			bPort->Status = BT_RECEIVE_COMPLETE;
			RxState = 0;
			GpioDataRegs.GPADAT.bit.GPIO27=0;
		}

	}

	/*
	здесь часть кода которая отвечает за отключение
	  RxState+=2;
	switch (RxState)
	{
		case 0: if (Data != 'N')	RxState = 0;	break;
		case 1: if (Data != 'O')	RxState = 0;	break;
		case 2: if (Data != ' ')	RxState = 0;	break;
		case 3: if (Data != 'C')	RxState = 0;	break;
		case 4: if (Data != 'A')	RxState = 0;	break;
		case 5: if (Data != 'R')	RxState = 0;	break;
		case 6: if (Data != 'R')	RxState = 0;	break;
		case 7: if (Data != 'I')	RxState = 0;	break;
	}

	// Прием данных для инф.модуля
	ImReceiveData(&g_Stat.Im, Data);

	if (RxState >= 7) //было 7 а теперь 10
	{	
		bPort->Mode = BT_COMMAND_MODE;
		RxState = 0;
	}*/
}

void BluetoothTxHandler(TBluetoothHandle bPort, TMbHandle hPort)
{
	TMbFrame *Frame = &hPort->Frame;
	Uns DataSend=0;
	Uns Stop=0;
	Uns i=0;

	bPort->TxBusy = false;

	if (bPort->Mode == BT_COMMAND_MODE) { return;}


	if (bPort->ModeProtocol==1)
	{
		if (TestCount==4) {
			StartTimer(&Frame->TimerPost);
			TestCount++;
			return;
		}
		if (TestCount>4) {
			TestCount++;
			return;
		}
		TestCount++;

		if ((Frame->Data - Frame->Buf) < Frame->TxLength){
		//if ((Frame->TxLength-hPort->Frame.AddCount)>0){

			//if (hPort->Params.HardWareType==UART_TYPE) SCI_transmit(hPort->Params.ChannelID, *Frame->Data++);
			//else
			//if (hPort->Params.HardWareType==MCBSP_TYPE)
			//{
			/*if (bPort->ModeProtocol==2)
			{
				count+=2;
				if (count>=12)
				{
					count=0;
					for(i=0; i<1000; i++) {}
				}
			}*/

				if (((Frame->TxLength)&0x01) && ((Frame->Data - Frame->Buf)>=(Frame->TxLength-1)))
				{
					Stop = 1;
					DataSend = ((*Frame->Data++)&0x00FF)|(((*Frame->Data++)<<8)&0xFF00);

					McBsp_transmit(hPort->Params.ChannelID, DataSend, Stop);
					hPort->Stat.TxBytesCount++;
					hPort->Frame.AddCount++;
				} else
				{
					DataSend = ((*Frame->Data++)&0x00FF)|(((*Frame->Data++)<<8)&0xFF00);

					McBsp_transmit(hPort->Params.ChannelID, DataSend, 0);
					hPort->Stat.TxBytesCount++;
					hPort->Frame.AddCount++;
					hPort->Stat.TxBytesCount++;
					hPort->Frame.AddCount++;
				}
				Stop=0;
			//}
		}
		else {
			StartTimer(&Frame->TimerPost);
		}
	}
}

void BluetoothTimer(TBluetoothHandle bPort)
{
	//if (bPort->WaitDelayAfterConnect >0) bPort->WaitDelayAfterConnect--;
	//if (bPort->WaitDelayAfterConnect >40) bPort->WaitDelayAfterConnect = 0;
	if (bPort->AssuredLaunchTimer<100) bPort->AssuredLaunchTimer++;
	if (bPort->Timer > 0) bPort->Timer--;
}

Bool CheckString(TBluetoothHandle bPort, char *Str)
{
	Uns i, j;
	Uns Lenght = strlen(Str);						// Длина текущей строки
	Uns BufIndex = 0;	Uns CheckIndex = 0;

	bPort->StrIndex = 0;

	if (Lenght > BT_RX_BUFFER_SIZE)		return false;							// Если строка превышает длину буфера, то выходим
	else								BufIndex = BT_RX_BUFFER_SIZE - Lenght;	// Считаем индекс проверки строки в буфере
															
	for (i = 0; i <= BufIndex; i++)						// Начинаем проверять всю строку с нулевого индекса массива, затем с первого
	{													// и т.д. до тех пор пока не превысим индекс проверки 
		for (j = 0; j < Lenght; j++)					// Сравниваем символы в строке с символами в буфере
		{												// до тех пор пока не превысим длину строки
			if (Str[j] == bPort->RxBuffer[j+i])				// Если символы совпали, то инкрементируем CheckIndex
				CheckIndex++;			
		}
		if (Lenght == CheckIndex)	return true;		// Если длина строки совпала с CheckIndex, то проверка успешна
		else						CheckIndex = 0;		// Иначе обнуляем индекс и продолжаем анализ
	}	

	return false;										// Строка не совпала
}


void SendOneString(TBluetoothHandle bPort, char *String)
{
	char symbol1 = 255;
	char symbol2 = 255;

	/*if (!bPort->TxBusy)
	{
		symbol = String[bPort->StrIndex];					// Загружаем текущий символ
		if (symbol == '\0')								// Достигли конца строки
		{
			bPort->Status = BT_TRANSMIT_COMPLETE;			// Статус завершения передачи
			bPort->StrIndex = 0;
		}
		else
		{
			bPort->StrIndex++;
			bPort->Status = BT_TRANSMIT_BUSY;				// Статус передачи
			bPort->TxBusy = true;							// Выставляем флаг передачи
			//DELAY_US(10);
			TransmitBtByte(symbol);					// Передаем на SCI

		}
	}*/

	if (!bPort->TxBusy)
	{

		/*if ()
		{
			bPort->StrIndex+=2;
			bPort->Status = BT_TRANSMIT_BUSY;				// Статус передачи
			bPort->TxBusy = true;							// Выставляем флаг передачи
			TransmitBtByte(symbol1|((symbol2<<8)&0xFF00));					// Передаем на SCI
		}
		else if (symbol1 == '\0') {
			bPort->Status = BT_TRANSMIT_COMPLETE;			// Статус завершения передачи
			bPort->StrIndex = 0;
		}


		if (symbol1 == '\0')								// Достигли конца строки
		{
			bPort->Status = BT_TRANSMIT_COMPLETE;			// Статус завершения передачи
			bPort->StrIndex = 0;

			return;
		}

		if (symbol2 == '\0')
		{
			symbol2 = '\n';
		}

		bPort->StrIndex+=2;
		bPort->Status = BT_TRANSMIT_BUSY;				// Статус передачи
		bPort->TxBusy = true;							// Выставляем флаг передачи
		TransmitBtByte(symbol1|((symbol2<<8)&0xFF00));					// Передаем на SCI
		*/

		//bPort->StrIndex+=2;
		symbol1 = String[bPort->StrIndex]; bPort->StrIndex++;					// Загружаем текущий символ
		symbol2 = String[bPort->StrIndex]; bPort->StrIndex++;					// Загружаем текущий символ

		if (g_Comm.Bluetooth.Stop == 1)
		{
			g_Comm.Bluetooth.Stop = 0;
			symbol1 = '\0';
		}

		if (symbol1 == '\0')								// Достигли конца строки
		{
			bPort->Status = BT_TRANSMIT_COMPLETE;			// Статус завершения передачи
			bPort->StrIndex = 0;
		}
		else
		{
			bPort->Status = BT_TRANSMIT_BUSY;				// Статус передачи
			bPort->TxBusy = true;							// Выставляем флаг передачи
			if (symbol2 == '\0')
			{
				g_Comm.Bluetooth.Stop = 1;
				TransmitBtByte(symbol1|((symbol2<<8)&0xFF00));					// Передаем на SCI
				//bPort->Status = BT_TRANSMIT_COMPLETE;			// Статус завершения передачи
				//bPort->StrIndex = 0;
			} else
			{
				TransmitBtByte(symbol1|((symbol2<<8)&0xFF00));					// Передаем на SCI
			}
			//g_Comm.Bluetooth.Stop = 0;

		}

		/*if (symbol2 == '\0')								// Достигли конца строки
		{
			bPort->Status = BT_TRANSMIT_COMPLETE;			// Статус завершения передачи
			bPort->StrIndex = 0;
		}*/

	}
}


void SendTwoString(TBluetoothHandle bPort, char *FirstString, char *SecondString)
{
	char symbol1 = 0;
	char symbol2 = 0;

	static Bool isFirstStr = true;
	static Bool isEndSymbols = false;
	static Byte len1 = 0;
	static Byte len2 = 0;

	if (!bPort->TxBusy)
	{
		if (bPort->Status != BT_TRANSMIT_BUSY)
		{
			len1 = strlen(FirstString);
			len2 = strlen(SecondString);
		}

		if (isFirstStr && (bPort->StrIndex >= len1))
		{
			bPort->StrIndex = 0;
			isFirstStr = false;
		}

		if (!isFirstStr && (bPort->StrIndex >= len2))
		{ 
			isEndSymbols = true;
			bPort->StrIndex = 0;
		}

		symbol1 = (isFirstStr) ? FirstString[bPort->StrIndex] : SecondString[bPort->StrIndex];
		symbol2 = (isFirstStr) ? FirstString[bPort->StrIndex+1] : SecondString[bPort->StrIndex+1];

		if (isEndSymbols && bPort->StrIndex < 2)
			symbol1 = (bPort->StrIndex == 0) ? '\r' : '\n';
		else if (isEndSymbols && bPort->StrIndex >= 2)
			symbol1 = '\0';

		if (symbol1 == '\0')								// Достигли конца строки
		{
			bPort->Status = BT_TRANSMIT_COMPLETE;			// Статус завершения передачи
			bPort->StrIndex = 0;
			isFirstStr = true;
			isEndSymbols = false;
			len1 = 0;
			len2 = 0;
		}
		else
		{
			bPort->StrIndex+=2;
			bPort->Status = BT_TRANSMIT_BUSY;				// Статус передачи
			bPort->TxBusy = true;							// Выставляем флаг передачи
			//TransmitBtByte(symbol);					// Передаем на SCI
			TransmitBtByte(symbol1|((symbol2<<8)&0xFF00));
		}
	}
}


/*
void SendTwoString(TBluetoothHandle bPort, char *FirstString, char *SecondString)
{
	char symbol = 0;

	static Bool isFirstStr = true;
	static Bool isEndSymbols = false;
	static Byte len1 = 0;
	static Byte len2 = 0;

	if (!bPort->TxBusy)
	{
		if (bPort->Status != BT_TRANSMIT_BUSY)
		{
			len1 = strlen(FirstString);
			len2 = strlen(SecondString);
		}

		if (isFirstStr && (bPort->StrIndex >= len1))
		{
			bPort->StrIndex = 0;
			isFirstStr = false;
		}

		if (!isFirstStr && (bPort->StrIndex >= len2))
		{
			isEndSymbols = true;
			bPort->StrIndex = 0;
		}

		symbol = (isFirstStr) ? FirstString[bPort->StrIndex] : SecondString[bPort->StrIndex];

		if (isEndSymbols && bPort->StrIndex < 2)
			symbol = (bPort->StrIndex == 0) ? '\r' : '\n';
		else if (isEndSymbols && bPort->StrIndex >= 2)
			symbol = '\0';

		if (symbol == '\0')								// Достигли конца строки
		{
			bPort->Status = BT_TRANSMIT_COMPLETE;			// Статус завершения передачи
			bPort->StrIndex = 0;
			isFirstStr = true;
			isEndSymbols = false;
			len1 = 0;
			len2 = 0;
		}
		else
		{
			bPort->StrIndex++;
			bPort->Status = BT_TRANSMIT_BUSY;				// Статус передачи
			bPort->TxBusy = true;							// Выставляем флаг передачи
			TransmitBtByte(symbol);					// Передаем на SCI
		}
	}
}
*/
//McBSP включение прерывания на приём
// и выключение прерывания на передачу
//void EnableBtRx(TBluetoothHandle bPort)
void EnableBtRx(void)
{
	LgUns i=0;
	for(i=0; i<30000; i++){}
	McBsp_tx_disable(MCBSPA);
	McBsp_rx_enable(MCBSPA);
}

//McBSP включение прерывания на передачу
// и выключение прерывания на приём
/*void EnableBtTx(TBluetoothHandle bPort)
{
	if (bPort->HardWareType==UART_TYPE)
	{
		SCI_rx_disable(bPort->ChannelID);
		SCI_tx_enable(bPort->ChannelID);

	} else if (bPort->HardWareType==UART_TYPE){

		McBsp_rx_disable(bPort->ChannelID);
		McBsp_tx_enable(bPort->ChannelID);
	}
}*/

void EnableBtTx(void)
{
	McBsp_rx_disable(MCBSPA);
	McBsp_tx_enable(MCBSPA);

}

//приём 1 байта по каналу McBSP
Uns ReceiveBtByte(void)
{
	return McBsp_recieve(MCBSPA);
}

/*Byte ReceiveBtByte(TBluetoothHandle bPort)
{
	if (bPort->HardWareType==UART_TYPE)
	{
		return SCI_recieve(bPort->ChannelID);

	} else if (bPort->HardWareType==UART_TYPE){

		return McBsp_recieve(bPort->ChannelID);
	}

	return 0;
}*/

//передача 1 байта по каналу McBSP
void TransmitBtByte(Uns Data)
{
	McBsp_transmit(MCBSPA, Data, g_Comm.Bluetooth.Stop);
}

//передача 1 байта по каналу McBSP
void TransmitBtByteIM(Uns Data)
{
	McBsp_transmit(MCBSPA, Data, 2);
	//McBsp_transmitIM(MCBSPA, Data);
}
/*void TransmitBtByte(TBluetoothHandle bPort, Byte Data)
{
	if (bPort->HardWareType==UART_TYPE)
	{
		McBsp_transmit(bPort->ChannelID, Data);

	} else if (bPort->HardWareType==UART_TYPE){

		McBsp_transmit(bPort->ChannelID, Data);
	}

}*/
