/*======================================================================
��� �����:          core_TorqueObserver.h
�����:				����� �.�.
������ �����:		1.00
���� ���������:		16.04.2014
��������:
������ ���������� ���������
======================================================================*/

#ifndef TORQUE_OBSERVER_
#define TORQUE_OBSERVER_

//----------- ����������� ������������ ������ ------------------------------
#include "config.h"
#include "peref_ApFilter3.h"
//--------------------- ���������-------------------------------------------
#define TORQ_MIN_PR		10	// ����������� �������� � ���������
#define TORQ_MAX_PR		110	// ������������ �������� � ���������
#define TQ_VOLT_MIN		120	// ����������� ���������� ��� ������� �������
#define TQ_VOLT_MAX		260	// ������������ ���������� ��� ������� �������
//--------------------- ������� --------------------------------------------
//-------------------- ��������� -------------------------------------------

// ��������� ��� ������ � �����
typedef struct {
	Int X;
	Int Y;
	Int Z;
} TCubPoint;

#define CUB_COUNT1		4
#define CUB_COUNT2		5
typedef struct {
	Int Data[CUB_COUNT1][CUB_COUNT2];
} TCubArray;

typedef struct {
	Int *X_Value;
	Int *X_Array;
	Int *Y_Value;
	Int *Y_Array;
} TCubConfig;

typedef struct {
	TCubPoint  Input;
	Int        Output;
	Uns        Num1;
	Uns        Num2;
	Int        PtR;
	Int        PtT;
	Int       *InputX;
	Int       *InputY;
	TCubPoint  Min;
	TCubPoint  Max;
	TCubPoint  Points[CUB_COUNT1][CUB_COUNT2];
} TCubStr;

// ��������� ��� ������� ������
typedef struct {
	Bool    	ObsEnable;		// ���������� �������
	Uns			*TransCurr;		// ��� �������� ����� �������������
	Uns  		TorqueMax;		// ������������ ������
	Uns			*TorqueSetPr;	// ������� �� ������ � ���������
	Uns 		*Imidpr;		// ������ �� �������� ���� � % �� In
	Uns     	Indication;		// ��������� �������� �������
	Uns			Corr40Trq;		// ����������� ������������� ��������� ������� ��� ������� ������ 40%
	Uns			Corr60Trq;		//	-//- 60%
	Uns			Corr80Trq;		//	-//- 80%
	Uns			Corr110Trq;		// 	-//- 110%
	APFILTER3	Trqfltr;		// ������ ��� ��������
	TCubStr 	Cub1;			// ��� �� ���� ��
	TCubStr 	Cub2;			// ��� �� ����
	TCubConfig 	TqCurr;			// ��������� ��� ���� �� ����
	TCubConfig 	TqAngUI;		// ��������� ��� ���� �� ���� ��
} TTorqObs;


//------------------- ���������� ���������� --------------------------------
//------------------- ��������� ������� ------------------------------------
void CubInit(TCubStr *, TCubConfig *Cfg);
void CubRefresh(TCubStr *, TCubArray *Array);
void CubCalc(TCubStr *);
void Core_TorqueInit(TTorqObs *);
void Core_TorqueCalc(TTorqObs *);
#endif
