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
typedef void (*TCs_Func)(Byte);

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
	TCs_Func    CsFunc;						// ������� chip select'�

	Uns		Set11;
	Uns		Set12;
	Uns		Set21;
	Uns		Set22;
	Uns		Set31;
	Uns		Set32;
	Uns		Set41;
	Uns		Set42;
	Uns		Set51;
	Uns		Set52;
	Uns		Set61;
	Uns		Set62;
	Uns		Set71;
	Uns		Set72;
	Uns		Set81;
	Uns		Set82;

	Uns		Timer1;
	Uns		Timer2;
	Uns		Timer3;
	Uns		Timer4;
	Uns		Timer5;
	Uns		Timer6;
	Uns		Timer7;
	Uns		Timer8;

} TPeref_74hc595;

//------------------- ���������� ���������� --------------------------------
//------------------- ��������� ������� ------------------------------------

void Peref_74HC595Init(TPeref_74hc595 *);
#if !NEW_RAZ
void Peref_74HC595Update(TPeref_74hc595 *, TOutputReg);
#else
void Peref_74HC595UpdateTs(TPeref_74hc595 *, TOutputReg);
void Peref_74HC595UpdateLed(TPeref_74hc595 *, TLedReg *);
void Peref_74HC595UpdateDisp(TPeref_74hc595 *);
void Peref_74HC595UpdateDispRW(TPeref_74hc595 *, Uns set);
void Peref_74HC595UpdateDispEN(TPeref_74hc595 *, Uns set);
void Peref_74HC595UpdateDispRS(TPeref_74hc595 *, Uns set);
void Peref_74HC595UpdateDispSendData(TPeref_74hc595 *, Byte Data);
#endif

#endif /* PEREF_74HC595_H_ */
