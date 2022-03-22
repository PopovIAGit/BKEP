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
		Uns KVO:1;			// 11
		Uns KVZ:1;			// 12
		Uns Ten:1;			// 13
		Uns Rsvd14:1;		// 14
		Uns Ready:1;		// 15
	} bit;
} TTek_TechReg;

typedef union _TTek_DefReg
{
	Uns all;
	struct
	{	// ��50
	    /*Uns Rsvd5:1;		// 0
		Uns ShC:1;		// 1	�������� ���������
		Uns I2t:1;		// 2	������������ ������
		Uns Rsvd3:1;		// 3
		Uns Phl:1;		// 4	������
		Uns Rsvd1:5;		// 5-9
		Uns Uv:1;		// 10	���������� ���������� ������� ����
		Uns Th:1;		// 11	�������� �����
		Uns Tl:1;		// 12
		Uns Ov:1;		// 13	���������� ���������� ������� ����
		Uns Rsvd2:1;		// 14
		Uns NoCalib:1;  	// 15*/
		// ���
		Uns I2t:1;			// 0	������������ ������
		Uns ShC:1;			// 1	�������� ���������
		Uns Drv_T:1;		// 2	�������� ����������������
		Uns Uv:1;			// 3	���������� ���������� ������� ����
		Uns Phl:1;			// 4	������
		Uns NoMove:1;		// 5	���������� ��������
		Uns Ov:1;			// 6	���������� ���������� ������� ����
		Uns Bv:1;			// 7	����� ��� ������� ����
		Uns Rsvd:1;			// 8	������
		Uns Th:1;			// 9	�������� �����
		Uns Tl:1;			// 10
		Uns Rsvd1:1;		// 11
		Uns PhOrdU:1;		// 12	�������� ����������� ��� ����
		Uns PhOrdDrv:1;		// 13	�������� ����������� ��� ���������
		Uns DevDef:1;		// 14
		Uns NoCalib:1;		// 15
	} bit;
} TTek_DefReg;

typedef union _TTEK_FaultReg
{
	Uns all;
	struct
	{
		Uns NoCalib:1;		// 0 	�����-������� ������
		Uns MuDu:1;			// 1 	�������� ���������
		Uns Uv:1;			// 2 	�������� ���������
		Uns Ov:1;			// 3 	���������� ���������� ��� ����
		Uns Vsk:1;			// 4 	����� ���� ����������������
		Uns Bv:1;			// 5 	��� ��������
		Uns ThErr:1;		// 6 	�������������� �� �����
		Uns Tl:1;			// 7 	����������� �������� ���������� �� �����
		Uns DevDef:1;		// 8 	������
		Uns Rsvd1:1;		// 9  	�������� �����
		Uns Rsvd2:1;		// 10 	�������������� �����
		Uns Rsvd3:1;		// 11 	������
		Uns Rsvd4:1;		// 12   �������� ����������� ��� ����
		Uns Rsvd5:1;		// 13   ����������� ��� ���������
		Uns Rsvd6:1;		// 14 	������ ���������� (����� ��������� � ��������������)
		Uns Rsvd7:1;		// 15 	������
	} bit;
} TTEK_FaultReg;

