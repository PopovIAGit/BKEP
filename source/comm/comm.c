/*======================================================================
��� �����:          comm.c
�����:
������ �����:
���� ���������:
��������:
�������� ������ ��������� ������������
======================================================================*/

#include "comm.h"
#include "g_Ram.h"

#define TEK_DEVICE_FAULT_MASK	0xF910				// ����� �� ������� �������� ���������� ��� ������� ���
//#define TEK_DEVICE_FAULT_MASK	0xF990

TComm	g_Comm;
Uint16 ASU_Data[10];
Uint16 SHN_Data[10];


//Uns DigitCmdModeUpdate (Uns *Output);
static char ReadRegs(TMbPort *Port, Uint16 *Data, Uint16 Addr, Uint16 Count);
//---------------------------------------------------
void Comm_Init(TComm *p)
{
	// ����-������������
	Comm_TuTsInit(&p->digitInterface);

	DigitalInputInit(&p->digitInput, (g_Ram.ramGroupB.TuTime * Prd50HZ), ICREMENT_DIV, DECREMENT_DIV);

	// ����� �������� ����������:
	Comm_LocalControlInit (&p->localControl);

	//������������� ������ ����� ����� ��� � ���
	SciMasterConnBetweenBlockInit(&g_Comm.mbBkp);

	//����� ����� � ������� ������� ������������
	InitChanelAsuModbus(&g_Comm.mbAsu);

	InitChanelShnModbus(&g_Comm.mbShn);
	InitChanelBtModbus(&g_Comm.mbBt);

	//����������� ���� � ��� �� ���������� ����� ��� �������� Bluetooth � Modbus
	g_Comm.Bluetooth.HardWareType = g_Comm.mbBt.Params.HardWareType;	// ������
	g_Comm.Bluetooth.ChannelID    = g_Comm.mbBt.Params.ChannelID;		// ������ ������
	g_Comm.Bluetooth.BaudRate	  = g_Comm.mbBt.Params.BaudRate;		//
	g_Comm.Bluetooth.UartBaud	  = g_Comm.mbBt.Params.UartBaud;		//
	g_Comm.Bluetooth.Mode	  	  = g_Comm.mbBt.Params.Mode;			//
	g_Comm.Bluetooth.Parity	   	  = g_Comm.mbBt.Params.Parity;			//
	SerialCommInit(&g_Comm.mbBt);
	g_Comm.mbBt.Frame.TimerPost.Timeout=g_Comm.mbBt.Frame.TimerPre.Timeout*2;
	InitChanelBt(&g_Comm.Bluetooth);

	SerialCommInit(&g_Comm.mbAsu);
	SerialCommInit(&g_Comm.mbShn);
	Comm_LocalControlInit(&g_Comm.localControl);

	// ��������� ������ ��������

	p->Shn.SHN_ReadFlag = 0;
	p->Shn.SHN_Mode = 1;
	p->btn_reset_alarmFlag = 0;
}

