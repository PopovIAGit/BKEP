/*======================================================================
Имя файла:          g_Ram.h
Автор:
Версия файла:
Дата изменения:
Описание:
Описание главной структуры
========================================================================*/

#ifndef G_RAM_
#define G_RAM_

#include "config.h"				// Структуры
#include "g_Structs.h"			// Структуры
#include "peref_Clock.h"		// Библиотека для преобразований чисел/строк
#include "core.h"
//#include "core_Protections.h" 	//структура защит
//#include "comm_LocalControl.h"

// Команда управления
typedef enum {
  genNone=0,		// Нет команды
  genStop,		
  genOpen,	
  genClose,		
  genRzvd1,
  genRzvd2,
  genResetProtect,
  genOnDU,	
  genOnMU,		
  genInTest_ON,
  genInTest_OFF,
  genOutTest_ON,
  genOutTest_OFF	
} TCommand;

//-------------------------------------------------------------------------
typedef union _TTek_TechReg
{
	Uns all;
	struct
	{
		Uns Opened:1;		// 0
		Uns Closed:1;		// 1
		Uns Mufta1:1;		// 2
		Uns Mufta2:1;		// 3
		Uns Rsvd2:3;		// 4-6
		Uns MuDu:1;			// 7
		Uns Opening:1;		// 8
		Uns Closing:1;		// 9
		Uns Stop:1;			// 10
		Uns Rsvd3:2;		// 11-12
		Uns Ten:1;			// 13
		Uns Rsvd4:2;		// 14-15
	} bit;
} TTek_TechReg;

typedef union _TTek_DefReg
{
	Uns all;
	struct
	{
		Uns I2t:1;		// 0
		Uns ShC:1;		// 1
		Uns Drv_T:1;	// 2
		Uns Uv:1;		// 3
		Uns Phl:1;		// 4
		Uns NoMove:1;	// 5
		Uns Ov:1;		// 6
		Uns Bv:1;		// 7
		Uns Rsvd:1;		// 8
		Uns Th:1;		// 9
		Uns Tl:1;		// 10
		Uns Rsvd1:1;	// 11
		Uns PhOrdU:1;	// 12
		Uns PhOrdDrv:1;	// 13
		Uns DevDef:1;	// 14
		Uns Rsvd2:1;	// 15
	} bit;
} TTek_DefReg;

typedef union _TTek_ComReg
{
	Uns all;
	struct
	{
		Uns Stop:1;				// 0
		Uns Open:1;				// 1
		Uns Close:1;			// 2
		Uns Rsvd:2;				// 3-4
		Uns PrtReset:1;			// 5
		Uns Rsvd1:2;			// 6-7
		Uns EnDiscrInTest:1; 	// 8
		Uns DisDiscrInTest:1; 	// 9
		Uns EnDiscrOutTest:1;	// 10
		Uns DisDiscrOutTest:1; 	// 11
		Uns Rsvd2:4;			// 12-15
	} bit;
} TTek_ComReg;

typedef union _TTek_Discrete
{
	Uns all;
	struct
	{
		Uns InOpen:1;			// 0
		Uns InClose:1;			// 1
		Uns InStop:1;			// 2
		Uns InMu:1;				// 3
		Uns InDu:1;				// 4
		Uns OutOpened:1;		// 5
		Uns OutClosed:1;		// 6
		Uns OutMufta:1;			// 7
		Uns OutFault:1;			// 8
		Uns OutOpening:1;		// 9
		Uns OutClosing:1;		// 10
		Uns OutMuDu:1;			// 11
		Uns OutNeispr:1;		// 12
		Uns Rsvd:1;				// 13
		Uns IsDiscrOutActive:1;	// 14
		Uns IsDiscrInActive:1;	// 15
	} bit;
} TTek_Discrete;
//----------------------------------------------------*--------------------------

