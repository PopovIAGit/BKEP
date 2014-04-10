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
	Byte Data = SCI_recieve(hPort->Params.UartID);
	
	if ((Frame->Data - Frame->Buf) < 256)
	{
		*Frame->Data++ = Data;
		StartTimer(&Frame->Timer1_5);
		StartTimer(&Frame->Timer3_5);
	}

	hPort->Stat.RxBytesCount++;
}

//-------------------------------------------------------------------------------
void ModBusTxIsr(TMbPort *hPort)
{
	TMbFrame *Frame = &hPort->Frame;
	
	if ((Frame->Data - Frame->Buf) < Frame->TxLength)
		SCI_transmit(hPort->Params.UartID, *Frame->Data++);
	else StartTimer(&Frame->TimerPost);
	
	hPort->Stat.TxBytesCount++;
}

#endif

