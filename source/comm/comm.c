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

TComm	g_Comm;
Uint16 ASU_Data[10];
Uint16 SHN_Data[10];


//Uns DigitCmdModeUpdate (Uns *Output);
static char ReadRegs(TMbPort *Port, Uint16 *Data, Uint16 Addr, Uint16 Count);
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

	InitChanelShnModbus(&g_Comm.mbShn);
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
	SerialCommInit(&g_Comm.mbShn);
	g_Comm.mbShn.Frame.TimerPost.Timeout=10;
	g_Comm.mbShn.Frame.TimerConn.Timeout=1000;
	g_Comm.mbShn.Frame.TimerAck.Timeout=80;

	Comm_LocalControlInit(&g_Comm.localControl);

	// настройка работы шнайдера
	//p->Shn.SHN_ReadFlag = 0;
	//p->Shn.SHN_Mode = 1;
	p->btn_reset_alarmFlag = 0;

}

/*
Uint16 Busy:1;		// 0 - занят идет выполнение последней соманды
					//
Uint16 Ready:1;		// 1 - готов команда выполнена
Uint16 Wait:1;		// 2 - ожидание команды
Uint16 NoConnect:1; // 3 - нет связи
Uint16 Error:1;		// 4 - ошибка при выполнении команды
Uint16 Rsvd:11;		// 5-15
*/

void mb_read_ATS48(TComm *p, Uns addr, Uns count)
{
	if (p->mbShn.Stat.Status.bit.Error==0)
	{
		p->ioATS48.Func = MB_READ_REGS ;
		p->ioATS48.Addr = addr;
		p->ioATS48.CountOutput = count;
		p->mbShn.Stat.Status.bit.Wait = 1;
	}
}

void mb_write_ATS48(TComm *p, Uns addr, Uns count , Uns data)
{
	if (p->mbShn.Stat.Status.bit.Error==0)
	{
		p->ioATS48.Func = MB_WRITE_REGS ;
		p->ioATS48.Addr = addr;
		p->ioATS48.CountOutput = count;
		p->ioATS48.DataOutput[0] = data;
		p->mbShn.Stat.Status.bit.Wait = 1;
	}
}

void Comm_ControlModbusUpdateAltistar48(TComm *p)
{
	Uns NumParam=0;
	Uns Addr=0;
	Uns *Dest = ToUnsPtr(&g_Ram) + p->ioATS48.Addr;

	if (p->mbShn.Stat.Status.bit.Error==1)
	{
		//p->mbShn.Stat.Status.bit.Error=0;
	}

	// в соответствии с адресом выставляем соответствующую функцию
	if (p->mbShn.Stat.Status.bit.Ready==1 && p->mbShn.Stat.Status.bit.Busy==1) p->mbShn.Stat.Status.bit.Busy=0;
	if (p->mbShn.Stat.Status.bit.Busy==1)
	{
		p->mbShn.Stat.Status.bit.Wait = 0;
		return;
	}

	// находимся в режиме ожидания команды
	if (p->mbShn.Stat.Status.bit.Wait==1)
	{
		p->mbShn.Stat.Status.bit.Ready = 0;
		p->mbShn.Packet.Request = p->ioATS48.Func;
		p->mbShn.Packet.Data[0] = p->ioATS48.DataOutput[0];

		NumParam = p->ioATS48.Addr - REG_CONTROL_ATS48;
		PFUNC_blkRead((Ptr)&g_Core.menu.AtsParams[NumParam], (Uns*)(&Addr), 1);
		p->mbShn.Packet.Addr = Addr;

		p->mbShn.Packet.Count = p->ioATS48.CountOutput;
		p->mbShn.Stat.Status.bit.Busy = 1;
		p->mbShn.Stat.Status.bit.Wait = 0;

		return;
	}

	// статус готовности команда выполнена можно забирать данные
	if (p->mbShn.Stat.Status.bit.Ready==1 && p->ioATS48.Func!=0)
	{
		if (p->ioATS48.Func==MB_READ_REGS)
		{
			memcpy(Dest, &(p->mbShn.Packet.Data[0]), p->mbShn.Packet.Count);
		}
		p->ioATS48.Func = 0;
	}

	// на выполнение команды отведено 500мс затем если не выполнилась команда
	// будет возможность заново подать команду
	//

}
//TODO test read write
Uns CommandATS48=0;

