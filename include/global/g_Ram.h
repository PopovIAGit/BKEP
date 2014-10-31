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
#include "comm.h"
#include "add_ValueConv.h"
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
// Группа A (Адрес = 500, Количество = 40) - Диагностика - просмотр
typedef struct _TRamGroupA
{
	TStatusReg      Status;           	// 0. 500 Статус работы
	TFltUnion	    Faults;				// 1-4. 501-504 Аварии
	TInputReg       StateTu;     	    // 5. 505 Состояние дискретных входов
	TOutputReg      StateTs;		    // 6. 506 Состояние дискретных выходов
	Uns             Position;        	// 7. 507 Положение
	Uns             Torque;           	// 8. 508 Момент
	Int             Speed;				// 9. 509 Скорость
	Uns             Ur;                 // 10. 510 Напряжение фазы R
  	Uns             Us;                 // 11. 511 Напряжение фазы S
  	Uns             Ut;                 // 12. 512 Напряжение фазы T
	Uns             Iu;                 // 13. 513 Ток фазы U
	Uns             Iv;                 // 14. 514 Ток фазы V
	Uns             Iw;                 // 15. 515 Ток фазы W
	Int             AngleUI;            // 16. 516 Угол нагрузки
	TCalibState     CalibState;         // 17. 517 Статус калибровки
	Int             PositionPr;         // 18. 518 Положение %
	Uns             FullWay;            // 19. 519 Полный ход
	Int             CurWay;             // 20. 520 Текущий ход
	Uns             CycleCnt;           // 21. 521 Счетчик циклов
	Int             Temper;             // 22. 522 Температура блока
	Int             TemperBKP;          // 23. 523 Температура блока БКП
	Uns             VersionPO;       	// 24. 524 Версия ПО
	Uns             VersionPOBkp;      	// 25. 525 Версия ПО БКП
	Uns				RevErrValue;		// 26. 526 Колво сбоев датчика положения
	Uns 			Rsvd[13];			// 27-39. 527-539 Резерв
} TRamGroupA;

