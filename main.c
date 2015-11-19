/**************************************************************************/
/*                                 main.c                                 */
/**************************************************************************/

#include "core.h"
#include "comm.h"
#include "peref.h"
#include "stat.h"

TRam			g_Ram;
extern void InterruptInit  (void);
extern void InterruptUpdate(void);

void main(void)
{
	// —начала инициализируетс€ процессор
	InitHardware();
	memset(&g_Core, 	0, sizeof(TCore));
	memset(&g_Ram, 	    0, sizeof(TRam));
	memset(&g_Comm, 	0, sizeof(TComm));
	memset(&g_Peref,	0, sizeof(TPeref));
	memset(&g_Stat,		0, sizeof(TStat));

	Core_Init(&g_Core);
	g_Ram_Init(&g_Ram);
	Comm_Init(&g_Comm);
	Peref_Init(&g_Peref);
	Stat_Init(&g_Stat);

	MonitorInit();

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


interrupt void CpuTimer0IsrHandler(void)	//	18 000
{
    InterruptUpdate();

    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}

//-------------------------------------------------------------
interrupt void SciaRxIsrHandler(void)
{
	SciMasterConnBetweenBlockRxHandler(&g_Comm.mbBkp);
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}
//-------------------------------------------------------------
interrupt void SciaTxIsrHandler(void)
{
	SciMasterConnBetweenBlockTxHandler(&g_Comm.mbBkp);
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
}
//-------------------------------------------------------------
interrupt void ScibRxIsrHandler(void)
{
	if (ScibRegs.SCIRXST.bit.BRKDT) SCI_reset(SCIB);
	else ModBusRxIsr(&g_Comm.mbAsu);

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

	PieCtrlRegs.PIEACK.bit.ACK8 = 1;	//???
}
//-------------------------------------------------------------
interrupt void ScicTxIsrHandler(void)
{
	ModBusTxIsr(&g_Comm.mbShn);
	PieCtrlRegs.PIEACK.bit.ACK8 = 1;	//???
}
//-------------------------------------------------------------
interrupt void McbspRxAHandler(void) // прерывание приема данных
{

	BluetoothRxHandler(&g_Comm.Bluetooth, &g_Comm.mbBt);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
}
//-------------------------------------------------------------
interrupt void McbspTxAHandler(void) // прерывание передачи данных
{

	BluetoothTxHandler(&g_Comm.Bluetooth, &g_Comm.mbBt);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
}
//-------------------------------------------------------------
