  // TI File $Revision: /main/16 $
// Checkin $Date: October 3, 2007   14:50:19 $
//###########################################################################
//
// FILE:	DSP2833x_McBSP.c
//
// TITLE:	DSP2833x Device McBSP Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "csl_mcbsp.h"
//#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//---------------------------------------------------------------------------
// MCBSP_INIT_DELAY determines the amount of CPU cycles in the 2 sample rate
// generator (SRG) cycles required for the Mcbsp initialization routine.
// MCBSP_CLKG_DELAY determines the amount of CPU cycles in the 2 clock
// generator (CLKG) cycles required for the Mcbsp initialization routine.
// For the functions defined in Mcbsp.c, MCBSP_INIT_DELAY and MCBSP_CLKG_DELAY
// are based off of either a 150 MHz SYSCLKOUT (default) or a 100 MHz SYSCLKOUT.
//
// CPU_FRQ_100MHZ and CPU_FRQ_150MHZ are defined in DSP2833x_Examples.h
//---------------------------------------------------------------------------

#if CPU_FRQ_150MHZ                                          // For 150 MHz SYSCLKOUT(default)
  #define CPU_SPD              150E6
  #define MCBSP_SRG_FREQ       CPU_SPD/12                    // SRG input is LSPCLK (SYSCLKOUT/4) for examples
#endif
#if CPU_FRQ_100MHZ                                          // For 100 MHz SYSCLKOUT
  #define CPU_SPD              100E6
  #define MCBSP_SRG_FREQ       CPU_SPD/4                    // SRG input is LSPCLK (SYSCLKOUT/4) for examples
#endif

#define CLKGDV_VAL           0x6C // - 115200 бод/сек
#define MCBSP_INIT_DELAY     2*(CPU_SPD/MCBSP_SRG_FREQ)                  // # of CPU cycles in 2 SRG cycles-init delay
#define MCBSP_CLKG_DELAY     2*(CPU_SPD/(MCBSP_SRG_FREQ/(1+CLKGDV_VAL))) // # of CPU cycles in 2 CLKG cycles-init delay

//void InitMcbspa(void);
//void InitMcbspaGpio(void);
void InitMcbspb(void);
void InitMcbspbGpio(void);

volatile struct MCBSP_REGS *McBspRegs[] = {
	&McbspaRegs,
	&McbspbRegs
};

//---------------------------------------------------------------------------
// InitMcbsp:
//---------------------------------------------------------------------------
// This function initializes the McBSP to a known state.
//

void delay_loop(void);		// Delay function used for SRG initialization
void clkg_delay_loop(void); // Delay function used for CLKG initialization

void InitMcbsp(void)
{
	InitMcbspa();
	#if DSP28_MCBSPB
	  InitMcbspb();
	#endif               // end DSP28_MCBSPB
}

//расчет скорости передачи (LSPCLK/Baud)-1