// Группа B (Адрес = 40, Количество = 100) - Параметры пользователя
typedef struct _TRamGroupB
{
	Uns				MoveCloseTorque;	// B0. 40.Момент закрытия
	Uns				MoveOpenTorque;		// B1. 41 Момент открытия
	Uns				BreakCloseTorque;	// B2. 42 Момент уплотнения на закрытие
	Uns				StartCloseTorque; 	// B3. 43 Момент трогания на закрытие
	Uns				BreakOpenTorque;  	// B4. 44 Момент уплотнения на открытие
	Uns				StartOpenTorque;  	// B5. 45 Момент трогания на открытие
	TValveType		ValveType;			// B6. 46 Тип задвижки (клин/шибер)
	TBreakMode      BreakMode;		   	// B7. 47 Тип уплотнения
	TRodType        RodType;            // B8. 48 .Тип штока
	Uns				CloseZone;		 	// B9. 49 Зона закрыто
	Uns				OpenZone;		 	// B10. 50 Зона открыто
	Uns             PositionAcc;        // B11. 51 Зона смещения
	Uns             MainCode;        	// B12. 52 Код доступа
	TTimeVar 		DevTime;			// B13. 53 Время
	TDateVar 		DevDate;			// B14. 54 Дата
	Int		 		TimeCorrection;		// B15. 55 Корректеровка времени
	TIndicMode      IIndicMode;         // B16. 56 Режим индикации тока
	TMuDuSetup      MuDuSetup;          // B17. 57 Настройка режима МУ/ДУ
	TDuSource		DuSource;			// B18. 58 Источник команд ДУ
	Uns             TuLockSeal;         // B19. 59 Блокировка залипани
	Uns             TuTime;             // B20. 60 Время команды
	TInputType 		InputType;			// B21. 61 Тип входного сигнала 24/220
	TUInvert	    TuInvert;			// B22. 62 Маска дискретных входов
	TSInvert 		TsInvert;			// B23. 63 Маска дискретных выходов
	TBaudRate       RsBaudRate;         // B24. 64 Скорость связи
	Uns             RsStation;          // B25. 65 Адрес станции
	TParityMode		RsMode;				// B26. 66 Режим связи
	Uns				Rsvd4;				// B27. 67
	Uns				KeyInvert;			// B28. 68 Маска кнопок управления
	Uns				MOD_FAULT;			// B29. 69
	Uns				RES_ERR;			// B30. 70
	Uns 				Sec3Mode;		// B31. 71
	Uns				NoMoveTime;		   	// B32. 72 Время отсутствия движения
	Uns				OverwayZone;		// B33. 73 Макси
	TInputReg		DigitalMode;		// B34. 74 режим потенциальный / импульсный
 	Uns             SleepTime;          // B35. 75 Дежурный режим
 	Uns 			Rsvd[4];			// B36-39. 76-79
 	//------Параметры для ТУ------------------------------------
	Uns				LevelOnOpen220;		 // B40. 80
	Uns				LevelOffOpen220;	 // B41. 81
	Uns				LevelOnOpen24;		 // B42. 82
	Uns				LevelOffOpen24;		 // B43. 83
	Uns				UOpen_Mpy220;   	 // B44. 84
	Uns				UOpen_Mpy24;   		 // B45. 85
	Uns				p_UOpen_Offset;		 // B46. 86
	//-------------------------------
	Uns				LevelOnClose220;	 // B47. 87
	Uns				LevelOffClose220;	 // B48. 88
	Uns				LevelOnClose24;		 // B49. 89
	Uns				LevelOffClose24;	 // B50. 90
	Uns				p_UClose_Mpy220;	 // B51. 91
	Uns				p_UClose_Mpy24;		 // B52. 92
	Uns				p_UClose_Offset;	 // B53. 93
	//-------------------------------
	Uns				LevelOnStop220;		 // B54. 94
	Uns				LevelOffStop220;	 // B55. 95
	Uns				LevelOnStop24;		 // B56. 96
	Uns				LevelOffStop24;		 // B57. 97
	Uns				p_UStop_Mpy220;		 // B58. 98
	Uns				p_UStop_Mpy24;		 // B59. 99
	Uns				p_UStop_Offset;		 // B60. 100
	//-------------------------------
	Uns				LevelOnMU220;		 // B61. 101
	Uns				LevelOffMU220;		 // B62. 102
	Uns				LevelOnMU24;		 // B63. 103
	Uns				LevelOffMU24;		 // B64. 104
	Uns				p_UMu_Mpy220;		 // B65. 105
	Uns				p_UMu_Mpy24;		 // B66. 106
	Uns				p_UMu_Offset;		 // B67. 107
	//-------------------------------
	Uns				LevelOnResetAlarm220;	 // B68. 108
	Uns				LevelOffResetAlarm220;	 // B69. 109
	Uns				LevelOnResetAlarm24; 	 // B70. 110
	Uns				LevelOffResetAlarm24;	 // B71. 111
	Uns				p_UResetAlarm_Mpy220;	 // B72. 112
	Uns				p_UResetAlarm_Mpy24;	 // B73. 113
	Uns				p_UResetAlarm_Offset;	 // B74. 114
	//-------------------------------
	Uns				LevelOnReadyTU220;	 // B75. 115
	Uns				LevelOffReadyTU220;	 // B76. 116
	Uns				LevelOnReadyTU24;	 // B77. 117
	Uns				LevelOffReadyTU24;	 // B78. 118
	Uns				p_UReadyTu_Mpy220;	 // B79. 119
	Uns				p_UReadyTu_Mpy24;	 // B80. 120
	Uns				p_UReadyTu_Offset;	 // B81. 121
	//-------------------------------
	Uns				LevelOnDU220;		 // B82. 122
	Uns				LevelOffDU220;		 // B83. 123
	Uns				LevelOnDU24;		 // B84. 124
	Uns				LevelOffDU24;		 // B85. 125
	Uns				p_UDu_Mpy220;		 // B86. 126
	Uns				p_UDu_Mpy24;		 // B87. 127
	Uns				p_UDu_Offset;		 // B88. 128
	//-------------------------------
	Uns 			Rsvd3[11];//B89-99 129-139
} TRamGroupB;

