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
#include "g_InitHardware.h"
#include "g_Ram.h"
#include "peref_ApFilter1.h"  		// PIA 03.04.14
#include "peref_ApFilter3.h"  		// PIA 04.04.14
#include "peref_SensObserver.h" 	// PIA 07.04.14
#include "peref_SinObserver.h"		// PIA 08.04.14
#include "peref_Calibs.h"			// PIA 14.04.14
#include "peref_ContactorControl.h"	// PIA 17.04.14
//#include "peref_DisplayDrv.h"
#include "peref_LedsDrv.h"
#include "peref_Clock.h"
//#include "peref_TempObserver.h"
//#include "peref_SDModDrv.h"
//#include "peref_EncoderDrv.h"
#include "peref_ADT75Drv.h"
#include "peref_DacMCP4276Drv.h"
#include  "peref_RtcDS3231Drv.h"
//
#ifdef __cplusplus
extern "C" {
#endif

//-------------------- ��������� -------------------------------------------
// ��������� ��� ������ � ��������� ���������
typedef struct {
	//--- ������� U -----------------
	APFILTER3  			URfltr;
	APFILTER3  			USfltr;
	APFILTER3  			UTfltr;
	//--- ������� I -----------------
	APFILTER3  			IUfltr;
	APFILTER3  			IVfltr;
	APFILTER3  			IWfltr;
	//--- ������� U ��� �������������� -----------------
	APFILTER3  			UfltrOpen;
	APFILTER3  			UfltrClose;
	APFILTER3  			UfltrStop;
	APFILTER3  			UfltrMu;
	APFILTER3  			UfltrResetAlarm;
	APFILTER3  			UfltrReadyTU;
	APFILTER3  			UfltrDU;
	// ------------------------------
	TSensObserver		sensObserver;		// ��������������� �������� � ��������
	TSinObserver		sinObserver;		// ���������� RMS
	TPhaseOrder			phaseOrder; 		// ����������� ��� ����
	//-------------------------------
	APFILTER1 			Phifltr;			// ������ ����� ��
	APFILTER1 			Umfltr;				// ������ �������� ����������
	APFILTER3 			Imfltr;				// ������ �������� ����
	//-------------------------------
	TPerefPosition 		Position;			// ���������� ������� ��������� � ������ ��������
	TContactorControl 	ContactorControl;	// ���������� ������������
	//-------------------------------
	Uns 				Umid;
	Uns 				Imid;
	Uns 				AngleUI;
	//---------------------------
	TLeds				leds;
	//---------------------------
	MCP4726				Dac;					// ���
	ADT75				TSens;					// ������ �����������
	DS3231				Rtc;					// ����
	RTC_Obj				RtcData;
	//---------------------------
	TSensTuObserver		InDigSignalObserver;	// ��������������� �������� � ��������
	TSinSignalObserver  InDigSignal;			// ���������� RMS

} TPeref;

//------------------- ��������� ������� ------------------------------------
void Peref_Init(TPeref *);
void Peref_18kHzCalc(TPeref *);
void Peref_50HzCalc (TPeref *);
void Peref_10HzCalc (TPeref *);
void I2CDevUpdate(TPeref *);
void RTC_Control(void);

//------------------- ���������� ���������� --------------------------------
extern	TPeref	g_Peref;

#ifdef __cplusplus
}
#endif // extern "C"

#endif
