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
 	p->ledCntr.blinkPRD 		= 1;

 	p->ledMpo.blinkPRD 			= Prd10HZ;
 	p->ledMpz.blinkPRD 			= Prd10HZ;
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

	p->ledMpo.timeOfBlink 		= freq/p->ledMpo.blinkPRD;			//состояние открыто
	p->ledMpz.timeOfBlink 		= freq/p->ledMpz.blinkPRD;			//состояние открыто

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

	p->pStatus = &g_Core.Status.all;

}
//--------------------------------------------------------
Uns BlinkConnect=0;
void Peref_LedsUpdate(pLeds p)
{

	if (g_Comm.Bluetooth.State<7) return;

	g_Ram.ramGroupH.BkpIndication.all = 0;

	if(g_Comm.mbAsu.Serial.RsState == 0)
	{
		BlinkConnect = 1;
	}
	else
	{
		BlinkConnect = 0;
	}
	//if (g_Comm.mbBkp.Frame.ConnFlag==1) BlinkConnect++;
	//if (g_Comm.mbShn.Serial.RsState==0) BlinkConnect++;

	p->ledConnect.timeOfBlink = 20;
	if (BlinkConnect==1) p->ledConnect.timeOfBlink = 1;
	//else if (BlinkConnect==1) p->ledConnect.timeOfBlink = 10;
	//else if (BlinkConnect==2) p->ledConnect.timeOfBlink = 5;
	//else if (BlinkConnect==3) p->ledConnect.timeOfBlink = 1;

	//-------Моргание лампочкой процессора-----------------------------
	LedTurnOnOff(&p->ledCntr, p->ledCntr.status);
	WORK_DSP_LED = p->ledCntr.status;

	//-------Моргание лампочкой процессора-----------------------------
	LedTurnOnOff(&p->ledConnect, p->ledConnect.status);
#if !NEW_RAZ
	if (BlinkConnect == 1)
        LED_CONNECT = p->ledConnect.status;
    else
        LED_CONNECT = 1;

	p->leds.bit.Connect = p->ledConnect.status;
#else
	if (BlinkConnect == 1)
		p->leds.bit.Connect = p->ledConnect.status;
    else
    	p->leds.bit.Connect = 1;
#endif
	// ------Авария----------------------------------------
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
		LedTurnOnOff(&p->ledMpo, p->leds.bit.Mpo);	// Зажигаем светодиод
		p->leds.bit.Mpo = p->ledMpo.status;
	}
	else 													// Если статус не "открыто/открывается"
		p->leds.all |= LED_MPO_MASK;					// Гасим светодиод

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
		LedTurnOnOff(&p->ledMpz, p->leds.bit.Mpz);	// Зажигаем светодиод
		p->leds.bit.Mpz = p->ledMpz.status;
	}
	else 												// Если статус не "закрывается"
		p->leds.all |= LED_MPZ_MASK;					// Гасим светодиод

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
	if (*p->pStatus & STATUS_MU_DU)// Если му\ду
	{
		LedTurnOnOff(&p->ledMuDu, p->leds.bit.MuDu);
		p->leds.bit.MuDu = p->ledMuDu.status;
	}
	else 											// Если статус нет "Неисправности"
	{
	    p->leds.bit.MuDu = 1;
	}

#if NEW_RAZ
  if(g_Core.Protections.outFaults.Proc.bit.MuDuDef)
        {
                p->leds.bit.MuDu = 0;
        }
        else if(g_Ram.ramGroupB.MuDuSetup == mdOff)
        {
                p->leds.bit.MuDu = 1;
        }

        if (g_Core.Protections.outFaults.Proc.bit.MuDuDef)
        {
                g_Ram.ramGroupH.BkpIndication.bit.MuDu = 0;
        }
        else if (g_Ram.ramGroupB.MuDuSetup == mdOff)
        {
                g_Ram.ramGroupH.BkpIndication.bit.MuDu = 1;
        }
        else if (g_Ram.ramGroupB.MuDuSetup != mdOff)
        {
                g_Ram.ramGroupH.BkpIndication.bit.MuDu = ~p->leds.bit.MuDu;
        }
#endif

	if(g_Ram.ramGroupG.Mode)
        {
          p->leds.all = ~g_Ram.ramGroupG.LedsReg.all;
        }
	else
	{
	#if !NEW_RAZ
	LED_MUFTA	=  p->leds.bit.Mufta;		asm(" RPT #9 || NOP");

	LED_DEFECT	=  p->leds.bit.Defect;		asm(" RPT #9 || NOP");
	LED_FAULT	=  p->leds.bit.Fault;		asm(" RPT #9 || NOP");

	LED_MPZ = p->leds.bit.Mpz;				asm(" RPT #9 || NOP");
	LED_MPO = p->leds.bit.Mpo;				asm(" RPT #9 || NOP");

	if(g_Core.Protections.outFaults.Proc.bit.MuDuDef)
	{
		p->leds.bit.MuDu = 0;
	}
	else if(g_Ram.ramGroupB.MuDuSetup == mdOff)
	{
		p->leds.bit.MuDu = 1;
	}

	LED_MUDU	= !p->leds.bit.MuDu;		asm(" RPT #9 || NOP");

	if ((!(*p->pStatus & STATUS_CLOSED)) && (!(*p->pStatus & STATUS_OPENED)))
	{
		GpioDataRegs.GPADAT.all &=~ 0x2400000;
		//LED_OPEN = 0;
		//DELAY_US(1);
		//LED_CLOSE = 0;
		//DELAY_US(1);
		p->leds.bit.Close = 0;
		p->leds.bit.Open = 0;
	} else {
		LED_OPEN	= p->leds.bit.Open;		asm(" RPT #9 || NOP");
		LED_CLOSE 	= p->leds.bit.Close;	asm(" RPT #9 || NOP");
	}

	//g_Ram.ramGroupH.BkpIndication = (~p->leds.all) & 0x00FF;
	if (g_Core.Protections.outFaults.Proc.bit.MuDuDef)
	{
		g_Ram.ramGroupH.BkpIndication.bit.MuDu = 0;
	}
	else if (g_Ram.ramGroupB.MuDuSetup == mdOff)
	{
		g_Ram.ramGroupH.BkpIndication.bit.MuDu = 1;
	}
	else if (g_Ram.ramGroupB.MuDuSetup != mdOff)
	{
		g_Ram.ramGroupH.BkpIndication.bit.MuDu = ~p->leds.bit.MuDu;
	}
	 #endif
	}

	//g_Ram.ramGroupH.BkpIndication.bit.MuDu  = ~p->leds.bit.MuDu;
	g_Ram.ramGroupH.BkpIndication.bit.Open  = ~p->leds.bit.Open;
	g_Ram.ramGroupH.BkpIndication.bit.Mpo   = ~p->leds.bit.Mpo;
	g_Ram.ramGroupH.BkpIndication.bit.Fault = ~p->leds.bit.Fault;
	g_Ram.ramGroupH.BkpIndication.bit.Close = ~p->leds.bit.Close;
	g_Ram.ramGroupH.BkpIndication.bit.Mpz   = ~p->leds.bit.Mpz;
	g_Ram.ramGroupH.BkpIndication.bit.Defect= ~p->leds.bit.Defect;
	g_Ram.ramGroupH.BkpIndication.bit.Mufta = ~p->leds.bit.Mufta;

#if NEW_RAZ
	Peref_74HC595UpdateLed(&g_Peref.LedReg, &p->leds);
#endif
}
//--------------------------------------------------------





