/*======================================================================
Èìÿ ôàéëà:          g_Structs.h
Àâòîð:
Âåðñèÿ ôàéëà:
Äàòà èçìåíåíèÿ:
Îïèñàíèå:
Îïèñàíèå ñòðóêòóð
========================================================================*/

#ifndef G_STRUCTS_
#define G_STRUCTS_

#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------
//================== ÑÒÀÒÓÑÛ ============================
//--------------------------------------------------------
// ñòàòóñíûé ðåãèñòð
#define STATUS_STOP				BIT0
#define STATUS_FAULT			BIT1
#define STATUS_CLOSING			BIT2
#define STATUS_OPENING			BIT3
#define STATUS_TEST				BIT4
#define STATUS_CLOSED			BIT5
#define STATUS_OPENED			BIT6
#define STATUS_MUFTA			BIT7
#define STATUS_MU_DU			BIT8
#define STATUS_TEN				BIT9
#define STATUS_BLUETOOTH		BIT10
#define STATUS_ALARM_BLK_INDIC	BIT11
#define STATUS_ALARM_TS			BIT12
#define STATUS_DEFECT			BIT13
#define STATUS_DEFECT_BLK_INDIC	BIT14
#define STATUS_DEFECT_TS		BIT15

//-------------------Ñòàòóñ ðàáîòû----------------------------
// Ñòàòóñ ðàáîòû
#define STATUS_MOVE_MASK	0xC

typedef union _TStatusReg {
	Uns all;
	struct {
		
		Uns Stop:1;				// 0     Ñòîï
		Uns Fault:1;			// 1     Àâàðèÿ
		Uns Closing:1;    		// 2     Èäåò çàêðûòèå
		Uns Opening:1;			// 3     Èäåò îòêðûòèå
		Uns Test:1;				// 4     Èäåò òåñò
		Uns Closed:1;			// 5     Çàêðûòî
		Uns Opened:1;			// 6     Îòêðûòî
		Uns Mufta:1;			// 7     Ìóôòà
		Uns MuDu:1;				// 8     Ìåñòíîå óïðàâëåíèå
		Uns Ten:1;        		// 9     Âêëþ÷åí ÒÅÍ
		Uns EnableBluetooth:1;  // 10    Ðåçåðâíîå ïèòàíèå
		Uns MiddlePosition:1;	// 11    Ïðîìåæóòî÷íîå ïîëîæåíèå
		Uns CycleMode:1;			    // 12	 Ðåçåðâ
		Uns Defect:1;			// 13	 Íåèñïðàâíîñòü
		Uns Rsvd2:2;		    // 14 -15 	Ðåçåðâ
	} bit;
} TStatusReg;

// Ñòàòóñ êàëèáðîâêè
typedef enum {
	csNone  = 0,			// Ïðèâîä íå êàëèáðîâàí
	csClose = 1,			// Çàäàíî òîëüêî çàêðûòî
	csOpen  = 2,			// Çàäàíî òîëüêî îòêðûòî
	csCalib = 3				// Ïðèâîä êàëèáðîâàí
} TCalibState;

// Ñêîðîñòü ñâÿçè ModBus
typedef enum {
	br2400   = 0,			// 2400 Áîä
	br4800   = 1,			// 4800 Áîä
	br9600   = 2,			// 9600 Áîä
	br19200  = 3,			// 19200 Áîä
	br38400  = 4,			// 38400 Áîä
	br57600  = 5,			// 57600 Áîä
	br115200 = 6			// 115200 Áîä
} TBaudRate;

// Ðåæèì ñâÿçè
typedef enum {
	pmNone = 0,				// Íåò ïðîâåðêè áèòà ÷åòíîñòè, 2 ñòîïîâûõ áèòà
	pmOdd  = 1,				// Ïðîâåðêà íà íå÷åòíîñòü, 1 ñòîïîâûé áèò
	pmEven = 2				// Ïðîâåðêà íà ÷åòíîñòü, 1 ñòîïîâûé áèò
} TParityMode;

