/*======================================================================
��� �����:          core_VlvDrvCtrl.h
�����:				����� �.�.
������ �����:		1.00
���� ���������:		16.04.2014
��������:
������ ���������� ���������
======================================================================*/

#ifndef VLV_DRV_CNTRL_
#define VLV_DRV_CNTRL_
//----------- ����������� ������������ ������ ------------------------------
#include "config.h"
#include "g_Structs.h"
#include "core_Menu.h"

//--------------------- ���������-------------------------------------------
// ���� ������ ����������
#define CMD_STOP				0x0001	// ����
#define CMD_CLOSE				0x0002	// �������
#define CMD_OPEN				0x0004	// �������
#define CMD_MOVE				0x0008	// �����������
#define CMD_DEFSTOP				0x0800	// ���� ��� ������(�� �������� ��������, ��������� ��� �������)  : ������� PIA 12.09.2012
#define CDM_DISCROUT_TEST		0x1000	// ���� ���������� �������
#define CMD_DISCRIN_TEST		0x2000	// ���� ���������� ������
#define CMD_ON_BLT				0x4000	// ��������� Bluetooth
#define CMD_OFF_BLT				0x8000	// ���������� Bluetooth

// �������� ������� ����������
#define CMD_SRC_BLOCK		0x0400	// ��������� ���������
#define CMD_SRC_PDU			0x0800	// ����� �������������� ����������
#define CMD_SRC_MPU			0x1000	// ������� ���� ����������
#define CMD_SRC_DIGITAL		0x2000	// ���������� ���������
#define CMD_SRC_SERIAL		0x4000	// ���������������� ���������
#define CMD_SRC_ANALOG		0x8000	// ��������� ���������
#define CMD_SRC_BLUETOOTH	0x200   // �������� ������� Bluetooth ���������
// ��������� ��� ������ � ����������
#define POS_UNDEF	0x7FFFFFFF
#define POS_ERR		10

// ��������� ������ ��
#define TU_OPEN			0x1
#define TU_CLOSE		0x2
#define TU_STOP_OPEN	0x4
#define TU_STOP_CLOSE	0x8

#define VLV_CLB_FLAG    0x03
//--------------------- ������� --------------------------------------------
//-------------------- ��������� -------------------------------------------

// ������� ����������
typedef enum {
  vcwNone=0,		// ��� �������
  vcwStop,			// ����
  vcwClose,			// �������
  vcwOpen,			// �������
  vcwTestClose,		// �������� �������
  vcwTestOpen,		// �������� �������
  vcwDemo,			// ���� �����
  vcwTestEng		// ���� ���������
} TValveCmd;

// ��������� ������ ��/��
typedef enum {
  mdOff=0,			// ��������
  mdSelect,			// ����� ������ ��/��
  mdMuOnly,			// ������ ����� ��
  mdDuOnly			// ������ ����� ��
} TMuDuSetup;

// ��� ����������
typedef enum {
  vtNone=1,			// ��� ����������
  vtClose,			// ���������� � �������
  vtOpen,			// ���������� � �������
  vtBoth			// ���������� � ������� � �������
} TBreakMode;

// �������� ������ ��� ������ ��
typedef enum {
	mdsAll=0,		// ��� ����������
	mdsDigital,		// ������ ����������
	mdsSerial		// ������ ����������������
} TDuSource;

// ��� �������
typedef enum {
  rvtStop=0,		// ������� � ����
  rvtAuto,			// �������������� ������
  rvtNone			// ������������� ������ ��� ������ ������ �� ������
} TReverseType;

// ��������� ��� ������ � ������� �����������
typedef struct {
	Bool				 Enable;				// ������� �������� ����������
	Uns				 	*BtnKey;				// ������� � �����
	Bool				 CancelFlag;			// ���� ������ �������
} TMpuControl;

// ��������� ��� ������ � ���������������
typedef struct {
	Bool				 Enable;			// ������� ��������������
	Bool				 LocalFlag;			// ���� TRUE, ���� ���������� �������
	Uns				 	*State;				// ��������� ������ ��������������
	Bool				 Ready;				// ���������� ��������� ������ �� �� ��������
	Uns					*LockSeal;			// ���������� ���������
} TTeleControl;

// ��������� ��� ������ � ���������
typedef struct {
	Bool				PosRegEnable;		// ������� ������ ����������������
	Bool            	BreakFlag;			// ���� ������ � ����������� SVS.1
	LgInt				Position;			// ������� ���������
	Uns				 	BreakDelta;			// ������������ �������� ��� ������ � �����������
	TBreakMode			*BreakMode;			// ����� ������ � �����������
	TCalibState			*CalibStates;		// ������ ����������
	LgUns				*CalibFullStep;		// ������ � ����������� ����� �������� ����������� ��� ����������� ����

} TValveControl;

// ��������� ��� ������ � �������� �������
typedef struct {
	Uns				 Value;					// �������� ������� ���������� ��� ������� �������
	Uns				 Source;				// �������� ������� ���������� ��� ������� �������
} TEvLogControl;

// ��������� ���������� ��������
typedef union {
	Uns all;
	struct {
		Uns Stop:1;			// 0		����
		Uns Fault:1;		// 1		������
		Uns Closing:1;		// 2		���� ��������
		Uns Opening:1;		// 3		���� ��������
		Uns Rsvd1:1;		// 4		������
		Uns Closed:1;		// 5		�������
		Uns Opened:1;		// 6		�������
		Uns Rsvd2:1;		// 7		������
		Uns MuDu:1;			// 8		������� ����������
		Uns Rsvd:7;			// 9-15 	������
	} bit;
} TVlvStatusReg;

// ��������� ���������� ��������
typedef struct {
	TVlvStatusReg	*Status;			// ������ ������
	TValveCmd		*ControlWord;		// ������� ����������
	TMuDuSetup		*MuDuSetup;			// ��������� ������ ��/��
	TDuSource		*DuSource;			// �������� ������ ��� ������ ��
	TReverseType	*ReverseType;		// ��� �������
	TMpuControl		 Mpu;				// ������� ����������
	TTeleControl	 Tu;				// ��������������
	TValveControl	 Valve;				// ���������� ���������
	TEvLogControl	 EvLog;				// ������ �������
	TValveCmd		 Command;			// ���������� �������
	Uns				 MuDuInput;			// ��������� ����� ��/��
	Uns				 ActiveControls;	// ��������� ���������� ����������� ����������
	Uns				 StartDelay;		// ����� ��� ��������� ������� ����������
	Uns 			 IgnorComFlag;		// ���� ������������� ������� ���������� ���� ��� ��������� � ������� ���������
	void (*StopControl)(void);			// ������� �������� ����������
	void (*StartControl)(TValveCmd ControlWord); // ������� ������ ����������
} TCoreVlvDrvCtrl;

//------------------- ���������� ���������� --------------------------------
//------------------- ��������� ������� ------------------------------------

void Core_ValveDriveInit(TCoreVlvDrvCtrl *);
void Core_ValveDriveStop(TCoreVlvDrvCtrl *);
void Core_ValveDriveUpdate(TCoreVlvDrvCtrl *);



#endif
