/*======================================================================
Имя файла:          g_Structs.h
Автор:
Версия файла:
Дата изменения:
Описание:
Описание структур
========================================================================*/

#ifndef G_STRUCTS_
#define G_STRUCTS_

#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------
//================== СТАТУСЫ ============================
//--------------------------------------------------------
// статусный регистр
#define STATUS_STOP				BIT0
#define STATUS_FAULT			BIT1
#define STATUS_CLOSING			BIT2
#define STATUS_OPENING			BIT3
#define STATUS_TEST				BIT4
#define STATUS_CLOSED			BIT5
#define STATUS_OPENED			BIT6
#define STATUS_MUFTA			BIT7
#define STATUS_MU_DU			BIT8
#define STATUS_TEN				BIT9
#define STATUS_BLUETOOTH		BIT10
#define STATUS_ALARM_BLK_INDIC	BIT11
#define STATUS_ALARM_TS			BIT12
#define STATUS_DEFECT			BIT13
#define STATUS_DEFECT_BLK_INDIC	BIT14
#define STATUS_DEFECT_TS		BIT15

//-------------------Статус работы----------------------------
// Статус работы
#define STATUS_MOVE_MASK	0xC

typedef union _TStatusReg {
	Uns all;
	struct {
		
		Uns Stop:1;				// 0     Стоп
		Uns Fault:1;			// 1     Авария
     	Uns Closing:1;    		// 2     Идет закрытие
     	Uns Opening:1;			// 3     Идет открытие
    	Uns Test:1;				// 4     Идет тест
     	Uns Closed:1;			// 5     Закрыто
     	Uns Opened:1;			// 6     Открыто
		Uns Mufta:1;			// 7     Муфта
		Uns MuDu:1;				// 8     Местное управление
     	Uns Ten:1;        		// 9     Включен ТЕН
     	Uns EnableBluetooth:1;  // 10    Резервное питание
		Uns Rsvd:2;			    // 11-12	 Резерв
		Uns Defect:1;			// 13	 Неисправность
		Uns Rsvd2:2;		    // 14 -15 	Резерв
	} bit;
} TStatusReg;

// Статус калибровки
typedef enum {
	csNone  = 0,			// Привод не калиброван
	csClose = 1,			// Задано только закрыто
	csOpen  = 2,			// Задано только открыто
	csCalib = 3				// Привод калиброван
} TCalibState;

// Скорость связи ModBus
typedef enum {
	br2400   = 0,			// 2400 Бод
	br4800   = 1,			// 4800 Бод
	br9600   = 2,			// 9600 Бод
	br19200  = 3,			// 19200 Бод
	br38400  = 4,			// 38400 Бод
	br57600  = 5,			// 57600 Бод
	br115200 = 6			// 115200 Бод
} TBaudRate;

// Режим связи
typedef enum {
	pmNone = 0,				// Нет проверки бита четности, 2 стоповых бита
	pmOdd  = 1,				// Проверка на нечетность, 1 стоповый бит
	pmEven = 2				// Проверка на четность, 1 стоповый бит
} TParityMode;

// Структура для работы с ПДУ
typedef struct _TPduData {
	Uns Read;            // Чтение данных
	Uns Write;           // Запись данных
	Uns Rsvd;            // Резерв
	Uns Type;            // Тип меню
	Uns Key;             // Код клавиши
} TPduData;

// Диагностика процесса
#define PROC_NOMOVE_MASK	0x0001
#define PROC_PHENG_MASK		0x0002
#define PROC_CALIB_MASK		0x001C
#define PROC_OVERW_MASK		0x0020
#define PROC_DRV_T_MASK 	0x0040
#define PROC_RESET_MASK		0x00A1
//#define PROCESS_EVLOG_MASK	0x00A1
#define PROCESS_EVLOG_MASK	0x00D2
typedef union _TProcessReg {
	Uns all;
	struct {
		Uns NoMove:1;		// 0     Нет движения
		Uns PhOrd:1;		// 1     Чередование фаз двигателя
		Uns NoClose:1;		// 2     Не задано положение закрыто
		Uns NoOpen:1;	   	// 3     Не задано положение открыто
		Uns NoCalib:1;		// 4     Калибровка не выполнена
		Uns Overway:1;		// 5     Уплотнение не достигнуто
		Uns Mufta:1;		// 6	 Муфта
		Uns MuDuDef:1;		// 7     Ошибка по дискретным входам Му/Ду
		Uns Rsvd:8;			// 8-15  Резерв
	} bit;
} TProcessReg;

