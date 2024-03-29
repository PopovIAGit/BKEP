#ifndef _IM_H_
#define _IM_H_

#include "std.h"

#define IM_TIMEOUT				(10.00 * 10)	 //���� 1���	//??? ������� ������ ������� �� ��������??? ������� ��������������� ������

#define IM_READ_BUF_SIZE		((LOG_EV_DATA_CNT * LOG_EV_DATA_CELL) + (LOG_EV_BUF_DATA_CNT * LOG_EV_BUF_DATA_CELL))

#define IM_LOGEV_TYPE			1
#define IM_LOGCMD_TYPE			2
#define IM_LOGPARAMS_TYPE		3
//ma LogSim
#define IM_LOGSIM_TYPE		4

#define IM_RD_BUFFER_SIZE			40
#define IM_WR_BUFFER_SIZE			200
#define IM_DOWNLOAD_BUFFER_SIZE		IM_READ_BUF_SIZE

#define RECEIVE_FUNC_MODE			0
#define DOWNLOAD_FUNC_MODE			1
#define SEND_DATA_MODE				2

#define IM_MAX_FUNC_INDEX_VALUE		4

typedef enum 
{
	imIdle = 0,
	imInit = 1,
	imDeviceInfo = 2,
	imSumLogsInfo = 3,
	imEvLogInfo = 4,
	imCmdLogInfo = 5,
	imParamLogInfo = 6,
	imSimLogInfo = 7,
	imEvLogMainAddr = 8,
	imEvLogBufAddr = 9,
	imCmdLogAddr = 10,
	imParLogAddr = 11,
	imSimLogAddr = 12,
	imLogDownload = 13,
	imParamDownload = 14

} TIMFuncState;

typedef enum
{
	imSrcNone = 0,
	imSrcBluetooth = 1,
	imSrcModbus = 2
} TImSrc;


typedef struct
{
	Byte ControlMode;
	Byte CorrectCount;
	Byte FuncState;
	Byte Index;
	Uns  TxIndex;
	Bool CanSendData;
	Bool *IsTxBusy;
	Uns Timer;
	Uns Period;
	Uns TimerIndex;
	Uns PeriodIndex;

	Bool CanReadNextRec;
	Bool IsReadRecBusy;
	Bool IsBufReady;
	Bool IsLogTransmit;

	Uns *DeviceDataPtr;
	//Uns *DownloadBuffer;
	TImSrc HardwareSrc;
	Uns BluetoothLedBlink;
		
	void (*EnableReceive)(void);
	void (*EnableTransmit)(void);
	void (*TransmitByte)(Uns Data);

	Byte RdBuffer[IM_RD_BUFFER_SIZE];
	Byte WrBuffer[IM_WR_BUFFER_SIZE];

	Byte WrBufferMb[IM_WR_BUFFER_SIZE];
	Uns  AddrData;
	Byte IndexMb;
	Byte CanSendDataMb;

	Uns ImReadBuf[(IM_READ_BUF_SIZE * 2)];			// ����� ��������������� ������

	Int WaitAndroidCounter;
	Int WaitTime;
} TInfoModule, *pTInfoModule;

// ������� ������� ������ ����
void ImReceiveData(pTInfoModule, Byte Data);
// �������� ������ ������ � �������������� �������
void ImUpdate(pTInfoModule);
// ����� ��������������� ������
void ImReset(pTInfoModule);
// ������� ������� ������ �������� � �����
void ImBufferReader(pTInfoModule, Byte LogType, Uns RecordNum);
void ImBufferReaderTest(pTInfoModule, Byte LogType, Uns RecordNum);
// ������� ������� �������
void ImTimer(pTInfoModule);
void ImTimerIndex(pTInfoModule);
#endif
