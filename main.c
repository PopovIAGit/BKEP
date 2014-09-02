/**************************************************************************/
/*                                 main.c                                 */
/**************************************************************************/

#include "core.h"
#include "comm.h"
#include "peref.h"
#include "stat.h"


TRam			g_Ram;
TTekDriveData	g_RamTek;

Uns PassCount=1;

extern void InterruptInit  (void);
extern void InterruptUpdate(void);
extern Uns CrcTable[256];
static Uns CalcFrameCrc1(Byte *Buf, Uns Count);
//Uns I2CA_setCommand(void);
//Uns I2CA_ReadTemper(void);

void main(void)
{

	// Сначала инициализируется процессор
	InitHardware();
	memset(&g_Core, 	0, sizeof(TCore));
	memset(&g_Ram, 	    0, sizeof(TRam));
	memset(&g_RamTek, 	0, sizeof(TTekDriveData));
	memset(&g_Comm, 	0, sizeof(TComm));
	memset(&g_Peref,	0, sizeof(TPeref));
	memset(&g_Stat,		0, sizeof(TStat));
	//

	g_Ram_Init(&g_Ram);
	//g_RamTek_Init(&g_Ram);
	Core_Init(&g_Core);
	Comm_Init(&g_Comm);
	Peref_Init(&g_Peref);
	Stat_Init(&g_Stat);

	MonitorInit();

	InterruptInit();

	StartCpuTimer0();

	EINT;          						// Enable Global interrupt INTM
	ERTM;

	EnableInterrupts();

	while(1)
	{
		Comm_Update(&g_Comm);
		ImUpdate(&g_Stat.Im);
	}
}

/*static Uns CalcFrameCrc1(Byte *Buf, Uns Count)
{

	Uns Crc = 0xFFFF;
	do {Crc = (Crc >> 8) ^ CrcTable[(Crc ^ (Uns)*Buf++) & 0x00FF];}
	while (--Count);
	return Crc;
}*/

interrupt void CpuTimer0IsrHandler(void)	//	18 000
{
	InterruptUpdate();
	PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}

//-------------------------------------------------------------
interrupt void SciaRxIsrHandler(void)
{
	//if (SciaRegs.SCIRXST.bit.BRKDT) SCI_reset(SCIA);
	//else ModBusRxIsr(&g_Comm.mbAsu);
	//g_Core.protections.existConnMB = 1; //связь по MODBUS есть

	//SerialCommRxHandler(&Mb);
	//if (SciaRegs.SCIRXST.bit.BRKDT) SCI_reset(SCIA);

	SciMasterConnBetweenBlockRxHandler(&g_Comm.mbBkp);
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}
//-------------------------------------------------------------
interrupt void SciaTxIsrHandler(void)
{
	//ModBusTxIsr(&g_Comm.mbAsu);
	//SerialCommTxHandler(&Mb);

	SciMasterConnBetweenBlockTxHandler(&g_Comm.mbBkp);
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}
//-------------------------------------------------------------
interrupt void ScibRxIsrHandler(void)
{
	if (ScibRegs.SCIRXST.bit.BRKDT) SCI_reset(SCIB);
	else ModBusRxIsr(&g_Comm.mbAsu);
	//g_Core.protections.existConnMB = 1; //связь по MODBUS есть

	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}
