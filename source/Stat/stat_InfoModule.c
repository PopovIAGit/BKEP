
#include "g_Ram.h"
#include "stat.h"
//#include "stat_InfoModule.h"

Uns ImEvLogMainAddrsTable[20];
Uns ImEvLogBufAddrsTable[12];
Uns ImCmdLogAddrsTable[6];
Uns ImParamLogAddrsTable[5];
Uns ImSimLogAddrsTable[13];

/*Uns ImEvLogMainAddrsTable[]	= {
								GetAdr(ramGroupB.DevTime),
								GetAdr(ramGroupB.DevDate),
								GetAdr(ramGroupA.Status),
								GetAdr(ramGroupA.Faults.Proc),
								GetAdr(ramGroupA.Faults.Net),
								GetAdr(ramGroupA.Faults.Load),
								GetAdr(ramGroupA.Faults.Dev),
								GetAdr(ramGroupA.PositionPr),
								GetAdr(ramGroupA.Torque),
								GetAdr(ramGroupA.Ur),
								GetAdr(ramGroupA.Us),
								GetAdr(ramGroupA.Ut),
								GetAdr(ramGroupA.Iu),
								GetAdr(ramGroupA.Iv),
								GetAdr(ramGroupA.Iw),
								GetAdr(ramGroupA.Temper),
								GetAdr(ramGroupA.StateTu.all),
								GetAdr(ramGroupA.StateTs.all),
								GetAdr(ramGroupH.Seconds),
								FIRST_RECORD_FLAG_ADDR
};

Uns ImEvLogBufAddrsTable[] = {
								GetAdr(ramGroupA.Status),
								GetAdr(ramGroupA.PositionPr),
								GetAdr(ramGroupA.Torque),
								GetAdr(ramGroupA.Ur),
								GetAdr(ramGroupA.Us),
								GetAdr(ramGroupA.Ut),
								GetAdr(ramGroupA.Iu),
								GetAdr(ramGroupA.Iv),
								GetAdr(ramGroupA.Iw),
								GetAdr(ramGroupA.Temper),
								GetAdr(ramGroupA.StateTu.all),
								GetAdr(ramGroupA.StateTs.all)
};

Uns ImCmdLogAddrsTable[] = {
								GetAdr(ramGroupB.DevTime),
								GetAdr(ramGroupB.DevDate),
								GetAdr(ramGroupH.Seconds),
								GetAdr(ramGroupH.LogControlWord),
								GetAdr(ramGroupA.Status)
};

Uns ImParamLogAddrsTable[] = {
								GetAdr(ramGroupB.DevTime),
								GetAdr(ramGroupB.DevDate),
								GetAdr(ramGroupH.Seconds),
								NEW_PARAM_ADDR,
								NEW_PARAM_VALUE_ADDR
};*/

Uns CurrentLogRec = 0;
Uns CurrentLogCnt = 0;

void ReceiveFunc(pTInfoModule);
void DownloadFunc(pTInfoModule);
void SendData(pTInfoModule);

__inline Bool FuncOne(pTInfoModule);
__inline Bool FuncTwo(pTInfoModule);
__inline Bool FuncThree(pTInfoModule, Byte LogType);
__inline Bool FuncFour(pTInfoModule, Byte LogType, Byte CellNum);
__inline Bool FuncFive(pTInfoModule, Byte LogType, Uns RecordNum);
__inline Bool FuncSix(pTInfoModule, Uns StartParamIndex);

void ImUpdate(TInfoModule *p)
{
	switch (p->ControlMode)
	{
		case RECEIVE_FUNC_MODE:		ReceiveFunc(p);		break;
		case DOWNLOAD_FUNC_MODE: 	DownloadFunc(p);	break;
		case SEND_DATA_MODE:		SendData(p);		break;
	}

	if (++p->ControlMode > SEND_DATA_MODE)
		p->ControlMode = 0;
}

void ImReset(TInfoModule *p)
{
	p->Timer = 0;
	p->Index = 0;
}

