/*======================================================================
Имя файла:          core_VlvDrvCtrl.h
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		16.04.2014
Описание:
модуль управления задвижкой
======================================================================*/

#ifndef VLV_DRV_CNTRL_
#define VLV_DRV_CNTRL_
//----------- Подключение заголовочных файлов ------------------------------
#include "config.h"
#include "g_Structs.h"
#include "core_Menu.h"

//--------------------- Константы-------------------------------------------
// Коды команд управления
#define CMD_STOP				0x0001	// Стоп
#define CMD_CLOSE				0x0002	// Закрыть
#define CMD_OPEN				0x0004	// Открыть
#define CMD_MOVE				0x0008	// Переместить
#define CMD_DEFSTOP				0x0800	// Стоп при аварии(не является командой, добавлено для журнала)  : Добавил PIA 12.09.2012
#define CDM_DISCROUT_TEST		0x1000	// Тест дискретных выходов
#define CMD_DISCRIN_TEST		0x2000	// Тест дискретных входов
#define CMD_ON_BLT				0x4000	// включение Bluetooth
#define CMD_OFF_BLT				0x8000	// выключение Bluetooth

// Источник команды управления
#define CMD_SRC_BLOCK		0x0400	// Аналогвый интерфейс
#define CMD_SRC_PDU			0x0800	// Пульт дистанционного управления
#define CMD_SRC_MPU			0x1000	// Местный пост управления
#define CMD_SRC_DIGITAL		0x2000	// Дискретный интерфейс
#define CMD_SRC_SERIAL		0x4000	// Последовательный интерфейс
#define CMD_SRC_ANALOG		0x8000	// Аналогвый интерфейс
#define CMD_SRC_BLUETOOTH	0x200   // источник команды Bluetooth интерфейс
// Константы для работы с положением
#define POS_UNDEF	0x7FFFFFFF
#define POS_ERR		10

// Состояния входов ТУ
#define TU_OPEN			0x1
#define TU_CLOSE		0x2
#define TU_STOP_OPEN	0x4
#define TU_STOP_CLOSE	0x8

#define VLV_CLB_FLAG    0x03
//--------------------- Макросы --------------------------------------------
//-------------------- Структуры -------------------------------------------

// Команда управления
typedef enum {
  vcwNone=0,		// Нет команды
  vcwStop,			// Стоп
  vcwClose,			// Закрыть
  vcwOpen,			// Открыть
  vcwTestClose,		// Тестовое закрыть
  vcwTestOpen,		// Тестовое открыть
  vcwDemo,			// Демо режим
  vcwTestEng		// Тест двигателя
} TValveCmd;

// Настройка режима МУ/ДУ
typedef enum {
  mdOff=0,			// Выключен
  mdSelect,			// Выбор режима МУ/ДУ
  mdMuOnly,			// Только режим МУ
  mdDuOnly			// Только режим ДУ
} TMuDuSetup;

// Тип уплотнения
typedef enum {
  vtNone=1,			// Без уплотнения
  vtClose,			// Уплотнение в закрыто
  vtOpen,			// Уплотнение в открыто
  vtBoth			// Уплотнение в закрыто и открыто
} TBreakMode;

// Источник команд для режима ДУ
typedef enum {
	mdsAll=0,		// Все интерфейсы
	mdsDigital,		// Только дискретный
	mdsSerial		// Только последовательный
} TDuSource;

// Тип реверса
typedef enum {
  rvtStop=0,		// Переход в стоп
  rvtAuto,			// Автоматический реверс
  rvtNone			// Игнорирование команд при подаче команд на реверс
} TReverseType;

// Структура для работы с местным управлением
typedef struct {
	Bool				 Enable;				// Наличие местного управления
	Uns				 	*BtnKey;				// Команда с ручек
	Bool				 CancelFlag;			// Флаг отмена команды
} TMpuControl;

// Структура для работы с телеуправлением
typedef struct {
	Bool				 Enable;			// Наличие телеуправления
	Bool				 LocalFlag;			// Флаг TRUE, если управление местное
	Uns				 	*State;				// Состояние входов телеуправления
	Bool				 Ready;				// Готовность обработки команд ТУ на движение
	Uns					*LockSeal;			// Блокировка залипания
} TTeleControl;

// Структура при работе с задвижкой
typedef struct {
	Bool				PosRegEnable;		// Наличие режима позиционирования
	Bool            	BreakFlag;			// Флаг работы с уплотнением SVS.1
	LgInt				Position;			// Целевое положение
	Uns				 	BreakDelta;			// Максимальное смещение при работе с уплотнением
	TBreakMode			*BreakMode;			// Режим работы с уплотнением
	TCalibState			*CalibStates;		// Статус калибровки
	LgUns				*CalibFullStep;		// Данные о колличестве меток энкодера необходимых для прохождения пути

} TValveControl;

// Структура для работы с журналом событий
typedef struct {
	Uns				 Value;					// Значение команды управления для журнала событий
	Uns				 Source;				// Источник команды управления для журнала событий
} TEvLogControl;

// Структура статусного регистра
typedef union {
	Uns all;
	struct {
		Uns Stop:1;			// 0		Стоп
		Uns Fault:1;		// 1		Авария
		Uns Closing:1;		// 2		Идет закрытие
		Uns Opening:1;		// 3		Идет открытие
		Uns Rsvd1:1;		// 4		Резерв
		Uns Closed:1;		// 5		Закрыто
		Uns Opened:1;		// 6		Открыто
		Uns Rsvd2:1;		// 7		Резерв
		Uns MuDu:1;			// 8		Местное управление
		Uns Rsvd:7;			// 9-15 	Резерв
	} bit;
} TVlvStatusReg;

// Структура управления приводом
typedef struct {
	TVlvStatusReg	*Status;			// Статус работы
	TValveCmd		*ControlWord;		// Команда управления
	TMuDuSetup		*MuDuSetup;			// Настройка режима МУ/ДУ
	TDuSource		*DuSource;			// Источник команд для режима ДУ
	TReverseType	*ReverseType;		// Тип реверса
	TMpuControl		 Mpu;				// Местное управление
	TTeleControl	 Tu;				// Телеуправление
	TValveControl	 Valve;				// Управление задвижкой
	TEvLogControl	 EvLog;				// Журнал событий
	TValveCmd		 Command;			// Внутренняя команда
	Uns				 MuDuInput;			// Состояние входа МУ/ДУ
	Uns				 ActiveControls;	// Состояние активности интерфейсов управления
	Uns				 StartDelay;		// Пауза при обработке команды управления
	Uns 			 IgnorComFlag;		// Флаг игнорирования команды управления если уже находимся в крайнем положении
	void (*StopControl)(void);			// Функция останова управления
	void (*StartControl)(TValveCmd ControlWord); // Функция старта управления
} TCoreVlvDrvCtrl;

//------------------- Глобальные переменные --------------------------------
//------------------- Протатипы функций ------------------------------------

void Core_ValveDriveInit(TCoreVlvDrvCtrl *);
void Core_ValveDriveStop(TCoreVlvDrvCtrl *);
void Core_ValveDriveUpdate(TCoreVlvDrvCtrl *);



#endif
