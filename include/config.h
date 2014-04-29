/*======================================================================
Имя файла:          config.h
Автор:
Версия файла:
Дата изменения:
Применяемость:      TMS320F28022
Описание:
Основной модуль конфигурации проекта системного ПО блока БКД
======================================================================*/

#ifndef CONFIG_
#define CONFIG_

// Глобальные настройки
#define VERSION				0		// Версия проекта
#define SUBVERSION			3		// Подверсия проекта

#define SYSCLK				150e6	// Системная частота (кол-во операций в сек)
//#define CLKIN				30e6	// Частота кварца
#define CLKIN				30e6	// Частота кварца

// Конфигурация операционной системы
#define Prd18kHZ			18000	// Частота основного прерывания RTOS
#define Prd2kHZ				2000	// Периодическая задача 2000 Гц
#define Prd200HZ			200		// Периодическая задача 200 Гц
#define Prd50HZ				50		// Периодическая задача 50 Гц
#define Prd10HZ				10		// Периодическая задача 10 Гц

// Тестовые режимы
#define TORQ_TEST			0		// Тест расчета момента
#define LOWPOW_TEST			0		// Тест выключение при провале напряжения

// Делитель основных частот
//#define HSPCLK_PRESC		1			// Делитель частоты HSPCLK
//#define LSPCLK_PRESC		0			// Делитель частоты LSPCLK

#define HSPCLK_DIV			1
#define LSPCLK_DIV			2

#define F_HSPCLK			(SYSCLK / HSPCLK_DIV)
#define F_LSPCLK			(SYSCLK / LSPCLK_DIV)

// Макрос статической проверки, который проверяет истинность выражения "expr"
// если выражение ложно, макрос выводит сообщение об ошибке во время компиляции
#define CONCAT2(First, Second) (First ## Second)
#define CONCAT(First, Second) CONCAT2(First, Second)
#define STATIC_ASSERT(expr) typedef char CONCAT(static_assert_failed_at_line_, __LINE__) [(expr) ? 1 : -1]

// Выбор физики для передачи данных
#define UART_TYPE			0
#define MCBSP_TYPE			1


// Подключение заголовочных файлов
#include "csl\csl_spi.h"
#include "csl\csl_sci.h"
#include "csl\csl_mcbsp.h"
#include "chip\DSP2833x_Device.h"	// Заголовные файлы переферии чипа
#include "std.h"					// Библиотека поддержки чипа
#include "IQmathLib.h"
#include "g_InitHardware.h"

#endif

