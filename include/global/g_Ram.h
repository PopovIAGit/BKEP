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
#include "comm.h"
#include "add_ValueConv.h"
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
// ������ A (����� = 500, ���������� = 40) - ����������� - ��������
typedef struct _TRamGroupA
{
	TStatusReg      Status;           	// 0. 500 ������ ������
	TFltUnion	    Faults;				// 1-4. 501-504 ������
	TInputReg       StateTu;     	    // 5. 505 ��������� ���������� ������
	TOutputReg      StateTs;		    // 6. 506 ��������� ���������� �������
	Uns             Position;        	// 7. 507 ���������
	Uns             Torque;           	// 8. 508 ������
	Int             Speed;				// 9. 509 ��������
	Uns             Ur;                 // 10. 510 ���������� ���� R
  	Uns             Us;                 // 11. 511 ���������� ���� S
  	Uns             Ut;                 // 12. 512 ���������� ���� T
	Uns             Iu;                 // 13. 513 ��� ���� U
	Uns             Iv;                 // 14. 514 ��� ���� V
	Uns             Iw;                 // 15. 515 ��� ���� W
	Int             AngleUI;            // 16. 516 ���� ��������
	TCalibState     CalibState;         // 17. 517 ������ ����������
	Int             PositionPr;         // 18. 518 ��������� %
	Uns             FullWay;            // 19. 519 ������ ���
	Int             CurWay;             // 20. 520 ������� ���
	Uns             CycleCnt;           // 21. 521 ������� ������
	Int             Temper;             // 22. 522 ����������� �����
	Int             TemperBKP;          // 23. 523 ����������� ����� ���
	Uns             VersionPO;       	// 24. 524 ������ ��
	Uns             VersionPOBkp;      	// 25. 525 ������ �� ���
	Uns				RevErrValue;		// 26. 526 ����� ����� ������� ���������
	Uns 			Rsvd[13];			// 27-39. 527-539 ������
} TRamGroupA;