// Ñòðóêòóðà äëÿ ðàáîòû ñ ÏÄÓ
typedef struct _TPduData {
	Uns Read;            // ×òåíèå äàííûõ
	Uns Write;           // Çàïèñü äàííûõ
	Uns Rsvd;            // Ðåçåðâ
	Uns Type;            // Òèï ìåíþ
	Uns Key;             // Êîä êëàâèøè
} TPduData;

// Äèàãíîñòèêà ïðîöåññà
#define PROC_NOMOVE_MASK	0x0001
#define PROC_PHENG_MASK		0x0002
#define PROC_CALIB_MASK		0x001C
#define PROC_OVERW_MASK		0x0020
#define PROC_DRV_T_MASK 	0x0040
#define PROC_RESET_MASK		0x00A1
//#define PROCESS_EVLOG_MASK	0x00A1
#define PROCESS_EVLOG_MASK	0x03FF
typedef union _TProcessReg {
	Uns all;
	struct {
		Uns NoMove:1;		// 0     Íåò äâèæåíèÿ
		Uns PhOrd:1;		// 1     ×åðåäîâàíèå ôàç äâèãàòåëÿ
		Uns NoClose:1;		// 2     Íå çàäàíî ïîëîæåíèå çàêðûòî
		Uns NoOpen:1;	   	// 3     Íå çàäàíî ïîëîæåíèå îòêðûòî
		Uns NoCalib:1;		// 4     Êàëèáðîâêà íå âûïîëíåíà
		Uns Overway:1;		// 5     Óïëîòíåíèå íå äîñòèãíóòî
		Uns Mufta:1;		// 6	 Ìóôòà
		Uns MuDuDef:1;		// 7     Îøèáêà ïî äèñêðåòíûì âõîäàì Ìó/Äó
		Uns SoftStarter:1;	// 8	 Îøèáêà ÓÏÏ
		Uns FireContErr:1;	// 9	 íå çàìêíóò êîíèòàêòîð â ðåæèìå ïîæðêà
		Uns Rsvd:6;			// 10-15  Ðåçåðâ
	} bit;
} TProcessReg;

typedef union _TBCP9Reg {
	Uns all;
	struct {
				Uns Drv_T:1;		// 0     Íåò äâèæåíèÿ
				Uns PosSens:1;		// 1     Ñáîé äàò÷èêà ïîëîæåíèÿ
				Uns Memory1:1;		// 2     Ñáîé ïàìÿòè 1
				Uns Calibration:1;	// 3     Ìîìåíò êàëèáðîâêè
				Uns RTC:1;			// 4     Ñáîé ÷àñîâ ðåàëüíîãî âðåìåíè
				Uns TSens:1;		// 5     Ñáîé äàò÷èêà òåìïåðàòóðû
				Uns Th_BCP:1;		// 6     Ïåðåãðåâ áëîêà
				Uns Tl_BCP:1;		// 7     Ïåðåîõëîæäåíèå áëîêà
				Uns Rsvd7:1;		// 8     Ðåçåðâ
				Uns Rsvd8:1;		// 9     Ðåçåðâ
				Uns Dac:1;			// 10     Ñáîé ÖÀÏ
				Uns Memory2:1;		// 11    Memory2 Error
				Uns Rsvd:4;			// 12-15  Ðåçåðâ
	} bit;
} TBCP9Reg;

// Äèàãíîñòèêà ñåòè
#define NET_UV_MASK			0x0007
#define NET_OV_MASK			0x0038
#define NET_PHO_MASK		0x0040
#define NET_VSK_MASK		0x0080
#define NET_BV_MASK			0x0700
#define NET_OV_MAX_MASK		0x3800
//#define NET_EVLOG_MASK		0x7FFF //0x3F7F
#define NET_EVLOG_MASK		0x3F3F