// Группа C (Адрес = 140, Количество = 120) - Заводские параметры
typedef struct _TRamGroupC
{
	Uns             FactCode;           // C0. 140 Код доступа
	TDriveType      DriveType;          // C1. 141 Тип привода
    Uns        		ProductYear;        // C2. 142 Год изготовления блока
  	Uns             FactoryNumber;      // C3. 143 Заводской номер блока
  	Uns             MaxTorque;          // C4. 144 Максимальный момент привода
	Uns             Inom;               // C5. 145 Номинальный ток
  	Uns             GearRatio;          // C6. 146 Передаточное число редуктора
	Int             BlockToGearRatio;   // C7. 147 Коэффициент передачи блок-редуктор коэффициент скорости
	Uns             PosPrecision;       // C8. 148 Точность положения
	Uns             PhOrdZone;      	// C9. 149 Расстояние чередования фаз двигателя
	Uns             MuffZone;           // C10. 150 Расстояние сброса муфты
	Uns			    PosSensPow;			// C11. 151 Тип датчика положения
	TPrtMode        CalibIndic;         // C12. 152 Индикация калибровки
	Uns			    DisplResTout;		// C13. 153 Время сброса индикатора
	// индикация
	Uns             OpenAngle;          // C14. 154 Угол управления СИФУ
	Int             AngleUI;            // C15. 155 Угол нагрузки
	TLedsReg        LedsReg;            // C16. 156 Состояние светодиодов блока
	THallBlock      HallBlock;          // C17. 157 Состояние датчиков холла блока
	Uns             SetDefaults;        // C18. 158 Задание параметров по умолчанию
	Uns       	PlugBrakeDisable;    	// C19. 159 Запрещение торможения противовключением
	TPrtMode        DriveTemper;        // C20. 160 Защита от перегрева двигателя. (дописать защиту перегрева блока в H)
	Uns             OvLevel_max;        // C21. 161 Уровень превышения напряжения при 47% превышения (320В)
	Uns             OvTime_max;     	// C22. 162 Время превышения напряжения при 47% превышения (1с)
	Uns             RevErrValue;       	// C23. 163 Количество сбоев датчика положения
	Uns			    RevErrLevel;		// C24. 164 Уровень сбоя датчика положения
	//-индикация конец--
	Uns             VoltAcc;            // C25. 165 Интенсивность разгона
	Uns             BrakePause;         // C26. 166 Пауза перед торможением
	Uns             BrakeTime;          // C27. 167 Время торможения
	Uns				KickCount;			// C28. 168 Колличество ударов
	Uns             OvLevel;    		// C29. 169 Уровень превышения напряжения
	Uns             OvTime;     		// C30. 170 Время превышения напряжения
	Uns             UvLevel;   			// C31. 171 Уровень понижения напряжения
	Uns             UvTime;    			// C32. 172 Время понижения напряжения
	Uns             VSkLevel;          	// C33. 173 Уровень асиметрии напряжения
	Uns             VSkTime;           	// C34. 174 Время асиметрии напряжения
	Uns             BvLevel;   			// C35. 175 Уровень обрыва входных фаз
	Uns             BvTime;    			// C36. 176 Время обрыва входных фаз
	TPrtMode        Phl;           		// C37. 177 Защита от обрыва фаз нагрузки
	Uns             PhlLevel;      		// C38. 178 Уровень обрыва фаз
	Uns             PhlTime;       		// C39. 179 Время определения обрыва фаз
	TPrtMode        I2t;                // C40. 180 Время-токовая защита
	Uns             I2tMinCurrent;      // C41. 181 Уровень минимального тока
	Uns             I2tMidCurrent;      // C42. 182 Уровень среднего тока
	Uns             I2tMaxCurrent;      // C43. 183 Уровень максимального тока
	Uns             I2tMinTime;         // C44. 184 Время на минимальном токе
	Uns             I2tMidTime;         // C45. 185 Время на среднем токе
	Uns             I2tMaxTime;         // C46. 186 Время на максимальном токе
	Uns				PhOrdTime;			// C47. 187 Время чередования фаз двигателя
	Uns				DrvTLevel;			// C48. 188 Уровень срабатывания защиты по перегреву двигателя
	Uns				DrvTInput;			// C49. 189
	TPrtMode        Ov;					// C50. 190 Защита от превышения напряжения
	TPrtMode        Uv;					// C51. 191 Защита от превышения напряжения
	TPrtMode        VSk;               	// C52. 192 Защита от асиметрии напряжения
	TPrtMode        Bv;        			// C53. 193 Защита от обрыва входных фаз
	TPrtMode        PhOrd;          	// C54. 194 Защита от неверного чередования фаз двигателя
	Uns             OvDTime;     		// C55. 195 Время превышения напряжения
	Uns             UvDTime;     		// C56. 196 Время превышения напряжения
	Uns             SubVersionPO;	    // C57. 197 Подверсия ПО
	TPrtMode        IUnLoad;            // C58. 198 Защита от малой нагрузки
	Uns             IUnLevel;           // C59. 199 Ток малой нагрузки
	Uns             IUnTime;            // C60. 200 Время малой нагрузки
	TPrtMode        ISkew;              // C61. 201 Защита от асиметрии тока
	Uns             ISkewLevel;         // C62. 202 Уровень асиметрии тока
	Uns             ISkewTime;          // C63. 203 Время асиметрии тока
	TPrtMode        ShC;        		// C64. 204 Защита от короткого замыкания
	Uns				Rsvd5[3];			// C65-67 205-207 Тут были уровни и задержка для старого КЗ
	TPrtMode        ErrIndic;           // C68. 208 Индикация аварий устройства
	TPrtMode        TemperTrack;        // C69. 209 Защита от перегрева/переохлаждения блока
	Int             TemperHigh;         // C70. 210 Уровень перегрева блока
	Int             TemperLow;          // C71. 211 Уровень переохлаждения блока
	Int             TenOnValue;         // C72. 212 Уровень включения ТЕНа
	Int             TenOffValue;        // C73. 213 Уровень выключения ТЕНа
	Int             CorrTemper;         // C74. 214 Корректировка температуры блока БКП
	TPrtMode        MuDuDef;        	// C75. 215 Защита ошибки входов Му/Ду
	Int				IU_Input;			// C76. 216 параметр для подстройки оффсетов тока IU
	Int				IV_Input;			// C77. 217 параметр для подстройки оффсетов тока IV
	Int				IW_Input;			// C78. 218 параметр для подстройки оффсетов тока IW
	Uns             IU_Mpy;             // C79. 219 Корректировка тока фазы U
	Uns             IV_Mpy;             // C80. 220 Корректировка тока фазы V
	Uns             IW_Mpy;             // C81. 221 Корректировка тока фазы W
	Uns             IU_Offset;          // C82. 222 Смещение тока фазы U
	Uns             IV_Offset;          // C83. 223 Смещение тока фазы V
	Uns             IW_Offset;          // C84. 224 Смещение тока фазы W
	Uns             UR_Mpy;             // C85. 225 Корректировка напряжения фазы R
	Uns             US_Mpy;             // C86. 226 Корректировка напряжения фазы S
	Uns             UT_Mpy;             // C87. 227 Корректировка напряжения фазы T
	Uns             UR_Offset;          // C88. 228 Смещение напряжения фазы R
	Uns             US_Offset;          // C89. 229 Смещение напряжения фазы S
	Uns             UT_Offset;          // C90. 230 Смещение напряжения фазы T
	Uns			    SinTf;				// C91. 231 ПОСТ.ВРЕМЕНИ ФИЛЬТРОВ СИНУСОВ
	Uns			    RmsTf;				// C92. 232 ПОСТ.ВРЕМЕНИ ФИЛЬТРОВ RMS
	Uns			    Dac_Mpy;			// C93. 233 Корректировка ЦАП
	Int			    Dac_Offset;			// C94. 234 Смещение ЦАП
	Uns             ClosePosition;      // C95. 235 Положение закрыто
	Uns             OpenPosition;       // C96. 236 Положение открыто
	TTenMode		Rsvd6[3];			// C97-99. 237-239 Резерв
	Uns				ShC_Up; 			// C100. 240 убрать
	Uns				ShC_Down;			// C101. 241 уброать в H
	Uns             Position;           // C102. 242 Положение
	Uns				LowPowerLvl;		// C103. 243 Значение напряжения в попугаях для 3х секунд
	Int				Upor25;				// C104. 244 Значение для добавлени/убавления значения упора при 220в 25% от максМ
	Int				Upor35;				// C105. 245 Значение для добавлени/убавления значения упора при 220в 35% от максМ
	Int				Upor50;				// C106. 246 Значение для добавлени/убавления значения упора при 220в 50% от максМ
	Int				Upor75;				// C107. 247 Значение для добавлени/убавления значения упора при 220в 75% от максМ
	Int				Upor100;			// C108. 248 Значение для добавлени/убавления значения упора при 220в 100% от максМ
	Int				Corr40Trq;			// C109. 249 Параметр для корректировки индикации малых моментов (меньше 60%)
	Int				Corr60Trq;			// C110. 250 Параметр для корректировки индикации больших моментов (больше 60%)
	Int				Corr80Trq;			// C111. 251
	Int				Corr110Trq;			// C112. 252
	TNetReg				FaultNetRST;	// C113. 253 обрыв фар питания
	Uns				LevelBreakRST;		// C114. 254
	Uns				TimeBreakRST;		// C115. 255
	Uns			    Rsvd2[4];			// C116-119. 256-259 Резерв
} TRamGroupC;