// ������ B (����� = 40, ���������� = 100) - ��������� ������������
typedef struct _TRamGroupB
{
	Uns				MoveCloseTorque;	// B0. 40.������ ��������
	Uns				MoveOpenTorque;		// B1. 41 ������ ��������
	Uns				BreakCloseTorque;	// B2. 42 ������ ���������� �� ��������
	Uns				StartCloseTorque; 	// B3. 43 ������ �������� �� ��������
	Uns				BreakOpenTorque;  	// B4. 44 ������ ���������� �� ��������
	Uns				StartOpenTorque;  	// B5. 45 ������ �������� �� ��������
	TValveType		ValveType;			// B6. 46 ��� �������� (����/�����)
	TBreakMode      BreakMode;		   	// B7. 47 ��� ����������
	TRodType        RodType;            // B8. 48 .��� �����
	Uns				CloseZone;		 	// B9. 49 ���� �������
	Uns				OpenZone;		 	// B10. 50 ���� �������
	Uns             PositionAcc;        // B11. 51 ���� ��������
	Uns             MainCode;        	// B12. 52 ��� �������
	TTimeVar 		DevTime;			// B13. 53 �����
	TDateVar 		DevDate;			// B14. 54 ����
	Int		 		TimeCorrection;		// B15. 55 ������������� �������
	TIndicMode      IIndicMode;         // B16. 56 ����� ��������� ����
	TMuDuSetup      MuDuSetup;          // B17. 57 ��������� ������ ��/��
	TDuSource		DuSource;			// B18. 58 �������� ������ ��
	Uns             TuLockSeal;         // B19. 59 ���������� ��������
	Uns             TuTime;             // B20. 60 ����� �������
	TInputType 		InputType;			// B21. 61 ��� �������� ������� 24/220
	TUInvert	    TuInvert;			// B22. 62 ����� ���������� ������
	TSInvert 		TsInvert;			// B23. 63 ����� ���������� �������
	TBaudRate       RsBaudRate;         // B24. 64 �������� �����
	Uns             RsStation;          // B25. 65 ����� �������
	TParityMode		RsMode;				// B26. 66 ����� �����
	Uns				Rsvd4;				// B27. 67
	Uns				KeyInvert;			// B28. 68 ����� ������ ����������
	Uns				MOD_FAULT;			// B29. 69
	Uns				RES_ERR;			// B30. 70
	Uns 				Sec3Mode;		// B31. 71
	Uns				NoMoveTime;		   	// B32. 72 ����� ���������� ��������
	Uns				OverwayZone;		// B33. 73 �����
	TInputReg		DigitalMode;		// B34. 74 ����� ������������� / ����������
 	Uns             SleepTime;          // B35. 75 �������� �����
 	Uns 			Rsvd[4];			// B36-39. 76-79
 	//------��������� ��� ��------------------------------------
	Uns				LevelOnOpen220;		 // B40. 80
	Uns				LevelOffOpen220;	 // B41. 81
	Uns				LevelOnOpen24;		 // B42. 82
	Uns				LevelOffOpen24;		 // B43. 83
	Uns				UOpen_Mpy220;   	 // B44. 84
	Uns				UOpen_Mpy24;   		 // B45. 85
	Uns				p_UOpen_Offset;		 // B46. 86
	//-------------------------------
	Uns				LevelOnClose220;	 // B47. 87
	Uns				LevelOffClose220;	 // B48. 88
	Uns				LevelOnClose24;		 // B49. 89
	Uns				LevelOffClose24;	 // B50. 90
	Uns				p_UClose_Mpy220;	 // B51. 91
	Uns				p_UClose_Mpy24;		 // B52. 92
	Uns				p_UClose_Offset;	 // B53. 93
	//-------------------------------
	Uns				LevelOnStop220;		 // B54. 94
	Uns				LevelOffStop220;	 // B55. 95
	Uns				LevelOnStop24;		 // B56. 96
	Uns				LevelOffStop24;		 // B57. 97
	Uns				p_UStop_Mpy220;		 // B58. 98
	Uns				p_UStop_Mpy24;		 // B59. 99
	Uns				p_UStop_Offset;		 // B60. 100
	//-------------------------------
	Uns				LevelOnMU220;		 // B61. 101
	Uns				LevelOffMU220;		 // B62. 102
	Uns				LevelOnMU24;		 // B63. 103
	Uns				LevelOffMU24;		 // B64. 104
	Uns				p_UMu_Mpy220;		 // B65. 105
	Uns				p_UMu_Mpy24;		 // B66. 106
	Uns				p_UMu_Offset;		 // B67. 107
	//-------------------------------
	Uns				LevelOnResetAlarm220;	 // B68. 108
	Uns				LevelOffResetAlarm220;	 // B69. 109
	Uns				LevelOnResetAlarm24; 	 // B70. 110
	Uns				LevelOffResetAlarm24;	 // B71. 111
	Uns				p_UResetAlarm_Mpy220;	 // B72. 112
	Uns				p_UResetAlarm_Mpy24;	 // B73. 113
	Uns				p_UResetAlarm_Offset;	 // B74. 114
	//-------------------------------
	Uns				LevelOnReadyTU220;	 // B75. 115
	Uns				LevelOffReadyTU220;	 // B76. 116
	Uns				LevelOnReadyTU24;	 // B77. 117
	Uns				LevelOffReadyTU24;	 // B78. 118
	Uns				p_UReadyTu_Mpy220;	 // B79. 119
	Uns				p_UReadyTu_Mpy24;	 // B80. 120
	Uns				p_UReadyTu_Offset;	 // B81. 121
	//-------------------------------
	Uns				LevelOnDU220;		 // B82. 122
	Uns				LevelOffDU220;		 // B83. 123
	Uns				LevelOnDU24;		 // B84. 124
	Uns				LevelOffDU24;		 // B85. 125
	Uns				p_UDu_Mpy220;		 // B86. 126
	Uns				p_UDu_Mpy24;		 // B87. 127
	Uns				p_UDu_Offset;		 // B88. 128
	//-------------------------------
	Uns 			Rsvd3[11];//B89-99 129-139
} TRamGroupB;

