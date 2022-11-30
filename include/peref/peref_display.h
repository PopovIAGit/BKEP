/*======================================================================
��� �����:          displ.h
�����:              ������ �.�.
������ �����:       01.03
���� ���������:		03/03/10
�������������:      ��������� � ����������� CSL
��������:
������� ������������� ����������
======================================================================*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "std.h"
#include "csl_io.h"

#ifdef __cplusplus
extern "C" {
#endif


#define LCD_PORT        GpioDataRegs.GPCDAT.all

#define LCD_RW          81
#define LCD_EN          82
#define LCD_RS          80


// ��������� ��� ������ � �����������
typedef struct _TDisplay {
	Byte   SpiId;        // ������������� SPI-����� (0-SPIA, 1-SPIB, ...)
	Uns    SpiBaud;      // ������� ��������������� (����������� ��� ������������ ����)
	Bool   Enable;       // ���������/���������� ����������
	Bool   Status;		 // ������� ��������� ������
	Bool   Restart;      // ����� ����������
	Uns    ResTimer; 	 // ������ ��� ������
	Uns    ResTout;  	 // ����-��� ������
	Uns    PauseTimer;	 // ������ ��� ������������ �����
	Byte   State;        // ������� ��������� ���������� ������
	Byte   CursorPos;    // ������� ��� �������
	Byte   CursorAddr;	 // ����� ��� ���������� ��������
	Char   HiStr[17];    // ����� ������� ������� ����������
	Char   LoStr[17];    // ����� ������ ������� ����������
	Char  *Data;         // ��������� �� ��������� ���� ������
	Char  *pSymbolTable; // ��������� �� ������� ��������
	Uns   *pIndicatorType;	// ��������� ��� ����������
	IOpin  EN;           // ����� ��� ������������ ������
	IOpin  RS;           // ����� ������ �������/������
	IOpin  RW;           // ����� ������ ������/������
	IOport Port;         // ���� ������
	Uns    data;         // ��� ���������� � �������� ����������
	Uns    keyTmp;
	Uns    rs;
	void (*CsFunc)(Byte);// ������� ������ ����������
} TDisplay;

__inline void DISPL_ClrHiStr(TDisplay *p) {memset(p->HiStr, ' ', 16); p->HiStr[16] = '\0';}
__inline void DISPL_ClrLoStr(TDisplay *p) {memset(p->LoStr, ' ', 16); p->LoStr[16] = '\0';}

__inline void DISPL_PutHiStr(TDisplay *p, Ptr Adr) {PFUNC_blkRead(Adr, (Ptr)p->HiStr, 16);}
__inline void DISPL_PutLoStr(TDisplay *p, Ptr Adr) {PFUNC_blkRead(Adr, (Ptr)p->LoStr, 16);}



void DISPL_init(TDisplay *);
void DISPL_Update(TDisplay *);
void DISPL_AddSymb(TDisplay *, Byte Addr, Ptr Data, Byte Count);
void DisplayStart(void);

#ifdef __cplusplus
}
#endif // extern "C"

#endif