// ПАРАМЕТРЫ НАСТРОЙКИ
// Группа A (Адрес = 0, Количество = 40) - Диагностика - просмотр
typedef struct _TRamGroupA
{
	TStatusReg      Status;           	// 0.Статус работы
	TFltUnion	    Faults;				// 1-4.Аварии
	TInputReg       Inputs;           	// 5.Состояние дискретных входов
	TOutputReg      Outputs;          	// 6.Состояние дискретных выходов
	Uns             Position;        	// 7.Положение
	Uns             Torque;           	// 8.Момент
	Int             Speed;				// 9.Скорость
	Uns             Ur;                 // 10.Напряжение фазы R
  	Uns             Us;                 // 11.Напряжение фазы S
  	Uns             Ut;                 // 12.Напряжение фазы T
	Uns             Iu;                 // 13.Ток фазы U
	Uns             Iv;                 // 14.Ток фазы V
	Uns             Iw;                 // 15.Ток фазы W
	Int             AngleUI;            // 16.Угол нагрузки
	TCalibState     CalibState;         // 17.Статус калибровки
	Int             PositionPr;         // 18.Положение %
	Uns             FullWay;            // 19.Полный ход
	Int             CurWay;             // 20.Текущий ход
	Uns             CycleCnt;           // 21.Счетчик циклов
	Int             Temper;             // 22.Температура блока
	Uns             MkuPoVersion;       // 23.Версия ПО
	Uns Rsvd[16];
} TRamGroupA;

// Группа B (Адрес = 40, Количество = 50) - Параметры пользователя
typedef struct _TRamGroupB
{
	Uns				MoveCloseTorque;	// 0.Момент закрытия
	Uns				MoveOpenTorque;		// 1.Момент открытия
	Uns				BreakCloseTorque;	// 2.Момент уплотнения на закрытие
	Uns				StartCloseTorque; 	// 3.Момент трогания на закрытие
	Uns				BreakOpenTorque;  	// 4.Момент уплотнения на открытие
	Uns				StartOpenTorque;  	// 5.Момент трогания на открытие
	TValveType		ValveType;			// 6.Тип задвижки (клин/шибер)
	TBreakMode      BreakMode;		   	// 7.Тип уплотнения
	TRodType        RodType;            // 8.Тип штока
	Uns				CloseZone;		 	// 9.Зона закрыто
	Uns				OpenZone;		 	// 10.Зона открыто
	Uns             PositionAcc;        // 11.Зона смещения
	Uns             MainCode;        	// 12.Код доступа
	TTimeVar 		DevTime;			// 13.Время
	TDateVar 		DevDate;			// 14.Дата
	Int		 		TimeCorrection;		// 15.Корректеровка времени
	TIndicMode      IIndicMode;         // 16.Режим индикации тока
	TMuDuSetup      MuDuSetup;          // 17.Настройка режима МУ/ДУ
	TDuSource		DuSource;			// 18.Источник команд ДУ
	Uns             TuLockSeal;         // 19.Блокировка залипани
	Uns             TuTime;             // 20.Время команды
	#if BUR_M
	Uns 			Rsvd1[2];
	#else
	TInputType 		InputType;			// 21.Тип входного сигнала 24/220
	TInputMask	    InputMask;			// 22.Маска дискретных входов
	#endif
	TOutputMask 	OutputMask;			// 23.Маска дискретных выходов
	TBaudRate       RsBaudRate;         // 24.Скорость связи
	Uns             RsStation;          // 25.Адрес станции
	TParityMode		RsMode;				// 26.Режим связи
	TSettingPlace	SettingPlace;		// 27.Место установки БУР
	Uns 			Rsvd2[4];			// 28-31.
	Uns				NoMoveTime;		   	// 32.Время отсутствия движения
	Uns				OverwayZone;		// 33.Макси
	Uns 			Rsvd3;				// 34.
 	Uns             SleepTime;          // 35.Дежурный режим
	Uns 			Rsvd[14];
} TRamGroupB;

