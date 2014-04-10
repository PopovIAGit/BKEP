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
//#include "comm_DigitalInputs.h"
//#include "comm_LocalControl.h"
#include "comm_ModbusRtu.h"
#include "comm_SerialComm.h"


#ifdef __cplusplus
extern "C" {
#endif

// �������� ������� ����������
#define CMD_SRC_MPU			BIT13	// ������� ���� ����������
#define CMD_SRC_DIGITAL		BIT14	// ���������� ���������
#define CMD_SRC_SERIAL		BIT15	// ���������������� ���������
//#define CMD_SRC_ANALOG		BIT15	// ��������� ���������

//--------------------------------------------------------
//================== ������������ ========================
//--------------------------------------------------------

typedef struct {
	//TCommMPU			localControl;	// ����� �������� ����������
	// ---

	// ---
	//Uns					outputCmdReg;	// �����: ��������� ������� ������
	// ---
	Uns					outputCmdSrc;	// �����: �������� ������
	TMbPort  			mbBkp;				// ModBus - ���
	TMbPort  			mbAsu;				// ModBus - ���
	TMbPort  			mbShn;				// ModBus - ���
	TMbPort  			mbBt;				// ModBus - Bluetooth
} TComm;
//--------------------------------------------------------

void Comm_Init(TComm *);
void Comm_Update(TComm *);
void UpdateTUTS_Test(TComm *p);
Uns DigitCmdModeUpdate (Uns *);

extern TComm	g_Comm;

#ifdef __cplusplus
}
#endif // extern "C"

#endif
