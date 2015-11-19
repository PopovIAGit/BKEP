/*======================================================================
Имя файла:          comm_BetweenBlock.h
Автор:
Версия файла:
Дата изменения:
Описание:
Модуль ModBus
======================================================================*/

#ifndef MODBUS_BETWEENBLOCK_RTU_
#define MODBUS_BETWEENBLOCK_RTU_

#ifdef __cplusplus
extern "C" {
#endif

#define BKP_MAX_SCI_BUF_LEN	10
#define BKP_SCI_RX_TIMEOUT	(Uns)(0.00175 * 18000)//(Uns)(0.00175 * 18000)
#define BKP_SCI_TX_TIMEOUT	(Uns)(0.00017 * 18000)
#define BKP_CONN_TIMEOUT	(Uns)(1.00000 * 18000)
//#define BKP_SCI_ID			SCIA
#define BKP_SCI_BAUD		SCI_BRR(384)
#define SCI_BREAK			0x20

#define BKP_SCI_TX_ENABLE()		RS485_DIR_BKD = 1
#define BKP_SCI_TX_DISABLE()	RS485_DIR_BKD = 0



typedef void (*TScTrEnable)(char);

typedef struct _TScParams {
	unsigned int UartID;
	unsigned int BrrValue;
	unsigned int Parity;
	unsigned int Mode;
	unsigned int TimeoutPre;
	unsigned int TimeoutPost;
	unsigned int TimeoutConn;
	unsigned int RetryCount;
	TScTrEnable  TrEnable;
} TScParams;

typedef struct _TScPacket {
	unsigned int Flag;
	unsigned int State;
	unsigned int Fcs;
	unsigned int Len;
	char         Data[10];
} TScPacket;

typedef struct _TScFrame {
	unsigned int ConnFlag;
	unsigned int RetryCounter;
	unsigned int TimerPre;
	unsigned int TimerPost;
	unsigned int TimerConn;
	unsigned int MsgCount;
	unsigned int TxDelayTimer;
	unsigned int TxDelayTimeout;
} TScFrame;

typedef struct _TScStat {
	unsigned int RxMsgCount;
	unsigned int TxMsgCount;
	unsigned int RxBytesCount;
	unsigned int TxBytesCount;
	unsigned int RxErrCount;
	unsigned int RxMissedErrCount;
	unsigned int RxCrcErrCount;
	unsigned int RxFrameErrLenCount;
	unsigned int RxOverflowErrCount;
	unsigned int RxNoRespErrCount;
} TScStat;

typedef struct _TMbBBPort {
	TScParams    Params;
	TScPacket    RxPacket;
	TScPacket    TxPacket;
	TScFrame     Frame;
	TScStat      Stat;
} TMbBBPort,*TMbBBHandle;

// Структура объекта драйвера
/*typedef struct _TMbBBPort {
	Uns IsConnected;
	Uns RxLength;
	Uns TxLength;
	Uns WaitResponse;
	Uns RxFlag;
	Uns RxTimer;
	Uns TxTimer;
	Uns ConnErr;
	Uns RxByteCount;
	Uns TxByteCount;
	Uns ConnTimer;
	Uns ConnTimeout;
	Uns Buffer[BKP_MAX_SCI_BUF_LEN];
} TMbBBPort,*TMbBBHandle;
*/
// структуры
typedef union {
	Uns all;
  struct {
		Uns Led0:1;		// 0
		Uns Led1:1;		// 1
		Uns Led2:1;		// 2
		Uns Led3:1;		// 3
		Uns Led4:1;		// 4
		Uns Rsvd:11;	// 5-15
  } bit;
} TBkpLedsReg;

typedef union {
	Uns all;
  struct {
		Uns Open:1;		// 0
		Uns StopMu:1;	// 1
		Uns Close:1;	// 2
		Uns StopDu:1;	// 3
		Uns Rsvd:12;	// 4-15
  } bit;
} TBkpHallReg;

// Режим связи
typedef enum {
	hmOpen 	 = 0,				//
	hmClose  = 1,				//
	hmStopMu = 2,				//
	hmStopDu = 3
} THallMask;

// Структура обмена данными с БКП
typedef struct _TBKPData {
	Uns      	BkpVersion;
	LgUns      	BkpEncPostion;
	TBkpLedsReg BkpLedsReg;
	TBkpHallReg BkpHallReg;
	Int       	BkpTemper;
	Uns      	BkpOffTen;
	Uns 		BkpFaultData;		// код аварий
	Uns			BkpCalibPosition;	// положение в процентах
} TBKPData,*TBKPDataHandle;


// Прототипы функций


void SciMasterConnBetweenBlockInit(TMbBBHandle);
void SciMasterConnBetweenBlockUpdate(TMbBBHandle);
void SciMasterConnBetweenBlockRxHandler(TMbBBHandle);
void SciMasterConnBetweenBlockTxHandler(TMbBBHandle);
void SciMasterConnBetweenBlockCommTimer(TMbBBHandle);


#ifdef __cplusplus
}
#endif // extern "C"

#endif

