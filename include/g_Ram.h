/*======================================================================
��� �����:          g_Ram.h
�����:
������ �����:
���� ���������:
��������:
�������� ������� ���������
========================================================================*/

#ifndef G_RAM_
#define G_RAM_

#include "config.h"				// ���������
#include "g_Structs.h"			// ���������
#include "peref_Clock.h"		// ���������� ��� �������������� �����/�����
#include "core.h"
//#include "core_Protections.h" 	//��������� �����
//#include "comm_LocalControl.h"

// ������� ����������
typedef enum {
  genNone=0,		// ��� �������
  genStop,		
  genOpen,	
  genClose,		
  genRzvd1,
  genRzvd2,
  genResetProtect,
  genOnDU,	
  genOnMU,		
  genInTest_ON,
  genInTest_OFF,
  genOutTest_ON,
  genOutTest_OFF	
} TCommand;

//-------------------------------------------------------------------------
typedef union _TTek_TechReg
{
	Uns all;
	struct
	{
		Uns Opened:1;		// 0
		Uns Closed:1;		// 1
		Uns Mufta1:1;		// 2
		Uns Mufta2:1;		// 3
		Uns Rsvd2:3;		// 4-6
		Uns MuDu:1;			// 7
		Uns Opening:1;		// 8
		Uns Closing:1;		// 9
		Uns Stop:1;			// 10
		Uns Rsvd3:2;		// 11-12
		Uns Ten:1;			// 13
		Uns Rsvd4:2;		// 14-15
	} bit;
} TTek_TechReg;

typedef union _TTek_DefReg
{
	Uns all;
	struct
	{
		Uns I2t:1;		// 0
		Uns ShC:1;		// 1
		Uns Drv_T:1;	// 2
		Uns Uv:1;		// 3
		Uns Phl:1;		// 4
		Uns NoMove:1;	// 5
		Uns Ov:1;		// 6
		Uns Bv:1;		// 7
		Uns Rsvd:1;		// 8
		Uns Th:1;		// 9
		Uns Tl:1;		// 10
		Uns Rsvd1:1;	// 11
		Uns PhOrdU:1;	// 12
		Uns PhOrdDrv:1;	// 13
		Uns DevDef:1;	// 14
		Uns Rsvd2:1;	// 15
	} bit;
} TTek_DefReg;

typedef union _TTek_ComReg
{
	Uns all;
	struct
	{
		Uns Stop:1;				// 0
		Uns Open:1;				// 1
		Uns Close:1;			// 2
		Uns Rsvd:2;				// 3-4
		Uns PrtReset:1;			// 5
		Uns Rsvd1:2;			// 6-7
		Uns EnDiscrInTest:1; 	// 8
		Uns DisDiscrInTest:1; 	// 9
		Uns EnDiscrOutTest:1;	// 10
		Uns DisDiscrOutTest:1; 	// 11
		Uns Rsvd2:4;			// 12-15
	} bit;
} TTek_ComReg;

typedef union _TTek_Discrete
{
	Uns all;
	struct
	{
		Uns InOpen:1;			// 0
		Uns InClose:1;			// 1
		Uns InStop:1;			// 2
		Uns InMu:1;				// 3
		Uns InDu:1;				// 4
		Uns OutOpened:1;		// 5
		Uns OutClosed:1;		// 6
		Uns OutMufta:1;			// 7
		Uns OutFault:1;			// 8
		Uns OutOpening:1;		// 9
		Uns OutClosing:1;		// 10
		Uns OutMuDu:1;			// 11
		Uns OutNeispr:1;		// 12
		Uns Rsvd:1;				// 13
		Uns IsDiscrOutActive:1;	// 14
		Uns IsDiscrInActive:1;	// 15
	} bit;
} TTek_Discrete;
//----------------------------------------------------*--------------------------