void InitMcbspa(void)
{
// McBSP-A register settings

    McbspaRegs.SPCR2.all=0x0000;		// Reset FS generator, sample rate generator & transmitter
	McbspaRegs.SPCR1.all=0x0000;		// Reset Receiver, Right justify word
	McbspaRegs.MFFINT.all=0x0;			// Disable all interrupts

    McbspaRegs.RCR2.all=0x0;			// Single-phase frame, 1 word/frame, No companding	(Receive)
    McbspaRegs.RCR1.all=0x0;

    McbspaRegs.XCR2.all=0x0;			// Single-phase frame, 1 word/frame, No companding	(Transmit)
    McbspaRegs.XCR1.all=0x0;

    McbspaRegs.PCR.bit.FSXM = 1;		// FSX generated internally, FSR derived from an external source
	McbspaRegs.PCR.bit.CLKXM = 1;		// CLKX generated internally, CLKR derived from an external source
	McbspaRegs.PCR.bit.SCLKME = 0;
	//или McbspaRegs.PCR.all=0x0A00;

	McbspaRegs.SRGR2.all=0x0000;		//
	//McbspaRegs.SRGR2.all=0x3008;		//
	McbspaRegs.SRGR2.bit.CLKSM = 1;
	McbspaRegs.SRGR1.all=0x0000;		//

	McbspaRegs.SRGR2.bit.FSGM = 0;
	McbspaRegs.SPCR2.bit.FREE = 1;

	McbspaRegs.RCR2.bit.RPHASE = 1;

	//---------------------------
	McbspaRegs.SRGR2.all = 0;
	McbspaRegs.SPCR2.bit.FREE = 1;
	McbspaRegs.SRGR2.bit.CLKSM = 1;
	McbspaRegs.SRGR1.bit.CLKGDV = CLKGDV_VAL;

	McbspaRegs.SPCR1.bit.RINTM = 0;

	McbspaRegs.SPCR2.all = 0;
	McbspaRegs.SPCR2.bit.FREE = 1;

	McbspaRegs.PCR.all = 0;
	McbspaRegs.PCR.bit.FSXM = 1;
	McbspaRegs.PCR.bit.FSRM = 0;
	McbspaRegs.PCR.bit.CLKXM = 1;
	McbspaRegs.PCR.bit.CLKRM = 1;//???
	McbspaRegs.PCR.bit.FSXP = 0;
	McbspaRegs.PCR.bit.FSRP = 1;


	//*****************************
	McbspaRegs.XCR2.bit.XPHASE   =0;
	McbspaRegs.XCR2.bit.XFRLEN2  =0;
	McbspaRegs.XCR2.bit.XWDLEN2  =0;
	McbspaRegs.XCR2.bit.XCOMPAND =0;
	McbspaRegs.XCR2.bit.XFIG 	 =0;
	McbspaRegs.XCR2.bit.XDATDLY  =0;
	McbspaRegs.XCR1.bit.XFRLEN1 = 0;
	McbspaRegs.XCR1.bit.XWDLEN1 = 3;

	//*****************************
	McbspaRegs.RCR2.bit.RPHASE   = 0;
	McbspaRegs.RCR2.bit.RFRLEN2  = 0;
	McbspaRegs.RCR2.bit.RWDLEN2  = 0;
	McbspaRegs.RCR2.bit.RCOMPAND = 0;
	McbspaRegs.RCR2.bit.RFIG 	 = 1;
	McbspaRegs.RCR2.bit.RDATDLY  = 0;
	McbspaRegs.RCR1.bit.RFRLEN1 =  0;
	McbspaRegs.RCR1.bit.RWDLEN1 =  3;


/*	McbspaRegs.RCR2.bit.RPHASE = 1;//. Enable dual-phase frame mode.
	McbspaRegs.RCR1.bit.RFRLEN1 = 8;//. Nine elements in the first phase of the frame.
	McbspaRegs.RCR2.bit.RFRLEN2 = 1;//. Two elements in the second phase of the frame.
	McbspaRegs.RCR1.bit.RWDLEN1 = 0;//. 16-bit words in the first phase (Start bit, data bits).
    McbspaRegs.RCR2.bit.RWDLEN2=0;//. 8-bit words in the second phase (Stop bits).
    McbspaRegs.RCR2.bit.RCOMPAND=0;//. No companding.
    McbspaRegs.RCR2.bit.RFIG = 1;//. For reception, since data line transitions are seen on the FSR pin, unexpected frame
    McbspaRegs.RCR2.bit.RDATDLY=1;//. 1-bit data delay.
*/
	/*McbspaRegs.RCR2.bit.RPHASE   =0;
	McbspaRegs.RCR2.bit.RFRLEN2  =0;
	McbspaRegs.RCR2.bit.RWDLEN2  =0;
	McbspaRegs.RCR2.bit.RCOMPAND =0;
	McbspaRegs.RCR2.bit.RFIG 	 =0;
	McbspaRegs.RCR2.bit.RDATDLY  =0;
	McbspaRegs.RCR1.bit.RFRLEN1 = 0;
	McbspaRegs.RCR1.bit.RWDLEN1 = 3;
	*/
	//McbspaRegs.RCR1.bit.RWDLEN1=2;      // 16-bit word

	//McbspaRegs.SPCR1.bit.RJUST = 2;
	//McbspaRegs.SPCR1.bit.
	/*McbspaRegs.XCR2.bit.XPHASE  = 1;
	McbspaRegs.RCR1.bit.RFRLEN1 = 8;
	McbspaRegs.RCR2.bit.RFRLEN2 = 1;
	McbspaRegs.RCR1.bit.RWDLEN1 = 2;
	McbspaRegs.RCR2.bit.RWDLEN2 = 0;
	McbspaRegs.RCR2.bit.RCOMPAND = 0;
	McbspaRegs.RCR2.bit.RFIG 	 = 1;
	McbspaRegs.XCR2.bit.XDATDLY = 0;
	McbspaRegs.RCR2.bit.RDATDLY = 1;*/
	/*
	 * McbspaRegs.SRGR2.all=0x3008;		//
	McbspaRegs.SRGR2.bit.CLKSM = 1;
	McbspaRegs.SRGR1.all=0x0100;
	 */

	////McbspaRegs.SPCR1.bit.RRST=0;
	//McbspaRegs.SRGR2.bit.CLKSM = 1;		// CLKSM=1 (If SCLKME=0, i/p clock to SRG is LSPCLK)
    ////McbspaRegs.SRGR1.bit.FWID = 0;              // Frame Width = 1 CLKG period
	//McbspaRegs.SPCR1.bit.CLKSTP = 1;
    //McbspaRegs.SRGR1.bit.CLKGDV = CLKGDV_VAL;	// CLKG frequency = LSPCLK/(CLKGDV+1)

    delay_loop();                // Wait at least 2 SRG clock cycles

    McbspaRegs.SPCR2.bit.GRST=1; // Enable the sample rate generator
	clkg_delay_loop();           // Wait at least 2 CLKG cycles
	//McbspaRegs.SPCR2.bit.XRST=1; // Release TX from Reset
	//McbspaRegs.SPCR1.bit.RRST=1; // Release RX from Reset
	//************ Disable TX/RX unit
    McbspaRegs.SPCR2.bit.XRST=1;
    McbspaRegs.SPCR1.bit.RRST=1;
    asm(" RPT #249 || NOP");
    // Frame Sync Generator reset
    McbspaRegs.SPCR2.bit.FRST=1;

}