typedef union _TNetReg {
	Uns all;
	struct {
		Uns UvR:1;			// 0     Ïîíèæåíèå íàïðÿæåíèÿ â ôàçå R
		Uns UvS:1;			// 1     Ïîíèæåíèå íàïðÿæåíèÿ â ôàçå S
		Uns UvT:1;			// 2     Ïîíèæåíèå íàïðÿæåíèÿ â ôàçå T
		Uns OvR:1;			// 3     Ïðåâûøåíèå íàïðÿæåíèÿ â ôàçå R
		Uns OvS:1;			// 4     Ïðåâûøåíèå íàïðÿæåíèÿ â ôàçå S
		Uns OvT:1;			// 5     Ïðåâûøåíèå íàïðÿæåíèÿ â ôàçå T
		Uns Rsvd2:1;		// 6
		Uns VSk:1;			// 7     Àñèììåòðèÿ íàïðÿæåíèÿ âõîäíîé ñåòè
		Uns BvR:1;			// 8     Îáðûâ ôàçû R âõîäíîé ñåòè
		Uns BvS:1;			// 9     Îáðûâ ôàçû S âõîäíîé ñåòè
		Uns BvT:1;			// 10    Îáðûâ ôàçû T âõîäíîé ñåòè
		Uns OvR_max:1;			// 11    Ïðåâûøåíèå íàïðÿæåíèÿ â ôàçå R íà 47%
		Uns OvS_max:1;			// 12    Ïðåâûøåíèå íàïðÿæåíèÿ â ôàçå S íà 47%
		Uns OvT_max:1;			// 13    Ïðåâûøåíèå íàïðÿæåíèÿ â ôàçå T íà 47%
		Uns Rsvd:2;			// 14-15 Ðåçåðâ
	} bit;
} TNetReg;

// Äèàãíîñòèêà íàãðóçêè
#define LOAD_PHL_MASK		0x0007
#define LOAD_I2T_MASK		0x0008
#define LOAD_SHC_MASK		0x00E0
#define LOAD_UNL_MASK		0x0400
#define LOAD_ISK_MASK		0x0800
#define LOAD_RESET_MASK		0x0807
#define LOAD_EVLOG_MASK		0x00EF
typedef union _TLoadReg {
	Uns all;
	struct {
		Uns PhlU:1;			// 0     Îáðûâ ôàçû U
		Uns PhlV:1;			// 1     Îáðûâ ôàçû V
		Uns PhlW:1;			// 2     Îáðûâ ôàçû W
		Uns I2t:1;			// 3     Âðåìÿ-òîêîâàÿ ïåðåãðóçêà
		Uns Rsvd1:1;			// 4     Ðåçåðâ
		Uns ShCU:1;			// 5	 Êîðîòêîå çàìûêàíèå â ôàçå U
		Uns ShCV:1;			// 6	 Êîðîòêîå çàìûêàíèå â ôàçå V
		Uns ShCW:1;			// 7	 Êîðîòêîå çàìûêàíèå â ôàçå W
		Uns Rsvd2:3;			// 8-10	 Ðåçåðâ
		Uns ISkew:1;			// 11    Àññèìåòðèÿ òîêà
		Uns Rsvd:4;			// 13-15 Ðåçåðâ
	} bit;
} TLoadReg;