// Группа D (Адрес = 260, Количество = 20 )  	- Команды
typedef struct _TRamGroupD
{
	TTaskReset     TaskClose;           // D0. 260 Задание закрыто
	TTaskReset     TaskOpen;            // D1. 261 Задание открыто
	Uns            RevOpen;             // D2. 262 Обороты на открытие
	Uns            RevClose;            // D3. 263 Обороты на закрытие
	Uns            AutoCalib;           // D4. 264 Автоматическая калибровка
	Uns            CalibReset;          // D5. 265 Сброс калибровки
	TValveCmd	   ControlWord;         // D6. 266 Команда управления
	Uns			   PrtReset;      		// D7. 267 Сброс защит
	Uns            SetDefaults;         // D8. 268 Задание параметров по умолчанию
	Uns 		   Rsvd1;				// D9. 269 отчистка журнала событий
	Uns            CycleReset;			// D10. 270 Сброс счетчика циклов
	Uns			   RsReset;				// D11. 271 Сброс связи
	Uns 		   Rsvd[8];				// D12-19. 272-279 Резерв
} TRamGroupD;


// Группа G (Адрес 280, Количество 30)  - Предположительно заводской тест
typedef struct _TRamGroupG
{
	Uns            Mode;                // G0. 280 Режим теста
	TLedsReg       LedsReg;             // G1. 281 Тест светодиодов блока
	TOutputReg     OutputReg;			// G2. 282 Тест дискретных выходов
	Int			   DacValue;			// G3. 283 Значение ЦАП
	Uns			   DisplShow;			// G4. 284 Старт дисплея
	Uns			   TestCamera;			// G5. 285 Тест с камеры
	Uns			   ThyrGroup;			// G6. 286 Выбор группы тиристоров
	Uns            SifuEnable;          // G7. 287 Разрешение работы СИФУ
	Uns			   Rsvd2[10];			// G8-17. 288-297-Резерв
	Uns			   ThyrOpenAngle;		// G18. 298 Тестовый угол открытия
	Uns			   DiscrInTest;			// G19. 299 Тест дискретных входов
	Uns			   DiscrOutTest;		// G20. 300 Тест дискретных выходов
	Bool		   IsDiscrTestActive;	// G21. 301 Активен ли тест ТС/ТУ
	Uns 		   Rsvd[8];			    // G22-29. 302-309 Резерв
} TRamGroupG;