// Группа C (Адрес = 90, Количество = 120) - Заводские параметры
typedef struct _TRamGroupC
{
	Uns             FactCode;           // 0.Код доступа
	TDriveType      DriveType;          // 1.Тип привода
    Uns        		ProductYear;        // 2.Год изготовления блока
  	Uns             FactoryNumber;      // 3.Заводской номер блока
  	Uns             MaxTorque;          // 4.Максимальный момент привода
	Uns             Inom;               // 5.Номинальный ток
  	Uns             GearRatio;          // 6.Передаточное число редуктора
	Int             BlockToGearRatio;   // 7.Коэффициент передачи блок-редуктор коэффициент скорости
	Uns             PosPrecision;       // 8.Точность положения
	Uns             PhOrdZone;      	// 9.Расстояние чередования фаз двигателя
	Uns             MuffZone;           // 10.Расстояние сброса муфты
	Uns			    PosSensPow;			// 11.Тип датчика положения
	TPrtMode        CalibIndic;         // 12.Индикация калибровки
	Uns			    DisplResTout;		// 13.Время сброса индикатора
	// индикация
	Uns             OpenAngle;          // 14.Угол управления СИФУ
	Int             AngleUI;            // 15.Угол нагрузки
	TLedsReg        LedsReg;            // 16.Состояние светодиодов блока
	THallBlock      HallBlock;          // 17.Состояние датчиков холла блока
	Uns             SetDefaults;        // 18.Задание параметров по умолчанию
#if BUR_M
	TPrtMode       	RST_Err;           	// 19.Неверное чередование фаз сети для БУР М
#else
	Uns       	    Rsvd1;           	// 19.Резерв
#endif
	TPrtMode        DriveTemper;        // 20.Защита от перегрева двигателя. (дописать защиту перегрева блока в H)
	Uns             OvLevel_max;        // 21.Уровень превышения напряжения при 47% превышения (320В)
	Uns             OvTime_max;     	// 22.Время превышения напряжения при 47% превышения (1с)
	Uns             RevErrValue;       	// 23.Количество сбоев датчика положения
	Uns			    RevErrLevel;		// 24.Уровень сбоя датчика положения
	//-индикация конец--
	Uns             VoltAcc;            // 25.Интенсивность разгона
	Uns             BrakeAngle;         // 26.Угол торможения
	Uns             BrakeTime;          // 27.Время торможения
	Uns				KickCount;			// 28.Колличество ударов
	Uns             OvLevel;    		// 29.Уровень превышения напряжения
	Uns             OvTime;     		// 30.Время превышения напряжения
	Uns             UvLevel;   			// 31.Уровень понижения напряжения
	Uns             UvTime;    			// 32.Время понижения напряжения
	Uns             VSkLevel;          	// 33.Уровень асиметрии напряжения
	Uns             VSkTime;           	// 34.Время асиметрии напряжения
	Uns             BvLevel;   			// 35.Уровень обрыва входных фаз
	Uns             BvTime;    			// 36.Время обрыва входных фаз
	TPrtMode        Phl;           		// 37.Защита от обрыва фаз нагрузки
	Uns             PhlLevel;      		// 38.Уровень обрыва фаз
	Uns             PhlTime;       		// 39.Время определения обрыва фаз
	TPrtMode        I2t;                // 40.Время-токовая защита
	Uns             I2tMinCurrent;      // 41.Уровень минимального тока
	Uns             I2tMidCurrent;      // 42.Уровень среднего тока
	Uns             I2tMaxCurrent;      // 43.Уровень максимального тока
	Uns             I2tMinTime;         // 44.Время на минимальном токе
	Uns             I2tMidTime;         // 45.Время на среднем токе
	Uns             I2tMaxTime;         // 46.Время на максимальном токе
	Uns				PhOrdTime;			// 47.Время чередования фаз двигателя
	Uns				DrvTLevel;			// 48. Уровень срабатывания защиты по перегреву двигателя
	Uns				DrvTInput;			// 49.
	TPrtMode        Ov;					// 50.Защита от превышения напряжения
	TPrtMode        Uv;					// 51.Защита от превышения напряжения
	TPrtMode        VSk;               	// 52.Защита от асиметрии напряжения
	TPrtMode        Bv;        			// 53.Защита от обрыва входных фаз
	TPrtMode        PhOrd;          	// 54.Защита от неверного чередования фаз двигателя
	Uns             OvDTime;     		// 55.Время превышения напряжения
	Uns             UvDTime;     		// 56.Время превышения напряжения
	Uns             MkuPoSubVersion;    // 24.Подверсия ПО
	TPrtMode        IUnLoad;            // 58.Защита от малой нагрузки
	Uns             IUnLevel;           // 59.Ток малой нагрузки
	Uns             IUnTime;            // 60.Время малой нагрузки
	TPrtMode        ISkew;              // 61.Защита от асиметрии тока
	Uns             ISkewLevel;         // 62.Уровень асиметрии тока
	Uns             ISkewTime;          // 63.Время асиметрии тока
	TPrtMode        ShC;        		// 64.Защита от короткого замыкания
	Uns				Rsvd5[3];			// 65-67 Тут были уровни и задержка для старого КЗ
	TPrtMode        ErrIndic;           // 68.Индикация аварий устройства
	TPrtMode        TemperTrack;        // 69.Защита от перегрева/переохлаждения блока
	Int             TemperHigh;         // 70.Уровень перегрева блока
	Int             TemperLow;          // 71.Уровень переохлаждения блока
	Int             TenOnValue;         // 72.Уровень включения ТЕНа
	Int             TenOffValue;        // 73.Уровень выключения ТЕНа
	Int             CorrTemper;         // 74.Корректировка температуры блока
	TPrtMode        MuDuDef;        	// 75.Защита ошибки входов Му/Ду
	Int				IU_Input;			// 76.параметр для подстройки оффсетов тока IU
	Int				IV_Input;			// 77.параметр для подстройки оффсетов тока IV
	Int				IW_Input;			// 78.параметр для подстройки оффсетов тока IW
	Uns             IU_Mpy;             // 79.Корректировка тока фазы U
	Uns             IV_Mpy;             // 80.Корректировка тока фазы V
	Uns             IW_Mpy;             // 81.Корректировка тока фазы W
	Uns             IU_Offset;          // 82.Смещение тока фазы U
	Uns             IV_Offset;          // 83.Смещение тока фазы V
	Uns             IW_Offset;          // 84.Смещение тока фазы W
	Uns             UR_Mpy;             // 85.Корректировка напряжения фазы R
	Uns             US_Mpy;             // 86.Корректировка напряжения фазы S
	Uns             UT_Mpy;             // 87.Корректировка напряжения фазы T
	Uns             UR_Offset;          // 88.Смещение напряжения фазы R
	Uns             US_Offset;          // 89.Смещение напряжения фазы S
	Uns             UT_Offset;          // 90.Смещение напряжения фазы T
	Uns			    CoefVoltFltr;		// 91.Коэффициент фильтрации входного напряжения
	Uns			    CoefCurrFltr;		// 92.Коэффициент фильтрации тока нагрузки
	Uns			    Dac_Mpy;			// 93.Корректировка ЦАП
	Int			    Dac_Offset;			// 94.Смещение ЦАП
	Uns             ClosePosition;      // 95.Положение закрыто
	Uns             OpenPosition;       // 96.Положение открыто
	TTenMode		Rsvd6[3];			// 97 - 99 Резерв
	Uns				ShC_Up; 			// 100 убрать
	Uns				ShC_Down;			// 101 уброать в H
	Uns             Position;           // 102.Положение
	Uns				LowPowerLvl;		// 103 Значение напряжения в попугаях для 3х секунд
	Int				Upor25;				// 104 Значение для добавлени/убавления значения упора при 220в 25% от максМ
	Int				Upor35;				// 105 Значение для добавлени/убавления значения упора при 220в 35% от максМ
	Int				Upor50;				// 106 Значение для добавлени/убавления значения упора при 220в 50% от максМ
	Int				Upor75;				// 107 Значение для добавлени/убавления значения упора при 220в 75% от максМ
	Int				Upor100;			// 108 Значение для добавлени/убавления значения упора при 220в 100% от максМ
	Int				Corr40Trq;			// 109 Параметр для корректировки индикации малых моментов (меньше 60%)
	Int				Corr60Trq;			// 110 Параметр для корректировки индикации больших моментов (больше 60%)
	Int				Corr80Trq;			// 111
	Int				Corr110Trq;			// 112
	Uns			    Rsvd2[7];			// 113 - 119 Резерв
} TRamGroupC;