// Äèàãíîñòèêà óñòðîéñòâà
#define DEV_ERR_MASK		0x009F
#define DEV_TMP_MASK		0x0060
#define DEV_RSC_MASK		0x0800
//#define DEV_EVLOG_MASK	0x0400
#define DEV_EVLOG_MASK		0x5FFF
typedef union _TDeviceReg {
	Uns all;
  struct {
		Uns PosSens:1;		// 0     Ñáîé äàò÷èêà ïîëîæåíèÿ
		Uns Memory1:1;		// 1     Ñáîé ïàìÿòè 1
		Uns Memory2:1;		// 2     Ñáîé ïàìÿòè 2
		Uns Rtc:1;			// 3     Ñáîé ÷àñîâ ðåàëüíîãî âðåìåíè
		Uns TSens:1;		// 4     Ñáîé äàò÷èêà òåìïåðàòóðû
		Uns Th_BCP:1;		// 5     Ïåðåãðåâ áëîêà ÁÊÏ
		Uns Tl_BCP:1;		// 6     Ïåðåîõëîæäåíèå áëîêà ÁÊÏ
		Uns Th_BCD:1;		// 7     Ïåðåãðåâ áëîêà ÁÊÄ
		Uns Tl_BCD:1;		// 8     Ïåðåîõëîæäåíèå áëîêà ÁÊÄ
		Uns Dac:1;			// 9     Ñáîé ÖÀÏ
		Uns LowPower:1;		// 10     Âûêëþ÷åíèå ÁÊÄ
		Uns NoBCP_Connect:1;// 11   íåò ñâÿçè ñ ÁÊÏ
		Uns TimeNotSet:1;	// 12   Âðåìÿ íå çàäàíî
		Uns BlueNoLaunch:1;	// 13	íå çàïóñòèëñÿ ÁÊÝÏ
		Uns BCP_ErrorType:1;// 14	íå âåðíûé òèï ÁÊÏ
		Uns Rsvd:1;       	// 15 Ðåçåðâ
  } bit;
} TDeviceReg;


// Ñòðóêóòóðà îáúåäèíåííûõ àâàðèé
typedef struct {
	TProcessReg Proc;
	TNetReg     Net;
	TLoadReg    Load;
	TDeviceReg  Dev;
} TFltUnion;

// Ãðóïïà àâàðèé
typedef enum {
	pgProcess = 0,			// Ãðóïïà àâàðèé ïðîöåññà
	pgNet     = 1,			// Ãðóïïà àâàðèé ñåòè
	pgLoad    = 2,			// Ãðóïïà àâàðèé íàãðóçêè
	pgDevice  = 3,			// Ãðóïïà àâàðèé óñòðîéñòâà
	pgCount   = 4			// Êîëè÷åñòâî ãðóïï
} TPrtGroup;

// Êîìàíäû ÁÓÐ
typedef enum
{
	bcmNone = 0,				// Íåò êîìàíäû
	bcmStop,					// Ñòîï
	bcmClose,					// Çàêðûòü
	bcmOpen,					// Îòêðûòü
	bcmSetDefaultsUser,			// Ïîëüçîâàòåëüñêèå ïàðàìåòðû ïî óìîë÷àíèþ
	bcmCalibReset,				// Ñáðîñ êàëèáðîâêè
	bcmPrtReset,				// Ñáðîñ çàùèò
	bcmLogClear,				// Î÷èñòêà æóðíàëà
	bcmCycleReset,				// Ñáðîñ öèêëîâ
	bcmSetDefaultsFact,			// Óñòàíîâêà çàâîäñêèõ ïàðàìåòðîâ ïî óìîë÷àíèþ
	bcmDefStop,					// Ñïñåâäî êîìàíäà ñòîï ïðè àâàðèÿõ, íóæíà äëÿ îïðåäåëåíèÿ îáùåé ïðîäîëæèòåëüíîñòè ðàáîòû
	bcmDiscrOutTest,			// Òåñò äèñêðåòíûõ âûõîäîâ
	bcmDiscrInTest,				// Òåñò äèñêðåòíûõ âõîäîâ
	bcmBlueOn,					// âêëþ÷åíèå Bluetooth
	bcmBlueOff,					// âûêëþ÷åíèå Bluetooth
	bcmFixPos					// êîìàíäà ôèêñàöèè ïîëîæåíèÿ
} TBurCmd;


