/*======================================================================
��� �����:          peref_SensObserver.h
�����:
������ �����:
���� ���������:
��������:
������ ��������� ��������
�����: ������� � ��� � Uns
������: ������� � IQ24
======================================================================*/

#ifndef PEREF_SENS_OBSERVER_
#define PEREF_SENS_OBSERVER_

//----------- ����������� ������������ ������ -----------------------------

#include "config.h"
//--------------------- ���������-------------------------------------------
//--------------------- ������� --------------------------------------------
//#define ADC_CONV(Value, Gain, Offset) 	(((LgInt)(Value - Offset) * Gain) >> 13)
#define ADC_CONV(Value, Gain, Offset) 		(0.00001 * (int16)((Value) - (Offset)) * (Gain))
//#define ADC_CONV(Value, Gain, Offset) 	((LgInt)(Value - Offset) * Gain)
//-------------------- ��������� ------------------------------------------
// ��������� ��������
typedef struct {
	Uns				*p_UR_Mpy;		// �������������� ��� ���������� R
	Uns 			*p_US_Mpy;		// �������������� ��� ���������� S
	Uns 			*p_UT_Mpy;		// �������������� ��� ���������� T
	Uns				*p_UR_Offset;	// ����� ��� ���������� R
	Uns 			*p_US_Offset;	// ����� ��� ���������� S
	Uns 			*p_UT_Offset;	// ����� ��� ���������� T
	Uns				*p_IU_Mpy;		// �������������� ��� ���� U
	Uns 			*p_IV_Mpy;		// �������������� ��� ���� V
	Uns 			*p_IW_Mpy;		// �������������� ��� ���� W
	Uns				*p_IU_Offset;	// ����� ��� ���� U
	Uns 			*p_IV_Offset;	// ����� ��� ���� V
	Uns 			*p_IW_Offset;	// ����� ��� ���� W
} TSensorsParams;

// ��������� �������� ���������������
typedef struct {
	Uns				*p_UOpen_Mpy;			// �������������� ��� ���������� ������� Open
	Uns				*p_UOpen_Offset;		// ����� ��� ���������� ������� Open
	Uns				*p_UClose_Mpy;			// �������������� ��� ���������� ������� Close
	Uns				*p_UClose_Offset;		// ����� ��� ���������� ������� Close
	Uns				*p_UStopOpen_Mpy;		// �������������� ��� ���������� ������� StopOpen
	Uns				*p_UStopOpen_Offset;	// ����� ��� ���������� ������� StopOpen
	Uns				*p_UStopClose_Mpy;		// �������������� ��� ���������� ������� StopClose
	Uns				*p_UStopClose_Offset;	// ����� ��� ���������� ������� StopClose
	Uns				*p_UMu_Mpy;				// �������������� ��� ���������� ������� MU
	Uns				*p_UMu_Offset;			// ����� ��� ���������� ������� MU
	Uns				*p_UDu_Mpy;				// �������������� ��� ���������� ������� DU
	Uns				*p_UDu_Offset;			// ����� ��� ���������� ������� DU
	Uns				*p_UResetAlarm_Mpy;		// �������������� ��� ���������� ������� ResetAlarm
	Uns				*p_UResetAlarm_Offset;	// ����� ��� ���������� ������� ResetAlarm
} TDigSinalParams;

// ����� ���������� ��������
typedef enum{
	mManual     = 0,				// ����� ������ ���������
	mAutoOffset = 1,				// ����� ������������ offset
	mTest		= 2					// ����� ����
} TOffsetMode;

// �������� ��������� ��������� �������� ���� � ����������
typedef struct {
	TSensorsParams		parSensors;		// ��������: ��������� ��������
	TOffsetMode		    pOffsetMode;	// ��������: ������ ���������� ������ (Offset)
	float32				URinp;			// ����: ���������� R
	float32 			USinp;			// ����: ���������� S
	float32 			UTinp;			// ����: ���������� T
	float32				IUinp;			// ����: ��� U
	float32 			IVinp;			// ����: ��� V
	float32	 			IWinp;			// ����: ��� W
	float32				URout;			// �����: ���������� R
	float32 			USout;			// �����: ���������� S
	float32 			UTout;			// �����: ���������� T
	float32				IUout;			// �����: ��� U
	float32 			IVout;			// �����: ��� V
	float32 			IWout;			// �����: ��� W
} TSensObserver;

// �������� ��������� ��������� �������� ���� � ����������
typedef struct {
	TDigSinalParams		parSensors;		// ��������: ��������� ��������
	TOffsetMode		    pOffsetMode;	// ��������: ������ ���������� ������ (Offset)

	float32				UOpenInp;		// ����: ����������
	float32 			UCloseInp;		// ����: ����������
	float32 			UStopOpenInp;	// ����: ����������
	float32 			UStopCloseInp;	// ����: ����������
	float32 			UMuInp;			// ����: ����������
	float32 			UDuInp;			// ����: ����������
	float32 			UResetAlarmInp;	// ����: ����������

	float32				UOpenOut;		// ����: ����������
	float32 			UCloseOut;		// ����: ����������
	float32 			UStopOpenOut;	// ����: ����������
	float32 			UStopCloseOut;	// ����: ����������
	float32 			UMuOut;			// ����: ����������
	float32 			UDuOut;			// ����: ����������
	float32 			UResetAlarmOut;	// ����: ����������

} TSensTuObserver;

//------------------- ���������� ���������� -------------------------------------
//------------------- ��������� ������� -----------------------------------------
void Peref_SensObserverInit(TSensObserver *p);
void SetOffsets(TSensObserver *p);
void Peref_SensObserverUpdate(TSensObserver *p);
void Peref_SensTuObserverUpdate(TSensTuObserver *p);

void Peref_SensTuObserverInit(TSensTuObserver *p);


#endif