//---------------------------------------------------
void Comm_Update(TComm *p)
{

	//Uint16 lAddr;

	if (g_Comm.Bluetooth.ModeProtocol == 0)
	{
		ModBusUpdate(&g_Comm.mbAsu); 	// slave канал связи с верхним уровнем АСУ
	}

	if (g_Comm.Bluetooth.ModeProtocol == 0 && g_Ram.ramGroupB.StopMethod == smDynBreak)
	{
		ModBusUpdate(&g_Comm.mbShn);  // master канал связи с устройством плавного пуска

		if (p->mbShn.Stat.Status.bit.Busy == 0)
		{
			CommandATS48++;
			p->mbShn.Stat.Status.bit.Ready = 0;
			switch (CommandATS48)
			{
			case 1:
				mb_read_ATS48(&g_Comm, GetAdr(ramGroupATS.State1), 1);
				break;
			case 2:
				mb_read_ATS48(&g_Comm, GetAdr(ramGroupATS.State2), 1);
				break;
			case 3:
				mb_read_ATS48(&g_Comm, GetAdr(ramGroupATS.State3), 1);

				break;

			case 4:
				mb_read_ATS48(&g_Comm, GetAdr(ramGroupATS.LFT), 1);

				break;
			case 5:
				mb_read_ATS48(&g_Comm, GetAdr(ramGroupATS.PHP), 1);

				break;
			case 6:
				/*	if(g_Ram.ramGroupATS.PHP == 1)
					{
						mb_write_ATS48(&g_Comm,GetAdr(ramGroupATS.PHP), 1, 0);
					}*/
				break;
			case 7:
				if (g_Ram.ramGroupATS.Control1.all != 0)
				{
					mb_write_ATS48(&g_Comm, GetAdr(ramGroupATS.Control1), 1,
							g_Ram.ramGroupATS.Control1.all);
				}
				break;
			case 8:
				if (g_Ram.ramGroupATS.Control2 != 0)
				{
					mb_write_ATS48(&g_Comm, GetAdr(ramGroupATS.Control2), 1,
							g_Ram.ramGroupATS.Control2);
				}
				CommandATS48 = 0;
				break;

			}
		}
		Comm_ControlModbusUpdateAltistar48(&g_Comm);

	}

	BluetoothWTUpdate(&g_Comm.Bluetooth); //драйвер Bluetooth
	if (g_Comm.Bluetooth.ModeProtocol>0) ModBusUpdate(&g_Comm.mbBt);  // slave

}
static char ReadRegs(TMbPort *Port, Uint16 *Data, Uint16 Addr, Uint16 Count)
{
	memcpy(Port->Packet.Data, &Data[Addr], Count);
	return 0;
}

void Comm_50HzCalc(TComm *p)
{
	//	КОМАНДЫ С МПУ !!!
	// передаем команду с кнопок управления

	// передаем команду с ручек БКП
	switch (Comm_LocalButtonUpdate(&p->localControl))
	{
	case (BTN_STOPMU_BIT | BTN_STOPDU_BIT):
		g_Ram.ramGroupH.CmdButton = KEY_STOP;
		break;
	case BTN_OPEN_BIT:
		//if (g_Ram.ramGroupH.CmdKey == KEY_STOP) break;
		g_Ram.ramGroupH.CmdButton = KEY_OPEN;
		break;
	case BTN_CLOSE_BIT:
		//if (g_Ram.ramGroupH.CmdKey == KEY_STOP) break;
		g_Ram.ramGroupH.CmdButton = KEY_CLOSE;
		break;
	default:
		g_Ram.ramGroupH.CmdButton = KEY_NONE;
	}

	//Кнопка сброса аварий на лицевой панели
	if (BTN_RESET_ALARM == 0) //&& (p->btn_reset_alarmFlag == 0))
	{
		if (p->btn_reset_alarmTimer++ >= 1 * Prd50HZ)
		{
			g_Ram.ramGroupD.PrtReset = 1;
			p->btn_reset_alarmFlag = 1;
			g_Peref.Display.data = 999;
			p->btn_reset_alarmTimer =0;
		}

	}
	else if (BTN_RESET_ALARM == 1) //&& p->btn_reset_alarmFlag == 1)
	{
		p->btn_reset_alarmFlag = 0;
		p->btn_reset_alarmTimer = 0;
	}

	//CommandUpdate(&g_Comm);
}