//---------------------------------------------------
void Comm_Update(TComm *p)
{

	Uint16 lAddr;

	if (g_Comm.Bluetooth.ModeProtocol == 0)
	{
		ModBusUpdate(&g_Comm.mbAsu); 	// slave ����� ����� � ������� ������� ���
	}

	if (g_Comm.Bluetooth.ModeProtocol == 0)
	{
		ModBusUpdate(&g_Comm.mbShn);  // master ����� ����� � ����������� �������� �����
		if (!p->Shn.SHN_Busy)
		{
			/*if(p->Shn.SHN_TaskFunc && (p->Shn.SHN_TaskStage == 1))
			{
				p->mbShn.Packet.Addr = p->Shn.SHN_TaskAddr;
				p->mbShn.Packet.Count = 1;
				if(p->Shn.SHN_TaskFunc == MB_WRITE_REGS) p->mbShn.Packet.Data[0] = *p->Shn.SHN_TaskData;
				p->mbShn.Packet.Request = p->Shn.SHN_TaskFunc;
				p->Shn.SHN_TaskEx = 0;
				p->Shn.SHN_Busy = 0;
			}
			else*/ if (p->Shn.SHN_WriteFlag)
			{
				//p->Shn.SHN_ReadFlag = 0;
				if (p->Shn.SHN_Mode==1)
				{
					p->mbShn.Packet.Addr = ATS48_CONTROL_REG;
					p->mbShn.Packet.Count = 1;
					p->mbShn.Packet.Data[0] = p->SHN_Regs.Control.all;
					//if(LVS_flag ==2) LVS_flag = 3;
				}
				else if (p->Shn.SHN_Mode==0)
				{
					p->Shn.SHN_Mode=1;
					//p->mbShn.Packet.Addr = 0;
					//p->mbShn.Packet.Count = 10;
					//memcpy(p->mbShn.Packet.Data, SHN_Data, p->mbShn.Packet.Count);
				} else if (p->Shn.SHN_Mode==2) {

				}
				p->mbShn.Packet.Request = MB_WRITE_REGS;
				p->Shn.SHN_Busy = 1;
			}
			else if (p->Shn.SHN_ReadFlag==1)
			{
				//p->Shn.SHN_WriteFlag=0;
				if(p->Shn.SHN_Mode)
				{
					p->mbShn.Packet.Addr = ATS48_STATUS_REG;
					p->mbShn.Packet.Count = 3;
				}
				else
				{
					//p->Shn.SHN_Mode=1;
					//p->mbShn.Packet.Addr = 0;
					//p->mbShn.Packet.Count = 10;
				}

				p->mbShn.Packet.Request = MB_READ_REGS;
				p->Shn.SHN_Busy = 1;
			}
		}
		else if (!p->mbShn.Packet.Request && !p->mbShn.Frame.WaitResponse)
		{
			p->Shn.SHN_Busy = 0;
			//g_Comm.Shn.SHN_WriteFlag=0;
		}
		else if (p->mbShn.Packet.Response)
		{
			if(p->mbShn.Packet.Exception)
			{
				if(p->Shn.SHN_TaskStage == 1)
				{
					p->Shn.SHN_TaskEx = p->mbShn.Packet.Exception;
					p->Shn.SHN_TaskStage = 2;
				}
				p->mbShn.Packet.Exception = 0;
			}
			else switch (p->mbShn.Packet.Response)
			{
			case MB_READ_REGS:

				if(p->Shn.SHN_Mode)
				{
					if (p->mbShn.Packet.Addr == ATS48_STATUS_REG)
					{
						p->SHN_Regs.Status.all = p->mbShn.Packet.Data[0];
						p->SHN_Regs.ExStatus.all = p->mbShn.Packet.Data[1];
						p->SHN_Regs.Ex2Status.all = p->mbShn.Packet.Data[2];
					}
					if (p->mbShn.Packet.Addr == 402)
					{
						SHN_Data[0]= p->mbShn.Packet.Data[0];
					}
				}
				else
				{
					memcpy(SHN_Data, p->mbShn.Packet.Data, p->mbShn.Packet.Count);
				}
				break;
			case MB_WRITE_REGS:
				if(p->mbShn.Packet.Addr == ATS48_CONTROL_REG) p->Shn.SHN_WriteFlag = 0;
				if(p->mbShn.Packet.Addr == ATS48_BIG_CONTROL_REG) p->Shn.SHN_WriteFlag = 0;

				p->Shn.SHN_WriteFlag = 0;
				break;
			}
			p->mbShn.Packet.Response = 0;
			p->Shn.SHN_Busy = 0;
		}

	}

	//SciMasterConnBetweenBlockUpdate(&g_Comm.mbBkp);// master ����� ����� �

	BluetoothWTUpdate(&g_Comm.Bluetooth); //������� Bluetooth
	if (g_Comm.Bluetooth.ModeProtocol>0) ModBusUpdate(&g_Comm.mbBt);  // slave

}
static char ReadRegs(TMbPort *Port, Uint16 *Data, Uint16 Addr, Uint16 Count)
{
	memcpy(Port->Packet.Data, &Data[Addr], Count);
	return 0;
}

