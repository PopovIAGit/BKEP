/*======================================================================
Имя файла:          core_Menu.h
Автор:
Версия файла:
Дата изменения:
Применяемость:      
Описание:
Библиотека для работы с меню под двухстрочный индикатор
======================================================================*/

#ifndef CORE_MENU_
#define CORE_MENU_

#include "config.h"
#include "core_MenuGlobal.h"

#define GROUP_EDIT_PAR		1
#define GROUP_FACT_PAR		2

#define GetDcr(Adr, pDcr)	Core_MenuReadDcr(&g_Core.menu, pDcr, Adr)

#define IsPassword1()		(g_Ram.ramGroupH.Password1 != 0)
#define IsPassword2()		(g_Ram.ramGroupH.Password2 != 0)

#define KEY_NONE       0  // Открыть
#define KEY_OPEN       1  // Открыть
#define KEY_CLOSE      2  // Закрыть
#define KEY_STOP       3  // Стоп
#define KEY_UP         4  // Вверх
#define KEY_LEFT       5  // Влево (при редактировании параметра)
#define KEY_RIGHT      6  // Вправо (при редактировании параметра)
#define KEY_DOWN       7  // Вниз
#define KEY_ESCAPE     8  // Сброс
#define KEY_ENTER      9  // Ввод
#define KEY_MENU       10 // Меню				// Не используется
#define KEY_MU_DU      11 // МУДУ				// Не используется
#define KEY_PROG       12 // Программирование	// Не используется

#define KEY_FORWARD		KEY_CLOSE   // Вперед	// Не используется
#define KEY_REVERSE		KEY_OPEN    // Назад	// Не используется


// Структура меню
typedef struct TCoreMenu {
	Byte   State;        // Режим меню
	Char   Key;          // Управляющая команда
	Byte   EditType;     // Тип режима редактирования
	Byte   Level;        // Текущий уровень меню
	Byte   MaxLevel;     // Уровень вложенности меню (1 или 2)
	Uns   *pIndicatorType;   // Флаг разрешения индикации
	Uns	   indicatorTypeIndex;	// Индекс типа индикатора
	Bool   updateMainValues;// Флаг обновления главных значений
	Uns	   groupAUpdateTimer;// Таймер обновления значений в группе А
	Bool   updateHiStr;
	Bool   updateLowStr;
	Uns	   updateTime;	 // Время обновления данных в группе А
	Uns	   timeStartLogo;// Таймер показа заставки
	Bool   logoIsShown;	 // Флаг показа заставки
	Bool   ShowReserved; // Флаг отображения резервов
	Uns   *MinMaxGain;   // Коэффициент диапазона
	Byte   setDefaultGroupNumber; // Номер группы для установки параметров по-умолчанию 1-B 2-C
	Uns    predStartShow;	// предыдущий режим стартового меню
	Uns    prevPositionPr;	// Предыдущее значение полжения
	Uns	   prevTorque;		// Предыдущее значение момента
	Uns	   prevAngl;		// Предыдущее значение угла открытия тиристоров
	Uns    Value;        // Текущее значение параметра
	Byte   Rate;         // Текущий разряд
	Byte   EditRate;     // Максимальный разряд для редактирования
	Bool   Blink;        // Состояния мигания значения
	Uns    BlinkTimer;   // Таймер для мигания
	Uns    BlinkHiTime;  // Время отображения при редактировании
	Uns    BlinkLoTime;  // Время скрытия при редактировании
	Byte   Bit;          // Выводимый бита числа в виде бегущей строки
	Uns    BitTimer;     // Таймер паузы при выводе бита
	Uns    BitTime;      // Пауза при выводе бита

	Bool   SleepActive;  // Состояние режима
	LgUns  SleepTimer;   // Таймер режима
	LgUns  SleepTime;    // Тайм-аут режима
	Bool  *pCalcelFlag;
	Bool  *pDriveTypeFlag;//Тип привода не задан
	Uns    ShowTimer;	 // Таймер показа доп. строк

	Uns   *Data;         // Указатель на буфер данных
	Char  *HiString;     // Указатель на буфер верхней строчки индикатора
	Char  *LoString;     // Указатель на буфер нижней строчки индикатора
	Char   BufValue[6];  // Временный буфер при редактирование в режиме 1
	Char   BufTmp[17];   // Временный буфер для хранения единицы измерения/строкового значения
	
	Bool   EvLogSelected;// Флаг выбора записи
	Uns    EvLogGroup;   // Номер группы журнала
	Uns    EvLogNumber;  // Номер параметра текущей записи в журнал
	Uns    EvLogTime;    // Адрес параметра времени записи журнала
	Uns    EvLogDate;    // Адрес параметра даты записи журнала
	
	struct MENU_CRD Group;     // Структура координаты группы
	struct MENU_CRD SubGroup;  // Структура координаты подгруппы
	struct MENU_CRD EvLog;     // Структура координаты записи журнала
	struct MENU_CRD Param;     // Структура координаты параметра
	struct MENU_DCR Dcr;       // Структура описания параметра
	
	const struct MENU_GROUP  *Groups;   // Указатель на структуры групп
	const struct MENU_GROUP  *SubGroups;// Указатель на структуры подгрупп
	const struct MENU_PARAM  *Params;   // Указатель на структуры параметров
	const struct MENU_STRING *Values;   // Указатель на структуры строковых значений
	const struct ADDR_PARAM_ATS48  *AtsParams;   // Указатель на структуры адресов параметров упп
	struct MENU_STRING        Rsvd;     // Резервная строка
	
	void (*StartDispl)(String Text, Byte Num);
	Bool (*EnableEdit)(Uns PaswwPrt);
	Bool (*WriteValue)(Uns Memory, Uns ParAddr, Uns *Value);
} TCoreMenu, *pCoreMenu;

void Core_MenuInit(TCoreMenu *);
void Core_MenuUpdate(TCoreMenu *);

//TODO Функция обновления кода
Bool UpdateCode(Uns, Uns, Uns, Uns);
Bool SleepMode(TCoreMenu *);
void StartState(TCoreMenu *);

//__inline Bool ReadLog(TCoreMenu *);

void SelectGroup(TCoreMenu *);
void SelectParam(TCoreMenu *);
__inline void EditParam(TCoreMenu *);
__inline void EditDec(TCoreMenu *, Char);
__inline void EditStr(TCoreMenu *, Char);
__inline void EditTime(TCoreMenu *, Char);
__inline void EditDate(TCoreMenu *, Char);
__inline void EditBin(TCoreMenu *);
void ReadDcrFull(TCoreMenu *, Uns);
void ChangeCoordinate(TCoreMenu *, Char, Bool);
void ShowStart(TCoreMenu *);
void ShowGroup(TCoreMenu *);
void ShowParam(TCoreMenu *, Bool);
__inline void ShowValue(TCoreMenu *, Uns, Bool);
__inline Byte DefineBit(TCoreMenu *, Uns);

void Core_MenuDisplay(TCoreMenu *);
void Core_MenuReadDcr(TCoreMenu *, struct MENU_DCR *, Uns);

void StartDispl(String, Byte);
Bool EnableEdit(Uns);
Bool WriteValue(Uns, Uns, Uns *);
void SetDefaultValues(TCoreMenu *, Byte *);
void ShowAddStrings(TCoreMenu *, Uns, Uns);
void SetModBusParams(void);
void SetModBusParams(void);

extern void PFUNC_strRead(Char *, Char *);
extern void PFUNC_strWrite(Char *, Char *);
extern  Int PFUNC_wordRead(Int *);
extern void PFUNC_wordWrite(Int *, Int);

#endif



