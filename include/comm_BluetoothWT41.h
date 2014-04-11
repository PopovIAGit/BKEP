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

#define BT_DBG					0

#define BT_COMMAND_MODE			0
#define BT_DATA_MODE			1

#define BT_RX_BUFFER_SIZE		10

#define BT_IDLE					40
#define BT_TRANSMIT_COMPLETE	41
#define BT_TRANSMIT_BUSY		42
#define BT_RECEIVE_COMPLETE		43
#define BT_RECEIVE_BUSY			44

#define CMD_CONTROL_CONFIG 		"SET CONTROL CONFIG 0000 0040 0080\r\n"
#define CMD_CONTROL_ECHO 		"SET CONTROL ECHO 0004\r\n"
#define CMD_CONTROL_NAME 		"SET BT NAME "
#define CMD_CONTROL_AUTH		"SET BT AUTH * "
#define CMD_CONTROL_BAUD		"SET CONTROL BAUD 115200,8N1\r\n"

#define BT_AUTH_CODE_STRING		"5124"	// ������ � ����� ����������� Bluetooth

// ��������� ������� ��������
typedef struct _TBluetoothPort
{
	Byte UartID;               // ������������� UART-�����
	Byte Mode;                 // ����� ������
	Byte Slave;                // ����� ������������ ����������
	Uns  BaudRate;             // �������� ������ / 100
	Uns  UartBaud;             // ����������� �������� ��� UART
	Byte Parity;               // ����� ��������
	Byte StopBit;               // ����� ��������

	Bool TxBusy;
	Byte State;
	Byte CmdState;
	Byte Status;
	Byte Mode;
	Uns StrIndex;
	Uns Timer;
	Uns Period;

	Bool IsConnected;
	Bool Error;

	char *DeviceNameString;
	char *DeviceAuthCodeString;

	Byte (*ReceiveByte)(void);
	void (*TransmitByte)(Byte Data);

	char RxBuffer[BT_RX_BUFFER_SIZE];

	#if BT_DBG
	Uns TxBytesCount;
	Uns RxBytesCount;
	#endif
			
} TBluetoothPort,*TBluetoothHandle;

// ��������� �������
void BluetoothWTUpdate(TBluetoothHandle);
void BluetoothRxHandler(TBluetoothHandle);
void BluetoothTxHandler(TBluetoothHandle);
void BluetoothTimer(TBluetoothHandle);


#ifdef __cplusplus
}
#endif // extern "C"

#endif