void Comm_50HzCalc(TComm *p)
{
	//	������� � ��� !!!
	// �������� ������� � ������ ����������

	// �������� ������� � ����� ���
	switch (Comm_LocalButtonUpdate(&p->localControl))
	{
	case (BTN_STOPMU_BIT | BTN_STOPDU_BIT):
		g_Ram.ramGroupH.CmdButton = KEY_STOP;
		break;
	case BTN_OPEN_BIT:
		//if (g_Ram.ramGroupH.CmdKey == KEY_STOP) break;
		g_Ram.ramGroupH.CmdButton = KEY_OPEN;
		break;
	case BTN_CLOSE_BIT:
		//if (g_Ram.ramGroupH.CmdKey == KEY_STOP) break;
		g_Ram.ramGroupH.CmdButton = KEY_CLOSE;
		break;
	default:
		g_Ram.ramGroupH.CmdButton = KEY_NONE;
	}

	//������ ������ ������ �� ������� ������
	if ((BTN_RESET_ALARM == 0) && (p->btn_reset_alarmFlag == 0))
	{
		g_Ram.ramGroupD.PrtReset = 1;
		p->btn_reset_alarmFlag = 1;
		g_Peref.Display.data = 999;

	}
	else if (BTN_RESET_ALARM == 1 && p->btn_reset_alarmFlag == 1)
	{
		p->btn_reset_alarmFlag = 0;
	}

	//CommandUpdate(&g_Comm);
}