// Группа D (Адрес = 210, Количество = 20 )  	- Команды
typedef struct _TRamGroupD
{
	TTaskReset     TaskClose;           // 0.Задание закрыто
	TTaskReset     TaskOpen;            // 1.Задание открыто
	Uns            RevOpen;             // 2.Обороты на открытие
	Uns            RevClose;            // 3.Обороты на закрытие
	Uns            AutoCalib;           // 4.Автоматическая калибровка
	Uns            CalibReset;          // 5.Сброс калибровки
	TValveCmd	   ControlWord;         // 6.Команда управления
	Uns			   PrtReset;      		// 7.Сброс защит
	Uns            SetDefaults;         // 8.Задание параметров по умолчанию
	Uns 		   Rsvd1;				// 9.отчистка журнала событий
	Uns            CycleReset;			// 10.Сброс счетчика циклов
	Uns			   RsReset;				// 11. Сброс связи
	Uns 		   Rsvd[8];				// 12-19.Резерв
} TRamGroupD;


// Группа G (Адрес 230, Количество 30)  - Предположительно заводской тест
typedef struct _TRamGroupG
{
	Uns            Mode;                // 0.Режим теста
	TLedsReg       LedsReg;             // 1.Тест светодиодов блока
	TOutputReg     OutputReg;			// 2.Тест дискретных выходов
	Int			   DacValue;			// 3.Значение ЦАП
	Uns			   DisplShow;			// 4.Старт дисплея
	Uns			   TestCamera;			// 5.Тест с камеры
	Uns			   ThyrGroup;			// 6.Выбор группы тиристоров
	Uns            SifuEnable;          // 7.Разрешение работы СИФУ
	Uns			   Rsvd2[10];			// 8-17.Резерв
	Uns			   ThyrOpenAngle;		// 18.Тестовый угол открытия
	Uns			   DiscrInTest;			// 19. Тест дискретных входов
	Uns			   DiscrOutTest;		// 20. Тест дискретных выходов
	Bool		   IsDiscrTestActive;	// 21. Активен ли тест ТС/ТУ
	Uns 		   Rsvd[8];			    // 22-29.Резерв
} TRamGroupG;

