#if NEW_RAZ

#include "peref.h"
#include "csl\csl_io.h"
#include "csl\csl_spi.h"
#include "g_Structs.h"

// Макросы формирования таймингов
/*
#if defined(_TMS320C2XX)
#define tas_delay()       asm(" RPT #2");  asm(" NOP")
#define tddr_delay()      asm(" RPT #14"); asm(" NOP")
#define tdsw_delay()      asm(" RPT #7");  asm(" NOP")
#define tah_delay()       asm(" RPT #1");  asm(" NOP")
#define tcl_delay()       asm(" RPT #18"); asm(" NOP")

#elif defined(__TMS320C28X__)
*/

#define tas_delay()       asm(" RPT #15 || NOP")
#define tddr_delay()      asm(" RPT #75 || NOP")
#define tdsw_delay()      asm(" RPT #35 || NOP")
#define tah_delay()       asm(" RPT #5  || NOP")
#define tcl_delay()       asm(" RPT #95 || NOP")

/*
#elif defined(__IAR_SYSTEMS_ICC__)
#include <ina90.h>
#define tas_delay()       __delay_cycles(6)
#define tddr_delay()      __delay_cycles(30)
#define tdsw_delay()      __delay_cycles(16)
#define tah_delay()       __delay_cycles(4)
#define tcl_delay()       __delay_cycles(38)

#elif defined(_BCB6_)
#define tas_delay()
#define tddr_delay()
#define tdsw_delay()
#define tah_delay()
#define tcl_delay()
#endif
*/
// Макросы для установки/снятия сигнала выбора микросхемы
#define SetCs()           p->CsFunc(1)
#define ClrCs()           p->CsFunc(0)

#define DISPL_SCALE				(1.000 * Prd2kHZ)		// Масштабный коэф. индикатора
// Макросы вывода команды/данных

#if DISPL
#define PutComm(c)    GpioDataRegs.GPCDAT.bit.GPIO80 = 0; PutData(p, c)
#define PutChar(c)    GpioDataRegs.GPCDAT.bit.GPIO80 = 1; PutData(p, c)

static   void SendData(TDisplay *, Byte Data);
static   void PutData(TDisplay *, Byte Data);
#else
#define PutComm(c)       g_Peref.Display.rs = 0;PutData2(p, c)
#define PutChar(c)       g_Peref.Display.rs = 1;PutData2(p, c)

void PutData2(TDisplay *, Byte Data);
#endif
// Локальные функции
__inline void DisplayRestart(TDisplay *);
__inline void DisplayOff(TDisplay *);
__inline void DisplayOn(TDisplay *);
__inline void DisplayPutText(TDisplay *);
__inline Char EncodeData(Char *table, Char Data);

void DISPL_init(TDisplay *p)
{
    p->SpiId        = SPIA;
    p->SpiBaud      = SPI_BRR(8000);
    p->Enable       = False;
    p->Status       = False;
    p->Restart      = False;
    p->ResTimer     = 0;
    p->ResTout      = 0;
    p->PauseTimer   = 0;
    p->State        = 0;
    p->CursorPos    = 0;
    p->CursorAddr   = 0;
 // p->HiStr = 0;
 // p->LoStr = 0;
    p->Data         = Null;
    p->pSymbolTable = WINSTAR_RusTable;
    p->pIndicatorType = &g_Ram.ramGroupC.IndicatorType;
/*  p->EN = IO_INIT(LCD_PORT, LCD_EN);
    p->RS = IO_INIT(LCD_PORT, LCD_RS);
    p->RW = IO_INIT(LCD_PORT, LCD_RW);*/
//Todo проверить работоспособность после разбивки макроса
 /*   p->EN.Bit = IO_BIT_INIT(LCD_EN);
    p->EN.PortAddr = IO_PORT_INIT(LCD_PORT, LCD_EN);

    p->RS.Bit = IO_BIT_INIT(LCD_RS);
    p->RS.PortAddr = IO_PORT_INIT(LCD_PORT, LCD_RS);

    p->RW.Bit = IO_BIT_INIT(LCD_RW);
    p->RW.PortAddr = IO_PORT_INIT(LCD_PORT, LCD_RW);*/

    p->Port.Addr    = Null;
    p->Port.Bit     = 0;
    p->Port.Mask    = 0UL;
    p->keyTmp = 0;
    p->rs = 0;
}

