/*
 * peref_74HC595.h
 *
 *  Created on: 15 ���. 2015 �.
 *      Author: PopovIA
 */

#ifndef PEREF_74HC595_H_
#define PEREF_74HC595_H_


//----------- ����������� ������������ ������ -----------------------------
//--------------------- ���������-------------------------------------------
#define ON_TS_RELE	1
#define OFF_TS_RELE	0
//--------------------- ������� --------------------------------------------
//-------------------- ��������� -------------------------------------------

// ��������� ���������� ��������
typedef void (*TCsFunc)(Byte);

// ������ ���������� ��������� �� ������������� 2 ��������� �� 3. 16 �� 24

typedef union _TShiftReg {
	Uns all;
	struct {

		Uns D_Out1:1;			// 1-1
		Uns D_Out2:1;			// 1-2
     	Uns D_Out3:1;    		// 2-1
     	Uns D_Out4:1;			// 2-2
    	Uns D_Out5:1;			// 3-1
     	Uns D_Out6:1;			// 3 2
     	Uns D_Out7:1;			// 4 1
		Uns D_Out8:1;			// 4 2
		Uns D_Out9:1;			// 5 1
		Uns D_Out10:1;			// 5 2
     	Uns D_Out11:1;    		// 6 1
     	Uns D_Out12:1;			// 6 2
    	Uns D_Out13:1;			// 7 1
     	Uns D_Out14:1;			// 7 2
     	Uns D_Out15:1;			// 8 1
		Uns D_Out16:1;			// 8 2

	} bit;
} TShiftReg;

// ��������� ��� ������ � �������� ���������
typedef struct TPeref_74hc595 {

	Byte  		SpiId;					// ������������� SPI ������ (SPIA, SPIB, ...)
	Uns   		SpiBaud;				// ������� ���������������
	TShiftReg  	ShiftReg;				// ������� ��������� ����
	Uns			Data;
	Uns			EmptyData;				// ���������� �������������� ���� � ������� ���������� � �������

	TCsFunc CsFunc;						// ������� chip select'�

} TPeref_74hc595;

//------------------- ���������� ���������� --------------------------------
//------------------- ��������� ������� ------------------------------------

void Peref_74HC595Init(TPeref_74hc595 *);
void Peref_74HC595Update(TPeref_74hc595 *, TOutputReg);

#endif /* PEREF_74HC595_H_ */
