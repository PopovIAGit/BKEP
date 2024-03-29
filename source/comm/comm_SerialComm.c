/*======================================================================
��� �����:          comm_SerialComm.c
�����:
������ �����:
���� ���������:
��������:
������ ModBus
======================================================================*/

#include "config.h"
#include "core.h"
#include "comm.h"
#include "comm_SerialComm.h"
#include "g_Ram.h"
#include "stat.h"

Bool MbBkpConnect=False;
Bool MbAsuConnect=False;
Bool MbShnConnect=False;
Bool MbBtConnect=False;

static Uns BaudRates[7] = SCI_DEFAULT_BAUD_RATES;
static Uns BrrValues[7] = SCI_DEFAULT_BRR_VALUES;

//#define TEK_MB_START_ADDR		40000
//#define CHECK_TEK_MB_ADDR(Addr)	(((Addr>>8) & 0x9C) == 0x9C)

#define IM_MB_START_ADDR		65123
#define CHECK_IM_MB_ADDR(Addr)	(Addr==IM_MB_START_ADDR)
#define IM_MB_START_DATA_ADDR		65124
#define CHECK_IM_MB_DATA_ADDR(Addr)	(Addr==IM_MB_START_DATA_ADDR)

__inline Byte UpdatePacket(TMbPacket *Packet);
__inline Byte WriteData(Uns Addr, Uns *Data, Uns Count);
//__inline Byte WriteDataRegsTek(Uns Addr, Uns *Data, Uns Count);

//---------------------------------------------------
void InitChanelAsuModbus(TMbHandle hPort)
{
	hPort->Params.ChannelID = ASU_SCI_ID;//
	hPort->Params.Mode     	= MB_SLAVE;//
	hPort->Params.Slave    	= g_Ram.ramGroupB.RsStation;//
	hPort->Params.BaudRate	= BaudRates[g_Ram.ramGroupB.RsBaudRate];//
	hPort->Params.UartBaud 	= BrrValues[g_Ram.ramGroupB.RsBaudRate];//
	hPort->Params.Parity   	= g_Ram.ramGroupB.RsMode;//

/*	hPort->Params.RetryCount  = 0;
	hPort->Params.Scale       = 2;//MB_SCALE;
//	hPort->Params.ConnTimeout = Serial->RsIndicTime * 100;
	hPort->Params.ConnTimeout = 2;//10
	hPort->Params.RxDelay     = 10;//
//	hPort->Params.TxDelay     = Serial->RsWaitTime - 3;
	hPort->Params.TxDelay     = 0;//10
	hPort->Params.AckTimeout  = 6;//1000
	hPort->Params.TrEnable    = &AsuMbSetTr;
	hPort->Frame.TimerPre.Timeout = 1; //
	*/
	hPort->Params.RetryCount  = 0;
	hPort->Params.Scale       = MB_SCALE;
	hPort->Params.ConnTimeout = 40;
	hPort->Params.RxDelay     = 10;
	hPort->Params.TxDelay     = 10;
	hPort->Params.AckTimeout  = 1;//1000;
	hPort->Params.TrEnable    = &AsuMbSetTr;
	hPort->Frame.TimerPre.Timeout = 1;


	hPort->Params.HardWareType	= UART_TYPE;
	hPort->Packet.ParamMode	= UART_TYPE;

	hPort->Params.TrEnable(0);

	//hPort->HardwareSetup = SerialCommInit;

}
//---------------------------------------------------
void InitChanelShnModbus(TMbHandle hPort)
{
	hPort->Params.ChannelID   = SHN_SCI_ID;
	hPort->Params.Mode     = MB_MASTER;
	hPort->Params.Slave    = 1;//g_Ram.ramGroupB.RS_STATION;
	hPort->Params.BaudRate = BaudRates[3];//BaudRates[g_Ram.ramGroupB.RS_BAUD_RATE];
	hPort->Params.UartBaud = BrrValues[3];//BrrValues[g_Ram.ramGroupB.RS_BAUD_RATE];
	hPort->Params.Parity   = 0;//g_Ram.ramGroupB.RS_MODE;

	hPort->Params.RetryCount  = 2;
	hPort->Params.Scale       = MB_SCALE;
	hPort->Params.ConnTimeout = 40;//400;
	hPort->Params.RxDelay     = 10;
	hPort->Params.TxDelay     = 10;//200;
	hPort->Params.AckTimeout  = 1;//2000;//1000;
	hPort->Params.TrEnable    = &ShnMbSetTr;
	hPort->Frame.TimerPre.Timeout = 1;

	hPort->Params.HardWareType	= UART_TYPE;
	hPort->Packet.ParamMode	= UART_TYPE;
}
//---------------------------------------------------
void InitChanelBtModbus(TMbHandle hPort)
{
	hPort->Params.ChannelID= BT_MCBSP_ID;
	hPort->Params.Mode     = MB_SLAVE;
	hPort->Params.Slave    = 1;//g_Ram.ramGroupB.RS_STATION;
	hPort->Params.BaudRate = 1152;//BaudRates[g_Ram.ramGroupB.RS_BAUD_RATE];
	hPort->Params.UartBaud = 0;//BrrValues[g_Ram.ramGroupB.RS_BAUD_RATE];
	hPort->Params.Parity   = 0;//g_Ram.ramGroupB.RS_MODE;

	hPort->Params.RetryCount  = 0;
	hPort->Params.Scale       = 2;
	hPort->Params.ConnTimeout = 40;
	hPort->Params.RxDelay     = 10;
	hPort->Params.TxDelay     = 10;
	hPort->Params.AckTimeout  = 1;//1000;
	hPort->Params.TrEnable    = &BtMbSetTr;
	hPort->Frame.TimerPre.Timeout = 1;

	/*
	 *
	hPort->Params.RetryCount  = 0;
	hPort->Params.Scale       = 2;
	hPort->Params.ConnTimeout = 2;
	hPort->Params.RxDelay     = 2;
	hPort->Params.TxDelay     = 10;
	hPort->Params.AckTimeout  = 2;//1000;
	hPort->Params.TrEnable    = &BtMbSetTr;
	hPort->Frame.TimerPre.Timeout = 2;
	 * */

	hPort->Params.HardWareType	= MCBSP_TYPE;
	hPort->Packet.ParamMode	= MCBSP_TYPE;

	//hPort->Params.TrEnable(0);

}

