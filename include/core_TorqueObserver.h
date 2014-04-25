/*======================================================================
Имя файла:          core_TorqueObserver.h
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		16.04.2014
Описание:
модуль управления задвижкой
======================================================================*/

#ifndef TORQUE_OBSERVER_
#define TORQUE_OBSERVER_

//----------- Подключение заголовочных файлов ------------------------------
#include "config.h"
#include "peref_ApFilter3.h"
//--------------------- Константы-------------------------------------------
#define TORQ_MIN_PR		10	// Минимальное значение в процентах
#define TORQ_MAX_PR		110	// Максимальное значение в процентах
#define TQ_VOLT_MIN		120	// Минимальное напряжение для расчета момента
#define TQ_VOLT_MAX		260	// Максимальное напряжение для расчета момента
//--------------------- Макросы --------------------------------------------
//-------------------- Структуры -------------------------------------------

// Структуры для работы с кубом
typedef struct {
	Int X;
	Int Y;
	Int Z;
} TCubPoint;

#define CUB_COUNT1		4
#define CUB_COUNT2		5
typedef struct {
	Int Data[CUB_COUNT1][CUB_COUNT2];
} TCubArray;

typedef struct {
	Int *X_Value;
	Int *X_Array;
	Int *Y_Value;
	Int *Y_Array;
} TCubConfig;

typedef struct {
	TCubPoint  Input;
	Int        Output;
	Uns        Num1;
	Uns        Num2;
	Int        PtR;
	Int        PtT;
	Int       *InputX;
	Int       *InputY;
	TCubPoint  Min;
	TCubPoint  Max;
	TCubPoint  Points[CUB_COUNT1][CUB_COUNT2];
} TCubStr;

// Структуры для расчета момета
typedef struct {
	Bool    	ObsEnable;		// Разрешение расчета
	Uns			*TransCurr;		// ток перехода между поверхностями
	Uns  		TorqueMax;		// Максимальный момент
	Uns			*TorqueSetPr;	// Задание на момент в процентах
	Uns 		*Imidpr;		// Данные по средниму току в % от In
	Uns     	Indication;		// Индикация значения момента
	Uns			Corr40Trq;		// Коэффициент корректировки индикации момента для значний меньше 40%
	Uns			Corr60Trq;		//	-//- 60%
	Uns			Corr80Trq;		//	-//- 80%
	Uns			Corr110Trq;		// 	-//- 110%
	APFILTER3	Trqfltr;		// фильтр для моментаы
	TCubStr 	Cub1;			// Куб по углу фи
	TCubStr 	Cub2;			// куб по току
	TCubConfig 	TqCurr;			// Настройки для куба по току
	TCubConfig 	TqAngUI;		// Настройки для куба по углу фи
} TTorqObs;


//------------------- Глобальные переменные --------------------------------
//------------------- Протатипы функций ------------------------------------
void CubInit(TCubStr *, TCubConfig *Cfg);
void CubRefresh(TCubStr *, TCubArray *Array);
void CubCalc(TCubStr *);
void Core_TorqueInit(TTorqObs *);
void Core_TorqueCalc(TTorqObs *);
#endif