// ������ C (����� = 140, ���������� = 120) - ��������� ���������
typedef struct _TRamGroupC
{
	Uns             FactCode;           // C0. 140 ��� �������
	TDriveType      DriveType;          // C1. 141 ��� �������
    Uns        		ProductYear;        // C2. 142 ��� ������������ �����
  	Uns             FactoryNumber;      // C3. 143 ��������� ����� �����
  	Uns             MaxTorque;          // C4. 144 ������������ ������ �������
	Uns             Inom;               // C5. 145 ����������� ���
  	Uns             GearRatio;          // C6. 146 ������������ ����� ���������
	Int             BlockToGearRatio;   // C7. 147 ����������� �������� ����-�������� ����������� ��������
	Uns             PosPrecision;       // C8. 148 �������� ���������
	Uns             PhOrdZone;      	// C9. 149 ���������� ����������� ��� ���������
	Uns             MuffZone;           // C10. 150 ���������� ������ �����
	Uns			    PosSensPow;			// C11. 151 ��� ������� ���������
	TPrtMode        CalibIndic;         // C12. 152 ��������� ����������
	Uns			    DisplResTout;		// C13. 153 ����� ������ ����������
	// ���������
	Uns             OpenAngle;          // C14. 154 ���� ���������� ����
	Int             AngleUI;            // C15. 155 ���� ��������
	TLedsReg        LedsReg;            // C16. 156 ��������� ����������� �����
	THallBlock      HallBlock;          // C17. 157 ��������� �������� ����� �����
	Uns             SetDefaults;        // C18. 158 ������� ���������� �� ���������
	Uns       	PlugBrakeDisable;    	// C19. 159 ���������� ���������� �����������������
	TPrtMode        DriveTemper;        // C20. 160 ������ �� ��������� ���������. (�������� ������ ��������� ����� � H)
	Uns             OvLevel_max;        // C21. 161 ������� ���������� ���������� ��� 47% ���������� (320�)
	Uns             OvTime_max;     	// C22. 162 ����� ���������� ���������� ��� 47% ���������� (1�)
	Uns             RevErrValue;       	// C23. 163 ���������� ����� ������� ���������
	Uns			    RevErrLevel;		// C24. 164 ������� ���� ������� ���������
	//-��������� �����--
	Uns             VoltAcc;            // C25. 165 ������������� �������
	Uns             BrakePause;         // C26. 166 ����� ����� �����������
	Uns             BrakeTime;          // C27. 167 ����� ����������
	Uns				KickCount;			// C28. 168 ����������� ������
	Uns             OvLevel;    		// C29. 169 ������� ���������� ����������
	Uns             OvTime;     		// C30. 170 ����� ���������� ����������
	Uns             UvLevel;   			// C31. 171 ������� ��������� ����������
	Uns             UvTime;    			// C32. 172 ����� ��������� ����������
	Uns             VSkLevel;          	// C33. 173 ������� ��������� ����������
	Uns             VSkTime;           	// C34. 174 ����� ��������� ����������
	Uns             BvLevel;   			// C35. 175 ������� ������ ������� ���
	Uns             BvTime;    			// C36. 176 ����� ������ ������� ���
	TPrtMode        Phl;           		// C37. 177 ������ �� ������ ��� ��������
	Uns             PhlLevel;      		// C38. 178 ������� ������ ���
	Uns             PhlTime;       		// C39. 179 ����� ����������� ������ ���
	TPrtMode        I2t;                // C40. 180 �����-������� ������
	Uns             I2tMinCurrent;      // C41. 181 ������� ������������ ����
	Uns             I2tMidCurrent;      // C42. 182 ������� �������� ����
	Uns             I2tMaxCurrent;      // C43. 183 ������� ������������� ����
	Uns             I2tMinTime;         // C44. 184 ����� �� ����������� ����
	Uns             I2tMidTime;         // C45. 185 ����� �� ������� ����
	Uns             I2tMaxTime;         // C46. 186 ����� �� ������������ ����
	Uns				PhOrdTime;			// C47. 187 ����� ����������� ��� ���������
	Uns				DrvTLevel;			// C48. 188 ������� ������������ ������ �� ��������� ���������
	Uns				DrvTInput;			// C49. 189
	TPrtMode        Ov;					// C50. 190 ������ �� ���������� ����������
	TPrtMode        Uv;					// C51. 191 ������ �� ���������� ����������
	TPrtMode        VSk;               	// C52. 192 ������ �� ��������� ����������
	TPrtMode        Bv;        			// C53. 193 ������ �� ������ ������� ���
	TPrtMode        PhOrd;          	// C54. 194 ������ �� ��������� ����������� ��� ���������
	Uns             OvDTime;     		// C55. 195 ����� ���������� ����������
	Uns             UvDTime;     		// C56. 196 ����� ���������� ����������
	Uns             SubVersionPO;	    // C57. 197 ��������� ��
	TPrtMode        IUnLoad;            // C58. 198 ������ �� ����� ��������
	Uns             IUnLevel;           // C59. 199 ��� ����� ��������
	Uns             IUnTime;            // C60. 200 ����� ����� ��������
	TPrtMode        ISkew;              // C61. 201 ������ �� ��������� ����
	Uns             ISkewLevel;         // C62. 202 ������� ��������� ����
	Uns             ISkewTime;          // C63. 203 ����� ��������� ����
	TPrtMode        ShC;        		// C64. 204 ������ �� ��������� ���������
	Uns				Rsvd5[3];			// C65-67 205-207 ��� ���� ������ � �������� ��� ������� ��
	TPrtMode        ErrIndic;           // C68. 208 ��������� ������ ����������
	TPrtMode        TemperTrack;        // C69. 209 ������ �� ���������/�������������� �����
	Int             TemperHigh;         // C70. 210 ������� ��������� �����
	Int             TemperLow;          // C71. 211 ������� �������������� �����
	Int             TenOnValue;         // C72. 212 ������� ��������� ����
	Int             TenOffValue;        // C73. 213 ������� ���������� ����
	Int             CorrTemper;         // C74. 214 ������������� ����������� ����� ���
	TPrtMode        MuDuDef;        	// C75. 215 ������ ������ ������ ��/��
	Int				IU_Input;			// C76. 216 �������� ��� ���������� �������� ���� IU
	Int				IV_Input;			// C77. 217 �������� ��� ���������� �������� ���� IV
	Int				IW_Input;			// C78. 218 �������� ��� ���������� �������� ���� IW
	Uns             IU_Mpy;             // C79. 219 ������������� ���� ���� U
	Uns             IV_Mpy;             // C80. 220 ������������� ���� ���� V
	Uns             IW_Mpy;             // C81. 221 ������������� ���� ���� W
	Uns             IU_Offset;          // C82. 222 �������� ���� ���� U
	Uns             IV_Offset;          // C83. 223 �������� ���� ���� V
	Uns             IW_Offset;          // C84. 224 �������� ���� ���� W
	Uns             UR_Mpy;             // C85. 225 ������������� ���������� ���� R
	Uns             US_Mpy;             // C86. 226 ������������� ���������� ���� S
	Uns             UT_Mpy;             // C87. 227 ������������� ���������� ���� T
	Uns             UR_Offset;          // C88. 228 �������� ���������� ���� R
	Uns             US_Offset;          // C89. 229 �������� ���������� ���� S
	Uns             UT_Offset;          // C90. 230 �������� ���������� ���� T
	Uns			    SinTf;				// C91. 231 ����.������� �������� �������
	Uns			    RmsTf;				// C92. 232 ����.������� �������� RMS
	Uns			    Dac_Mpy;			// C93. 233 ������������� ���
	Int			    Dac_Offset;			// C94. 234 �������� ���
	Uns             ClosePosition;      // C95. 235 ��������� �������
	Uns             OpenPosition;       // C96. 236 ��������� �������
	TTenMode		Rsvd6[3];			// C97-99. 237-239 ������
	Uns				ShC_Up; 			// C100. 240 ������
	Uns				ShC_Down;			// C101. 241 ������� � H
	Uns             Position;           // C102. 242 ���������
	Uns				LowPowerLvl;		// C103. 243 �������� ���������� � �������� ��� 3� ������
	Int				Upor25;				// C104. 244 �������� ��� ���������/��������� �������� ����� ��� 220� 25% �� �����
	Int				Upor35;				// C105. 245 �������� ��� ���������/��������� �������� ����� ��� 220� 35% �� �����
	Int				Upor50;				// C106. 246 �������� ��� ���������/��������� �������� ����� ��� 220� 50% �� �����
	Int				Upor75;				// C107. 247 �������� ��� ���������/��������� �������� ����� ��� 220� 75% �� �����
	Int				Upor100;			// C108. 248 �������� ��� ���������/��������� �������� ����� ��� 220� 100% �� �����
	Int				Corr40Trq;			// C109. 249 �������� ��� ������������� ��������� ����� �������� (������ 60%)
	Int				Corr60Trq;			// C110. 250 �������� ��� ������������� ��������� ������� �������� (������ 60%)
	Int				Corr80Trq;			// C111. 251
	Int				Corr110Trq;			// C112. 252
	TNetReg				FaultNetRST;	// C113. 253 ����� ��� �������
	Uns				LevelBreakRST;		// C114. 254
	Uns				TimeBreakRST;		// C115. 255
	Uns			    Rsvd2[4];			// C116-119. 256-259 ������
} TRamGroupC;