void ReceiveFunc(TInfoModule *p)
{
	Bool IsFuncReceived = false;

	if (p->CanSendData)
		return;

	switch (p->RdBuffer[0])
	{
		case 127: if (p->Index>28 && p->Index<32){
			g_Comm.mbBt.Packet.Request = 0x10;
			g_Comm.mbBt.Packet.Count = 10; //или 20
			g_Comm.mbBt.Packet.Addr = 65345;
			g_Comm.mbBt.Packet.Data[0] = p->RdBuffer[7]<<8 | p->RdBuffer[8];
			g_Comm.mbBt.Packet.Data[1] = p->RdBuffer[9]<<8 | p->RdBuffer[10];
			g_Comm.mbBt.Packet.Data[2] = p->RdBuffer[11]<<8 | p->RdBuffer[12];
			g_Comm.mbBt.Packet.Data[3] = p->RdBuffer[13]<<8 | p->RdBuffer[14];
			g_Comm.mbBt.Packet.Data[4] = p->RdBuffer[15]<<8 | p->RdBuffer[16];
			g_Comm.mbBt.Packet.Data[5] = p->RdBuffer[17]<<8 | p->RdBuffer[18];
			g_Comm.mbBt.Packet.Data[6] = p->RdBuffer[19]<<8 | p->RdBuffer[20];
			g_Comm.mbBt.Packet.Data[7] = p->RdBuffer[21]<<8 | p->RdBuffer[22];
			g_Comm.mbBt.Packet.Data[8] = p->RdBuffer[23]<<8 | p->RdBuffer[24];
			g_Comm.mbBt.Packet.Data[9] = p->RdBuffer[25]<<8 | p->RdBuffer[26];
		}
		break;
		// Device Info
		case 0x01: 	if (p->Index>=2) p->Index = 1;

					if (p->Index == 1)
					{
						 IsFuncReceived = true;
						 p->FuncState = imDeviceInfo;
						 p->CorrectCount=1;
					}
					break;
					
		// Summary Logs Info
		case 0x02:	if (p->CorrectCount==1 && p->Index>=2) p->Index = 1;
					if (p->Index == 1)
					{
						 IsFuncReceived = true;
						 p->FuncState = imSumLogsInfo;
						 p->CorrectCount = 0;
					}
					break;
					
		// Concrete Logs Info
		case 0x03:	if (p->Index == 2)
					{
						 IsFuncReceived = true;
						
						// В зависимости от типа журнала
						 p->FuncState = imEvLogInfo + (p->RdBuffer[1] - 1);
						 p->CorrectCount=1;
					}
					break;
		
		// Logs Parameters Addresses
		case 0x04:	if (p->CorrectCount==1 && p->Index>=4) p->Index = 3;
					if (p->Index == 3)
					{
						 IsFuncReceived = true;
						 						 	
						 // В зависимости от типа журнала и номера ячейки					
						if ((p->RdBuffer[1] == IM_LOGEV_TYPE) && (p->RdBuffer[2] == 1))
								p->FuncState = imEvLogMainAddr;
						else if ((p->RdBuffer[1] == IM_LOGEV_TYPE) && (p->RdBuffer[2] > 1))
							p->FuncState = imEvLogBufAddr;
						else if (p->RdBuffer[1] == IM_LOGCMD_TYPE)
							p->FuncState = imCmdLogAddr;
						else if (p->RdBuffer[1] == IM_LOGPARAMS_TYPE)
							p->FuncState = imParLogAddr;
						else if (p->RdBuffer[1] == IM_LOGSIM_TYPE)
							p->FuncState = imSimLogAddr;
						p->CorrectCount=0;
					}
					break;
					
		// Logs Data
		case 0x05:	if (p->Index == 4 || p->Index == 5)
					{
						p->Index = 4;
						IsFuncReceived = true;
						p->FuncState = imLogDownload;

						CurrentLogCnt = (p->RdBuffer[2] << 8) | (p->RdBuffer[3]);
						CurrentLogRec = 0;
						p->CanReadNextRec = true;
						p->CorrectCount=1;
					}

					break;

		// Parameters
		case 0x06:	if (p->CorrectCount==1 && p->Index>=4)
					{
						if (p->HardwareSrc==imSrcModbus) p->Index = 4;
						else p->Index = 3;
					}
					if (((p->Index == 3)&&(p->HardwareSrc!=imSrcModbus)) ||
						((p->Index == 4)&&(p->HardwareSrc==imSrcModbus)))
					{
						 IsFuncReceived = true;
						 p->FuncState = imParamDownload;
						 p->CorrectCount=1;
					}

					break;
		default: return;
	}

	if (IsFuncReceived)
	{
		memcpy(&p->WrBuffer, p->RdBuffer, p->Index);

		p->RdBuffer[0] = 0;
	}
}

