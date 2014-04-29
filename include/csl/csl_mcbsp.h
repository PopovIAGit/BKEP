/*======================================================================
Имя файла:          csl_mcbsp.h
Автор:
Версия файла:
Дата изменения:
Описание:
Заголовочный файл для работы с интерфейсом McBSP
======================================================================*/

#ifndef CSL_MCBSP_
#define CSL_MCBSP_

#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MCBSPA	0
#define MCBSPB	1

//#define MCBSP_PARITY_ERROR	0x04
//#define MCBSP_OVERRUN_ERROR	0x08
//#define MCBSP_FRAME_ERROR	0x10
#define MCBSP_ERROR		0x03
#define MCBSP_TX_SYNC_ERROR		0x01
#define MCBSP_RX_SYNC_ERROR		0x02

//#define SCI_BRR(Baud)			(Uint16)((Uint32)(150e6 / 800)  / (Baud) - 1)

//---------------------------------------------------------------------------
/*#define SCI_DEFAULT_BAUD_RATES { \
	24,48,96,192,384,576,1152 \
	}

#define SCI_DEFAULT_BRR_VALUES { \
	SCI_BRR(24),SCI_BRR(48),SCI_BRR(96),SCI_BRR(192),SCI_BRR(384),SCI_BRR(576),SCI_BRR(1152) \
	}*/
//---------------------------------------------------------------------------

//extern void SCI_init(Byte Id, Uns BaudRate, Byte Parity, Byte Length);
extern Uns McBsp_recieve(Byte Id);
extern void McBsp_transmit(Byte Id, Uns Data);
extern Byte McBsp_getstatus(Byte Id);
extern void McBsp_reset(Byte Id);
extern void McBsp_rx_enable(Byte Id);
extern void McBsp_rx_disable(Byte Id);
extern void McBsp_tx_enable(Byte Id);
extern void McBsp_tx_disable(Byte Id);

#ifdef __cplusplus
}
#endif // extern "C"

#endif /* CSL_MCBSP */

