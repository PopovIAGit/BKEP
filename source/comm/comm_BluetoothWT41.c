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
//��� �������� Bluetooth
char StrDev[] = {"BUR-M_000000"};


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
/*
void EnableBtRx(TBluetoothHandle);
void EnableBtTx(TBluetoothHandle);
Byte ReceiveBtByte(TBluetoothHandle);
void TransmitBtByte(TBluetoothHandle, Byte Data);
 */

void InitChanelBt(TBluetoothHandle bPort)
{

	bPort->TxBusy	= false;
	bPort->State	= 0;
	bPort->CmdState = 0;
	bPort->Status	= 0;
	bPort->Mode 	= BT_COMMAND_MODE;
	bPort->StrIndex	= 0;
	bPort->Timer	= 0;
	bPort->Period	= 10;//(1.00 * PRD3)

	bPort->IsConnected 	= false;
	bPort->Error		= false;

	bPort->EnableRx 	= EnableBtRx;
	bPort->EnableTx 	= EnableBtTx;
	bPort->ReceiveByte 	= ReceiveBtByte;
	bPort->TransmitByte = TransmitBtByte;

	memset(&bPort->RxBuffer[0],	0, sizeof(BT_RX_BUFFER_SIZE));

	#if BT_DBG
	bPort->RxBytesCount = 0;
	bPort->TxBytesCount = 0;
	#endif

	bPort->DeviceNameString = &StrDev[0];
	bPort->DeviceAuthCodeString = BT_AUTH_CODE_STRING;

	//!!!Init ����� ������ ���� ������������� McBSP
	if (bPort->HardWareType==UART_TYPE)
	{
		SCI_init(bPort->ChannelID, bPort->UartBaud, bPort->Parity, 8);
		SciaRegs.SCICCR.bit.STOPBITS = 0;
	} else
	if (bPort->HardWareType==MCBSP_TYPE){
		//InitMcbspa();
	}
	bPort->EnableRx();

	// ����� ��������� Bluetooth ��������� ������
	bPort->Timer = bPort->Period;
}

