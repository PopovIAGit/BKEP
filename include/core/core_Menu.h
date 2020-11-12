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

#if !NEW_RAZ
#define GetDcr(Adr, pDcr)	Core_MenuReadDcr(&g_Core.menu, pDcr, Adr)
#else
#define GetDcr(Adr, pDcr)	Core_MenuReadDcr(&/*g_Core.*/menu, pDcr, Adr)
#endif

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


#define BLINK_HI_TOUT			(0.500 * Prd10HZ)		// ����� ����������� ��� ��������������
#define BLINK_LO_TOUT			(0.300 * Prd10HZ)		// ����� ������� ��� ��������������
#define BIT_TOUT				(1.000 * Prd10HZ)		// ����� ��� ������ ����

#define PutAddHiStr(Addr)			DISPL_PutHiStr(&g_Peref.Display, ToPtr(&AddStr[Addr]))
#define PutAddLoStr(Addr)			DISPL_PutLoStr(&g_Peref.Display, ToPtr(&AddStr[Addr]))

#define GetSetParPercent(A)			_IQrmpy(A, _IQ(50.0/RAM_DATA_SIZE))

#define MENU_DEFAULT { \
    MS_START, 0, 0, 0, 1, True, True, False, \
    &g_Ram.ramGroupC.MaxTorque, \
    0, 0, 0, False, 0, BLINK_HI_TOUT, BLINK_LO_TOUT, 0xFF, 0, BIT_TOUT, \
    False, 0, 0, \
    ToPtr(&g_Ram), g_Peref.Display.HiStr, g_Peref.Display.LoStr, \
    REG_START_IND, RAM_ADR, "", "", \
    False, False, MENU_EVLOG_GROUP, REG_LOG_ADDR, REG_LOG_TIME, REG_LOG_DATE, &g_Ram.ramGroupH.LogEvCount, \
    0, 0, MENU_GROUPS_COUNT, \
    0, 0, 0, \
    0, 0, LOG_EV_CNT, \
    0, 0, 0, \
    0, 0, 0, 0, \
    FALSE, FALSE, 0, 0, FALSE, 0, \
    4, {GetAdr(ramGroupB.DevTime),GetAdr(ramGroupB.DevDate),GetAdr(ramGroupB.RodType),GetAdr(ramGroupB.ValveType),0},   \
    2, {GetAdr(ramGroupB.MoveCloseTorque),GetAdr(ramGroupB.MoveOpenTorque),0,0,0},    \
    1, {GetAdr(ramGroupD.TaskClose),0,0,0,0},  \
    1, {GetAdr(ramGroupD.TaskOpen),0,0,0,0},   \
    1, {GetAdr(ramGroupD.RevOpen),0,0,0,0},    \
    1, {GetAdr(ramGroupD.RevClose),0,0,0,0},   \
    1, {GetAdr(ramGroupB.BreakCloseTorque),0,0,0,0},   \
    1, {GetAdr(ramGroupB.StartCloseTorque),0,0,0,0},   \
    1, {GetAdr(ramGroupB.BreakOpenTorque),0,0,0,0},    \
    1, {GetAdr(ramGroupB.StartOpenTorque),0,0,0,0},    \
    1, {0,0,0,0,0}, \
    1, {GetAdr(ramGroupB.MuDuSetup),0,0,0,0},  \
    groups, Null, params, values, addr_params_altistar48, "������", \
    StartDispl, EnableEdit, WriteValue/*, GetExpressText, GetExpressState*/\
}

