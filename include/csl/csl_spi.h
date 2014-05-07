/*======================================================================
Имя файла:          csl_spi.h
Автор:
Версия файла:
Дата изменения:
Описание:
Заголовочный файл для работы с интерфейсом SPI
======================================================================*/

#ifndef CSL_SPI_
#define CSL_SPI_

#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_SLAVE	0
#define SPI_MASTER	1

#define SPIA			0
#define SPIB			1
#define SPIC			2
#define SPID			3

#define SPI_BRR(Baud)			(Uns)((LgUns)(150e6/12 / 1000) / (Baud) - 1)

extern void SPI_init(Byte Id, Byte Type, Byte Mode, Uns BaudRate, Byte Length);
extern Uns  SPI_send(Byte Id, Uns Data);
extern void SPI_transmit(Byte Id, Uns Data);
extern Uns  SPI_recieve(Byte Id);
extern Bool SPI_ready(Byte Id);

#ifdef __cplusplus
}
#endif // extern "C"

#endif /* CSL_SPI_ */




