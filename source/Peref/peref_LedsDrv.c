/*======================================================================
Имя файла:          peref_LedsDrv.c
Автор:
Версия файла:
Дата изменения:
Описание:
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

 	p->ledOpening.blinkPRD 		= Prd10HZ;
 	p->ledClosing.blinkPRD 		= Prd10HZ;
	p->ledOpen.blinkPRD 		= Prd10HZ;
	p->ledClose.blinkPRD 		= Prd10HZ;
	p->ledFault.blinkPRD 		= Prd10HZ;
	p->ledDefect.blinkPRD 		= Prd10HZ;
	p->ledMufta.blinkPRD 		= Prd10HZ;
	p->ledMuDu.blinkPRD 		= Prd10HZ;
	p->ledConnect.blinkPRD 		= 1;

	p->ledCntr.isBlink 			= 1;
	p->ledConnect.isBlink 		= 1;
	p->ledBluetooth.isBlink		= 0;

	p->ledOpening.timeOfBlink 		= freq/p->ledOpening.blinkPRD;			//состояние открыто
	p->ledClosing.timeOfBlink 		= freq/p->ledClosing.blinkPRD;			//состояние открыто

	p->ledMuDu.timeOfBlink 		= freq/p->ledMuDu.blinkPRD;			//состояние открыто

	p->ledCntr.timeOfBlink 		= freq/p->ledCntr.blinkPRD;			//работа процессора
	p->ledOpen.timeOfBlink 		= freq/p->ledOpen.blinkPRD;			//состояние открыто
	p->ledClose.timeOfBlink 	= freq/p->ledClose.blinkPRD;		//состояние закрыто
	p->ledFault.timeOfBlink 	= freq/p->ledFault.blinkPRD;		//авария
	p->ledDefect.timeOfBlink 	= freq/p->ledDefect.blinkPRD;		//неисправность
	p->ledMufta.timeOfBlink 	= freq/p->ledMufta.blinkPRD;		//муфта
	p->ledBluetooth.timeOfBlink	= 0;								//bluetooth
	p->ledConnect.timeOfBlink 	= freq/p->ledConnect.blinkPRD;		//сеть
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

	g_Ram.ramGroupH.BkpIndication = 0;

	if (g_Comm.mbAsu.Serial.RsState==0) BlinkConnect++;
	if (g_Comm.mbBkp.Frame.ConnFlag==1) BlinkConnect++;
	//if (g_Comm.mbShn.Serial.RsState==0) BlinkConnect++;

	p->ledConnect.timeOfBlink = 20;
	if (BlinkConnect==0) p->ledConnect.timeOfBlink = 20;
	else if (BlinkConnect==1) p->ledConnect.timeOfBlink = 10;
	else if (BlinkConnect==2) p->ledConnect.timeOfBlink = 5;
	else if (BlinkConnect==3) p->ledConnect.timeOfBlink = 1;

	//-------Моргание лампочкой процессора-----------------------------
	LedTurnOnOff(&p->ledCntr, p->ledCntr.status);
	WORK_DSP_LED = p->ledCntr.status;

	//-------Моргание лампочкой процессора-----------------------------
	LedTurnOnOff(&p->ledConnect, p->ledConnect.status);
	LED_CONNECT = p->ledConnect.status;
	p->leds.bit.Connect = p->ledConnect.status;

	// ------Авария----------------------------------------
	//------------------------------------------------------------------

	//p->ledOpen.isBlink  = (*p->pStatus & STATUS_OPENING) ? 1: 0;// Статус - "открывается"? Да - Led "моргает". Нет - Led не "моргает"
	//p->ledClose.isBlink = (*p->pStatus & STATUS_CLOSING) ? 1: 0;// Статус - "закрывается"? Да - Led "моргает". Нет - Led не "моргает"

	//------------------------------------------------------------------
	if (*p->pStatus & STATUS_OPENED)						// Если статус - "открыто"
	{
		LedTurnOnOff(&p->ledOpen, p->leds.bit.Open);		// Зажигаем светодиод
		p->leds.bit.Open = p->ledOpen.status;
	}
	else 													// Если статус не "открыто/открывается"
		p->leds.all |= LED_OPEN_MASK;						// Гасим светодиод

	//------------------------------------------------------------------
	if (*p->pStatus & STATUS_OPENING)						// Если статус - "открыто/открывается"
	{
		LedTurnOnOff(&p->ledOpening, p->leds.bit.Opening);	// Зажигаем светодиод
		p->leds.bit.Opening = p->ledOpening.status;
	}
	else 													// Если статус не "открыто/открывается"
		p->leds.all |= LED_OPENING_MASK;					// Гасим светодиод

	// ------Закрыто----------------------------------------
	if (*p->pStatus & STATUS_CLOSED)						// Если статус - "закрыто/закрывается"
	{
		LedTurnOnOff(&p->ledClose, p->leds.bit.Close);		// Зажигаем светодиод
		p->leds.bit.Close = p->ledClose.status;
	}
	else 													// Если статус не "закрыто/закрывается"
		p->leds.all |= LED_CLOSE_MASK;						// Гасим светодиод

	// ------Закрывается----------------------------------------
	if (*p->pStatus & STATUS_CLOSING)						// Если статус - "закрывается"
	{
		LedTurnOnOff(&p->ledClosing, p->leds.bit.Closing);	// Зажигаем светодиод
		p->leds.bit.Closing = p->ledClosing.status;
	}
	else 													// Если статус не "закрыто/закрывается"
		p->leds.all |= LED_CLOSING_MASK;					// Гасим светодиод

	// ------Муфта----------------------------------------
	if (*p->pStatus & STATUS_MUFTA)							// Если авария "Муфта"
	{
		LedTurnOnOff(&p->ledMufta, p->leds.bit.Mufta);		// Зажигаем светодиод
		p->leds.bit.Mufta = p->ledMufta.status;
	}
	else 													// Если статус не "муфта"
		p->leds.all |= LED_MUFTA_MASK;						// Гасим светодиод

	// ------Авария----------------------------------------
	if (*p->pStatus & STATUS_FAULT)// Если авария
	{
		LedTurnOnOff(&p->ledFault, p->leds.bit.Fault);// Зажигаем светодиод
		p->leds.bit.Fault = p->ledFault.status;
	}
	else 											// Если статус не "Авария"
		p->leds.all |= LED_FAULT_MASK;				// Гасим светодиод

	// ------Дефект----------------------------------------
	if (*p->pStatus & STATUS_DEFECT)// Если неисправность
	{
		LedTurnOnOff(&p->ledDefect, p->leds.bit.Defect);// Зажигаем светодиод
		p->leds.bit.Defect = p->ledDefect.status;
	}
	else 											// Если статус нет "Неисправности"
		p->leds.all |= LED_DEFECT_MASK;				// Гасим светодиод

	// ------Му/Ду----------------------------------------
	if (*p->pStatus & STATUS_MU_DU)// Если неисправность
	{
		LedTurnOnOff(&p->ledMuDu, p->leds.bit.MuDu);// Зажигаем светодиод
		p->leds.bit.MuDu = p->ledMuDu.status;
	}
	else 											// Если статус нет "Неисправности"
		p->leds.all |= LED_MUDU_MASK;				// Гасим светодиод

	LED_OPEN	= p->leds.bit.Open;		DELAY_US(1);
	LED_MUFTA	= p->leds.bit.Mufta;	DELAY_US(1);
	LED_DEFECT	= p->leds.bit.Defect;	DELAY_US(1);
	LED_FAULT	= p->leds.bit.Fault;	DELAY_US(1);
	LED_CLOSE 	= p->leds.bit.Close;	DELAY_US(1);
	LED_CLOSING = p->leds.bit.Closing;	DELAY_US(1);
	LED_OPENING = p->leds.bit.Opening;	DELAY_US(1);
	LED_MUDU	= p->leds.bit.MuDu;		DELAY_US(1);

	if ((!(*p->pStatus & STATUS_OPENING)) && (!(*p->pStatus & STATUS_CLOSING)) && (!(*p->pStatus & STATUS_CLOSED)) && (!(*p->pStatus & STATUS_OPENED)))
	{
		LED_OPEN = 0;
		DELAY_US(1);
		LED_CLOSE = 0;
		DELAY_US(1);
		p->leds.bit.Close = 0;
		p->leds.bit.Open = 0;
	}

	g_Ram.ramGroupH.BkpIndication = (~p->leds.all) & 0x00FF;


}
//--------------------------------------------------------