#if !NEW_RAZ
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
#else
// ��������� ����
typedef struct TCoreMenu {
	Byte   State;        // ����� ����
	Char   Key;          // ����������� �������
	Byte   EditType;     // ��� ������ ��������������
	Byte   Level;        // ������� ������� ����
	Byte   MaxLevel;     // ������� ����������� ���� (1 ��� 2)
	Bool   Indication;   // ���� ���������� ���������
	Bool   Update;       // ���� ���������� ������ �� ��������
	Bool   ShowReserved; // ���� ����������� ��������
	Uns   *MinMaxGain;   // ����������� ���������

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
	Uns    SleepTimer;   // ������ ������
	Uns    SleepTime;    // ����-��� ������

	Uns   *Data;         // ��������� �� ����� ������
	String HiString;     // ��������� �� ����� ������� ������� ����������
	String LoString;     // ��������� �� ����� ������ ������� ����������
	Uns    StartAddr;    // ����� ��������� ��� ������ � ������ ������
	Uns    StartOffset;  // �������� ������ ��� ������ � ������ ������
	Char   BufValue[6];  // ��������� ����� ��� �������������� � ������ 1
	Char   BufTmp[17];   // ��������� ����� ��� �������� ������� ���������/���������� ��������

	Bool   EvLogFlag;    // ���� ������ ������
	Bool   EvLogSelected;// ���� ������ ������
	Uns    EvLogGroup;   // ����� ������ �������
	Uns    EvLogAdress;  // ����� ��������� ������� ������ � ������
	Uns    EvLogTime;    // ����� ��������� ������� ������ �������
	Uns    EvLogDate;    // ����� ��������� ���� ������ �������
	Uns	   *EvLogCount;	 // ���������� ��������� ������� � ������

	struct MENU_CRD Group;     // ��������� ���������� ������
	struct MENU_CRD SubGroup;  // ��������� ���������� ���������
	struct MENU_CRD EvLog;     // ��������� ���������� ������ �������
	struct MENU_CRD Param;     // ��������� ���������� ���������
	struct MENU_DCR Dcr;       // ��������� �������� ���������

	Byte   setDefaultGroupNumber; // ����� ������ ��� ��������� ���������� ��-��������� 1-B 2-C

	struct MENU_EXPRESS Express;	// ��������� �������� ���������

	const struct MENU_GROUP  		*Groups;   // ��������� �� ��������� �����
	const struct MENU_GROUP  		*SubGroups;// ��������� �� ��������� ��������
	const struct MENU_PARAM  		*Params;   // ��������� �� ��������� ����������
	const struct MENU_STRING 		*Values;   // ��������� �� ��������� ��������� ��������
	const struct ADDR_PARAM_ATS48  	*AtsParams;   // ��������� �� ��������� ������� ���������� ���
	struct MENU_STRING        		Rsvd;     // ��������� ������

	void (*StartDispl)(String Text);
	Bool (*EnableEdit)(Uns PaswwPrt);
	Bool (*WriteValue)(Uns Memory, Uns ParAddr, Uns *Value);
//	void (*GetExpressText)(Uns State);
//	Byte (*GetExpressState)(Byte CurrentState, Uns State);

} TCoreMenu, *pCoreMenu;

#endif

void Core_MenuInit(TCoreMenu *);
void Core_MenuUpdate(TCoreMenu *);
void Core_MENU_Display(TCoreMenu *);


Bool UpdateCode(Uns, Uns, Uns, Uns);
Bool SleepMode(TCoreMenu *);
void StartState(TCoreMenu *);
void Core_SetDeff(TCoreMenu *);
void Core_MenuReadDcr(TCoreMenu *, struct MENU_DCR *, Uns);
#if NEW_RAZ
void StartDispl(String Str);
#else
void StartDispl(String, Byte);
void Core_MenuDisplay(TCoreMenu *);
#endif
Bool EnableEdit(Uns);
Bool WriteValue(Uns, Uns, Uns *);
void SetDefaultValues(TCoreMenu *, Byte *);
void ShowAddStrings(TCoreMenu *, Uns, Uns);
void SetModBusParams(void);
void SetModBusParams(void);
/*
void GetExpressText(Uns State);
Byte GetExpressState(Byte CurrentState, Uns State);*/
void AddControl(void);

extern void PFUNC_strRead(Char *, Char *);
extern void PFUNC_strWrite(Char *, Char *);
extern  Int PFUNC_wordRead(Int *);
extern void PFUNC_wordWrite(Int *, Int);

#if NEW_RAZ
extern TCoreMenu menu;
#endif

#endif