void SerialCommInit(TMbHandle hPort)
{
	ModBusSlaveReset(hPort);// ModBusReset
}
//---------------------------------------------------
void SerialCommTimings(TMbHandle hPort)
{
	ModBusTimings(hPort);
}
//---------------------------------------------------
void ModBusSlaveReset(TMbHandle hPort)
{
	ModBusInit(hPort);
}
//---------------------------------------------------
void ModBusUpdate(TMbHandle hPort)
{
	TMbPacket *Packet = &hPort->Packet;

	if (IsSlave())
	{
		if (Packet->Request)
		{
			Packet->Exception = UpdatePacket(Packet);
			Packet->Response  = Packet->Request;
			Packet->Request   = 0;
			if (hPort->Params.HardWareType==MCBSP_TYPE)
			{
				GpioDataRegs.GPATOGGLE.bit.GPIO27=1;//ENABLE_BLUETOOTH=1;//off
			}
		}
	}

	hPort->Serial.RsState = Packet->Exception; //???MbConnect = !Packet->Exception;
	
	ModBusInvoke(hPort);
}
//---------------------------------------------------
void ModBusControl(TMbHandle hPort)
{
	//if (hPort->Serial.RsReset) {ModBusSlaveReset(hPort); hPort->Serial.RsReset = 0;}

//	if (Serial->PduRead)  {Pdu.Read  = Serial->PduRead;  Serial->PduRead  = 0;}
//	if (Serial->PduWrite) {Pdu.Write = Serial->PduWrite; Serial->PduWrite = 0;}
}

