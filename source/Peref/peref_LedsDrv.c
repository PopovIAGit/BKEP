/*======================================================================
��� �����:          peref_LedsDrv.c
�����:
������ �����:
���� ���������:
��������:
======================================================================*/
#include "peref_LedsDrv.h"
#include "core.h"

void LedTurnOnOff(pLedParams ledParams, Uns led)
{
	if (ledParams->isBlink==0)
	{
		//led = 0;
		ledParams->status = 0;//!led;
	}
	else if (++ledParams->blinkTimer > ledParams->timeOfBlink)
	{
		if(ledParams->timeOfBlink==0) ledParams->status = 0;
		else ledParams->status = !led;
		//led = !led;
		ledParams->blinkTimer = 0;
	}
}

//--------------------------------------------------------
void Peref_LedsInit(pLeds p, Uns freq)
{
 	p->ledCntr.blinkPRD 	= 1;

	p->ledOpen.blinkPRD 		= Prd10HZ;
	p->ledClose.blinkPRD 		= Prd10HZ;
	p->ledFault.blinkPRD 		= Prd10HZ;
	p->ledDefect.blinkPRD 		= Prd10HZ;
	p->ledMufta.blinkPRD 		= Prd10HZ;
	p->ledConnect.blinkPRD 		= 1;

	p->ledCntr.isBlink 			= 1;
	p->ledConnect.isBlink 		= 1;
	p->ledBluetooth.isBlink		= 0;

	p->ledCntr.timeOfBlink 		= freq/p->ledCntr.blinkPRD;			//������ ����������
	p->ledOpen.timeOfBlink 		= freq/p->ledOpen.blinkPRD;			//��������� �������
	p->ledClose.timeOfBlink 	= freq/p->ledClose.blinkPRD;		//��������� �������
	p->ledFault.timeOfBlink 	= freq/p->ledFault.blinkPRD;		//������
	p->ledDefect.timeOfBlink 	= freq/p->ledDefect.blinkPRD;		//�������������
	p->ledMufta.timeOfBlink 	= freq/p->ledMufta.blinkPRD;		//�����
	p->ledBluetooth.timeOfBlink	= 0;								//bluetooth
	p->ledConnect.timeOfBlink 	= freq/p->ledConnect.blinkPRD;		//����
	p->ledConnect.timeOfBlink	= 30;

	//p->pStatus = &g_Core.Status.all;
	p->pStatus = &g_Core.TestStatus.all;

}
//--------------------------------------------------------
void Peref_LedsUpdate(pLeds p)
{

	Uns tmp=0;
	Uns i=0;
	Uns BlinkConnect=0;

	if (g_Comm.mbAsu.Serial.RsState==0) BlinkConnect++;
	if (g_Comm.mbBkp.Frame.ConnFlag==1) BlinkConnect++;
	//if (g_Comm.mbShn.Serial.RsState==0) BlinkConnect++;

	p->ledConnect.timeOfBlink = 20;
	if (BlinkConnect==0) p->ledConnect.timeOfBlink = 20;
	else if (BlinkConnect==1) p->ledConnect.timeOfBlink = 10;
	else if (BlinkConnect==2) p->ledConnect.timeOfBlink = 5;
	else if (BlinkConnect==3) p->ledConnect.timeOfBlink = 1;

	//-------�������� ��������� ����������-----------------------------
	LedTurnOnOff(&p->ledCntr, p->ledCntr.status);
	WORK_DSP_LED = p->ledCntr.status;

	//-------�������� ��������� ����������-----------------------------
	LedTurnOnOff(&p->ledConnect, p->ledConnect.status);
	LED_CONNECT = p->ledConnect.status;
	p->leds.bit.Connect = p->ledConnect.status;

	// ------������----------------------------------------
	//------------------------------------------------------------------

	p->ledOpen.isBlink  = (*p->pStatus & STATUS_OPENING) ? 1: 0;// ������ - "�����������"? �� - Led "�������". ��� - Led �� "�������"
	p->ledClose.isBlink = (*p->pStatus & STATUS_CLOSING) ? 1: 0;// ������ - "�����������"? �� - Led "�������". ��� - Led �� "�������"

	//------------------------------------------------------------------
	if ((*p->pStatus & STATUS_OPENED)||(*p->pStatus & STATUS_OPENING))	// ���� ������ - "�������/�����������"
	{
		LedTurnOnOff(&p->ledOpen, p->leds.bit.Open);	// �������� ���������
		p->leds.bit.Open = p->ledOpen.status;
	}
	else 												// ���� ������ �� "�������/�����������"
		p->leds.all |= LED_OPEN_MASK;					// ����� ���������

	// ------�������/�����������----------------------------------------
	if ((*p->pStatus & STATUS_CLOSED)||(*p->pStatus & STATUS_CLOSING))	// ���� ������ - "�������/�����������"
	{
		LedTurnOnOff(&p->ledClose, p->leds.bit.Close);	// �������� ���������
		p->leds.bit.Close = p->ledClose.status;
	}
	else 												// ���� ������ �� "�������/�����������"
		p->leds.all |= LED_CLOSE_MASK;					// ����� ���������

	// ------�����----------------------------------------
	if (*p->pStatus & STATUS_MUFTA)						// ���� ������ "�����"
	{
		LedTurnOnOff(&p->ledMufta, p->leds.bit.Mufta);	// �������� ���������
		p->leds.bit.Mufta = p->ledMufta.status;
	}
	else 												// ���� ������ �� "�����"
		p->leds.all |= LED_MUFTA_MASK;					// ����� ���������

	// ------������----------------------------------------
	if (*p->pStatus & STATUS_FAULT)// ���� ������
	{
		LedTurnOnOff(&p->ledFault, p->leds.bit.Fault);// �������� ���������
		p->leds.bit.Fault = p->ledFault.status;
	}
	else 											// ���� ������ �� "������"
		p->leds.all |= LED_FAULT_MASK;				// ����� ���������

	// ------������----------------------------------------
	if (*p->pStatus & STATUS_DEFECT)// ���� �������������
	{
		LedTurnOnOff(&p->ledDefect, p->leds.bit.Defect);// �������� ���������
		p->leds.bit.Defect = p->ledDefect.status;
	}
	else 											// ���� ������ ��� "�������������"
		p->leds.all |= LED_DEFECT_MASK;				// ����� ���������

	LED_OPEN	= p->leds.bit.Open;		DELAY_US(1);
	LED_MUFTA	= p->leds.bit.Mufta;	DELAY_US(1);
	LED_DEFECT	= p->leds.bit.Defect;	DELAY_US(1);
	LED_FAULT	= p->leds.bit.Fault;	DELAY_US(1);
	LED_CLOSE 	= p->leds.bit.Close;	DELAY_US(1);

	if ((!(*p->pStatus & STATUS_OPENING)) && (!(*p->pStatus & STATUS_CLOSING)) && (!(*p->pStatus & STATUS_OPENED)) && (!(*p->pStatus & STATUS_CLOSED)))
	{
		LED_NOOPEN = 0;
		DELAY_US(1);
		LED_NOCLOSE = 0;
		DELAY_US(1);
	} else {
		LED_NOOPEN = 1;
		DELAY_US(1);
		LED_NOCLOSE = 1;
		DELAY_US(1);
	}

	//ENABLE_BLUETOOTH = p->leds.bit.Bluetooth;

	g_Ram.ramGroupH.BkpIndication = 0;

	tmp = !p->leds.bit.Fault;		g_Ram.ramGroupH.BkpIndication = tmp;
	tmp = !p->leds.bit.Connect;		g_Ram.ramGroupH.BkpIndication |=(tmp<<1)&0x2;
	tmp = !p->leds.bit.Defect;		g_Ram.ramGroupH.BkpIndication |=(tmp<<2)&0x4;
	tmp = !p->leds.bit.Mufta;		g_Ram.ramGroupH.BkpIndication |=(tmp<<3)&0x8;
	tmp = !p->leds.bit.Open;		g_Ram.ramGroupH.BkpIndication |=(tmp<<4)&0x10;
	tmp = !p->leds.bit.Close;		g_Ram.ramGroupH.BkpIndication |=(tmp<<5)&0x20;


}
//--------------------------------------------------------