// ��������� ���������
// ������ A (����� = 0, ���������� = 40) - ����������� - ��������
typedef struct _TRamGroupA
{
	TStatusReg      Status;           	// 0.������ ������
	TFltUnion	    Faults;				// 1-4.������
	TInputReg       Inputs;           	// 5.��������� ���������� ������
	TOutputReg      Outputs;          	// 6.��������� ���������� �������
	Uns             Position;        	// 7.���������
	Uns             Torque;           	// 8.������
	Int             Speed;				// 9.��������
	Uns             Ur;                 // 10.���������� ���� R
  	Uns             Us;                 // 11.���������� ���� S
  	Uns             Ut;                 // 12.���������� ���� T
	Uns             Iu;                 // 13.��� ���� U
	Uns             Iv;                 // 14.��� ���� V
	Uns             Iw;                 // 15.��� ���� W
	Int             AngleUI;            // 16.���� ��������
	TCalibState     CalibState;         // 17.������ ����������
	Int             PositionPr;         // 18.��������� %
	Uns             FullWay;            // 19.������ ���
	Int             CurWay;             // 20.������� ���
	Uns             CycleCnt;           // 21.������� ������
	Int             Temper;             // 22.����������� �����
	Uns             MkuPoVersion;       // 23.������ ��
	Uns Rsvd[16];
} TRamGroupA;

// ������ B (����� = 40, ���������� = 50) - ��������� ������������
typedef struct _TRamGroupB
{
	Uns				MoveCloseTorque;	// 0.������ ��������
	Uns				MoveOpenTorque;		// 1.������ ��������
	Uns				BreakCloseTorque;	// 2.������ ���������� �� ��������
	Uns				StartCloseTorque; 	// 3.������ �������� �� ��������
	Uns				BreakOpenTorque;  	// 4.������ ���������� �� ��������
	Uns				StartOpenTorque;  	// 5.������ �������� �� ��������
	TValveType		ValveType;			// 6.��� �������� (����/�����)
	TBreakMode      BreakMode;		   	// 7.��� ����������
	TRodType        RodType;            // 8.��� �����
	Uns				CloseZone;		 	// 9.���� �������
	Uns				OpenZone;		 	// 10.���� �������
	Uns             PositionAcc;        // 11.���� ��������
	Uns             MainCode;        	// 12.��� �������
	TTimeVar 		DevTime;			// 13.�����
	TDateVar 		DevDate;			// 14.����
	Int		 		TimeCorrection;		// 15.������������� �������
	TIndicMode      IIndicMode;         // 16.����� ��������� ����
	TMuDuSetup      MuDuSetup;          // 17.��������� ������ ��/��
	TDuSource		DuSource;			// 18.�������� ������ ��
	Uns             TuLockSeal;         // 19.���������� ��������
	Uns             TuTime;             // 20.����� �������
	#if BUR_M
	Uns 			Rsvd1[2];
	#else
	TInputType 		InputType;			// 21.��� �������� ������� 24/220
	TInputMask	    InputMask;			// 22.����� ���������� ������
	#endif
	TOutputMask 	OutputMask;			// 23.����� ���������� �������
	TBaudRate       RsBaudRate;         // 24.�������� �����
	Uns             RsStation;          // 25.����� �������
	TParityMode		RsMode;				// 26.����� �����
	TSettingPlace	SettingPlace;		// 27.����� ��������� ���
	Uns 			Rsvd2[4];			// 28-31.
	Uns				NoMoveTime;		   	// 32.����� ���������� ��������
	Uns				OverwayZone;		// 33.�����
	Uns 			Rsvd3;				// 34.
 	Uns             SleepTime;          // 35.�������� �����
	Uns 			Rsvd[14];
} TRamGroupB;

