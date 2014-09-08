/*======================================================================
Имя файла:          modbus_interrupts.h
Автор:              
Версия файла:       01.00
Описание:
Модуль обработки прерываний приема и передачи
======================================================================*/

#ifndef MODBUS_INTERRUPTS_
#define MODBUS_INTERRUPTS_

//-------------------------------------------------------------------------------
void ModBusRxIsr(TMbPort *hPort)
{
	TMbFrame *Frame = &hPort->Frame;
	Byte Data = 0;

	if (hPort->Params.HardWareType==UART_TYPE)
	{
		Data = SCI_recieve(hPort->Params.ChannelID);
	}
	else if (hPort->Params.HardWareType==MCBSP_TYPE) Data = McBsp_recieve(hPort->Params.ChannelID);
	
	if ((Frame->Data - Frame->Buf) < 256)
	{
		if (hPort->Params.HardWareType==MCBSP_TYPE)
		{
			*Frame->Data++ = (char)(Data&0x00FF);
			*Frame->Data++ = (char)(Data>>8)&0x00ff;
		}else
		{
			*Frame->Data++ = Data;
		}
		StartTimer(&Frame->Timer1_5);
		StartTimer(&Frame->Timer3_5);
	}

	hPort->Stat.RxBytesCount++;
}

//-------------------------------------------------------------------------------
void ModBusTxIsr(TMbPort *hPort)
{
	TMbFrame *Frame = &hPort->Frame;
	Uns DataSend=0;
	Uns Stop=0;
	
	if ((Frame->Data - Frame->Buf) < Frame->TxLength){
		if (hPort->Params.HardWareType==UART_TYPE)
		{
			SCI_transmit(hPort->Params.ChannelID, *Frame->Data++);
			StartTimer(&Frame->TimerPost);
		}
		else if (hPort->Params.HardWareType==MCBSP_TYPE)
		{
			if (((Frame->TxLength)&0x01) && ((Frame->Data - Frame->Buf)==1))
			{
				Stop = 1;
				DataSend = ((*Frame->Data++)&0x00FF)|((*Frame->Data++<<8)&0xFF00);
				McBsp_transmit(hPort->Params.ChannelID, DataSend, Stop);
				//StartTimer(&Frame->TimerPost);
			} else
			{
				DataSend = ((*Frame->Data++)&0x00FF)|((*Frame->Data++<<8)&0xFF00);
				McBsp_transmit(hPort->Params.ChannelID, DataSend, 0);
				//StartTimer(&Frame->TimerPost);
			}

			Stop=0;
			//
		}
	}
	else
	{
		StartTimer(&Frame->TimerPost);
	}
	//StartTimer(&Frame->TimerPost);
	
	hPort->Stat.TxBytesCount++;
}

#endif

