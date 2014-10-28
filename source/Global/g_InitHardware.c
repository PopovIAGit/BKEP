/*======================================================================
Имя файла:          g_InitHardware.c
Автор:
Версия файла:
Дата изменения:
Описание:
Модуль конфигурации оборудования
======================================================================*/

#include "g_InitHardware.h"
#include "comm.h"

extern interrupt void adc_isr(void);			// АЦП
interrupt void CpuTimer0IsrHandler(void);		// Ядро
interrupt void SciaRxIsrHandler(void);			// SCI A RX
interrupt void SciaTxIsrHandler(void);			// SCI A TX
interrupt void ScibRxIsrHandler(void);			// SCI B RX
interrupt void ScibTxIsrHandler(void);			// SCI B TX
interrupt void ScicRxIsrHandler(void);			// SCI C RX
interrupt void ScicTxIsrHandler(void);			// SCI C TX
interrupt void McbspRxAHandler(void);			// McBSP A RX
interrupt void McbspTxAHandler(void);			// McBSP A TX

void ADCCinit(void);

void   I2CA_Init(void);
//extern interrupt void i2c_int1a_isr(void);


void InitClocks(void);
void InitGpio(void);

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

//---------------------------------------------------
void InitHardware(void)
{

	// Initialize System Control
	// Получаем системную частоту процессора = SYSCLK (30e6)
	InitSysCtrl();

	// Initalize peripheral
	InitGpio();			// - GPIO
	InitSpiGpio();		// - SPIA
	InitI2CGpio();  	// - I2C
	InitSciGpio();  	// - SCI - BKP-A, ASU-B, SHN-C
	//McBSP				// - SCI - BT
	InitMcbspaGpio();

	// Disable global Interrupts and higher priority real-time debug events

	// Copy time critical code and Flash setup code to RAM
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	// Call Flash Initialization to setup flash waitstates
	InitFlash();

	DINT;
	
	// Initialize the PIE control registers to their default state
	InitPieCtrl();
		
	// Disable CPU interrupts and clear all CPU interrupt flags
	IER = 0x0000;
	IFR = 0x0000;
	
	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR)
	InitPieVectTable();
	
	// User interrupts that are used in this project are re-mapped to
	// ISR functions found within this file
	EALLOW;
	PieVectTable.TINT0     = &CpuTimer0IsrHandler;
	PieVectTable.SCIRXINTA = &SciaRxIsrHandler;
	PieVectTable.SCITXINTA = &SciaTxIsrHandler;
	PieVectTable.SCIRXINTB = &ScibRxIsrHandler;
	PieVectTable.SCITXINTB = &ScibTxIsrHandler;
	//PieVectTable.SCIRXINTC = &ScicRxIsrHandler;
	//PieVectTable.SCITXINTC = &ScicTxIsrHandler;
	PieVectTable.ADCINT    = &adc_isr;
	//PieVectTable.I2CINT1A  = &i2c_int1a_isr;
	PieVectTable.MRINTA= &McbspRxAHandler;
    PieVectTable.MXINTA= &McbspTxAHandler;
	EDIS;

	// Initialize all the Device Peripherals
	InitAdc();

	InitI2C();

	// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
	// 30MHz CPU Freq, 18кГц  (55 in uSeconds)
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0, 150, 55);

	PieCtrlRegs.PIEIER1.bit.INTx7 = 1; // TINT0
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1; // Enable the PIE block
	//SCI-A
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1; // PIE Group 9, INT1 // SCIRXINTA
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1; // PIE Group 9, INT2 // SCITXINTA
	//SCI-B
	PieCtrlRegs.PIEIER9.bit.INTx3 = 1; // PIE Group 9, INT3 // SCIRXINTB
	PieCtrlRegs.PIEIER9.bit.INTx4 = 1; // PIE Group 9, INT4 // SCITXINTB
	//SCI-C
	PieCtrlRegs.PIEIER8.bit.INTx5 = 1; // PIE Group 8, INT5 // SCIRXINTC
	PieCtrlRegs.PIEIER8.bit.INTx6 = 1; // PIE Group 8, INT6 // SCITXINTC
	//McBSP
	PieCtrlRegs.PIEIER6.bit.INTx5 = 1; // PIE Group 6, INT5 // MCBSPRXA
	PieCtrlRegs.PIEIER6.bit.INTx6 = 1; // PIE Group 6, INT6 // MCBSPTXA

	PieCtrlRegs.PIEIER1.bit.INTx6 = 1;

    //IER |= M_INT8;   // для I2C
	IER |= M_INT1;   // для АЦП
	IER |= M_INT6;   // для McBSP
	IER |= M_INT9;   // для SCI A,B,C
	EnableCpuTimer0();

	// Configure ADC

	/*AdcRegs.ADCTRL1.bit.ACQ_PS = 1;  // Sequential mode: Sample rate   = 1/[(2+ACQ_PS)*ADC clock in ns]
	//                     = 1/(3*40ns) =8.3MHz (for 150 MHz SYSCLKOUT)
	//                     = 1/(3*80ns) =4.17MHz (for 100 MHz SYSCLKOUT)
	// If Simultaneous mode enabled: Sample rate = 1/[(3+ACQ_PS)*ADC clock in ns]*/

	AdcRegs.ADCREFSEL.bit.REF_SEL = 0;		// internal

	AdcRegs.ADCTRL3.bit.ADCCLKPS = 4;		// clock prescaler, FCLK=HSPCLK/(2*ADCCLKPS)
	AdcRegs.ADCTRL1.bit.CPS 	 = 0;		// ADCCLK = FCLK/2
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;       // 1  Cascaded mode
	AdcRegs.ADCTRL1.bit.CONT_RUN = 1;       // Setup continuous run
	AdcRegs.ADCTRL1.bit.SUSMOD 	 = 0;
	AdcRegs.ADCTRL1.bit.ACQ_PS 	 = 8;		// 8 // 16
	AdcRegs.ADCTRL1.bit.SEQ_OVRD = 0;

	AdcRegs.ADCMAXCONV.all        = 0x000F; // Setup 2 conv's on SEQ1
	AdcRegs.ADCCHSELSEQ1.all      = 0x3210;
	AdcRegs.ADCCHSELSEQ2.all      = 0x7654;
	AdcRegs.ADCCHSELSEQ3.all      = 0xBA98;
	AdcRegs.ADCCHSELSEQ4.all      = 0xFEDC;

	// Assumes ePWM1 clock is already enabled in InitSysCtrl();
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;        // Enable SOC on A group
	EPwm1Regs.ETSEL.bit.SOCBEN = 1;        // Enable SOC on B group

	EPwm1Regs.ETSEL.bit.SOCASEL = 4;       // Select SOC from from CPMA on upcount
	EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on 1st event

	EPwm1Regs.ETSEL.bit.SOCBSEL = 4;       // Select SOC from from CPMA on upcount
	EPwm1Regs.ETPS.bit.SOCBPRD = 1;        // Generate pulse on 1st event

	EPwm1Regs.CMPA.half.CMPA = 0x0080;	   // Set compare A value
	EPwm1Regs.TBPRD = 0xFFFF;              // Set period for ePWM1
	EPwm1Regs.TBCTL.bit.CTRMODE = 0;	   // count up and start

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

	//входы

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

	GpioCtrlRegs.GPBPUD.bit.GPIO46	= 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO46	= 0;	//кнопка акривации bluetooth
	GpioDataRegs.GPBDAT.bit.GPIO46	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO39	= 0;	//MOD_FAULT - ???
	GpioDataRegs.GPBDAT.bit.GPIO39	= 0;

	//выходы

	GpioCtrlRegs.GPBDIR.bit.GPIO48	= 1;	//RESET_FAULT
	GpioDataRegs.GPBDAT.bit.GPIO48	= 0;

	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;
	GpioCtrlRegs.GPAPUD.bit.GPIO29	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO29	= 1;	// WORK_DSP_LED
	GpioDataRegs.GPADAT.bit.GPIO29	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO3	= 1;	// ENC_ADDR0
	GpioDataRegs.GPADAT.bit.GPIO3	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO4	= 1;	// ENC_ADDR1
	GpioDataRegs.GPADAT.bit.GPIO4	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO5	= 1;	// ENC_ADDR2
	GpioDataRegs.GPADAT.bit.GPIO5	= 0;

	GpioCtrlRegs.GPAPUD.bit.GPIO22	= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO22	= 1;	//LED0
	GpioDataRegs.GPADAT.bit.GPIO22	= 1;

	GpioCtrlRegs.GPAPUD.bit.GPIO23	= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO23	= 1;	//LED1
	GpioDataRegs.GPADAT.bit.GPIO23	= 1;

	GpioCtrlRegs.GPAPUD.bit.GPIO24	= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO24	= 1;	//LED2
	GpioDataRegs.GPADAT.bit.GPIO24	= 1;

	GpioCtrlRegs.GPBPUD.bit.GPIO34	= 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO34	= 1;	//LED3
	GpioDataRegs.GPBDAT.bit.GPIO34	= 1;

	GpioCtrlRegs.GPAPUD.bit.GPIO25	= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO25	= 1;	//LED4
	GpioDataRegs.GPADAT.bit.GPIO25	= 1;

	GpioCtrlRegs.GPAPUD.bit.GPIO26	= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO26	= 1;	//LED5
	GpioDataRegs.GPADAT.bit.GPIO26	= 1;

	GpioCtrlRegs.GPCPUD.bit.GPIO66	= 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO66	= 1;	//LED7
	GpioDataRegs.GPCDAT.bit.GPIO66	= 1;

	GpioCtrlRegs.GPCPUD.bit.GPIO65	= 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO65	= 1;	//LED8
	GpioDataRegs.GPCDAT.bit.GPIO65	= 1;

	GpioCtrlRegs.GPCPUD.bit.GPIO64	= 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO64	= 1;	//LED9
	GpioDataRegs.GPCDAT.bit.GPIO64	= 1;

	GpioCtrlRegs.GPAPUD.bit.GPIO27	= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO27	= 1;	//ENABLE_BLUETOOTH
	GpioDataRegs.GPADAT.bit.GPIO27	= 1;

	GpioCtrlRegs.GPBDIR.bit.GPIO50	= 1;	//RS485_DIR_SHN - УПП
	GpioDataRegs.GPBDAT.bit.GPIO50	= 0;

 	GpioCtrlRegs.GPBDIR.bit.GPIO51	= 1;	//RS485_DIR_ASU	- АСУ
	GpioDataRegs.GPBDAT.bit.GPIO51	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO52	= 1;	//RS485_DIR_BKD	- БКД
	GpioDataRegs.GPBDAT.bit.GPIO52	= 0;

	GpioCtrlRegs.GPBDIR.bit.GPIO53	= 1;	//RS485_DIR_BT	- Bluetooth
	GpioDataRegs.GPBDAT.bit.GPIO53	= 0;

	GpioCtrlRegs.GPCDIR.bit.GPIO79	= 1;	//SC_EEPROM1
	GpioDataRegs.GPCDAT.bit.GPIO79	= 0;

	GpioCtrlRegs.GPCDIR.bit.GPIO78	= 1;	//SC_EEPROM2
	GpioDataRegs.GPCDAT.bit.GPIO78	= 0;

	GpioCtrlRegs.GPADIR.bit.GPIO12	= 1;	//ON_TY - ???
	GpioDataRegs.GPADAT.bit.GPIO12	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO0	= 1;	//TS_1
	GpioDataRegs.GPADAT.bit.GPIO0	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO1	= 1;	//TS_2
	GpioDataRegs.GPADAT.bit.GPIO1	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO2	= 1;	//TS_3
	GpioDataRegs.GPADAT.bit.GPIO2	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO3	= 1;	//TS_4
	GpioDataRegs.GPADAT.bit.GPIO3	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO4	= 1;	//TS_5
	GpioDataRegs.GPADAT.bit.GPIO4	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO5	= 1;	//TS_6
	GpioDataRegs.GPADAT.bit.GPIO5	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO6	= 1;	//TS_7
	GpioDataRegs.GPADAT.bit.GPIO6	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO7	= 1;	//TS_8
	GpioDataRegs.GPADAT.bit.GPIO7	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO8	= 1;	//TS_9
	GpioDataRegs.GPADAT.bit.GPIO8	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO9	= 1;	//TS_10
	GpioDataRegs.GPADAT.bit.GPIO9	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO10	= 1;	//TS_11 - управление контактором ЗАКРЫТЬ
	GpioDataRegs.GPADAT.bit.GPIO10	= 1;

	GpioCtrlRegs.GPADIR.bit.GPIO11	= 1;	//TS_12 - управление контактором ОТКРЫТЬ
	GpioDataRegs.GPADAT.bit.GPIO11	= 1;

	EDIS;
}

//--------------Конец файла--------------------------