// Группа H (Адрес 310, Количество 200)
typedef struct _TRamGroupH
{
	TCalibState     CalibState;          // H0. 310 Состояние калибровки
	Uns             CalibRsvd;           // H1. 311 Резерв для калибровки
	LgUns           ClosePosition;    	 // H2-3. 312-313 Положение закрыто		???
	LgUns           OpenPosition;     	 // H4-5. 314-315 Положение открыто		???
	Uns             Password1;           // H6. 316 Основной пароль
	Uns             Password2;           // H7. 317 Заводской пароль
	Uns             ScFaults;            // H8. 318 Аварии КЗ
	Uns				UporOnly;			 // H9. 319 Параметр включающий работу только на упоре
	Uns             CycleCnt;            // H10. 320 Счетчик циклов
	Uns				CmdKey;				 // H11. 321 Команда с кнопок ЩСУ
	Uns				CmdButton;			 // H12. 322 Команда с ручек управления
	Uns				Rsvd1[3];			 // H13-15. 323-325 Резерв
	Uns				TransCurr;			 // H16. 326 Ток перехода
	TCubArray		TqCurr;				 // H17-36. 327-346 Ток поверхности
	TCubArray		TqAngUI;			 // H37-56. 347-366 Углы нагрузки
	TCubArray		TqAngSf;			 // H57-76. 367-386 Углы СИФУ
	Uns				ZazorTime;			 // H77. 387 Время выборки зазора
	Uns             TuState;             // H78. 388 Команды ТУ (открыть закрыть стоп)
  	Uns             Ur;                  // H79. 389 Напряжение фазы R
  	Uns             Us;                  // H80. 390 Напряжение фазы S
  	Uns             Ut;                  // H81. 391 Напряжение фазы T
	Uns             Umid;             	 // H82. 392 Среднее напряжение
	Uns             VSkValue;          	 // H83. 393 Асиметрия фаз питающей сети
  	Uns     	    PhOrdValue;        	 // H84. 394 Чередование фаз сети
	Uns             Rsvd5;          	 // H85. 395 Диагностика сети (для неисправностей)
	Uns				Imidpr;			 	 // H86 396 Резерв
	Uns             Rsvd6;          	 // H87. 397 Диагностика нагрузки
	Uns             IuPr;                // H88. 398 Ток фазы U
	Uns             IvPr;                // H89. 399 Ток фазы V
	Uns             IwPr;                // H90. 400 Ток фазы W
	Uns             Imid;				 // H91. 401 Средний ток
  	Uns             ISkewValue;          // H92. 402 Асиметрия токов нагрузки
  	LgUns           Position;            // H93-94. 403-404 Положение
	LgUns  			FullStep;        	 // H95-96. 405-406 Полный ход
	TReverseType	ReverseType;         // H97. 407 Тип реверса
	Uns				Rsvd4;				 // H98. 408
	TInputReg       StateTu;           	 // H99. 409 Состояние дискретных входов
	TOutputReg      StateTs;          	 // H100. 410 Состояние дискретных выходов
	Uns             TuReleMode;          // H101. 411 Релейный режим
	Uns      	    Rsvd2[12];		 	 // H102-113. 412-423Нормальное состояние входов
	Uns             StartIndic;			 // H114. 424 Индикация в старте
 	Uns             SleepTime;           // H115. 425 Дежурный режим  mb to C
	Uns             BusyValue;       	 // H116. 426 Процент исполнения
	Uns 			LogEvAddr;			 // H117. 427 Текущий адрес журнала событий
	Uns 			LogCmdAddr;			 // H118. 428 Текущий адрес журнала команд
	Uns 			LogParamAddr;		 // H119. 429 Текущий адрес журнала изменения параметров
	Uns				LogEvCount;			 // H120. 430 Количество записанных ячеек журнала событий
	Uns				LogCmdCount;		 // H121. 431 Количество записанных ячеек журнала команд
	Uns				LogParamCount;		 // H122. 432 Количество записанных ячеек журнала изменения параметров
	Uns             Torque;           	 // H123. 433 Момент
	Int             Speed;				 // H124. 434 Скорость
	Uns				Seconds;			 // H125. 435 Секунды
	TContactorGroup ContGroup;			 // H126. 436 Управление контакторами
	TBurCmd 		LogControlWord;		 // H127. 437 Команды БУР
	Uns				LogReset;			 // H128. 438 Сброс журналов
	Uns 			BkpIndication;		 // H129. 439 Индикация на БКП
	//-------------------------------
	Uns 			Rsvd8[59];			 // H130-189. 440-499 Резерв
} TRamGroupH;

