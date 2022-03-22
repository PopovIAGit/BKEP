#ifndef DRIVE_
#define DRIVE_

//----------- ����������� ������������ ������ ------------------------------
#include "config.h"
//--------------------- ���������-------------------------------------------
#define LENGTH_TRQ		72	// ������ ������ ������������� ������� �������
//--------------------- ������� --------------------------------------------
//-------------------- ��������� -------------------------------------------

typedef struct {

	Uns *Inom;
	Uns *MaxTorqe;
	Uns *GearRatio;

} TCoreDrive;
//------------------- ���������� ���������� --------------------------------
extern Int
	drive1,  drive2,  drive3,	drive4,	drive5,
	drive6,  drive7,  drive8,  drive9,	drive10,
	drive11, drive12, drive13, drive14,	drive15,
	drive16, drive17, drive18, drive19, drive20,drive21,drive22,drive23,drive24,drive25,drive26,drive27,drive28,drive29;
//------------------- ��������� ������� ------------------------------------

void Core_Drive_Update(void);
void Drive_ReWrite_Update(void);

#endif