// Ìåñòî óñòàíîâêè ÁÓÐ (ïîæàðêà - Ëèíåéíàÿ ÷àñòü/Àâòîìàòèêà è òåëåìåõàíèêà)
typedef enum {
	spLinAuto   = 0,		// 0 Ëèíåéíàÿ ÷àñòü/Àâòîìàòèêà è òåëåìåõàíèêà
	spFire 		= 1 		// 1 Ïîæàðêà
}TSettingPlace;

// Ìåòîä îñòàíîâà
typedef enum {
	smSlowDown 		= 0,	// âûáåã
	smReverse   	= 1,	// ðåâåðñ
	smDynBreak  	= 2		// äèíàìèêà
} TStopMethod;

// Óïðàâëåíèå êîíòàêòîðàìè ä
typedef enum {
	cgStop  = 0,
	cgOpen  = 1,
	cgClose = 2
} TContactorGroup;

// òèï ïðèâîäà
typedef enum {
	Empty 	  	    = 0,
	dt100_A25 	    = 1,
	dt100_A50 	    = 2,
	dt400_B20 	    = 3,
	dt400_B50 	    = 4,
	dt800_V40 	    = 5,
	dt1000_V20  	= 6,
	dt4000_G9   	= 7,
	dt4000_G18  	= 8,
	dt10000_D6  	= 9,
	dt10000_D12 	= 10,
	dt10000_D10 	= 11,
	dt15000_D10 	= 12,
	dt20000_F40 	= 13,
	dt35000_F48 	= 14,
	dt50000_F48 	= 15,
	dt100_A25_S 	= 16,// Ñàðàïóëü
	dt100_A50_S 	= 17,
	dt400_B20_S 	= 18,
	dt400_B50_S 	= 19,
	dt800_V40_S 	= 20,
	dt1000_V20_S  	= 21,
	dt4000_G9_S   	= 22,
	dt4000_G18_S  	= 23,
	dt10000_D6_S  	= 24,
	dt10000_D12_S 	= 25,
	dt15000_D10_T   = 26,
	dt10000_D12_U2 	= 27,
	dt4000_G18_U2 	= 28,
	dt2000_EPZR 	= 29,
	dt4000_EPZR 	= 30,
	dtNUM_OF = 31

}TDriveType;

// Óïðàâëåíèå ðàáîòîé çàùèò
typedef enum {
  pmOff       = 0,		// Çàùèòà âûêëþ÷åíà
  pmOn  = 1,		// Ñèãíàëèçàöèÿ è îñòàíîâ
  pmCount     = 2		// Êîëè÷åñòâî ðåæèìîâ
} TPrtMode;

// Òèï øòîêà
typedef enum {
  rdtNormal  = 0,			// Íîðìàëüíûé
  rdtInverse = 1			// Èíâåðñíûé
} TRodType;

// Êîìàíäû çàäàíèÿ/ñáðîñà
typedef enum {
  trNone     = 0,			// Íåò êîìàíäû
  trTask     = 1,			// Çàäàòü
  trReset    = 2 			// Ñáðîñèòü
} TTaskReset;

// Íîðìàëüíîå ñîñòîÿíèå âõîäà
typedef enum {
  nsOpened = 0,				// Íîðìàëüíî-îòêðûòûé êîíòàêò
  nsClosed = 1				// Íîðìàëüíî-çàêðûòûé êîíòàêò
} TNormState;

// Ðåæèì èíäèêàöèè òîêà
typedef enum {
  imRms     = 0,		  	// Äåéñòâóþùèå çíà÷åíèÿ
  imPercent = 1			  	// Â ïðîöåíòàõ îò íîìèíàëüíîãî òîêà äâèãàòåëÿ
} TIndicMode;

// Òèï çàäâèæêè êëèí/øèáåð
typedef enum {
  vtKlin     = 0,		  	// Êëèíîâàÿ çàäâèæêà
  vtShiber   = 1			// Øèáåðíàÿ çàäâèæêà
} TValveType;

