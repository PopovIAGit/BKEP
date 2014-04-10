/*======================================================================
Имя файла:          g_InterruptUpdate.с
Автор:
Версия файла:
Дата изменения:
Описанте: 
	В этом файле содержится функция обработки прерываний. Эта функция
должна вызываться с частотой 18 кГц, тогда она симулирует выполнение прерываний
с частотой 2 кГц, 200 Гц, 50 Гц, и 10 Гц. Эта функция основана на алгоритме
из файла "Алгоритм работы прерываний 3.1.vsd".
	Идея алгоритма следущая: организовать прерывания так, чтобы на каждом такте 
в 18 кГц выполнялось фиксированное количество задач. Например, на каждом такте 
в 18 кГц будут выполняться задачи группы 18 кГц + одна задача из частотных 
групп 2 кГц, 200 Гц, 50 Гц и 10 Гц. 
	Так мы сможем распределить нагрузку процессора равномерно.
======================================================================*/
#include "g_InterruptUpdate.h"

// Статическая проверка на соответствие условиям алгоритма
// если количество заданных задач для конкретной частоты
// превышает максимально возможное количество, то выводится
// сообщение об ошибке при компиляции
STATIC_ASSERT(TaskCount(Task2kHz) < TASK_COUNT_MAX_2kHZ);
STATIC_ASSERT(TaskCount(Task200Hz)< TASK_COUNT_MAX_200HZ);
STATIC_ASSERT(TaskCount(Task50Hz) < TASK_COUNT_MAX_50HZ);
STATIC_ASSERT(TaskCount(Task10Hz) < TASK_COUNT_MAX_10HZ);
 


//---------ФУНКЦИЯ ИНИЦИАЛИЗАЦИИ ПРЕРЫВАНИЙ----------------------------------
void InterruptInit    (void)  
{
    //Обнуление счетчиков прерываний
    count18kHz = 0;
    count2kHz  = 0;
    count200Hz = 0;
    count50Hz  = 0;
    //Обнуление счетчиков выполненых задач
    tskCnt2kHz  = 0;
    tskCnt200Hz = 0;
    tskCnt50Hz  = 0;
    tskCnt10Hz  = 0;
    //Определение количества задач для частот 2кГц, 200Гц, 50Гц, 10Гц
    tskMaxCnt18kHz = TaskCount(Task18kHz);
    tskMaxCnt2kHz  = TaskCount(Task2kHz);
    tskMaxCnt200Hz = TaskCount(Task200Hz);
    tskMaxCnt50Hz  = TaskCount(Task50Hz);
    tskMaxCnt10Hz  = TaskCount(Task10Hz);
    //Обнуление вспомогательных переменных УДАЛИТЬ (для отладки)
    X   = 0;
    X10  = 0;
    X18 = 0;
}

//---------ФУНКЦИЯ ОБРАБОТКИ ПРЕРЫВАНИЙ--------------------------------------
void InterruptUpdate(void)  //Функция обработки прерываний
{
	int i;
	for (i=tskMaxCnt18kHz; i!=0; i--)
		Task18kHz[i-1].Func(Task18kHz[i-1].Data);
    count18kHz++;
    if (tskCnt2kHz<tskMaxCnt2kHz) // Есть ли невыполненые функции для частоты 2 кГц?
    {
        Task2kHz[tskCnt2kHz].Func(Task2kHz[tskCnt2kHz].Data);
        tskCnt2kHz++;           
    }
    else if (tskCnt200Hz<tskMaxCnt200Hz) // Есть ли невыполненые функции для частоты 200 Гц?
    {
        Task200Hz[tskCnt200Hz].Func(Task200Hz[tskCnt200Hz].Data);
        tskCnt200Hz++;
    }
    else  if (tskCnt50Hz<tskMaxCnt50Hz) // Есть ли невыполненые функции для частоты 50 Гц?
    {
        Task50Hz[tskCnt50Hz].Func(Task50Hz[tskCnt50Hz].Data);
        tskCnt50Hz++;
    }
    else if (tskCnt10Hz<tskMaxCnt10Hz) // Есть ли невыполненые функции для частоты 10 Гц?
    {
        Task10Hz[tskCnt10Hz].Func(Task10Hz[tskCnt10Hz].Data);
        tskCnt10Hz++;
    }

    if (count18kHz == 9)
    {
        count18kHz = 0;
        count2kHz++;
        tskCnt2kHz = 0;
        if (count2kHz == 10)
        {
            count2kHz = 0;
            count200Hz++;
            tskCnt200Hz = 0;
            if (count200Hz == 4)
            {
                count200Hz=0;
                count50Hz++;
                tskCnt50Hz = 0;
                if (count50Hz == 5)
                {
                    count50Hz = 0;
                    tskCnt10Hz = 0;
                }
            }
        }
    }
}
//-----------ОПИСАНИЕ  ФУНКЦИЙ-----------------------------------------------
void task1(void) // Для отладки (УБРАТЬ при необходимости)
{
	X += 1;
}

void task1_18Hz(void) // Для отладки (УБРАТЬ при необходимости)
{

}

void task1_10Hz(void) // Мигание светодиода
{
	if (++X10 > 10)
	{
		//OUT_TOGGLE(GpioDataRegs.GPADAT.bit.GPIO7);
		GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;
		GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
		X10 = 0;
	}
}
//------------Конец файла-----------------------------------------------