//-------------------------------------------------------------------------------------------
Uns McBsp_recieve(Byte Id)
{
	Uns Data=0;
	Uns Data1=0;
	Uns Data2=0;
	Byte Data11=0;
	Byte Data22=0;
	LgUns LgData=0;
	Uns Result=0;
	Uns i=0;

	if (McBspRegs[Id]->SPCR1.bit.RRDY==1)
	{

		Data1 = McBspRegs[Id]->DRR1.all;
		Data2 = McBspRegs[Id]->DRR2.all;

		LgData = (LgUns)Data1 | (((LgUns)(Data2)<<16)&0xFFFF0000);

		Data1 = (Uns)((LgData>>11)&0x00ff);
		Data2 = (Uns)((LgData>>1)&0x00ff);

		for (i=0; i<8;i++)
		{
			if ((Data1>>7-i)&0x01==1) Data11 |=(1<<i);
		}
		for (i=0; i<8;i++)
		{
			if ((Data2>>7-i)&0x01==1) Data22 |=(1<<i);
		}

		//Data1 = (Uns)((LgData>>1)&0x00FF);
		//Data2 = (Uns)((LgData>>11)&0x00FF);


		Result = (Uns)Data11 | (Uns)((Data22<<8)&0xFF00);

		return Result;
	}
	else return 0;

}

void McBsp_transmit(Byte Id, Uns Data, Uns Stop)
{
	Byte DataN=0,i=0;

	Uns Data1=0;
	Uns Data2=0;
	LgUns Data3=0;
	if (McBspRegs[Id]->SPCR2.bit.XRDY==1)
	{
		Data1 = Data&0x00FF;
		Data2 = Data>>8;

		DataN = (Data1<<1) + 0x0200;
		Data1 = DataN;
		DataN = 0;
		for (i=0; i<10;i++)
		{
			if ((Data1>>9-i)&0x01==1) DataN |=(1<<i);
		}Data1 = DataN;

		DataN = (Data2<<1) + 0x0200;
		Data2 = DataN;
		DataN=0;
		for (i=0; i<10;i++)
		{
			if ((Data2>>9-i)&0x01==1) DataN |=(1<<i);
		}Data2 = DataN;

		Data3 = (LgUns)(Data2&0x03ff) | ((LgUns)(Data1&0x03ff)<<10);

		if (Stop==1) Data3 |= 0x03ff;
		McBspRegs[Id]->DXR1.all = (Uns)(Data3&0x0000FFFF);
		McBspRegs[Id]->DXR2.all = (Uns)((Data3>>16)&0x0000FFFF);
	}
	/*	for (i=0; i<8;i++)
		{
			if ((Data>>7-i)&0x01==1) DataN |=(1<<i);
		}
	 McBspRegs[Id]->DXR1.all = ((((Uns)DataN<<2)+0x0803)&0x0FFF);}//((((Uns)DataN<<1)+0x0E01)&0x0CFF);}
	 */
	}

