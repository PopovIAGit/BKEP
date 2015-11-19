/*
 * core_DisplayFaults.h
 *
 *  Created on: 14 ���. 2015 �.
 *      Author: PopovIA
 */

#ifndef CORE_DISPLAYFAULTS_H_
#define CORE_DISPLAYFAULTS_H_
//----------- ����������� ������������ ������ ------------------------------
#include "peref.h"
//--------------------- ���������-------------------------------------------
#define DISPL_FAULT_TIME		(Uint16)(1.000 * Prd10HZ)


//--------------------- ������� --------------------------------------------
//-------------------- ��������� -------------------------------------------

typedef struct _TCoreDislpayFaults{

	//-----����������� ������--------------
		Uns 				DisplFaulstTimer; 	// ������ ��� ����������� ������� ������
		Uns					DisplFault;			// ������� ������������ ������
		Uns 				TestData;
		Uns					DisplFaultFlag;		// ���� ���������� ���������� ������
		TFltUnion			DisplFaultUnion;	// ��������� ��� ����������� ������

}TCoreDislpayFaults;

//------------------- ���������� ���������� --------------------------------
//------------------- ��������� ������� ------------------------------------

void Core_DisplayFaultsUpdate(TCoreDislpayFaults *);
void Core_DisplayFaultsInit(TCoreDislpayFaults *);

#endif /* CORE_DISPLAYFAULTS_H_ */
