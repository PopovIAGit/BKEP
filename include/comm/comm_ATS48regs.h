/*======================================================================
Имя файла:          comm_ATS48regs.h
Автор:
Версия файла:
Дата изменения:
Описание:
Модуль ModBus
======================================================================*/
#ifndef __ATS48_REGS__
#define __ATS48_REGS__

#define ATS48_STATUS_REG				458
#define ATS48_EX_STATUS_REG				459
#define ATS48_EX2_STATUS_REG			460
#define ATS48_CONTROL_REG				400
#define ATS48_BIG_CONTROL_REG			402
#define ATS48_NOM_CURR					4026
#define ATS48_CURR_LIMIT				4039
#define ATS48_ACC_TIME					4043
#define ATS48_INIT_TORQ					4037
#define ATS48_STOP_TYPE					4029
#define ATS48_INT_TORQ					4041
#define ATS48_INJ_TIME					4042
#define ATS48_START_TORQ				4028

typedef union {
	Uint16 all;
	struct {
		Uint16 ReadyToSwitchOn:1;		// 0
		Uint16 SwichedOn:1;				// 1
		Uint16 OperationEnabled:1;		// 2
		Uint16 Malfunction:1;			// 3
		Uint16 VoltageDisabled:1;		// 4
		Uint16 QuickStopDisable:1;		// 5
		Uint16 SwitchOnDisable:1;		// 6
		Uint16 AlarmPresent:1;			// 7
		Uint16 Rsvd1:1;					// 8
		Uint16 ForcedLocalDiable:1;	// 9
		Uint16 Rsvd:6;						// 10-15
	} bit;
} TATS48_StatusReg;

typedef union {
	Uint16 all;
	struct {
		Uint16 WriteParamAuth:1;		// 0
		Uint16 ParConsCheck:1;			// 1
		Uint16 StartResetAuth:1;		// 2
		Uint16 MotorPreheating:1;		// 3
		Uint16 MotorRunning:1;			// 4
		Uint16 BrakingActive:1;			// 5
		Uint16 ContOperation:1;			// 6
		Uint16 ThermalOverload:1;		// 7
		Uint16 Rsvd1:1;					// 8
		Uint16 Accelerating:1;			// 9
		Uint16 Decelerating:1;			// 10
		Uint16 CurrentLimit:1;			// 11
		Uint16 TorqueLimit:1;			// 12
		Uint16 ActiveMode:2;				// 13-14
		Uint16 Rsvd:1;						// 15
	} bit;
} TATS48_ExStatusReg;

typedef union {
	Uint16 all;
	struct {
		Uint16 Rsvd1:6;					// 0-5
		Uint16 CurrOverload:1;			// 6
		Uint16 Rsvd3:3;					// 7-9
		Uint16 Underload:1;				// 10
		Uint16 PtcProbe:1;				// 11
		Uint16 UseSecondCfg:1;			// 12
		Uint16 StartingDelay:1;			// 13
		Uint16 CascadeOp:1;				// 14
		Uint16 Rsvd:1;						// 15
	} bit;
} TATS48_Ex2StatusReg;

typedef union {
	Uint16 all;
	struct {
		Uint16 SwitchOn:1;				// 0
		Uint16 EnableVoltage:1;			// 1
		Uint16 DisableQuickStop:1;		// 2
		Uint16 EnableOp:1;				// 3
		Uint16 Rsvd1:3;					// 4-6
		Uint16 ResetFaults:1;			// 7
		Uint16 Rsvd2:3;					// 8-10
		Uint16 CascadeFunc:1;			// 11
		Uint16 StopRequest:1;			// 12
		Uint16 BrakedStopReq:1;			// 13
		Uint16 DecelStopReq:1;			// 14
		Uint16 Rsvd:1;						// 15
	} bit;
} TATS48_ControlReg;

typedef struct {
	TATS48_StatusReg 		Status;
	TATS48_ExStatusReg		ExStatus;
	TATS48_Ex2StatusReg		Ex2Status;
	TATS48_ControlReg		Control;
	Uint16					NomCurr;
	Uint16					CurrLimit;
	Uint16					AccTime;
	Uint16					InitTorq;
	Uint16					StopType;
	Uint16					IntTorq;
	Uint16					InjTime;
	Uint16					StartTorq;
} TATS48_Regs;

#endif