// Группа E (Адрес 500+40, Количество 32)
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
	TInputReg      LogStateTu;           // 14.Состояние дискретных входов
	TOutputReg     LogSataeTs;          // 15.Состояние дискретных выходов
	Uns 		   Rsvd[14];			// 16-29.Резерв
} TRamGroupE;


typedef struct _TRamLogBuff
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
} TRamLogBuff;



// ОБЩИЕ ПАРАМЕТРЫ
// Группа E (Адрес 0, Количество 40)
// начальный адрес 0
typedef struct _TRamGroupT
{
	TTek_TechReg 	TechReg;			// T1 0 Технологический регистр
	TTek_DefReg  	DefReg;				// T2 1 Регистр дефектов
	Uns 		 	PositionPr;			// T3 2 Текущее положение %
	TTek_ComReg  	ComReg;				// T4 3 Регистр команд
	Uns 		 	CycleCnt;			// T5 4 Счетчик циклов
	Uns			 	Rsvd1;				// T6 5 Резерв
	Uns 		 	Iu;					// T7 6 Ток фазы U
	Uns 		 	Rsvd2[9];			// T8-16 7-15 Резерв
	Uns 			Ur;					// T17 16 Напряжение входной сети
	Uns				Rsvd3;				// T18 17 Резерв
	Uns 		 	Speed;				// T19 18 Текущая скорость
	Uns			 	Rsvd4;				// T20 19 Резерв
	Uns 		 	Torque;				// T21 20 Текущий момент нагрузки Н*м
	TTek_Discrete 	TsTu;				// T22 21 ТС/ТУ
	Uns				Rsvd6[4];			// T23-26 22-25 Резерв
	Uns				RsStation;			// T27 26 Адрес станции (только для чтения)
	Uns				Rsvd7;				// T28 27 Резерв
	Uns				Rsvd8[12];			// T29-40 28-39 Резерв
} TRamGroupT;


