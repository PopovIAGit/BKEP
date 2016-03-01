/*======================================================================
Имя файла:          csl_sci.h
Автор:
Версия файла:
Дата изменения:
Описание:
Заголовочный файл для работы с интерфейсом SCI
======================================================================*/

#ifndef CSL_SCI_
#define CSL_SCI_

#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCIA	0
#define SCIB	1
#define SCIC	2
#define SCID	3

#define MCBSPA	0
#define MCBSPB	1

#define SCI_PARITY_ERROR	0x04
#define SCI_OVERRUN_ERROR	0x08
#define SCI_FRAME_ERROR		0x10
#define SCI_RX_ERROR		0x1C

#define SCI_BRR(Baud)		(Uint16)((Uint32)(150e6/14 / 800)  / (Baud) - 1)
#define SCI_BRR20700(Baud)		(Uint16)((Uint32)(150e6/14 / 800)  / (Baud) - 1)
//---------------------------------------------------------------------------
#define SCI_DEFAULT_BAUD_RATES { \
	24,48,96,192,384,576,1152 \
	}

#define SCI_DEFAULT_BRR_VALUES { \
	SCI_BRR(24),SCI_BRR(48),SCI_BRR(96),SCI_BRR(192),SCI_BRR(384),SCI_BRR(576),SCI_BRR(1152) \
	}
//---------------------------------------------------------------------------

extern void SCI_init(Byte Id, Uns BaudRate, Byte Parity, Byte Length);
extern Byte SCI_recieve(Byte Id);
extern void SCI_transmit(Byte Id, Byte Data);
extern Byte SCI_getstatus(Byte Id);
extern void SCI_reset(Byte Id);
extern void SCI_rx_enable(Byte Id);
extern void SCI_rx_disable(Byte Id);
extern void SCI_tx_enable(Byte Id);
extern void SCI_tx_disable(Byte Id);
extern void SCI_stopBit2(Byte Id);
extern void SCI_stopBit1(Byte Id);

#ifdef __cplusplus
}
#endif // extern "C"

#endif /* CSL_SCI_ */

