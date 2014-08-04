/*======================================================================
Имя файла:          communication_TU_TS.h
Автор:
Версия файла:		1.0
Дата изменения:
Описание:
Заголовочный файл модуля обработки сигналов телеуправления и телесигнализации
======================================================================*/
#ifndef _TU_TS
#define _TU_TS

//----------- Подключение заголовочных файлов ------------------------------
#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif


#define DIN_OPEN_BIT		0
#define DIN_CLOSE_BIT		1
#define DIN_STOP_BIT		2
#define DIN_MU_BIT		 	3
#define DIN_RESETALARM_BIT 	4
#define DIN_READYTU_BIT 	5
#define DIN_DU_BIT 			6

#define OPEN_BIT			BIT0
#define CLOSE_BIT			BIT1
#define STOP_BIT			BIT2
#define MU_BIT		 		BIT3
#define RESETALARM_BIT 		BIT4
#define READYTU_BIT 		BIT5
#define DU_BIT 				BIT6

#define DOUT_OPEN_BIT		0
#define DOUT_CLOSE_BIT		1
#define DOUT_OPENING_BIT	2
#define DOUT_CLOSING_BIT	3
#define DOUT_DEFECT_BIT 	4
#define DOUT_MUFTA_BIT 		5
#define DOUT_ALARM_BIT 		6
#define DOUT_MUDU_BIT 		7


// Тип входного сигнала
typedef enum {
	it24  =  0,		// 0 Тип сигнала 24 В
	it220 = 1 		// 1 Тип сигнала 220 В
}TInputType;

typedef struct {
	Int 	inputDIN;
	Uns 	*outputReg;
	Int		*enableLevel220;
	Int		*disableLevel220;
	Int		*enableLevel24;
	Int		*disableLevel24;
	Uns		timeOut;
	Uns		timer;
} TDIN;

// Состояние дискретных входов
typedef union _TInputReg {
  Uns all;
	struct {
		Uns Open:1;			// 0	Стоп
		Uns Close:1;		// 1	Закрыть
		Uns Stop:1;			// 2	Открыть
		Uns Mu:1;			// 3	Резерв
		Uns ResetAlarm:1;	// 4	Резерв
		Uns ReadyTu:1;		// 5	Резерв
		Uns Du:1;			// 6	Резерв
		Uns NoUse:9;		// 7-15	Не используются
	} bit;
} TInputReg;


// Состояние дискретных выходов ТС
typedef union _TOutputReg {
	Uns all;
	struct {
		Uns Opened:1;		// 0	Открыто
		Uns Closed:1;		// 1	Закрыто
		Uns Opening:1;		// 2	Открывается
		Uns Closing:1;		// 3	Закрывается
		Uns Defect:1;		// 4	Питание
		Uns Mufta:1;		// 5	Муфта
		Uns Alarm:1;		// 6	Авария
		Uns MUDU:1;			// 7	МУ/ДУ
		Uns Fault:1;		// 8	Неисправность 
		Uns Rsvd:7;			// 9-15  Резерв
	} bit;
} TOutputReg;

typedef struct TDigitalInterface
{
	TInputReg		Inputs;			// Состояние дискретных входов
	TOutputReg		Outputs;		// Состояние дискретных выходов
	TDIN			dinStop;		// Дискретный вход "стоп"
	TDIN			dinClose;		// Дискретный вход "закрыть"
	TDIN			dinOpen;		// Дискретный вход "открыть"
	TDIN			dinResetAlarm;	// Дискретный вход "сброс аварий"
	TDIN			dinPredReady;	// Дискретный вход "пред продготовка ТУ"
	TDIN			dinMu;			// Дискретный вход "МУ"
	TDIN			dinDu;			// Дискретный вход "ДУ"
	Uns				*TypeVoltSignal;	// 24 / 220
	Uns				*TypeLogicSignal;// тип сигнала по логике нормально розомкнутый/замкнутый - 0/1
} TDigitalInterface;

// Маска дискретных входов
typedef union _TUInvert {
	Uns all;
	 struct {
		Uns Open:1;       	// 0     Открыть
		Uns Close:1;      	// 1     Закрыть
		Uns Stop:1;       	// 2     Стоп
		Uns Mu:1;			// 3	 Му
		Uns Du:1;			// 4	 Ду
		Uns Rsvd:11;      	// 5-15  Резерв
	 } bit;
} TUInvert;

// Маска дискретных выходов
typedef union _TSInvert {
	Uns all;
	 struct {
	 	Uns Dout0:1;		// 0 - Открыто
		Uns Dout1:1;		// 1 - Закрыто
		Uns Dout2:1;		// 2 - Муфта
		Uns Dout3:1;		// 3 - Авария
		Uns Dout4:1;		// 4 - Открывается/ Блок включен
		Uns Dout5:1;		// 5 - Закрывается/ КВО
		Uns Dout6:1;		// 6 - Му/Ду /	КВЗ
		Uns Dout7:1;		// 7 - Питание/ ---
		Uns Rsvd:8;      	// 8-15  Резерв
	 } bit;
} TSInvert;


void Comm_TuTsInit (TDigitalInterface *);
void Comm_TuTsUpdate (TDigitalInterface *);
void DIN_Update_On (TDIN *p,Uns *Type,Uns bit);
void DIN_Update_Off (TDIN *p,Uns *Type,Uns bit);


#ifdef __cplusplus
}
#endif // extern "C"

#endif	//_TU_TS
