// TI File $Revision: /main/2 $
// Checkin $Date: March 1, 2007   16:06:07 $
//###########################################################################
//
// FILE:	DSP2833x_Sci.c
//
// TITLE:	DSP2833x SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "chip\DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "csl\csl_sci.h"
//#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

volatile struct SCI_REGS *SciRegs[] = {
	&SciaRegs,
	&ScibRegs,
	&ScicRegs
};

//---------------------------------------------------------------------------
// InitSci: 
//---------------------------------------------------------------------------
void InitSciaGpio(void);
void InitScibGpio(void);
void InitScicGpio(void);
//---------------------------------------------------------------------------
// InitSci:
//---------------------------------------------------------------------------
// This function initializes the SCI(s) to a known state.
//
void SCI_init(Byte Id, Uns BaudRate, Byte Parity, Byte Length)
{
	volatile struct SCI_REGS *Regs = SciRegs[Id];

	Regs->SCICTL1.all = 0x0000;
	Regs->SCICTL2.all = 0x0000;
	Regs->SCICCR.all = (Length - 1) & 0x7;
	switch(Parity)
	{
		case 0: Regs->SCICCR.bit.STOPBITS  = 1; break;//1
		case 1: Regs->SCICCR.bit.PARITYENA = 1; break;
		case 2:
			Regs->SCICCR.bit.PARITYENA = 1;
			Regs->SCICCR.bit.PARITY    = 1;
		break;
	}
	Regs->SCIHBAUD = BaudRate >> 8;
	Regs->SCILBAUD = BaudRate & 0x00FF;
	Regs->SCIPRI.all = 0x18;
	Regs->SCICTL1.bit.SWRESET = 1;
}

/*void SCI_init(Uint16 Id, Uint16 BaudRate, Uint16 Parity, Uint16 Length, Uint16 StopBits)
{
	volatile struct SCI_REGS *Regs = SciRegs[Id];

	Regs->SCICTL1.all = 0x0000;
	Regs->SCICTL2.all = 0x0000;
	Regs->SCICCR.all = (Length - 1) & 0x7;

	switch(Parity)
	{
		case 0: break;
		case 1: Regs->SCICCR.bit.PARITYENA = 1; break;
		case 2:
			Regs->SCICCR.bit.PARITYENA = 1;
			Regs->SCICCR.bit.PARITY    = 1;
		break;
	}
	Regs->SCICCR.bit.STOPBITS  = (StopBits == 2);

	Regs->SCIHBAUD = BaudRate >> 8;
	Regs->SCILBAUD = BaudRate & 0x00FF;
	Regs->SCIPRI.all = 0x18;
	Regs->SCICTL1.bit.SWRESET = 1;
}
*/

Byte SCI_recieve(Byte Id)
	{return SciRegs[Id]->SCIRXBUF.all;}
void SCI_transmit(Byte Id, Byte Data)
	{SciRegs[Id]->SCITXBUF = (Data);}
Byte SCI_getstatus(Byte Id)
	{return SciRegs[Id]->SCIRXST.all;}
void SCI_reset(Byte Id)
	{SciRegs[Id]->SCICTL1.bit.SWRESET = 0; SciRegs[Id]->SCICTL1.bit.SWRESET    = 1;}
void SCI_rx_enable(Byte Id)
	{SciRegs[Id]->SCICTL1.bit.RXENA   = 1; SciRegs[Id]->SCICTL2.bit.RXBKINTENA = 1;}
void SCI_rx_disable(Byte Id)
	{SciRegs[Id]->SCICTL1.bit.RXENA   = 0; SciRegs[Id]->SCICTL2.bit.RXBKINTENA = 0;}
void SCI_tx_enable(Byte Id)
	{SciRegs[Id]->SCICTL1.bit.TXENA   = 1; SciRegs[Id]->SCICTL2.bit.TXINTENA   = 1;}
void SCI_tx_disable(Byte Id)
	{SciRegs[Id]->SCICTL1.bit.TXENA   = 0; SciRegs[Id]->SCICTL2.bit.TXINTENA   = 0;}


//---------------------------------------------------------------------------
// Example: InitSciGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as SCI pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 
// Caution: 
// Only one GPIO pin should be enabled for SCITXDA/B operation.
// Only one GPIO pin shoudl be enabled for SCIRXDA/B operation. 
// Comment out other unwanted lines.

void InitSciGpio()
{
   InitSciaGpio();
   InitScibGpio();
   InitScicGpio();
}

void InitSciaGpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.  
// This will enable the pullups for the specified pins.

	//GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
	//GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)

   GpioCtrlRegs.GPBPUD.bit.GPIO35 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
   GpioCtrlRegs.GPBPUD.bit.GPIO36 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)

/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.  
// This will select asynch (no qualification) for the selected pins.

	//GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPBQSEL1.bit.GPIO36 = 3;  // Asynch input GPIO28 (SCIRXDA)

/* Configure SCI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.

	//GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
	//GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation

	GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 1;   // Configure GPIO28 for SCIRXDA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 1;   // Configure GPIO29 for SCITXDA operation

    EDIS;
}

void InitScibGpio(void)
{
   EALLOW;
	
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.  
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

//  GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;     // Enable pull-up for GPIO9  (SCITXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;    // Enable pull-up for GPIO14 (SCITXDB)
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	   // Enable pull-up for GPIO18 (SCITXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;    // Enable pull-up for GPIO22 (SCITXDB)

	
//  GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;    // Enable pull-up for GPIO11 (SCIRXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // Enable pull-up for GPIO15 (SCIRXDB)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	   // Enable pull-up for GPIO19 (SCIRXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;    // Enable pull-up for GPIO23 (SCIRXDB)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

//  GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;  // Asynch input GPIO11 (SCIRXDB)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (SCIRXDB)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (SCIRXDB)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO23 (SCIRXDB)

/* Configure SCI-B pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.
// Comment out other unwanted lines.

//  GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;    // Configure GPIO9 for SCITXDB operation
//  GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;   // Configure GPIO14 for SCITXDB operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO18 for SCITXDB operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;   // Configure GPIO22 for SCITXDB operation
	
//  GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;   // Configure GPIO11 for SCIRXDB operation
//  GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;   // Configure GPIO15 for SCIRXDB operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for SCIRXDB operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;   // Configure GPIO23 for SCIRXDB operation
	
    EDIS;
}


void InitScicGpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.  
// This will enable the pullups for the specified pins.

	GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
	GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;	   // Enable pull-up for GPIO63 (SCITXDC)

/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.  
// This will select asynch (no qualification) for the selected pins.

	GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO62 (SCIRXDC)

/* Configure SCI-C pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.

	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO62 for SCIRXDC operation
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;   // Configure GPIO63 for SCITXDC operation
	
    EDIS;
}


	
//===========================================================================
// End of file.
//===========================================================================
