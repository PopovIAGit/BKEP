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


//--------------------- ���������-------------------------------------------
//--------------------- ������� --------------------------------------------
#define ADC_CONV(Value, Gain, Offset) 	(((LgInt)(Value - Offset) * Gain) >> 13)
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
	LgInt				URinp;			// ����: ���������� R
	LgInt 				USinp;			// ����: ���������� S
	LgInt 				UTinp;			// ����: ���������� T
	LgInt				IUinp;			// ����: ��� U
	LgInt 				IVinp;			// ����: ��� V
	LgInt	 			IWinp;			// ����: ��� W
	LgInt				URout;			// �����: ���������� R
	LgInt 				USout;			// �����: ���������� S
	LgInt 				UTout;			// �����: ���������� T
	LgInt				IUout;			// �����: ��� U
	LgInt 				IVout;			// �����: ��� V
	LgInt 				IWout;			// �����: ��� W
} TSensObserver;

//------------------- ���������� ���������� -------------------------------------
//------------------- ��������� ������� -----------------------------------------
void Peref_SensObserverInit(TSensObserver *p);
void SetOffsets(TSensObserver *p);
void Peref_SensObserverUpdate(TSensObserver *p);


#endif