// Структура параметров устройства
typedef struct TRam
{
  TRamGroupT		ramGroupT;			//адреса 0-39
  TRamGroupB		ramGroupB;			//адреса 40-139
  TRamGroupC		ramGroupC;			//адреса 140-259
  TRamGroupD		ramGroupD;			//адреса 260-279
  TRamGroupG		ramGroupG;			//адреса 280-309
  TRamGroupH		ramGroupH;			//адреса 310-
  TRamGroupA		ramGroupA;			// ПАРАМЕТРЫ НАСТРОЙКИ
  TRamGroupE		ramGroupE;
} TRam;

// Структура параметров устройства
/*typedef struct _TTEKDriveData
{
	TTek_Mb_Group	tekGroup;
} TTekDriveData;
*/

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
#define REG_CODE				GetAdr(ramGroupB.MainCode)
#define REG_FCODE				GetAdr(ramGroupC.FactCode)

#define REG_GEAR_RATIO			GetAdr(ramGroupC.GearRatio)

#define REG_SIN_FILTER_TF		GetAdr(ramGroupC.SinTf)
#define REG_RMS_FILTER_TF		GetAdr(ramGroupC.RmsTf)

#define REG_TU_TYPE				GetAdr(ramGroupB.InputType)
#define REG_TU_INVERT			GetAdr(ramGroupB.TuInvert.all)
#define REG_TS_INVERT			GetAdr(ramGroupB.TsInvert.all)