// ������ D (����� = 260, ���������� = 20 )  	- �������
typedef struct _TRamGroupD
{
	TTaskReset     TaskClose;           // D0. 260 ������� �������
	TTaskReset     TaskOpen;            // D1. 261 ������� �������
	Uns            RevOpen;             // D2. 262 ������� �� ��������
	Uns            RevClose;            // D3. 263 ������� �� ��������
	Uns            AutoCalib;           // D4. 264 �������������� ����������
	Uns            CalibReset;          // D5. 265 ����� ����������
	TValveCmd	   ControlWord;         // D6. 266 ������� ����������
	Uns			   PrtReset;      		// D7. 267 ����� �����
	Uns            SetDefaults;         // D8. 268 ������� ���������� �� ���������
	Uns 		   Rsvd1;				// D9. 269 �������� ������� �������
	Uns            CycleReset;			// D10. 270 ����� �������� ������
	Uns			   RsReset;				// D11. 271 ����� �����
	Uns 		   Rsvd[8];				// D12-19. 272-279 ������
} TRamGroupD;


// ������ G (����� 280, ���������� 30)  - ���������������� ��������� ����
typedef struct _TRamGroupG
{
	Uns            Mode;                // G0. 280 ����� �����
	TLedsReg       LedsReg;             // G1. 281 ���� ����������� �����
	TOutputReg     OutputReg;			// G2. 282 ���� ���������� �������
	Int			   DacValue;			// G3. 283 �������� ���
	Uns			   DisplShow;			// G4. 284 ����� �������
	Uns			   TestCamera;			// G5. 285 ���� � ������
	Uns			   ThyrGroup;			// G6. 286 ����� ������ ����������
	Uns            SifuEnable;          // G7. 287 ���������� ������ ����
	Uns			   Rsvd2[10];			// G8-17. 288-297-������
	Uns			   ThyrOpenAngle;		// G18. 298 �������� ���� ��������
	Uns			   DiscrInTest;			// G19. 299 ���� ���������� ������
	Uns			   DiscrOutTest;		// G20. 300 ���� ���������� �������
	Bool		   IsDiscrTestActive;	// G21. 301 ������� �� ���� ��/��
	Uns 		   Rsvd[8];			    // G22-29. 302-309 ������
} TRamGroupG;