// ������ C (����� = 90, ���������� = 120) - ��������� ���������
typedef struct _TRamGroupC
{
	Uns             FactCode;           // 0.��� �������
	TDriveType      DriveType;          // 1.��� �������
    Uns        		ProductYear;        // 2.��� ������������ �����
  	Uns             FactoryNumber;      // 3.��������� ����� �����
  	Uns             MaxTorque;          // 4.������������ ������ �������
	Uns             Inom;               // 5.����������� ���
  	Uns             GearRatio;          // 6.������������ ����� ���������
	Int             BlockToGearRatio;   // 7.����������� �������� ����-�������� ����������� ��������
	Uns             PosPrecision;       // 8.�������� ���������
	Uns             PhOrdZone;      	// 9.���������� ����������� ��� ���������
	Uns             MuffZone;           // 10.���������� ������ �����
	Uns			    PosSensPow;			// 11.��� ������� ���������
	TPrtMode        CalibIndic;         // 12.��������� ����������
	Uns			    DisplResTout;		// 13.����� ������ ����������
	// ���������
	Uns             OpenAngle;          // 14.���� ���������� ����
	Int             AngleUI;            // 15.���� ��������
	TLedsReg        LedsReg;            // 16.��������� ����������� �����
	THallBlock      HallBlock;          // 17.��������� �������� ����� �����
	Uns             SetDefaults;        // 18.������� ���������� �� ���������
#if BUR_M
	TPrtMode       	RST_Err;           	// 19.�������� ����������� ��� ���� ��� ��� �
#else
	Uns       	    Rsvd1;           	// 19.������
#endif
	TPrtMode        DriveTemper;        // 20.������ �� ��������� ���������. (�������� ������ ��������� ����� � H)
	Uns             OvLevel_max;        // 21.������� ���������� ���������� ��� 47% ���������� (320�)
	Uns             OvTime_max;     	// 22.����� ���������� ���������� ��� 47% ���������� (1�)
	Uns             RevErrValue;       	// 23.���������� ����� ������� ���������
	Uns			    RevErrLevel;		// 24.������� ���� ������� ���������
	//-��������� �����--
	Uns             VoltAcc;            // 25.������������� �������
	Uns             BrakeAngle;         // 26.���� ����������
	Uns             BrakeTime;          // 27.����� ����������
	Uns				KickCount;			// 28.����������� ������
	Uns             OvLevel;    		// 29.������� ���������� ����������
	Uns             OvTime;     		// 30.����� ���������� ����������
	Uns             UvLevel;   			// 31.������� ��������� ����������
	Uns             UvTime;    			// 32.����� ��������� ����������
	Uns             VSkLevel;          	// 33.������� ��������� ����������
	Uns             VSkTime;           	// 34.����� ��������� ����������
	Uns             BvLevel;   			// 35.������� ������ ������� ���
	Uns             BvTime;    			// 36.����� ������ ������� ���
	TPrtMode        Phl;           		// 37.������ �� ������ ��� ��������
	Uns             PhlLevel;      		// 38.������� ������ ���
	Uns             PhlTime;       		// 39.����� ����������� ������ ���
	TPrtMode        I2t;                // 40.�����-������� ������
	Uns             I2tMinCurrent;      // 41.������� ������������ ����
	Uns             I2tMidCurrent;      // 42.������� �������� ����
	Uns             I2tMaxCurrent;      // 43.������� ������������� ����
	Uns             I2tMinTime;         // 44.����� �� ����������� ����
	Uns             I2tMidTime;         // 45.����� �� ������� ����
	Uns             I2tMaxTime;         // 46.����� �� ������������ ����
	Uns				PhOrdTime;			// 47.����� ����������� ��� ���������
	Uns				DrvTLevel;			// 48. ������� ������������ ������ �� ��������� ���������
	Uns				DrvTInput;			// 49.
	TPrtMode        Ov;					// 50.������ �� ���������� ����������
	TPrtMode        Uv;					// 51.������ �� ���������� ����������
	TPrtMode        VSk;               	// 52.������ �� ��������� ����������
	TPrtMode        Bv;        			// 53.������ �� ������ ������� ���
	TPrtMode        PhOrd;          	// 54.������ �� ��������� ����������� ��� ���������
	Uns             OvDTime;     		// 55.����� ���������� ����������
	Uns             UvDTime;     		// 56.����� ���������� ����������
	Uns             MkuPoSubVersion;    // 24.��������� ��
	TPrtMode        IUnLoad;            // 58.������ �� ����� ��������
	Uns             IUnLevel;           // 59.��� ����� ��������
	Uns             IUnTime;            // 60.����� ����� ��������
	TPrtMode        ISkew;              // 61.������ �� ��������� ����
	Uns             ISkewLevel;         // 62.������� ��������� ����
	Uns             ISkewTime;          // 63.����� ��������� ����
	TPrtMode        ShC;        		// 64.������ �� ��������� ���������
	Uns				Rsvd5[3];			// 65-67 ��� ���� ������ � �������� ��� ������� ��
	TPrtMode        ErrIndic;           // 68.��������� ������ ����������
	TPrtMode        TemperTrack;        // 69.������ �� ���������/�������������� �����
	Int             TemperHigh;         // 70.������� ��������� �����
	Int             TemperLow;          // 71.������� �������������� �����
	Int             TenOnValue;         // 72.������� ��������� ����
	Int             TenOffValue;        // 73.������� ���������� ����
	Int             CorrTemper;         // 74.������������� ����������� �����
	TPrtMode        MuDuDef;        	// 75.������ ������ ������ ��/��
	Int				IU_Input;			// 76.�������� ��� ���������� �������� ���� IU
	Int				IV_Input;			// 77.�������� ��� ���������� �������� ���� IV
	Int				IW_Input;			// 78.�������� ��� ���������� �������� ���� IW
	Uns             IU_Mpy;             // 79.������������� ���� ���� U
	Uns             IV_Mpy;             // 80.������������� ���� ���� V
	Uns             IW_Mpy;             // 81.������������� ���� ���� W
	Uns             IU_Offset;          // 82.�������� ���� ���� U
	Uns             IV_Offset;          // 83.�������� ���� ���� V
	Uns             IW_Offset;          // 84.�������� ���� ���� W
	Uns             UR_Mpy;             // 85.������������� ���������� ���� R
	Uns             US_Mpy;             // 86.������������� ���������� ���� S
	Uns             UT_Mpy;             // 87.������������� ���������� ���� T
	Uns             UR_Offset;          // 88.�������� ���������� ���� R
	Uns             US_Offset;          // 89.�������� ���������� ���� S
	Uns             UT_Offset;          // 90.�������� ���������� ���� T
	Uns			    CoefVoltFltr;		// 91.����������� ���������� �������� ����������
	Uns			    CoefCurrFltr;		// 92.����������� ���������� ���� ��������
	Uns			    Dac_Mpy;			// 93.������������� ���
	Int			    Dac_Offset;			// 94.�������� ���
	Uns             ClosePosition;      // 95.��������� �������
	Uns             OpenPosition;       // 96.��������� �������
	TTenMode		Rsvd6[3];			// 97 - 99 ������
	Uns				ShC_Up; 			// 100 ������
	Uns				ShC_Down;			// 101 ������� � H
	Uns             Position;           // 102.���������
	Uns				LowPowerLvl;		// 103 �������� ���������� � �������� ��� 3� ������
	Int				Upor25;				// 104 �������� ��� ���������/��������� �������� ����� ��� 220� 25% �� �����
	Int				Upor35;				// 105 �������� ��� ���������/��������� �������� ����� ��� 220� 35% �� �����
	Int				Upor50;				// 106 �������� ��� ���������/��������� �������� ����� ��� 220� 50% �� �����
	Int				Upor75;				// 107 �������� ��� ���������/��������� �������� ����� ��� 220� 75% �� �����
	Int				Upor100;			// 108 �������� ��� ���������/��������� �������� ����� ��� 220� 100% �� �����
	Int				Corr40Trq;			// 109 �������� ��� ������������� ��������� ����� �������� (������ 60%)
	Int				Corr60Trq;			// 110 �������� ��� ������������� ��������� ������� �������� (������ 60%)
	Int				Corr80Trq;			// 111
	Int				Corr110Trq;			// 112
	Uns			    Rsvd2[7];			// 113 - 119 ������
} TRamGroupC;