// Группа H (Адрес 260, Количество 140)
typedef struct _TRamGroupH
{
	TCalibState     CalibState;          // 0.Состояние калибровки
	Uns             CalibRsvd;           // 1.Резерв для калибровки
	Uns             ClosePosition[2];    // 2-3.Положение закрыто
	Uns             OpenPosition[2];     // 4-5.Положение открыто
										 // Дальше адрес +2
	Uns             Password1;           // 6.Основной пароль
	Uns             Password2;           // 7.Заводской пароль
	Uns             ScFaults;            // 8.Аварии КЗ
	Uns				UporOnly;			 // 9.Параметр включающий работу только на упоре
	Uns             CycleCnt;            // 10.Счетчик циклов
	Uns				Rsvd1[5];			 // 11-15.Резерв
	Uns				TransCurr;			 // 16.Ток перехода
	TCubArray		TqCurr;				 // 17-36.Ток поверхности
	TCubArray		TqAngUI;			 // 37-56.Углы нагрузки
	TCubArray		TqAngSf;			 // 57-76.Углы СИФУ
	Uns				ZazorTime;			 // 77.Время выборки зазора
	TNetReg         FaultsNet;           // 78.Диагностика сети
  	Uns             Ur;                  // 79.Напряжение фазы R
  	Uns             Us;                  // 80.Напряжение фазы S
  	Uns             Ut;                  // 81.Напряжение фазы T
	Uns             Umid;             	 // 82.Среднее напряжение
	Uns             VSkValue;          	 // 83.Асиметрия фаз питающей сети
  	Uns     	    PhOrdValue;        	 // 84.Чередование фаз сети
	TNetReg         DefectsNet;          // 85.Диагностика сети (для неисправностей)
	Uns             Rsvd7;         	 	 // 86. Резерв
	TLoadReg        FaultsLoad;          // 87.Диагностика нагрузки
	Uns             Iu;                  // 88.Ток фазы U
	Uns             Iv;                  // 89.Ток фазы V
	Uns             Iw;                  // 90.Ток фазы W
	Uns             Imid;				 // 91.Средний ток
  	Uns             ISkewValue;          // 92.Асиметрия токов нагрузки
	Uns             Position;            // 93.Положение
	LgUns  			FullStep;        	 // 94.Полный ход
	Uns				Imidpr;			 	 // 95 Резерв
	TReverseType	ReverseType;         // 96.Тип реверса
	Uns				DemoPosition1;       // 97.Демо положение 1
	Uns				DemoPosition2;       // 98.Демо положение 2
	TInputReg       Inputs;           	 // 99.Состояние дискретных входов
	TOutputReg      Outputs;          	 // 100.Состояние дискретных выходов
	Uns             TuReleMode;          // 101.Релейный режим
	TNormState	    NormState[3];		 // 102-104.Нормальное состояние входов    - Не используется но если не заработают маски
	TNormState 	    NormOut[8];			 // 105-112.Нормальное состояние выходов - то пользуем это
	TDeviceReg      FaultsDev;           // 113.Диагностика устройства
	Uns             StartIndic;			 // 114.Индикация в старте
 	Uns             SleepTime;           // 115.Дежурный режим  mb to C
	Uns             BusyValue;       	 // 116.Процент исполнения
	Uns 			LogEvAddr;			 // 117. Текущий адрес журнала событий
	Uns 			LogCmdAddr;			 // 118. Текущий адрес журнала команд
	Uns 			LogParamAddr;		 // 119. Текущий адрес журнала изменения параметров
	Uns				LogEvCount;			 // 120. Количество записанных ячеек журнала событий
	Uns				LogCmdCount;		 // 121. Количество записанных ячеек журнала команд
	Uns				LogParamCount;		 // 122. Количество записанных ячеек журнала изменения параметров
	Uns             Torque;           	 // 123.Момент
	Int             Speed;				 // 124.Скорость
	Uns				Seconds;			 // 125.Секунды
	TContactorGroup ContGroup;			 // 126. Управление контакторами
	TBurCmd 		LogControlWord;		 // 127. Команды БУР
	Uns				LogReset;			 // 128. Сброс журналов
	Uns 			Rsvd3[11];			 // 129-139.Резерв
} TRamGroupH;

