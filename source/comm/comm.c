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

TComm	g_Comm;

void CommandUpdate(TComm *p);
Uns DigitCmdModeUpdate (Uns *Output);
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

	//InitChanelShnModbus(&g_Comm.mbShn);
	InitChanelBtModbus(&g_Comm.mbBt);

	//����������� ���� � ��� �� ���������� ����� ��� �������� Bluetooth � Modbus
	g_Comm.Bluetooth.HardWareType = g_Comm.mbBt.Params.HardWareType;	// ������
	g_Comm.Bluetooth.ChannelID    = g_Comm.mbBt.Params.ChannelID;		// ������ ������
	g_Comm.Bluetooth.BaudRate	  = g_Comm.mbBt.Params.BaudRate;		//
	g_Comm.Bluetooth.UartBaud	  = g_Comm.mbBt.Params.UartBaud;		//
	g_Comm.Bluetooth.Mode	  	  = g_Comm.mbBt.Params.Mode;			//
	g_Comm.Bluetooth.Parity	   	  = g_Comm.mbBt.Params.Parity;			//
	SerialCommInit(&g_Comm.mbBt);
	InitChanelBt(&g_Comm.Bluetooth);

	SerialCommInit(&g_Comm.mbAsu);
	//SerialCommInit(&g_Comm.mbShn);
	Comm_LocalControlInit(&g_Comm.localControl);

}
//---------------------------------------------------
void Comm_Update(TComm *p)
{

	if (g_Comm.Bluetooth.ModeProtocol==0) ModBusUpdate(&g_Comm.mbAsu); 	// slave ����� ����� � ������� ������� ���
	//ModBusUpdate(&g_Comm.mbShn);  // master ����� ����� � ����������� �������� �����

	//SciMasterConnBetweenBlockUpdate(&g_Comm.mbBkp);// master ����� ����� �

	BluetoothWTUpdate(&g_Comm.Bluetooth); //������� Bluetooth
	if (g_Comm.Bluetooth.ModeProtocol>0) ModBusUpdate(&g_Comm.mbBt);  // slave

	//SerialCommUpdate(&Mb);


}

void Comm_50HzCalc(TComm *p)
{
	//	������� � ��� !!!
	// �������� ������� � ������ ����������
	if (g_Ram.ramGroupH.CmdButton != KEY_STOP)
	g_Ram.ramGroupH.CmdKey = Comm_LocalKeyUpdate(&p->localControl);

	// �������� ������� � ����� ���
	switch (Comm_LocalButtonUpdate(&p->localControl))
	{
	case (BTN_STOPMU_BIT | BTN_STOPDU_BIT):
		g_Ram.ramGroupH.CmdButton = KEY_STOP;
		break;
	case BTN_OPEN_BIT:
		if (g_Ram.ramGroupH.CmdKey == KEY_STOP) break;
		g_Ram.ramGroupH.CmdButton = KEY_OPEN;
		break;
	case BTN_CLOSE_BIT:
		if (g_Ram.ramGroupH.CmdKey == KEY_STOP) break;
		g_Ram.ramGroupH.CmdButton = KEY_CLOSE;
		break;
	default:
		g_Ram.ramGroupH.CmdButton = KEY_NONE;
	}

	CommandUpdate(&g_Comm);
}

//-----------��������� ���������� ������ -----------------------------
void CommandUpdate(TComm *p)
{
	static Byte clrReset=0;

	p->outputCmdReg = 0;//CMD_NO;

	//����� �� ������ ��������� �� �� � ����� ��
	Comm_TuTsUpdate(&p->digitInterface); // ��������������, ����������������

	//���� �������������� ���������
	p->digitInput.input = p->digitInterface.Inputs.all;
	DigitalInpUpdate(&p->digitInput); // ����� ������� ��������� ��������� �������, ������ �� �����
	g_Ram.ramGroupA.StateTu.all = p->digitInput.output;

	if (!(p->digitInput.output & 0x20))
	{
		p->outputCmdReg = (p->digitInput.output & 0x4);
	}
	else
	{
		p->outputCmdReg = (p->digitInput.output & 0x7);
	}

	g_Ram.ramGroupH.TuState = p->outputCmdReg;

	if ((p->digitInput.output>>4)&0x01==1)
	{
		if (clrReset==0) g_Ram.ramGroupD.PrtReset = 1;
		clrReset=1;
	} else if ((p->digitInput.output>>4)&0x01==0)
	{
		clrReset=0;
	}
//	p->outputCmdReg |= DigitCmdModeUpdate(&p->digitInput.output);
///	if (p->outputCmdReg)
//	{
//		p->outputCmdSrc = CMD_SRC_DIGITAL;
//	}
}
//---------------------------------------------------
// ������� ��������� ������ ������ ������ ����������� ���������� (����� - ����������, �������������)
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
					result |= DIN_STOP_BIT;    							// �������, ��� ������ ������� ����
				else 													// ����� (������ �� ���������)
					result &= ~(0x01<<i);								// ������� ������ �� ������
			}
		}else
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
			else								// ����� (������ �� ���������)
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
	return result;*/
}
//---------------------------------------------------