/*
 if (McBspRegs[Id]->SPCR2.bit.XRDY==1)
	{
		for (i=0; i<8;i++)
		{
			if ((Data>>7-i)&0x01==1) DataN |=(1<<i);
		}
//		D = ((((Uns)DataN<<1)+0x0E01)&0x0CFF);
	 McBspRegs[Id]->DXR1.all = ((((Uns)DataN<<2)+0x0803)&0x0FFF);}//((((Uns)DataN<<1)+0x0E01)&0x0CFF);}
	}
 */
/*
void McBsp_transmit(Byte Id, Byte Data)
	{ if (McBspRegs[Id]->SPCR2.bit.XRDY==1) McBspRegs[Id]->DXR1.all = (Data&0x00FF);}
*/

Byte McBsp_getstatus(Byte Id)
{
	Byte Error=0;

	if (McBspRegs[Id]->SPCR1.bit.RSYNCERR == 1){ // ошибка синхронизации по приёму
		Error |= MCBSP_RX_SYNC_ERROR;
	}
	if (McBspRegs[Id]->SPCR2.bit.XSYNCERR == 1){ // ошибка синхронизации при передаче
		Error |= MCBSP_TX_SYNC_ERROR;
	}
    return Error;
}

void McBsp_reset(Byte Id)
	{ McBspRegs[Id]->SPCR1.bit.RSYNCERR = 0;// -- сброс приемника
	  McBspRegs[Id]->SPCR2.bit.XSYNCERR = 0;// -- сброс передатчика
	}
void McBsp_rx_enable(Byte Id)
	{McBspRegs[Id]->MFFINT.bit.RINT = 1; McBspRegs[Id]->SPCR1.bit.RRST=1;}
void McBsp_rx_disable(Byte Id)
	{McBspRegs[Id]->MFFINT.bit.RINT = 0; McBspRegs[Id]->SPCR1.bit.RRST=0;}
void McBsp_tx_enable(Byte Id)
	{McBspRegs[Id]->MFFINT.bit.XINT = 1; McBspRegs[Id]->SPCR2.bit.XRST=1;}
void McBsp_tx_disable(Byte Id)
	{McBspRegs[Id]->MFFINT.bit.XINT = 0; McBspRegs[Id]->SPCR2.bit.XRST=0;}
//-------------------------------------------------------------------------------------------

#if (DSP28_MCBSPB)
void InitMcbspb(void)
{

// McBSP-B register settings

    McbspbRegs.SPCR2.all=0x0000;		// Reset FS generator, sample rate generator & transmitter
	McbspbRegs.SPCR1.all=0x0000;		// Reset Receiver, Right justify word
	McbspbRegs.SPCR1.bit.DLB = 1;       // Enable loopback mode for test. Comment out for normal McBSP transfer mode.

	McbspbRegs.MFFINT.all=0x0;			// Disable all interrupts

    McbspbRegs.RCR2.all=0x0;			// Single-phase frame, 1 word/frame, No companding	(Receive)
    McbspbRegs.RCR1.all=0x0;

    McbspbRegs.XCR2.all=0x0;			// Single-phase frame, 1 word/frame, No companding	(Transmit)
    McbspbRegs.XCR1.all=0x0;

    McbspbRegs.SRGR2.bit.CLKSM = 1;		// CLKSM=1 (If SCLKME=0, i/p clock to SRG is LSPCLK)
	McbspbRegs.SRGR2.bit.FPER = 31;		// FPER = 32 CLKG periods

    McbspbRegs.SRGR1.bit.FWID = 0;              // Frame Width = 1 CLKG period
    McbspbRegs.SRGR1.bit.CLKGDV = CLKGDV_VAL;	// CLKG frequency = LSPCLK/(CLKGDV+1)

   	McbspbRegs.PCR.bit.FSXM = 1;		// FSX generated internally, FSR derived from an external source
   	McbspbRegs.PCR.bit.FSRM = 1;//*
   	McbspbRegs.PCR.bit.CLKXM = 1;		// CLKX generated internally, CLKR derived from an external source
    delay_loop();                // Wait at least 2 SRG clock cycles
    McbspbRegs.SPCR2.bit.GRST=1; // Enable the sample rate generator
	clkg_delay_loop();           // Wait at least 2 CLKG cycles
	McbspbRegs.SPCR2.bit.XRST=1; // Release TX from Reset
	McbspbRegs.SPCR1.bit.RRST=1; // Release RX from Reset
    McbspbRegs.SPCR2.bit.FRST=1; // Frame Sync Generator reset

}