void DownloadFunc(TInfoModule *p)
{
		switch (p->FuncState)
		{
			case imInit:				if (p->HardwareSrc==imSrcBluetooth) p->EnableReceive();
										ImReset(p);
										p->FuncState = imIdle;
			break;

			case imDeviceInfo:			p->CanSendData = FuncOne(p);
			break;

			case imSumLogsInfo:			p->CanSendData = FuncTwo(p);
			break;

			case imEvLogInfo:			p->CanSendData = FuncThree(p, IM_LOGEV_TYPE);
			break;
			
			case imCmdLogInfo:			p->CanSendData = FuncThree(p, IM_LOGCMD_TYPE);
			break;

			case imParamLogInfo:		p->CanSendData = FuncThree(p, IM_LOGPARAMS_TYPE);
			break;

			case imSimLogInfo:		    p->CanSendData = FuncThree(p, IM_LOGSIM_TYPE);
			break;

			case imEvLogMainAddr:		p->CanSendData = FuncFour(p, IM_LOGEV_TYPE, 1);
			break;

			case imEvLogBufAddr:		p->CanSendData = FuncFour(p, IM_LOGEV_TYPE, 2);
			break;

			case imCmdLogAddr:			p->CanSendData = FuncFour(p, IM_LOGCMD_TYPE, 1);
			break;

			case imParLogAddr:			p->CanSendData = FuncFour(p, IM_LOGPARAMS_TYPE, 1);
			break;

			case imSimLogAddr:			p->CanSendData = FuncFour(p, IM_LOGSIM_TYPE, 1);
			break;

			case imLogDownload:

				if ((CurrentLogCnt > 0) && !p->CanSendData)
				{
					p->IsLogTransmit = true;
					p->CanSendData = FuncFive(p, p->RdBuffer[1], CurrentLogRec);
				}
			break;

			case imParamDownload:
				p->CanSendData = FuncSix(p, (p->RdBuffer[1] << 8) | (p->RdBuffer[2]));
			break;
			
		}

		if (p->CanSendData)
		{
			if (!p->IsLogTransmit)	
				p->FuncState = imIdle;

			p->Timer = p->Period;
			p->TimerIndex = p->PeriodIndex;
			if (p->HardwareSrc==imSrcBluetooth) p->EnableTransmit();
		}


		//p->Timer = p->Period;
}

void ImReceiveData(TInfoModule *p, Byte Data)
{
	Uns i=0;

	p->RdBuffer[p->Index] = Data;

	if (p->Index>0)
	{
		i=p->Index;
		i=i+1;
	}

	if (p->Index < IM_RD_BUFFER_SIZE)
		p->Index++;

	// На всякий случай заводим таймер сброса
	p->Timer = p->Period;
	p->TimerIndex = p->PeriodIndex;
	p->HardwareSrc = imSrcBluetooth;
}


void SendData(TInfoModule *p)
{
	Byte Data = 0;
	Byte i=0;

	if (!p->CanSendData)
		return;
	
	if (!(*p->IsTxBusy))						// Проверяем, что передатчик свободен
	{
		if (p->TxIndex > p->Index)
		{
			p->TxIndex = 0;						// Сбрасываем параметры
			p->Index = 0;
			p->CanSendData = false;
			p->Timer = 0;
			p->Index = 0;

			//p->IsLogTransmit = false;

			if (p->HardwareSrc==imSrcBluetooth) {
				if (!p->IsLogTransmit)				// Включаем передатчик, если не осуществляется
				p->EnableReceive();				// непрерывная передача журнала
			}

			//p->FuncState = imInit;
		}
		else
		{
			p->TimerIndex = p->PeriodIndex;		// Заводим таймер сброса
			p->Timer = p->Period;				// Заводим таймер сброса
			*p->IsTxBusy = true;				// Выставляем флаг занятости передатчика. Нужно делать это именно в ИМ

			if (p->HardwareSrc==imSrcBluetooth) {
				//if (p->WaitAndroidCounter==0) {
					Data = p->WrBuffer[p->TxIndex++];
					//for(i=0; i<20; i++) asm("NOP");
					p->TransmitByte(Data);// в драйвере Bluetooth
					//p->WaitAndroidCounter = 1;
				//}
			}
			else if (p->HardwareSrc==imSrcModbus) {
				if (p->CanSendDataMb) return;
				for (i=0; i<p->Index+1; i++)
				{
					p->WrBufferMb[i] = p->WrBuffer[i];
					p->IndexMb=p->TxIndex;
					p->TxIndex++;
				}
				if (p->FuncState==imLogDownload) {
					p->CanSendData = true;
					p->CanSendDataMb = 1;
				} else p->CanSendData = false;
				*p->IsTxBusy = false;
				p->TxIndex = 0;						// Сбрасываем параметры
				p->Index = 0;
				p->Timer = 0;
			}

		}
	}
}

