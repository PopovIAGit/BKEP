/*#include "comm_BluetoothWT41.h"
#include "structs.h"
#include "interface.h"*/

#include "config.h"
#include "comm.h"
#include "g_Ram.h"

#define BT_TIMER_SCALE			PRD3
#define BT_TIMER				1.00 * BT_TIMER_SCALE

Byte RxState = 0;

void BluetoothWTInit(TBluetoothHandle);
void SendOneString(TBluetoothHandle, char *String);
void SendTwoString(TBluetoothHandle, char *FirstString, char *SecondString);
void SendCommandOne(TBluetoothHandle, char *ComStr);
void SendCommandTwo(TBluetoothHandle, char *ComStr, char *AddStr);

void ClearInitValues(TBluetoothHandle);
void ClearValues(TBluetoothHandle);
__inline void RxCommandMode(TBluetoothHandle);
__inline void RxDataMode(TBluetoothHandle);
Bool CheckString(TBluetoothHandle, char *Str);
Bool CheckSciCommErr(TBluetoothHandle);

void BluetoothWTInit(TBluetoothHandle bPort)
{

	#if BT_DBG
	bPort->RxBytesCount = 0;
	bPort->TxBytesCount = 0;
	#endif

	EnableBtRx();

	bPort->BaudRate=1152;             // �������� ������ / 100
	bPort->UartBaud=1212;             // ����������� �������� ��� McBSP

	bPort->DeviceNameString = &StrDev[0];
	bPort->DeviceAuthCodeString = BT_AUTH_CODE_STRING;

	//!!!Init ����� ������ ���� ������������� McBSP
	/*SCI_init(BT_SCI, SCI_BRR(BT_SCI_BAUD), 0, 8);
	SciaRegs.SCICCR.bit.STOPBITS = 0;
	EnableBtRx();*/


	// ����� ��������� Bluetooth ��������� ������
	bPort->Timer = bPort->Period;
}

void BluetoothWTUpdate(TBluetoothHandle bPort)
{
	switch (bPort->State)
	{
		// ������������ ��������
		case 0: BluetoothWTInit(bPort);
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
				
		case 4:	SendCommandTwo(bPort, CMD_CONTROL_NAME, bPort->DeviceNameString);
				if (bPort->Status == BT_RECEIVE_COMPLETE)
				{ 
					ClearValues(bPort);	bPort->State++;
				}
				break;

		case 5: SendCommandTwo(bPort, CMD_CONTROL_AUTH, bPort->DeviceAuthCodeString);
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
				//bPort->EnableTx();
				bPort->StrIndex = 0;
				bPort->CmdState = 1;
				break;

		case 1: SendOneString(bPort, ComStr);

				if (bPort->Status == BT_TRANSMIT_COMPLETE)
					bPort->CmdState = 2;
				break;

		case 2:	EnableBtRx();
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
				//bPort->EnableTx();
				bPort->StrIndex = 0;
				bPort->CmdState = 1;
				break;

		case 1: SendTwoString(bPort, ComStr, AddStr);
				if (bPort->Status == BT_TRANSMIT_COMPLETE)
					bPort->CmdState = 2;
				break;

		case 2:	EnableBtRx();
				//bPort->EnableRx();
				bPort->StrIndex = 0;
				bPort->CmdState = 3;
				break;
				
		case 3:	if (bPort->Status == BT_RECEIVE_COMPLETE)		// ���� ���������� ������, �.�. ��� �������� ������� ������ ��������
					bPort->CmdState = 4;
				break;
	}
}



void BluetoothRxHandler(TBluetoothHandle bPort)
{
	// ���������� ���������� ������� �� �������� ������ Bluetooth
	if 		(bPort->Mode == BT_COMMAND_MODE)	RxCommandMode(bPort);
	else if (bPort->Mode == BT_DATA_MODE)		RxDataMode(bPort);
}

//�������� ������� McBSP
Bool CheckSciCommErr(TBluetoothHandle bPort)
{
	//Byte Data = SCI_getstatus(BT_SCI);
	Bool Error = false;

	//if (Data & SCI_BREAK)
	//	{	SCI_reset(BT_SCI); Error = true;	}

	return Error;
}


__inline void RxCommandMode(TBluetoothHandle bPort)
{
	char Data;

	bPort->Error = CheckCommError(bPort);

	if (bPort->Error)
		return;

	Data = bPort->ReceiveByte();

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

__inline void RxDataMode(TBluetoothHandle bPort)
{
	char Data;

	bPort->Error = CheckCommError(bPort);

	if (bPort->Error)
		return;

	Data = bPort->ReceiveByte();

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
	//ImReceiveData(&Im, Data); ???

	if (RxState >= 7)	
	{	
		bPort->Mode = BT_COMMAND_MODE;
		RxState = 0;
	}
}

void BluetoothTxHandler(TBluetoothHandle bPort)
{
#if BT_DBG
	bPort->TxBytesCount++;
#endif

	bPort->TxBusy = false;
}

void BtTimer(TBluetoothHandle bPort)
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
	char symbol = 0;

	if (!bPort->TxBusy)
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
			bPort->TransmitByte(symbol);					// �������� �� SCI
		}
	}
}


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
			bPort->TransmitByte(symbol);					// �������� �� SCI
		}
	}
}

//McBSP ��������� ���������� �� ����
// � ���������� ���������� �� ��������
void EnableBtRx(void)
{
	//SCI_tx_disable(BT_SCI);
	//SCI_rx_enable(BT_SCI);
}

//McBSP ��������� ���������� �� ��������
// � ���������� ���������� �� ����
void EnableBtTx(void)
{
	//SCI_rx_disable(BT_SCI);
	//SCI_tx_enable(BT_SCI);
}

//���� 1 ����� �� ������ McBSP
Byte ReceiveBtByte(void)
{
	return 0;//SCI_recieve(BT_SCI);
}

//�������� 1 ����� �� ������ McBSP
void TransmitBtByte(Byte Data)
{
	//SCI_transmit(BT_SCI, Data);
}
