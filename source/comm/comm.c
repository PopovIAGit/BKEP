/*======================================================================
Имя файла:          comm.c
Автор:
Версия файла:
Дата изменения:
Описание:
Основной модуль обработки коммуникаций
======================================================================*/

#include "comm.h"
#include "g_Ram.h"

#define TEK_DEVICE_FAULT_MASK	0xF910				// Маска на регистр дефектов устройства для модбаса ТЭК
//#define TEK_DEVICE_FAULT_MASK	0xF990

TComm	g_Comm;

void CommandUpdate(TComm *p);
Uns DigitCmdModeUpdate (Uns *Output);
//---------------------------------------------------
void Comm_Init(TComm *p)
{
	// Теле-сигнализация
	Comm_TuTsInit(&p->digitInterface);

	DigitalInputInit(&p->digitInput, (g_Ram.ramGroupB.TuTime * Prd50HZ), ICREMENT_DIV, DECREMENT_DIV);

	// Пульт местного управления:
	Comm_LocalControlInit (&p->localControl);

	//инициализация канала связи между БКП и БКД
	SciMasterConnBetweenBlockInit(&g_Comm.mbBkp);

	//канал связи с верхним уровнем контроллеров
	InitChanelAsuModbus(&g_Comm.mbAsu);

	//InitChanelShnModbus(&g_Comm.mbShn);
	InitChanelBtModbus(&g_Comm.mbBt);

	//настраиваем один и тот же физический канал для драйвера Bluetooth и Modbus
	g_Comm.Bluetooth.HardWareType = g_Comm.mbBt.Params.HardWareType;	// физика
	g_Comm.Bluetooth.ChannelID    = g_Comm.mbBt.Params.ChannelID;		// индекс канала
	g_Comm.Bluetooth.BaudRate	  = g_Comm.mbBt.Params.BaudRate;		//
	g_Comm.Bluetooth.UartBaud	  = g_Comm.mbBt.Params.UartBaud;		//
	g_Comm.Bluetooth.Mode	  	  = g_Comm.mbBt.Params.Mode;			//
	g_Comm.Bluetooth.Parity	   	  = g_Comm.mbBt.Params.Parity;			//
	SerialCommInit(&g_Comm.mbBt);
	g_Comm.mbBt.Frame.TimerPost.Timeout=g_Comm.mbBt.Frame.TimerPre.Timeout*2;
	InitChanelBt(&g_Comm.Bluetooth);

	SerialCommInit(&g_Comm.mbAsu);
	//SerialCommInit(&g_Comm.mbShn);
	Comm_LocalControlInit(&g_Comm.localControl);

}
//---------------------------------------------------
void Comm_Update(TComm *p)
{

	if (g_Comm.Bluetooth.ModeProtocol==0) ModBusUpdate(&g_Comm.mbAsu); 	// slave канал связи с верхним уровнем АСУ
	//ModBusUpdate(&g_Comm.mbShn);  // master канал связи с устройством плавного пуска

	//SciMasterConnBetweenBlockUpdate(&g_Comm.mbBkp);// master канал связи с

	BluetoothWTUpdate(&g_Comm.Bluetooth); //драйвер Bluetooth
	if (g_Comm.Bluetooth.ModeProtocol>0) ModBusUpdate(&g_Comm.mbBt);  // slave

	//SerialCommUpdate(&Mb);


}

void Comm_50HzCalc(TComm *p)
{
	//	КОМАНДЫ С МПУ !!!
	// передаем команду с кнопок управления
	if (g_Ram.ramGroupH.CmdButton != KEY_STOP)
	g_Ram.ramGroupH.CmdKey = Comm_LocalKeyUpdate(&p->localControl);

	// передаем команду с ручек БКП
	switch (Comm_LocalButtonUpdate(&p->localControl))
	{
	case (BTN_STOPMU_BIT | BTN_STOPDU_BIT):
		g_Ram.ramGroupH.CmdButton = KEY_STOP;
		break;
	case BTN_OPEN_BIT:
		if (g_Ram.ramGroupH.CmdKey == KEY_STOP) break;
		g_Ram.ramGroupH.CmdButton = KEY_OPEN;
		break;
	case BTN_CLOSE_BIT:
		if (g_Ram.ramGroupH.CmdKey == KEY_STOP) break;
		g_Ram.ramGroupH.CmdButton = KEY_CLOSE;
		break;
	default:
		g_Ram.ramGroupH.CmdButton = KEY_NONE;
	}

	CommandUpdate(&g_Comm);
}

