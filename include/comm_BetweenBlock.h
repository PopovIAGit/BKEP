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

#define MAX_SCI_BUF_LEN		10

// Структура объекта драйвера
typedef struct _TMbBBPort {
	Byte	rx_len;
	Byte	tx_len;
	Byte	wait_resp;
	Uns 	postable;
	Uns		timeout;
	Uns		timer;
	Byte	buf[MAX_SCI_BUF_LEN];
	Uns 	conn_enable;
	Uns 	max_time;
	Uns 	conn_err;
} TMbBBPort,*TMbBBHandle;

// Прототипы функций

void SciMasterConnBetweenBlockInit(TMbBBHandle);
void SciMasterConnBetweenBlockUpdate(TMbBBHandle);
void SciMasterConnBetweenBlockRxHandler(TMbBBHandle);
void SciMasterConnBetweenBlockTxHandler(TMbBBHandle);

#ifdef __cplusplus
}
#endif // extern "C"

#endif