// ������ D (����� = 210, ���������� = 20 )  	- �������
typedef struct _TRamGroupD
{
	TTaskReset     TaskClose;           // 0.������� �������
	TTaskReset     TaskOpen;            // 1.������� �������
	Uns            RevOpen;             // 2.������� �� ��������
	Uns            RevClose;            // 3.������� �� ��������
	Uns            AutoCalib;           // 4.�������������� ����������
	Uns            CalibReset;          // 5.����� ����������
	TValveCmd	   ControlWord;         // 6.������� ����������
	Uns			   PrtReset;      		// 7.����� �����
	Uns            SetDefaults;         // 8.������� ���������� �� ���������
	Uns 		   Rsvd1;				// 9.�������� ������� �������
	Uns            CycleReset;			// 10.����� �������� ������
	Uns			   RsReset;				// 11. ����� �����
	Uns 		   Rsvd[8];				// 12-19.������
} TRamGroupD;


// ������ G (����� 230, ���������� 30)  - ���������������� ��������� ����
typedef struct _TRamGroupG
{
	Uns            Mode;                // 0.����� �����
	TLedsReg       LedsReg;             // 1.���� ����������� �����
	TOutputReg     OutputReg;			// 2.���� ���������� �������
	Int			   DacValue;			// 3.�������� ���
	Uns			   DisplShow;			// 4.����� �������
	Uns			   TestCamera;			// 5.���� � ������
	Uns			   ThyrGroup;			// 6.����� ������ ����������
	Uns            SifuEnable;          // 7.���������� ������ ����
	Uns			   Rsvd2[10];			// 8-17.������
	Uns			   ThyrOpenAngle;		// 18.�������� ���� ��������
	Uns			   DiscrInTest;			// 19. ���� ���������� ������
	Uns			   DiscrOutTest;		// 20. ���� ���������� �������
	Bool		   IsDiscrTestActive;	// 21. ������� �� ���� ��/��
	Uns 		   Rsvd[8];			    // 22-29.������
} TRamGroupG;