__inline Bool FuncOne(TInfoModule *p)
{
	Uns StartIndex = p->Index;
	Uns CurrentIndex = StartIndex + 1;
	Uns i=0;
	
	//for(i=0; i<1000; i++) asm("NOP");
	if (p->HardwareSrc!=imSrcModbus) {
		p->WaitTime = 10;
		while(p->WaitTime>0){};
	}

	p->WrBuffer[CurrentIndex++] = IM_FIRMWARE_VERSION >> 8;
	p->WrBuffer[CurrentIndex++] = IM_FIRMWARE_VERSION & 0xFF;
	p->WrBuffer[CurrentIndex++] = IM_DEVICE_ID >> 8;
	p->WrBuffer[CurrentIndex++] = IM_DEVICE_ID & 0xFF;
	p->WrBuffer[CurrentIndex++] = IM_PARAMS_CNT >> 8;
	p->WrBuffer[CurrentIndex++] = IM_PARAMS_CNT & 0xFF;
	p->WrBuffer[CurrentIndex++] = IM_LOGS_CNT;
	p->WrBuffer[CurrentIndex++] = IM_LOGEV_TYPE;
	p->WrBuffer[CurrentIndex++] = IM_LOGCMD_TYPE;
	p->WrBuffer[CurrentIndex++] 	= IM_LOGPARAMS_TYPE;
	//ma LogSim p->WrBuffer[CurrentIndex++] 	= IM_LOGPARAMS_TYPE;
	p->WrBuffer[CurrentIndex] 	= IM_LOGSIM_TYPE;


	// Количество полей
	p->WrBuffer[StartIndex]		= CurrentIndex - StartIndex;
	p->Index = CurrentIndex;

	return true;
}

__inline Bool FuncTwo(TInfoModule *p)
{
	Uns StartIndex = p->Index;
	Uns CurrentIndex = StartIndex + 1;
	Uns i = 0;

	//for(i=0; i<1000; i++) asm("NOP");
	if (p->HardwareSrc!=imSrcModbus) {
		p->WaitTime = 10;
		while(p->WaitTime>0){};
	}

	p->WrBuffer[CurrentIndex++] = IM_LOGEV_REC_ADDR >> 8;
	p->WrBuffer[CurrentIndex++] = IM_LOGEV_REC_ADDR & 0xFF;
	p->WrBuffer[CurrentIndex++] = IM_LOGEV_REC_CNT >> 8;
	p->WrBuffer[CurrentIndex++] = IM_LOGEV_REC_CNT & 0xFF;

	p->WrBuffer[CurrentIndex++] = IM_LOGCMD_REC_ADDR >> 8;
	p->WrBuffer[CurrentIndex++] = IM_LOGCMD_REC_ADDR & 0xFF;
	p->WrBuffer[CurrentIndex++] = IM_LOGCMD_REC_CNT >> 8;
	p->WrBuffer[CurrentIndex++] = IM_LOGCMD_REC_CNT & 0xFF;

	/* ma LogSIM p->WrBuffer[CurrentIndex++] = IM_LOGPAR_REC_ADDR >> 8;
	p->WrBuffer[CurrentIndex++] = IM_LOGPAR_REC_ADDR & 0xFF;
	p->WrBuffer[CurrentIndex++] = IM_LOGPAR_REC_CNT >> 8;
	p->WrBuffer[CurrentIndex]	= IM_LOGPAR_REC_CNT & 0xFF;*/

	//ma LogSIM
	p->WrBuffer[CurrentIndex++] = IM_LOGPAR_REC_ADDR >> 8;
	p->WrBuffer[CurrentIndex++] = IM_LOGPAR_REC_ADDR & 0xFF;
	p->WrBuffer[CurrentIndex++] = IM_LOGPAR_REC_CNT >> 8;
	p->WrBuffer[CurrentIndex++]	= IM_LOGPAR_REC_CNT & 0xFF;

	p->WrBuffer[CurrentIndex++] = IM_LOGSIM_REC_ADDR >> 8;
	p->WrBuffer[CurrentIndex++] = IM_LOGSIM_REC_ADDR & 0xFF;
	p->WrBuffer[CurrentIndex++] = IM_LOGSIM_REC_CNT >> 8;
	p->WrBuffer[CurrentIndex]	= IM_LOGSIM_REC_CNT & 0xFF;

	p->WrBuffer[StartIndex]		= CurrentIndex - StartIndex;
	p->Index = CurrentIndex;

	return true;
}



