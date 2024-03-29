/*======================================================================
��� �����:          peref_SinObserver.h
�����:				����� �.�.
������ �����:		1.0
���� ���������:		04.04.14
��������: ��������� ��������� ���� � ����������
����  - �������������� �������� ���� � ����������
����� - RMS �������� I � U, ����������, ���� ���
======================================================================*/

#ifndef PEREF_SIN_OBSERVER_
#define PEREF_SIN_OBSERVER_

//----------- ����������� ������������ ������ -----------------------------
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif
//--------------------- ���������------------------------------------------
#define DIRECT_RST	0
#define REVERS_TSR  1

#define PHO_ANGLE	30	//
#define MIN_ANGLE	120
#define MAX_ANGLE	250
#define BASE_ANGLE	360
#define RMS_SCALE	(LgUns)(1.11*32768/BASE_ANGLE)

//--------------------- ������� -------------------------------------------
//--------------------  ��������� ------------------------------------------
typedef struct
{
  Int 	Input;			// 	������� ���������� ��������
  Uns 	Output;			//	�������� ����������� ��������
  Uns 	Sign;			//	���� �������� �������
  Int 	Polarity;		//	����������
  Uns 	CurAngle;		//	���� ���� �������
  Uns   StepAngle;		// 	��� ��������� ����
  Uns   Counter;		// 	������� ��� ����� ������� ������� ���������
  LgUns Sum;			//	�������� ���������� ��������
} TSinPhaseObserver;

typedef struct
{
  Float Input;			// 	������� ���������� ��������
  Float Output;			//	�������� ����������� ��������
  Uns 	Sign;			//	���� �������� �������
  Int 	Polarity;		//	����������
  Uns 	CurAngle;		//	���� ���� �������
  Uns   StepAngle;		// 	��� ��������� ����
  Uns   Counter;		// 	������� ��� ����� ������� ������� ���������
  Float Sum;			//	�������� ���������� ��������
} TSinPhaseObserverFloat;

typedef struct
{
	Int Direction;
	Int TmpDir;
	Uns Timeout;
	Uns Timer;
	TSinPhaseObserverFloat 	*UR;
	TSinPhaseObserverFloat	*US;
	TSinPhaseObserverFloat	*UT;
} TPhaseOrder;

typedef struct
{
	TSinPhaseObserverFloat UR;
	TSinPhaseObserverFloat US;
	TSinPhaseObserverFloat UT;
	TSinPhaseObserverFloat IU;
	TSinPhaseObserverFloat IV;
	TSinPhaseObserverFloat IW;

}TSinObserver;

typedef struct
{
	TSinPhaseObserverFloat	sigOpen;
	TSinPhaseObserverFloat	sigClose;
	TSinPhaseObserverFloat	sigStopOpen;
	TSinPhaseObserverFloat	sigMU;
	TSinPhaseObserverFloat	sigResetAlarm;
	TSinPhaseObserverFloat	sigStopClose;
	TSinPhaseObserverFloat	sigDU;
}TSinSignalObserver;

//--------------------  ���������� ���������� ------------------------------
//--------------------  ��������� ������� ----------------------------------

void Peref_SinObserverInit(TSinPhaseObserver *p, Uns Freq);
void Peref_SinObserverUpdate(TSinPhaseObserver *p);
void Peref_PhaseOrderUpdate(TPhaseOrder *p);

void Peref_SinObserverInitFloat(TSinPhaseObserverFloat *p, Uns Freq);
void Peref_SinObserverUpdateFloat(TSinPhaseObserverFloat *p);



#ifdef __cplusplus
}
#endif // extern "C"

#endif