// ������ H (����� 260, ���������� 140)
typedef struct _TRamGroupH
{
	TCalibState     CalibState;          // 0.��������� ����������
	Uns             CalibRsvd;           // 1.������ ��� ����������
	Uns             ClosePosition[2];    // 2-3.��������� �������
	Uns             OpenPosition[2];     // 4-5.��������� �������
										 // ������ ����� +2
	Uns             Password1;           // 6.�������� ������
	Uns             Password2;           // 7.��������� ������
	Uns             ScFaults;            // 8.������ ��
	Uns				UporOnly;			 // 9.�������� ���������� ������ ������ �� �����
	Uns             CycleCnt;            // 10.������� ������
	Uns				Rsvd1[5];			 // 11-15.������
	Uns				TransCurr;			 // 16.��� ��������
	TCubArray		TqCurr;				 // 17-36.��� �����������
	TCubArray		TqAngUI;			 // 37-56.���� ��������
	TCubArray		TqAngSf;			 // 57-76.���� ����
	Uns				ZazorTime;			 // 77.����� ������� ������
	TNetReg         FaultsNet;           // 78.����������� ����
  	Uns             Ur;                  // 79.���������� ���� R
  	Uns             Us;                  // 80.���������� ���� S
  	Uns             Ut;                  // 81.���������� ���� T
	Uns             Umid;             	 // 82.������� ����������
	Uns             VSkValue;          	 // 83.��������� ��� �������� ����
  	Uns     	    PhOrdValue;        	 // 84.����������� ��� ����
	TNetReg         DefectsNet;          // 85.����������� ���� (��� ��������������)
	Uns             Rsvd7;         	 	 // 86. ������
	TLoadReg        FaultsLoad;          // 87.����������� ��������
	Uns             Iu;                  // 88.��� ���� U
	Uns             Iv;                  // 89.��� ���� V
	Uns             Iw;                  // 90.��� ���� W
	Uns             Imid;				 // 91.������� ���
  	Uns             ISkewValue;          // 92.��������� ����� ��������
	Uns             Position;            // 93.���������
	LgUns  			FullStep;        	 // 94.������ ���
	Uns				Imidpr;			 	 // 95 ������
	TReverseType	ReverseType;         // 96.��� �������
	Uns				DemoPosition1;       // 97.���� ��������� 1
	Uns				DemoPosition2;       // 98.���� ��������� 2
	TInputReg       Inputs;           	 // 99.��������� ���������� ������
	TOutputReg      Outputs;          	 // 100.��������� ���������� �������
	Uns             TuReleMode;          // 101.�������� �����
	TNormState	    NormState[3];		 // 102-104.���������� ��������� ������    - �� ������������ �� ���� �� ���������� �����
	TNormState 	    NormOut[8];			 // 105-112.���������� ��������� ������� - �� �������� ���
	TDeviceReg      FaultsDev;           // 113.����������� ����������
	Uns             StartIndic;			 // 114.��������� � ������
 	Uns             SleepTime;           // 115.�������� �����  mb to C
	Uns             BusyValue;       	 // 116.������� ����������
	Uns 			LogEvAddr;			 // 117. ������� ����� ������� �������
	Uns 			LogCmdAddr;			 // 118. ������� ����� ������� ������
	Uns 			LogParamAddr;		 // 119. ������� ����� ������� ��������� ����������
	Uns				LogEvCount;			 // 120. ���������� ���������� ����� ������� �������
	Uns				LogCmdCount;		 // 121. ���������� ���������� ����� ������� ������
	Uns				LogParamCount;		 // 122. ���������� ���������� ����� ������� ��������� ����������
	Uns             Torque;           	 // 123.������
	Int             Speed;				 // 124.��������
	Uns				Seconds;			 // 125.�������
	TContactorGroup ContGroup;			 // 126. ���������� ������������
	TBurCmd 		LogControlWord;		 // 127. ������� ���
	Uns				LogReset;			 // 128. ����� ��������
	Uns 			Rsvd3[11];			 // 129-139.������
} TRamGroupH;