#endif // end DSP28_MCBSPB

// McBSP-A Data Lengths
void InitMcbspa8bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1=0;     // 8-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=0;     // 8-bit word
}

void InitMcbspa12bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1=1;     // 12-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=1;     // 12-bit word
}

void InitMcbspa16bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1=2;      // 16-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=2;      // 16-bit word
}

void InitMcbspa20bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1=3;     // 20-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=3;     // 20-bit word
}

void InitMcbspa24bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1=4;     // 24-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=4;     // 24-bit word
}

void InitMcbspa32bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1=5;     // 32-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=5;     // 32-bit word
}

// McBSP-B Data Lengths
#if (DSP28_MCBSPB)

void InitMcbspb8bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1=0;     // 8-bit word
    McbspbRegs.XCR1.bit.XWDLEN1=0;     // 8-bit word
}

void InitMcbspb12bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1=1;     // 12-bit word
    McbspbRegs.XCR1.bit.XWDLEN1=1;     // 12-bit word
}

void InitMcbspb16bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1=2;      // 16-bit word
    McbspbRegs.XCR1.bit.XWDLEN1=2;      // 16-bit word
}

void InitMcbspb20bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1=3;     // 20-bit word
    McbspbRegs.XCR1.bit.XWDLEN1=3;     // 20-bit word
}

void InitMcbspb24bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1=4;     // 24-bit word
    McbspbRegs.XCR1.bit.XWDLEN1=4;     // 24-bit word
}

void InitMcbspb32bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1=5;     // 32-bit word
    McbspbRegs.XCR1.bit.XWDLEN1=5;     // 32-bit word
}

#endif //end DSP28_MCBSPB



void InitMcbspGpio(void)
{
	InitMcbspaGpio();
	#if DSP28_MCBSPB
	  InitMcbspbGpio();
	#endif               // end DSP28_MCBSPB
}

