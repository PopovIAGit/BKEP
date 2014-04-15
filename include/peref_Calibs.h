/*======================================================================
Имя файла:          peref_Calibs.h
Автор:              Попов И.А.
Версия файла:       01.00
Дата изменения:		04/04/14
Описание:
Работа с калибровкой датчика положения
======================================================================*/

#ifndef CALIBS_
#define CALIBS_

#ifdef __cplusplus
extern "C" {
#endif

//----------- Подключение заголовочных файлов -----------------------------
#include "config.h"
#include "g_Ram.h"

//--------------------- Константы-------------------------------------------
// Статусы калибровки
#define	CLB_CLOSE   0x01  // Задано Закрыто/находимся в зоне Закрыто
#define	CLB_OPEN    0x02  // Задано Открыто/находимся в зоне Открыто
#define	CLB_FLAG    0x03  // Калибровка выполнена

#define REV_MAX		0x3FFF
#define MUFF_CLB_TIME	(3.000 * Prd50HZ)	// Время удержания муфты для калибровок
#define SP_POS_LIMIT 400

//--------------------- Макросы --------------------------------------------
//-------------------- Структуры -------------------------------------------

// Структура управления калибровками
typedef struct {
	Uns TaskClose;          // Задание/сброс Закрыто
	Uns TaskOpen;           // Задание/сброс Открыто
	Uns RevOpen;            // Задание количества оборотов на открытие
	Uns RevClose;           // Задание количества оборотов на закрытие
	Uns RevAuto;            // Автоматическая калибровка
} ClbCommand;

// Структура индикации калибровок
typedef struct {
	Uns    Status;          // Статус калибровки
	Uns    Reserved;        // Резерв
	LgUns  ClosePos;        // Положение Закрыто
	LgUns  OpenPos;         // Положение Открыто
} ClbIndication;

// Структура для работы с датчиком положения
typedef struct TPerefPosition {
	Bool   ResetFlag;       	// Флаг сброса
	Bool   CancelFlag;      	// Флаг отмены команды
	Uns    GearRatio;       	// Передаточное число редуктора * 100
	Uns    Zone;            	// Зона калибровки
	LgUns  RevMax;          	// Максимальное количество оборотов - 1
	LgInt  LinePos;        		// Текущее линейное положение
	LgUns  FullStep;        	// Полный ход
	LgInt  BasePos;         	// Базовое положение
	Uns    CycleData;       	// Параметр для расчета циклов
	Uns    MuffTime;        	// Таймаут успокоения муфты
	Uns    MuffTimer;       	// Таймер успокоения(ожидания срабатывания) муфты
	LgUns  speedMPS;       		// скорость энкодера (метки/с) MarkersPerSecond
	LgUns  speedRPM;        	// скорость двигателя (об/м) RevolutionsPerMinut
	Uns    Reserved;        	// Резерв
	LgInt  GearInv;         	// Коэффициент обратный передаточныму числу редуктора
	LgInt  FstepInv;        	// Коэффициент обратный полному ходу
	Uns   *PosSensPow;    		// Указатель на тип датчика положения
	Uns   *PositionAcc;     	// Указатель на смещение конечных положений
	Uns   *RodType;         	// Указатель на тип штока
	LgUns *AbsPosition;     	// Указатель на текущее положение
	Int   *PositionPr;      	// Указатель на текущее положение в %
	Uns   *FullWay;         	// Указатель на полный ход выходного звена
	Int   *CurWay;          	// Указатель на текущий ход выходного звена
	Uns   *CycleCnt;        	// Указатель на количество расчитанных циклов
	Uns   *MuffFlag;        	// Сигнал муфты
	ClbCommand    *Command;    	// Указатель на структуру управления калибровками
	ClbIndication *Indication; 	// Указатель на структуру индикации
} TPerefPosition;

//------------------- Глобальные переменные --------------------------------
//------------------- Протатипы функций ------------------------------------
void Peref_CalibInit(TPerefPosition *);
void Peref_Calibration(TPerefPosition *);
void Peref_CalibUpdate(TPerefPosition *);
void Peref_CalibControl(TPerefPosition *);
void  CalcClbCycle(TPerefPosition *);

static Byte CheckStatus(TPerefPosition *, Uns);

void Peref_SpeedCalc(TPerefPosition *);

void SpeedTest(void);


#ifdef __cplusplus
}
#endif // extern "C"

#endif