// ������ E (����� 400, ���������� 32)
typedef struct _TRamGroupE
{
	TTimeVar       LogTime;				// T.�����
	TDateVar       LogDate;				// D.����
	TStatusReg     LogStatus;           // 0.������ ������
	TFltUnion	   LogFaults;			// 1-4.������
	Int            LogPositionPr;       // 5.��������� %
	Uns            LogTorque;           // 6.������
	Uns            LogUr;               // 7.���������� ���� R
  	Uns            LogUs;               // 8.���������� ���� S
  	Uns            LogUt;               // 9.���������� ���� T
	Uns            LogIu;               // 10.��� ���� U
	Uns            LogIv;               // 11.��� ���� V
	Uns            LogIw;               // 12.��� ���� W
	Int            LogTemper;           // 13.����������� �����
	TInputReg      LogInputs;           // 14.��������� ���������� ������
	TOutputReg     LogOutputs;          // 15.��������� ���������� �������
	Uns 		   Rsvd[14];			// 16-29.������
} TRamGroupE;


typedef struct _TRamLog
{
	TStatusReg     	LogStatus;          // 0.������ ������
	Int            	LogPositionPr;		// 1.��������� %
	Uns            	LogTorque;          // 2.������
	Uns            	LogUr;              // 3.���������� ���� R
  	Uns            	LogUs;              // 4.���������� ���� S
  	Uns            	LogUt;              // 5.���������� ���� T
	Uns            	LogIu;              // 6.��� ���� U
	Uns            	LogIv;              // 7.��� ���� V
	Uns            	LogIw;              // 8.��� ���� W
	Int            	LogTemper;          // 9.����������� �����
	Uns      		LogInputs;        	// 10.��������� ���������� ������
	Uns				LogOutputs;			// 11.��������� ���������� �������
} TRamLog;



// ����� ���������
// ��������� ����� 40001
typedef struct _TTek_Mb_Group
{
	Uns 			Rsvd;				// 0 ������
	TTek_TechReg 	TechReg;			// 1 ��������������� �������
	TTek_DefReg  	DefReg;				// 2 ������� ��������
	Uns 		 	PositionPr;			// 3 ������� ��������� %
	TTek_ComReg  	ComReg;				// 4 ������� ������
	Uns 		 	CycleCnt;			// 5 ������� ������
	Uns			 	Rsvd1;				// 6 ������
	Uns 		 	Iu;					// 7 ��� ���� U
	Uns 		 	Rsvd2[9];			// 8 - 16 ������
	Uns 			Ur;					// 17 ���������� ������� ����
	Uns				Rsvd3;				// 18 ������
	Uns 		 	Speed;				// 19 ������� ��������
	Uns			 	Rsvd4;				// 20 ������
	Uns 		 	Torque;				// 21 ������� ������ �������� �*�
	TTek_Discrete 	TsTu;				// 22 ��/��
	Uns				Rsvd6[4];			// 23 - 26 ������
	Uns				RsStation;			// 27 ����� ������� (������ ��� ������)
	Uns				Rsvd7;				// 28 ������
} TTek_Mb_Group;


