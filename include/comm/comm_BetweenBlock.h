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
#define BKP_SCI_BAUD		SCI_BRR(1152)
#define SCI_BREAK			0x20

#define BKP_SCI_TX_ENABLE()		RS485_DIR_BKD = 1
#define BKP_SCI_TX_DISABLE()	RS485_DIR_BKD = 0


// Структура объекта драйвера
typedef struct _TMbBBPort {
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

// Структура обмена данными с БКП
typedef struct _TBKPData {
	Uns      	BkpVersion;
	LgUns      	BkpEncPostion;
	TBkpLedsReg BkpLedsReg;
	TBkpHallReg BkpHallReg;
	Int       	BkpTemper;
	Uns      	BkpOffTen;
} TBKPData,*TBKPDataHandle;


// Прототипы функций

void SciMasterConnBetweenBlockInit(TMbBBHandle);
void SciMasterConnBetweenBlockUpdate(TMbBBHandle , TBKPDataHandle);
void SciMasterConnBetweenBlockRxHandler(TMbBBHandle);
void SciMasterConnBetweenBlockTxHandler(TMbBBHandle);
void SciMasterConnBetweenBlockCommTimer(TMbBBHandle);


#ifdef __cplusplus
}
#endif // extern "C"

#endif

