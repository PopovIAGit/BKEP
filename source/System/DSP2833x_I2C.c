// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:46:27 $
//###########################################################################
//
// FILE:	DSP2833x_I2C.c
//
// TITLE:	DSP2833x SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "chip\DSP2833x_Device.h"     // DSP2833x Headerfile Include File
//#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define I2C_STOP	0x0800
#define I2C_START	0x2000

struct I2CMSG I2cMsg;

static void ResetFifo(void);

//---------------------------------------------------------------------------
// InitI2C: 
//---------------------------------------------------------------------------
// This function initializes the I2C to a known state.
//
void InitI2C(void)
{

	memset(&I2cMsg, 0, sizeof(struct I2CMSG));

// Initialize I2C
  // I2caRegs.I2CSAR = 0x0048;		// Slave address - EEPROM control code

   //#if (CPU_FRQ_150MHZ)             // Default - For 150MHz SYSCLKOUT
	/*	I2caRegs.I2CPSC.all = 14;   // Prescaler - need 7-12 Mhz on module clk (150/15 = 10MHz)
   //#endif

   I2caRegs.I2CCLKL = 10;			// NOTE: must be non zero
   I2caRegs.I2CCLKH = 5;			// NOTE: must be non zero
   I2caRegs.I2CIER.all = 0x24;		// Enable SCD & ARDY interrupts

   I2caRegs.I2CMDR.all = 0x0020;	// Take I2C out of reset
									// Stop I2C when suspended
   I2caRegs.I2CFFTX.all = 0x6000;	// Enable FIFO mode and TXFIFO
   I2caRegs.I2CFFRX.all = 0x2040;	// Enable RXFIFO, clear RXFFINT,*/

		I2caRegs.I2CMDR.all  = 0x00;
		#ifdef CPU_FRQ_150MHZ
		I2caRegs.I2CPSC.all  = 14;
		#endif
		#ifdef CPU_FRQ_100MHZ
		I2caRegs.I2CPSC.all  = 9;
		#endif
		I2caRegs.I2CCLKL     = 45;
		I2caRegs.I2CCLKH     = 50;
		I2caRegs.I2CIER.all  = 0x00;
		I2caRegs.I2CFFTX.all = 0x6000;
		I2caRegs.I2CFFRX.all = 0x2040;
		I2caRegs.I2CMDR.all  = 0x20;

}	

//---------------------------------------------------------------------------
// Example: InitI2CGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as I2C pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 
// Caution: 
// Only one GPIO pin should be enabled for SDAA operation.
// Only one GPIO pin shoudl be enabled for SCLA operation. 
// Comment out other unwanted lines.

void InitI2CGpio(void)
{

   EALLOW;
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.  
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)

/* Configure SCI pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be I2C functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA operation
	
    EDIS;
}

void I2C_update(struct I2CMSG *Msg)
{
	Uint16 i;

	if (I2caRegs.I2CSTR.bit.ARDY)
	{
		if (I2caRegs.I2CSTR.bit.NACK)
		{
			I2caRegs.I2CMDR.bit.STP  = 1;
			I2caRegs.I2CSTR.bit.NACK = 1;
			Msg->Status = I2C_MSGSTAT_NACK;
		}
		else if (Msg->Status == I2C_MSGSTAT_SEND_NOSTOP_BUSY)
		{
			Msg->Status = I2C_MSGSTAT_RESTART;
		}
		I2caRegs.I2CSTR.bit.ARDY = 1;
	}

	if (I2caRegs.I2CSTR.bit.SCD)
	{
		switch(Msg->Status)
		{
			case I2C_MSGSTAT_WRITE_BUSY:
				Msg->Status = I2C_MSGSTAT_INACTIVE;
				break;
			case I2C_MSGSTAT_SEND_NOSTOP_BUSY:
				Msg->Status = I2C_MSGSTAT_SEND_NOSTOP;
				break;
			case I2C_MSGSTAT_READ_BUSY:
   			for (i=0; i < Msg->RxBytes; i++)
					Msg->Buffer[i] = I2caRegs.I2CDRR;
				Msg->Status = I2C_MSGSTAT_INACTIVE;
				break;
		}
		I2caRegs.I2CSTR.bit.SCD = 1;
	}

	switch(Msg->Status)
	{
		case I2C_MSGSTAT_SEND_WITHSTOP:
			if (I2caRegs.I2CMDR.bit.STP) break;
			if (I2caRegs.I2CSTR.bit.BB)  break;
			ResetFifo();

			I2caRegs.I2CSAR = Msg->Slave;
			I2caRegs.I2CCNT = Msg->TxBytes;
   		for (i=0; i < Msg->TxBytes; i++)
				I2caRegs.I2CDXR = Msg->Buffer[i];
			I2caRegs.I2CMDR.all = (0x4620|I2C_STOP|I2C_START);
			Msg->Status = I2C_MSGSTAT_WRITE_BUSY;
			break;

		case I2C_MSGSTAT_SEND_NOSTOP:
			if (I2caRegs.I2CMDR.bit.STP) break;
			if (I2caRegs.I2CSTR.bit.BB)  break;
			ResetFifo();

			I2caRegs.I2CSAR = Msg->Slave;
			I2caRegs.I2CCNT = Msg->TxBytes;
   		for (i=0; i < Msg->TxBytes; i++)
				I2caRegs.I2CDXR = Msg->Buffer[i];
			I2caRegs.I2CMDR.all = (0x4620|I2C_START);
			Msg->Status = I2C_MSGSTAT_SEND_NOSTOP_BUSY;
			break;

		case I2C_MSGSTAT_RESTART:
			if (I2caRegs.I2CMDR.bit.STP) break;
			ResetFifo();

			I2caRegs.I2CSAR = Msg->Slave;
			I2caRegs.I2CCNT = Msg->RxBytes;
			I2caRegs.I2CMDR.all = (0x4420|I2C_STOP|I2C_START);
			Msg->Status = I2C_MSGSTAT_READ_BUSY;
			break;
	}
}

static void ResetFifo(void)
{
	I2caRegs.I2CFFRX.bit.RXFFRST = 0;
	I2caRegs.I2CFFTX.bit.TXFFRST = 0;
	I2caRegs.I2CFFRX.bit.RXFFRST = 1;
	I2caRegs.I2CFFTX.bit.TXFFRST = 1;
}
	
//===========================================================================
// End of file.
//===========================================================================
