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

	p->ledOpen.blinkPRD 		= Prd10HZ;
	p->ledClose.blinkPRD 		= Prd10HZ;
	p->ledFault.blinkPRD 		= Prd10HZ;
	p->ledDefect.blinkPRD 		= Prd10HZ;
	p->ledMufta.blinkPRD 		= Prd10HZ;
	p->ledConnect.blinkPRD 		= 1;

	p->ledCntr.isBlink 			= 1;
	p->ledConnect.isBlink 		= 1;
	p->ledBluetooth.isBlink		= 0;

	p->ledCntr.timeOfBlink 		= freq/p->ledCntr.blinkPRD;			//работа процессора
	p->ledOpen.timeOfBlink 		= freq/p->ledOpen.blinkPRD;			//состояние открыто
	p->ledClose.timeOfBlink 	= freq/p->ledClose.blinkPRD;		//состояние закрыто
	p->ledFault.timeOfBlink 	= freq/p->ledFault.blinkPRD;		//авария
	p->ledDefect.timeOfBlink 	= freq/p->ledDefect.blinkPRD;		//неисправность
	p->ledMufta.timeOfBlink 	= freq/p->ledMufta.blinkPRD;		//муфта
	p->ledBluetooth.timeOfBlink	= 0;								//bluetooth
	p->ledConnect.timeOfBlink 	= freq/p->ledConnect.blinkPRD;		//сеть

	p->pStatus = &g_Core.Status.all;
	//p->pStatus = &g_Core.TestStatus.all;

}
//--------------------------------------------------------
void Peref_LedsUpdate(pLeds p)
{

	Uns tmp=0;

	//-------Моргание лампочкой процессора-----------------------------
	LedTurnOnOff(&p->ledCntr, p->ledCntr.status);
	WORK_DSP_LED = p->ledCntr.status;

	//-------Моргание лампочкой процессора-----------------------------
	LedTurnOnOff(&p->ledConnect, p->ledConnect.status);
	LED_CONNECT = p->ledConnect.status;

	// ------Bluettoth режим----------------------------------------------
	// ------Авария----------------------------------------

	if (*p->pStatus & STATUS_BLUETOOTH)// Если авария
	{
		LedTurnOnOff(&p->ledBluetooth, p->leds.bit.Bluetooth);// Зажигаем светодиод
		p->leds.bit.Bluetooth = p->ledBluetooth.status;
	}
	else 											// Если статус не "Авария"
		p->leds.all |= LED_BLUETOOTH_MASK;				// Гасим светодиод

	/*if (*p->pStatus & STATUS_BLUETOOTH)		// Если "Режим передачи по Bluettoth"
	{
		p->ledBluetooth.isBlink		= 0;
	}
	else
	{
		p->ledBluetooth.isBlink		= 1;
	}
	LedTurnOnOff(&p->ledBluetooth, p->ledBluetooth.status);
	ENABLE_BLUETOOTH = p->ledBluetooth.status;*/
	//------------------------------------------------------------------

	p->ledOpen.isBlink  = (*p->pStatus & STATUS_OPENING) ? 1: 0;// Статус - "открывается"? Да - Led "моргает". Нет - Led не "моргает"
	p->ledClose.isBlink = (*p->pStatus & STATUS_CLOSING) ? 1: 0;// Статус - "закрывается"? Да - Led "моргает". Нет - Led не "моргает"

	//------------------------------------------------------------------
	if ((*p->pStatus & STATUS_OPENED)||(*p->pStatus & STATUS_OPENING))	// Если статус - "открыто/открывается"
	{
		LedTurnOnOff(&p->ledOpen, p->leds.bit.Open);	// Зажигаем светодиод
		p->leds.bit.Open = p->ledOpen.status;
	}
	else 												// Если статус не "открыто/открывается"
		p->leds.all |= LED_OPEN_MASK;					// Гасим светодиод

	// ------Закрыто/Закрывается----------------------------------------
	if ((*p->pStatus & STATUS_CLOSED)||(*p->pStatus & STATUS_CLOSING))	// Если статус - "закрыто/закрывается"
	{
		LedTurnOnOff(&p->ledClose, p->leds.bit.Close);	// Зажигаем светодиод
		p->leds.bit.Close = p->ledClose.status;
	}
	else 												// Если статус не "закрыто/закрывается"
		p->leds.all |= LED_CLOSE_MASK;					// Гасим светодиод

	// ------Муфта----------------------------------------
	if (*p->pStatus & STATUS_MUFTA)						// Если авария "Муфта"
	{
		LedTurnOnOff(&p->ledMufta, p->leds.bit.Mufta);	// Зажигаем светодиод
		p->leds.bit.Mufta = p->ledMufta.status;
	}
	else 												// Если статус не "муфта"
		p->leds.all |= LED_MUFTA_MASK;					// Гасим светодиод

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


	//LED_OPEN	= p->leds.bit.Open;
	//LED_MUFTA	= g_Core.TestStatus.bit.Mufta;// p->leds.bit.Mufta;
	//LED_DEFECT	= p->leds.bit.Defect;
	//LED_FAULT	= p->leds.bit.Fault;
	//LED_CLOSE = p->leds.bit.Close;

	//ENABLE_BLUETOOTH = p->leds.bit.Bluetooth;

	g_Ram.ramGroupH.BkpIndication = 0;

	tmp = !p->leds.bit.Fault;		g_Ram.ramGroupH.BkpIndication = tmp;
	tmp = !p->leds.bit.Defect;		g_Ram.ramGroupH.BkpIndication |=(tmp<<1)&0x2;
	tmp = !p->leds.bit.Mufta;		g_Ram.ramGroupH.BkpIndication |=(tmp<<2)&0x4;
	tmp = !p->leds.bit.Open;		g_Ram.ramGroupH.BkpIndication |=(tmp<<3)&0x8;
	tmp = !p->leds.bit.Close;		g_Ram.ramGroupH.BkpIndication |=(tmp<<4)&0x10;
	tmp = !p->leds.bit.Connect;		g_Ram.ramGroupH.BkpIndication |=(tmp<<5)&0x20;

}
//--------------------------------------------------------





