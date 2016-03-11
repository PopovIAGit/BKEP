/*======================================================================
��� �����:          core_Menu.h
�����:
������ �����:
���� ���������:
�������������:      
��������:
���������� ��� ������ � ���� ��� ������������ ���������
======================================================================*/

#ifndef CORE_MENU_
#define CORE_MENU_

#include "config.h"
#include "core_MenuGlobal.h"

#define GROUP_EDIT_PAR		1
#define GROUP_FACT_PAR		2

#define GetDcr(Adr, pDcr)	Core_MenuReadDcr(&g_Core.menu, pDcr, Adr)

#define IsPassword1()		(g_Ram.ramGroupH.Password1 != 0)
#define IsPassword2()		(g_Ram.ramGroupH.Password2 != 0)

#define KEY_NONE       0  // �������
#define KEY_OPEN       1  // �������
#define KEY_CLOSE      2  // �������
#define KEY_STOP       3  // ����
#define KEY_UP         4  // �����
#define KEY_LEFT       5  // ����� (��� �������������� ���������)
#define KEY_RIGHT      6  // ������ (��� �������������� ���������)
#define KEY_DOWN       7  // ����
#define KEY_ESCAPE     8  // �����
#define KEY_ENTER      9  // ����
#define KEY_MENU       10 // ����				// �� ������������
#define KEY_MU_DU      11 // ����				// �� ������������
#define KEY_PROG       12 // ����������������	// �� ������������

#define KEY_FORWARD		KEY_CLOSE   // ������	// �� ������������
#define KEY_REVERSE		KEY_OPEN    // �����	// �� ������������


// ��������� ����
typedef struct TCoreMenu {
	Byte   State;        // ����� ����
	Char   Key;          // ����������� �������
	Byte   EditType;     // ��� ������ ��������������
	Byte   Level;        // ������� ������� ����
	Byte   MaxLevel;     // ������� ����������� ���� (1 ��� 2)
	Uns   *pIndicatorType;   // ���� ���������� ���������
	Uns	   indicatorTypeIndex;	// ������ ���� ����������
	Bool   updateMainValues;// ���� ���������� ������� ��������
	Uns	   groupAUpdateTimer;// ������ ���������� �������� � ������ �
	Bool   updateHiStr;
	Bool   updateLowStr;
	Uns	   updateTime;	 // ����� ���������� ������ � ������ �
	Uns	   timeStartLogo;// ������ ������ ��������
	Bool   logoIsShown;	 // ���� ������ ��������
	Bool   ShowReserved; // ���� ����������� ��������
	Uns   *MinMaxGain;   // ����������� ���������
	Byte   setDefaultGroupNumber; // ����� ������ ��� ��������� ���������� ��-��������� 1-B 2-C
	Uns    predStartShow;	// ���������� ����� ���������� ����
	Uns    prevPositionPr;	// ���������� �������� ��������
	Uns	   prevTorque;		// ���������� �������� �������
	Uns	   prevAngl;		// ���������� �������� ���� �������� ����������
	Uns    Value;        // ������� �������� ���������
	Byte   Rate;         // ������� ������
	Byte   EditRate;     // ������������ ������ ��� ��������������
	Bool   Blink;        // ��������� ������� ��������
	Uns    BlinkTimer;   // ������ ��� �������
	Uns    BlinkHiTime;  // ����� ����������� ��� ��������������
	Uns    BlinkLoTime;  // ����� ������� ��� ��������������
	Byte   Bit;          // ��������� ���� ����� � ���� ������� ������
	Uns    BitTimer;     // ������ ����� ��� ������ ����
	Uns    BitTime;      // ����� ��� ������ ����

	Bool   SleepActive;  // ��������� ������
	LgUns  SleepTimer;   // ������ ������
	LgUns  SleepTime;    // ����-��� ������
	Bool  *pCalcelFlag;
	Bool  *pDriveTypeFlag;//��� ������� �� �����
	Uns    ShowTimer;	 // ������ ������ ���. �����

	Uns   *Data;         // ��������� �� ����� ������
	Char  *HiString;     // ��������� �� ����� ������� ������� ����������
	Char  *LoString;     // ��������� �� ����� ������ ������� ����������
	Char   BufValue[6];  // ��������� ����� ��� �������������� � ������ 1
	Char   BufTmp[17];   // ��������� ����� ��� �������� ������� ���������/���������� ��������
	
	Bool   EvLogSelected;// ���� ������ ������
	Uns    EvLogGroup;   // ����� ������ �������
	Uns    EvLogNumber;  // ����� ��������� ������� ������ � ������
	Uns    EvLogTime;    // ����� ��������� ������� ������ �������
	Uns    EvLogDate;    // ����� ��������� ���� ������ �������
	
	struct MENU_CRD Group;     // ��������� ���������� ������
	struct MENU_CRD SubGroup;  // ��������� ���������� ���������
	struct MENU_CRD EvLog;     // ��������� ���������� ������ �������
	struct MENU_CRD Param;     // ��������� ���������� ���������
	struct MENU_DCR Dcr;       // ��������� �������� ���������
	
	const struct MENU_GROUP  *Groups;   // ��������� �� ��������� �����
	const struct MENU_GROUP  *SubGroups;// ��������� �� ��������� ��������
	const struct MENU_PARAM  *Params;   // ��������� �� ��������� ����������
	const struct MENU_STRING *Values;   // ��������� �� ��������� ��������� ��������
	const struct ADDR_PARAM_ATS48  *AtsParams;   // ��������� �� ��������� ������� ���������� ���
	struct MENU_STRING        Rsvd;     // ��������� ������
	
	void (*StartDispl)(String Text, Byte Num);
	Bool (*EnableEdit)(Uns PaswwPrt);
	Bool (*WriteValue)(Uns Memory, Uns ParAddr, Uns *Value);
} TCoreMenu, *pCoreMenu;

