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
