/*======================================================================
��� �����:          comm.h
�����:
������ �����:
���� ���������:
��������:
�������� �������� � ������� ������ ������������
========================================================================*/

#ifndef COMM_
#define COMM_

#include "std.h"
//#include "config.h"
#include "comm_DigitalInputs.h"
#include "comm_LocalControl.h"
#include "comm_ModbusRtu.h"
#include "comm_SerialComm.h"
#include "comm_BetweenBlock.h"
#include "comm_BluetoothWT41.h"
#include "comm_TuTs.h"
#include "comm_ATS48regs.h"


#ifdef __cplusplus
extern "C" {
#endif

#define ICREMENT_DIV 25		// ������������� �������� �������� �������
#define DECREMENT_DIV 10	// ������������� �������� ���������� �������
//--------------------------------------------------------
//================== ������������ ========================
//--------------------------------------------------------

typedef struct {

	Uint16 SHN_Connected;
	Uint16 SHN_Busy;
	Uint16 SHN_ReadFlag;
	Uint16 SHN_WriteFlag;
	Uint16 SHN_TaskFunc;
	Uint16 SHN_TaskAddr;
	Uint16 *SHN_TaskData;
	Uint16 SHN_TaskStage;
	Uint16 SHN_TaskEx;
	Uint16 SHN_Mode;
	Uint16 SHN_StopFlag;

} TShn;

typedef struct {
	TCommMPU			localControl;	// ����� �������� ����������
	//----
	TDigitalInput 		digitInput;		// �������� ����: ������������
	// ---
	TDigitalInterface	digitInterface;	// ���������� ���������: ���������������� � ��������������
	// ---
	Uns					outputCmdReg;	// �����: ��������� ������� ������
	// ---
	Uns					outputCmdSrc;	// �����: �������� ������
	// ---
	TMbBBPort  			mbBkp;			// ModBus - ���
	TMbPort  			mbAsu;			// ModBus - ���
	TMbPort  			mbShn;			// ModBus - ���
	TMbPort  			mbBt;			// ModBus - Bluetooth
	TATS48_Regs			SHN_Regs;		// �������� ��� ������ � ������������ �������
	TShn				Shn;
	TBluetoothPort		Bluetooth;		// Bluetooth
	TBKPData			BkpData;		// ��������� ������ ������� � ���

	Uns 				btn_reset_alarmFlag; // ���� ������� ������ ����� ������, ��� ��������� ������������� �������� ������
} TComm;
//--------------------------------------------------------

void Comm_Init(TComm *);
void Comm_Update(TComm *);
void Comm_50HzCalc(TComm *);
void Comm_CommandUpdate(TComm *p);
void UpdateTUTS_Test(TComm *p);
//Uns DigitCmdModeUpdate (Uns *);
void TekModbusParamsUpdate(void);

extern TComm	g_Comm;
extern Uint16 	SHN_Data[10];
#ifdef __cplusplus
}
#endif // extern "C"

#endif