//-----------обработка источников команд -----------------------------
void CommandUpdate(TComm *p)
{
	static Byte clrReset=0;

	p->outputCmdReg = 0;//CMD_NO;

	//здесь не только обработка ТС но и вывод ТС
	Comm_TuTsUpdate(&p->digitInterface); // Телеуправление, телесигнализация

	//если телеуправление разрешено
	p->digitInput.input = p->digitInterface.Inputs.all;
	DigitalInpUpdate(&p->digitInput); // Вызов функции обработки цифрового сигнала, защита от помех
	g_Ram.ramGroupA.StateTu.all = p->digitInput.output;

	if (!(p->digitInput.output & 0x20))
	{
		p->outputCmdReg = (p->digitInput.output & 0x4);
	}
	else
	{
		p->outputCmdReg = (p->digitInput.output & 0x7);
	}

	g_Ram.ramGroupH.TuState = p->outputCmdReg;

	if ((p->digitInput.output>>4)&0x01==1)
	{
		if (clrReset==0) g_Ram.ramGroupD.PrtReset = 1;
		clrReset=1;
	} else if ((p->digitInput.output>>4)&0x01==0)
	{
		clrReset=0;
	}
//	p->outputCmdReg |= DigitCmdModeUpdate(&p->digitInput.output);
///	if (p->outputCmdReg)
//	{
//		p->outputCmdSrc = CMD_SRC_DIGITAL;
//	}
}
//---------------------------------------------------
// Функция обработки режима работы команд дискретного интерфейса (режим - импульсный, потенциальный)
Uns DigitCmdModeUpdate (Uns *Output)
{
	static Uns prevOutput;
	Uns result=0;
	Uns i=0;

	for(i=0; i<7; i++)
	{
		if((g_Ram.ramGroupB.DigitalMode.all>>i)&0x01) //потенциальный
		{

			if ((*Output)&0x03)	    									// Если выходе есть команды на движение
			{
				if ((((*Output)>>i)&0x01) != (((prevOutput)>>i)&0x01))	// Если сигнал изменился
					result |= (0x01<<i);//*Output;						// Что нажали - туда и едем
				else													// Иначе (Сигнал не изменился)
					result &= ~(0x01<<i);								// Никаких команд не подаем
			}
			else														// Если сигнал ушел
			{
				if ((*Output)&0x03)										// Но на предыдущей итерации были команды на движение
					result |= DIN_STOP_BIT;    							// Считаем, что пришла команда стоп
				else 													// Иначе (Сигнал не изменился)
					result &= ~(0x01<<i);								// Никаких команд не подаем
			}
		}else
		{
			if ((((*Output)>>i)&0x01) != (((prevOutput)>>i)&0x01))		// Если сигнал изменился
				result |= (0x01<<i);//*Output;							// Что нажали - туда и едем
			else														// Иначе (Сигнал не изменился)
				result &= ~(0x01<<i);									// Никаких команд не подаем
		}
	}

	prevOutput = *Output;						// Запоминаем предыдущие значение сигнала с ТУ
	return result;


	//--------------------------------------------------------------------
	/*if (IsImpulseMode())						// Если режим - импульсный
	{
		if (*Output != prevOutput)				// Если сигнал изменился
			result = *Output;					// Что нажали - туда и едем
		else									// Иначе (Сигнал не изменился)
			result = 0;							// Никаких команд не подаем
	}
	else if (IsPotentialMode())					// Если режим - потенциальный
	{
		if (*Output >> 1)						// Если выходе есть команды на движение
		{
			if (*Output != prevOutput)			// Если сигнал изменился
				result = *Output;				// Что нажали - туда и едем
			else								// Иначе (Сигнал не изменился)
				result = 0;						// Никаких команд не подаем
		}
		else									// Если сигнал ушел
		{
			if (prevOutput >> 1)				// Но на предыдущей итерации были команды на движение
				result = CMD_STOP_ESCAPE; 		// Считаем, что пришла команда стоп
			else 								// Иначе (Сигнал не изменился)
				result = 0;						// Никаких команд не подаем
		}
	}

	prevOutput = *Output;						// Запоминаем предыдущие значение сигнала с ТУ
	return result;*/
}
//---------------------------------------------------

