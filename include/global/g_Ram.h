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
#include "peref_LedsDrv.h"
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
		Uns Rsvd1:4;		// 3-6
		Uns MuDu:1;			// 7
		Uns Opening:1;		// 8
		Uns Closing:1;		// 9
		Uns Stop:1;			// 10
		Uns Rsvd3:2;		// 11-12
		Uns Ten:1;			// 13
		Uns Rsvd4:1;		// 14
		Uns Ready:1;		// 15
	} bit;
} TTek_TechReg;

typedef union _TTek_DefReg
{
	Uns all;
	struct
	{
		Uns I2t:1;		// 0	������������ ������
		Uns ShC:1;		// 1	�������� ���������
		Uns Drv_T:1;	// 2	�������� ����������������
		Uns Uv:1;		// 3	���������� ���������� ������� ����
		Uns Phl:1;		// 4	������
		Uns NoMove:1;	// 5	���������� ��������
		Uns Ov:1;		// 6	���������� ���������� ������� ����
		Uns Bv:1;		// 7	����� ��� ������� ����
		Uns Rsvd:1;		// 8	������
		Uns Th:1;		// 9	�������� �����
		Uns Tl:1;		// 10
		Uns Rsvd1:1;	// 11
		Uns PhOrdU:1;	// 12	�������� ����������� ��� ����
		Uns PhOrdDrv:1;	// 13	�������� ����������� ��� ���������
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
	TRodType        RodType;            // B8. 48 ��� �����
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
	Uns				MuffTimer;			// B27. 67
	Uns				KeyInvert;			// B28. 68 ����� ������ ����������
	Uns				MOD_FAULT;			// B29. 69
	Uns				RES_ERR;			// B30. 70
	Uns 			Sec3Mode;			// B31. 71
	Uns				NoMoveTime;		   	// B32. 72 ����� ���������� ��������
	Uns				OverwayZone;		// B33. 73 �����
	TInputReg		DigitalMode;		// B34. 74 ����� ������������� / ����������
 	Uns             SleepTime;          // B35. 75 �������� �����
 	TStopMethod		StopMethod;			// B36. 76 ����� ���� ���������� (��������, ����������������)
 	Uns 			Rsvd[13];			// B37-49. 77-89
} TRamGroupB;

// ������ C (����� = 90, ���������� = 120) - ��������� ���������
typedef struct _TRamGroupC
{
	Uns             FactCode;           // C0. 90 ��� �������
	TDriveType      DriveType;          // C1. 91 ��� �������
    Uns        		ProductYear;        // C2. 92 ��� ������������ �����
  	Uns             FactoryNumber;      // C3. 93 ��������� ����� �����
  	Uns             MaxTorque;          // C4. 94 ������������ ������ �������
	Uns             Inom;               // C5. 95 ����������� ���
  	Uns             GearRatio;          // C6. 96 ������������ ����� ���������
	Uns             RevErrValue;       	// C7. 97 ���������� ����� ������� ���������
	Uns			    RevErrLevel;		// C8. 98 ������� ���� ������� ���������
	THallBlock      HallBlock;          // C9. 99 ��������� �������� ����� �����
	Uns             SubVersionPO;	    // C10. 100 ��������� ��
	Int             BlockToGearRatio;   // C11.  101 ����������� �������� ����-�������� ����������� ��������
	Uns             PhOrdZone;      	// C12.  102 ���������� ����������� ��� ���������
	Uns             MuffZone;           // C13. 103 ���������� ������ �����
	Uns			    PosSensPow;			// C14. 104 ��� ������� ���������
	Uns			    DisplResTout;		// C15. 105 ����� ������ ����������
	Uns             SetDefaults;        // C16. 106 ������� ���������� �� ���������
	Uns             Rsvd7[2];          	// C17-18. 107-108 ������
	Uns             BrakePause;         // C19. 109 ����� ����� �����������
	Uns             BrakeTime;          // C20. 110 ����� ����������
	Uns       		StopShnTime;   		// C21. 111 ����� ���������� ������������ �����������
	Int				BreakZone;			// C22. 112 ����� �������� �� �� ������� �������� ���������
	Uns				Rsvd5[12];			// C23-34. 113-124 ������
	Int             CorrTemper;         // C35. 125 ������������� ����������� ����� ���
	Uns        		Rsvd8[2];           // C36-37. 126-127 ������
	Uns             IU_Mpy;             // C38. 128 ������������� ���� ���� U
	Uns             IV_Mpy;             // C39. 129 ������������� ���� ���� V
	Uns             IW_Mpy;             // C40. 130 ������������� ���� ���� W
	Uns             IU_Offset;          // C41. 131 �������� ���� ���� U
	Uns             IV_Offset;          // C42. 132 �������� ���� ���� V
	Uns             IW_Offset;          // C43. 133 �������� ���� ���� W
	Uns             UR_Mpy;             // C44. 134 ������������� ���������� ���� R
	Uns             US_Mpy;             // C45. 135 ������������� ���������� ���� S
	Uns             UT_Mpy;             // C46. 136 ������������� ���������� ���� T
	Uns             UR_Offset;          // C47. 137 �������� ���������� ���� R
	Uns             US_Offset;          // C48. 138 �������� ���������� ���� S
	Uns             UT_Offset;          // C49. 139 �������� ���������� ���� T
	Uns			    SinTf;				// C50. 140 ����.������� �������� �������
	Uns			    RmsTf;				// C51. 141 ����.������� �������� RMS
	Uns			    Dac_Mpy;			// C52. 142 ������������� ���
	Int			    Dac_Offset;			// C53. 143 �������� ���
	TPrtMode        Ov;					// C54. 144 ������ �� ���������� ����������
	Uns             OvLevel_max;        // C55. 145 ������� ���������� ���������� ��� 47% ���������� (320�)
	Uns             OvTime_max;     	// C56. 146 ����� ���������� ���������� ��� 47% ���������� (1�)
	Uns             OvLevel;    		// C57. 147 ������� ���������� ����������
	Uns             OvDTime;     		// C58. 148 ����� ���������� ���������� �������������
	Uns             OvTime;     		// C59. 149 ����� ���������� ����������
	TPrtMode        Uv;					// C60. 150 ������ �� ���������� ����������
	Uns             UvLevel;   			// C61. 151 ������� ��������� ����������
	Uns             UvDTime;    		// C62. 152 ����� ��������� ����������
	Uns             UvTime;     		// C63. 153 ����� ���������� ����������
	TPrtMode        VSk;               	// C64. 154 ������ �� ��������� ����������
	Uns             VSkLevel;          	// C65. 155 ������� ��������� ����������
	Uns             VSkTime;           	// C66. 156 ����� ��������� ����������
	TPrtMode        Bv;        			// C67. 157 ������ �� ������ ������� ���
	Uns             BvLevel;   			// C68. 158 ������� ������ ������� ���
	Uns				BvLevelMove;		// C69. 159 ������� ������ �������� ��� � ��������
	Uns             BvTime;    			// C70. 160 ����� ������ ������� ���
	Uns				Rsvd14;				// C71  161 ������
	TPrtMode        Phl;           		// C72. 162 ������ �� ������ ��� ��������
	Uns             PhlLevel;      		// C73. 163 ������� ������ ���
	Uns             PhlTime;       		// C74. 164 ����� ����������� ������ ���
	TPrtMode        I2t;                // C75. 165 �����-������� ������
	Uns				Rsvd6[9];			// C76-84. 166-274 ������
	TPrtMode        ISkew;              // C85. 175 ������ �� ��������� ����
	Uns             ISkewLevel;         // C86. 176 ������� ��������� ����
	Uns             ISkewTime;          // C87. 177 ����� ��������� ����
	TPrtMode        ShC;        		// C88. 178 ������ �� ��������� ���������
	Uns				ShC_Up; 			// C89. 179 ������
	Uns				ShC_Down;			// C90. 180 ������� � H
	TPrtMode        CalibIndic;         // C91. 181 ��������� ����������
	TPrtMode        ErrIndic;           // C92. 182 ��������� ������ ����������
	TPrtMode        TemperTrack;        // C93. 183 ������ �� ���������/�������������� �����
	Int				Rsvd13[3];			// C94 - 96. ������
	Int             TenOnValue;         // C97. 187 ������� ��������� ����
	Int             TenOffValue;        // C98. 188 ������� ���������� ����
	Uns 			Rsvd11[4];			// �99-102.  189-192 ������
	TPrtMode        PhOrd;          	// C103. 193 ������ �� ��������� ����������� ��� ���������
	Uns				PhOrdTime;			// C104. 194 ����� ����������� ��� ���������
	TPrtMode        MuDuDef;        	// C105. 195 ������ ������ ������ ��/��
	Uns			    Rsvd12[3];			// C106-108. 196-198 ������
	Int				Upor25;				// C109. 199 �������� ��� ���������/��������� �������� ����� ��� 220� 25% �� �����
	Int				Upor35;				// C110. 200 �������� ��� ���������/��������� �������� ����� ��� 220� 35% �� �����
	Int				Upor50;				// C111. 201 �������� ��� ���������/��������� �������� ����� ��� 220� 50% �� �����
	Int				Upor75;				// C112. 202 �������� ��� ���������/��������� �������� ����� ��� 220� 75% �� �����
	Int				Upor100;			// C113. 203 �������� ��� ���������/��������� �������� ����� ��� 220� 100% �� �����
	Int				Corr40Trq;			// C114. 204 �������� ��� ������������� ��������� ����� �������� (������ 60%)
	Int				Corr60Trq;			// C115. 205 �������� ��� ������������� ��������� ������� �������� (������ 60%)
	Int				Corr80Trq;			// C116. 206
	Int				Corr110Trq;			// C117. 207
	Uns			    Rsvd2;			// C118-119. 208-209 ������
	Uns				TuOffsetCalib;		// C119.  209. ���������� ������� �� - ��������� ��� �� 220 ��� ������� ��������.
	//------��������� ��� ��------------------------------------
	Uns				LevelOn220;			// C120. 210
	Uns				LevelOff220;		// C121. 211
	Uns				LevelOn24;			// C122. 212
	Uns				LevelOff24;			// C123. 213
	//--------------------------------------------------
	Uns				p_UOpen_Mpy220;   	// C124. 214
	Uns				p_UOpen_Mpy24;   		// C125. 215
	Uns				p_UOpen_Offset;		// C126. 216
		//------------------------------
	Uns				p_UClose_Mpy220;	// C127. 217
	Uns				p_UClose_Mpy24;		// C128. 218
	Uns				p_UClose_Offset;	// C129. 219
		//-------------------------------
	Uns				p_UStopOpen_Mpy220;	// C130. 220
	Uns				p_UStopOpen_Mpy24;	// C131. 221
	Uns				p_UStopOpen_Offset;	// C132. 222
	//-------------------------------
	Uns				p_UStopClose_Mpy220;	 // C133. 223
	Uns				p_UStopClose_Mpy24;	 	 // C134. 224
	Uns				p_UStopClose_Offset;	 // C135. 225
		//-------------------------------
	Uns				p_UMu_Mpy220;		// C136. 226
	Uns				p_UMu_Mpy24;		// C137. 227
	Uns				p_UMu_Offset;		// C138. 228
		//------------------------------
	Uns				p_UDu_Mpy220;		 // C139. 229
	Uns				p_UDu_Mpy24;		 // C140. 230
	Uns				p_UDu_Offset;		 // C141. 231
	//-------------------------------
	Uns				p_UResetAlarm_Mpy220;	 // C142. 232
	Uns				p_UResetAlarm_Mpy24; 	 // C143. 233
	Uns				p_UResetAlarm_Offset;	 // C144. 234

	Uns 			Rsvd20[25];			 //�145-169 235-259
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
	TLedReg        LedsReg;             // G1. 281 ���� ����������� �����
	TOutputReg     OutputReg;			// G2. 282 ���� ���������� �������
	Int			   DacValue;			// G3. 283 �������� ���
	Uns			   DisplShow;			// G4. 284 ����� �������
	Uns			   TestCamera;			// G5. 285 ���� � ������
	Uns			   Rsvd2[13];			// G6-18. 286-298-������
	Uns			   DiscrInTest;			// G19. 299 ���� ���������� ������	// �� ������������
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
	Uns				MuffFault;			 // H9. 319 ������ ������ �����
	Uns             CycleCnt;            // H10. 320 ������� ������
	Uns             TemperBCPFault;      // H11. 321 ������ ������ ����������� ��� 110
	Uns				CmdButton;			 // H12. 322 ������� � ����� ����������
	Uns				Rsvd1[3];			 // H13-15. 323-325 ������
	Uns				TransCurr;			 // H16. 326 ��� ��������
	TCubArray		TqCurr;				 // H17-36. 327-346 ��� �����������
	TCubArray		TqAngUI;			 // H37-56. 347-366 ���� ��������
	TCubArray		TqAngSf;			 // H57-76. 367-386 ���� ����
	Uns				Rsvd7;				 // H98. 387
	Uns             TuState;             // H78. 388 ������� �� (������� ������� ����� �����)
  	Uns             Rsvd3[3];            // H79. 389 ������
	Uns             Umid;             	 // H82. 392 ������� ����������
	Uns             VSkValue;          	 // H83. 393 ��������� ��� �������� ����
	Uns				Rsvd8;				 // H98. 408
	Uns             BKP_Temper;          // H85. 395 ����������� ���
	Uns				Imidpr;			 	 // H86 396 ������
	Uns             Rsvd12;          	 // H87. 397 ����������� ��������
	Uns             IuPr;                // H88. 398 ��� ���� U
	Uns             IvPr;                // H89. 399 ��� ���� V
	Uns             IwPr;                // H90. 400 ��� ���� W
	Uns             Imid;				 // H91. 401 ������� ���
  	LgUns           Position;            // H92-93. 402-403 ���������
	LgUns  			FullStep;        	 // H94-95. 404-405 ������ ���
  	Uns             ISkewValue;          // H96. 406 ��������� ����� ��������
	TReverseType	ReverseType;         // H97. 407 ��� �������
	Uns				Rsvd4;				 // H98. 408
	TInputReg       StateTu;           	 // H99. 409 ��������� ���������� ������
	TOutputReg      StateTs;          	 // H100. 410 ��������� ���������� �������
	Uns				Rsvd5;				 // H98. 408
	//-------------------------------
	Uns				BadTask_2kHz;        // H102. 412 ������� "������" ����� ���������� 2 ���
	Uns				BadTask_200Hz;       // H103. 413 ������� "������" ����� ���������� 200 ��
	Uns				BadTask_50Hz[3];     // H104-106. 414-416 ������� "������" ����� ���������� 50 ��
	Uns				BadTask_10Hz;        // H107. 417 ������� "������" ����� ���������� 10 ��
	Uns				BadTask_Reset;       // H108. 418 ����� ��������� "������" �����
	Uns				CpuTime;             // H109. 419 ������������ ����� ���������� ������
	Uns				TaskList;            // H110. 420 ����� ������ �����
	Uns				TaskNumber;          // H111. 421 ����� ������ � ������
	//-------------------------------
	Uns      	    Rsvd2[5];		 	 // H112-113. 422-423 ������
	Uns 			LogEvAddr;			 // H117. 427 ������� ����� ������� �������
	Uns 			LogCmdAddr;			 // H118. 428 ������� ����� ������� ������
	Uns 			LogParamAddr;		 // H119. 429 ������� ����� ������� ��������� ����������
	Uns 			LogSimAddr;		     // H120. 430 ������� ����� ������� SIM
	Uns				LogEvCount;			 // H121. 431 ���������� ���������� ����� ������� �������
	Uns				LogCmdCount;		 // H122. 432 ���������� ���������� ����� ������� ������
	Uns				LogParamCount;		 // H123. 433 ���������� ���������� ����� ������� ��������� ����������
	Uns				LogSimCount;		 // H124. 434 ���������� ���������� ����� ������� SIM
	Uns				Seconds;			 // H125. 435 �������
	TContactorGroup ContGroup;			 // H126. 436 ���������� ������������
	TBurCmd 		LogControlWord;		 // H127. 437 ������� ���
	Uns				LogReset;			 // H128. 438 ����� ��������
	TLedRegBKP 		BkpIndication;		 // H129. 439 ��������� �� ���
	//-------------------------------
	Uns 			Rsvd9[59];			 // H130-189. 440-499 ������
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

#define SHN_DATA_ADR		4000
#define SHN_DATA_SIZE		50
#define SHN_DATA_LADR		(SHN_DATA_ADR + SHN_DATA_SIZE - 1)

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

#define REG_CONTROL				GetAdr(ramGroupD.ControlWord)
#define REG_TASKCLOSE			GetAdr(ramGroupD.TaskClose)
#define REG_RSRESET				GetAdr(ramGroupD.RsReset)

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
#define REG_SET_DEFAULTS	GetAdr(ramGroupC.SetDefaults)
#define REG_SHC_FAULT		GetAdr(ramGroupH.ScFaults)
#define REG_MUFF_FAULT		GetAdr(ramGroupH.MuffFault)
#define REG_TEMP_BCP_FAULT	GetAdr(ramGroupH.TemperBCPFault)
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

#define TU_24_220 		    GpioDataRegs.GPADAT.bit.GPIO12

void g_Ram_Init(TRam *);
void g_Ram_Update(TRam *);
void RefreshParams(Uns);
Int MinMax3IntValue (Int, Int, Int);
Int Max3Int (Int , Int , Int );


extern TRam 			g_Ram;
//extern TTekDriveData	g_RamTek;


#endif




