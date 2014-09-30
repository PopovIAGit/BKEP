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


#ifdef __cplusplus
extern "C" {
#endif

#define ICREMENT_DIV 25		// ������������� �������� �������� �������
#define DECREMENT_DIV 10	// ������������� �������� ���������� �������
//--------------------------------------------------------
//================== ������������ ========================
//--------------------------------------------------------

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
	TBluetoothPort		Bluetooth;		// Bluetooth
	TBKPData			BkpData;		// ��������� ������ ������� � ���
} TComm;
//--------------------------------------------------------

void Comm_Init(TComm *);
void Comm_Update(TComm *);
void Comm_50HzCalc(TComm *);
void UpdateTUTS_Test(TComm *p);
Uns DigitCmdModeUpdate (Uns *);
void TekModbusParamsUpdate(void);

extern TComm	g_Comm;

#ifdef __cplusplus
}
#endif // extern "C"

#endif