void BluetoothWTUpdate(TBluetoothHandle bPort)
{
	switch (bPort->State)
	{
		// ������������ ��������
		case 0: InitChanelBt(bPort);
				bPort->State++;
				break;
				
		// ����������� ������ ���� ����� ����� �������������
		case 1: if (!bPort->Timer)
				{ 
					ClearValues(bPort);	bPort->State++;
				}
				break;

		//
		case 2: SendCommandOne(bPort, CMD_CONTROL_CONFIG);
				if (bPort->Status == BT_RECEIVE_COMPLETE)
				{ 
					ClearValues(bPort);	bPort->State++;
				}
				break;

		case 3: SendCommandOne(bPort, CMD_CONTROL_ECHO);
				if (bPort->Status == BT_RECEIVE_COMPLETE)
				{ 
					ClearValues(bPort);	bPort->State++;
				}
				break;
				
		case 4:	SendCommandOne(bPort, CMD_CONTROL_NAME);//SendCommandTwo(bPort, CMD_CONTROL_NAME, bPort->DeviceNameString);
				if (bPort->Status == BT_RECEIVE_COMPLETE)
				{ 
					ClearValues(bPort);	bPort->State++;
				}
				break;

		case 5: SendCommandOne(bPort, CMD_CONTROL_AUTH);//SendCommandTwo(bPort, CMD_CONTROL_AUTH, bPort->DeviceAuthCodeString);
				if (bPort->Status == BT_RECEIVE_COMPLETE)
				{ 
					ClearValues(bPort);	bPort->State++;
				}
				break;
		case 6: SendCommandOne(bPort, CMD_CONTROL_BAUD);
				if (bPort->Status == BT_RECEIVE_COMPLETE)
				{ 
					ClearValues(bPort);	bPort->State++;
				}
				break;

		// ����� ����� ���� �������� ������� ������� ������ ������
		// ���� �������� ��� ���

		// ����� �������� ����������
		case 7:	if (bPort->Status == BT_RECEIVE_COMPLETE)
				{
		 			if (CheckString(bPort, "RING"))				// ������� ����������
		 				bPort->State++;							// ������ ������ RING
					else 
						ClearValues(bPort);						// ������ ����� ��������, ���� ������ ���-�� ������
				}											// ������ �������� OK ��� PAIR
				break;


		case 8: bPort->Mode = BT_DATA_MODE;					// ������� � ����� ������
				ClearValues(bPort);
				bPort->State++;
				break;

		case 9: if (bPort->Mode == BT_COMMAND_MODE)			// �������� � ������ ������,
				{										// ������� ������� � ����� ������
					ClearValues(bPort);
					bPort->State = 7;						// ������� � ����� �������� ����������
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

// �������� ������� �� ���, �.�. strlen ������� ���-�� �����
void SendCommandOne(TBluetoothHandle bPort, char *ComStr)
{
	switch(bPort->CmdState)
	{
		case 0: EnableBtTx();
				//McBsp_rx_enable(MCBSPA);
				//McBsp_tx_enable(MCBSPA);
				//bPort->EnableTx();
				bPort->StrIndex = 0;
				bPort->CmdState = 1;
				break;

		case 1: SendOneString(bPort, ComStr);

				if (bPort->Status == BT_TRANSMIT_COMPLETE)
					bPort->CmdState = 2;
				break;

		case 2:	EnableBtRx();
				//McBsp_tx_enable(MCBSPA);
				//McBsp_rx_enable(MCBSPA);
				//bPort->EnableRx();
				bPort->StrIndex = 0;
				bPort->CmdState = 3;
				break;
				
		case 3:	if (bPort->Status == BT_RECEIVE_COMPLETE)		// ���� ���������� ������, �.�. ��� �������� ������� ������ ��������
					bPort->CmdState = 4;
				break;
	}
}


void SendCommandTwo(TBluetoothHandle bPort, char *ComStr, char *AddStr)
{
	switch(bPort->CmdState)
	{
		case 0: EnableBtTx();
				//McBsp_rx_enable(MCBSPA);
				//McBsp_tx_enable(MCBSPA);
				//bPort->EnableTx();
				bPort->StrIndex = 0;
				bPort->CmdState = 1;
				break;

		case 1: SendTwoString(bPort, ComStr, AddStr);
				if (bPort->Status == BT_TRANSMIT_COMPLETE)
					bPort->CmdState = 2;
				break;

		case 2:	EnableBtRx();
				//McBsp_tx_enable(MCBSPA);
				//McBsp_rx_enable(MCBSPA);
				//bPort->EnableRx();
				bPort->StrIndex = 0;
				bPort->CmdState = 3;
				break;
				
		case 3:	if (bPort->Status == BT_RECEIVE_COMPLETE)		// ���� ���������� ������, �.�. ��� �������� ������� ������ ��������
					bPort->CmdState = 4;
				break;
	}
}



void BluetoothRxHandler(TBluetoothHandle bPort, TMbHandle hPort)
{
	// ���������� ���������� ������� �� �������� ������ Bluetooth
	if 		(bPort->Mode == BT_COMMAND_MODE)	RxCommandMode(bPort);
	else if (bPort->Mode == BT_DATA_MODE)		RxDataMode(bPort, hPort);
}

//�������� ������� McBSP
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
		bPort->RxBuffer[bPort->StrIndex] = Data;

	// ����� ��������� ������� - 2 ������� "��", ������ ��������� ����� "\r\n", ������� ���� ������ �������
	// if ((p->StrIndex >= 2) && (Data == '\n'))
	bPort->Status = (Data == '\n') ? BT_RECEIVE_COMPLETE : BT_RECEIVE_BUSY;

	bPort->StrIndex++;
}

__inline void RxDataMode(TBluetoothHandle bPort, TMbHandle hPort)
{
	char Data;

	bPort->Error = CheckCommError(bPort);

	if (bPort->Error)
		return;

	Data = ReceiveBtByte();

#if BT_DBG
	bPort->RxBytesCount++;
#endif

	switch (++RxState)
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

	// ����� ������ ��� ���.������
	ImReceiveData(&g_Stat.Im, Data);

	if (RxState >= 7)	
	{	
		bPort->Mode = BT_COMMAND_MODE;
		RxState = 0;
	}
}

void BluetoothTxHandler(TBluetoothHandle bPort, TMbHandle hPort)
{
#if BT_DBG
	bPort->TxBytesCount++;
#endif

	bPort->TxBusy = false;
}

void BluetoothTimer(TBluetoothHandle bPort)
{
	if (bPort->Timer > 0) bPort->Timer--;
}

Bool CheckString(TBluetoothHandle bPort, char *Str)
{
	Uns i, j;
	Uns Lenght = strlen(Str);						// ����� ������� ������
	Uns BufIndex = 0;	Uns CheckIndex = 0;

	bPort->StrIndex = 0;

	if (Lenght > BT_RX_BUFFER_SIZE)		return false;							// ���� ������ ��������� ����� ������, �� �������
	else								BufIndex = BT_RX_BUFFER_SIZE - Lenght;	// ������� ������ �������� ������ � ������
															
	for (i = 0; i <= BufIndex; i++)						// �������� ��������� ��� ������ � �������� ������� �������, ����� � �������
	{													// � �.�. �� ��� ��� ���� �� �������� ������ �������� 
		for (j = 0; j < Lenght; j++)					// ���������� ������� � ������ � ��������� � ������
		{												// �� ��� ��� ���� �� �������� ����� ������
			if (Str[j] == bPort->RxBuffer[j+i])				// ���� ������� �������, �� �������������� CheckIndex
				CheckIndex++;			
		}
		if (Lenght == CheckIndex)	return true;		// ���� ����� ������ ������� � CheckIndex, �� �������� �������
		else						CheckIndex = 0;		// ����� �������� ������ � ���������� ������
	}	

	return false;										// ������ �� �������
}


void SendOneString(TBluetoothHandle bPort, char *String)
{
	char symbol1 = 0;
	char symbol2 = 0;

	/*if (!bPort->TxBusy)
	{
		symbol = String[bPort->StrIndex];					// ��������� ������� ������
		if (symbol == '\0')								// �������� ����� ������
		{
			bPort->Status = BT_TRANSMIT_COMPLETE;			// ������ ���������� ��������
			bPort->StrIndex = 0;
		}
		else
		{
			bPort->StrIndex++;
			bPort->Status = BT_TRANSMIT_BUSY;				// ������ ��������
			bPort->TxBusy = true;							// ���������� ���� ��������
			//DELAY_US(10);
			TransmitBtByte(symbol);					// �������� �� SCI

		}
	}*/

	if (!bPort->TxBusy)
	{
		symbol1 = String[bPort->StrIndex];					// ��������� ������� ������
		symbol2 = String[bPort->StrIndex+1];					// ��������� ������� ������

		if (symbol1 == '\0')								// �������� ����� ������
		{
			bPort->Status = BT_TRANSMIT_COMPLETE;			// ������ ���������� ��������
			bPort->StrIndex = 0;
		}
		else
		{
			bPort->StrIndex+=2;
			bPort->Status = BT_TRANSMIT_BUSY;				// ������ ��������
			bPort->TxBusy = true;							// ���������� ���� ��������
			TransmitBtByte(symbol1|((symbol2<<8)&0xFF00));					// �������� �� SCI

		}

		/*if (symbol2 == '\0')								// �������� ����� ������
		{
			bPort->Status = BT_TRANSMIT_COMPLETE;			// ������ ���������� ��������
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

		if (symbol1 == '\0')								// �������� ����� ������
		{
			bPort->Status = BT_TRANSMIT_COMPLETE;			// ������ ���������� ��������
			bPort->StrIndex = 0;
			isFirstStr = true;
			isEndSymbols = false;
			len1 = 0;
			len2 = 0;
		}
		else
		{
			bPort->StrIndex+=2;
			bPort->Status = BT_TRANSMIT_BUSY;				// ������ ��������
			bPort->TxBusy = true;							// ���������� ���� ��������
			//TransmitBtByte(symbol);					// �������� �� SCI
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

		if (symbol == '\0')								// �������� ����� ������
		{
			bPort->Status = BT_TRANSMIT_COMPLETE;			// ������ ���������� ��������
			bPort->StrIndex = 0;
			isFirstStr = true;
			isEndSymbols = false;
			len1 = 0;
			len2 = 0;
		}
		else
		{
			bPort->StrIndex++;
			bPort->Status = BT_TRANSMIT_BUSY;				// ������ ��������
			bPort->TxBusy = true;							// ���������� ���� ��������
			TransmitBtByte(symbol);					// �������� �� SCI
		}
	}
}
*/
//McBSP ��������� ���������� �� ����
// � ���������� ���������� �� ��������
//void EnableBtRx(TBluetoothHandle bPort)
void EnableBtRx(void)
{
	McBsp_tx_disable(MCBSPA);
	McBsp_rx_enable(MCBSPA);
}

//McBSP ��������� ���������� �� ��������
// � ���������� ���������� �� ����
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

//���� 1 ����� �� ������ McBSP
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

//�������� 1 ����� �� ������ McBSP
void TransmitBtByte(Uns Data)
{
	McBsp_transmit(MCBSPA, Data);
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
