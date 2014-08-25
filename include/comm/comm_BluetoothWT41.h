/*======================================================================
��� �����:          comm_BluetoothWT41.h
�����:
������ �����:
���� ���������:
��������:
������ ������� Bluetooth
======================================================================*/

#ifndef BLUETOOTH_WT41_
#define BLUETOOTH_WT41_

#ifdef __cplusplus
extern "C" {
#endif

#include "comm_ModBusTimers.h"

#define BT_DBG					0

#define BT_COMMAND_MODE			0
#define BT_DATA_MODE			1

#define BT_RX_BUFFER_SIZE		12//10

#define BT_IDLE					40
#define BT_TRANSMIT_COMPLETE	41
#define BT_TRANSMIT_BUSY		42
#define BT_RECEIVE_COMPLETE		43
#define BT_RECEIVE_BUSY			44

//#define CMD_CONTROL_CONFIG 		"AT\r\n"
//#define CMD_CONTROL_CONFIG
//0123456789012345678901234567890123456
//"SET CONTROL CONFIG 0000 0040 0080\r\n\n"
//#define CMD_CONTROL_CONFIG 		"SET CONTROL CONFIG 0000 0050 0490\r\n"
//#define CMD_CONTROL_CONFIG 		"SET CONTROL CONFIG 0000 0040 0080\r\n"???
//#define CMD_CONTROL_ECHO 		"SET CONTROL ECHO 7\r\n"

#define CMD_CONTROL_CONFIG 		"SET CONTROL CONFIG 0000 0040 0080\r\n"
#define CMD_CONTROL_ECHO 		"SET CONTROL ECHO 0004\r\n"

#define CMD_CONTROL_PROFILE		"SET PROFILE SPP Bluetooth Serial Port\r\n"
#define CMD_CONTROL_CLASS		"SET BT CLASS 001f00\r\n"
#define CMD_CONTROL_SET			"SET\r\n"

#define CMD_CONTROL_NAME 		"SET BT NAME "
//#define CMD_CONTROL_NAME 		"SET BT NAME BKD.v0002\r\n"
#define CMD_CONTROL_AUTH		"SET BT AUTH * 5124\r\n"
#define CMD_CONTROL_BAUD		"SET CONTROL BAUD 115200,8N1\r\n"

#define BT_AUTH_CODE_STRING		"5124"	// ������ � ����� ����������� Bluetooth

// ��������� ������� ��������
typedef struct _TBluetoothPort
{
	Byte ChannelID;               // ������������� UART-�����
	Byte Slave;                // ����� ������������ ����������
	Uns  BaudRate;             // �������� ������ / 100
	Uns  UartBaud;             // ����������� �������� ��� UART
	Byte Parity;               // ����� ��������
	Byte StopBit;               // ����� ��������
	Byte RepeatInit;

	Uns  HardWareType;			// ��� ����������� ������ �������� ������

	Bool TxBusy;
	Byte State;
	Byte CmdState;
	Byte Status;
	Byte Mode;
	Uns StrIndex;
	Uns Timer;
	Uns Period;
	Uns Stop;		//���� ���� ��� � ������� ����� ������ �������� �� ����������
	Bool IsConnected;
	Bool Error;

	Bool        Enabled;		//��������� ������ bluetooth
	TTimerList  TimerActive;	//
	TTimerList  TimerBlink;	//
	Bool		BlinkConnect;
	Bool Connect;

	Byte ModeProtocol;
	Uns  ButtActivTimer;
	Byte Function;

	char *DeviceNameString;
	char *DeviceAuthCodeString;

	char RxBuffer[BT_RX_BUFFER_SIZE+50];

	void (*EnableRx)(void);
	void (*EnableTx)(void);
	Uns (*ReceiveByte)(void);
	void (*TransmitByte)(Uns Data);
	void (*TransmitByteIM)(Uns Data);

	#if BT_DBG
		Uns TxBytesCount;
		Uns RxBytesCount;
	#endif
			
} TBluetoothPort,*TBluetoothHandle;

// ��������� �������
void InitChanelBt(TBluetoothHandle);
void BluetoothWTUpdate(TBluetoothHandle);
void BluetoothRxHandler(TBluetoothHandle, TMbHandle);
void BluetoothTxHandler(TBluetoothHandle, TMbHandle);
void BluetoothTimer(TBluetoothHandle);
void BluetoothActivation(TBluetoothHandle);


#ifdef __cplusplus
}
#endif // extern "C"

#endif