//-----------обработка источников команд -----------------------------
void Comm_CommandUpdate(TComm *p)
{
	static Byte clrReset=0;

	p->outputCmdReg = 0;

	//здесь не только обработка ТС но и вывод ТС
//	Comm_TuTsUpdate(&p->digitInterface); // Телеуправление, телесигнализация

	//если телеуправление разрешено
	p->digitInput.input = p->digitInterface.Inputs.all;
	DigitalInpUpdate(&p->digitInput); // Вызов функции обработки цифрового сигнала, защита от помех
	g_Ram.ramGroupA.StateTu.all = p->digitInput.output;

	if (!(p->digitInput.output & 0x20)) // Если не выставлен ТУ дистанции то игнорируем конмады кроме стопа.
	{
		p->outputCmdReg = (p->digitInput.output & 0xC);
	}
	else
	{
		p->outputCmdReg = (p->digitInput.output & 0xF);
	}

	g_Ram.ramGroupH.TuState = p->outputCmdReg;

	if ((p->digitInput.output & 0x40) >> 6 == 1)
	{
		if (clrReset==0) g_Ram.ramGroupD.PrtReset = 1;
		clrReset=1;
	}
	else if ((p->digitInput.output & 0x040) >> 6 == 0)
	{
		clrReset=0;
	}
//	p->outputCmdReg |= DigitCmdModeUpdate(&p->digitInput.output);
// 	if (p->outputCmdReg)
//	{
//		p->outputCmdSrc = CMD_SRC_DIGITAL;
//	}
}
//---------------------------------------------------
// Функция обработки режима работы команд дискретного интерфейса (режим - импульсный, потенциальный)
/*
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
					result |= DIN_STOP_OPEN_BIT;    					// Считаем, что пришла команда стоп
				else 													// Иначе (Сигнал не изменился)
					result &= ~(0x01<<i);								// Никаких команд не подаем
			}
		}
		else
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
			else								// Иначе (Сигнал не изменился
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
	return result;
}*/
//---------------------------------------------------

void TekModbusParamsUpdate(void) //??? необходимы проверки
{
	TRamGroupT *tek = &g_Ram.ramGroupT;

	// Заполняем технологический регистр
	tek->TechReg.bit.Opened  = g_Ram.ramGroupA.Status.bit.Opened;
	tek->TechReg.bit.Closed  = g_Ram.ramGroupA.Status.bit.Closed;
	tek->TechReg.bit.Mufta1  = g_Ram.ramGroupA.Status.bit.Mufta;
	tek->TechReg.bit.MuDu    = !g_Ram.ramGroupA.Status.bit.MuDu;
	tek->TechReg.bit.Opening = g_Ram.ramGroupA.Status.bit.Opening;
	tek->TechReg.bit.Closing = g_Ram.ramGroupA.Status.bit.Closing;
	tek->TechReg.bit.Stop    = g_Ram.ramGroupA.Status.bit.Stop;
	tek->TechReg.bit.Ten     = g_Ram.ramGroupA.Status.bit.Ten;
	tek->TechReg.bit.Ready   = !g_Ram.ramGroupA.Status.bit.Fault;

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
	tek->DefReg.bit.PhOrdU = 0;//g_Ram.ramGroupA.Faults.Net.bit.PhOrd;
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
	tek->TsTu.bit.InStop 	 = g_Comm.digitInterface.Inputs.bit.StopOpen || g_Comm.digitInterface.Inputs.bit.StopClose;
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
