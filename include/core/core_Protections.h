/*======================================================================
��� �����:          core_Protections.h
�����:				����� �.�.
������ �����:		1.00
���� ���������:		25.04.2014
��������:
������ ����� ��������������
======================================================================*/

#ifndef CORE_PROTECTIONS_
#define CORE_PROTECTIONS_

//----------- ����������� ������������ ������ ------------------------------
#include "config.h"
#include "core_ProtectionsAlarm.h"
#include "core_ProtectionI2T.h"
#include "core_ProtectionSHC.h"
#include "g_Structs.h"

//--------------------- ���������-------------------------------------------
#define PROTECT_SCALE (0.100 * Prd50HZ)

// ������ ����� g_Structs.h -> TProcessReg

#define NoMove_bit			0
#define PhOrd_bit			1
#define NoClose_bit			2
#define	NoOpen_bit			3
#define NoCalib_bit			4
#define Overway_bit			5
#define Mufta_bit			6
#define MuDuDef_bit			7

// ������ ����� g_Structs.h -> TNetReg
#define UvR_bit				0
#define UvS_bit				1
#define UvT_bit				2
#define OvR_bit				3
#define OvS_bit				4
#define OvT_bit				5
#define VSk_bit				7
#define BvR_bit				8
#define BvS_bit				9
#define BvT_bit				10
#define OvR_max_bit			11
#define OvS_max_bit			12
#define OvT_max_bit			13


// ������ ����� g_Structs.h -> TLoadReg
#define PhlU_bit			0
#define PhlV_bit			1
#define PhlW_bit			2
#define I2t_bit				3
#define ShCU_bit			4
#define ShCV_bit			5
#define ShCW_bit			6
#define ISkew_bit			11

// ������ ����� g_Structs.h -> TDeviceReg
#define PosSens_bit			0
#define Memory1_bit			1
#define Memory2_bit			2
#define Rtc_bit				3
#define TSens_bit			4
#define Th_BCP_bit			5
#define Tl_BCP_bit			6
#define Th_BCD_bit			7
#define Tl_BCD_bit			8

//--------------------- ������� --------------------------------------------
#define PRT_CFG_SET(CanBeReseted, Level, Bit, Hyst) \
	((CanBeReseted<<1)|(Level<<3)|(Bit<<4)|(Hyst<<8))
//-------------------- ��������� -------------------------------------------

typedef struct {
	// ����� --------------------------------------------------
	// -------- ����������� �������� ------------
		TAlarmElem			NoMove;				// ��� �������� - ������
	// -------- ����������� ���� ----------------
		TAlarmElem 			underVoltageR;		// ��������� ���������� R  - ������
		TAlarmElem 			underVoltageS;		// ��������� ���������� S  - ������
		TAlarmElem 			underVoltageT;		// ��������� ���������� T  - ������
		TAlarmElem 			underDefVoltageR;	// ��������� ���������� R  - �������������
		TAlarmElem 			underDefVoltageS;	// ��������� ���������� S  - �������������
		TAlarmElem 			underDefVoltageT;	// ��������� ���������� T  - �������������
		TAlarmElem 			overVoltageR;		// ���������� ���������� R - ������
		TAlarmElem 			overVoltageS;		// ���������� ���������� S - ������
		TAlarmElem 			overVoltageT;		// ���������� ���������� T - ������
		TAlarmElem 			overDefVoltageR;	// ���������� ���������� R - �������������
		TAlarmElem 			overDefVoltageS;	// ���������� ���������� S - �������������
		TAlarmElem 			overDefVoltageT;	// ���������� ���������� T - �������������
		TAlarmElem 			overMax_VoltageR;	// ���������� ���������� R �� 47% - ������
		TAlarmElem 			overMax_VoltageS;	// ���������� ���������� S �� 47% - ������
		TAlarmElem 			overMax_VoltageT;	// ���������� ���������� T �� 47% - ������
		TAlarmElem			voltSkew;			// ���������� ���������� ������� ���� - �������������
		TAlarmElem 			breakVoltR;			// ����� ���� R - ������
		TAlarmElem 			breakVoltS;			// ����� ���� S - ������
		TAlarmElem 			breakVoltT;			// ����� ���� T - ������
	// -------- ����������� �������� ------------
		TAlarmElem			breakCurrU;			// ����� ���� U	- �������������
		TAlarmElem			breakCurrV;			// ����� ���� V - �������������
		TAlarmElem			breakCurrW;			// ����� ���� W - �������������
		TAlarmElem			currSkew;			// ���������� ���� - �������������
		TAlarmI2T 			I2t;				// �����-������� ���������� - ������
		TAlarmSHC			ShC_U;				// �������� ��������� ���� U - ������
		TAlarmSHC			ShC_V;				// �������� ��������� ���� V - ������
		TAlarmSHC			ShC_W;				// �������� ��������� ���� W - ������
	// -------- ����������� ���������� ----------
		TAlarmElem			overHeatBCP;			// ��������	��� - ������
		TAlarmElem			underColdBCP; 			// �������������� ��� - �������������
		TAlarmElem			overHeatBCD;			// �������� ��� - �������������
		TAlarmElem			underColdBCD; 			// �������������� ��� - �������������
	// ������ ---------------------------------------------------------
		TFltUnion			outFaults;			// ������
		TFltUnion			outDefects;			// �������������
		Uns					ShcTmpState;		// ������������� ���������� ��� ����������� ��
		Uns					registerBrCurr;		// ������� ������ ������ ��� ���������
		Uns					registerBrVolt;		// ������� ������ ������ ��� �������
		Uns 				MuffFlag;			// ���� �����
		Uns				NoMoveFlag;
		Bool				ShcReset;			// ���� �� �������� ������������ ��
		Uns				FaultDelay;			// ����� ����� ������������� ��� ��������� �����

}TCoreProtections;

//------------------- ���������� ���������� --------------------------------
//------------------- ��������� ������� ------------------------------------

void Core_ProtectionsInit(TCoreProtections *);		// ������������� ������ �����
void Core_ProtectionsUpdate(TCoreProtections *);	// ������� ������� �� ������������ �����
void Core_ProtectionsReset(TCoreProtections *);		// ����� ����� ��� ��������� �����
void Core_ProtectionsClear(TCoreProtections *);		// ����� ���� �����
void Core_ProtectionsEnable(TCoreProtections *);	// ���������/���������� �����
void Core_DevProc_FaultIndic(TCoreProtections *);	// ��������� ������ ���������� � ���������������� ��������
void EngPhOrdPrt(TCoreProtections *);				// �������� ����������� ��� ���������

#endif
