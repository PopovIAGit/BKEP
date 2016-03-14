/*======================================================================
Имя файла:          stat.h
Автор:
Версия файла:
Дата изменения:
Описание:
Библиотека для работы с журналом событий
======================================================================*/

#ifndef STAT_
#define STAT_

#include "csl_memory.h"
#include "stat_fm25v10.h"
#include "stat_LogEvent.h"
#include "stat_InfoModuleParams.h"
#include "stat_InfoModule.h"



// Переопределение для работы с памятью
//#define MemPar	Eeprom1								// Структуры памяти параметров
//#define MemLog	Eeprom2
#define ParFunc	ReadWriteEeprom							// Функция для работы с данными структуры параметров
#define LogFunc ReadWriteEeprom

// Макросы флеш
//#define IsMemParReady()				(!Eeprom1.Func)
//#define IsMemLogReady()				(!Eeprom2.Func)

//#define IsMainFlashReady()		(!g_MainFlash.func)
//#define IsMemParReady()			(!g_Eeprom.func)

#define	WaitForMainFlash()		while (!IsMainFlashReady()) {AT25DF041A_Update(&g_MainFlash);DELAY_US(1000);}
// Команды работы с флеш
#define FLASH_READ				0x01
#define FLASH_WRITE				0x02
#define FLASH_ERASE_FIRST		0x04
#define FLASH_ERASE_ALL			0x08

// Адресация журнала
#define EVLOG_MOD_BUS_ADDR		0x0500
//#define EVLOG_ADDR				REG_LOG
#define EVLOG_DATA_START_ADR	0
#define EVLOG_DATA_CNT			13
#define EVLOG_CNT				2500
#define EVLOG_SIZE				(EVLOG_CNT * EVLOG_DATA_CNT)
#define EVLOG_MOD_BUS_EADDR		(EVLOG_MOD_BUS_ADDR + EVLOG_SIZE - 1)
// Маски статуса
// Маска глобального статуса для изменения состояния журнала и записи
#define STATUS_EVLOG_MASK		0x6E9 // 11011101001 

#define COMMAND_EVLOG_MASK	0x0007 // 0000000000111 Маска глобальной команды для изменения состояния журнала

// Структура для работы с журналом событий
typedef struct {
	Bool  *pEnable;				// Разрешение работы
	Bool  execFlag;				// Флаг формирования записи в журнал
	Bool  writeFlag;			// Флаг записи в память
	Byte  cmd;					// Команда работы с журналом
	Bool  profEnable;			// Признак наличия режима профилирования
	Uns	  voltagesCount;		// Количество регистров напряжений
	Uns   faultsCount;			// Количество регистров аварий
	Uns	  *pLogNumber;			// Указатель на номер текущей записи
	Uns	  readLogNumber;		// Номер записи при чтении
	Uns  *pTime;                // Указатель на текущее время
	Uns  *pDate;                // Указатель на текущую дату
	Uns  *pStatus;              // Указатель на статус работы
	Uns   statusPrev;			// Предыдущее состояние статуса работы
//	Uns   statusMask;			// Маска статусного регистра
	Uns  extCmdValue;         // Указатель на команду в журнал событий
	Uns   cmdValue;         // Указатель на команду в журнал событий
	Uns  *pCmdSource;           // Указатель на источник команды
	Uns  *pPosition;            // Указатель на текущее положение
	Uns  *pTorque;              // Указатель на момент
	Uns  *pVoltages;			// Указатель на напряжения сети
	Uns  *pFaults[4];			// Буфер аварий
	Uns   faultsFiltr[4];		// Буфер для фильтрации аварий
	Uns   cmdWord;				// Значение команды для записи/чтения в журнал
	Uns   prevCmd;
	Uns   delayTimer;			// Таймер паузы предварительного запуска
	Uns   delayTout;			// Таймаут задержки для формирования записи в журнал
	Uns   data[20];  			// Буфер данных для записи в память


	TInfoModule			Im;

	TLogEvent			LogEvent;								// Экземпляр структуры журнала событий
	TRamLogBuff  		LogEventBuffer[LOG_EV_BUF_CELL_COUNT];	// Буфер параметров, которое записываются до наступления события
	TLogCmd				LogCmd;									// Экземпляр структуры журнала команд
	TLogParam 			LogParam;								// Экземпляр структуры журнала изменения параметров
	TLogSim 			LogSim;								// Экземпляр структуры журнала SIM
} TStat;

void Stat_Init(TStat *);
void Stat_Update(TStat *);
__inline void Stat_DataUpdate(TStat *);
void EEPROM_Func(Byte Memory, Byte Func, Uns Addr, Uns *Data, Uns Count);

Bool ReadWriteEeprom(pFM25V10 eeprom, Byte func, Uns addr, Uns *pData, Uns count);
void DataBufferPre(TStat *);
void LogEvControl(TStat *);
void LogCmdControl(TStat *);
void LogParamMbExec(TStat *);
void LogParamControl(TStat *);
//ma LogSim
void LogSimControl(TStat *);
void BlinkBluetoothLed(TInfoModule *);
void GetCurrentCmd(TStat *);

//Bool ReadWriteClearFlash(pAT25DF041A, Byte, Uns, Uns *, Uns);
//Bool ReadWriteEeprom(pAT25XXX, Byte, Uns, Uns *, Uns);

//extern TAT25XXX 	g_Eeprom;
extern TStat 		g_Stat;
//extern TFM25V10 	g_MainFlash;	// параметры и часть информационного блока
//extern TFM25V10 	g_SecondFlash;  // часть информационного блока
// Глобальные структуры


#endif