// ìåñòî óñòàíîâêè ïðèâîäà
typedef enum {
  ptNormal     	= 0,			// îáû÷íàÿ çàäâèæêà
  ptAgregat     = 1,			// àãðåãàòíàÿ çàäâèæêà
  ptFire    	= 2 			// ïîæàðíàÿ çàäâèæêà
} TPlaceType;

// Òèï ðàáîòû òåíà
typedef enum {
	tmControl 		= 0, 	// Óïðàâëåíèå ïî óñòàâêàì
	tmTimerControl	= 1, 	// Óïðàâëåíèå ïî óñòàâêàì ñ çàäåðæêîé âðåìåíè
	tmAlwaysOn 		= 2,	// Âñåãäà âêëþ÷åí
	tmHandControl 	= 3		// Ðó÷íîå óïðàâëåíèå
}TTenMode;

// Ñòàòóñ øàãîâîãî ðåæèìà
typedef enum {
	smsStop 	= 0, 	// Ïðèâîä îñòàíîâëåí
	smsMoving	= 1, 	// Ïðèâîä â äâèæåíèè
	smsInPause 	= 2		// Ïðèâîä â ïàóçå
}TStepModeStatus;

// Ñòðóêòóðû äëÿ ÒÝÊîâñêîãî ìîäáàñà

// Ðåãèñòðû ÒÝÊà
// Òåõíîëîãè÷åñêèé ðåãèñòð
typedef union _TTEK_TechReg
{
	Uns all;
	struct
	{
		Uns Opened:1;		// 0 		Ïîëîæåíèå "Îòêðûòî"
		Uns Closed:1;		// 1 		Ïîëîæåíèå "Çàêðûòî"
		Uns Mufta1:1;		// 2 		Â íàøåì ñëó÷àå ìóôòà íå ðàçäåëÿåòñÿ íà òðîãàíèå èëè äâèæåíèå
		Uns Mufta2:1;		// 3 		Äóáëèðóåì ìóôòó
		Uns Rsvd2:3;		// 4-6 		Ðåçåðâ
		Uns MuDu:1;		// 7 		ÌÓ/ÄÓ
		Uns Opening:1;		// 8 		"Îòêðûâàåòñÿ"
		Uns Closing:1;		// 9 		"Çàêðûâàåòñÿ"
		Uns Stop:1;		// 10 		Ñòîï
		Uns Rsvd3:2;		// 11-12 	Ðåçåðâ
		Uns Ten:1;		// 13; 		Âêëþ÷åí òåí
		Uns Rsvd4:2;		// 14-15; 	Ðåçåðâ
	} bit;
} TTEK_TechReg;

// Ðåãèñòð äåôåêòîâ
typedef union _TTEK_DefReg
{
	Uns all;
	struct
	{
		Uns I2t:1;		// 0 	Âðåìÿ-òîêîâàÿ çàùèòà
		Uns ShC:1;		// 1 	Êîðîòêîå çàìûêàíèå
		Uns Drv_T:1;	// 2 	Ïåðåãðåâ äâèãàòåëÿ
		Uns Uv:1;		// 3 	Ïîíèæåíåîå íàïðÿæåíèå ôàç ñåòè
		Uns Phl:1;		// 4 	Îáðûâ ôàçû ýëåêòðîäâèãàòåëÿ
		Uns NoMove:1;	// 5 	Íåò äâèæåíèÿ
		Uns Ov:1;		// 6 	Ïåðåíàïðÿæåíèå íà âõîäå
		Uns Bv:1;		// 7 	Êðèòè÷åñêîå ñíèæåíèå íàïðÿæåíèÿ íà âõîäå
		Uns Rsvd:1;		// 8 	Ðåçåðâ
		Uns Th:1;		// 9  	Ïåðåãðåâ áëîêà
		Uns Tl:1;		// 10 	Ïåðåîõëîæäåíèå áëîêà
		Uns Rsvd1:1;	// 11 	Ðåçåðâ
		Uns PhOrdU:1;	// 12   Íåâåðíîå ÷åðåäîâàíèå ôàç ñåòè
		Uns PhOrdDrv:1;	// 13   ×åðåäîâàíèå ôàç äâèãàòåëÿ
		Uns DevDef:1;	// 14 	Äåôåêò óñòðîéñòâà (êðîìå ïåðåãðåâà è ïåðåîõëàæäåíèÿ)
		Uns Rsvd2:1;	// 15 	Ðåçåðâ
	} bit;
} TTEK_DefReg;