__inline Bool FuncThree(TInfoModule *p, Byte LogType)
{
	Uns StartIndex = p->Index;
	Uns CurrentIndex = StartIndex + 1;

	Uns i;

	//for(i=0; i<1000; i++) asm("NOP");
	//p->WaitTime = 10;
	//while(p->WaitTime>0){}


	switch (LogType)
	{
		case IM_LOGEV_TYPE: 
			// Количество ячеек = основная + буферные
			p->WrBuffer[CurrentIndex++] = 1 + IM_LOGEV_BUF_CELL_CNT;
			p->WrBuffer[CurrentIndex++] = IM_LOGEV_MAIN_CELL_DATA_CNT;

			for (i = 0; i < IM_LOGEV_BUF_CELL_CNT; i++)
			{
				p->WrBuffer[CurrentIndex++] = IM_LOGEV_BUF_CELL_DATA_CNT;
			}

			CurrentIndex--;							// В цикле было лишнее инкрементирование
					
		break;

		case IM_LOGCMD_TYPE:
		
			p->WrBuffer[CurrentIndex++] = 1;
			p->WrBuffer[CurrentIndex] = LOG_CMD_DATA_CNT; 
		
		break;

		case IM_LOGPARAMS_TYPE:
			p->WrBuffer[CurrentIndex++] = 1;
			p->WrBuffer[CurrentIndex] = LOG_PARAM_DATA_CNT;
		
		break;
		//ma LogSIM
		case IM_LOGSIM_TYPE:
			p->WrBuffer[CurrentIndex++] = 1;
			p->WrBuffer[CurrentIndex] = LOG_SIM_DATA_CNT;

		break;
	}

	p->WrBuffer[StartIndex] = CurrentIndex - StartIndex;
	p->Index = CurrentIndex;

	return true;
}


__inline Bool FuncFour(TInfoModule *p, Byte LogType, Byte CellNum)
{
	Uns *table;	Uns size = 0; Uns index = 0; Uns addr = 0;
	Uns StartIndex = p->Index;
	Uns CurrentIndex = StartIndex + 1;


	switch (LogType)
	{
		case IM_LOGEV_TYPE:
								if (CellNum == 1)
								{
									table = (Uns *)&ImEvLogMainAddrsTable;
									size = sizeof(ImEvLogMainAddrsTable);
								}
								else
								{
									table = (Uns *)&ImEvLogBufAddrsTable;
									size = sizeof(ImEvLogBufAddrsTable);
								}
		break;

		case IM_LOGCMD_TYPE:	
								table = (Uns *)&ImCmdLogAddrsTable;
								size = sizeof(ImCmdLogAddrsTable);
		break;

		case IM_LOGPARAMS_TYPE:	
								table = (Uns *)&ImParamLogAddrsTable;
								size = sizeof(ImParamLogAddrsTable);		
		break;

		//ma logSimConnect
		case IM_LOGSIM_TYPE:
								table = (Uns *)&ImSimLogAddrsTable;
								size = sizeof(ImSimLogAddrsTable);
			break;

	}

	for (index = 0; index < size; index++)
	{
		addr = table[index];
		p->WrBuffer[CurrentIndex++] = addr >> 8;
		p->WrBuffer[CurrentIndex++] = addr & 0xFF;
	}
	
	// В цикле было лишнее инкрементирование
	CurrentIndex--;

	p->WrBuffer[StartIndex] = CurrentIndex - StartIndex;
	p->Index = CurrentIndex;

	return true;
}



