/*======================================================================
��� �����:          peref.h
�����:
������ �����:
���� ���������:
�������������:      ��������� � ����������� IQmath
��������:
======================================================================*/

#ifndef PEREF_
#define PEREF_

#include "std.h"
#include "config.h"
#include "g_Ram.h"
#include "peref_ApFilter1.h"  	// PIA 03.04.14
#include "peref_ApFilter3.h"  	// PIA 04.04.14
#include "peref_SensObserver.h" // PIA 07.04.14
#include "peref_SinObserver.h"	// PIA 08.04.14
//#include "peref_DisplayDrv.h"
//#include "peref_Position.h"
//#include "peref_LedsDrv.h"
//#include "peref_TempObserver.h"
//#include "peref_Clock.h"
//#include "peref_SDModDrv.h"
//#include "peref_EncoderDrv.h"
//#include "peref_TempDrv.h"
//#include "peref_Dac7513Drv.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
	Uns all;
	struct {
   		Uns ten:1;			// 0     ��� ���.����
		Uns testIso:1;		// 1     ���� ��������
		Uns dsp_enb_bf:1;	// 2     ���������� ���� (����������� ������)
		Uns ten_bubi:1;		// 3	 ���������� ����� �� ����
		Uns reserv:12;		// 4-15	 ������
	} bit;
} TShiftReg2Data;

typedef struct
{
	Uns					input;			// ����: Uns
	LgUns				buffer[20];		// ��������� ������
	Uns					output;			// �����
} TSensor;

// ��������� ��� ���������� �������������� ��������� (���)
typedef struct
{
	Uns					timerTen;		// ������
	Uns					timerTenTimeOut;// ������ ����� ������ ����
	Uns					tenEnableTime;	// ���������� ��������� ����
} TTen;

//----------------------------------------------
// ��������� ��� ������ � ��������� ���������
typedef struct {
	// ������� U
	APFILTER3  URfltr;
	APFILTER3  USfltr;
	APFILTER3  UTfltr;
	// ������� I
	APFILTER3  IUfltr;
	APFILTER3  IVfltr;
	APFILTER3  IWfltr;
	// ------
	TSensObserver	sensObserver;	// ��������������� �������� � ��������
	TSinObserver	sinObserver;	// ���������� RMS
	TPhaseOrder		phaseOrder; 	// ����������� ��� ����

	//------
	APFILTER1 Phifltr;				// ������ ����� ��
	APFILTER1 Umfltr;				// ������ �������� ����������
	APFILTER1 Imfltr;				// ������ �������� ����
} TPeref;

void Peref_Init(TPeref *);
void Peref_18kHzCalc(TPeref *);
void Peref_50HzCalc (TPeref *);
void Peref_10HzCalc (TPeref *);
void Peref_TenInit(TTen *, Uns, Uns, Uns);
void Peref_TenControl(void);
void Peref_TenRateCalc(TTen *);
Uns CircleBufferCalc (TSensor *);

extern	TPeref	g_Peref;

#ifdef __cplusplus
}
#endif // extern "C"

#endif