// ������ H (����� 310, ���������� 200)
typedef struct _TRamGroupH
{
	TCalibState     CalibState;          // H0. 310 ��������� ����������
	Uns             CalibRsvd;           // H1. 311 ������ ��� ����������
	LgUns           ClosePosition;    	 // H2-3. 312-313 ��������� �������		???
	LgUns           OpenPosition;     	 // H4-5. 314-315 ��������� �������		???
	Uns             Password1;           // H6. 316 �������� ������
	Uns             Password2;           // H7. 317 ��������� ������
	Uns             ScFaults;            // H8. 318 ������ ��
	Uns				UporOnly;			 // H9. 319 �������� ���������� ������ ������ �� �����
	Uns             CycleCnt;            // H10. 320 ������� ������
	Uns				CmdKey;				 // H11. 321 ������� � ������ ���
	Uns				CmdButton;			 // H12. 322 ������� � ����� ����������
	Uns				Rsvd1[3];			 // H13-15. 323-325 ������
	Uns				TransCurr;			 // H16. 326 ��� ��������
	TCubArray		TqCurr;				 // H17-36. 327-346 ��� �����������
	TCubArray		TqAngUI;			 // H37-56. 347-366 ���� ��������
	TCubArray		TqAngSf;			 // H57-76. 367-386 ���� ����
	Uns				ZazorTime;			 // H77. 387 ����� ������� ������
	Uns             TuState;             // H78. 388 ������� �� (������� ������� ����)
  	Uns             Ur;                  // H79. 389 ���������� ���� R
  	Uns             Us;                  // H80. 390 ���������� ���� S
  	Uns             Ut;                  // H81. 391 ���������� ���� T
	Uns             Umid;             	 // H82. 392 ������� ����������
	Uns             VSkValue;          	 // H83. 393 ��������� ��� �������� ����
  	Uns     	    PhOrdValue;        	 // H84. 394 ����������� ��� ����
	Uns             Rsvd5;          	 // H85. 395 ����������� ���� (��� ��������������)
	Uns				Imidpr;			 	 // H86 396 ������
	Uns             Rsvd6;          	 // H87. 397 ����������� ��������
	Uns             IuPr;                // H88. 398 ��� ���� U
	Uns             IvPr;                // H89. 399 ��� ���� V
	Uns             IwPr;                // H90. 400 ��� ���� W
	Uns             Imid;				 // H91. 401 ������� ���
  	Uns             ISkewValue;          // H92. 402 ��������� ����� ��������
  	LgUns           Position;            // H93-94. 403-404 ���������
	LgUns  			FullStep;        	 // H95-96. 405-406 ������ ���
	TReverseType	ReverseType;         // H97. 407 ��� �������
	Uns				Rsvd4;				 // H98. 408
	TInputReg       StateTu;           	 // H99. 409 ��������� ���������� ������
	TOutputReg      StateTs;          	 // H100. 410 ��������� ���������� �������
	Uns             TuReleMode;          // H101. 411 �������� �����
	Uns      	    Rsvd2[12];		 	 // H102-113. 412-423���������� ��������� ������
	Uns             StartIndic;			 // H114. 424 ��������� � ������
 	Uns             SleepTime;           // H115. 425 �������� �����  mb to C
	Uns             BusyValue;       	 // H116. 426 ������� ����������
	Uns 			LogEvAddr;			 // H117. 427 ������� ����� ������� �������
	Uns 			LogCmdAddr;			 // H118. 428 ������� ����� ������� ������
	Uns 			LogParamAddr;		 // H119. 429 ������� ����� ������� ��������� ����������
	Uns				LogEvCount;			 // H120. 430 ���������� ���������� ����� ������� �������
	Uns				LogCmdCount;		 // H121. 431 ���������� ���������� ����� ������� ������
	Uns				LogParamCount;		 // H122. 432 ���������� ���������� ����� ������� ��������� ����������
	Uns             Torque;           	 // H123. 433 ������
	Int             Speed;				 // H124. 434 ��������
	Uns				Seconds;			 // H125. 435 �������
	TContactorGroup ContGroup;			 // H126. 436 ���������� ������������
	TBurCmd 		LogControlWord;		 // H127. 437 ������� ���
	Uns				LogReset;			 // H128. 438 ����� ��������
	Uns 			BkpIndication;		 // H129. 439 ��������� �� ���
	//-------------------------------
	Uns 			Rsvd8[59];			 // H130-189. 440-499 ������
} TRamGroupH;

