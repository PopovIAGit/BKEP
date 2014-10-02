#ifndef DRIVE_
#define DRIVE_

//----------- ����������� ������������ ������ ------------------------------
#include "config.h"
//--------------------- ���������-------------------------------------------
#define LENGTH_TRQ		60	// ������ ������ ������������� ������� �������
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
	drive16, drive17, drive18, drive19, drive20;
//------------------- ��������� ������� ------------------------------------

void Core_Drive_Update(void);

#endif