// Ðåãèñòð êîìàíä
typedef union _TTEK_ComReg
{
	Uns all;
	struct
	{
		Uns Stop:1;				// 0 	 Ñòîï
		Uns Open:1;				// 1	 Îòêðûòü
		Uns Close:1;			// 2 	 Çàêðûòü
		Uns Rsvd:2;				// 3-4 	 Ðåçåðâ
		Uns PrtReset:1;			// 5 	 Ñáðîñ çàùèò
		Uns Rsvd1:2;			// 6-7   Ðåçåðâ
		Uns EnDiscrInTest:1; 	// 8 	 Âêëþ÷åíèå òåñèðîâàíèÿ äèñêðåòíûõ âõîäîâ
		Uns DisDiscrInTest:1; 	// 9 	 Âêëþ÷åíèå òåñèðîâàíèÿ äèñêðåòíûõ âõîäîâ
		Uns EnDiscrOutTest:1;	// 10 	 Âêëþ÷åíèå òåñèðîâàíèÿ äèñêðåòíûõ âûõîäîâ
		Uns DisDiscrOutTest:1; 	// 11 	 Âêëþ÷åíèå òåñèðîâàíèÿ äèñêðåòíûõ âûõîäîâ
		Uns Rsvd2:4;			// 12-15 Ðåçåðâ
	} bit;
} TTEK_ComReg;

// Ðåãèñòð ÒÑ/ÒÓ
typedef union _TTEK_Discrete
{
	Uns all;
	struct
	{
		Uns InOpen:1;			// 0 	 Âõîä "Îòêðûòü"
		Uns InClose:1;			// 1 	 Âõîä "Çàêðûòü"
		Uns InStop:1;			// 2 	 Âõîä "Ñòîï"
		Uns InMu:1;				// 3 	 Âõîä "ÌÓ"
		Uns InDu:1;				// 4 	 Âõîä "ÄÓ"
		Uns OutOpened:1;		// 5 	 Âûõîä "Îòêðûòî"
		Uns OutClosed:1;		// 6 	 Âûõîä "Çàêðûòî"
		Uns OutMufta:1;			// 7 	 Âûõîä "Ìóôòà"
		Uns OutFault:1;			// 8  	 Âûõîä "Àâàðèÿ"
		Uns OutOpening:1;		// 9 	 Âûõîä "Îòêðûâàåòñÿ"
		Uns OutClosing:1;		// 10 	 Âûõîä "Çàêðûâàåòñÿ"
		Uns OutMuDu:1;			// 11    Âûõîä "ÌÓ/ÄÓ"
		Uns OutNeispr:1;		// 12	Âûõîä "Íåèñïðàâíîñòü"
		Uns Rsvd:1;				// 13 	Ðåçåðâ
		Uns IsDiscrOutActive:1;	// 14 	Ñîñòîÿíèå ðåæèìà òåñòèðîâàíèÿ âûõîäîâ
		Uns IsDiscrInActive:1;	// 15 	Ñîñòîÿíèå ðåæèìà òåñòèðîâàíèÿ âõîäîâ
	} bit;
} TTEK_Discrete;


#ifdef __cplusplus
}
#endif // extern "C"

#endif