void DISPL_Update(TDisplay *p)
{
	//if (p->CsFunc)	SPI_init(p->SpiId, SPI_MASTER, 0, p->SpiBaud, 8);

	if (g_Comm.Bluetooth.State < 7 && g_Core.Protections.outDefects.Dev.bit.BlueNoLaunch == 0) return;


	DisplayRestart(p);

	if (!p->Restart)
		DisplayPutText(p);
	else if (!p->Enable)
		DisplayOff(p);
	else
		DisplayOn(p);
}

void DISPL_AddSymb(TDisplay *p, Byte Addr, Ptr Data, Byte Count)
{
	memcpy(&Symbols[Addr][0], Data, Count * 7);
}

__inline void DisplayRestart(TDisplay *p)
{
	if (!p->ResTout) p->ResTimer = 0;
	else if (++p->ResTimer >= p->ResTout) {p->State = 0; p->Restart = TRUE; p->ResTimer = 0;}
	if (p->Status != p->Enable) {p->Status = p->Enable; p->State = 0; p->Restart = TRUE;}
}

__inline void DisplayOff(TDisplay *p)
{
	PutComm(DISPLAY_OFF);
	//SendData(p, 0x00);
	/*IOclr(&p->RW);
	IOclr(&p->EN);
	IOclr(&p->RS);*/
#if DISPL
	GpioDataRegs.GPCDAT.bit.GPIO80 = 0;
	GpioDataRegs.GPCDAT.bit.GPIO81 = 0;
	GpioDataRegs.GPCDAT.bit.GPIO82 = 0;
#else
	Peref_74HC595UpdateDispSendData(&g_Peref.DisplReg, 0x00);
	Peref_74HC595UpdateDispRW(&g_Peref.DisplReg, 0);
	Peref_74HC595UpdateDispEN(&g_Peref.DisplReg, 0);
	Peref_74HC595UpdateDispRS(&g_Peref.DisplReg, 0);
#endif
	p->Restart = FALSE;
}

__inline void DisplayOn(TDisplay *p)
{
	if (p->PauseTimer > 0) {p->PauseTimer--; return;}

	switch (++p->State)
	{
		case 1: if ( *p->pIndicatorType == it_WINSTAR )
				{
					PutComm(WINSTAR_FUNCTION_SET);
				}
				else
				{
					PutComm(FUTABA_FUNCTION_SET);	
				}
											p->PauseTimer = DISPL_PAUSE_TIME;		break;
		case 2:  PutComm(DISPLAY_OFF);		p->PauseTimer = DISPL_PAUSE_TIME;		break;
		case 3:  PutComm(__CLR);			p->PauseTimer = DISPL_PAUSE_TIME;		break;
		case 4:  PutComm(ENTRY_MODE_SET);	p->PauseTimer = DISPL_PAUSE_TIME;		break;
		case 5:  PutComm(DISPLAY_ON);		p->PauseTimer = DISPL_PAUSE_TIME;		break;
		case 6:  PutComm(0x00|__CGADDR);	p->PauseTimer = DISPL_PAUSE_TIME;
				 p->CursorAddr = 0;
				 p->Data = &Symbols[0][0];
				 break;
		default:
			PutChar(((p->CursorAddr & 0x7) != p->CursorPos) ? *p->Data++ : 0x00);
			if (++p->CursorAddr >= 64) {p->State = 0; p->Restart = FALSE;}
	}

}
//--------------------------------------------------------
void DisplayReset(TDisplay *p, Uns index)
{
	if (index == it_WINSTAR)				// Если индикатор OLED WINSTAR
		p->pSymbolTable = WINSTAR_RusTable;	// таблица символов для этого индикатора
	else 									// в остальных случаях это VAC FUTABA
		p->pSymbolTable = FUTABA_RusTable;
	
	p->Enable = true;
	p->Restart = true;
}
//--------------------------------------------------------
__inline void DisplayPutText(TDisplay *p)
{
	if (p->Enable) switch(++p->State)
	{
		case 1:  PutComm(__FSTR); p->Data = p->HiStr; break;
		case 18: PutComm(__SSTR); p->Data = p->LoStr; break;
		default: PutChar(EncodeData(p->pSymbolTable, *p->Data++)); if (p->State == 34) p->State = 0;
	}
}
//--------------------------------------------------------
#if DISPL
static void SendData(TDisplay *p, Byte Data)
{
	SetCs();
	SPI_send(p->SpiId, Data);
	ClrCs();
}