// Диагностика сети
#define NET_UV_MASK			0x0007
#define NET_OV_MASK			0x0038
#define NET_PHO_MASK		0x0040
#define NET_VSK_MASK		0x0080
#define NET_BV_MASK			0x0700
#define NET_OV_MAX_MASK		0x3800
//#define NET_EVLOG_MASK		0x7FFF //0x3F7F
#define NET_EVLOG_MASK		0x3F3F

typedef union _TNetReg {
	Uns all;
	struct {
		Uns UvR:1;			// 0     Понижение напряжения в фазе R
		Uns UvS:1;			// 1     Понижение напряжения в фазе S
		Uns UvT:1;			// 2     Понижение напряжения в фазе T
		Uns OvR:1;			// 3     Превышение напряжения в фазе R
		Uns OvS:1;			// 4     Превышение напряжения в фазе S
		Uns OvT:1;			// 5     Превышение напряжения в фазе T
		Uns PhOrd:1;			// 6     Неверное чередование фаз сети
		Uns VSk:1;			// 7     Асимметрия напряжения входной сети
		Uns BvR:1;			// 8     Обрыв фазы R входной сети
		Uns BvS:1;			// 9     Обрыв фазы S входной сети
		Uns BvT:1;			// 10    Обрыв фазы T входной сети
		Uns OvR_max:1;			// 11    Превышение напряжения в фазе R на 47%
		Uns OvS_max:1;			// 12    Превышение напряжения в фазе S на 47%
		Uns OvT_max:1;			// 13    Превышение напряжения в фазе T на 47%
		Uns BreakRST:1;			// 14    Обрыв входного напряжения выключение вводного автомата
		Uns Rsvd:1;			// 15 Резерв
	} bit;
} TNetReg;

// Диагностика нагрузки
#define LOAD_PHL_MASK		0x0007
#define LOAD_I2T_MASK		0x0008
#define LOAD_SHC_MASK		0x00E0
#define LOAD_UNL_MASK		0x0400
#define LOAD_ISK_MASK		0x0800
#define LOAD_RESET_MASK		0x0807
#define LOAD_EVLOG_MASK		0x00EF
typedef union _TLoadReg {
	Uns all;
	struct {
		Uns PhlU:1;			// 0     Обрыв фазы U
		Uns PhlV:1;			// 1     Обрыв фазы V
		Uns PhlW:1;			// 2     Обрыв фазы W
		Uns I2t:1;			// 3     Время-токовая перегрузка
		Uns Rsvd1:1;			// 4     Резерв
		Uns ShCU:1;			// 5	 Короткое замыкание в фазе U
		Uns ShCV:1;			// 6	 Короткое замыкание в фазе V
		Uns ShCW:1;			// 7	 Короткое замыкание в фазе W
		Uns Rsvd2:3;			// 8-10	 Резерв
		Uns ISkew:1;			// 11    Ассиметрия тока
		Uns Rsvd:4;			// 13-15 Резерв
	} bit;
} TLoadReg;

// Диагностика устройства
#define DEV_ERR_MASK		0x009F
#define DEV_TMP_MASK		0x0060
#define DEV_RSC_MASK		0x0800
//#define DEV_EVLOG_MASK		0x0400
#define DEV_EVLOG_MASK		0x0421
typedef union _TDeviceReg {
	Uns all;
  struct {
		Uns PosSens:1;		// 0     Сбой датчика положения
		Uns Memory1:1;		// 1     Сбой памяти 1
		Uns Memory2:1;		// 2     Сбой памяти 2
		Uns Rtc:1;		// 3     Сбой часов реального времени
		Uns TSens:1;		// 4     Сбой датчика температуры
		Uns Th_BCP:1;		// 5     Перегрев блока БКП
		Uns Tl_BCP:1;		// 6     Переохлождение блока БКП
		Uns Th_BCD:1;		// 7     Перегрев блока БКД
		Uns Tl_BCD:1;		// 8     Переохлождение блока БКД
		Uns Dac:1;		// 9     Сбой ЦАП
		Uns LowPower:1;		// 10     Выключение БКД
		Uns NoBCP_Connect:1;	// 11
		Uns Rsvd:4;       	// 12-15 Резерв
  } bit;
} TDeviceReg;


// Струкутура объединенных аварий
typedef struct {
	TProcessReg Proc;
	TNetReg     Net;
	TLoadReg    Load;
	TDeviceReg  Dev;
} TFltUnion;

// Группа аварий
typedef enum {
	pgProcess = 0,			// Группа аварий процесса
	pgNet     = 1,			// Группа аварий сети
	pgLoad    = 2,			// Группа аварий нагрузки
	pgDevice  = 3,			// Группа аварий устройства
	pgCount   = 4			// Количество групп
} TPrtGroup;

