/*======================================================================
Имя файла:          core_ProtectionsAlarm.h
Автор:				Попов И.А.
Версия файла:		1.00
Дата изменения:		25.04.2014
Описание:
модуль реализации алгоритма защит
======================================================================*/

#ifndef CORE_PROTECTIONS_ALARM_
#define CORE_PROTECTIONS_ALARM_

//----------- Подключение заголовочных файлов ------------------------------
#include "config.h"

//--------------------- Константы-------------------------------------------

#define ON				0
#define OFF				1

#define	CAN_NOT_BE_RESETED		0 // Авария не снимается автоматически
#define	CAN_BE_RESETED			1 // Авария снимается автоматически

#define	INP_LESS_LEVEL			0 // Авария при принижении сигнала заданного уровня
#define	INP_GREATER_LEVEL		1 // Авария при превышении сигнала заданного уровня

#define CAUSE_SHTDWN_ON			1 // После срабатывания аварии вызывает останов по истечении времени
#define	CAUSE_SHTDWN_OFF		0 // Такая опция отключена

#define	HYST_OFF				0 // Без гистерезиса
//--------------------- Макросы --------------------------------------------

#define Alarm_On_Off(cfg,ALARM) 	g_Core.protections.alarmModes.##ALARM##.bit.off = cfg
#define Alarm_On(ALARM)			Alarm_On_Off(ON,ALARM)
#define Alarm_Off(ALARM)		Alarm_On_Off(OFF,ALARM)

#define IsAlarm_On_Off(cfg,ALARM) 	(g_Core.protections.alarmModes.##ALARM##.bit.off == cfg)
#define IsAlarm_On(ALARM) 		IsAlarm_On_Off(ON,ALARM)
#define IsAlarm_Off(ALARM) 		IsAlarm_On_Off(OFF,ALARM)

#define AlarmElem_On_Off(cfg,ELEM) 	g_Core.protections.##ELEM##.Cfg.bit.Enable = !cfg
#define AlarmElem_On(ALARM)		AlarmElem_On_Off(ON,ALARM)
#define AlarmElem_Off(ALARM)	AlarmElem_On_Off(OFF,ALARM)

#define IsAlarmElem_On_Off(cfg,ELEM) (g_Core.protections.##ELEM##.Cfg.bit.Enable == !cfg)
#define IsAlarmElem_On(ALARM)	IsAlarmElem_On_Off(ON,ALARM)
#define IsAlarmElem_Off(ALARM)	IsAlarmElem_On_Off(OFF,ALARM)

//-------------------- Структуры -------------------------------------------

// Структура настройки аварий
typedef union {
	Uns all;
	struct
	{
		Uns Enable	:1;		// 0	Разрешение работы (0-выкл, 1-вкл.)
		Uns CanBeReseted:1;	// 1	Режим работы (1-снимается, 0-не снимается)
		Uns Level	:1;		// 2	Активный уровень 0:Input <= Level, 1:Input >= Level
		Uns Num		:4;		// 3-6	Номер бита в регистре аварий
		Uns Hyst	:9;		// 7-15 Значение гистерезиса
	} bit;
} TAlarmCfg;
//--------------------------------------------------------
// Структура расчёта аварий
typedef struct {
	TAlarmCfg Cfg;			// Конфигурация защиты
	Int    *Input;			// Вход
	Uns    *Output;			// Выход
	Int    *EnableLevel;	// Уровень срабатывания аварии
	Int    *DisableLevel;	// Уровень снятия аварии
	Uns    *Timeout;		// Тайм-аут срабатывания
	Uns     Scale;			// Коэффициент масштабирования времени
	Uns   	Timer;			// Таймер срабатывания
} TAlarmElem;

//------------------- Глобальные переменные --------------------------------
//------------------- Протатипы функций ------------------------------------

void Core_ProtectionsAlarmInit(TAlarmElem *);
void Core_ProtectionsAlarmUpdate(TAlarmElem *);

#endif
