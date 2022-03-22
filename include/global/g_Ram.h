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
#include "peref_LedsDrv.h"
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
		Uns Rsvd1:4;		// 3-6
		Uns MuDu:1;			// 7
		Uns Opening:1;		// 8
		Uns Closing:1;		// 9
		Uns Stop:1;			// 10
		Uns KVO:1;			// 11
		Uns KVZ:1;			// 12
		Uns Ten:1;			// 13
		Uns Rsvd14:1;		// 14
		Uns Ready:1;		// 15
	} bit;
} TTek_TechReg;

typedef union _TTek_DefReg
{
	Uns all;
	struct
	{	// БУ50
	    /*Uns Rsvd5:1;		// 0
		Uns ShC:1;		// 1	короткое замыкание
		Uns I2t:1;		// 2	времятоковая защита
		Uns Rsvd3:1;		// 3
		Uns Phl:1;		// 4	Резерв
		Uns Rsvd1:5;		// 5-9
		Uns Uv:1;		// 10	пониженное напряжение входной сети
		Uns Th:1;		// 11	Перегрев блока
		Uns Tl:1;		// 12
		Uns Ov:1;		// 13	Повышенное напряжение входной сети
		Uns Rsvd2:1;		// 14
		Uns NoCalib:1;  	// 15*/
		// БУР
		Uns I2t:1;			// 0	времятоковая защита
		Uns ShC:1;			// 1	короткое замыкание
		Uns Drv_T:1;		// 2	перегрев электродвигателя
		Uns Uv:1;			// 3	пониженное напряжение входной сети
		Uns Phl:1;			// 4	Резерв
		Uns NoMove:1;		// 5	Отсутствие движения
		Uns Ov:1;			// 6	Повышенное напряжение входной сети
		Uns Bv:1;			// 7	Обрыв фаз входной сети
		Uns Rsvd:1;			// 8	Резерв
		Uns Th:1;			// 9	Перегрев блока
		Uns Tl:1;			// 10
		Uns Rsvd1:1;		// 11
		Uns PhOrdU:1;		// 12	Неверное чередование фаз сети
		Uns PhOrdDrv:1;		// 13	Неверное чередование фаз двигателя
		Uns DevDef:1;		// 14
		Uns NoCalib:1;		// 15
	} bit;
} TTek_DefReg;

typedef union _TTEK_FaultReg
{
	Uns all;
	struct
	{
		Uns NoCalib:1;		// 0 	Время-токовая защита
		Uns MuDu:1;			// 1 	Короткое замыкание
		Uns Uv:1;			// 2 	Перегрев двигателя
		Uns Ov:1;			// 3 	Пониженеое напряжение фаз сети
		Uns Vsk:1;			// 4 	Обрыв фазы электродвигателя
		Uns Bv:1;			// 5 	Нет движения
		Uns ThErr:1;		// 6 	Перенапряжение на входе
		Uns Tl:1;			// 7 	Критическое снижение напряжения на входе
		Uns DevDef:1;		// 8 	Резерв
		Uns Rsvd1:1;		// 9  	Перегрев блока
		Uns Rsvd2:1;		// 10 	Переохлождение блока
		Uns Rsvd3:1;		// 11 	Резерв
		Uns Rsvd4:1;		// 12   Неверное чередование фаз сети
		Uns Rsvd5:1;		// 13   Чередование фаз двигателя
		Uns Rsvd6:1;		// 14 	Дефект устройства (кроме перегрева и переохлаждения)
		Uns Rsvd7:1;		// 15 	Резерв
	} bit;
} TTEK_FaultReg;