// Группа E (Адрес 400, Количество 32)
typedef struct _TRamGroupE
{
	TTimeVar       LogTime;				// T.Время
	TDateVar       LogDate;				// D.Дата
	TStatusReg     LogStatus;           // 0.Статус работы
	TFltUnion	   LogFaults;			// 1-4.Аварии
	Int            LogPositionPr;       // 5.Положение %
	Uns            LogTorque;           // 6.Момент
	Uns            LogUr;               // 7.Напряжение фазы R
  	Uns            LogUs;               // 8.Напряжение фазы S
  	Uns            LogUt;               // 9.Напряжение фазы T
	Uns            LogIu;               // 10.Ток фазы U
	Uns            LogIv;               // 11.Ток фазы V
	Uns            LogIw;               // 12.Ток фазы W
	Int            LogTemper;           // 13.Температура блока
	TInputReg      LogInputs;           // 14.Состояние дискретных входов
	TOutputReg     LogOutputs;          // 15.Состояние дискретных выходов
	Uns 		   Rsvd[14];			// 16-29.Резерв
} TRamGroupE;


typedef struct _TRamLog
{
	TStatusReg     	LogStatus;          // 0.Статус работы
	Int            	LogPositionPr;		// 1.Положение %
	Uns            	LogTorque;          // 2.Момент
	Uns            	LogUr;              // 3.Напряжение фазы R
  	Uns            	LogUs;              // 4.Напряжение фазы S
  	Uns            	LogUt;              // 5.Напряжение фазы T
	Uns            	LogIu;              // 6.Ток фазы U
	Uns            	LogIv;              // 7.Ток фазы V
	Uns            	LogIw;              // 8.Ток фазы W
	Int            	LogTemper;          // 9.Температура блока
	Uns      		LogInputs;        	// 10.Состояние дискретных входов
	Uns				LogOutputs;			// 11.Состояние дискретных выходов
} TRamLog;



// ОБЩИЕ ПАРАМЕТРЫ
// начальный адрес 40001
typedef struct _TTek_Mb_Group
{
	Uns 			Rsvd;				// 0 Резерв
	TTek_TechReg 	TechReg;			// 1 Технологический регистр
	TTek_DefReg  	DefReg;				// 2 Регистр дефектов
	Uns 		 	PositionPr;			// 3 Текущее положение %
	TTek_ComReg  	ComReg;				// 4 Регистр команд
	Uns 		 	CycleCnt;			// 5 Счетчик циклов
	Uns			 	Rsvd1;				// 6 Резерв
	Uns 		 	Iu;					// 7 Ток фазы U
	Uns 		 	Rsvd2[9];			// 8 - 16 Резерв
	Uns 			Ur;					// 17 Напряжение входной сети
	Uns				Rsvd3;				// 18 Резерв
	Uns 		 	Speed;				// 19 Текущая скорость
	Uns			 	Rsvd4;				// 20 Резерв
	Uns 		 	Torque;				// 21 Текущий момент нагрузки Н*м
	TTek_Discrete 	TsTu;				// 22 ТС/ТУ
	Uns				Rsvd6[4];			// 23 - 26 Резерв
	Uns				RsStation;			// 27 Адрес станции (только для чтения)
	Uns				Rsvd7;				// 28 Резерв
} TTek_Mb_Group;


