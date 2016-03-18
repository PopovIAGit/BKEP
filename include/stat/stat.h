/*======================================================================
��� �����:          stat.h
�����:
������ �����:
���� ���������:
��������:
���������� ��� ������ � �������� �������
======================================================================*/

#ifndef STAT_
#define STAT_

#include "csl_memory.h"
#include "stat_fm25v10.h"
#include "stat_LogEvent.h"
#include "stat_InfoModuleParams.h"
#include "stat_InfoModule.h"



// ��������������� ��� ������ � �������
//#define MemPar	Eeprom1								// ��������� ������ ����������
//#define MemLog	Eeprom2
#define ParFunc	ReadWriteEeprom							// ������� ��� ������ � ������� ��������� ����������
#define LogFunc ReadWriteEeprom

// ������� ����
//#define IsMemParReady()				(!Eeprom1.Func)
//#define IsMemLogReady()				(!Eeprom2.Func)

//#define IsMainFlashReady()		(!g_MainFlash.func)
//#define IsMemParReady()			(!g_Eeprom.func)

#define	WaitForMainFlash()		while (!IsMainFlashReady()) {AT25DF041A_Update(&g_MainFlash);DELAY_US(1000);}
// ������� ������ � ����
#define FLASH_READ				0x01
#define FLASH_WRITE				0x02
#define FLASH_ERASE_FIRST		0x04
#define FLASH_ERASE_ALL			0x08

// ��������� �������
#define EVLOG_MOD_BUS_ADDR		0x0500
//#define EVLOG_ADDR				REG_LOG
#define EVLOG_DATA_START_ADR	0
#define EVLOG_DATA_CNT			13
#define EVLOG_CNT				2500
#define EVLOG_SIZE				(EVLOG_CNT * EVLOG_DATA_CNT)
#define EVLOG_MOD_BUS_EADDR		(EVLOG_MOD_BUS_ADDR + EVLOG_SIZE - 1)
// ����� �������
// ����� ����������� ������� ��� ��������� ��������� ������� � ������
#define STATUS_EVLOG_MASK		0x6E9 // 11011101001 

#define COMMAND_EVLOG_MASK	0x0007 // 0000000000111 ����� ���������� ������� ��� ��������� ��������� �������

// ��������� ��� ������ � �������� �������
typedef struct {
	Bool  *pEnable;				// ���������� ������
	Bool  execFlag;				// ���� ������������ ������ � ������
	Bool  writeFlag;			// ���� ������ � ������
	Byte  cmd;					// ������� ������ � ��������
	Bool  profEnable;			// ������� ������� ������ ��������������
	Uns	  voltagesCount;		// ���������� ��������� ����������
	Uns   faultsCount;			// ���������� ��������� ������
	Uns	  *pLogNumber;			// ��������� �� ����� ������� ������
	Uns	  readLogNumber;		// ����� ������ ��� ������
	Uns  *pTime;                // ��������� �� ������� �����
	Uns  *pDate;                // ��������� �� ������� ����
	Uns  *pStatus;              // ��������� �� ������ ������
	Uns   statusPrev;			// ���������� ��������� ������� ������
//	Uns   statusMask;			// ����� ���������� ��������
	Uns  extCmdValue;         // ��������� �� ������� � ������ �������
	Uns   cmdValue;         // ��������� �� ������� � ������ �������
	Uns  *pCmdSource;           // ��������� �� �������� �������
	Uns  *pPosition;            // ��������� �� ������� ���������
	Uns  *pTorque;              // ��������� �� ������
	Uns  *pVoltages;			// ��������� �� ���������� ����
	Uns  *pFaults[4];			// ����� ������
	Uns   faultsFiltr[4];		// ����� ��� ���������� ������
	Uns   cmdWord;				// �������� ������� ��� ������/������ � ������
	Uns   prevCmd;
	Uns   delayTimer;			// ������ ����� ���������������� �������
	Uns   delayTout;			// ������� �������� ��� ������������ ������ � ������
	Uns   data[20];  			// ����� ������ ��� ������ � ������


	TInfoModule			Im;

	TLogEvent			LogEvent;								// ��������� ��������� ������� �������
	TRamLogBuff  		LogEventBuffer[LOG_EV_BUF_CELL_COUNT];	// ����� ����������, ������� ������������ �� ����������� �������
	TLogCmd				LogCmd;									// ��������� ��������� ������� ������
	TLogParam 			LogParam;								// ��������� ��������� ������� ��������� ����������
	TLogSim 			LogSim;								// ��������� ��������� ������� SIM
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
//extern TFM25V10 	g_MainFlash;	// ��������� � ����� ��������������� �����
//extern TFM25V10 	g_SecondFlash;  // ����� ��������������� �����
// ���������� ���������


#endif

