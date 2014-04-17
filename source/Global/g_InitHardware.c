/*======================================================================
��� �����:          g_InitHardware.c
�����:
������ �����:
���� ���������:
��������:
������ ������������ ������������
======================================================================*/

#include "g_InitHardware.h"
#include "comm.h"

extern interrupt void adc_isr(void);			// ���
interrupt void CpuTimer0IsrHandler(void);		// ����
interrupt void SciaRxIsrHandler(void);			// SCI A RX
interrupt void SciaTxIsrHandler(void);			// SCI A TX
interrupt void ScibRxIsrHandler(void);			// SCI B RX
interrupt void ScibTxIsrHandler(void);			// SCI B TX
interrupt void ScicRxIsrHandler(void);			// SCI C RX
interrupt void ScicTxIsrHandler(void);			// SCI C TX

void ADCCinit(void);

void   I2CA_Init(void);
extern interrupt void i2c_int1a_isr(void);


void InitClocks(void);
void InitGpio(void);

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

//---------------------------------------------------
void InitHardware(void)
{
	// Initialize System Control
	// �������� ��������� ������� ���������� = SYSCLK (30e6)
	InitSysCtrl();

	// Initalize peripheral
	InitGpio();			// - GPIO
	//InitSpiGpio();	// - SPIA
	//InitI2CGpio();  	// - I2C
	InitSciGpio();  	// - SCI - BKP-A, ASU-B, SHN-C
	//McBSP				// - SCI - BT

	// Disable global Interrupts and higher priority real-time debug events

	// Copy time critical code and Flash setup code to RAM
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	// Call Flash Initialization to setup flash waitstates
	InitFlash();

	DINT;
	//DRTM;
	
	// Initialize the PIE control registers to their default state
	InitPieCtrl();
		
	// Disable CPU interrupts and clear all CPU interrupt flags
	IER = 0x0000;
	IFR = 0x0000;
	
	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR)
	InitPieVectTable();
	
	//spi_fifo_init();	// Initialize the Spi FIFO
    //spi_init();			// init SPI

	// User interrupts that are used in this project are re-mapped to
	// ISR functions found within this file
	EALLOW;
	PieVectTable.TINT0     = &CpuTimer0IsrHandler;
	PieVectTable.SCIRXINTA = &SciaRxIsrHandler;
	PieVectTable.SCITXINTA = &SciaTxIsrHandler;
	PieVectTable.SCIRXINTB = &ScibRxIsrHandler;
	PieVectTable.SCITXINTB = &ScibTxIsrHandler;
	PieVectTable.SCIRXINTC = &ScicRxIsrHandler;
	PieVectTable.SCITXINTC = &ScicTxIsrHandler;
	PieVectTable.ADCINT    = &adc_isr;
	//PieVectTable.I2CINT1A  = &i2c_int1a_isr;
	EDIS;

	// Initialize all the Device Peripherals
	InitAdc();
	//I2CA_Init();

	// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
	// 30MHz CPU Freq, 18���  (55 in uSeconds)
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0, 150, 55);

	PieCtrlRegs.PIEIER1.bit.INTx7 = 1; // TINT0
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1; // Enable the PIE block

	PieCtrlRegs.PIEIER9.bit.INTx1 = 1; // PIE Group 9, INT1 // SCIRXINTA
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1; // PIE Group 9, INT2 // SCITXINTA

	PieCtrlRegs.PIEIER9.bit.INTx3 = 1; // PIE Group 9, INT3 // SCIRXINTB
	PieCtrlRegs.PIEIER9.bit.INTx4 = 1; // PIE Group 9, INT4 // SCITXINTB

	PieCtrlRegs.PIEIER8.bit.INTx5 = 1; // PIE Group 8, INT5 // SCIRXINTC
	PieCtrlRegs.PIEIER8.bit.INTx6 = 1; // PIE Group 8, INT6 // SCITXINTC

	//PieCtrlRegs.PIEIER1.bit.INTx1 = 1; // ADC
	PieCtrlRegs.PIEIER1.bit.INTx6 = 1;
	//PieCtrlRegs.PIEIER8.bit.INTx1 = 1; // I2C

	// Enable CPU INT8 which is connected to PIE group 8
    //IER |= M_INT8; // ��� I2C
	IER |= M_INT1; // ��� ���
	IER |= 0x100;
	EnableCpuTimer0();

	// Configure ADC

		/*AdcRegs.ADCTRL1.bit.ACQ_PS = 1;  // Sequential mode: Sample rate   = 1/[(2+ACQ_PS)*ADC clock in ns]
	                        //                     = 1/(3*40ns) =8.3MHz (for 150 MHz SYSCLKOUT)
	                        //                     = 1/(3*80ns) =4.17MHz (for 100 MHz SYSCLKOUT)
	                        // If Simultaneous mode enabled: Sample rate = 1/[(3+ACQ_PS)*ADC clock in ns]*/


	AdcRegs.ADCREFSEL.bit.REF_SEL = 0;			// internal


	   AdcRegs.ADCTRL3.bit.ADCCLKPS = 4;		// clock prescaler, FCLK=HSPCLK/(2*ADCCLKPS)
	   AdcRegs.ADCTRL1.bit.CPS 		= 0;		// ADCCLK = FCLK/2
	   AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        // 1  Cascaded mode
	   AdcRegs.ADCTRL1.bit.CONT_RUN = 1;        // Setup continuous run
	   AdcRegs.ADCTRL1.bit.SUSMOD 	= 0;
	   AdcRegs.ADCTRL1.bit.ACQ_PS 	= 16;
	   AdcRegs.ADCTRL1.bit.SEQ_OVRD = 0;

	   AdcRegs.ADCMAXCONV.all = 0x000F;       // Setup 2 conv's on SEQ1
	   AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0; // Setup ADCINA3 as 1st SEQ1 conv.
	   AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1; // Setup ADCINA2 as 2nd SEQ1 conv.
	   AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x3; // Setup ADCINA3 as 1st SEQ1 conv.
   	   AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 0x4; // Setup ADCINA2 as 2nd SEQ1 conv.
   	   AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 0x5; // Setup ADCINA2 as 2nd SEQ1 conv.
   	   AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 0x6; // Setup ADCINA2 as 2nd SEQ1 conv.
   	   AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 0x7; // Setup ADCINA2 as 2nd SEQ1 conv.

   	   AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 0x8; // Setup ADCINA2 as 2nd SEQ1 conv.
   	   AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 0x9; // Setup ADCINA2 as 2nd SEQ1 conv.
   	   AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 0xA; // Setup ADCINA2 as 2nd SEQ1 conv.
   	   AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 0xB; // Setup ADCINA2 as 2nd SEQ1 conv.

   	   AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 0xC; // Setup ADCINA2 as 2nd SEQ1 conv.
   	   AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 0xD; // Setup ADCINA2 as 2nd SEQ1 conv.
   	   AdcRegs.ADCCHSELSEQ4.bit.CONV14 = 0xE; // Setup ADCINA2 as 2nd SEQ1 conv.
   	   AdcRegs.ADCCHSELSEQ4.bit.CONV15 = 0xF; // Setup ADCINA2 as 2nd SEQ1 conv.

   	   AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;// Enable SOCA from ePWM to start SEQ1
   	   AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)

	   AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = 1;// Enable SOCA from ePWM to start SEQ1
   	   AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 1;  // Enable SEQ1 interrupt (every EOS)

   	   // Assumes ePWM1 clock is already enabled in InitSysCtrl();
	   EPwm1Regs.ETSEL.bit.SOCAEN = 1;        // Enable SOC on A group
	   EPwm1Regs.ETSEL.bit.SOCBEN = 1;        // Enable SOC on B group

	   EPwm1Regs.ETSEL.bit.SOCASEL = 4;       // Select SOC from from CPMA on upcount
	   EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on 1st event

	   EPwm1Regs.ETSEL.bit.SOCBSEL = 4;       // Select SOC from from CPMA on upcount
   	   EPwm1Regs.ETPS.bit.SOCBPRD = 1;        // Generate pulse on 1st event

	   EPwm1Regs.CMPA.half.CMPA = 0x0080;	  // Set compare A value
	   EPwm1Regs.TBPRD = 0xFFFF;              // Set period for ePWM1
	   EPwm1Regs.TBCTL.bit.CTRMODE = 0;		  // count up and start


	   AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;
	   AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 1;

}
//---------------------------------------------------
interrupt void  adc_isr(void)
{
  // Reinitialize for next ADC sequence

  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
  AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;         // Reset SEQ2
  AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
  AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;       // Clear INT SEQ2 bit
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE

  return;
}
//---------------------------------------------------
void I2CA_Init(void)
{
   // Initialize I2C
   I2caRegs.I2CSAR = 0x0048;		// Slave address - EEPROM control code

   I2caRegs.I2CPSC.all = 6;		    // Prescaler - need 7-12 Mhz on module clk
   I2caRegs.I2CCLKL = 10;			// NOTE: must be non zero
   I2caRegs.I2CCLKH = 5;			// NOTE: must be non zero
   I2caRegs.I2CIER.all = 0x24;		// Enable SCD & ARDY interrupts

   I2caRegs.I2CMDR.all = 0x0020;	// Take I2C out of reset
   									// Stop I2C when suspended

   I2caRegs.I2CFFTX.all = 0x6000;	// Enable FIFO mode and TXFIFO
   I2caRegs.I2CFFRX.all = 0x2040;	// Enable RXFIFO, clear RXFFINT,

   return;
}
//---------------------------------------------------