//-------------------------------------------------------------
interrupt void ScibTxIsrHandler(void)
{
	ModBusTxIsr(&g_Comm.mbAsu);
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}
//-------------------------------------------------------------
interrupt void ScicRxIsrHandler(void)
{
	if (ScicRegs.SCIRXST.bit.BRKDT) SCI_reset(SCIC);
	else ModBusRxIsr(&g_Comm.mbShn);
	//g_Core.protections.existConnMB = 1; //связь по MODBUS есть

	PieCtrlRegs.PIEACK.bit.ACK8 = 1;
}
//-------------------------------------------------------------
interrupt void ScicTxIsrHandler(void)
{
	ModBusTxIsr(&g_Comm.mbShn);
	PieCtrlRegs.PIEACK.bit.ACK8 = 1;
}
//-------------------------------------------------------------
interrupt void McbspRxAHandler(void) // прерывание приема данных
{

	BluetoothRxHandler(&g_Comm.Bluetooth, &g_Comm.mbBt);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
}
//-------------------------------------------------------------
interrupt void McbspTxAHandler(void) // прерывание передачи данных
{

	BluetoothTxHandler(&g_Comm.Bluetooth, &g_Comm.mbBt);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
}
//-------------------------------------------------------------
/*Uns I2CA_setCommand(void)
{
   // Wait until the STP bit is cleared from any previous master communication.
   // Clearing of this bit by the module is delayed until after the SCD bit is
   // set. If this bit is not checked prior to initiating a new message, the
   // I2C could get confused.
   if (I2caRegs.I2CMDR.bit.STP == 1)
   {
      return I2C_STP_NOT_READY_ERROR;
   }

   I2caRegs.I2CSAR = 0x0048;

   // Check if bus busy

    I2caRegs.I2CCNT = 1;
    I2caRegs.I2CDXR = 0;
    I2caRegs.I2CMDR.all = 0x2620;			// Send data to setup EEPROM address
    PassCount=0;
   return I2C_SUCCESS;
}

Uns I2CA_ReadTemper(void)
{
   // Wait until the STP bit is cleared from any previous master communication.
   // Clearing of this bit by the module is delayed until after the SCD bit is
   // set. If this bit is not checked prior to initiating a new message, the
   // I2C could get confused.
   if (I2caRegs.I2CMDR.bit.STP == 1)
   {
      return I2C_STP_NOT_READY_ERROR;
   }

   I2caRegs.I2CSAR = 0x0048;

   I2caRegs.I2CCNT = 2;	// Setup how many bytes to expect
   I2caRegs.I2CMDR.all = 0x2C20;			// Send restart as master receiver
   PassCount=0;
   return I2C_SUCCESS;
}

interrupt void i2c_int1a_isr(void)     // I2C-A
{
   Uns IntSource;
   Uns Temper1=0;
   Uns Temper2=0;
   Uns Temper=0;

   // Read interrupt source
   IntSource = I2caRegs.I2CISRC.all;

   // Interrupt source = stop condition detected
   if(IntSource == I2C_SCD_ISRC)
   {
	  Temper1 = I2caRegs.I2CDRR;
	  Temper2 = I2caRegs.I2CDRR;
	  Temper = (Temper1<<8 | Temper2&0xF0) >>4;
	  g_Ram.ramGroupA.Temper = Temper/16;
	  PassCount=1;
   }  // end of stop condition detected

   // Interrupt source = Register Access Ready
   // This interrupt is used to determine when the EEPROM address setup portion of the
   // read data communication is complete. Since no stop bit is commanded, this flag
   // tells us when the message has been sent instead of the SCD flag. If a NACK is
   // received, clear the NACK bit and command a stop. Otherwise, move on to the read
   // data portion of the communication.
   else if(IntSource == I2C_ARDY_ISRC)
   {
      if(I2caRegs.I2CSTR.bit.NACK == 1)
      {
         I2caRegs.I2CMDR.bit.STP = 1;
         I2caRegs.I2CSTR.all = I2C_CLR_NACK_BIT;
         //PassCount=2;
      }
      PassCount=2;
      //Temper = I2caRegs.I2CDRR;
   }  // end of register access ready

   else


   if (PassCount>2)PassCount=0;
   // Enable future I2C (PIE Group 8) interrupts
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}
*/
//-----------------------------------------------
/*interrupt void i2c_int1a_isr(void)     // I2C-B
{
	Uint16 IntSource, i;
	Uint16 Data[2];

	Data[0]=0;
	Data[1]=0;
	   // Read interrupt source
	   IntSource = I2caRegs.I2CISRC.all;

	   // Interrupt source = stop condition detected
	   if(IntSource == I2C_SCD_ISRC)
	   {

	         // If a message receives a NACK during the address setup portion of the
	         // EEPROM read, the code further below included in the register access ready
	         // interrupt source code will generate a stop condition. After the stop
	         // condition is received (here), set the message status to try again.
	         // User may want to limit the number of retries before generating an error.

	            for(i=0; i < 2; i++)
	            {
	            	Data[i] = I2caRegs.I2CDRR;
	            }
	            g_Peref.tempSensor = Data[0] | (Data[1]<<8)&0x00FF; // изменить структуру
	            g_Ram.ramGroup.TEMP_SENS = g_Peref.tempSensor;

	   }  // end of stop condition detected

	   // Interrupt source = Register Access Ready
	   // This interrupt is used to determine when the EEPROM address setup portion of the
	   // read data communication is complete. Since no stop bit is commanded, this flag
	   // tells us when the message has been sent instead of the SCD flag. If a NACK is
	   // received, clear the NACK bit and command a stop. Otherwise, move on to the read
	   // data portion of the communication.
	   else if(IntSource == I2C_ARDY_ISRC)
	   {
	      if(I2caRegs.I2CSTR.bit.NACK == 1)
	      {
	         I2caRegs.I2CMDR.bit.STP = 1;
	         I2caRegs.I2CSTR.all = I2C_CLR_NACK_BIT;
	      }
	   }  // end of register access ready

	   // Enable future I2C (PIE Group 8) interrupts
	   PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

}*/