static void PutData(TDisplay *p, Byte Data)
{
/*
	switch (p->keyTmp) {
		case 0:
			g_Peref.DisplReg.ShiftReg.bit.D_Out9 = p->rs;
			g_Peref.DisplReg.ShiftReg.bit.D_Out10 = 0;
			Peref_74HC595UpdateDispSendData(&g_Peref.DisplReg, Data);
			Peref_74HC595UpdateDisp(&g_Peref.DisplReg);
			p->keyTmp = 1;
			break;

		case 1:
			g_Peref.DisplReg.ShiftReg.bit.D_Out11 = 1;
			Peref_74HC595UpdateDisp(&g_Peref.DisplReg);
			p->keyTmp = 2;
			tcl_delay();
			break;

		case 2:
			g_Peref.DisplReg.ShiftReg.bit.D_Out11 = 0;
			Peref_74HC595UpdateDisp(&g_Peref.DisplReg);
			p->keyTmp = 3;
			break;

		case 3:
			g_Peref.DisplReg.ShiftReg.bit.D_Out10 = 1;
			Peref_74HC595UpdateDisp(&g_Peref.DisplReg);
			p->keyTmp = 0;
			break;
	}*/

	//IOclr(&p->RW);
	GpioDataRegs.GPCDAT.bit.GPIO81 = 0;


	//Peref_74HC595UpdateDispRW(&g_Peref.DisplReg, 0);
	tas_delay();

	SendData(p, Data);
	//Peref_74HC595UpdateDispSendData(&g_Peref.DisplReg, Data);

	tdsw_delay();

	//IOset(&p->EN);     					// После отправки данных выставили и сняли строб
	GpioDataRegs.GPCDAT.bit.GPIO82 = 1;
	//Peref_74HC595UpdateDispEN(&g_Peref.DisplReg, 1);
	DelayUs(1);
	//Peref_74HC595UpdateDispEN(&g_Peref.DisplReg, 0);
//	IOclr(&p->EN);
	GpioDataRegs.GPCDAT.bit.GPIO82 = 0;
	tah_delay();

	//Peref_74HC595UpdateDispRW(&g_Peref.DisplReg, 1);
	//IOset(&p->RW);
	GpioDataRegs.GPCDAT.bit.GPIO81 = 1;

	tcl_delay();
}
#else
void PutData2(TDisplay *p, Byte Data)
{
	g_Peref.DisplReg.ShiftReg.bit.D_Out9 = p->rs;
	g_Peref.DisplReg.ShiftReg.bit.D_Out10 = 0;
	Peref_74HC595UpdateDispSendData(&g_Peref.DisplReg, Data);
	g_Peref.DisplReg.ShiftReg.bit.D_Out11 = 1;
	Peref_74HC595UpdateDisp(&g_Peref.DisplReg);

	g_Peref.DisplReg.ShiftReg.bit.D_Out11 = 0;
	g_Peref.DisplReg.ShiftReg.bit.D_Out10 = 1;
	Peref_74HC595UpdateDisp(&g_Peref.DisplReg);
}
#endif

__inline Char EncodeData(Char *table, Char Data)
{
	Data &= 0xFF;
	if (Data == 'Ё')  return 0x40;
	if (Data == 'ё')  return 0x41;
	if ((Int)Data >= 0xC0) return table[Data - 0xC0];
	return Data;
}

void DisplayStart(void)
{
    if (g_Core.DisplayTimer > 0)
    {
        g_Core.DisplayTimer--;
        if (!g_Core.DisplayTimer) g_Peref.Display.Enable = true;
    }

    if (g_Core.DisplayRestartTimer > 0)
    {
        g_Peref.Display.ResTout = 5;
        g_Core.DisplayRestartTimer--;
    }
    else
    {
        g_Peref.Display.ResTout = g_Ram.ramGroupC.DisplResTout * (Uns)DISPL_SCALE;
    }
}

#endif