// Структура параметров устройства
typedef struct TRam
{
  TRamGroupA		ramGroupA;			// ПАРАМЕТРЫ НАСТРОЙКИ
  TRamGroupB		ramGroupB;
  TRamGroupC		ramGroupC;
  TRamGroupD		ramGroupD;
  TRamGroupG		ramGroupG;
  TRamGroupH		ramGroupH;
  TRamGroupE		ramGroupE;
  TRamLog			ramLog;
} TRam;

// Структура параметров устройства
typedef struct _TTEKDriveData
{
	TTek_Mb_Group	tekGroup;
} TTekDriveData;


#define RAM_ADR				0
#define RAM_SIZE			sizeof(TRam)
#define RAM_SET_MAX			50
#define RAM_DATA_SIZE		(RAM_SIZE)
#define RAM_DATA_SIZE_GRB   sizeof(TRam_groupB)
#define RAM_EADR			(RAM_DATA_SIZE - 1)

#define DLOG_ADR			0x0500
#define DLOG_SIZE			0x0200
#define DLOG_DATA_ADR1		0x3FA000
#define DLOG_DATA_ADR2		0x3FB000
#define DLOG_EADR			(DLOG_ADR + (DLOG_SIZE<<1) - 1)

#define IR_ADR				0x0485
#define IR_EADR				0x0489

#define GetAdr(Elem)		((LgUns)&(((TRam *)RAM_ADR)->Elem))