__inline Bool FuncFive(TInfoModule *p, Byte LogType, Uns RecordNum)
{
	Uns StartIndex = p->Index;
	Uns CurrentIndex = StartIndex;
	Uns CellIndex = 0;	Uns ImIndex = 0; Uns i;

	ImBufferReader(p, LogType, RecordNum);
	//ImBufferReaderTest(p, LogType, RecordNum);


	if (p->IsBufReady)
	{
		p->IsBufReady = false;

		// Добавляем номер текущей записи
		p->WrBuffer[CurrentIndex++] = CurrentLogRec >> 8;
		p->WrBuffer[CurrentIndex++] = CurrentLogRec & 0xFF;

		switch (LogType)
		{
			case IM_LOGEV_TYPE:

				for (CellIndex = 1; CellIndex <= (IM_LOGEV_BUF_CELL_CNT + 1); CellIndex++)
				{

					// Основная ячейка
					if (CellIndex == 1)
					{
						if (!(CurrentLogRec & 0x01))
						{	ImIndex = 0;	}
						else
						{	ImIndex = IM_DOWNLOAD_BUFFER_SIZE;	}

						for (i = 0; i < IM_LOGEV_MAIN_CELL_DATA_CNT; i++)
						{
							p->WrBuffer[CurrentIndex++] = p->ImReadBuf[ImIndex + i] >> 8;
							p->WrBuffer[CurrentIndex++] = p->ImReadBuf[ImIndex + i] & 0xFF;
						}
					}
					else if (CellIndex >= 2)
					{
				
						for (i = 0; i < IM_LOGEV_BUF_CELL_DATA_CNT; i++)
						{

							if (!(CurrentLogRec & 0x01))
							{	ImIndex = IM_LOGEV_MAIN_CELL_DATA_CNT + (IM_LOGEV_BUF_CELL_DATA_CNT * (CellIndex - 2));	}
							else
							{	ImIndex = IM_LOGEV_MAIN_CELL_DATA_CNT + IM_DOWNLOAD_BUFFER_SIZE + (IM_LOGEV_BUF_CELL_DATA_CNT * (CellIndex - 2));	}
								
							p->WrBuffer[CurrentIndex++] = p->ImReadBuf[ImIndex + i] >> 8;
							p->WrBuffer[CurrentIndex++] = p->ImReadBuf[ImIndex + i] & 0xFF;
						}
					}
				}

			break;

			case IM_LOGCMD_TYPE:

				if (!(CurrentLogRec & 0x01))
				{	ImIndex = 0;	}
				else
				{	ImIndex = IM_LOGCMD_CELL_DATA_CNT;	}

				for (i = 0; i < IM_LOGCMD_CELL_DATA_CNT; i++)
				{
					p->WrBuffer[CurrentIndex++] = p->ImReadBuf[ImIndex + i] >> 8;
					p->WrBuffer[CurrentIndex++] = p->ImReadBuf[ImIndex + i] & 0xFF;
				}
				
			break;

			case IM_LOGPARAMS_TYPE:

				if (!(CurrentLogRec & 0x01))
				{	ImIndex = 0;	}
				else
				{	ImIndex = IM_LOGPAR_CELL_DATA_CNT;	}

				for (i = 0; i < IM_LOGPAR_CELL_DATA_CNT; i++)
				{
					p->WrBuffer[CurrentIndex++] = p->ImReadBuf[ImIndex + i] >> 8;
					p->WrBuffer[CurrentIndex++] = p->ImReadBuf[ImIndex + i] & 0xFF;
				}

			break;

			//ma LogSIM
			case IM_LOGSIM_TYPE:

				if (!(CurrentLogRec & 0x01))
				{	ImIndex = 0;	}
				else
				{	ImIndex = IM_LOGSIM_CELL_DATA_CNT;	}

				for (i = 0; i < IM_LOGSIM_CELL_DATA_CNT; i++)
				{
					p->WrBuffer[CurrentIndex++] = p->ImReadBuf[ImIndex + i] >> 8;
					p->WrBuffer[CurrentIndex++] = p->ImReadBuf[ImIndex + i] & 0xFF;
				}

			break;


		}

		CurrentIndex--;								// В циклах лишнее инкрементирование

		//for(i=0; i<2000; i++) asm("NOP");
		//p->WaitTime = 5;
		//while(p->WaitTime>0){}

		if (p->HardwareSrc!=imSrcModbus) {
			p->WaitTime = 5;
			while(p->WaitTime>0){};
		}

		p->Index = CurrentIndex;

		if (++CurrentLogRec < CurrentLogCnt)
		{
			p->CanReadNextRec = true;
		}
		else
		{
			p->CanReadNextRec = false;
			p->IsLogTransmit = false;
			p->FuncState = imInit;
		}

		return true;
	}
		
	return false;

}	  


__inline Bool FuncSix(TInfoModule *p, Uns StartParamIndex)
{
	Uns CurrentIndex = p->Index;
	Uns i;

	//for(i=0; i<200; i++) asm("NOP");
	//p->WaitTime = 10;
	//while(p->WaitTime>0){}

	// Передаем за раз определенное количество параметров
	for (i = 0; i < IM_PARAM_DOWNLOAD_NUM; i++)
	{
		p->WrBuffer[CurrentIndex++] = (p->DeviceDataPtr[StartParamIndex + i]) >> 8;
		p->WrBuffer[CurrentIndex++] = (p->DeviceDataPtr[StartParamIndex + i]) & 0xFF;
	}

	CurrentIndex--;

	p->Index = CurrentIndex;

	return true;	
}


void ImTimer(TInfoModule *p)
{
	if (p->WaitTime>0) p->WaitTime--;
	if (p->WaitTime<0) p->WaitTime=0;

	if (p->WaitAndroidCounter>0) p->WaitAndroidCounter--;
	if (p->WaitAndroidCounter> 100) p->WaitAndroidCounter=0;
	if (p->WaitAndroidCounter<0) p->WaitAndroidCounter=0;

	if (p->Timer > 0)	p->Timer--; 

	// Сброс инф. модуля по таймауту
	if (p->Timer == 1)
	{
		p->FuncState = imInit;

		CurrentLogCnt = 0;
		CurrentLogRec = 0;

		p->IsLogTransmit = false;
	}
}

void ImTimerIndex(TInfoModule *p)
{
	if (p->TimerIndex > 0)	p->TimerIndex--;

	// Сброс инф. модуля по таймауту
	if (p->TimerIndex == 1)
	{
		p->Index=0;
	}
}