// Команды БУР
typedef enum
{
	bcmNone = 0,				// Нет команды
	bcmStop,					// Стоп
	bcmClose,					// Закрыть
	bcmOpen,					// Открыть
	bcmSetDefaultsUser,			// Пользовательские параметры по умолчанию
	bcmCalibReset,				// Сброс калибровки
	bcmPrtReset,				// Сброс защит
	bcmLogClear,				// Очистка журнала
	bcmCycleReset,				// Сброс циклов
	bcmSetDefaultsFact,			// Установка заводских параметров по умолчанию
	bcmDefStop,					// Спсевдо команда стоп при авариях, нужна для определения общей продолжительности работы
	bcmDiscrOutTest,			// Тест дискретных выходов
	bcmDiscrInTest				// Тест дискретных входов
} TBurCmd;


// Место установки БУР (пожарка - Линейная часть/Автоматика и телемеханика)
typedef enum {
	spLinAuto   = 0,		// 0 Линейная часть/Автоматика и телемеханика
	spFire 		= 1 		// 1 Пожарка
}TSettingPlace;

// Метод останова
typedef enum {
	smSlowDown 		= 0,	// выбег
	smReverse   	= 1,	// реверс
	smDynBreak  	= 2		// динамика
} TStopMethod;

// Управление контакторами д
typedef enum {
	cgStop  = 0,
	cgOpen  = 1,
	cgClose = 2
} TContactorGroup;

// тип привода
typedef enum {
	Empty 	  = 0,
	dt100_A25 = 1,
	dt100_A50 = 2,
	dt400_B20 = 3,
	dt400_B50 = 4,
	dt800_V40 = 5,
	dt1000_V20  = 6,
	dt4000_G9   = 7,
	dt4000_G18  = 8,
	dt10000_D6  = 9,
	dt10000_D12 = 10,
	dt10000_D10 = 11,
	dt15000_D10 = 12,
	dt20000_F40 = 13
}TDriveType;
/*
// Управление работой защит
typedef enum {
  pmOff       = 0,		// Защита выключена
  pmBlkSign   = 1,		// Сигнализация на блоке
  pmBlkTsSign = 2,		// Сигнализация на блоке и ТС
  pmSignStop  = 3,		// Сигнализация и останов
  pmCount     = 4		// Количество режимов
} TPrtMode;*/

// Управление работой защит
typedef enum {
  pmOff       = 0,		// Защита выключена
  pmOn  = 1,		// Сигнализация и останов
  pmCount     = 2		// Количество режимов
} TPrtMode;

// Тип штока
typedef enum {
  rdtNormal  = 0,			// Нормальный
  rdtInverse = 1			// Инверсный
} TRodType;

// Команды задания/сброса
typedef enum {
  trNone     = 0,			// Нет команды
  trTask     = 1,			// Задать
  trReset    = 2 			// Сбросить
} TTaskReset;

// Нормальное состояние входа
typedef enum {
  nsOpened = 0,				// Нормально-открытый контакт
  nsClosed = 1				// Нормально-закрытый контакт
} TNormState;

// Режим индикации тока
typedef enum {
  imRms     = 0,		  	// Действующие значения
  imPercent = 1			  	// В процентах от номинального тока двигателя
} TIndicMode;

// Состояние светодиодов блока
typedef union _TLedsReg {	// МОИ
	Uns all;
	struct {
		Uns Opened:1;     	// 0     Открыто
		Uns Muff:1;      	// 1     Муфта
		Uns Defect:1;       // 2     Неисправность
		Uns Closed:1;     	// 4     Закрыто
		Uns Fault:1;     	// 3     Авария
		Uns Pdu:1;			// 5	 ПДУ
		Uns MuDu:1;       	// 6     Режим
		Uns DspOn:1;		// 7	 DSP_ON
		Uns Rsvd:8;      	// 6-15  Резерв
	} bit;
} TLedsReg;

// Тип задвижки клин/шибер
typedef enum {
  vtKlin     = 0,		  	// Клиновая задвижка
  vtShiber   = 1			// Шиберная задвижка
} TValveType;

// Тип работы тена
typedef enum {
	tmControl 		= 0, 	// Управление по уставкам
	tmTimerControl	= 1, 	// Управление по уставкам с задержкой времени
	tmAlwaysOn 		= 2,	// Всегда включен
	tmHandControl 	= 3		// Ручное управление
}TTenMode;

// Структуры для ТЭКовского модбаса