//-------- НАСТРОЙКИ ГРУПП					
#define REG_CODE			GetAdr(ramGroupA.PASSWORD1)
/*
#define GLOBAL_REG_GR		0
#define GLOBAL_COUNT_GR		1

#define REG_GR				GetAdr(ramGroup.STATUS)
#define COUNT_GR			30
*/
/*
#define REG_TASK_TIME		GetAdr(ramGroupD.TASK_TIME)
#define REG_TASK_DATE		GetAdr(ramGroupD.TASK_DATE)
//-------- СКРЫТЫЕ ПАРАМЕТРЫ
#define REG_PASSW1_NEW		GetAdr(ramGroupF.PASSW1)
#define REG_PASSW2_NEW		GetAdr(ramGroupF.PASSW2)
#define REG_REV_ERR_VALUE	GetAdr(ramGroupF.REV_ERR_VALUE)
#define REG_LAST_MIN		GetAdr(ramGroupF.LAST_MIN)
#define REG_LAST_SEC		GetAdr(ramGroupF.LAST_SEC)
#define REG_LAST_STATUS		GetAdr(ramGroupF.LAST_STATUS)
#define REG_CALIB_CLOSE		GetAdr(ramGroupF.CLB_CLOSE)
#define REG_CALIB_OPEN		REG_CALIB_CLOSE+2
#define REG_CALIB_STATUS	GetAdr(ramGroupF.CLB_STATUS)
//-------- ФИЛЬТРЫ
#define REG_SIN_FILTER_TF	GetAdr(ramGroupC.SIN_TF)
#define REG_RMS_FILTER_TF	GetAdr(ramGroupC.RMS_TF)
#define REG_TEMP_FILTER_TF	GetAdr(ramGroupC.TEMP_TF)
#define REG_ISOL_FILTER_TF	GetAdr(ramGroupC.ISO_TF)
//-------- ЗАЩИТЫ
#define REG_INOM			GetAdr(ramGroupC.INOM)
#define REG_OVER_VOL_TIME	GetAdr(ramGroupC.OVER_VOL_TIME)
#define REG_UNDER_VOLT_TIME	GetAdr(ramGroupC.UNDER_VOLT_TIME)
#define REG_OVER_VOL_TIME_SHTDWN	GetAdr(ramGroupC.OVER_VOL_TIME_SHTDWN)
#define REG_UNDER_VOLT_TIME_SHTDWN	GetAdr(ramGroupC.UNDER_VOLT_TIME_SHTDWN)
#define REG_VSK_TIME		GetAdr(ramGroupC.VSK_TIME)
#define REG_BV_TIME			GetAdr(ramGroupC.BV_TIME)
#define REG_TEST_ISO_TIME	GetAdr(ramGroupC.TEST_ISO_TIME)
#define REG_OVER_CURRENT_TIME	GetAdr(ramGroupC.OVER_CURRENT_TIME)
#define REG_TEMP_MOTOR_TIME	GetAdr(ramGroupC.TEMP_MOTOR_TIME)
#define REG_PHL_TIME		GetAdr(ramGroupC.PHL_TIME)
#define REG_ISKEW_TIME		GetAdr(ramGroupC.ISKEW_TIME)
#define REG_TEMP_HIGH_TIME	GetAdr(ramGroupC.TEMP_HIGH_TIME)
#define REG_TEMP_LOW_TIME	GetAdr(ramGroupC.TEMP_LOW_TIME)
#define REG_CHANGE_RST		GetAdr(ramGroupC.CHANGE_RST)
#define REG_CHANGE_UVW_MOVE		GetAdr(ramGroupC.CHANGE_UVW_MOVE)
#define REG_CHANGE_UVW_STOP		GetAdr(ramGroupC.CHANGE_UVW_STOP)
#define REG_TEST_MOTOR_SC_TIME  GetAdr(ramGroupC.TEST_MOTOR_SC_TIME)

#define REG_TEMP_MOTOR_ENABLE_LEVEL		GetAdr(ramGroupC.TEMP_MOTOR_ENABLE_LEVEL)
#define REG_TEMP_MOTOR_DISABLE_LEVEL  	GetAdr(ramGroupC.TEMP_MOTOR_DISABLE_LEVEL)

#define REG_GEAR_RATIO		GetAdr(ramGroupC.GEAR_RATIO)
#define REG_FACTORY_NUMBER	GetAdr(ramGroupC.FACTORY_NUMBER)
#define REG_PRODUCT_DATE	GetAdr(ramGroupC.PRODUCT_DATE)

#define REG_TIME_TU			GetAdr(ramGroupB.TIME_TU)
#define REG_INDICATOR_TYPE	GetAdr(ramGroupB.INDICATOR_TYPE)
#define REG_LOCK_CONTROL	GetAdr(ramGroupB.LOCK_CONTROL)
#define REG_TEMP_SENS_TYPE	GetAdr(ramGroupB.TEMP_SENS_TYPE)
#define REG_LEVEL_DYN_BRAKE_BIG	GetAdr(ramGroupB.LEVEL_DYN_BRAKE_BIG)
*/
#define REG_RS_BAUD_RATE	GetAdr(ramGroup.RS_BAUD_RATE)

// Глобальные переменные модуля
#define REG_VER_PO			GetAdr(ramGroupA.VER_PO)
#define REG_SUBVER_PO		GetAdr(ramGroupA.SUBVER_PO)

#define IsImpulseMode()		(!g_Ram.ramGroupA.DIGITAL_CMD_MODE)
#define IsPotentialMode()	(g_Ram.ramGroupA.DIGITAL_CMD_MODE)

// Коды блокировки
#define LOCK_MPU			BIT0	// Блокировка МПУ
#define LOCK_TU				BIT1	// Блокировка ТУ
#define LOCK_RS485			BIT2	// Блокировка RS485
#define IsLockMPU() 		(g_Ram.ramGroupA.LOCK_CONTROL & LOCK_MPU)
#define IsLockTU()			(g_Ram.ramGroupA.LOCK_CONTROL & LOCK_TU)

void g_Ram_Init(TRam *);
void g_Ram_Update(TRam *);
void RefreshParams(Uns);
Int MinMax3IntValue (Int, Int, Int);
Int Max3Int (Int , Int , Int );
void g_Ram_SetTorque(void);

extern TRam 			g_Ram;
extern TTekDriveData	g_RamTek;


#endif