//---------------------------------------------------------------------------------------------
void ImBufferReader(TInfoModule *im, Byte LogType, Uns RecordNum)
{
	static Uns PrevRecNum = -1U;
	static Bool EvLogMainCellRead = false;
	static Bool EvLogBufCellRead = false;
	Uns	*ImBuf, Addr;

	if (!im->CanReadNextRec)
		return;

	im->IsReadRecBusy = !(IsMemParReady() && IsMemLogReady());

	if (!im->IsReadRecBusy)
	{
		if ((LogType == IM_LOGEV_TYPE) && EvLogBufCellRead)
		{
			if (PrevRecNum == RecordNum)
			{
				EvLogBufCellRead = false;
				im->CanReadNextRec = false;
				im->IsBufReady = true;

				return;
			}
		}
		else if ((LogType != IM_LOGEV_TYPE) && (PrevRecNum == RecordNum))
		{
			im->CanReadNextRec = false;
			im->IsBufReady = true;
		}
		else
		{
			PrevRecNum = RecordNum;

			switch (LogType)
			{
				case IM_LOGEV_TYPE:

				if (!EvLogMainCellRead)
				{
					if (!(RecordNum & 0x01))
						{	ImBuf = &im->ImReadBuf[0];	}
					else
						{	ImBuf = &im->ImReadBuf[IM_READ_BUF_SIZE];	}

					Addr = LOG_EV_START_ADDR + RecordNum * LOG_EV_DATA_CNT * LOG_EV_DATA_CELL;
					ReadPar(Addr, ImBuf, LOG_EV_DATA_CNT);

					EvLogMainCellRead = true;
				}
				else
				{
					Addr = LOG_EV_BUF_START_ADDR + RecordNum * LOG_EV_BUF_DATA_CNT * LOG_EV_BUF_DATA_CELL;

					if (!(RecordNum & 0x01))
						{	ImBuf = &im->ImReadBuf[LOG_EV_DATA_CNT];	}
					else
						{	ImBuf = &im->ImReadBuf[LOG_EV_DATA_CNT + IM_READ_BUF_SIZE];		}

					ReadLog(Addr, ImBuf, (LOG_EV_BUF_DATA_CNT * LOG_EV_BUF_DATA_CELL));
					EvLogMainCellRead = false;
					EvLogBufCellRead = true;
				}

				break;

				case IM_LOGCMD_TYPE:

					if (!(RecordNum & 0x01))
					{	ImBuf = &im->ImReadBuf[0];	}
					else
					{	ImBuf = &im->ImReadBuf[LOG_CMD_DATA_CNT];	}

					Addr = LOG_CMD_START_ADDR + RecordNum * LOG_CMD_DATA_CNT;
					ReadPar(Addr, ImBuf, LOG_CMD_DATA_CNT);

				break;

				case IM_LOGPARAMS_TYPE:

					if (!(RecordNum & 0x01))
					{	ImBuf = &im->ImReadBuf[0];	}
					else
					{	ImBuf = &im->ImReadBuf[LOG_PARAM_DATA_CNT];	}

					Addr = LOG_PARAM_START_ADDR + RecordNum * LOG_PARAM_DATA_CNT;
					ReadPar(Addr, ImBuf, LOG_PARAM_DATA_CNT);

				break;

				//ma LogSIM
				case IM_LOGSIM_TYPE:

					if (!(RecordNum & 0x01))
					{	ImBuf = &im->ImReadBuf[0];	}
					else
					{	ImBuf = &im->ImReadBuf[LOG_SIM_DATA_CNT];	}

					Addr = LOG_SIM_START_ADDR + RecordNum * LOG_SIM_DATA_CNT;
					ReadPar(Addr, ImBuf, LOG_SIM_DATA_CNT);

				break;
			}
		}
	}
}