// ��������� ���������� ����������
typedef struct TRam
{
  TRamGroupA		ramGroupA;			// ��������� ���������
  TRamGroupB		ramGroupB;
  TRamGroupC		ramGroupC;
  TRamGroupD		ramGroupD;
  TRamGroupG		ramGroupG;
  TRamGroupH		ramGroupH;
  TRamGroupE		ramGroupE;
  TRamLog			ramLog;
} TRam;

// ��������� ���������� ����������
typedef struct _TTEKDriveData
{
	TTek_Mb_Group	tekGroup;
} TTekDriveData;


#define RAM_ADR				0
#define RAM_SIZE			sizeof(TRam)
#define RAM_SET_MAX			50
#define RAM_DATA_SIZE		(RAM_SIZE)
#define RAM_DATA_SIZE_GRB   sizeof(TRam_groupB)
#define RAM_EADR			(RAM_DATA_SIZE - 1)

#define DLOG_ADR			0x0500
#define DLOG_SIZE			0x0200
#define DLOG_DATA_ADR1		0x3FA000
#define DLOG_DATA_ADR2		0x3FB000
#define DLOG_EADR			(DLOG_ADR + (DLOG_SIZE<<1) - 1)

#define IR_ADR				0x0485
#define IR_EADR				0x0489

#define GetAdr(Elem)		((LgUns)&(((TRam *)RAM_ADR)->Elem))

