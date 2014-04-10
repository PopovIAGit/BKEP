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
#include "comm_BetweenBlock.h"


#ifdef __cplusplus
extern "C" {
#endif

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
	TMbBBPort  			mbBkp;				// ModBus - ���
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