//----------------------------------------------------------------------
// TEST
// TEST
void ImBufferReaderTest(TInfoModule *im, Byte LogType, Uns RecordNum)
{
	static Uns PrevRecNum = -1U;
	static Bool EvLogMainCellRead = false;
	static Bool EvLogBufCellRead = false;
	Uns	*ImBuf, Addr;
	Uns i=0;

	if (!im->CanReadNextRec)
		return;

	im->IsReadRecBusy = false;//!(IsMemParReady() && IsMemLogReady());

	if (!im->IsReadRecBusy)
	{
		if ((LogType == IM_LOGEV_TYPE) && EvLogBufCellRead)
		{
			if (PrevRecNum == RecordNum)
			{
				EvLogBufCellRead = false;
				im->CanReadNextRec = false;
				im->IsBufReady = true;

				return;
			}
		}
		else if ((LogType != IM_LOGEV_TYPE) && (PrevRecNum == RecordNum))
		{
			im->CanReadNextRec = false;
			im->IsBufReady = true;
		}
		else
		{
			PrevRecNum = RecordNum;

			switch (LogType)
			{
				case IM_LOGEV_TYPE:

				if (!EvLogMainCellRead)
				{
					if (!(RecordNum & 0x01))
						{	ImBuf = &im->ImReadBuf[0];
						for(i=0; i<LOG_EV_DATA_CNT; i++) im->ImReadBuf[i]=1+i;
						}
					else
						{	ImBuf = &im->ImReadBuf[IM_READ_BUF_SIZE];
							for(i=0; i<LOG_EV_DATA_CNT; i++) im->ImReadBuf[i+IM_READ_BUF_SIZE]=1+i;
						}
					//TODO почему закоментированны строки чтения данных с FLASH памяти
					Addr = LOG_EV_START_ADDR + RecordNum * LOG_EV_DATA_CNT * LOG_EV_DATA_CELL;
					//ReadPar(Addr, ImBuf, LOG_EV_DATA_CNT);
					//for(i=0; i<LOG_EV_DATA_CNT; i++) im->ImReadBuf[i]=1+i;

					EvLogMainCellRead = true;
				}
				else
				{
					Addr = LOG_EV_BUF_START_ADDR + RecordNum * LOG_EV_BUF_DATA_CNT * LOG_EV_BUF_DATA_CELL;

					if (!(RecordNum & 0x01))
						{	ImBuf = &im->ImReadBuf[LOG_EV_DATA_CNT];
						for(i=0; i<(LOG_EV_BUF_DATA_CNT * LOG_EV_BUF_DATA_CELL); i++) im->ImReadBuf[LOG_EV_DATA_CNT]=10+i;
						}
					else
						{	ImBuf = &im->ImReadBuf[LOG_EV_DATA_CNT + IM_READ_BUF_SIZE];
						for(i=0; i<(LOG_EV_BUF_DATA_CNT * LOG_EV_BUF_DATA_CELL); i++) im->ImReadBuf[LOG_EV_DATA_CNT+IM_READ_BUF_SIZE]=10+i;}

					//ReadLog(Addr, ImBuf, (LOG_EV_BUF_DATA_CNT * LOG_EV_BUF_DATA_CELL));

					EvLogMainCellRead = false;
					EvLogBufCellRead = true;
				}

				break;

				case IM_LOGCMD_TYPE:

					if (!(RecordNum & 0x01))
					{	ImBuf = &im->ImReadBuf[0];
						for(i=0; i<LOG_CMD_DATA_CNT; i++) im->ImReadBuf[i]=20+i;
					}
					else
					{	ImBuf = &im->ImReadBuf[LOG_CMD_DATA_CNT];
						for(i=0; i<LOG_CMD_DATA_CNT; i++) im->ImReadBuf[LOG_CMD_DATA_CNT+i]=20+i;
					}

					//Addr = LOG_CMD_START_ADDR + RecordNum * LOG_CMD_DATA_CNT;
					//ReadPar(Addr, ImBuf, LOG_CMD_DATA_CNT);

					//for(i=0; i<LOG_CMD_DATA_CNT; i++) im->ImReadBuf[i]=20+i;

				break;

				case IM_LOGPARAMS_TYPE:

					if (!(RecordNum & 0x01))
					{	ImBuf = &im->ImReadBuf[0];
						for(i=0; i<LOG_PARAM_DATA_CNT; i++) im->ImReadBuf[i]=30+i;
					}
					else
					{	ImBuf = &im->ImReadBuf[LOG_PARAM_DATA_CNT];
						for(i=0; i<LOG_PARAM_DATA_CNT; i++) im->ImReadBuf[LOG_PARAM_DATA_CNT+i]=30+i;
					}

					//Addr = LOG_PARAM_START_ADDR + RecordNum * LOG_PARAM_DATA_CNT;
					//ReadPar(Addr, ImBuf, LOG_PARAM_DATA_CNT);

					//for(i=0; i<LOG_PARAM_DATA_CNT; i++) im->ImReadBuf[i]=30+i;

				break;

				//ma LogSIM
				case IM_LOGSIM_TYPE:

					if (!(RecordNum & 0x01))
					{	ImBuf = &im->ImReadBuf[0];
						for(i=0; i<LOG_SIM_DATA_CNT; i++) im->ImReadBuf[i]=40+i;
					}
					else
					{	ImBuf = &im->ImReadBuf[LOG_SIM_DATA_CNT];
						for(i=0; i<LOG_SIM_DATA_CNT; i++) im->ImReadBuf[LOG_SIM_DATA_CNT+i]=40+i;
					}

					//Addr = LOG_SIM_START_ADDR + RecordNum * LOG_SIM_DATA_CNT;
					//ReadPar(Addr, ImBuf, LOG_SIM_DATA_CNT);
					//for(i=0; i<LOG_SIM_DATA_CNT; i++) im->ImReadBuf[i]=40+i;
				break;
			}
		}
	}
}