void InitClocks(void)
{
	InitPeripheralClocks();

}
//---------------------------------------------------
void InitGpio(void)
{
	EALLOW;

	// Group A pins
	GpioCtrlRegs.GPACTRL.all  = 0x0;	 // QUALPRD = SYSCLKOUT for all group A GPIO
	GpioCtrlRegs.GPAQSEL1.all = 0x0;	 // No qualification for all group A GPIO 0-15
	GpioCtrlRegs.GPAQSEL2.all = 0x0;	 // No qualification for all group A GPIO 16-31
	GpioCtrlRegs.GPAPUD.all   = 0x0;	 // All group A pullups enabled
	GpioCtrlRegs.GPAMUX1.all  = 0x0;	 // All GPIO
	GpioCtrlRegs.GPAMUX2.all  = 0x0;	 // All GPIO
	GpioCtrlRegs.GPADIR.all   = 0x0;	 // All inputs
	
	// Group B pins
	GpioCtrlRegs.GPBCTRL.all  = 0x0;	 // QUALPRD = SYSCLKOUT for all group B GPIO
	GpioCtrlRegs.GPBQSEL1.all = 0x0;	 // No qualification for all group B GPIO 32-35
										 // GPBQSEL2 is reserved on F2808/06/01 devices
	GpioCtrlRegs.GPBPUD.all   = 0x0;	 // All group B pullups enabled
	GpioCtrlRegs.GPBMUX1.all  = 0x0;	 // All GPIO
	//GpioCtrlRegs.GPBMUX2.all  = 0x0;	 // All GPIO
	GpioCtrlRegs.GPBDIR.all   = 0x0;	 // All inputs

	//������
	//GpioCtrlRegs.GPBDIR.bit.GPIO43	= 1;
	//GpioDataRegs.GPBDAT.bit.GPIO43	= 0;

	//---------------------------
	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
	//---------------------------
/*
	//�����

	GpioCtrlRegs.GPBDIR.bit.GPIO40	= 0;	//STATE_TU24
	GpioDataRegs.GPBDAT.bit.GPIO40	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO41	= 0;	//KEY_1
	GpioDataRegs.GPBDAT.bit.GPIO41	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO42	= 0;	//KEY_2
	GpioDataRegs.GPBDAT.bit.GPIO42	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO43	= 0;	//KEY_3
	GpioDataRegs.GPBDAT.bit.GPIO43	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO44	= 0;	//CONTACTOR_1
	GpioDataRegs.GPBDAT.bit.GPIO44	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO45	= 0;	//CONTACTOR_2
	GpioDataRegs.GPBDAT.bit.GPIO45	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO39	= 0;	//MOD_FAULT - ???
	GpioDataRegs.GPBDAT.bit.GPIO39	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO48	= 0;	//RESET_FAULT
	GpioDataRegs.GPBDAT.bit.GPIO48	= 0;

	//������

	GpioCtrlRegs.GPADIR.bit.GPIO19	= 1;	// WORK_DSP_LED
	GpioDataRegs.GPADAT.bit.GPIO19	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO6	= 1;	// OFF_TEN
	GpioDataRegs.GPADAT.bit.GPIO6	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO3	= 1;	// ENC_ADDR0
	GpioDataRegs.GPADAT.bit.GPIO3	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO4	= 1;	// ENC_ADDR1
	GpioDataRegs.GPADAT.bit.GPIO4	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO5	= 1;	// ENC_ADDR2
	GpioDataRegs.GPADAT.bit.GPIO5	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO22	= 1;	//LED0
	GpioDataRegs.GPADAT.bit.GPIO22	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO23	= 1;	//LED1
	GpioDataRegs.GPADAT.bit.GPIO23	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO24	= 1;	//LED2
	GpioDataRegs.GPADAT.bit.GPIO24	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO34	= 1;	//LED3
	GpioDataRegs.GPBDAT.bit.GPIO34	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO25	= 1;	//LED4
	GpioDataRegs.GPADAT.bit.GPIO25	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO26	= 1;	//LED5
	GpioDataRegs.GPADAT.bit.GPIO26	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO27	= 1;	//LED6
	GpioDataRegs.GPADAT.bit.GPIO27	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO50	= 1;	//RS485_DIR_SHN - ���
	GpioDataRegs.GPBDAT.bit.GPIO50	= 0;

 	GpioCtrlRegs.GPBDIR.bit.GPIO51	= 1;	//RS485_DIR_ASU	- ���
	GpioDataRegs.GPBDAT.bit.GPIO51	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO52	= 1;	//RS485_DIR_BKD	- ���
	GpioDataRegs.GPBDAT.bit.GPIO52	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO53	= 1;	//RS485_DIR_BT	- Bluetooth
	GpioDataRegs.GPBDAT.bit.GPIO53	= 0;

	GpioCtrlRegs.GPCDIR.bit.GPIO79	= 1;	//SC_EEPROM1
	GpioDataRegs.GPCDAT.bit.GPIO79	= 0;

	GpioCtrlRegs.GPCDIR.bit.GPIO78	= 1;	//SC_EEPROM2
	GpioDataRegs.GPCDAT.bit.GPIO78	= 0;

	GpioCtrlRegs.GPCDIR.bit.GPIO78	= 1;	//ON_TY - ???
	GpioDataRegs.GPCDAT.bit.GPIO78	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO0	= 1;	//TS_1
	GpioDataRegs.GPADAT.bit.GPIO0	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO1	= 1;	//TS_2
	GpioDataRegs.GPADAT.bit.GPIO1	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO2	= 1;	//TS_3
	GpioDataRegs.GPADAT.bit.GPIO2	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO3	= 1;	//TS_4
	GpioDataRegs.GPADAT.bit.GPIO3	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO4	= 1;	//TS_5
	GpioDataRegs.GPADAT.bit.GPIO4	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO5	= 1;	//TS_6
	GpioDataRegs.GPADAT.bit.GPIO5	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO6	= 1;	//TS_7
	GpioDataRegs.GPADAT.bit.GPIO6	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO7	= 1;	//TS_8
	GpioDataRegs.GPADAT.bit.GPIO7	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO8	= 1;	//TS_9
	GpioDataRegs.GPADAT.bit.GPIO8	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO9	= 1;	//TS_10
	GpioDataRegs.GPADAT.bit.GPIO9	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO10	= 1;	//TS_11
	GpioDataRegs.GPADAT.bit.GPIO10	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO11	= 1;	//TS_12
	GpioDataRegs.GPADAT.bit.GPIO11	= 0;
*/
	EDIS;
}

//------ SPI ----------------------------------------

void spi_init()
{
	SpiaRegs.SPICCR.all =0x000F;	             // Reset on, rising edge, 16-bit char bits
	SpiaRegs.SPICTL.all =0x0006;    		     // Enable master mode, normal phase,
                                                 // enable talk, and SPI int disabled.
	SpiaRegs.SPIBRR =0x007F;
    SpiaRegs.SPICCR.all =0x009F;		         // Relinquish SPI from Reset
    SpiaRegs.SPIPRI.bit.FREE = 1;                // Set so breakpoints don't disturb xmission
}

/*void spi_xmit(Uint16 a)
{
    SpiaRegs.SPITXBUF=a;
} */

void spi_fifo_init()
{
// Initialize SPI FIFO registers
    SpiaRegs.SPIFFTX.all=0xE040;
    SpiaRegs.SPIFFRX.all=0x204f;
    SpiaRegs.SPIFFCT.all=0x0;
}

//--------------����� �����--------------------------







