/*======================================================================
Имя файла:          comm_BluetoothWT41.h
Автор:
Версия файла:
Дата изменения:
Описание:
Модуль драйвер Bluetooth
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

//#define CMD_CONTROL_CONFIG 		"AT\r\n"
//#define CMD_CONTROL_CONFIG 		"SET CONTROL CONFIG 0000 0040 0080\r\n\n"
//#define CMD_CONTROL_ECHO 		"SET CONTROL ECHO 0004\r\n\n"

#define CMD_CONTROL_ECHO 		"SET CONTROL CONFIG 0000 0040 0080\r\n"
#define CMD_CONTROL_CONFIG 		"SET CONTROL ECHO 0004\r\n"

#define CMD_CONTROL_NAME 		"SET BT NAME M11\r\n"
#define CMD_CONTROL_AUTH		"SET BT AUTH * 5124\r\n"
#define CMD_CONTROL_BAUD		"SET CONTROL BAUD 115200,8N1\r\n"

#define BT_AUTH_CODE_STRING		"5124"	// Строка с кодом авторизации Bluetooth

// Структура объекта драйвера
typedef struct _TBluetoothPort
{
	Byte ChannelID;               // Идентификатор UART-порта
	Byte Slave;                // Адрес подчиненного устройства
	Uns  BaudRate;             // Скорость обмена / 100
	Uns  UartBaud;             // Расчитанная скорость для UART
	Byte Parity;               // Режим паритета
	Byte StopBit;               // Режим паритета

	Uns  HardWareType;			// тип аппаратного канала передачи данных

	Bool TxBusy;
	Byte State;
	Byte CmdState;
	Byte Status;
	Byte Mode;
	Uns StrIndex;
	Uns Timer;
	Uns Period;
	Uns Stop;		//флаг того что в посылке будет вторая половина не посылаемая
	Bool IsConnected;
	Bool Error;

	char *DeviceNameString;
	char *DeviceAuthCodeString;

	char RxBuffer[BT_RX_BUFFER_SIZE];

	void (*EnableRx)(void);
	void (*EnableTx)(void);
	Uns (*ReceiveByte)(void);
	void (*TransmitByte)(Uns Data);

	#if BT_DBG
		Uns TxBytesCount;
		Uns RxBytesCount;
	#endif
			
} TBluetoothPort,*TBluetoothHandle;

// Прототипы функций
void InitChanelBt(TBluetoothHandle);
void BluetoothWTUpdate(TBluetoothHandle);
void BluetoothRxHandler(TBluetoothHandle, TMbHandle);
void BluetoothTxHandler(TBluetoothHandle, TMbHandle);
void BluetoothTimer(TBluetoothHandle);


#ifdef __cplusplus
}
#endif // extern "C"

#endif