// ������ E (����� 500+40, ���������� 32)
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
	TInputReg      LogStateTu;           // 14.��������� ���������� ������
	TOutputReg     LogSataeTs;          // 15.��������� ���������� �������
	Uns 		   Rsvd[14];			// 16-29.������
} TRamGroupE;


typedef struct _TRamLogBuff
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
} TRamLogBuff;



// ����� ���������
// ������ E (����� 0, ���������� 40)
// ��������� ����� 0
typedef struct _TRamGroupT
{
	TTek_TechReg 	TechReg;			// T1 0 ��������������� �������
	TTek_DefReg  	DefReg;				// T2 1 ������� ��������
	Uns 		 	PositionPr;			// T3 2 ������� ��������� %
	TTek_ComReg  	ComReg;				// T4 3 ������� ������
	Uns 		 	CycleCnt;			// T5 4 ������� ������
	Uns			 	Rsvd1;				// T6 5 ������
	Uns 		 	Iu;					// T7 6 ��� ���� U
	Uns 		 	Rsvd2[9];			// T8-16 7-15 ������
	Uns 			Ur;					// T17 16 ���������� ������� ����
	Uns				Rsvd3;				// T18 17 ������
	Uns 		 	Speed;				// T19 18 ������� ��������
	Uns			 	Rsvd4;				// T20 19 ������
	Uns 		 	Torque;				// T21 20 ������� ������ �������� �*�
	TTek_Discrete 	TsTu;				// T22 21 ��/��
	Uns				Rsvd6[4];			// T23-26 22-25 ������
	Uns				RsStation;			// T27 26 ����� ������� (������ ��� ������)
	Uns				Rsvd7;				// T28 27 ������
	Uns				Rsvd8[12];			// T29-40 28-39 ������
} TRamGroupT;


