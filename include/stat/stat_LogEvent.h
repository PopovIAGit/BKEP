#ifndef _LOG_EV_H
#define _LOG_EV_H

#include "std.h"

#define NUMBER_OF_LOGS				4//ma LOgSim 3
#define LOG_EVENT_TYPE				0x01
#define LOG_CMD_TYPE				0x02
#define LOG_PARAMS_TYPE				0x03
#define LOG_SIM_TYPE				0x04

#define LOGS_CNT				4//ma LOgSim было 3									// Количество журналов устройства

#define LOG_EV_START_ADDR		15000								// Начальный адрес области памяти основной записи журнала
#define LOG_EV_DATA_CNT			20		//sizeof(TGroupE)			// Количество полей в 1 ячейке основной записи журнала
#define LOG_EV_CNT				500									// Количество записей в журнал событий
#define LOG_EV_DATA_CELL		1									// Количество структур в основной записи журнала

#define LOG_EV_BUF_START_ADDR	0									// Начальный адрес буфера журнала
#define LOG_EV_BUF_DATA_CNT		sizeof(TRamLogBuff)					// Количество полей в одной ячейке буфера
#define LOG_EV_BUF_DATA_CELL	5									// Количество ячеек буфера в одной записи

#define LOG_EV_BUF_CELL_COUNT	5									// Количество структур в буфере журнала событий

#define PREV_LEV_INDEX		0										//Индексы для запоминания предыдущего адреса журналов
#define PREV_LCMD_INDEX		1
#define PREV_LPAR_INDEX		2
//ma LogSim
#define PREV_LSIM_INDEX		3

#define LOG_EV_RAM_DATA_ADR		GetAdr(ramGroupE)				// Адрес группы журнала в Ram
#define LOG_EV_RAM_DATA_LADR	(LOG_EV_RAM_DATA_ADR * LOG_EV_DATA_CNT)
#define LOG_EV_SIZE				(LOG_EV_CNT * LOG_EV_DATA_CNT * LOG_EV_DATA_CELL)

// 7500 полей
#define LOG_CMD_START_ADDR		2000								// Начальный адрес журнала команд
#define LOG_CMD_DATA_CNT		7 //добавилось 1 поле положение								//было 5 с появлением дискретных выходов добавилось								// Количество записываемых в память полей
#define LOG_CMD_CNT				800								    // Емкость журнала команд (количество записей)

// 2500 полей
#define LOG_PARAM_START_ADDR	11000								// Начальный адрес журанала изменения параметров
#define LOG_PARAM_DATA_CNT		5									// Количество записываемых полей (поля структуры журнала: Дата, Время и т.д.)
#define LOG_PARAM_CNT			500									// Емкость журнала изменения параметров

// 10 полей //ma LogSIM
#define LOG_SIM_START_ADDR		14000								// Начальный адрес журнала команд
#define LOG_SIM_DATA_CNT		13									// Количество записываемых в память полей
#define LOG_SIM_CNT				10		   						    // Емкость журнала команд (количество записей)


#define FAULTS_COUNT_DEF			4				// Количество регистров дефектов
#define LOG_EV_BUF_DATA_COUNT		20				// Определяет емкость буфера журнала событий
#define LOG_CMD_BUF_DATA_COUNT		7//6				// Определяет емкость буфера журнала команд
#define LOG_PARAM_BUF_DATA_COUNT	5				// Определяет емкость буфера журнала изменения параметров
//ma LogSIM
#define LOG_SIM_BUF_DATA_COUNT		13				// Определяет емкость буфера журнала SimID
#define MB_PARAM_BUF_COUNT			5

// Структура для работы с журналом событий
typedef struct
{
	Bool Enable;							// Разрешение работы журнала событий
	Bool ExecFlag;							// Флаг формирования буфера журнала
	Bool WriteFlag;							// Флаг разрешения записи журнала в память
	Bool EventFlag;							// Флаг возникновения события
	Bool FirstEvent;						// Флаг первого события после включения питания
	Uns  FaultsCount;						// Количество регистров неисправностей
	Uns  FaultsState[FAULTS_COUNT_DEF];		// Буфер регистров неисправностей
	Uns  FaultsFiltr[FAULTS_COUNT_DEF];		// Буфер фильтрации регистров неисправностей
	Uns  FaultsFiltrPrev[FAULTS_COUNT_DEF];	// Буфер фильтрации регистров неисправностей
	Uns *Time;
	Uns *Date;
	Uns *Status;
	Int *PositionPr;
	Uns *Torque;
	Uns *Ur;
	Uns *Us;
	Uns *Ut;
	Uns *Iu;
	Uns *Iv;
	Uns *Iw;
	Int *Temper;
	Uns *Inputs;
	Uns *Outputs;
	Uns *Seconds;
	Uns Data[LOG_EV_BUF_DATA_COUNT];		// Буфер журнала событий
} TLogEvent, *pTLogEvent;



typedef struct
{
	Bool Enable;							// Флаг разрешения работы журнала
	Bool ExecFlag;							// Флаг формирования буфера журнала
	Bool WriteFlag;							// Флаг разрешения записи журнала
	Uns *Time;								// Указатель на время
	Uns *Date;								// Указатель на дату
	Uns *Seconds;							// Указатель на секунды
	Uns *StatusReg;							// Указатель на статусный регистр блока
	Uns CmdReg;								// Регистр команд
	Uns CmdRegPrev;							// Предыдущее значение регистра команд 
	Uns *StatDigOut;							// регистр состояния дискретных выходов
	Uns *Position;
	Uns Data[LOG_CMD_BUF_DATA_COUNT];		// Буфер журнала команд
} TLogCmd, *pTLogCmd;

typedef struct
{
	Bool Enable;							// Флаг разрешения работы журнала
	Bool ExecFlag;							// Флаг формирования буфера журнала
	Bool WriteFlag;							// Флаг разрешения записи
	Uns *Time;								// Указатель на время
	Uns *Date;								// Указатель на дату
	Uns *Seconds;							// Указатель на секунды
	Uns Addr;								// Адрес параметра
	Uns NewValue;							// Новое значение параметра
//	Bool MbChangeParam;
	Uns MbIndex;
	Uns Data[LOG_PARAM_BUF_DATA_COUNT];		// Буфер журнала
	Uns MbBuffer[MB_PARAM_BUF_COUNT];		// Буфер параметров Модбас
} TLogParam, *pTLogParam;

typedef struct
{
	Bool Enable;							// Флаг разрешения работы журнала
	Bool ExecFlag;							// Флаг формирования буфера журнала
	Bool WriteFlag;							// Флаг разрешения записи журнала
	Uns *Time;								// Указатель на время
	Uns *Date;								// Указатель на дату
	Uns *Seconds;							// Указатель на секунды
	Uns NewSimID[10];							// Регистр команд
	Uns Data[LOG_SIM_BUF_DATA_COUNT];		// Буфер журнала команд
} TLogSim, *pTLogSim;

void LogEventUpdate(TLogEvent *);					// Функция формирования журнала событий
void LogCmdUpdate(TLogCmd *);				// Функция формирования журнала команд
void LogParamUpdate(TLogParam *);			// Функция формирования журнала параметров
//ma LogSIM
void LogSimUpdate(TLogSim *);			// Функция формирования журнала SimID

#endif