typedef union _TTek_ComReg
{
	Uns all;
	struct
	{
		Uns Stop:1;				// 0
		Uns Open:1;				// 1
		Uns Close:1;				// 2
		Uns Rsvd:2;				// 3-4
		Uns PrtReset:1;				// 5
		Uns Rsvd1:2;				// 6-7
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
	TStatusReg      Status;         // 0. 500 Статус работы
	TFltUnion	    Faults;	        // 1-4. 501-504 Аварии
	TInputReg       StateTu;     	// 5. 505 Состояние дискретных входов
	TOutputReg      StateTs;	    // 6. 506 Состояние дискретных выходов
	Uns             Position;       // 7. 507 Положение
	Uns             Torque;         // 8. 508 Момент
	Int             Speed;		    // 9. 509 Скорость
	Uns             Ur;             // 10. 510 Напряжение фазы R
  	Uns             Us;             // 11. 511 Напряжение фазы S
  	Uns             Ut;             // 12. 512 Напряжение фазы T
	Uns             Iu;             // 13. 513 Ток фазы U
	Uns             Iv;             // 14. 514 Ток фазы V
	Uns             Iw;             // 15. 515 Ток фазы W
	Int             AngleUI;        // 16. 516 Угол нагрузки
	TCalibState     CalibState;     // 17. 517 Статус калибровки
	Int             PositionPr;     // 18. 518 Положение %
	Uns             FullWay;        // 19. 519 Полный ход
	Int             CurWay;         // 20. 520 Текущий ход
	Uns             CycleCnt;       // 21. 521 Счетчик циклов
	Int             Temper;         // 22. 522 Температура блока
	Int             TemperBKP;      // 23. 523 Температура блока БКП
	Uns             VersionPO;      // 24. 524 Версия ПО
	Uns             VersionPOBkp;   // 25. 525 Версия ПО БКП
	Uns				VoltageDown;	// 26. 526 Падение напряжения
	Uns				RevErrValue;	// 27. 527 Колво сбоев датчика положения
	Uns 			TemperED;		// 28. 528 температура ЭД для БКП 9.1
	TBCP9Reg		BCP9Reg;		// 29. 529 faults from BCP91
	Uns 			Rsvd[8];	    // 30-39. 530-539 Резерв
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
	TBreakMode      BreakMode;			// B7. 47 Тип уплотнения
	TRodType        RodType;           	// B8. 48 Тип штока
	Uns				CloseZone;			// B9. 49 Зона закрыто
	Uns				OpenZone;			// B10. 50 Зона открыто
	Uns             PositionAcc;        // B11. 51 Зона смещения
	Uns            	MainCode;        	// B12. 52 Код доступа
	TTimeVar 		DevTime;			// B13. 53 Время
	TDateVar 		DevDate;			// B14. 54 Дата
	Int		 		TimeCorrection;		// B15. 55 Корректеровка времени
	TIndicMode  	IIndicMode;         // B16. 56 Режим индикации тока
	TMuDuSetup  	MuDuSetup;          // B17. 57 Настройка режима МУ/ДУ
	TDuSource		DuSource;			// B18. 58 Источник команд ДУ
	Uns         	TuLockSeal;         // B19. 59 Блокировка залипани
	Uns         	TuTime;             // B20. 60 Время команды
	TInputType 		InputType;			// B21. 61 Тип входного сигнала 24/220
	TUInvert	    TuInvert;			// B22. 62 Маска дискретных входов
	TSInvert 		TsInvert;			// B23. 63 Маска дискретных выходов
	TBaudRate       RsBaudRate;         // B24. 64 Скорость связи
	Uns             RsStation;          // B25. 65 Адрес станции
	TParityMode		RsMode;				// B26. 66 Режим связи
	Uns				MuffTimer;			// B27. 67
	Uns				SwitcherMuDuMode;	// B28. режим с использованием переключателя МУ/ДУ на ЩТМ (опрос ТУ в режиме МУ)
	Uns				MuffStartTimer;		// B29. 69 Муфта по старту
	Uns				StartTime;		    // В30. 70 Время разгона
	Uns 			Sec3Mode;			// B31. 71
	Uns				NoMoveTime;			// B32. 72 Время отсутствия движения
	Uns				OverwayZone;		// B33. 73 Максимальный путь уплотнения
	TPlaceType		PlaceType;			// B34. 74 место установки привода (обычн, агрегатная, пожарка)
	Uns				KvoKvzOffOnStop;	// B35. 75 Размыкание КВО и КВЗ при СТОП, 0 - Нет, 1 - Да // SDV ЦПА
 	TStopMethod		StopMethod;			// B36. 76 Выбор типа торможения (Динамика, Противовключение)
 	Uns				ConductorSection;   // B37. 77 Сечение проводника
 	Uns             ConductorLength;	// B38. 78 Длина проводника
 	Int				VoltCorr;			// B39.
 	Uns 			Rsvd[10];			// B40 41 42 43 44 45 46 47 48 49 -49. 77-89
} TRamGroupB;

// Группа C (Адрес = 90, Количество = 120) - Заводские параметры
typedef struct _TRamGroupC
{
	Uns             FactCode;           	// C0. 90 Код доступа
	TDriveType      DriveType;          	// C1. 91 Тип привода
	Uns        		ProductYear;        	// C2. 92 Год изготовления блока
  	Uns             FactoryNumber;      	// C3. 93 Заводской номер блока
  	Uns             MaxTorque;          	// C4. 94 Максимальный момент привода
	Uns             Inom;               	// C5. 95 Номинальный ток
  	Uns             GearRatio;          	// C6. 96 Передаточное число редуктора
  	Uns				BKP91;	    			// С7. 97 Резерв
  	Uns				TekDefRegSwitch;		// С8. 98 Резерв
	THallBlock      HallBlock;          	// C9. 99 Состояние датчиков холла блока
	Uns             SubVersionPO;	    	// C10. 100 Подверсия ПО
	Uns				progonCycles;			// С11. 101 циклы прогона
	Uns				reserv102;				// С12. 102 Резерв
	Uns             MuffZone;           	// C13. 103 Расстояние сброса муфты
	Uns				PosSensPow;				// C14. 104 Тип датчика положения
	Uns				reserv105;				// C15. 105 Резерв
	Uns             SetDefaults;        	// C16. 106 Задание параметров по умолчанию
	Uns				ConnFlagCountBCP;		// C17. 107 временная задержка на определение аварии связь с БКП
	Uns             ModbusPauseStart;       // C18. 108 пауза при старте на модбас и ТС - 09,01,2020 - требование Обриев (и здравый смысл)
	Uns             BrakePause;         	// C19. 109 Пауза перед торможением
	Uns             BrakeTime;          	// C20. 110 Время торможения
	Uns       		StopShnTime;   			// C21. 111 Время торможения динамическим торможением
	Int				BreakZone;				// C22. 112 Число оборотов дв за которое начинаем тормозить
	Uns				BreakControl;			// c23. 113 управление тормозом БКП9.1
	Uns				Rsvd5[8];				// C24-33. 114-123 резерв
	Uns				CounterDisconect;		// C32  122 Счетчик обрывов связи между БКП БКЭП (реальных а не по аварии)
	Uns				TimeBtnStopKVOKVZ;		// C33  123 время разрыва КВО КВЗ при повороте ручки стоп
	Uns				BCPConTimeMove;			// C34  124 Время Выставления аварии нет связи в движении
	Int             CorrTemper;         	// C35. 125 Корректировка температуры блока БКП
	Uns        		BCPConTime;            // C36. 126 Время Выставления аварии нет связи в стопе
	Uns				CurrentMpyType;			// C37. 127 Управление ножкой уселителя сигнала АЦП для ДТ. работает только если тип привода не определен
	Uns             IU_Mpy;             	// C38. 128 Корректировка тока фазы U
	Uns             IV_Mpy;             	// C39. 129 Корректировка тока фазы V
	Uns             IW_Mpy;             	// C40. 130 Корректировка тока фазы W
	Uns             IU_Offset;          	// C41. 131 Смещение тока фазы U
	Uns             IV_Offset;          	// C42. 132 Смещение тока фазы V
	Uns             IW_Offset;          	// C43. 133 Смещение тока фазы W
	Uns             UR_Mpy;             	// C44. 134 Корректировка напряжения фазы R
	Uns             US_Mpy;             	// C45. 135 Корректировка напряжения фазы S
	Uns             UT_Mpy;             	// C46. 136 Корректировка напряжения фазы T
	Uns             UR_Offset;          	// C47. 137 Смещение напряжения фазы R
	Uns             US_Offset;          	// C48. 138 Смещение напряжения фазы S
	Uns             UT_Offset;          	// C49. 139 Смещение напряжения фазы T
	Uns				SinTf;					// C50. 140 ПОСТ.ВРЕМЕНИ ФИЛЬТРОВ СИНУСОВ
	Uns				RmsTf;					// C51. 141 ПОСТ.ВРЕМЕНИ ФИЛЬТРОВ RMS
	Uns				Dac_Mpy;				// C52. 142 Корректировка ЦАП
	Int				Dac_Offset;				// C53. 143 Смещение ЦАП
	TPrtMode        Ov;						// C54. 144 Защита от превышения напряжения
	Uns             OvLevel_max;        	// C55. 145 Уровень превышения напряжения при 47% превышения (320В)
	Uns             OvTime_max;     		// C56. 146 Время превышения напряжения при 47% превышения (1с)
	Uns             OvLevel;    			// C57. 147 Уровень превышения напряжения
	Uns             OvDTime;     			// C58. 148 Время превышения напряжения неисправность
	Uns             OvTime;     		// C59. 149 Время превышения напряжения
	TPrtMode        Uv;			// C60. 150 Защита от превышения напряжения
	Uns             UvLevel;   		// C61. 151 Уровень понижения напряжения
	Uns             UvDTime;    		// C62. 152 Время понижения напряжения
	Uns             UvTime;     		// C63. 153 Время превышения напряжения
  	Uns				reserv154;		// С64. 154 Резерв
  	Uns				reserv155;		// С65. 155 Резерв
  	Uns				reserv156;		// С66. 156 Резерв
	TPrtMode        Bv;        		// C67. 157 Защита от обрыва входных фаз
	Uns             BvLevel;   		// C68. 158 Уровень обрыва входных фаз
	Uns				BvLevelMove;		// C69. 159 Уровень обрыва питающих фаз в движении
	Uns             BvTime;    		// C70. 160 Время обрыва входных фаз
	Uns				Rsvd14;			// C71  161 резерв
	TPrtMode        Phl;           		// C72. 162 Защита от обрыва фаз нагрузки
	Uns             PhlLevel;      		// C73. 163 Уровень обрыва фаз
	Uns             PhlTime;       		// C74. 164 Время определения обрыва фаз
	TPrtMode        I2t;                	// C75. 165 Время-токовая защита
	Uns				Rsvd6[12];		// C76-84. 166-274 Резерв
	TPrtMode        ShC;        		// C88. 178 Защита от короткого замыкания
	Uns				ShC_Up; 		// C89. 179 убрать
	Uns				ShC_Down;		// C90. 180 уброать в H
	TPrtMode        CalibIndic;         	// C91. 181 Индикация калибровки
	TPrtMode        ErrIndic;           	// C92. 182 Индикация аварий устройства
	TPrtMode        TemperTrack;        	// C93. 183 Защита от перегрева/переохлаждения блока
	Int				Rsvd13[3];		// C94 - 96. Резерв
	Int             TenOnValue;         	// C97. 187 Уровень включения ТЕНа
	Int             TenOffValue;        	// C98. 188 Уровень выключения ТЕНа
	Uns 			Rsvd11[3];		// С99-101.  189-191 резерв
	TPrtMode        PhOrd;          	// C102. 192 Защита от неверного чередования фаз двигателя
	Uns				PhOrdTime;		// C103. 193 Время чередования фаз двигателя
	Uns             PhOrdZone;      	// C104. 194 Расстояние чередования фаз двигателя
	TPrtMode        MuDuDef;        	// C105. 195 Защита ошибки входов Му/Ду
	Uns				Rsvd12[2];		// C106-107. 196-197 Резерв
	Int				CorrClose30Trq;		// C108. 198
	Int				CorrClose40Trq;		// C109. 199 Параметр для корректировки индикации малых моментов (меньше 60%)
	Int				CorrClose60Trq;		// C110. 200 Параметр для корректировки индикации больших моментов (больше 60%)		Int		CorrOpen80Trq;		// C116. 206
	Int				CorrClose80Trq;		// C111. 201
	Int				CorrClose110Trq;	// C112. 202
	Int				CorrOpen30Trq;		// C113. 203
	Int				CorrOpen40Trq;		// C114. 204 Параметр для корректировки индикации малых моментов (меньше 60%)
	Int				CorrOpen60Trq;		// C115. 205 Параметр для корректировки индикации больших моментов (больше 60%)
	Int				CorrOpen80Trq;		// C116. 206
	Int				CorrOpen110Trq;		// C117. 207
	Uns				Rsvd118;		// C118. 208 Корректеровка момента в открытие
	Uns				TuOffsetCalib;		// C119.  209. калибровка офсетов ТУ - проводить при ту 220 для большей точности.
	//------Параметры для ТУ------------------------------------
	Uns				LevelOn220;		// C120. 210
	Uns				LevelOff220;		// C121. 211
	Uns				LevelOn24;		// C122. 212
	Uns				LevelOff24;		// C123. 213
	//--------------------------------------------------
	Uns				p_UOpen_Mpy220;   	// C124. 214
	Uns				p_UOpen_Mpy24;   	// C125. 215
	Uns				p_UOpen_Offset;		// C126. 216
		//------------------------------
	Uns				p_UClose_Mpy220;	// C127. 217
	Uns				p_UClose_Mpy24;		// C128. 218
	Uns				p_UClose_Offset;	// C129. 219
		//-------------------------------
	Uns				p_UStopOpen_Mpy220;	// C130. 220
	Uns				p_UStopOpen_Mpy24;	// C131. 221
	Uns				p_UStopOpen_Offset;	// C132. 222
	//-------------------------------
	Uns				p_UStopClose_Mpy220;	 // C133. 223
	Uns				p_UStopClose_Mpy24;	 // C134. 224
	Uns				p_UStopClose_Offset;	 // C135. 225
		//-------------------------------
	Uns				p_UMu_Mpy220;		 // C136. 226
	Uns				p_UMu_Mpy24;		 // C137. 227
	Uns				p_UMu_Offset;		 // C138. 228
		//------------------------------
	Uns				p_UDu_Mpy220;		 // C139. 229
	Uns				p_UDu_Mpy24;		 // C140. 230
	Uns				p_UDu_Offset;		 // C141. 231
	//-------------------------------
	Uns				p_UResetAlarm_Mpy220;	 // C142. 232
	Uns				p_UResetAlarm_Mpy24; 	 // C143. 233
	Uns				p_UResetAlarm_Offset;	 // C144. 234
	Uns				Tu_Mpy_Calib;		 // C145. 235
	Uns				Tu_Open_Value;		 // C146. 236
	Uns				Tu_Close_Value;		 // C147. 237
	Uns				Tu_StopOpen_Value;	 // C148. 238
	Uns				Tu_StopClose_Value;	 // C149. 239
	Uns				Tu_Du_Value;		 // C150. 240
	Uns				Tu_Mu_Value;		 // C151. 241
	Uns				Tu_ResetAlarm_Value;	 // C152. 242
	Uns 			Rsvd20[17];		 //С153-169 243-259
} TRamGroupC;

// Группа D (Адрес = 260, Количество = 20 )  	- Команды
typedef struct _TRamGroupD
{
	TTaskReset     	TaskClose;           // D0. 260 Задание закрыто
	TTaskReset     	TaskOpen;            // D1. 261 Задание открыто
	Uns            	RevOpen;             // D2. 262 Обороты на открытие
	Uns           	RevClose;            // D3. 263 Обороты на закрытие
	Uns	       		reserv264;			// D4  264 резерв
	Uns            	CalibReset;          // D5. 265 Сброс калибровки
	TValveCmd      	ControlWord;         // D6. 266 Команда управления
	Uns	       		PrtReset;      		// D7. 267 Сброс защит
	Uns           	SetDefaults;         // D8. 268 Задание параметров по умолчанию
	Uns 			Rsvd1;				// D9. 269 отчистка журнала событий
	Uns            	CycleReset;			// D10. 270 Сброс счетчика циклов
	Uns				RsReset;				// D11. 271 Сброс связи
	Uns 		   	Rsvd[8];				// D12-19. 272-279 Резерв
} TRamGroupD;


// Группа G (Адрес 280, Количество 30)  - Предположительно заводской тест
typedef struct _TRamGroupG
{
	Uns            Mode;                // G0. 280 Режим теста
	TLedReg        LedsReg;             // G1. 281 Тест светодиодов блока
	TOutputReg     OutputReg;			// G2. 282 Тест дискретных выходов
	Int			   DacValue;			// G3. 283 Значение ЦАП
	Uns			   DisplShow;			// G4. 284 Старт дисплея
	Uns			   TestCamera;			// G5. 285 Тест с камеры
	Uns			   TestContactor;		// G6. 286 Тест контакторов
	Uns			   Rsvd2[12];			// G7-18. 286-298-Резерв
	Uns			   DiscrInTest;			// G19. 299 Тест дискретных входов	// НЕ РЕАЛИЗОВАННО
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
	Uns				MuffFault;			 // H9. 319 Запись аварии МУФТА
	Uns             CycleCnt;            // H10. 320 Счетчик циклов
	Uns             TemperBCPFault;      // H11. 321 запись аварии ТЕМПЕРАТУРА БКП 110
	Uns				CmdButton;			 // H12. 322 Команда с ручек управления
	Uns				BkpType;			 // H13. 323 тип БКП для выставления соответсвующего типа привода
	TDateVar 		HideDate;			 // H14. 324 Дата
	Uns				TransCurr;			 // H16. 325 Ток перехода
	TCubArray		TqCurr;				 // H17-36. 326-349 Ток поверхности
	TCubArray		TqAngUI;			 // H37-56. 350-373 Углы нагрузки
	TCubArray		TqAngSf;			 // H57-88. 374-397 Углы СИФУ
	Uns             TuState;             // H78. 398 Команды ТУ (открыть закрыть стопО СтопЗ)
	Uns             Umid;             	 // H82. 399 Среднее напряжение
	Uns             BKP_Temper;          // H85. 400 Температура БКП
	Uns				Imidpr;			 	 // H86  401 Резерв
	Uns             IuPr;                // H88. 402 Ток фазы U
	Uns             IvPr;                // H89. 403 Ток фазы V
	Uns             IwPr;                // H90. 404 Ток фазы W
	Uns             Imid;				 // H91. 405 Средний ток
  	LgUns           Position;            // H92-93. 406-407 Положение
	LgUns  			FullStep;        	 // H94-95. 408-409 Полный ход
	TReverseType	ReverseType;         // H97. 410 Тип реверса
	//-------------------------------
	Uns				BadTask_2kHz;        // H102. 411 Регистр "плохих" задач прерывания 2 кГц
	Uns				BadTask_200Hz;       // H103. 412 Регистр "плохих" задач прерывания 200 Гц
	Uns				BadTask_50Hz[3];     // H104-106. 413-415 Регистр "плохих" задач прерывания 50 Гц
	Uns				BadTask_10Hz;        // H107. 416 Регистр "плохих" задач прерывания 10 Гц
	Uns				BadTask_Reset;       // H108. 417 Сброс регистров "плохих" задач
	Uns				CpuTime;             // H109. 418 Процессорное время конкретной задачи
	Uns				TaskList;            // H110. 419 Номер списка задач
	Uns				TaskNumber;          // H111. 420 Номер задачи в списке
	//-------------------------------
	Uns      	    Rsvd2[5];		 	 // H112-116. 421-425 Резерв
	Uns 			LogEvAddr;			 // H117. 426 Текущий адрес журнала событий
	Uns 			LogCmdAddr;			 // H118. 427 Текущий адрес журнала команд
	Uns 			LogParamAddr;		 // H119. 428 Текущий адрес журнала изменения параметров
	Uns 			LogSimAddr;		     // H120. 429 Текущий адрес журнала SIM
	Uns				LogEvCount;			 // H121. 430 Количество записанных ячеек журнала событий
	Uns				LogCmdCount;		 // H122. 431 Количество записанных ячеек журнала команд
	Uns				LogParamCount;		 // H123. 432 Количество записанных ячеек журнала изменения параметров
	Uns				LogSimCount;		 // H124. 433 Количество записанных ячеек журнала SIM
	Uns				Seconds;			 // H125. 434 Секунды
	TContactorGroup ContGroup;			 // H126. 435 Управление контакторами
	TBurCmd 		LogControlWord;		 // H127. 436 Команды БУР
	Uns				LogReset;			 // H128. 437 Сброс журналов
	TLedRegBKP 		BkpIndication;		 // H129. 438 Индикация на БКП
	//-------------------------------
	Uns				PosFix;				 // H130. 439
	Uns				PositionAccTemp;	 // H131. 440 14.07.21 Временная переменная для Зоны смещения. Добавлена для того, чтобы зона смещения обнулялась во время движения электропривода
	Uns 			Rsvd9[58];			 // H132-189. 441-499 Резерв
} TRamGroupH;

// Группа E (Адрес 500+40, Количество 32)
typedef struct _TRamGroupE
{
	TTimeVar       LogTime;				// T.Время
	TDateVar       LogDate;				// D.Дата
	TStatusReg     LogStatus;           // 0.Статус работы
	TFltUnion      LogFaults;			// 1-4.Аварии
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

//---------------------------------------------------------------------------

// ПАРАМЕТРЫ УСТРОЙСТВА ПЛАВНОГО ПУСКА
// Группа ATS (Адрес = 540, Количество = 20) - Диагностика - просмотр
typedef struct _TRamGroupATS
{
	TATS48_ControlReg  		Control1;           // 0.
	Uns	            		Control2;			// 1.
	TATS48_StatusReg        State1;     	    // 2.
	TATS48_ExStatusReg      State2;		    	// 3.
	TATS48_Ex2StatusReg     State3;        		// 4.
	Uns						LFT;				// 5. последняя появившаяся неисправность (код стр. 43 мануала)
	Uns						PHP;				// 6. защита от обрыва фаз
	Uns 					Rsvd[13];			// 7-20. Резерв
} TRamGroupATS;

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
	TTek_DefReg		DefReg;				// T2 1 Регистр дефектов
	Int 			PositionPr;			// T3 2 Текущее положение %
	TTek_ComReg  	ComReg;				// T4 3 Регистр команд
	Uns 			CycleCnt;			// T5 4 Счетчик циклов
	Uns				Rsvd1;				// T6 5 Резерв
	Uns 		 	Iu;					// T7 6 Ток фазы U
	Uns             Rsvd8;              // T8 7 Резерв
	Uns             Rsvd9;              // T9 8 Резерв
	Uns             Rsvd10;             // T10 9 Резерв
	Uns             Rsvd11;             // T11 10 Резерв
	Uns             Rsvd12;             // T12 11 Резерв
	Uns             Rsvd13;             // T13 12 Резерв
	Uns             Rsvd14;             // T14 13 Резерв
	Uns             Rsvd15;             // T15 14 Резерв
	Uns             Rsvd16;             // T16 15 Резерв
	Uns 			Ur;					// T17 16 Напряжение входной сети
	Uns				Rsvd17;				// T18 17 Резерв
	Uns 		 	Speed;				// T19 18 Текущая скорость
	Uns			 	Rsvd4;				// T20 19 Резерв
	Uns 		 	Torque;				// T21 20 Текущий момент нагрузки Н*м
	TTek_Discrete 	TsTu;				// T22 21 ТС/ТУ
	Uns				Rsvd23;  			// T23 22 Резерв
	Uns             Rsvd24;             // T24 23 Резерв
	Uns             Rsvd25;             // T25 24 Резерв
	Uns             Rsvd26;             // T26 25 Резерв
	Uns				RsStation;			// T27 26 Адрес станции (только для чтения) 40027
	Uns				MuDu;				// T28 27 Резерв
	Uns				Rsvd28;				// T29 28 Резерв
	Uns				PoVersion;			// T30 29 Резерв
	TTEK_FaultReg	FaultReg;			// T31 30 допалнительный регистр защит
	Uns				Rsvd32;				// T32 31 Резерв
	Uns				Rsvd33;				// T33 32 Резерв
	Uns				Rsvd34;				// T34 33 Резерв
	Uns				Rsvd35;				// T35 34 Резерв
	Uns				Rsvd36;				// T36 35 Резерв
	Uns				Rsvd37;				// T37 36 Резерв
	Uns				Rsvd38;				// T38 37 Резерв
	Uns				Rsvd39;				// T39 38 Резерв
	Uns				Rsvd40;				// T40 39 Резерв
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
  TRamGroupATS		ramGroupATS;		// ПАРАМЕТРЫ УПП-ATS48
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

#define SHN_DATA_ADR		4000
#define SHN_DATA_SIZE		50
#define SHN_DATA_LADR		(SHN_DATA_ADR + SHN_DATA_SIZE - 1)

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
#define REG_ROD_TYPE			GetAdr(ramGroupB.RodType)

#define REG_DRIVE_TYPE			GetAdr(ramGroupC.DriveType)
#define REG_MAX_TRQE			GetAdr(ramGroupC.MaxTorque)
#define REG_I_NOM			GetAdr(ramGroupC.Inom)
#define REG_I_MPY			GetAdr(ramGroupC.IU_Mpy)

#define REG_TORQUE_CURR 		GetAdr(ramGroupH.TqCurr)
#define REG_TORQUE_ANGLE_UI		GetAdr(ramGroupH.TqAngUI)
#define REG_TORQUE_ANG_SF		GetAdr(ramGroupH.TqAngSf)

#define REG_TASK_TIME			GetAdr(ramGroupB.DevTime)
#define REG_TASK_DATE			GetAdr(ramGroupB.DevDate)

#define REG_PASSW1_NEW			GetAdr(ramGroupH.Password1)
#define REG_PASSW2_NEW			GetAdr(ramGroupH.Password2)

#define REG_OVERWAY_ZONE		GetAdr(ramGroupB.OverwayZone)

#define REG_CONTROL				GetAdr(ramGroupD.ControlWord)
#define REG_TASKCLOSE			GetAdr(ramGroupD.TaskClose)
#define REG_REVCLOSE			GetAdr(ramGroupD.RevClose)
#define REG_RSRESET				GetAdr(ramGroupD.RsReset)
#define REG_COM_REG				GetAdr(ramGroupT.ComReg)


#define REG_CALIB_STATE		GetAdr(ramGroupH.CalibState)
#define REG_CALIB_CLOSE		GetAdr(ramGroupH.ClosePosition)
#define REG_CALIB_OPEN		REG_CALIB_CLOSE+2

#define REG_FACTORY_NUMBER	GetAdr(ramGroupC.FactoryNumber)
#define REG_PRODUCT_DATE	GetAdr(ramGroupC.ProductYear)
#define REG_CYCLE_CNT		GetAdr(ramGroupH.CycleCnt)
#define REG_SET_DEFAULTS	GetAdr(ramGroupC.SetDefaults)
#define REG_SHC_FAULT		GetAdr(ramGroupH.ScFaults)
#define REG_MUFF_FAULT		GetAdr(ramGroupH.MuffFault)
#define REG_TEMP_BCP_FAULT	GetAdr(ramGroupH.TemperBCPFault)
#define REG_BATTERY_FAULT	GetAdr(ramGroupH.HideDate)

#define REG_CONTROL_ATS48	GetAdr(ramGroupATS.Control1) //540

#define REG_RS_BAUD_RATE	GetAdr(ramGroupB.RsBaudRate)

#define REG_TU_OPEN_220		GetAdr(ramGroupC.p_UOpen_Mpy220)
#define REG_TU_OPEN_24		GetAdr(ramGroupC.p_UOpen_Mpy24)
#define REG_TU_OPEN_OFF		GetAdr(ramGroupC.p_UOpen_Offset)
#define REG_TU_CLOSE_220	GetAdr(ramGroupC.p_UClose_Mpy220)
#define REG_TU_CLOSE_24		GetAdr(ramGroupC.p_UClose_Mpy24)
#define REG_TU_CLOSE_OFF	GetAdr(ramGroupC.p_UClose_Offset)
#define REG_TU_STOPC_220	GetAdr(ramGroupC.p_UStopClose_Mpy220)
#define REG_TU_STOPC_24		GetAdr(ramGroupC.p_UStopClose_Mpy24)
#define REG_TU_STOPC_OFF	GetAdr(ramGroupC.p_UStopClose_Offset)
#define REG_TU_STOPO_220	GetAdr(ramGroupC.p_UStopOpen_Mpy220)
#define REG_TU_STOPO_24		GetAdr(ramGroupC.p_UStopOpen_Mpy24)
#define REG_TU_STOPO_OFF	GetAdr(ramGroupC.p_UStopOpen_Offset)
#define REG_TU_MU_220		GetAdr(ramGroupC.p_UMu_Mpy220)
#define REG_TU_MU_24		GetAdr(ramGroupC.p_UMu_Mpy24)
#define REG_TU_MU_OFF		GetAdr(ramGroupC.p_UMu_Offset)
#define REG_TU_DU_220		GetAdr(ramGroupC.p_UDu_Mpy220)
#define REG_TU_DU_24		GetAdr(ramGroupC.p_UDu_Mpy24)
#define REG_TU_DU_OFF		GetAdr(ramGroupC.p_UDu_Offset)
#define REG_TU_RESETAL_220	GetAdr(ramGroupC.p_UResetAlarm_Mpy220)
#define REG_TU_RESETAL_24	GetAdr(ramGroupC.p_UResetAlarm_Mpy24)
#define REG_TU_RESETAL_OFF	GetAdr(ramGroupC.p_UResetAlarm_Offset)

#define REG_CORR_CLOSE_30		GetAdr(ramGroupC.CorrClose30Trq)		// C108. 198
#define REG_CORR_CLOSE_40		GetAdr(ramGroupC.CorrClose40Trq)		// C109. 199 Параметр для корректировки индикации малых моментов (меньше 60%)
#define REG_CORR_CLOSE_60		GetAdr(ramGroupC.CorrClose60Trq)		// C110. 200 Параметр для корректировки индикации больших моментов (больше 60%)		Int		CorrOpen80Trq;		// C116. 206
#define REG_CORR_CLOSE_80		GetAdr(ramGroupC.CorrClose80Trq)		// C111. 201
#define REG_CORR_CLOSE_110		GetAdr(ramGroupC.CorrClose110Trq)	// C112. 202
#define REG_CORR_OPEN_30		GetAdr(ramGroupC.CorrOpen30Trq)		// C113. 203
#define REG_CORR_OPEN_40		GetAdr(ramGroupC.CorrOpen40Trq)		// C114. 204 Параметр для корректировки индикации малых моментов (меньше 60%)
#define REG_CORR_OPEN_60		GetAdr(ramGroupC.CorrOpen60Trq)		// C115. 205 Параметр для корректировки индикации больших моментов (больше 60%)
#define REG_CORR_OPEN_80		GetAdr(ramGroupC.CorrOpen80Trq)		// C116. 206
#define REG_CORR_OPEN_110		GetAdr(ramGroupC.CorrOpen110Trq)		// C117. 207

// Глобальные переменные модуля
#define REG_VER_PO			GetAdr(ramGroupA.VersionPO)
#define REG_SUBVER_PO		GetAdr(ramGroupC.SubVersionPO)

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

#define TU_24_220 		    GpioDataRegs.GPADAT.bit.GPIO12

void g_Ram_Init(TRam *);
void g_Ram_Update(TRam *);
void RefreshParams(Uns);
Int MinMax3IntValue (Int, Int, Int);
Int Max3Int (Int , Int , Int );


extern TRam 			g_Ram;
//extern TTekDriveData	g_RamTek;


#endif




