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
#include "peref_Calibs.h"		// PIA 14.04.14
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
	APFILTER1  			URfltr;
	APFILTER1  			USfltr;
	APFILTER1  			UTfltr;
	APFILTER3  			UR3fltr;
	APFILTER3  			US3fltr;
	APFILTER3  			UT3fltr;
	//--- ������� I -----------------
	APFILTER1  			IUfltr;
	APFILTER1  			IVfltr;
	APFILTER1  			IWfltr;
	APFILTER3  			IU3fltr;
	APFILTER3  			IV3fltr;
	APFILTER3  			IW3fltr;
	//--- ������� U ��� �������������� -----------------
	APFILTER1  			UfltrOpen;
	APFILTER1  			UfltrClose;
	APFILTER1  			UfltrStop;
	APFILTER1  			UfltrMu;
	APFILTER1  			UfltrResetAlarm;
	APFILTER1  			UfltrReadyTU;
	APFILTER1  			UfltrDU;
	//-------------------------------
	//--- ������� U ��� �������������� -----------------
	APFILTER3  			U3fltrOpen;
	APFILTER3  			U3fltrClose;
	APFILTER3  			U3fltrStop;
	APFILTER3  			U3fltrMu;
	APFILTER3  			U3fltrResetAlarm;
	APFILTER3  			U3fltrReadyTU;
	APFILTER3  			U3fltrDU;
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
	Float 				Imid;
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
	Uns					TaktTuSensUpdate;
	TSinSignalObserver  InDigSignal;			// ���������� RMS
	//-----------------------------
	Uns Peref_StertDelayTimeout;


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