//-----------��������� ���������� ������ -----------------------------
void Comm_CommandUpdate(TComm *p)
{
	static Byte clrReset=0;

	p->outputCmdReg = 0;

	//����� �� ������ ��������� �� �� � ����� ��
//	Comm_TuTsUpdate(&p->digitInterface); // ��������������, ����������������

	//���� �������������� ���������
	p->digitInput.input = p->digitInterface.Inputs.all;
	DigitalInpUpdate(&p->digitInput); // ����� ������� ��������� ��������� �������, ������ �� �����
	g_Ram.ramGroupA.StateTu.all = p->digitInput.output;

	if (!(p->digitInput.output & 0x20)) // ���� �� ��������� �� ��������� �� ���������� ������� ����� �����.
	{
		p->outputCmdReg = (p->digitInput.output & 0xC);
	}
	else
	{
		p->outputCmdReg = (p->digitInput.output & 0xF);
	}

	g_Ram.ramGroupH.TuState = p->outputCmdReg;

	if ((p->digitInput.output & 0x40) >> 6 == 1)
	{
		if (clrReset==0) g_Ram.ramGroupD.PrtReset = 1;
		clrReset=1;
	}
	else if ((p->digitInput.output & 0x040) >> 6 == 0)
	{
		clrReset=0;
	}
//	p->outputCmdReg |= DigitCmdModeUpdate(&p->digitInput.output);
// 	if (p->outputCmdReg)
//	{
//		p->outputCmdSrc = CMD_SRC_DIGITAL;
//	}
}
//---------------------------------------------------
// ������� ��������� ������ ������ ������ ����������� ���������� (����� - ����������, �������������)
/*
Uns DigitCmdModeUpdate (Uns *Output)
{
	static Uns prevOutput;
	Uns result=0;
	Uns i=0;

	for(i=0; i<7; i++)
	{
		if((g_Ram.ramGroupB.DigitalMode.all>>i)&0x01) //�������������
		{

			if ((*Output)&0x03)	    									// ���� ������ ���� ������� �� ��������
			{
				if ((((*Output)>>i)&0x01) != (((prevOutput)>>i)&0x01))	// ���� ������ ���������
					result |= (0x01<<i);//*Output;						// ��� ������ - ���� � ����
				else													// ����� (������ �� ���������)
					result &= ~(0x01<<i);								// ������� ������ �� ������
			}
			else														// ���� ������ ����
			{
				if ((*Output)&0x03)										// �� �� ���������� �������� ���� ������� �� ��������
					result |= DIN_STOP_OPEN_BIT;    					// �������, ��� ������ ������� ����
				else 													// ����� (������ �� ���������)
					result &= ~(0x01<<i);								// ������� ������ �� ������
			}
		}
		else
		{
			if ((((*Output)>>i)&0x01) != (((prevOutput)>>i)&0x01))		// ���� ������ ���������
				result |= (0x01<<i);//*Output;							// ��� ������ - ���� � ����
			else														// ����� (������ �� ���������)
				result &= ~(0x01<<i);									// ������� ������ �� ������
		}
	}

	prevOutput = *Output;						// ���������� ���������� �������� ������� � ��
	return result;

	//--------------------------------------------------------------------
	/*if (IsImpulseMode())						// ���� ����� - ����������
	{
		if (*Output != prevOutput)				// ���� ������ ���������
			result = *Output;					// ��� ������ - ���� � ����
		else									// ����� (������ �� ���������)
			result = 0;							// ������� ������ �� ������
	}
	else if (IsPotentialMode())					// ���� ����� - �������������
	{
		if (*Output >> 1)						// ���� ������ ���� ������� �� ��������
		{
			if (*Output != prevOutput)			// ���� ������ ���������
				result = *Output;				// ��� ������ - ���� � ����
			else								// ����� (������ �� ���������
				result = 0;						// ������� ������ �� ������
		}
		else									// ���� ������ ����
		{
			if (prevOutput >> 1)				// �� �� ���������� �������� ���� ������� �� ��������
				result = CMD_STOP_ESCAPE; 		// �������, ��� ������ ������� ����
			else 								// ����� (������ �� ���������)
				result = 0;						// ������� ������ �� ������
		}
	}

	prevOutput = *Output;						// ���������� ���������� �������� ������� � ��
	return result;
}*/
//---------------------------------------------------