void InitMcbspaGpio(void)
{
	EALLOW;

/* Configure McBSP-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be McBSP functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 2;	// GPIO20 is MDXA pin
	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 2;	// GPIO21 is MDRA pin
    //for my 	 GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 2;	// GPIO22 is MCLKXA pin
	//for my     GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 2;		// GPIO7 is MCLKRA pin (Comment as needed)
	//GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 1;	// GPIO58 is MCLKRA pin (Comment as needed)
	//for my 	 GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 2;	// GPIO23 is MFSXA pin
	//for my 	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 2;		// GPIO5 is MFSRA pin (Comment as needed)
	GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 1;	// GPIO59 is MFSRA pin (Comment as needed)

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;     // Enable pull-up on GPIO20 (MDXA)
	GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;     // Enable pull-up on GPIO21 (MDRA)
	//for my 	 GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;     // Enable pull-up on GPIO22 (MCLKXA)
	//for my 	 GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;      // Enable pull-up on GPIO7 (MCLKRA) (Comment as needed)
	//GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0;   // Enable pull-up on GPIO58 (MCLKRA) (Comment as needed)
	//for my   	 GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;     // Enable pull-up on GPIO23 (MFSXA)
	//GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;      // Enable pull-up on GPIO5 (MFSRA) (Comment as needed)
	GpioCtrlRegs.GPBPUD.bit.GPIO59 = 1;   // Enable pull-up on GPIO59 (MFSRA) (Comment as needed)

/* Set qualification for selected input pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.


    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3;   // Asynch input GPIO21 (MDRA)
    //for my 	 GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 3;   // Asynch input GPIO22 (MCLKXA)
    //for my 	 GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 3;    // Asynch input GPIO7 (MCLKRA) (Comment as needed)
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3; // Asynch input GPIO58(MCLKRA) (Comment as needed)
    //for my 	 GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;   // Asynch input GPIO23 (MFSXA)
    //for my  	 GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 3;    // Asynch input GPIO5 (MFSRA) (Comment as needed)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 3; // Asynch input GPIO59 (MFSRA) (Comment as needed)

	EDIS;
}

#if DSP28_MCBSPB
void InitMcbspbGpio(void)
{
    EALLOW;
/* Configure McBSP-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be McBSP functional pins.
// Comment out other unwanted lines.
/*
	//GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 3;	// GPIO12 is MDXB pin (Comment as needed)
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 3;	// GPIO24 is MDXB pin (Comment as needed)
	//GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 3;	// GPIO13 is MDRB pin (Comment as needed)
	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 3;	// GPIO25 is MDRB pin (Comment as needed)
	//GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 3;	// GPIO14 is MCLKXB pin (Comment as needed)
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 3;	// GPIO26 is MCLKXB pin (Comment as needed)
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 3;		// GPIO3 is MCLKRB pin (Comment as needed)
	//GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 1;	// GPIO60 is MCLKRB pin (Comment as needed)
	//GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 3;	// GPIO15 is MFSXB pin (Comment as needed)
	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 3;	// GPIO27 is MFSXB pin (Comment as needed)
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 3;		// GPIO1 is MFSRB pin (Comment as needed)
	//GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 1;	// GPIO61 is MFSRB pin (Comment as needed)
*/
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.
/*	GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;	    // Enable pull-up on GPIO24 (MDXB) (Comment as needed)
	//GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;	// Enable pull-up on GPIO12 (MDXB) (Comment as needed)
	GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;	    // Enable pull-up on GPIO25 (MDRB) (Comment as needed)
	//GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;	// Enable pull-up on GPIO13 (MDRB) (Comment as needed)
	GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;	    // Enable pull-up on GPIO26 (MCLKXB) (Comment as needed)
	//GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;	// Enable pull-up on GPIO14 (MCLKXB) (Comment as needed)
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;		// Enable pull-up on GPIO3 (MCLKRB) (Comment as needed)
	//GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0;	// Enable pull-up on GPIO60 (MCLKRB) (Comment as needed)
	GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0;	    // Enable pull-up on GPIO27 (MFSXB) (Comment as needed)
	//GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;	// Enable pull-up on GPIO15 (MFSXB) (Comment as needed)
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;		// Enable pull-up on GPIO1 (MFSRB) (Comment as needed)
	//GpioCtrlRegs.GPBPUD.bit.GPIO61 = 0;	// Enable pull-up on GPIO61 (MFSRB) (Comment as needed)
*/

/* Set qualification for selected input pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.
/*     GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 3;   // Asynch input GPIO25 (MDRB) (Comment as needed)
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3; // Asynch input GPIO13 (MDRB) (Comment as needed)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 3;   // Asynch input GPIO26(MCLKXB) (Comment as needed)
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3; // Asynch input GPIO14 (MCLKXB) (Comment as needed)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 3;    // Asynch input GPIO3 (MCLKRB) (Comment as needed)
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3; // Asynch input GPIO60 (MCLKRB) (Comment as needed)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 3;   // Asynch input GPIO27 (MFSXB) (Comment as needed)
	//GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3; // Asynch input GPIO15 (MFSXB) (Comment as needed)
	GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 3;    // Asynch input GPIO1 (MFSRB) (Comment as needed)
	//GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3; // Asynch input GPIO61 (MFSRB) (Comment as needed)
*/

	EDIS;
}
#endif                // end DSP28_MCBSPB

void delay_loop(void)
{
    long      i;
    for (i = 0; i < MCBSP_INIT_DELAY; i++) {} //delay in McBsp init. must be at least 2 SRG cycles
}

void clkg_delay_loop(void)
{
    long      i;
    for (i = 0; i < MCBSP_CLKG_DELAY; i++) {} //delay in McBsp init. must be at least 2 SRG cycles
}
//===========================================================================
// No more.
//===========================================================================











