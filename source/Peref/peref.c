/*======================================================================
��� �����:          peref.c
�����:
������ �����:
���� ���������:
��������:
������ ��������� ���������
======================================================================*/

#include "peref.h"
#include "g_Ram.h"
#include "comm.h"

#define SetCs()		SetCs2()
#define ClrCs()		ClrCs2()

//#define OFFSET_ISO 33580L
//#define GAIN_ISO 10000 

LgUns OFFSET_ISO = 32450;
Uns		GAIN_ISO = 10000;

// �������� ������� �������������� ����� � ��� �� ������� �����������
// � ��������������� ������������� R(x) = b*x + c, ��� x - ��, ��� ������ � ���,
// R - �������������, b � c - ������������, ������� �������� ����:
#define koef_B		_IQ15(-9.6774)
#define koef_C		_IQ15(6600.0)
#define ADC_to_R(inpADC)	_IQ15int(_IQ15mpy(koef_B, _IQ15(inpADC) ) + koef_C) 	// ��� ������ 3 �������

#define koef_B1		_IQ15(-10.204)
#define koef_C1		_IQ15(6928.57)
#define ADC_to_R1(inpADC1)	_IQ15int(_IQ15mpy(koef_B1, _IQ15(inpADC1) ) + koef_C1)	// ��� ������ 2 �������

TPeref	g_Peref;
Uns		loadAngleSwitch = 0;

//---------------------------------------------------
void Peref_Init(TPeref *p)
{
	//g_Ram.ramGroup.POSITION 	= 0;
	//g_Ram.ramGroup.TEMP_SENS 	= 0;
	//g_Ram.ramGroup.SENSORS.all 	= 0;
	//g_Ram.ramGroup.COMMAND.all	= 0;

	// ----������������ ����������-----------------------
	//Peref_LedsInit(&p->leds, Prd10HZ);
	//ADT75_Init(&p->tempSensor);
	//��������� ��������
	//Peref_EncoderInit(&p->position);
}
//---------------------------------------------------
void Peref_18kHzCalc(TPeref *p) // 18 ���
{

}

//---------------------------------------------------
// ��������� ��������
void Peref_50HzCalc(TPeref *p)	// 50 ��
{
	//Peref_TenControl(p);
}

void Peref_10HzCalc(TPeref *p)	// 10 ��
{

}

//---------------------------------------------------
void Peref_TenInit(TTen *p, Uns freq, Uns tenTime, Uns tenRate) 
{
	//p->timerTen = 0;
	//p->timerTenTimeOut = freq*tenTime;
	//p->tenEnableTime = (Uns)(p->timerTenTimeOut * tenRate)/100;
}
//---------------------------------------------------
void Peref_TenControl(void) // 10 ��
{
	// ������� ��� ���������� ���������� ��� ��������� ���� ���� ��������
	//if (g_Ram.ramGroup.COMMAND.bit.ten==1) OFF_TEN=1;
	//else if (g_Ram.ramGroup.COMMAND.bit.ten==0) OFF_TEN=0;

	/*if (!g_Ram.ramGroup.TEN_ENABLE)		// ���� ��� ��������
	{
		if (!g_Comm.digitInterface.Outputs.bit.Dout5)		// ���� ��� �������
			g_Comm.digitInterface.Outputs.bit.Dout5 = 1; 	// �� ��������� ���
		if (!shiftReg2.data.bit.ten)		// ���� ��� �������
			shiftReg2.data.bit.ten = 1; 	// �� ��������� ���
		if (!shiftReg2.data.bit.ten_bubi)	// � ������� �� �������
			shiftReg2.data.bit.ten_bubi = 1;
		return;								
	}

	if (g_Peref.tempSensor.error == true)
	{
		g_Comm.digitInterface.Outputs.bit.Dout5 = 0;
		shiftReg2.data.bit.ten = 0;
		shiftReg2.data.bit.ten_bubi = 0;
		return;
	}
	// ���� ��� ������ �� ��������� ������������� ����
	//Peref_TenRateCalc(p);					// ��������� ������������� ���

	if (g_Ram.ramGroupA.TEMPER_PU <= g_Ram.ramGroupC.TEN_ON_LEVEL)		// ���� ��� �������
	{
		g_Comm.digitInterface.Outputs.bit.Dout5 = 0;
		shiftReg2.data.bit.ten = 0;
		shiftReg2.data.bit.ten_bubi = 0;
	}
	else if (g_Ram.ramGroupA.TEMPER_PU >= g_Ram.ramGroupC.TEN_OFF_LEVEL)									// ����� (���� ��� ��������)
	{
		if (!g_Comm.digitInterface.Outputs.bit.Dout5)		// ���� ��� �������
			g_Comm.digitInterface.Outputs.bit.Dout5 = 1; 	// �� ��������� ���
		if (!shiftReg2.data.bit.ten)						// ���� ��� �������
			shiftReg2.data.bit.ten = 1; 					// �� ��������� ���
		if (!shiftReg2.data.bit.ten_bubi)					// ���� ��� �������
			shiftReg2.data.bit.ten_bubi = 1; 				// �� ��������� ���
	}*/
}
//---------------------------------------------------
void Peref_TenRateCalc(TTen *p)
{
	/*Int deltaT;
	static Uns tenRate, prevTenRate;

	deltaT = g_Ram.ramGroup.TEN_OFF_LEVEL - g_Ram.ramGroup.TEMPER_PU;
	
	if (deltaT <= 1)		// ����������� ���� 4 ����
		tenRate = 30;
	else if (deltaT <= 5)	// ����������� ���� 0 ����
		tenRate = 50;
	else if (deltaT <= 10)	// ����������� ���� -5 ����
		tenRate = 80;
	else 					// ����������� ���� -10 ����
		tenRate = 100;		

	if (tenRate != prevTenRate)
	{
		Peref_TenInit(p, Prd10HZ, g_Ram.ramGroup.TEN_PERIOD, tenRate);
		prevTenRate = tenRate;
	}*/
} 
//---------------------------------------------------