void TekModbusParamsUpdate(void) //??? необходимы проверки
{
	TRamGroupT *tek = &g_Ram.ramGroupT;

	// Заполняем технологический регистр
	tek->TechReg.bit.Opened  = g_Ram.ramGroupA.Status.bit.Opened;
	tek->TechReg.bit.Closed  = g_Ram.ramGroupA.Status.bit.Closed;
	tek->TechReg.bit.Mufta1  = g_Ram.ramGroupA.Status.bit.Mufta;
	tek->TechReg.bit.Mufta2  = g_Ram.ramGroupA.Status.bit.Mufta;
	tek->TechReg.bit.MuDu    = !g_Ram.ramGroupA.Status.bit.MuDu;
	tek->TechReg.bit.Opening = g_Ram.ramGroupA.Status.bit.Opening;
	tek->TechReg.bit.Closing = g_Ram.ramGroupA.Status.bit.Closing;
	tek->TechReg.bit.Stop    = g_Ram.ramGroupA.Status.bit.Stop;
	tek->TechReg.bit.Ten     = g_Ram.ramGroupA.Status.bit.Ten;

	// Заполняем регистр дефектов
	tek->DefReg.bit.I2t = g_Ram.ramGroupA.Faults.Load.bit.I2t;
	tek->DefReg.bit.ShC = (g_Ram.ramGroupA.Faults.Load.bit.ShCU || g_Ram.ramGroupA.Faults.Load.bit.ShCV || g_Ram.ramGroupA.Faults.Load.bit.ShCW);
	tek->DefReg.bit.Drv_T = 0;
	tek->DefReg.bit.Uv = (g_Ram.ramGroupA.Faults.Net.bit.UvR || g_Ram.ramGroupA.Faults.Net.bit.UvS || g_Ram.ramGroupA.Faults.Net.bit.UvT);
	tek->DefReg.bit.Phl = (g_Ram.ramGroupA.Faults.Load.bit.PhlU || g_Ram.ramGroupA.Faults.Load.bit.PhlV || g_Ram.ramGroupA.Faults.Load.bit.PhlW);
	tek->DefReg.bit.NoMove = g_Ram.ramGroupA.Faults.Proc.bit.NoMove;
	tek->DefReg.bit.Ov = (g_Ram.ramGroupA.Faults.Net.bit.OvR || g_Ram.ramGroupA.Faults.Net.bit.OvS || g_Ram.ramGroupA.Faults.Net.bit.OvT);
	tek->DefReg.bit.Bv = (g_Ram.ramGroupA.Faults.Net.bit.BvR || g_Ram.ramGroupA.Faults.Net.bit.BvS || g_Ram.ramGroupA.Faults.Net.bit.BvT);
	tek->DefReg.bit.Th = g_Ram.ramGroupA.Faults.Dev.bit.Th_BCP;
	tek->DefReg.bit.Tl = g_Ram.ramGroupA.Faults.Dev.bit.Tl_BCP;
	tek->DefReg.bit.PhOrdU = g_Ram.ramGroupA.Faults.Net.bit.PhOrd;
	tek->DefReg.bit.PhOrdDrv = g_Ram.ramGroupA.Faults.Proc.bit.PhOrd;
	tek->DefReg.bit.DevDef 	 = ((g_Ram.ramGroupA.Faults.Dev.all & TEK_DEVICE_FAULT_MASK) != 0); //маску переделать

	// Регистр команд
	// При срабатывании одной команды, сбрасываем все
	if (g_Core.VlvDrvCtrl.ActiveControls & CMD_SRC_SERIAL)
	{
		if(tek->ComReg.bit.Stop)
		{
			g_Ram.ramGroupD.ControlWord = vcwStop;

			tek->ComReg.bit.Stop = 0;
			tek->ComReg.bit.Open = 0;
			tek->ComReg.bit.Close = 0;
		}
		else if(tek->ComReg.bit.Open)
		{
			g_Ram.ramGroupD.ControlWord = vcwOpen;

			tek->ComReg.bit.Stop = 0;
			tek->ComReg.bit.Open = 0;
			tek->ComReg.bit.Close = 0;
		}
		else if(tek->ComReg.bit.Close)
		{
			g_Ram.ramGroupD.ControlWord = vcwClose;

			tek->ComReg.bit.Stop = 0;
			tek->ComReg.bit.Open = 0;
			tek->ComReg.bit.Close = 0;
		}
	}
	else
	{
		tek->ComReg.bit.Stop = 0;
		tek->ComReg.bit.Open = 0;
		tek->ComReg.bit.Close = 0;
	}

	if (tek->ComReg.bit.PrtReset)
	{
		g_Ram.ramGroupD.PrtReset = 1;
		tek->ComReg.bit.PrtReset = 0;
	}

	if (tek->ComReg.bit.EnDiscrOutTest)
	{
		g_Ram.ramGroupG.DiscrOutTest = 1;
	}
	else if (tek->ComReg.bit.DisDiscrOutTest)
	{
		if (g_Ram.ramGroupG.DiscrOutTest == 1)
		{
			g_Ram.ramGroupG.Mode = 0;
			g_Ram.ramGroupG.DiscrOutTest = 0;
		}
	}
	else if (tek->ComReg.bit.EnDiscrInTest)
	{
		g_Ram.ramGroupG.DiscrInTest = 1;
	}
	else if (tek->ComReg.bit.DisDiscrInTest)
	{
		if (g_Ram.ramGroupG.DiscrInTest == 1)
		{
			g_Ram.ramGroupG.Mode = 0;
			g_Ram.ramGroupG.DiscrInTest = 0;
		}
	}

	// На всякий случай сбрасываем регистр команд
	if (tek->ComReg.all != 0)
		tek->ComReg.all = 0;

// Убрали переключение МУ/ДУ
/*
	if (tek->ComReg.bit.SetDu && IsStopped())
		{
			Mcu.MuDuInput = 0;
			tek->ComReg.bit.SetDu = 0;
		}
	else if (tek->ComReg.bit.SetMu && IsStopped())
		{
			Mcu.MuDuInput = 1;
			tek->ComReg.bit.SetMu = 0;
		}
*/
	tek->PositionPr 	 = g_Ram.ramGroupA.PositionPr;
	tek->CycleCnt 		 = g_Ram.ramGroupA.CycleCnt;
	tek->Iu				 = g_Ram.ramGroupA.Iu;
	tek->Ur				 = g_Ram.ramGroupA.Ur;
	tek->Torque			 = g_Ram.ramGroupA.Torque;
	tek->Speed			 = g_Ram.ramGroupA.Speed;
	tek->RsStation		 = g_Ram.ramGroupB.RsStation;

	//Состояние дискретных входов и выходов
	tek->TsTu.bit.IsDiscrOutActive = (g_Ram.ramGroupG.DiscrOutTest);
	tek->TsTu.bit.IsDiscrInActive = (g_Ram.ramGroupG.DiscrInTest);

	tek->TsTu.bit.InOpen 	 = g_Comm.digitInterface.Inputs.bit.Open;
	tek->TsTu.bit.InClose	 = g_Comm.digitInterface.Inputs.bit.Close;
	tek->TsTu.bit.InStop 	 = g_Comm.digitInterface.Inputs.bit.Stop;
	tek->TsTu.bit.InMu 		 = g_Comm.digitInterface.Inputs.bit.Mu;
	tek->TsTu.bit.InDu 		 = g_Comm.digitInterface.Inputs.bit.Du;

	if (!g_Ram.ramGroupG.DiscrOutTest)
	{
		tek->TsTu.bit.OutOpened  = g_Comm.digitInterface.Outputs.bit.Opened;
		tek->TsTu.bit.OutClosed  = g_Comm.digitInterface.Outputs.bit.Closed;
	 	tek->TsTu.bit.OutMufta   = g_Comm.digitInterface.Outputs.bit.Mufta;
		tek->TsTu.bit.OutFault   = g_Comm.digitInterface.Outputs.bit.Fault;
		tek->TsTu.bit.OutOpening = g_Comm.digitInterface.Outputs.bit.Opening;
		tek->TsTu.bit.OutClosing = g_Comm.digitInterface.Outputs.bit.Closing;
		tek->TsTu.bit.OutMuDu 	 = g_Comm.digitInterface.Outputs.bit.MUDU;
		tek->TsTu.bit.OutNeispr  = g_Comm.digitInterface.Outputs.bit.Defect;
	}
	else
	{
		g_Comm.digitInterface.Outputs.bit.Opened = tek->TsTu.bit.OutOpened;
		g_Comm.digitInterface.Outputs.bit.Closed = tek->TsTu.bit.OutClosed;
		g_Comm.digitInterface.Outputs.bit.Mufta = tek->TsTu.bit.OutMufta;
		g_Comm.digitInterface.Outputs.bit.Fault = tek->TsTu.bit.OutFault;
		g_Comm.digitInterface.Outputs.bit.Opening = tek->TsTu.bit.OutOpening;
		g_Comm.digitInterface.Outputs.bit.Closing = tek->TsTu.bit.OutClosing;
		g_Comm.digitInterface.Outputs.bit.MUDU = tek->TsTu.bit.OutMuDu;
		g_Comm.digitInterface.Outputs.bit.Defect = tek->TsTu.bit.OutNeispr;
	}
}