__inline Byte UpdatePacket(TMbPacket *Packet)
{
	Uns Addr=0, Res=0, Tmp=0, i=0;

		Addr = Packet->Addr;

		Tmp  = Packet->Addr + Packet->Count - 1;

		if (Tmp <= (RAM_DATA_SIZE + 5)) {Res = 1;}
		else if (CHECK_IM_MB_ADDR(Addr))	{Res = 5;}
		else if (CHECK_IM_MB_DATA_ADDR(Addr))	{Res = 6;}
		else if (Addr==65125)	{Res = 6;}
		else if (Addr==65345)	{Res = 7;}
		else {Res = 0;}

		if (!Res && Packet->Request!=MB_REPORT_ID) {return EX_ILLEGAL_DATA_ADDRESS;}

		switch(Packet->Request)
		{

			case MB_REPORT_ID:
				Packet->Data[0] = (DEVICE_ID>>8)&0x00FF;
				Packet->Data[1] = DEVICE_ID&0x00FF;
				Packet->Data[2] = (DEVICE_YEAR>>8)&0x00FF;
				Packet->Data[3] = DEVICE_YEAR&0x00FF;
				//
				Packet->Data[4] = (g_Ram.ramGroupA.VersionPO>>8)&0x00FF;
				Packet->Data[5] = g_Ram.ramGroupA.VersionPO&0x00FF;
				if (g_Ram.ramGroupH.Password1!=0) Packet->Data[6] = 1; else Packet->Data[6] = 0;
				if (g_Ram.ramGroupH.Password2!=0) Packet->Data[7] = 1; else Packet->Data[7] = 0;

				break;
			case MB_READ_REGS:
				switch(Res)
				{
					case 1:memcpy(Packet->Data, /*ToUnsPtr*/(&g_Ram.ramGroupT.TechReg) + Packet->Addr, Packet->Count);
					    //if (Packet->Addr==522) memcpy(Packet->Data, &g_Ram.ramGroupA.Rsvd1, Packet->Count);
					    //else memcpy(Packet->Data, /*ToUnsPtr*/(&g_Ram.ramGroupT.TechReg) + Packet->Addr, Packet->Count);
							//Port->Frame.Exception = ReadRegs(Port, (Uint16 *)&Ram, Addr, Count);
							break;
					case 5:
						//Packet->Count = g_Stat.Im.IndexMb;
						memcpy(Packet->Data, ToUnsPtr(&g_Stat.Im.WrBufferMb[0]), Packet->Count);
						g_Stat.Im.HardwareSrc=imSrcModbus;
						g_Stat.Im.AddrData = 0;
						g_Stat.Im.CanSendDataMb = 0;
						break;
					case 6:
						if (Addr==65125) g_Stat.Im.AddrData = g_Stat.Im.AddrData - Packet->Count;
						memcpy(Packet->Data, ToUnsPtr(&g_Stat.Im.WrBufferMb[g_Stat.Im.AddrData]), Packet->Count);
						g_Stat.Im.AddrData = g_Stat.Im.AddrData + Packet->Count;
						if (g_Stat.Im.AddrData>=g_Stat.Im.IndexMb)
						{
							g_Stat.Im.CanSendData = false;
							g_Stat.Im.CanSendDataMb = 0;
							g_Stat.Im.AddrData = 0;
							*g_Stat.Im.IsTxBusy = false;
						}
						break;
							//Port->Frame.Exception = ReadRegs(Port, (Uint16 *)&RamTek, (Addr - TEK_MB_START_ADDR), Count);
					//		break;
					default: return EX_ILLEGAL_FUNCTION;
				}
				break;
			case MB_WRITE_REGS:
				switch(Res)
				{
					case 1:
						if (Addr>=REG_TASKCLOSE && Addr<=REG_RSRESET){
							if (Packet->ParamMode==UART_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
							else if (Packet->ParamMode==MCBSP_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_BLUETOOTH;
						}
						if (Addr == REG_SET_DEFAULTS){
							if (Packet->ParamMode==UART_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
							else if (Packet->ParamMode==MCBSP_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_BLUETOOTH;
							//g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
						}
						if (Addr == REG_COM_REG)
						{
							if (Packet->ParamMode==UART_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
							else if (Packet->ParamMode==MCBSP_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_BLUETOOTH;
						}
						return WriteData(Packet->Addr, Packet->Data, Packet->Count);
						//if (!Port->Frame.Exception) SerialCommRefresh();
					case 5:
						for(i=0; i<Packet->Count; i++)
						{
							g_Stat.Im.RdBuffer[i] = g_Comm.mbAsu.Frame.Buf[8+i*2];
						}
						//memcpy(g_Stat.Im.RdBuffer, ToUnsPtr(&g_Comm.mbAsu.Frame.Buf[4]), Packet->Count);
						g_Stat.Im.Index = Packet->Count;
						g_Stat.Im.HardwareSrc=imSrcModbus;
						g_Stat.Im.CanSendDataMb = 0;
						//*g_Stat.Im.IsTxBusy = true;
						//g_Stat.Im.CanSendData = false;
						//g_Stat.Im.AddrData = 0;
						return 0;
						//return WriteRegsTek(Port, (Uint16 *)&RamTek, (Uint16 *)&Ram, (Addr - TEK_MB_START_ADDR), Count);
						//return WriteDataRegsTek(Packet->Addr, Packet->Data, Packet->Count);
						//if (!Port->Frame.Exception) SerialCommRefresh();
					case 7:
						//������� ������ � ������ id bluetooth ���������� ������� ������������ � �����
						//�������� � ��������� � ��������� ����
						//WriteLogConnectSimID(Packet->Data, Packet->Count);
						Packet->Request = 0;
						Packet->Response = 0;
						g_Stat.Im.RdBuffer[0]=0;
						for(i=0; i<Packet->Count; i++)
						{
							g_Stat.LogSim.NewSimID[i] = Packet->Data[i];
						}
						for(i=0; i<Packet->Count; i++)
						{
							Packet->Data[i] = 0;
							g_Comm.mbBt.Frame.Buf[i]=0;
						}
						Packet->Count = 0;
						g_Stat.LogSim.ExecFlag = true;
						McBsp_tx_disable(0);
						McBsp_rx_enable(0);

						return 0;
						//return WriteData(Packet->Addr, Packet->Data, Packet->Count);
					default: return EX_ILLEGAL_FUNCTION;
				}
				case MB_WRITE_REG:

					switch(Res)
							{
							case 1:
							if (Addr>=REG_TASKCLOSE && Addr<=REG_RSRESET){
								if (Packet->ParamMode==UART_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
								else if (Packet->ParamMode==MCBSP_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_BLUETOOTH;
							}
							if (Addr == REG_SET_DEFAULTS){
								if (Packet->ParamMode==UART_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
								else if (Packet->ParamMode==MCBSP_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_BLUETOOTH;
								//g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
							}
							if (Addr == REG_COM_REG)
							{
								if (Packet->ParamMode==UART_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_SERIAL;
								else if (Packet->ParamMode==MCBSP_TYPE) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_BLUETOOTH;
							}
							return WriteData(Packet->Addr, Packet->Data, 1);
							default: return EX_ILLEGAL_FUNCTION;
							}
			default: return EX_ILLEGAL_FUNCTION;
		}
		return 0;
}


//---------------------------------------------------
/*__inline Byte WriteDataRegsTek(Uns Addr, Uns *Data, Uns Count)
{
	Uns  *Dest = ToUnsPtr(&g_RamTek) + Addr;

	if ((Addr == 4 || Addr == 22) && Count==1)
	{
		memcpy(Dest, Data, Count);
		//memcpy(&Data[Addr], MbTmpData, Count);
		return FR_SUCCESS;
	}

	return EX_ILLEGAL_DATA_ADDRESS;
}*/
//---------------------------------------------------
__inline Byte WriteData(Uns Addr, Uns *Data, Uns Count)
{
	struct MENU_DCR Dcr;
	struct MENU_VAL_CFG *Val = &Dcr.Config.Val;
	Uns  i, *Dest = ToUnsPtr(&g_Ram) + Addr;
	Bool Nvm = False;
	

	for (i=0; i < Count; i++)
	{
		GetDcr(Addr + i, &Dcr);
		if (Val->ReadOnly) {return EX_ILLEGAL_FUNCTION;}
		if (!CheckRange(Data[i], Dcr.Min, Dcr.Max)) 
			{return EX_ILLEGAL_DATA_VALUE;}
		if (Val->Memory)
		{
			Nvm = true;
			g_Stat.LogParam.MbBuffer[g_Stat.LogParam.MbIndex] = i + Addr;			// ��������� ����� ���������, ���������������� ������
			g_Stat.LogParam.MbIndex++;
		}
		if (Addr==REG_TASK_TIME || Addr==REG_TASK_DATE)
		{
			g_Stat.LogParam.MbBuffer[g_Stat.LogParam.MbIndex] = i + Addr;			// ��������� ����� ���������, ���������������� ������
			g_Stat.LogParam.MbIndex++;
		}
		if ((Addr >= REG_TASKCLOSE)&&(Addr <= REG_REVCLOSE))
		{
			g_Stat.LogParam.MbBuffer[g_Stat.LogParam.MbIndex] = i + Addr;			// ��������� ����� ���������, ���������������� ������
			g_Stat.LogParam.MbIndex++;
		}

		//MbTmpData[i] = Tmp;
	}

	//TODO ���������� ������
	if (Addr == REG_CODE){
		g_Core.VlvDrvCtrl.EvLog.Source = 0;
		if ( UpdateCode(REG_PASSW1_NEW, Addr, *Data, DEF_USER_PASS) )
			return FR_SUCCESS;
	}

	if (Addr == REG_FCODE){
		g_Core.VlvDrvCtrl.EvLog.Source = 0;
		if ( UpdateCode(REG_PASSW2_NEW, Addr, *Data, DEF_FACT_PASS) )
			return FR_SUCCESS;
	}

	//if (!g_Core.menu.EnableEdit(Val->PaswwPrt)) return FR_SUCCESS;

	//	��������� ���� �� ������ ������� �� ����������������� ����������.
	//	���� ���� ������, � ����� �� ��������, �� ������ ����������

	if (Addr == REG_CONTROL)
	{
		// ���� ����� ����������, �� �� ���������� �������, ����� ������� ����
		if (!(g_Core.VlvDrvCtrl.ActiveControls & CMD_SRC_SERIAL) && (*Data != vcwStop))
			return EX_ILLEGAL_DATA_VALUE;
	}

	if (!g_Core.menu.EnableEdit(Val->PaswwPrt))
	{
		return FR_SUCCESS;
	}

	//�������� �� ��������� ������ �������� � ������
	if (Nvm && !IsMemParReady()) return EX_SLAVE_DEVICE_BUSY;

	memcpy(Dest, Data, Count);

	if (Nvm)
	{
		//if ( WriteToEeprom(Addr, Dest, Count) )
		if (ReadWriteEeprom(&Eeprom1,F_WRITE,Addr,Dest,Count))
		{
			RefreshParams(Addr);
			return FR_SUCCESS;
		}
	}

	return FR_SUCCESS;

	//-----------------------------------
	//	��������� ���� �� ������ ������� �� ����������������� ����������.
	//	���� ���� ������, � ����� �� ��������, �� ������� ����������

	/*if (InRange(REG_CONTROL, Addr, Addr + (Count - 1)))
	{
		Tmp = MbTmpData[REG_CONTROL - Addr];
		// ���� ����� ����������, �� �� ���������� �������, ����� ������� ����
		if (!(Mcu.ActiveControls & CMD_SRC_SERIAL) && (Tmp != 0) && Tmp != vcwStop)
			return EX_ILLEGAL_DATA_VALUE;
		if (Tmp == vcwStop) Mcu.Tu.Ready = False;
	}

	if (Nvm && !IsMemParReady()) return EX_SLAVE_DEVICE_BUSY;

	memcpy(&Data[Addr], MbTmpData, Count);

	if (Nvm)
	{
		WritePar(Addr, &Data[Addr], Count);
		RefreshData();
	}

	return 0;*/

}
//---------------------------------------------------