void Core_MenuInit(TCoreMenu *);
void Core_MenuUpdate(TCoreMenu *);

//TODO ������� ���������� ����
Bool UpdateCode(Uns, Uns, Uns, Uns);
Bool SleepMode(TCoreMenu *);
void StartState(TCoreMenu *);

//__inline Bool ReadLog(TCoreMenu *);

void SelectGroup(TCoreMenu *);
void SelectParam(TCoreMenu *);
__inline void EditParam(TCoreMenu *);
__inline void EditDec(TCoreMenu *, Char);
__inline void EditStr(TCoreMenu *, Char);
__inline void EditTime(TCoreMenu *, Char);
__inline void EditDate(TCoreMenu *, Char);
__inline void EditBin(TCoreMenu *);
void ReadDcrFull(TCoreMenu *, Uns);
void ChangeCoordinate(TCoreMenu *, Char, Bool);
void ShowStart(TCoreMenu *);
void ShowGroup(TCoreMenu *);
void ShowParam(TCoreMenu *, Bool);
__inline void ShowValue(TCoreMenu *, Uns, Bool);
__inline Byte DefineBit(TCoreMenu *, Uns);

void Core_MenuDisplay(TCoreMenu *);
void Core_MenuReadDcr(TCoreMenu *, struct MENU_DCR *, Uns);

void StartDispl(String, Byte);
Bool EnableEdit(Uns);
Bool WriteValue(Uns, Uns, Uns *);
void SetDefaultValues(TCoreMenu *, Byte *);
void ShowAddStrings(TCoreMenu *, Uns, Uns);
void SetModBusParams(void);
void SetModBusParams(void);

extern void PFUNC_strRead(Char *, Char *);
extern void PFUNC_strWrite(Char *, Char *);
extern  Int PFUNC_wordRead(Int *);
extern void PFUNC_wordWrite(Int *, Int);

#endif



