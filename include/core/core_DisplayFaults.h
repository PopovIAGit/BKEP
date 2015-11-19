/*
 * core_DisplayFaults.h
 *
 *  Created on: 14 окт. 2015 г.
 *      Author: PopovIA
 */

#ifndef CORE_DISPLAYFAULTS_H_
#define CORE_DISPLAYFAULTS_H_
//----------- Подключение заголовочных файлов ------------------------------
#include "peref.h"
//--------------------- Константы-------------------------------------------
#define DISPL_FAULT_TIME		(Uint16)(1.000 * Prd10HZ)


//--------------------- Макросы --------------------------------------------
//-------------------- Структуры -------------------------------------------

typedef struct _TCoreDislpayFaults{

	//-----Отображение аварий--------------
		Uns 				DisplFaulstTimer; 	// таймер для отображения текущей аварии
		Uns					DisplFault;			// текущая отображаемая авария
		Uns 				TestData;
		Uns					DisplFaultFlag;		// флаг готовности считывания аварий
		TFltUnion			DisplFaultUnion;	// хранилище для отображения аварий

}TCoreDislpayFaults;

//------------------- Глобальные переменные --------------------------------
//------------------- Протатипы функций ------------------------------------

void Core_DisplayFaultsUpdate(TCoreDislpayFaults *);
void Core_DisplayFaultsInit(TCoreDislpayFaults *);

#endif /* CORE_DISPLAYFAULTS_H_ */