// ��������� ���������� ����������
typedef struct TRam
{
  TRamGroupT		ramGroupT;			//������ 0-39
  TRamGroupB		ramGroupB;			//������ 40-139
  TRamGroupC		ramGroupC;			//������ 140-259
  TRamGroupD		ramGroupD;			//������ 260-279
  TRamGroupG		ramGroupG;			//������ 280-309
  TRamGroupH		ramGroupH;			//������ 310-
  TRamGroupA		ramGroupA;			// ��������� ���������
  TRamGroupE		ramGroupE;
} TRam;

// ��������� ���������� ����������
/*typedef struct _TTEKDriveData
{
	TTek_Mb_Group	tekGroup;
} TTekDriveData;
*/

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
#define REG_CODE				GetAdr(ramGroupB.MainCode)
#define REG_FCODE				GetAdr(ramGroupC.FactCode)

#define REG_GEAR_RATIO			GetAdr(ramGroupC.GearRatio)

#define REG_SIN_FILTER_TF		GetAdr(ramGroupC.SinTf)
#define REG_RMS_FILTER_TF		GetAdr(ramGroupC.RmsTf)

#define REG_TU_TYPE				GetAdr(ramGroupB.InputType)
#define REG_TU_INVERT			GetAdr(ramGroupB.TuInvert.all)
#define REG_TS_INVERT			GetAdr(ramGroupB.TsInvert.all)