#define REG_DRIVE_TYPE			GetAdr(ramGroupC.DriveType)
#define REG_MAX_TRQE			GetAdr(ramGroupC.MaxTorque)
#define REG_I_NOM				GetAdr(ramGroupC.Inom)

#define REG_TORQUE_CURR 		GetAdr(ramGroupH.TqCurr)
#define REG_TORQUE_ANGLE_UI		GetAdr(ramGroupH.TqAngUI)
#define REG_TORQUE_ANG_SF		GetAdr(ramGroupH.TqAngSf)

#define REG_TASK_TIME			GetAdr(ramGroupB.DevTime)
#define REG_TASK_DATE			GetAdr(ramGroupB.DevDate)

#define REG_PASSW1_NEW			GetAdr(ramGroupH.Password1)
#define REG_PASSW2_NEW			GetAdr(ramGroupH.Password2)

#define REG_OVERWAY_ZONE		GetAdr(ramGroupB.OverwayZone)

/*
#define GLOBAL_REG_GR		0
#define GLOBAL_COUNT_GR		1

#define REG_GR				GetAdr(ramGroup.STATUS)
#define COUNT_GR			30
*/

#define REG_CALIB_STATE		GetAdr(ramGroupH.CalibState)
#define REG_CALIB_CLOSE		GetAdr(ramGroupH.ClosePosition)
#define REG_CALIB_OPEN		REG_CALIB_CLOSE+2

#define REG_FACTORY_NUMBER	GetAdr(ramGroupC.FactoryNumber)
#define REG_PRODUCT_DATE	GetAdr(ramGroupC.ProductYear)
#define REG_CYCLE_CNT		GetAdr(ramGroupH.CycleCnt)

#define REG_SHC_FAULT		GetAdr(ramGroupH.ScFaults)
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
#define REG_RS_BAUD_RATE	GetAdr(ramGroupB.RsBaudRate)

// Глобальные переменные модуля
#define REG_VER_PO			GetAdr(ramGroupA.VersionPO)
#define REG_SUBVER_PO		GetAdr(ramGroupC.SubVersionPO)

//#define IsImpulseMode		(!g_Ram.ramGroupA.digitalMode.all)
//#define IsPotentialMode		(g_Ram.ramGroupA.digitalMode.all)

// Коды блокировки
#define LOCK_MPU			BIT0	// Блокировка МПУ
#define LOCK_TU				BIT1	// Блокировка ТУ
#define LOCK_RS485			BIT2	// Блокировка RS485
#define IsLockMPU() 		(g_Ram.ramGroupA.LOCK_CONTROL & LOCK_MPU)
#define IsLockTU()			(g_Ram.ramGroupA.LOCK_CONTROL & LOCK_TU)

#define CMD_DEFAULTS_USER	0x0010	// Пользовательские параметры по умолчанию
#define CMD_RES_CLB			0x0020	// Сброс калибровки датчика положения
#define CMD_RES_FAULT		0x0040	// Сброс защит
#define CMD_CLR_LOG			0x0080	// Очистка журнала
#define CMD_RES_CYCLE		0x0100	// Сброс счетчика циклов
#define CMD_PAR_CHANGE		0x0200	// Изменение параметров
#define CMD_DEFAULTS_FACT 	0x0400	// Заводские параметры по умолчанию

#define STATE_TU24 		GpioDataRegs.GPBDAT.bit.GPIO40

void g_Ram_Init(TRam *);
void g_Ram_Update(TRam *);
void RefreshParams(Uns);
Int MinMax3IntValue (Int, Int, Int);
Int Max3Int (Int , Int , Int );


extern TRam 			g_Ram;
//extern TTekDriveData	g_RamTek;


#endif




