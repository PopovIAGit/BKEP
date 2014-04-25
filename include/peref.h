/*======================================================================
��� �����:          peref.h
�����:
������ �����:
���� ���������:
�������������:
��������:
======================================================================*/

#ifndef PEREF_
#define PEREF_

//----------- ����������� ������������ ������ -----------------------------
#include "std.h"
#include "config.h"
#include "g_Ram.h"
#include "peref_ApFilter1.h"  		// PIA 03.04.14
#include "peref_ApFilter3.h"  		// PIA 04.04.14
#include "peref_SensObserver.h" 	// PIA 07.04.14
#include "peref_SinObserver.h"		// PIA 08.04.14
#include "peref_Calibs.h"			// PIA 14.04.14
#include "peref_ContactorControl.h"	// PIA 17.04.14
//#include "peref_DisplayDrv.h"
//#include "peref_LedsDrv.h"
//#include "peref_TempObserver.h"
//#include "peref_Clock.h"
//#include "peref_SDModDrv.h"
//#include "peref_EncoderDrv.h"
//#include "peref_TempDrv.h"
//#include "peref_Dac7513Drv.h"
//
#ifdef __cplusplus
extern "C" {
#endif

//-------------------- ��������� -------------------------------------------
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
	TSensObserver	sensObserver;		// ��������������� �������� � ��������
	TSinObserver	sinObserver;		// ���������� RMS
	TPhaseOrder		phaseOrder; 		// ����������� ��� ����
	//------
	APFILTER1 Phifltr;					// ������ ����� ��
	APFILTER1 Umfltr;					// ������ �������� ����������
	APFILTER3 Imfltr;					// ������ �������� ����

	TPerefPosition Position;			// ���������� ������� ��������� � ������ ��������
	TContactorControl ContactorControl;	// ���������� ������������

	Uns Umid;
	Uns Imid;
	Uns AngleUI;
} TPeref;

//------------------- ��������� ������� ------------------------------------
void Peref_Init(TPeref *);
void Peref_18kHzCalc(TPeref *);
void Peref_50HzCalc (TPeref *);
void Peref_10HzCalc (TPeref *);

//------------------- ���������� ���������� --------------------------------
extern	TPeref	g_Peref;

#ifdef __cplusplus
}
#endif // extern "C"

#endif