#define REG_DRIVE_TYPE			GetAdr(ramGroupC.DriveType)
#define REG_MAX_TRQE			GetAdr(ramGroupC.MaxTorque)
#define REG_I_NOM				GetAdr(ramGroupC.Inom)

#define REG_TORQUE_CURR 		GetAdr(ramGroupH.TqCurr)
#define REG_TORQUE_ANGLE_UI		GetAdr(ramGroupH.TqAngUI)
#define REG_TORQUE_ANG_SF		GetAdr(ramGroupH.TqAngSf)

#define REG_TASK_TIME			GetAdr(ramGroupB.DevTime)
#define REG_TASK_DATE			GetAdr(ramGroupB.DevDate)

#define REG_PASSW1_NEW			GetAdr(ramGroupH.Password1)
#define REG_PASSW2_NEW			GetAdr(ramGroupH.Password2)

#define REG_OVERWAY_ZONE		GetAdr(ramGroupB.OverwayZone)

/*
#define GLOBAL_REG_GR		0
#define GLOBAL_COUNT_GR		1

#define REG_GR				GetAdr(ramGroup.STATUS)
#define COUNT_GR			30
*/

#define REG_CALIB_STATE		GetAdr(ramGroupH.CalibState)
#define REG_CALIB_CLOSE		GetAdr(ramGroupH.ClosePosition)
#define REG_CALIB_OPEN		REG_CALIB_CLOSE+2

#define REG_FACTORY_NUMBER	GetAdr(ramGroupC.FactoryNumber)
#define REG_PRODUCT_DATE	GetAdr(ramGroupC.ProductYear)
#define REG_CYCLE_CNT		GetAdr(ramGroupH.CycleCnt)

#define REG_SHC_FAULT		GetAdr(ramGroupH.ScFaults)
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
#define REG_RS_BAUD_RATE	GetAdr(ramGroupB.RsBaudRate)

// ���������� ���������� ������
#define REG_VER_PO			GetAdr(ramGroupA.VersionPO)
#define REG_SUBVER_PO		GetAdr(ramGroupC.SubVersionPO)

//#define IsImpulseMode		(!g_Ram.ramGroupA.digitalMode.all)
//#define IsPotentialMode		(g_Ram.ramGroupA.digitalMode.all)

// ���� ����������
#define LOCK_MPU			BIT0	// ���������� ���
#define LOCK_TU				BIT1	// ���������� ��
#define LOCK_RS485			BIT2	// ���������� RS485
#define IsLockMPU() 		(g_Ram.ramGroupA.LOCK_CONTROL & LOCK_MPU)
#define IsLockTU()			(g_Ram.ramGroupA.LOCK_CONTROL & LOCK_TU)

#define CMD_DEFAULTS_USER	0x0010	// ���������������� ��������� �� ���������
#define CMD_RES_CLB			0x0020	// ����� ���������� ������� ���������
#define CMD_RES_FAULT		0x0040	// ����� �����
#define CMD_CLR_LOG			0x0080	// ������� �������
#define CMD_RES_CYCLE		0x0100	// ����� �������� ������
#define CMD_PAR_CHANGE		0x0200	// ��������� ����������
#define CMD_DEFAULTS_FACT 	0x0400	// ��������� ��������� �� ���������

#define STATE_TU24 		GpioDataRegs.GPBDAT.bit.GPIO40

void g_Ram_Init(TRam *);
void g_Ram_Update(TRam *);
void RefreshParams(Uns);
Int MinMax3IntValue (Int, Int, Int);
Int Max3Int (Int , Int , Int );


extern TRam 			g_Ram;
//extern TTekDriveData	g_RamTek;


#endif