// Регистры ТЭКа
// Технологический регистр
typedef union _TTEK_TechReg
{
	Uns all;
	struct
	{
		Uns Opened:1;		// 0 		Положение "Открыто"
		Uns Closed:1;		// 1 		Положение "Закрыто"
		Uns Mufta1:1;		// 2 		В нашем случае муфта не разделяется на трогание или движение
		Uns Mufta2:1;		// 3 		Дублируем муфту
		Uns Rsvd2:3;		// 4-6 		Резерв
		Uns MuDu:1;			// 7 		МУ/ДУ
		Uns Opening:1;		// 8 		"Открывается"
		Uns Closing:1;		// 9 		"Закрывается"
		Uns Stop:1;			// 10 		Стоп
		Uns Rsvd3:2;		// 11-12 	Резерв
		Uns Ten:1;			// 13; 		Включен тен
		Uns Rsvd4:2;		// 14-15; 	Резерв
	} bit;
} TTEK_TechReg;

// Регистр дефектов
typedef union _TTEK_DefReg
{
	Uns all;
	struct
	{
		Uns I2t:1;		// 0 	Время-токовая защита
		Uns ShC:1;		// 1 	Короткое замыкание
		Uns Drv_T:1;	// 2 	Перегрев двигателя
		Uns Uv:1;		// 3 	Пониженеое напряжение фаз сети
		Uns Phl:1;		// 4 	Обрыв фазы электродвигателя
		Uns NoMove:1;	// 5 	Нет движения
		Uns Ov:1;		// 6 	Перенапряжение на входе
		Uns Bv:1;		// 7 	Критическое снижение напряжения на входе
		Uns Rsvd:1;		// 8 	Резерв
		Uns Th:1;		// 9  	Перегрев блока
		Uns Tl:1;		// 10 	Переохлождение блока
		Uns Rsvd1:1;	// 11 	Резерв
		Uns PhOrdU:1;	// 12   Неверное чередование фаз сети
		Uns PhOrdDrv:1;	// 13   Чередование фаз двигателя
		Uns DevDef:1;	// 14 	Дефект устройства (кроме перегрева и переохлаждения)
		Uns Rsvd2:1;	// 15 	Резерв
	} bit;
} TTEK_DefReg;

// Регистр команд
typedef union _TTEK_ComReg
{
	Uns all;
	struct
	{
		Uns Stop:1;				// 0 	 Стоп
		Uns Open:1;				// 1	 Открыть
		Uns Close:1;			// 2 	 Закрыть
		Uns Rsvd:2;				// 3-4 	 Резерв
		Uns PrtReset:1;			// 5 	 Сброс защит
		Uns Rsvd1:2;			// 6-7   Резерв
		Uns EnDiscrInTest:1; 	// 8 	 Включение тесирования дискретных входов
		Uns DisDiscrInTest:1; 	// 9 	 Включение тесирования дискретных входов
		Uns EnDiscrOutTest:1;	// 10 	 Включение тесирования дискретных выходов
		Uns DisDiscrOutTest:1; 	// 11 	 Включение тесирования дискретных выходов
		Uns Rsvd2:4;			// 12-15 Резерв
	} bit;
} TTEK_ComReg;

// Регистр ТС/ТУ
typedef union _TTEK_Discrete
{
	Uns all;
	struct
	{
		Uns InOpen:1;			// 0 	 Вход "Открыть"
		Uns InClose:1;			// 1 	 Вход "Закрыть"
		Uns InStop:1;			// 2 	 Вход "Стоп"
		Uns InMu:1;				// 3 	 Вход "МУ"
		Uns InDu:1;				// 4 	 Вход "ДУ"
		Uns OutOpened:1;		// 5 	 Выход "Открыто"
		Uns OutClosed:1;		// 6 	 Выход "Закрыто"
		Uns OutMufta:1;			// 7 	 Выход "Муфта"
		Uns OutFault:1;			// 8  	 Выход "Авария"
		Uns OutOpening:1;		// 9 	 Выход "Открывается"
		Uns OutClosing:1;		// 10 	 Выход "Закрывается"
		Uns OutMuDu:1;			// 11    Выход "МУ/ДУ"
		Uns OutNeispr:1;		// 12	Выход "Неисправность"
		Uns Rsvd:1;				// 13 	Резерв
		Uns IsDiscrOutActive:1;	// 14 	Состояние режима тестирования выходов
		Uns IsDiscrInActive:1;	// 15 	Состояние режима тестирования входов
	} bit;
} TTEK_Discrete;


#ifdef __cplusplus
}
#endif // extern "C"

#endif