void TekModbusParamsUpdate(void) //??? ���������� ��������
{
	TRamGroupT *tek = &g_Ram.ramGroupT;

	// ��������� ��������������� �������
	tek->TechReg.bit.Opened  = g_Ram.ramGroupA.Status.bit.Opened;
	tek->TechReg.bit.Closed  = g_Ram.ramGroupA.Status.bit.Closed;
	tek->TechReg.bit.Mufta1  = g_Ram.ramGroupA.Status.bit.Mufta;
	tek->TechReg.bit.MuDu    = !g_Ram.ramGroupA.Status.bit.MuDu;
	tek->TechReg.bit.Opening = g_Ram.ramGroupA.Status.bit.Opening;
	tek->TechReg.bit.Closing = g_Ram.ramGroupA.Status.bit.Closing;
	tek->TechReg.bit.Stop    = g_Ram.ramGroupA.Status.bit.Stop;
	tek->TechReg.bit.Ten     = g_Ram.ramGroupA.Status.bit.Ten;
	tek->TechReg.bit.Ready   = !g_Ram.ramGroupA.Status.bit.Fault;

	// ��������� ������� ��������
	tek->DefReg.bit.I2t = g_Ram.ramGroupA.Faults.Load.bit.I2t;
	tek->DefReg.bit.ShC = (g_Ram.ramGroupA.Faults.Load.bit.ShCU || g_Ram.ramGroupA.Faults.Load.bit.ShCV || g_Ram.ramGroupA.Faults.Load.bit.ShCW);
	tek->DefReg.bit.Drv_T = 0;
	tek->DefReg.bit.Uv = (g_Ram.ramGroupA.Faults.Net.bit.UvR || g_Ram.ramGroupA.Faults.Net.bit.UvS || g_Ram.ramGroupA.Faults.Net.bit.UvT);
	tek->DefReg.bit.Phl = (g_Ram.ramGroupA.Faults.Load.bit.PhlU || g_Ram.ramGroupA.Faults.Load.bit.PhlV || g_Ram.ramGroupA.Faults.Load.bit.PhlW);
	tek->DefReg.bit.NoMove = g_Ram.ramGroupA.Faults.Proc.bit.NoMove;
	tek->DefReg.bit.Ov = (g_Ram.ramGroupA.Faults.Net.bit.OvR || g_Ram.ramGroupA.Faults.Net.bit.OvS || g_Ram.ramGroupA.Faults.Net.bit.OvT);
	tek->DefReg.bit.Bv = (g_Ram.ramGroupA.Faults.Net.bit.BvR || g_Ram.ramGroupA.Faults.Net.bit.BvS || g_Ram.ramGroupA.Faults.Net.bit.BvT);
	tek->DefReg.bit.Th = g_Ram.ramGroupA.Faults.Dev.bit.Th_BCP;
	tek->DefReg.bit.Tl = g_Ram.ramGroupA.Faults.Dev.bit.Tl_BCP;
	tek->DefReg.bit.PhOrdU = 0;//g_Ram.ramGroupA.Faults.Net.bit.PhOrd;
	tek->DefReg.bit.PhOrdDrv = g_Ram.ramGroupA.Faults.Proc.bit.PhOrd;
	tek->DefReg.bit.DevDef 	 = ((g_Ram.ramGroupA.Faults.Dev.all & TEK_DEVICE_FAULT_MASK) != 0); //����� ����������

	// ������� ������
	// ��� ������������ ����� �������, ���������� ���
	if (g_Core.VlvDrvCtrl.ActiveControls & CMD_SRC_SERIAL)
	{
		if(tek->ComReg.bit.Stop)
		{
			g_Ram.ramGroupD.ControlWord = vcwStop;

			tek->ComReg.bit.Stop = 0;
			tek->ComReg.bit.Open = 0;
			tek->ComReg.bit.Close = 0;
		}
		else if(tek->ComReg.bit.Open)
		{
			g_Ram.ramGroupD.ControlWord = vcwOpen;

			tek->ComReg.bit.Stop = 0;
			tek->ComReg.bit.Open = 0;
			tek->ComReg.bit.Close = 0;
		}
		else if(tek->ComReg.bit.Close)
		{
			g_Ram.ramGroupD.ControlWord = vcwClose;

			tek->ComReg.bit.Stop = 0;
			tek->ComReg.bit.Open = 0;
			tek->ComReg.bit.Close = 0;
		}
	}
	else
	{
		tek->ComReg.bit.Stop = 0;
		tek->ComReg.bit.Open = 0;
		tek->ComReg.bit.Close = 0;
	}

	if (tek->ComReg.bit.PrtReset)
	{
		g_Ram.ramGroupD.PrtReset = 1;
		tek->ComReg.bit.PrtReset = 0;
	}

	if (tek->ComReg.bit.EnDiscrOutTest)
	{
		g_Ram.ramGroupG.DiscrOutTest = 1;
	}
	else if (tek->ComReg.bit.DisDiscrOutTest)
	{
		if (g_Ram.ramGroupG.DiscrOutTest == 1)
		{
			g_Ram.ramGroupG.Mode = 0;
			g_Ram.ramGroupG.DiscrOutTest = 0;
		}
	}
	else if (tek->ComReg.bit.EnDiscrInTest)
	{
		g_Ram.ramGroupG.DiscrInTest = 1;
	}
	else if (tek->ComReg.bit.DisDiscrInTest)
	{
		if (g_Ram.ramGroupG.DiscrInTest == 1)
		{
			g_Ram.ramGroupG.Mode = 0;
			g_Ram.ramGroupG.DiscrInTest = 0;
		}
	}

	// �� ������ ������ ���������� ������� ������
	if (tek->ComReg.all != 0)
		tek->ComReg.all = 0;

// ������ ������������ ��/��
/*
	if (tek->ComReg.bit.SetDu && IsStopped())
		{
			Mcu.MuDuInput = 0;
			tek->ComReg.bit.SetDu = 0;
		}
	else if (tek->ComReg.bit.SetMu && IsStopped())
		{
			Mcu.MuDuInput = 1;
			tek->ComReg.bit.SetMu = 0;
		}
*/
	tek->PositionPr 	 = g_Ram.ramGroupA.PositionPr;
	tek->CycleCnt 		 = g_Ram.ramGroupA.CycleCnt;
	tek->Iu				 = g_Ram.ramGroupA.Iu;
	tek->Ur				 = g_Ram.ramGroupA.Ur;
	tek->Torque			 = g_Ram.ramGroupA.Torque;
	tek->Speed			 = g_Ram.ramGroupA.Speed;
	tek->RsStation		 = g_Ram.ramGroupB.RsStation;

	//��������� ���������� ������ � �������
	tek->TsTu.bit.IsDiscrOutActive = (g_Ram.ramGroupG.DiscrOutTest);
	tek->TsTu.bit.IsDiscrInActive = (g_Ram.ramGroupG.DiscrInTest);

	tek->TsTu.bit.InOpen 	 = g_Comm.digitInterface.Inputs.bit.Open;
	tek->TsTu.bit.InClose	 = g_Comm.digitInterface.Inputs.bit.Close;
	tek->TsTu.bit.InStop 	 = g_Comm.digitInterface.Inputs.bit.StopOpen || g_Comm.digitInterface.Inputs.bit.StopClose;
	tek->TsTu.bit.InMu 		 = g_Comm.digitInterface.Inputs.bit.Mu;
	tek->TsTu.bit.InDu 		 = g_Comm.digitInterface.Inputs.bit.Du;

	if (!g_Ram.ramGroupG.DiscrOutTest)
	{
		tek->TsTu.bit.OutOpened  = g_Comm.digitInterface.Outputs.bit.Opened;
		tek->TsTu.bit.OutClosed  = g_Comm.digitInterface.Outputs.bit.Closed;
	 	tek->TsTu.bit.OutMufta   = g_Comm.digitInterface.Outputs.bit.Mufta;
		tek->TsTu.bit.OutFault   = g_Comm.digitInterface.Outputs.bit.Fault;
		tek->TsTu.bit.OutOpening = g_Comm.digitInterface.Outputs.bit.Opening;
		tek->TsTu.bit.OutClosing = g_Comm.digitInterface.Outputs.bit.Closing;
		tek->TsTu.bit.OutMuDu 	 = g_Comm.digitInterface.Outputs.bit.MUDU;
		tek->TsTu.bit.OutNeispr  = g_Comm.digitInterface.Outputs.bit.Defect;
	}
	else
	{
		g_Comm.digitInterface.Outputs.bit.Opened = tek->TsTu.bit.OutOpened;
		g_Comm.digitInterface.Outputs.bit.Closed = tek->TsTu.bit.OutClosed;
		g_Comm.digitInterface.Outputs.bit.Mufta = tek->TsTu.bit.OutMufta;
		g_Comm.digitInterface.Outputs.bit.Fault = tek->TsTu.bit.OutFault;
		g_Comm.digitInterface.Outputs.bit.Opening = tek->TsTu.bit.OutOpening;
		g_Comm.digitInterface.Outputs.bit.Closing = tek->TsTu.bit.OutClosing;
		g_Comm.digitInterface.Outputs.bit.MUDU = tek->TsTu.bit.OutMuDu;
		g_Comm.digitInterface.Outputs.bit.Defect = tek->TsTu.bit.OutNeispr;
	}
}