//-------- ��������� �����					
#define REG_CODE			GetAdr(ramGroupA.PASSWORD1)
/*
#define GLOBAL_REG_GR		0
#define GLOBAL_COUNT_GR		1

#define REG_GR				GetAdr(ramGroup.STATUS)
#define COUNT_GR			30
*/
/*
#define REG_TASK_TIME		GetAdr(ramGroupD.TASK_TIME)
#define REG_TASK_DATE		GetAdr(ramGroupD.TASK_DATE)
//-------- ������� ���������
#define REG_PASSW1_NEW		GetAdr(ramGroupF.PASSW1)
#define REG_PASSW2_NEW		GetAdr(ramGroupF.PASSW2)
#define REG_REV_ERR_VALUE	GetAdr(ramGroupF.REV_ERR_VALUE)
#define REG_LAST_MIN		GetAdr(ramGroupF.LAST_MIN)
#define REG_LAST_SEC		GetAdr(ramGroupF.LAST_SEC)
#define REG_LAST_STATUS		GetAdr(ramGroupF.LAST_STATUS)
#define REG_CALIB_CLOSE		GetAdr(ramGroupF.CLB_CLOSE)
#define REG_CALIB_OPEN		REG_CALIB_CLOSE+2
#define REG_CALIB_STATUS	GetAdr(ramGroupF.CLB_STATUS)
//-------- �������
#define REG_SIN_FILTER_TF	GetAdr(ramGroupC.SIN_TF)
#define REG_RMS_FILTER_TF	GetAdr(ramGroupC.RMS_TF)
#define REG_TEMP_FILTER_TF	GetAdr(ramGroupC.TEMP_TF)
#define REG_ISOL_FILTER_TF	GetAdr(ramGroupC.ISO_TF)
//-------- ������
#define REG_INOM			GetAdr(ramGroupC.INOM)
#define REG_OVER_VOL_TIME	GetAdr(ramGroupC.OVER_VOL_TIME)
#define REG_UNDER_VOLT_TIME	GetAdr(ramGroupC.UNDER_VOLT_TIME)
#define REG_OVER_VOL_TIME_SHTDWN	GetAdr(ramGroupC.OVER_VOL_TIME_SHTDWN)
#define REG_UNDER_VOLT_TIME_SHTDWN	GetAdr(ramGroupC.UNDER_VOLT_TIME_SHTDWN)
#define REG_VSK_TIME		GetAdr(ramGroupC.VSK_TIME)
#define REG_BV_TIME			GetAdr(ramGroupC.BV_TIME)
#define REG_TEST_ISO_TIME	GetAdr(ramGroupC.TEST_ISO_TIME)
#define REG_OVER_CURRENT_TIME	GetAdr(ramGroupC.OVER_CURRENT_TIME)
#define REG_TEMP_MOTOR_TIME	GetAdr(ramGroupC.TEMP_MOTOR_TIME)
#define REG_PHL_TIME		GetAdr(ramGroupC.PHL_TIME)
#define REG_ISKEW_TIME		GetAdr(ramGroupC.ISKEW_TIME)
#define REG_TEMP_HIGH_TIME	GetAdr(ramGroupC.TEMP_HIGH_TIME)
#define REG_TEMP_LOW_TIME	GetAdr(ramGroupC.TEMP_LOW_TIME)
#define REG_CHANGE_RST		GetAdr(ramGroupC.CHANGE_RST)
#define REG_CHANGE_UVW_MOVE		GetAdr(ramGroupC.CHANGE_UVW_MOVE)
#define REG_CHANGE_UVW_STOP		GetAdr(ramGroupC.CHANGE_UVW_STOP)
#define REG_TEST_MOTOR_SC_TIME  GetAdr(ramGroupC.TEST_MOTOR_SC_TIME)

#define REG_TEMP_MOTOR_ENABLE_LEVEL		GetAdr(ramGroupC.TEMP_MOTOR_ENABLE_LEVEL)
#define REG_TEMP_MOTOR_DISABLE_LEVEL  	GetAdr(ramGroupC.TEMP_MOTOR_DISABLE_LEVEL)

#define REG_GEAR_RATIO		GetAdr(ramGroupC.GEAR_RATIO)
#define REG_FACTORY_NUMBER	GetAdr(ramGroupC.FACTORY_NUMBER)
#define REG_PRODUCT_DATE	GetAdr(ramGroupC.PRODUCT_DATE)

#define REG_TIME_TU			GetAdr(ramGroupB.TIME_TU)
#define REG_INDICATOR_TYPE	GetAdr(ramGroupB.INDICATOR_TYPE)
#define REG_LOCK_CONTROL	GetAdr(ramGroupB.LOCK_CONTROL)
#define REG_TEMP_SENS_TYPE	GetAdr(ramGroupB.TEMP_SENS_TYPE)
#define REG_LEVEL_DYN_BRAKE_BIG	GetAdr(ramGroupB.LEVEL_DYN_BRAKE_BIG)
*/
#define REG_RS_BAUD_RATE	GetAdr(ramGroup.RS_BAUD_RATE)

// ���������� ���������� ������
#define REG_VER_PO			GetAdr(ramGroupA.VER_PO)
#define REG_SUBVER_PO		GetAdr(ramGroupA.SUBVER_PO)

#define IsImpulseMode()		(!g_Ram.ramGroupA.DIGITAL_CMD_MODE)
#define IsPotentialMode()	(g_Ram.ramGroupA.DIGITAL_CMD_MODE)

// ���� ����������
#define LOCK_MPU			BIT0	// ���������� ���
#define LOCK_TU				BIT1	// ���������� ��
#define LOCK_RS485			BIT2	// ���������� RS485
#define IsLockMPU() 		(g_Ram.ramGroupA.LOCK_CONTROL & LOCK_MPU)
#define IsLockTU()			(g_Ram.ramGroupA.LOCK_CONTROL & LOCK_TU)

void g_Ram_Init(TRam *);
void g_Ram_Update(TRam *);
void RefreshParams(Uns);
Int MinMax3IntValue (Int, Int, Int);
Int Max3Int (Int , Int , Int );
void g_Ram_SetTorque(void);

extern TRam 			g_Ram;
extern TTekDriveData	g_RamTek;


#endif