typedef union _TTek_ComReg
{
	Uns all;
	struct
	{
		Uns Stop:1;				// 0
		Uns Open:1;				// 1
		Uns Close:1;				// 2
		Uns Rsvd:2;				// 3-4
		Uns PrtReset:1;				// 5
		Uns Rsvd1:2;				// 6-7
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
	TStatusReg      Status;         // 0. 500 ������ ������
	TFltUnion	    Faults;	        // 1-4. 501-504 ������
	TInputReg       StateTu;     	// 5. 505 ��������� ���������� ������
	TOutputReg      StateTs;	    // 6. 506 ��������� ���������� �������
	Uns             Position;       // 7. 507 ���������
	Uns             Torque;         // 8. 508 ������
	Int             Speed;		    // 9. 509 ��������
	Uns             Ur;             // 10. 510 ���������� ���� R
  	Uns             Us;             // 11. 511 ���������� ���� S
  	Uns             Ut;             // 12. 512 ���������� ���� T
	Uns             Iu;             // 13. 513 ��� ���� U
	Uns             Iv;             // 14. 514 ��� ���� V
	Uns             Iw;             // 15. 515 ��� ���� W
	Int             AngleUI;        // 16. 516 ���� ��������
	TCalibState     CalibState;     // 17. 517 ������ ����������
	Int             PositionPr;     // 18. 518 ��������� %
	Uns             FullWay;        // 19. 519 ������ ���
	Int             CurWay;         // 20. 520 ������� ���
	Uns             CycleCnt;       // 21. 521 ������� ������
	Int             Temper;         // 22. 522 ����������� �����
	Int             TemperBKP;      // 23. 523 ����������� ����� ���
	Uns             VersionPO;      // 24. 524 ������ ��
	Uns             VersionPOBkp;   // 25. 525 ������ �� ���
	Uns				VoltageDown;	// 26. 526 ������� ����������
	Uns				RevErrValue;	// 27. 527 ����� ����� ������� ���������
	Uns 			TemperED;		// 28. 528 ����������� �� ��� ��� 9.1
	TBCP9Reg		BCP9Reg;		// 29. 529 faults from BCP91
	Uns 			Rsvd[8];	    // 30-39. 530-539 ������
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
	TBreakMode      BreakMode;			// B7. 47 ��� ����������
	TRodType        RodType;           	// B8. 48 ��� �����
	Uns				CloseZone;			// B9. 49 ���� �������
	Uns				OpenZone;			// B10. 50 ���� �������
	Uns             PositionAcc;        // B11. 51 ���� ��������
	Uns            	MainCode;        	// B12. 52 ��� �������
	TTimeVar 		DevTime;			// B13. 53 �����
	TDateVar 		DevDate;			// B14. 54 ����
	Int		 		TimeCorrection;		// B15. 55 ������������� �������
	TIndicMode  	IIndicMode;         // B16. 56 ����� ��������� ����
	TMuDuSetup  	MuDuSetup;          // B17. 57 ��������� ������ ��/��
	TDuSource		DuSource;			// B18. 58 �������� ������ ��
	Uns         	TuLockSeal;         // B19. 59 ���������� ��������
	Uns         	TuTime;             // B20. 60 ����� �������
	TInputType 		InputType;			// B21. 61 ��� �������� ������� 24/220
	TUInvert	    TuInvert;			// B22. 62 ����� ���������� ������
	TSInvert 		TsInvert;			// B23. 63 ����� ���������� �������
	TBaudRate       RsBaudRate;         // B24. 64 �������� �����
	Uns             RsStation;          // B25. 65 ����� �������
	TParityMode		RsMode;				// B26. 66 ����� �����
	Uns				MuffTimer;			// B27. 67
	Uns				SwitcherMuDuMode;	// B28. ����� � �������������� ������������� ��/�� �� ��� (����� �� � ������ ��)
	Uns				MuffStartTimer;		// B29. 69 ����� �� ������
	Uns				StartTime;		    // �30. 70 ����� �������
	Uns 			Sec3Mode;			// B31. 71
	Uns				NoMoveTime;			// B32. 72 ����� ���������� ��������
	Uns				OverwayZone;		// B33. 73 ������������ ���� ����������
	TPlaceType		PlaceType;			// B34. 74 ����� ��������� ������� (�����, ����������, �������)
	Uns				KvoKvzOffOnStop;	// B35. 75 ���������� ��� � ��� ��� ����, 0 - ���, 1 - �� // SDV ���
 	TStopMethod		StopMethod;			// B36. 76 ����� ���� ���������� (��������, ����������������)
 	Uns				ConductorSection;   // B37. 77 ������� ����������
 	Uns             ConductorLength;	// B38. 78 ����� ����������
 	Int				VoltCorr;			// B39.
 	Uns 			Rsvd[10];			// B40 41 42 43 44 45 46 47 48 49 -49. 77-89
} TRamGroupB;

// ������ C (����� = 90, ���������� = 120) - ��������� ���������
typedef struct _TRamGroupC
{
	Uns             FactCode;           	// C0. 90 ��� �������
	TDriveType      DriveType;          	// C1. 91 ��� �������
	Uns        		ProductYear;        	// C2. 92 ��� ������������ �����
  	Uns             FactoryNumber;      	// C3. 93 ��������� ����� �����
  	Uns             MaxTorque;          	// C4. 94 ������������ ������ �������
	Uns             Inom;               	// C5. 95 ����������� ���
  	Uns             GearRatio;          	// C6. 96 ������������ ����� ���������
  	Uns				BKP91;	    			// �7. 97 ������
  	Uns				TekDefRegSwitch;		// �8. 98 ������
	THallBlock      HallBlock;          	// C9. 99 ��������� �������� ����� �����
	Uns             SubVersionPO;	    	// C10. 100 ��������� ��
	Uns				progonCycles;			// �11. 101 ����� �������
	Uns				reserv102;				// �12. 102 ������
	Uns             MuffZone;           	// C13. 103 ���������� ������ �����
	Uns				PosSensPow;				// C14. 104 ��� ������� ���������
	Uns				reserv105;				// C15. 105 ������
	Uns             SetDefaults;        	// C16. 106 ������� ���������� �� ���������
	Uns				ConnFlagCountBCP;		// C17. 107 ��������� �������� �� ����������� ������ ����� � ���
	Uns             ModbusPauseStart;       // C18. 108 ����� ��� ������ �� ������ � �� - 09,01,2020 - ���������� ������ (� ������� �����)
	Uns             BrakePause;         	// C19. 109 ����� ����� �����������
	Uns             BrakeTime;          	// C20. 110 ����� ����������
	Uns       		StopShnTime;   			// C21. 111 ����� ���������� ������������ �����������
	Int				BreakZone;				// C22. 112 ����� �������� �� �� ������� �������� ���������
	Uns				BreakControl;			// c23. 113 ���������� �������� ���9.1
	Uns				Rsvd5[8];				// C24-33. 114-123 ������
	Uns				CounterDisconect;		// C32  122 ������� ������� ����� ����� ��� ���� (�������� � �� �� ������)
	Uns				TimeBtnStopKVOKVZ;		// C33  123 ����� ������� ��� ��� ��� �������� ����� ����
	Uns				BCPConTimeMove;			// C34  124 ����� ����������� ������ ��� ����� � ��������
	Int             CorrTemper;         	// C35. 125 ������������� ����������� ����� ���
	Uns        		BCPConTime;            // C36. 126 ����� ����������� ������ ��� ����� � �����
	Uns				CurrentMpyType;			// C37. 127 ���������� ������ ��������� ������� ��� ��� ��. �������� ������ ���� ��� ������� �� ���������
	Uns             IU_Mpy;             	// C38. 128 ������������� ���� ���� U
	Uns             IV_Mpy;             	// C39. 129 ������������� ���� ���� V
	Uns             IW_Mpy;             	// C40. 130 ������������� ���� ���� W
	Uns             IU_Offset;          	// C41. 131 �������� ���� ���� U
	Uns             IV_Offset;          	// C42. 132 �������� ���� ���� V
	Uns             IW_Offset;          	// C43. 133 �������� ���� ���� W
	Uns             UR_Mpy;             	// C44. 134 ������������� ���������� ���� R
	Uns             US_Mpy;             	// C45. 135 ������������� ���������� ���� S
	Uns             UT_Mpy;             	// C46. 136 ������������� ���������� ���� T
	Uns             UR_Offset;          	// C47. 137 �������� ���������� ���� R
	Uns             US_Offset;          	// C48. 138 �������� ���������� ���� S
	Uns             UT_Offset;          	// C49. 139 �������� ���������� ���� T
	Uns				SinTf;					// C50. 140 ����.������� �������� �������
	Uns				RmsTf;					// C51. 141 ����.������� �������� RMS
	Uns				Dac_Mpy;				// C52. 142 ������������� ���
	Int				Dac_Offset;				// C53. 143 �������� ���
	TPrtMode        Ov;						// C54. 144 ������ �� ���������� ����������
	Uns             OvLevel_max;        	// C55. 145 ������� ���������� ���������� ��� 47% ���������� (320�)
	Uns             OvTime_max;     		// C56. 146 ����� ���������� ���������� ��� 47% ���������� (1�)
	Uns             OvLevel;    			// C57. 147 ������� ���������� ����������
	Uns             OvDTime;     			// C58. 148 ����� ���������� ���������� �������������
	Uns             OvTime;     		// C59. 149 ����� ���������� ����������
	TPrtMode        Uv;			// C60. 150 ������ �� ���������� ����������
	Uns             UvLevel;   		// C61. 151 ������� ��������� ����������
	Uns             UvDTime;    		// C62. 152 ����� ��������� ����������
	Uns             UvTime;     		// C63. 153 ����� ���������� ����������
  	Uns				reserv154;		// �64. 154 ������
  	Uns				reserv155;		// �65. 155 ������
  	Uns				reserv156;		// �66. 156 ������
	TPrtMode        Bv;        		// C67. 157 ������ �� ������ ������� ���
	Uns             BvLevel;   		// C68. 158 ������� ������ ������� ���
	Uns				BvLevelMove;		// C69. 159 ������� ������ �������� ��� � ��������
	Uns             BvTime;    		// C70. 160 ����� ������ ������� ���
	Uns				Rsvd14;			// C71  161 ������
	TPrtMode        Phl;           		// C72. 162 ������ �� ������ ��� ��������
	Uns             PhlLevel;      		// C73. 163 ������� ������ ���
	Uns             PhlTime;       		// C74. 164 ����� ����������� ������ ���
	TPrtMode        I2t;                	// C75. 165 �����-������� ������
	Uns				Rsvd6[12];		// C76-84. 166-274 ������
	TPrtMode        ShC;        		// C88. 178 ������ �� ��������� ���������
	Uns				ShC_Up; 		// C89. 179 ������
	Uns				ShC_Down;		// C90. 180 ������� � H
	TPrtMode        CalibIndic;         	// C91. 181 ��������� ����������
	TPrtMode        ErrIndic;           	// C92. 182 ��������� ������ ����������
	TPrtMode        TemperTrack;        	// C93. 183 ������ �� ���������/�������������� �����
	Int				Rsvd13[3];		// C94 - 96. ������
	Int             TenOnValue;         	// C97. 187 ������� ��������� ����
	Int             TenOffValue;        	// C98. 188 ������� ���������� ����
	Uns 			Rsvd11[3];		// �99-101.  189-191 ������
	TPrtMode        PhOrd;          	// C102. 192 ������ �� ��������� ����������� ��� ���������
	Uns				PhOrdTime;		// C103. 193 ����� ����������� ��� ���������
	Uns             PhOrdZone;      	// C104. 194 ���������� ����������� ��� ���������
	TPrtMode        MuDuDef;        	// C105. 195 ������ ������ ������ ��/��
	Uns				Rsvd12[2];		// C106-107. 196-197 ������
	Int				CorrClose30Trq;		// C108. 198
	Int				CorrClose40Trq;		// C109. 199 �������� ��� ������������� ��������� ����� �������� (������ 60%)
	Int				CorrClose60Trq;		// C110. 200 �������� ��� ������������� ��������� ������� �������� (������ 60%)		Int		CorrOpen80Trq;		// C116. 206
	Int				CorrClose80Trq;		// C111. 201
	Int				CorrClose110Trq;	// C112. 202
	Int				CorrOpen30Trq;		// C113. 203
	Int				CorrOpen40Trq;		// C114. 204 �������� ��� ������������� ��������� ����� �������� (������ 60%)
	Int				CorrOpen60Trq;		// C115. 205 �������� ��� ������������� ��������� ������� �������� (������ 60%)
	Int				CorrOpen80Trq;		// C116. 206
	Int				CorrOpen110Trq;		// C117. 207
	Uns				Rsvd118;		// C118. 208 ������������� ������� � ��������
	Uns				TuOffsetCalib;		// C119.  209. ���������� ������� �� - ��������� ��� �� 220 ��� ������� ��������.
	//------��������� ��� ��------------------------------------
	Uns				LevelOn220;		// C120. 210
	Uns				LevelOff220;		// C121. 211
	Uns				LevelOn24;		// C122. 212
	Uns				LevelOff24;		// C123. 213
	//--------------------------------------------------
	Uns				p_UOpen_Mpy220;   	// C124. 214
	Uns				p_UOpen_Mpy24;   	// C125. 215
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
	Uns				p_UStopClose_Mpy24;	 // C134. 224
	Uns				p_UStopClose_Offset;	 // C135. 225
		//-------------------------------
	Uns				p_UMu_Mpy220;		 // C136. 226
	Uns				p_UMu_Mpy24;		 // C137. 227
	Uns				p_UMu_Offset;		 // C138. 228
		//------------------------------
	Uns				p_UDu_Mpy220;		 // C139. 229
	Uns				p_UDu_Mpy24;		 // C140. 230
	Uns				p_UDu_Offset;		 // C141. 231
	//-------------------------------
	Uns				p_UResetAlarm_Mpy220;	 // C142. 232
	Uns				p_UResetAlarm_Mpy24; 	 // C143. 233
	Uns				p_UResetAlarm_Offset;	 // C144. 234
	Uns				Tu_Mpy_Calib;		 // C145. 235
	Uns				Tu_Open_Value;		 // C146. 236
	Uns				Tu_Close_Value;		 // C147. 237
	Uns				Tu_StopOpen_Value;	 // C148. 238
	Uns				Tu_StopClose_Value;	 // C149. 239
	Uns				Tu_Du_Value;		 // C150. 240
	Uns				Tu_Mu_Value;		 // C151. 241
	Uns				Tu_ResetAlarm_Value;	 // C152. 242
	Uns 			Rsvd20[17];		 //�153-169 243-259
} TRamGroupC;

// ������ D (����� = 260, ���������� = 20 )  	- �������
typedef struct _TRamGroupD
{
	TTaskReset     	TaskClose;           // D0. 260 ������� �������
	TTaskReset     	TaskOpen;            // D1. 261 ������� �������
	Uns            	RevOpen;             // D2. 262 ������� �� ��������
	Uns           	RevClose;            // D3. 263 ������� �� ��������
	Uns	       		reserv264;			// D4  264 ������
	Uns            	CalibReset;          // D5. 265 ����� ����������
	TValveCmd      	ControlWord;         // D6. 266 ������� ����������
	Uns	       		PrtReset;      		// D7. 267 ����� �����
	Uns           	SetDefaults;         // D8. 268 ������� ���������� �� ���������
	Uns 			Rsvd1;				// D9. 269 �������� ������� �������
	Uns            	CycleReset;			// D10. 270 ����� �������� ������
	Uns				RsReset;				// D11. 271 ����� �����
	Uns 		   	Rsvd[8];				// D12-19. 272-279 ������
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
	Uns			   TestContactor;		// G6. 286 ���� �����������
	Uns			   Rsvd2[12];			// G7-18. 286-298-������
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
	Uns				BkpType;			 // H13. 323 ��� ��� ��� ����������� ��������������� ���� �������
	TDateVar 		HideDate;			 // H14. 324 ����
	Uns				TransCurr;			 // H16. 325 ��� ��������
	TCubArray		TqCurr;				 // H17-36. 326-349 ��� �����������
	TCubArray		TqAngUI;			 // H37-56. 350-373 ���� ��������
	TCubArray		TqAngSf;			 // H57-88. 374-397 ���� ����
	Uns             TuState;             // H78. 398 ������� �� (������� ������� ����� �����)
	Uns             Umid;             	 // H82. 399 ������� ����������
	Uns             BKP_Temper;          // H85. 400 ����������� ���
	Uns				Imidpr;			 	 // H86  401 ������
	Uns             IuPr;                // H88. 402 ��� ���� U
	Uns             IvPr;                // H89. 403 ��� ���� V
	Uns             IwPr;                // H90. 404 ��� ���� W
	Uns             Imid;				 // H91. 405 ������� ���
  	LgUns           Position;            // H92-93. 406-407 ���������
	LgUns  			FullStep;        	 // H94-95. 408-409 ������ ���
	TReverseType	ReverseType;         // H97. 410 ��� �������
	//-------------------------------
	Uns				BadTask_2kHz;        // H102. 411 ������� "������" ����� ���������� 2 ���
	Uns				BadTask_200Hz;       // H103. 412 ������� "������" ����� ���������� 200 ��
	Uns				BadTask_50Hz[3];     // H104-106. 413-415 ������� "������" ����� ���������� 50 ��
	Uns				BadTask_10Hz;        // H107. 416 ������� "������" ����� ���������� 10 ��
	Uns				BadTask_Reset;       // H108. 417 ����� ��������� "������" �����
	Uns				CpuTime;             // H109. 418 ������������ ����� ���������� ������
	Uns				TaskList;            // H110. 419 ����� ������ �����
	Uns				TaskNumber;          // H111. 420 ����� ������ � ������
	//-------------------------------
	Uns      	    Rsvd2[5];		 	 // H112-116. 421-425 ������
	Uns 			LogEvAddr;			 // H117. 426 ������� ����� ������� �������
	Uns 			LogCmdAddr;			 // H118. 427 ������� ����� ������� ������
	Uns 			LogParamAddr;		 // H119. 428 ������� ����� ������� ��������� ����������
	Uns 			LogSimAddr;		     // H120. 429 ������� ����� ������� SIM
	Uns				LogEvCount;			 // H121. 430 ���������� ���������� ����� ������� �������
	Uns				LogCmdCount;		 // H122. 431 ���������� ���������� ����� ������� ������
	Uns				LogParamCount;		 // H123. 432 ���������� ���������� ����� ������� ��������� ����������
	Uns				LogSimCount;		 // H124. 433 ���������� ���������� ����� ������� SIM
	Uns				Seconds;			 // H125. 434 �������
	TContactorGroup ContGroup;			 // H126. 435 ���������� ������������
	TBurCmd 		LogControlWord;		 // H127. 436 ������� ���
	Uns				LogReset;			 // H128. 437 ����� ��������
	TLedRegBKP 		BkpIndication;		 // H129. 438 ��������� �� ���
	//-------------------------------
	Uns				PosFix;				 // H130. 439
	Uns				PositionAccTemp;	 // H131. 440 14.07.21 ��������� ���������� ��� ���� ��������. ��������� ��� ����, ����� ���� �������� ���������� �� ����� �������� ��������������
	Uns 			Rsvd9[58];			 // H132-189. 441-499 ������
} TRamGroupH;

// ������ E (����� 500+40, ���������� 32)
typedef struct _TRamGroupE
{
	TTimeVar       LogTime;				// T.�����
	TDateVar       LogDate;				// D.����
	TStatusReg     LogStatus;           // 0.������ ������
	TFltUnion      LogFaults;			// 1-4.������
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

//---------------------------------------------------------------------------

// ��������� ���������� �������� �����
// ������ ATS (����� = 540, ���������� = 20) - ����������� - ��������
typedef struct _TRamGroupATS
{
	TATS48_ControlReg  		Control1;           // 0.
	Uns	            		Control2;			// 1.
	TATS48_StatusReg        State1;     	    // 2.
	TATS48_ExStatusReg      State2;		    	// 3.
	TATS48_Ex2StatusReg     State3;        		// 4.
	Uns						LFT;				// 5. ��������� ����������� ������������� (��� ���. 43 �������)
	Uns						PHP;				// 6. ������ �� ������ ���
	Uns 					Rsvd[13];			// 7-20. ������
} TRamGroupATS;

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
	TTek_DefReg		DefReg;				// T2 1 ������� ��������
	Int 			PositionPr;			// T3 2 ������� ��������� %
	TTek_ComReg  	ComReg;				// T4 3 ������� ������
	Uns 			CycleCnt;			// T5 4 ������� ������
	Uns				Rsvd1;				// T6 5 ������
	Uns 		 	Iu;					// T7 6 ��� ���� U
	Uns             Rsvd8;              // T8 7 ������
	Uns             Rsvd9;              // T9 8 ������
	Uns             Rsvd10;             // T10 9 ������
	Uns             Rsvd11;             // T11 10 ������
	Uns             Rsvd12;             // T12 11 ������
	Uns             Rsvd13;             // T13 12 ������
	Uns             Rsvd14;             // T14 13 ������
	Uns             Rsvd15;             // T15 14 ������
	Uns             Rsvd16;             // T16 15 ������
	Uns 			Ur;					// T17 16 ���������� ������� ����
	Uns				Rsvd17;				// T18 17 ������
	Uns 		 	Speed;				// T19 18 ������� ��������
	Uns			 	Rsvd4;				// T20 19 ������
	Uns 		 	Torque;				// T21 20 ������� ������ �������� �*�
	TTek_Discrete 	TsTu;				// T22 21 ��/��
	Uns				Rsvd23;  			// T23 22 ������
	Uns             Rsvd24;             // T24 23 ������
	Uns             Rsvd25;             // T25 24 ������
	Uns             Rsvd26;             // T26 25 ������
	Uns				RsStation;			// T27 26 ����� ������� (������ ��� ������) 40027
	Uns				MuDu;				// T28 27 ������
	Uns				Rsvd28;				// T29 28 ������
	Uns				PoVersion;			// T30 29 ������
	TTEK_FaultReg	FaultReg;			// T31 30 �������������� ������� �����
	Uns				Rsvd32;				// T32 31 ������
	Uns				Rsvd33;				// T33 32 ������
	Uns				Rsvd34;				// T34 33 ������
	Uns				Rsvd35;				// T35 34 ������
	Uns				Rsvd36;				// T36 35 ������
	Uns				Rsvd37;				// T37 36 ������
	Uns				Rsvd38;				// T38 37 ������
	Uns				Rsvd39;				// T39 38 ������
	Uns				Rsvd40;				// T40 39 ������
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
  TRamGroupATS		ramGroupATS;		// ��������� ���-ATS48
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
#define REG_ROD_TYPE			GetAdr(ramGroupB.RodType)

#define REG_DRIVE_TYPE			GetAdr(ramGroupC.DriveType)
#define REG_MAX_TRQE			GetAdr(ramGroupC.MaxTorque)
#define REG_I_NOM			GetAdr(ramGroupC.Inom)
#define REG_I_MPY			GetAdr(ramGroupC.IU_Mpy)

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
#define REG_REVCLOSE			GetAdr(ramGroupD.RevClose)
#define REG_RSRESET				GetAdr(ramGroupD.RsReset)
#define REG_COM_REG				GetAdr(ramGroupT.ComReg)


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
#define REG_BATTERY_FAULT	GetAdr(ramGroupH.HideDate)

#define REG_CONTROL_ATS48	GetAdr(ramGroupATS.Control1) //540

#define REG_RS_BAUD_RATE	GetAdr(ramGroupB.RsBaudRate)

#define REG_TU_OPEN_220		GetAdr(ramGroupC.p_UOpen_Mpy220)
#define REG_TU_OPEN_24		GetAdr(ramGroupC.p_UOpen_Mpy24)
#define REG_TU_OPEN_OFF		GetAdr(ramGroupC.p_UOpen_Offset)
#define REG_TU_CLOSE_220	GetAdr(ramGroupC.p_UClose_Mpy220)
#define REG_TU_CLOSE_24		GetAdr(ramGroupC.p_UClose_Mpy24)
#define REG_TU_CLOSE_OFF	GetAdr(ramGroupC.p_UClose_Offset)
#define REG_TU_STOPC_220	GetAdr(ramGroupC.p_UStopClose_Mpy220)
#define REG_TU_STOPC_24		GetAdr(ramGroupC.p_UStopClose_Mpy24)
#define REG_TU_STOPC_OFF	GetAdr(ramGroupC.p_UStopClose_Offset)
#define REG_TU_STOPO_220	GetAdr(ramGroupC.p_UStopOpen_Mpy220)
#define REG_TU_STOPO_24		GetAdr(ramGroupC.p_UStopOpen_Mpy24)
#define REG_TU_STOPO_OFF	GetAdr(ramGroupC.p_UStopOpen_Offset)
#define REG_TU_MU_220		GetAdr(ramGroupC.p_UMu_Mpy220)
#define REG_TU_MU_24		GetAdr(ramGroupC.p_UMu_Mpy24)
#define REG_TU_MU_OFF		GetAdr(ramGroupC.p_UMu_Offset)
#define REG_TU_DU_220		GetAdr(ramGroupC.p_UDu_Mpy220)
#define REG_TU_DU_24		GetAdr(ramGroupC.p_UDu_Mpy24)
#define REG_TU_DU_OFF		GetAdr(ramGroupC.p_UDu_Offset)
#define REG_TU_RESETAL_220	GetAdr(ramGroupC.p_UResetAlarm_Mpy220)
#define REG_TU_RESETAL_24	GetAdr(ramGroupC.p_UResetAlarm_Mpy24)
#define REG_TU_RESETAL_OFF	GetAdr(ramGroupC.p_UResetAlarm_Offset)

#define REG_CORR_CLOSE_30		GetAdr(ramGroupC.CorrClose30Trq)		// C108. 198
#define REG_CORR_CLOSE_40		GetAdr(ramGroupC.CorrClose40Trq)		// C109. 199 �������� ��� ������������� ��������� ����� �������� (������ 60%)
#define REG_CORR_CLOSE_60		GetAdr(ramGroupC.CorrClose60Trq)		// C110. 200 �������� ��� ������������� ��������� ������� �������� (������ 60%)		Int		CorrOpen80Trq;		// C116. 206
#define REG_CORR_CLOSE_80		GetAdr(ramGroupC.CorrClose80Trq)		// C111. 201
#define REG_CORR_CLOSE_110		GetAdr(ramGroupC.CorrClose110Trq)	// C112. 202
#define REG_CORR_OPEN_30		GetAdr(ramGroupC.CorrOpen30Trq)		// C113. 203
#define REG_CORR_OPEN_40		GetAdr(ramGroupC.CorrOpen40Trq)		// C114. 204 �������� ��� ������������� ��������� ����� �������� (������ 60%)
#define REG_CORR_OPEN_60		GetAdr(ramGroupC.CorrOpen60Trq)		// C115. 205 �������� ��� ������������� ��������� ������� �������� (������ 60%)
#define REG_CORR_OPEN_80		GetAdr(ramGroupC.CorrOpen80Trq)		// C116. 206
#define REG_CORR_OPEN_110		GetAdr(ramGroupC.CorrOpen110Trq)		// C117. 207

// ���������� ���������� ������
#define REG_VER_PO			GetAdr(ramGroupA.VersionPO)
#define REG_SUBVER_PO		GetAdr(ramGroupC.SubVersionPO)

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




