/*======================================================================
��� �����:          core_Menu.c
�����:
������ �����:
���� ���������:
======================================================================*/

#include "add_ValueConv.h"
#include "core_MenuParams.h" 	// ���� ����������
#include "core.h"
#include "peref.h"
#include "g_Ram.h"
#include "stat.h"
#include "project_Version.h"

#if NEW_RAZ
TCoreMenu menu;// = MENU_DEFAULT;
#define EXPRESS_MAX_STATE		34

__inline Bool SleepMode(TCoreMenu *);
__inline void StartState(TCoreMenu *);
__inline void SelectGroup(TCoreMenu *);
__inline void SelectParam(TCoreMenu *);
__inline void EditParam(TCoreMenu *);
//__inline void ExpressState(TCoreMenu *);
__inline void EditDec(TCoreMenu *, Char Key);
__inline void EditStr(TCoreMenu *, Char Key);
__inline void EditTime(TCoreMenu *, Char Key);
__inline void EditDate(TCoreMenu *, Char Key);
__inline void EditBin(TCoreMenu *);
static   void ReadDcrFull(TCoreMenu *, Uns Addr);
static   void ChangeCoordinate(TCoreMenu *, Char Key, Bool GroupFlag);
__inline void ShowStart(TCoreMenu *);
__inline void ShowGroup(TCoreMenu *);
__inline Byte DefineBit(TCoreMenu *, Uns Value);
static   void ShowValue(TCoreMenu *, Uns Position, Bool Edit);
static   void ShowParam(TCoreMenu *, Bool Edit);
/*__inline void ShowExpress(TCoreMenu *);
__inline void ShowExpressChoice(TCoreMenu *p, Byte YesOne, Byte YesTwo,
									Byte NoOne, Byte NoTwo, Bool Yes);*/
__inline void ReadWriteAllParams(Byte, TCoreMenu *);
static   void PutAddData(Uns Addr, Uns *Value);

#endif

Byte 	showLog = 0, 
		startReadLog = 0, 
		TimeOutReadLog = 0, 
		taskStartShow = 0;
Uns 	grcGear = 0;
Uns 	a = 0;
Bool ExpNextStateRdy = false;
Byte SelectParamShow=0;

extern Uns predBreakCloseTorque;
extern Uns predTaskZoneStopReg;
extern Uns predStartCloseTorque;
extern Uns predRunCloseTorque;
extern Uns predUporCloseTorque;
extern Uns predStartOpenTorque;
extern Uns predRunOpenTorque;
extern Uns predUporOpenTorque;
extern Uns predTaskZoneStartReg;
extern Uns predNoMoveTime1;
extern Uns predNoMoveTime2;
extern Uns predNoMoveTime3;
extern Uns MAX_TORQUE;
Bool fl_TestIndicator = false;

__inline void ReadWriteAllParams(Byte, TCoreMenu *);
//---------------------------------------------------
void Core_MenuInit(TCoreMenu *p)
{
#if NEW_RAZ
		p->State = MS_START;        		// ����� ����
		p->Key = 0;          				// ����������� �������
		p->EditType = 0;     				// ��� ������ ��������������
		p->Level = 0;        				// ������� ������� ����
		p->MaxLevel = 1;     				// ������� ����������� ���� (1 ��� 2)
		p->Indication = True;   			// ���� ���������� ���������
		p->Update = True;       			// ���� ���������� ������ �� ��������
		p->ShowReserved = False; 			// ���� ����������� ��������

		p->MinMaxGain = &g_Ram.ramGroupC.MaxTorque;   // ����������� ���������

		p->Value = 0;        				// ������� �������� ���������
		p->Rate = 0;         				// ������� ������
		p->EditRate = 0;     				// ������������ ������ ��� ��������������
		p->Blink = False;        			// ��������� ������� ��������
		p->BlinkTimer = 0;   				// ������ ��� �������
		p->BlinkHiTime = BLINK_HI_TOUT;  	// ����� ����������� ��� ��������������
		p->BlinkLoTime = BLINK_LO_TOUT;  	// ����� ������� ��� ��������������
		p->Bit = 0xFF;          			// ��������� ���� ����� � ���� ������� ������
		p->BitTimer = 0;     				// ������ ����� ��� ������ ����
		p->BitTime = BIT_TOUT;      		// ����� ��� ������ ����

		p->SleepActive = false;  			// ��������� ������
		p->SleepTimer = 0;   				// ������ ������
		p->SleepTime = 0;    				// ����-��� ������

		p->Data = ToPtr(&g_Ram);         		// ��������� �� ����� ������
		p->HiString = g_Peref.Display.HiStr;     // ��������� �� ����� ������� ������� ����������
		p->LoString = g_Peref.Display.LoStr;     // ��������� �� ����� ������ ������� ����������

		p->StartAddr = REG_START_IND;   	// ����� ��������� ��� ������ � ������ ������
		p->StartOffset = RAM_ADR;  			// �������� ������ ��� ������ � ������ ������

		p->EvLogFlag = False;    			// ���� ������ ������
		p->EvLogSelected = False;			// ���� ������ ������
		p->EvLogGroup = MENU_EVLOG_GROUP;   // ����� ������ �������
		p->EvLogAdress = REG_LOG_ADDR;  	// ����� ��������� ������� ������ � ������
		p->EvLogTime = REG_LOG_TIME;    	// ����� ��������� ������� ������ �������
		p->EvLogDate = REG_LOG_DATE;    	// ����� ��������� ���� ������ �������
		p->EvLogCount = &g_Ram.ramGroupH.LogEvCount;	 // ���������� ��������� ������� � ������

		p->Group.Position = 0;
		p->Group.Addr = 0;
		p->Group.Count = MENU_GROUPS_COUNT;

		p->SubGroup.Position = 0;
		p->SubGroup.Addr = 0;
		p->SubGroup.Count = 0;

		p->EvLog.Position = 0;
		p->EvLog.Addr = 0;
		p->EvLog.Count = LOG_EV_CNT;

		p->Param.Position = 0;
		p->Param.Addr = 0;
		p->Param.Count = 0;

		p->Dcr.Min = 0;
		p->Dcr.Max = 0;
		p->Dcr.Def = 0;
		p->Dcr.Config.all = 0;

		p->setDefaultGroupNumber = 0;

		p->Express.Enable = False;
		p->Express.Select = False;
		p->Express.State = 0;
		p->Express.Index = 0;
		p->Express.First = False;
		p->Express.List = 0;

		p->Express.List1.Cnt = 4;		// ��������� �������� ���������
		p->Express.List1.Buf[0] = GetAdr(ramGroupB.DevTime);		// ��������� �������� ���������
		p->Express.List1.Buf[1] = GetAdr(ramGroupB.DevDate);		// ��������� �������� ���������
		p->Express.List1.Buf[2] = GetAdr(ramGroupB.RodType);		// ��������� �������� ���������
		p->Express.List1.Buf[3] = GetAdr(ramGroupB.ValveType);		// ��������� �������� ���������
		p->Express.List1.Buf[4] = 0;		// ��������� �������� ���������

		p->Express.List2.Cnt = 2;		// ��������� �������� ���������
		p->Express.List2.Buf[0] = GetAdr(ramGroupB.MoveCloseTorque);		// ��������� �������� ���������
		p->Express.List2.Buf[1] = GetAdr(ramGroupB.MoveOpenTorque);		// ��������� �������� ���������
		p->Express.List2.Buf[2] = 0;		// ��������� �������� ���������
		p->Express.List2.Buf[3] = 0;		// ��������� �������� ���������
		p->Express.List2.Buf[4] = 0;		// ��������� �������� ���������

		p->Express.List3.Cnt = 1;		// ��������� �������� ���������
		p->Express.List3.Buf[0] = GetAdr(ramGroupD.TaskClose);		// ��������� �������� ���������
		p->Express.List3.Buf[1] = 0;		// ��������� �������� ���������
		p->Express.List3.Buf[2] = 0;		// ��������� �������� ���������
		p->Express.List3.Buf[3] = 0;		// ��������� �������� ���������
		p->Express.List3.Buf[4] = 0;		// ��������� �������� ���������

		p->Express.List4.Cnt = 1;		// ��������� �������� ���������
		p->Express.List4.Buf[0] = GetAdr(ramGroupD.TaskOpen);		// ��������� �������� ���������
		p->Express.List4.Buf[1] = 0;		// ��������� �������� ���������
		p->Express.List4.Buf[2] = 0;		// ��������� �������� ���������
		p->Express.List4.Buf[3] = 0;		// ��������� �������� ���������
		p->Express.List4.Buf[4] = 0;		// ��������� �������� ���������

		p->Express.List5.Cnt = 1;		// ��������� �������� ���������
		p->Express.List5.Buf[0] = GetAdr(ramGroupD.RevOpen);		// ��������� �������� ���������
		p->Express.List5.Buf[1] = 0;		// ��������� �������� ���������
		p->Express.List5.Buf[2] = 0;		// ��������� �������� ���������
		p->Express.List5.Buf[3] = 0;		// ��������� �������� ���������
		p->Express.List5.Buf[4] = 0;		// ��������� �������� ���������

		p->Express.List6.Cnt = 1;		// ��������� �������� ���������
		p->Express.List6.Buf[0] = GetAdr(ramGroupD.RevClose);		// ��������� �������� ���������
		p->Express.List6.Buf[1] = 0;		// ��������� �������� ���������
		p->Express.List6.Buf[2] = 0;		// ��������� �������� ���������
		p->Express.List6.Buf[3] = 0;		// ��������� �������� ���������
		p->Express.List6.Buf[4] = 0;		// ��������� �������� ���������

		p->Express.List7.Cnt = 1;		// ��������� �������� ���������
		p->Express.List7.Buf[0] = GetAdr(ramGroupB.BreakCloseTorque);		// ��������� �������� ���������
		p->Express.List7.Buf[1] = 0;		// ��������� �������� ���������
		p->Express.List7.Buf[2] = 0;		// ��������� �������� ���������
		p->Express.List7.Buf[3] = 0;		// ��������� �������� ���������
		p->Express.List7.Buf[4] = 0;		// ��������� �������� ���������

		p->Express.List8.Cnt = 1;		// ��������� �������� ���������
		p->Express.List8.Buf[0] = GetAdr(ramGroupB.StartCloseTorque);		// ��������� �������� ���������
		p->Express.List8.Buf[1] = 0;		// ��������� �������� ���������
		p->Express.List8.Buf[2] = 0;		// ��������� �������� ���������
		p->Express.List8.Buf[3] = 0;		// ��������� �������� ���������
		p->Express.List8.Buf[4] = 0;		// ��������� �������� ���������

		p->Express.List9.Cnt = 1;		// ��������� �������� ���������
		p->Express.List9.Buf[0] = GetAdr(ramGroupB.BreakOpenTorque);		// ��������� �������� ���������
		p->Express.List9.Buf[1] = 0;		// ��������� �������� ���������
		p->Express.List9.Buf[2] = 0;		// ��������� �������� ���������
		p->Express.List9.Buf[3] = 0;		// ��������� �������� ���������
		p->Express.List9.Buf[4] = 0;		// ��������� �������� ���������

		p->Express.List10.Cnt = 1;		// ��������� �������� ���������
		p->Express.List10.Buf[0] = GetAdr(ramGroupB.StartOpenTorque);		// ��������� �������� ���������
		p->Express.List10.Buf[1] = 0;		// ��������� �������� ���������
		p->Express.List10.Buf[2] = 0;		// ��������� �������� ���������
		p->Express.List10.Buf[3] = 0;		// ��������� �������� ���������
		p->Express.List10.Buf[4] = 0;		// ��������� �������� ���������

		p->Express.List11.Cnt = 1;		// ��������� �������� ���������
		p->Express.List11.Buf[0] = 0;		// ��������� �������� ���������
		p->Express.List11.Buf[1] = 0;		// ��������� �������� ���������
		p->Express.List11.Buf[2] = 0;		// ��������� �������� ���������
		p->Express.List11.Buf[3] = 0;		// ��������� �������� ���������
		p->Express.List11.Buf[4] = 0;		// ��������� �������� ���������

		p->Express.List12.Cnt = 1;		// ��������� �������� ���������
		p->Express.List12.Buf[0] = GetAdr(ramGroupB.MuDuSetup);		// ��������� �������� ���������
		p->Express.List12.Buf[1] = 0;		// ��������� �������� ���������
		p->Express.List12.Buf[2] = 0;		// ��������� �������� ���������
		p->Express.List12.Buf[3] = 0;		// ��������� �������� ���������
		p->Express.List12.Buf[4] = 0;		// ��������� �������� ���������

		p->Groups 		= groups;
		p->SubGroups 	= Null;
		p->Params 		= params;
		p->Values 		= values;
		p->AtsParams 	= addr_params_altistar48;

		strcpy(p->Rsvd.Str, "������");

		p->StartDispl = StartDispl;
		p->EnableEdit = EnableEdit;
		p->WriteValue = WriteValue;
		/*p->GetExpressText = GetExpressText;
		p->GetExpressState = GetExpressState;*/

	// ������� ������
		g_Ram.ramGroupH.StartIndic = 8;
#else
	p->Data = ToPtr(&g_Ram);      	// ��������� �� ����� ������
	p->Group.Count = GROUP_COUNT;    // ���������� �����
	p->Params = params;   			// ��������� �� ��������� ����������
	p->AtsParams = addr_params_altistar48;   			// ��������� �� ��������� ����������
	p->Values = values;   			// ��������� �� ��������� ��������� ��������
	p->EnableEdit = EnableEdit;
	p->WriteValue = WriteValue;
#endif
	// ������ ���� ���������� �� Eeprom
	ReadWriteAllParams(F_READ,p);
	//RefreshParams(REG_DRIVE_TYPE);
	//RefreshParams(REG_SIN_FILTER_TF);
	//RefreshParams(REG_RMS_FILTER_TF);
	// ��������� �������� �� ModBus
	SetModBusParams();
	// ����� ������
	//g_Ram.ramGroupA.VER_PO = SVN_REVISION/100;
	//g_Ram.ramGroupA.SUBVER_PO = SVN_REVISION;
	//while (g_Ram.ramGroupA.SUBVER_PO >= 100) g_Ram.ramGroupA.SUBVER_PO -= 100;
	p->MinMaxGain = &g_Ram.ramGroupC.MaxTorque;

}
//---------------------------------------------------
#if NEW_RAZ

void Core_MenuUpdate(TCoreMenu *p)
{
	if (SleepMode(p))
		return;

	if (!p->EvLogFlag)
	{
		switch (p->State)
		{
		   case MS_START:	StartState(p);  break;
		   case MS_SELGR:   SelectGroup(p); break;
		   case MS_SELPAR:  SelectParam(p); break;
		   case MS_EDITPAR: EditParam(p);   break;
		  // case MS_EXPRESS: ExpressState(p); break;
		}
	}

	p->Key = 0;
}

void Core_MENU_Display(TCoreMenu *p)
{
	if (!p->Indication) {p->Update = TRUE; return;}

	if (p->EvLogFlag) return;

	if (p->Update)
	{
		memset(p->HiString, ' ', 16);
		memset(p->LoString, ' ', 16);
	}

	if (!p->SleepActive)
	{
		switch (p->State)
		{
		   case MS_START:   ShowStart(p);        break;
		   case MS_SELGR:   ShowGroup(p);        break;
		   case MS_SELPAR:  ShowParam(p, FALSE); break;
		   case MS_EDITPAR: ShowParam(p, TRUE);  break;
		  // case MS_EXPRESS: ShowExpress(p);   	 break;
		}
	}
	p->Update = FALSE;
}

__inline Bool SleepMode(TCoreMenu *p)
{
	if (!p->Key)
	{
		if (!p->SleepActive)
		{
			if (!p->SleepTime) p->SleepTimer = 0;
			else if (p->SleepTimer < p->SleepTime) p->SleepTimer++;
			else {p->SleepActive = TRUE; p->Update = TRUE;}
		}
	}
	else
	{
		p->SleepTimer = 0;
		if (p->SleepActive)
		{
			p->State = MS_START;
			p->SleepActive = FALSE;
			p->Update = TRUE;
			p->Key = 0;
		}
	}
	return p->SleepActive;
}

__inline void StartState(TCoreMenu *p)
{
	if (p->Express.Enable)
	{
		p->State = MS_EXPRESS;
		return;
	}

	if (p->Key == KEY_ENTER)
	{
		p->State = MS_SELGR;
		p->Level = 1;
		p->EvLogSelected = FALSE;
		p->Update = TRUE;
		p->Group.Position  = p->Group.Count - 1;
		ChangeCoordinate(p, KEY_UP, TRUE);
	}
}

__inline void SelectGroup(TCoreMenu *p)
{
	switch (p->Key)
	{
	   case KEY_UP:
	   case KEY_DOWN:
		   ChangeCoordinate(p, p->Key, TRUE);
		   p->EvLogFlag = p->EvLogSelected;
		   break;
	   case KEY_ESCAPE:
		   if (p->EvLogSelected) p->EvLogSelected = FALSE;
		   else if (p->Level == 2) p->Level = 1;
		   else p->State = MS_START;
		   break;
	   case KEY_ENTER:
		   if ((p->Group.Position == p->EvLogGroup) && !p->EvLogSelected)
		   {
			   p->EvLogSelected = TRUE;
			   if (p->Data[p->EvLogAdress] > 0) p->EvLog.Position = p->Data[p->EvLogAdress];
			   else p->EvLog.Position = *p->EvLogCount;	//!!!
//			   else p->EvLog.Position = p->EvLog.Count;	//!!!

			   p->EvLogFlag = TRUE;
				ChangeCoordinate(p, KEY_DOWN, TRUE);
			   break;
		   }

		   if ((p->Level == 1) && (p->MaxLevel == 2))
		   {
			   p->Level = 2;
			   p->SubGroup.Position = p->SubGroup.Count - 1;
			   ChangeCoordinate(p, KEY_UP, TRUE);
		   }
		   else
		   {
			   p->Param.Position = p->Param.Count - 1;
			   ChangeCoordinate(p, KEY_UP, FALSE);
			   p->State = MS_SELPAR;
		   }
		   break;
	   default:
		   return;
	}

	p->Update = TRUE;
}

__inline void SelectParam(TCoreMenu *p)
{
	struct MENU_VAL_CFG *Val = &p->Dcr.Config.Val;

	switch (p->Key)
	{
	   case KEY_UP:
	   case KEY_DOWN:
		   ChangeCoordinate(p, p->Key, FALSE);
		   break;
	   case KEY_ESCAPE:
		   p->State = MS_SELGR;
		   break;
	   case KEY_ENTER:
		   if (Val->ReadOnly) return;
		   if (!p->EnableEdit(Val->PaswwPrt)) return;
		   p->Rate  = 0;
		   p->Blink = FALSE;
		   p->State = MS_EDITPAR;
		   break;
	   default:
		   return;
	}

	p->Update = TRUE;
}

__inline void EditParam(TCoreMenu *p)
{
	struct MENU_VAL_CFG *Val = &p->Dcr.Config.Val;

	switch (p->Key)
	{
	   case KEY_UP:
	   case KEY_DOWN:
		   switch (Val->Type)
		   {
		      case MT_DEC:  EditDec (p, p->Key); break;
		      case MT_STR:  EditStr (p, p->Key); break;
		      case MT_TIME: EditTime(p, p->Key); break;
		      case MT_DATE: EditDate(p, p->Key); break;
		      case MT_BIN:  EditBin (p);         break;
		   }
		   break;
	   case KEY_LEFT:   if (p->Rate < p->EditRate) p->Rate++; else p->Rate = 0; break;
	   case KEY_RIGHT:  if (p->Rate > 0) p->Rate--; else p->Rate = p->EditRate; break;
	   case KEY_ESCAPE: p->State = MS_SELPAR; break;
	   case KEY_ENTER:
		   if (!CheckRange(p->Value, p->Dcr.Min, p->Dcr.Max)) break;
		   if (p->State == MS_EXPRESS)
			{
				if (!p->WriteValue(Val->Memory, p->Express.List->Buf[p->Express.Index], &p->Value)) break;
			}
		   else
		   {
		   		if (!p->WriteValue(Val->Memory, p->Param.Position + p->Param.Addr, &p->Value)) break;
		   }
		   p->State = MS_SELPAR;
		   break;
	}
}
/*
__inline void ExpressState(TCoreMenu *p)
{
	Byte NextState;

	p->Express.List = 0;

	if (p->Express.Enable)						// ����� ��������� � ������ ������ �������� ���������
	{
		switch(p->Key)
		{
			case KEY_LEFT:
			case KEY_RIGHT:
				p->Express.Select = !p->Express.Select;			// �������� "��" ��� "���"
				break;
			case KEY_ENTER:
				if (!p->Express.Select) p->State = MS_START;	// ���� ������� ���, �� ������������ � ��������� ��������� ����
				p->Express.State = 0;
				p->Express.Index = 0;
				p->Update = TRUE;								// ���������� ���� ��� ���������� �������� ���������
				p->Express.Enable = FALSE;						// ��������� ��������� � ������ ������ �������� ���������
				p->Express.First = true;
		}
		return;
	}

	if (p->Update) return;										// ���� ����������� �������� ���������, �� �������

	switch(p->Express.State)									// ���� �������� ���������
	{
		case 0: p->Express.List = &p->Express.List1; NextState = 1;		break;
		// ������ ������ ����������
		case 1: NextState = 2;											break;
		case 2: p->Express.List = &p->Express.List2; NextState = 3;		break;
		// ��� ����������
		case 3: if (p->Express.Select) NextState = 4;	// ������
				else				   NextState = 8;	// ����������� ��������
																		break;
//-------------------------------------------------------------------------------
		// ���������� � ��������� �������
		case 4: NextState = 5;											break;
		// ������� �������
		case 5: p->Express.List = &p->Express.List3;	NextState = 6;	break;
		// ���������� � ��������� �������
		case 6: NextState = 7;											break;
		// ������� �������
		case 7: p->Express.List = &p->Express.List4;	NextState = 12;	break;
//-------------------------------------------------------------------------------
		// ���������� � ������� ���������
		case 8: NextState = 9;	break;
		// ��������� ������� ��� �������
		case 9: if (p->Express.Select)	NextState = 10;
				else					NextState = 11;
																					break;
		// ������� �� ��������
		case 10: p->Express.List = &p->Express.List5;	NextState = 12;				break;
		// ������� �� ��������
		case 11: p->Express.List = &p->Express.List6;	NextState = 12;				break;
//-------------------------------------------------------------------------------
		// ��������� ����������?
		case 12: if (p->Express.Select)	NextState = 13;			// ����������� ����
				 else					NextState = 27;			// ���������� ����������� ����
				 break;
		// ��������� ������� ���������: ������� ��� �������. �� ������ ����� �������� ���������
		case 13: NextState = GetExpressState(NextState, 0); p->Key = KEY_ENTER;			break;
	//---------------------------------------------------------------------------
		// ������� � �������
		case 14: NextState = GetExpressState(NextState, 1);								break;
		// ������� ��������� � ��������
		case 15: NextState = GetExpressState(NextState, 2);								break;
		// ������������ �� "����". ���������� �������� "����" ��� �������� � ���������� ���������
		case 16: NextState = 17;	p->Key = KEY_ENTER; 								break;
		// �������� ����������?
		case 17: if (p->Express.Select)		NextState = 18;		// ��. ����� ����������
				 else						NextState = 14;		// ���. ���������� ���������
				 break;
		// ����� ���������� � ������� � ������ ���� ����������
		case 18: NextState = GetExpressState(NextState, 3);
				 p->Key = KEY_ENTER;	//	���������� �������� "����" ��� �������� � ���������� ���������
				 break;
		// ������� �� "�������"
		case 19: NextState = GetExpressState(NextState, 4);
				 p->Key = KEY_ENTER;	//	���������� �������� "����" ��� �������� � ���������� ���������
				 break;
	//---------------------------------------------------------------------------
		// ������� � �������
		case 20: NextState = GetExpressState(NextState, 5);								break;
		// ������� ��������� � ��������
		case 21: NextState = GetExpressState(NextState, 6);								break;
		// ������������ �� "����". ���������� �������� "����" ��� �������� � ���������� ���������
		case 22: NextState = 23;	p->Key = KEY_ENTER; 								break;
		// �������� ����������?
		case 23: if (p->Express.Select)		NextState = 24;		// ��. ����� ����������
				 else						NextState = 20;		// ���. ���������� ���������
				 break;
		// ����� ���������� � ������� � ������ ���� ����������
		case 24: NextState = GetExpressState(NextState, 3);
				 p->Key = KEY_ENTER;	//	���������� �������� "����" ��� �������� � ���������� ���������
				 break;
		// ������� �� "�������"
		case 25: NextState = GetExpressState(NextState, 7);
				 p->Key = KEY_ENTER;	//	���������� �������� "����" ��� �������� � ���������� ���������
				 break;
		// ��� ���������� ����������?
		case 26: if (p->Express.Select)		NextState = 27;		// ��. ��������� �� ��������� ���
				 else						NextState = 18;		// ���. ���������� ����������
				 break;
//-------------------------------------------------------------------------------
		// ������ ������� �������
		case 27: NextState = 28;												break;
		// ������ �������
		case 28: p->Express.List = &p->Express.List2;
				 NextState = GetExpressState(NextState, 8);			// ��������� � ����������� �� ���� ��������
				 break;
		// ������ ���������� ������ ��� ��������
		case 29: p->Express.List = &p->Express.List7;	NextState = 30;			break;
		// ������ ������
		case 30: p->Express.List = &p->Express.List8;
				 NextState = GetExpressState(NextState, 9);			// ��������� � ����������� �� ���� ��������
				 break;
		// ������ ���������� ������ ��� ��������
		case 31: p->Express.List = &p->Express.List9;	NextState = 32;			break;
		// ������ ������
		case 32: p->Express.List = &p->Express.List10;
				 NextState = GetExpressState(NextState, 10);			// ��������� � ����������� �� ���� ����������� ���
				 break;
		// �������� ��� �������� ������� ������ ��� ���-�
		case 33: p->Express.List = &p->Express.List11;	NextState = 34;			break;
		// ����� ������ ��/��
		case 34: p->Express.List = &p->Express.List12;			break;
	}

	// ������� ������� ���������
	if ((NextState == 15) || (NextState == 16) ||  (NextState == 19) ||
			(NextState == 21) || (NextState == 22) || (NextState == 25))
		{
			p->Express.State = NextState;
			return;
		}

	if (p->Express.List) {EditParam(p); p->State = MS_EXPRESS;}			// ���� ������ List � �������� ����������, �� �������� ������� �������������� ���������,
																		// ����� ������� �������� ��������� ���� MS_EXPRESS, �.�. ����� �������������� ��� ������������ ������


	switch (p->Key)
	{
		case KEY_LEFT:
		case KEY_RIGHT:
						if (!p->Express.List)
							p->Express.Select = !p->Express.Select;			// �������� "��" ��� "���"
						break;
		case KEY_ENTER:
						if (p->Express.List)
						{
							ExpNextStateRdy = true;					// ���������� ����, ��� ������ ���������� � ���������� ��������� ��� ���������
						}
						else
						{
							if (p->Express.State < EXPRESS_MAX_STATE) p->Express.State = NextState;			// 3 - ������������ �����
							else {p->Express.State = 0; p->State = MS_START;}
						}
						p->Express.First = true;
						p->Update = TRUE;
						break;
		// ����� �� ��������-���������
		case KEY_ESCAPE:
						p->Express.State = 0;
						p->State = MS_START;
						p->Update = true;
						break;
		}


	if (ExpNextStateRdy)
	{
		if (IsMemParReady())		// ���������� ���������, ����� ������� ��������, ������, ��� ���������� � �������
		{
			ExpNextStateRdy = false;
			p->Update = true;

			if (++p->Express.Index >= p->Express.List->Cnt)
			{
				if (p->Express.State < EXPRESS_MAX_STATE) p->Express.State = NextState;
				else {p->Express.State = 0; p->State = MS_START;}
				p->Express.Index = 0;
			}
		}
	}
}*/

__inline void EditDec(TCoreMenu *p, Char Key)
{
	struct MENU_VAL_CFG *Val = &p->Dcr.Config.Val;
	Char  i, *pBuffer, SignChange;
	Uns   Step;
	LgInt Data;

	Step = 1;
	for (i=0; i < p->Rate; i++) Step = Step * 10;

	pBuffer = &p->BufValue[5 - p->Rate];
	SignChange = (Val->Signed && (p->Rate == p->EditRate));

	switch (Key)
	{
	   case KEY_UP:
		   if (!p->EditType)
		   {
			   if (Val->Signed)
			   {
				   Data = (LgInt)((Int)p->Value) + (LgInt)Step;
				   if (Data > (LgInt)((Int)p->Dcr.Max)) p->Value = p->Dcr.Max;
				   else p->Value = p->Value + Step;
			   }
			   else
			   {
				   if (Step > (p->Dcr.Max - p->Value)) p->Value = p->Dcr.Max;
				   else p->Value = p->Value + Step;
			   }
			   return;
		   }
		   else if (!SignChange)
		   {
			   if (*pBuffer < '9') *pBuffer = *pBuffer + 1;
			   else *pBuffer = '0';
		   }
		   break;
	   case KEY_DOWN:
		   if (!p->EditType)
		   {
			   if (Val->Signed)
			   {
				   Data = (LgInt)((Int)p->Value) - (LgInt)Step;
				   if (Data < (LgInt)((Int)p->Dcr.Min)) p->Value = p->Dcr.Min;
				   else p->Value = p->Value - Step;
			   }
			   else
			   {
				   if (Step > (p->Value - p->Dcr.Min)) p->Value = p->Dcr.Min;
				   else p->Value = p->Value - Step;
			   }
			   return;
		   }
		   else if (!SignChange)
		   {
			   if (*pBuffer > '0') *pBuffer = *pBuffer - 1;
			   else *pBuffer = '9';
		   }
		   break;
	   default:
		   return;
	}

	pBuffer = &p->BufValue[4 - Val->RateMax];
	if (SignChange)
	{
		if (*pBuffer == '-') *pBuffer = '+';
		else *pBuffer = '-';
	}
	p->Value = StrToDec(pBuffer + 1, Val->RateMax);
	if (*pBuffer == '-') p->Value = -p->Value;
}

__inline void EditStr(TCoreMenu *p, Char Key)
{
	Uns  Value, Count, RsvdLen, MaxLen;
	Char Tmp, *BufAddr, *BufPtr;

	Value   = p->Value;
	Count   = p->Dcr.Max - p->Dcr.Min + 1;
	RsvdLen = strlen(p->Rsvd.Str);
	MaxLen  = 16 - RsvdLen;

	while(1)
	{
		if (Count > 0) Count--; else break;

		switch (Key)
		{
	   	case KEY_UP:   if (Value < p->Dcr.Max) Value++; else Value = p->Dcr.Min; break;
		   case KEY_DOWN: if (Value > p->Dcr.Min) Value--; else Value = p->Dcr.Max; break;
		}

		PFUNC_blkRead((Int *)p->Values[p->Dcr.Config.Str.Addr + Value].Str, (Int *)p->BufTmp, 16);
		if (p->ShowReserved) break;

		BufAddr = p->BufTmp;
		BufAddr[16] = '\0';
		while (*BufAddr) {if (*BufAddr != ' ') break; BufAddr++;}
		if ((Uns)(BufAddr - p->BufTmp) > MaxLen) break;

		BufPtr = &BufAddr[RsvdLen];
		Tmp = *BufPtr; *BufPtr = '\0';
		if (strcmp(p->Rsvd.Str, BufAddr)) {*BufPtr = Tmp; break;}
	}

	p->Value = Value;
}

__inline void EditTime(TCoreMenu *p, Char Key)
{
	TTimeVar *Time = (TTimeVar *)&p->Value;

	switch (Key)
	{
	   case KEY_UP:
		   switch (p->Rate)
		   {
		      case 0: if (Time->bit.Min  < 59) Time->bit.Min++;  else Time->bit.Min  = 0;  break;
		      case 1: if (Time->bit.Hour < 23) Time->bit.Hour++; else Time->bit.Hour = 0;  break;
		   }
		   break;
	   case KEY_DOWN:
		   switch (p->Rate)
		   {
		      case 0: if (Time->bit.Min  > 0)  Time->bit.Min--;  else Time->bit.Min  = 59; break;
		      case 1: if (Time->bit.Hour > 0)  Time->bit.Hour--; else Time->bit.Hour = 23; break;
		   }
		   break;
	}
}

__inline void EditDate(TCoreMenu *p, Char Key)
{
	TDateVar *Date = (TDateVar *)&p->Value;

	switch (Key)
	{
	   case KEY_UP:
		   switch (p->Rate)
		   {
		      case 0: if (Date->bit.Year  < 99) Date->bit.Year++;  else Date->bit.Year  = 0; break;
		      case 1: if (Date->bit.Month < 12) Date->bit.Month++; else Date->bit.Month = 1; break;
		      case 2:
			      if (Date->bit.Day < RTC_DefMaxDay(Date->bit.Year, Date->bit.Month)) Date->bit.Day++;
			      else Date->bit.Day = 1;
			      break;
		   }
		   break;
	   case KEY_DOWN:
		   switch (p->Rate)
		   {
		      case 0: if (Date->bit.Year  > 0) Date->bit.Year--;  else Date->bit.Year  = 99; break;
		      case 1: if (Date->bit.Month > 1) Date->bit.Month--; else Date->bit.Month = 12; break;
		      case 2:
			      if (Date->bit.Day > 1) Date->bit.Day--;
			      else Date->bit.Day = RTC_DefMaxDay(Date->bit.Year, Date->bit.Month);
			      break;
		   }
		   break;
	}
}

__inline void EditBin(TCoreMenu *p)
{
	Uns Step = 1U << p->Rate;

	if (p->Value & Step) p->Value &= ~Step;
	else p->Value |= Step;
}

static void ChangeCoordinate(TCoreMenu *p, Char Key, Bool GroupFlag)
{
	struct MENU_CRD *Crd;
	Uns Count, RsvdLen, MaxLen;
	Char *Addr, *GrAddr, Name[17];

	if (GroupFlag)
	{
		if (p->EvLogSelected) {Crd = &p->EvLog; Addr = (Char *)p->Groups;}
		else if (p->Level == 1) {Crd = &p->Group; Addr = (Char *)p->Groups;}
		else {Crd = &p->SubGroup; Addr = (Char *)p->SubGroups;}
	}
	else
	{
		Crd = &p->Param;
		Count = p->Param.Count;
		RsvdLen = strlen(p->Rsvd.Str);
		MaxLen = 16 - RsvdLen;
	}

	while(1)
	{
		switch (Key)
		{
		   case KEY_UP:
			   if (Crd->Position < Crd->Count-1) Crd->Position++;
			   else Crd->Position = 0;
			   break;
		   case KEY_DOWN:
			   if (Crd->Position > 0) Crd->Position--;
			   else Crd->Position = Crd->Count-1;
			   break;
		}

		if (GroupFlag)
		{
			if (p->EvLogSelected) GrAddr = Addr + p->Group.Position * sizeof(struct MENU_GROUP);
			else GrAddr = Addr + Crd->Position * sizeof(struct MENU_GROUP);
			Count = PFUNC_wordRead((Int *)&((struct MENU_GROUP *)GrAddr)->Count);
			if (Count || p->ShowReserved) break;
		}
		else
		{
			if (p->ShowReserved) break;
			if (Count > 0) Count--; else break;
			PFUNC_blkRead((Int *)p->Params[Crd->Position + Crd->Addr].Name, (Int *)Name, 16);
			Addr = strchr(Name, (Int)'.');
			if (!Addr) break;
			Addr++;
			if ((Int)(Addr - Name) > MaxLen) break;
			Addr[RsvdLen] = '\0';
			if (strcmp(p->Rsvd.Str, Addr)) break;
			continue;
		}
	}

	if (GroupFlag)
	{
		Crd = (p->Level == 2) ? &p->SubGroup : &p->Param;
		Crd->Addr  = PFUNC_wordRead((Int *)&((struct MENU_GROUP *)GrAddr)->Addr);
		Crd->Count = Count;
	}
	else ReadDcrFull(p, p->Param.Position + p->Param.Addr);
}

__inline void ShowStart(TCoreMenu *p)
{
#if !IR_IMP_TEST
	Uns Position;
#endif

	memset(p->HiString, ' ', 16);

#if !IR_IMP_TEST
	Position = p->Data[p->StartAddr] + p->StartOffset;
	if (p->Update) ReadDcrFull(p, Position);
	ShowValue(p, Position, FALSE);
#endif

	p->StartDispl(p->HiString);
}

static void ShowGroup(TCoreMenu *p)
{
	const struct MENU_GROUP *GroupTbl;
	Char *Str;

	if (p->Update)
	{
		if (p->EvLogSelected)
		{
			PFUNC_blkRead((Int *)p->Values[p->EvLog.Addr].Str, (Int *)p->HiString, 16);
			Str = &p->HiString[15]; while (*(Str-6) == ' ') Str--;
			DecToStr(p->EvLog.Position + 1, Str, 0, 4, FALSE, FALSE);
			TimeToStr((TTimeVar *)&p->Data[p->EvLogTime], (String)&p->LoString[1]);
			DateToStr((TDateVar *)&p->Data[p->EvLogDate], (String)&p->LoString[7]);
		}
		else
		{
			if (p->Level == 1) GroupTbl = &p->Groups[p->Group.Position];
			if (p->Level == 2) GroupTbl = &p->SubGroups[p->SubGroup.Position + p->SubGroup.Addr];
			PFUNC_blkRead((Int *)GroupTbl->HiString, (Int *)p->HiString, 16);
			PFUNC_blkRead((Int *)GroupTbl->LoString, (Int *)p->LoString, 16);

			if (p->Group.Position == 2)
			  {
			    if (IsPassword2()) menu.HiString[15] = CODEON_ICO;
			    else menu.HiString[15] = CODEOFF_ICO;
			  }
                        if (p->Group.Position == 1)
                          {
                            if (IsPassword1()) menu.HiString[15] = CODEON_ICO;
                            else menu.HiString[15] = CODEOFF_ICO;
                          }

                        if (p->Group.Position == 3)
                          {
                            if (IsPassword1()) menu.HiString[15] = CODEON_ICO;
                            else menu.HiString[15] = CODEOFF_ICO;
                          }
		}
	}
}

__inline Byte DefineBit(TCoreMenu *p, Uns Value)
{
	if (!Value) {p->Bit = 0xFF; return 0;}

	if (p->Bit == 0xFF) p->BitTimer = 0;

	if (p->BitTimer > 0) p->BitTimer--;
	else
	{
		do {if (++p->Bit >= 15) p->Bit = 0;}
		while (!(Value & (1 << p->Bit)));
		p->BitTimer = p->BitTime;
	}

	return (p->Bit + 1);
}

static void ShowValue(TCoreMenu *p, Uns Position, Bool Edit)
{
	struct MENU_VAL_CFG *Val = &p->Dcr.Config.Val;
	Char i, *Str;
	Uns  Addr, Timeout;

	if (Edit)
	{
		Timeout = !p->Blink ? p->BlinkHiTime : p->BlinkLoTime;
		if (++p->BlinkTimer >= Timeout)
		{
			p->Blink = !p->Blink;
			p->BlinkTimer = 0;
		}
	}
	else
	{
//----- �������� ��������� -------
		if ((p->State == MS_EXPRESS) && ((p->Express.State == 5) || (p->Express.State == 7)))
				{	p->Value = 1;	}
		else	p->Value = p->Data[Position];
//-----------------
		p->Blink = FALSE;
		p->BlinkTimer = 0;
	}

	memset(p->LoString, ' ', 16);

	switch (Val->Type)
	{
	   case MT_VERS:
		   Str = &p->LoString[10];

		   if (!Edit)
		   {
			   DecToStr(p->Value, Str, Val->Precision, Val->RateMax, TRUE, Val->Signed);
				p->LoString[4] = p->LoString[5];
				p->LoString[5] = p->LoString[6];
				p->LoString[6] = '.';
		   }

	   break;
	   case MT_DEC:
		   Str = &p->LoString[8];

		   if (!p->EditType)
			   DecToStr(p->Value, Str, Val->Precision, Val->RateMax, !Edit, Val->Signed);
		   else if (!Edit)
		   {
			   DecToStr(p->Value, Str, Val->Precision, Val->RateMax, TRUE, Val->Signed);
			   if (Val->Signed) FloatPositive = '+';
			   DecToStr(p->Value, &p->BufValue[5], 0, Val->RateMax, FALSE, Val->Signed);
			   FloatPositive = ' ';
		   }
		   else for (i=0; i <= p->EditRate; i++)
        {
				if (Val->Precision && (i == Val->Precision)) *Str-- = FloatSeparator;
				*Str-- = p->BufValue[5 - i];
		   }

		   if (p->Blink)
		   {
			   Str = &p->LoString[8 - p->Rate];
			   if (Val->Precision && (p->Rate >= Val->Precision)) Str--;
			   *Str = ' ';
		   }

		   if (p->Update) PFUNC_blkRead((Int *)p->Params[Position].Unit, (Int *)p->BufTmp, 4);
		   else memcpy(&p->LoString[10], p->BufTmp, 4);
		   break;

	   case MT_STR:
		   if (p->Blink) break;
			if (!Edit)
			{
				Addr = p->Dcr.Config.Str.Addr + p->Value;
				PFUNC_blkRead((Int *)p->Values[Addr].Str, (Int *)p->BufTmp, 16);
			}
			memcpy(p->LoString, p->BufTmp, 16);
		   break;

	   case MT_RUN:
		   if (p->Blink) break;
		   Addr = p->Dcr.Config.Str.Addr + DefineBit(p, p->Value);
		   PFUNC_blkRead((Int *)p->Values[Addr].Str, (Int *)p->LoString, 16);
		   break;

	   case MT_TIME:
		   TimeToStr((TTimeVar *)&p->Value, (String)&p->LoString[5]);
		   if (p->Blink)
		   {
			   switch (p->Rate)
			   {
			      case 0: p->LoString[8] = ' '; p->LoString[9] = ' '; break;
			      case 1: p->LoString[5] = ' '; p->LoString[6] = ' '; break;
			   }
		   }
		   break;

	   case MT_DATE:
		   DateToStr((TDateVar *)&p->Value, (String)&p->LoString[4]);
		   if (p->Blink)
		   {
			   switch (p->Rate)
			   {
			      case 0: p->LoString[10] = ' '; p->LoString[11] = ' '; break;
			      case 1: p->LoString[7]  = ' '; p->LoString[8]  = ' '; break;
			      case 2: p->LoString[4]  = ' '; p->LoString[5]  = ' '; break;
			   }
		   }
		   break;

	   case MT_BIN:
		   Str = &p->LoString[9 + Val->RateMax / 2];
		   for (i=0; i <= (Char)Val->RateMax; i++)
		   {
			   Str--;
			   if (p->Blink && (i == p->Rate)) continue;
			   *Str = (p->Value & (1 << i)) ? '1' : '0';
		   }
		   break;
	}
}

static void ShowParam(TCoreMenu *p, Bool Edit)
{
	Uns Position = p->Param.Position + p->Param.Addr;

	if (p->Update) PFUNC_blkRead((Int *)p->Params[Position].Name, (Int *)p->HiString, 16);

	ShowValue(p, Position, Edit);
}
/*
__inline void ShowExpress(TCoreMenu *p)
{
	Uns Addr;

	p->Express.List = 0;

	if (p->Express.Enable)							// �������� ��������� � ������ �������� ���������
	{
		p->GetExpressText(0);						// ������� ������ ���������
		ShowExpressChoice(p, 3,6,7,11, false);		// ������� �������� �������� ������
		return;
	}

	if (ExpNextStateRdy)	return;

	switch(p->Express.State)							// ���� �������� ���������
	{
		case 0: p->Express.List = &p->Express.List1; 								break;
		case 1:	p->GetExpressText(1);	ShowExpressChoice(p, 11,14, 0, 0, true);	break;
		case 2: p->Express.List = &p->Express.List2;								break;
		case 3:	p->GetExpressText(2);	ShowExpressChoice(p,   0, 5, 6,15, true);	break;
		// ���������� � ��������� �������
		case 4: p->GetExpressText(3);	ShowExpressChoice(p,  10,13, 0, 0, true);	break;
		// ������� �������
		case 5: p->Express.List = &p->Express.List3;								break;
		// ���������� � ��������� �������
		case 6: p->GetExpressText(4);	ShowExpressChoice(p,  10, 13, 0, 0, true);	break;
		// ������� �������
		case 7: p->Express.List = &p->Express.List4;								break;
		// ���������� � ������� ���������
		case 8: p->GetExpressText(5);	ShowExpressChoice(p,  11, 14, 0, 0, true);	break;
		// ��������� ������� ��� �������
		case 9: p->GetExpressText(6);	ShowExpressChoice(p,   2,  7, 8,13, true);	break;
		// ������� �� ��������
		case 10: p->Express.List = &p->Express.List5;								break;
		// ������� �� ��������
		case 11: p->Express.List = &p->Express.List6;								break;
		// ��������� ����������
		case 12: p->GetExpressText(7);	ShowExpressChoice(p,   3,  6, 7,11, true);	break;
		// ��������� ������� ���������: ������� ��� �������
		case 13:																	break;
		// ������� � �������
		case 14: p->GetExpressText(8);												break;
		// ������� ��������� � ��������
		case 15: p->GetExpressText(9);												break;
		// ������������ �� "����". ���������� �������� "����" ��� �������� � ���������� ���������
		case 16: 																	break;
		// �������� ����������?
		case 17: p->GetExpressText(10);	ShowExpressChoice(p,   3,  6, 7,11, false);	break;
		// ����� ���������� � ������� � ������ ���� ����������
		case 18:																	break;
		// ������� �� "�������"
		case 19:																	break;
	//---------------------------------------------------------------------------
		// ������� � �������
		case 20: p->GetExpressText(11);												break;
		// ������� ��������� � ��������
		case 21: p->GetExpressText(9);												break;
		// ������������ �� "����". ���������� �������� "����" ��� �������� � ���������� ���������
		case 22: 																	break;
		// �������� ����������?
		case 23: p->GetExpressText(10);	ShowExpressChoice(p,   3,  6, 7,11, true);	break;
		// ����� ���������� � ������� � ������ ���� ����������
		case 24:																	break;
		// ������� �� "�������"
		case 25:																	break;
		// ��� ���������� ����������?
		case 26: p->GetExpressText(12);	ShowExpressChoice(p,   3,  6, 7,11, true);	break;
		// ������ ������� �������
		case 27: p->GetExpressText(13);	ShowExpressChoice(p,   11,14, 0, 0, true);	break;
		// ������ �������
		case 28: p->Express.List = &p->Express.List2;				 				break;
		// ������ ���������� ������ ��� ��������
		case 29: p->Express.List = &p->Express.List7;								break;
		// ������ ������
		case 30: p->Express.List = &p->Express.List8;								break;
		// ������ ���������� ������ ��� ��������
		case 31: p->Express.List = &p->Express.List9;								break;
		// ������ ������
		case 32: p->Express.List = &p->Express.List10;							 	break;
		// �������� ��� �������� ������� ������ ��� ���-�
		case 33: p->Express.List = &p->Express.List11;								break;
		// ����� ������ ��/��
		case 34: p->Express.List = &p->Express.List12;								break;
	}


	if (p->Express.List)																	// ���� ������ List � �������� ����������
	{
		Addr = p->Express.List->Buf[p->Express.Index];										// ��������� ����� �������� ���������
		PFUNC_blkRead((Int *)p->Params[Addr].Name, (Int *)p->HiString, 16);		// ��������� �������� �������� ���������
		if (p->Update)
		{
			ReadDcrFull(p, Addr);									// ����� ���� update, �� ��������� �������� ���������
			if ((p->Express.State == 5) || (p->Express.State == 7))
				{	p->Dcr.Min = 1;	p->Dcr.Max = 1;	}
		}
		if (p->Express.First)												// ������� �������� ��������� �� �������
			{
				p->Rate = 0;
				ShowValue(p, Addr, FALSE);	p->Express.First = false;
			}
		else	ShowValue(p, Addr, TRUE);									// ������� �������������� �� �������
	}
}



__inline void ShowExpressChoice(TCoreMenu *p, Byte YesOne, Byte YesTwo,
									Byte NoOne, Byte NoTwo, Bool Yes)
{
	Uns Timeout;

	Timeout = !p->Blink ? p->BlinkHiTime : p->BlinkLoTime;
	if (++p->BlinkTimer >= Timeout)
	{
		p->Blink = !p->Blink;
		p->BlinkTimer = 0;
	}

	if (p->Blink)	return;

	if (p->Express.First)
	{
		p->Express.Select = Yes;
		p->Express.First = false;
	}

	if (p->Express.Select)				// ������� ��������, ������������ ������� ����� �� ����������
	{
		p->LoString[YesOne] = '[';
		p->LoString[YesTwo] = ']';
	}
	else
	{
		if ((!NoOne) && (!NoTwo))
		{	p->Express.Select = true;	return;	}

			p->LoString[NoOne] = '[';
			p->LoString[NoTwo] = ']';
	}
}*/

//---------------------------------------------------
// ���������� ����������, ������� ������������ �� ������� ������ ����
void StartDispl(String Str)
{
    Uns displPosition = 0;
    displPosition = !g_Ram.ramGroupA.Faults.Dev.bit.PosSens ? g_Ram.ramGroupA.PositionPr : 9999; // ���� "���� ������� ���������", ������� 999.9
    DecToStr(displPosition, &menu.HiString[8], 1, 4, True, True);
    menu.HiString[9] = '%';

	if (IsPassword1())
		menu.HiString[11] = CODE_ICO;
	if (g_Comm.mbAsu.Stat.Status.bit.NoConnect == 1)
		menu.HiString[12] = CONN_ICO;
	if (g_Comm.Bluetooth.IsConnected)
	{
		menu.HiString[14] = BT_ICO;
		if (g_Comm.Bluetooth.ModeProtocol == 1 && menu.State == MS_START) menu.HiString[15] = '�';
		if (g_Comm.Bluetooth.ModeProtocol == 2 && menu.State == MS_START) menu.HiString[15] = '�';
	}
}

#endif
//------------------------------------------------------------------
//TODO ������������� ��� ��������� �������
Bool UpdateCode(Uns addrPassw, Uns addrCode, Uns value, Uns def)	// � WriteValue()
{
	Uns *password = ToUnsPtr(&g_Ram) + addrPassw;
	Uns *code = ToUnsPtr(&g_Ram) + addrCode;
	Bool writeFlag = false;

	if (!IsMemParReady()) return false;
	if (*password != 0)
	{
		if ((value == *password) || (value == def))
		{
			*code = 0; *password = 0;
			//writeFlag = true;
		}
	}
	else if (value != 0)
	{
		*password = value; *code = 0;
		writeFlag = true;
	}
	else if (value == 0)
	{
		*password = value; *code = 0;
		writeFlag = true;
	}
	
	if (writeFlag)
	{
		if (ReadWriteEeprom(&Eeprom1,F_WRITE,addrPassw,password,1))
		{
			return true;
		}
	}

	return true;
}
//---------------------------------------------------
// ������ �������� ���������
void ReadDcrFull(TCoreMenu *p, Uns Addr)	// � ChangeCoordinate()
{
	struct MENU_VAL_CFG *Val = &p->Dcr.Config.Val;
	
	Core_MenuReadDcr(p, &p->Dcr, Addr);

	p->Bit = 0xFF;

	// ��� ��������� �������� ����������� ��������
	p->EditRate = Val->RateMax;
	if (p->EditType && (Val->Type == MT_DEC) && Val->Signed) p->EditRate++;
}
//---------------------------------------------------
// ������ � ��������� ������ �������� ���������
void Core_MenuReadDcr(TCoreMenu *p, struct MENU_DCR *Dcr, Uns Addr) // � ReadDcrFull()
{
	Uns Kmm = *p->MinMaxGain;
	Uns tmp =0;

	PFUNC_blkRead((Ptr)&p->Params[Addr].Dcr, (Ptr)Dcr, sizeof(struct MENU_DCR));
	
	if ((Dcr->Config.Val.Type == MT_DEC) && (Dcr->Config.Val.MinMax))
	{

		Dcr->Min = Dcr->Min * Kmm;

		tmp = Dcr->Max*(Kmm/100);///100 * Kmm;
		if (tmp>1200) Dcr->Max = 60000;
		else Dcr->Max = Dcr->Max * Kmm;

		//Dcr->Max = Dcr->Max * Kmm;
		Dcr->Def = Dcr->Def * Kmm;
	}
}
//---------------------------------------------------
// ������ ���������
Bool WriteValue(Uns memory, Uns param, Uns *value) // � EditParam()
{
	if (memory && !IsMemParReady()) return false;
	
	if (param == REG_CODE)
	{
		if ( UpdateCode(REG_PASSW1_NEW, param, *value, DEF_USER_PASS) )
			return true;
	}
	else if (param == REG_FCODE)
	{
		if ( UpdateCode(REG_PASSW2_NEW, param, *value, DEF_FACT_PASS) )
			return true;
	}

	*(ToUnsPtr(&g_Ram) + param) = *value;
	if (memory)
	{
		//if ( WriteToEeprom(param, value, 1) )
		if (ReadWriteEeprom(&Eeprom1,F_WRITE,param,value,1))

		{ 
			RefreshParams(param);
			return true;
		}
	}
	else return true;
	return false;
}
//---------------------------------------------------
// ���������� ��������������
Bool EnableEdit(Uns password)
{
	switch (password)
	{
		case 1: if (IsPassword1()) return false; break;
		case 2: if (IsPassword2()) return false; break;
	}

	return true;
}
//---------------------------------------------------
#if NEW_RAZ
void Core_SetDeff(TCoreMenu *p)	// 50 ��
{
	// ������ ��������� ��������� (������ C), ���������� ������� 2
	if (g_Ram.ramGroupC.FactCode)
	{
		p->setDefaultGroupNumber = GROUP_FACT_PAR;
		g_Ram.ramGroupH.Password2 = 0;
		g_Ram.ramGroupC.FactCode = 0;
	}

	// �� ���������
	if(!g_Core.DefFlag)
	{
		if (g_Ram.ramGroupD.SetDefaults)
		{
			if (!g_Ram.ramGroupA.Status.bit.Stop) g_Core.VlvDrvCtrl.Mpu.CancelFlag = True;
			else
			{
				g_Core.VlvDrvCtrl.EvLog.Value = CMD_DEFAULTS_USER;
				g_Core.DefFlag = 1;
				p->setDefaultGroupNumber = 1;
				g_Ram.ramGroupD.SetDefaults = 0;
			}
		}
		if (g_Ram.ramGroupC.SetDefaults)
		{
			if (!g_Ram.ramGroupA.Status.bit.Stop) g_Core.VlvDrvCtrl.Mpu.CancelFlag = True;
			else
				{
					g_Core.VlvDrvCtrl.EvLog.Value = CMD_DEFAULTS_FACT;
					g_Core.DefFlag = 1;
					p->setDefaultGroupNumber = 2;
					g_Ram.ramGroupC.SetDefaults = 0;
				}
		}
	}
	if (p->setDefaultGroupNumber)
		SetDefaultValues(p, &p->setDefaultGroupNumber);
}
#else
void Core_MenuDisplay(TCoreMenu *p)	// 50 ��
{
	// ������ ��������� ��������� (������ C), ���������� ������� 2
	if (g_Ram.ramGroupC.FactCode)
	{
		p->setDefaultGroupNumber = GROUP_FACT_PAR;
		g_Ram.ramGroupH.Password2 = 0;
		g_Ram.ramGroupC.FactCode = 0;
	}

	// �� ���������
	if (g_Ram.ramGroupD.SetDefaults)
	{
		g_Core.VlvDrvCtrl.EvLog.Value = CMD_DEFAULTS_USER;
		p->setDefaultGroupNumber = 1;
		g_Ram.ramGroupD.SetDefaults = 0;
	}
	if (g_Ram.ramGroupC.SetDefaults)
	{
		g_Core.VlvDrvCtrl.EvLog.Value = CMD_DEFAULTS_FACT;
		p->setDefaultGroupNumber = 2;
		g_Ram.ramGroupC.SetDefaults = 0;
	}

	if (p->setDefaultGroupNumber)
		SetDefaultValues(p, &p->setDefaultGroupNumber);
}
#endif
//---------------------------------------------------
// ��������� �������� �� ���������
void SetDefaultValues(TCoreMenu *p, Byte *groupNumber) // � Core_MenuDisplay()
{
	struct MENU_DCR Dcr;
	static LgUns DefAddr = 0; 
	Byte DefCode = 0;

	if (!IsMemParReady()) return;
	 // ������ ��������� ��������� (������ B), ���������� ������� 1
	if (*groupNumber == GROUP_EDIT_PAR) DefCode = M_EDIT_PAR;
	 // ������ ��������� ��������� (������ C), ���������� ������� 2
	else DefCode = M_FACT_PAR;

	if (DefAddr < RAM_DATA_SIZE)
	{
	    Core_MenuReadDcr(p, &Dcr, DefAddr);

   		a++;
	    g_Ram.ramGroupH.BusyValue = 100 - ((a * 100)/RAM_DATA_SIZE);//(a * (25/RAM_DATA_SIZE));
	    // ��������� ����� ��������� ����������
	    // ���� �� ����� � �� ���� ���
	    // ���� ��� ������� ����� B � C
	    if ((((Dcr.Config.all & DefCode) == DefCode)
		    && (DefAddr != REG_DRIVE_TYPE) // �� ��� �������
		    && (DefAddr != REG_GEAR_RATIO) // �� ��� ���������
		    && (DefAddr != REG_FACTORY_NUMBER) // �� �����
		    && (DefAddr != REG_PRODUCT_DATE) // �� ���� ������������
		    && (DefAddr != REG_ROD_TYPE)     // �� ��� �����
		    && (DefAddr != REG_TASK_TIME)
		    && (DefAddr != REG_MAX_TRQE)
		    && (DefAddr != REG_I_NOM)
		    && (DefAddr != REG_TU_OPEN_220)
		    && (DefAddr != REG_TU_OPEN_24)
		    && (DefAddr != REG_TU_OPEN_OFF)
		    && (DefAddr != REG_TU_CLOSE_220)
		    && (DefAddr != REG_TU_CLOSE_24)
		    && (DefAddr != REG_TU_CLOSE_OFF)
		    && (DefAddr != REG_TU_STOPC_220)
		    && (DefAddr != REG_TU_STOPC_24)
		    && (DefAddr != REG_TU_STOPC_OFF)
		    && (DefAddr != REG_TU_STOPO_220)
		    && (DefAddr != REG_TU_STOPO_24)
		    && (DefAddr != REG_TU_STOPO_OFF)
		    && (DefAddr != REG_TU_MU_220)
		    && (DefAddr != REG_TU_MU_24)
		    && (DefAddr != REG_TU_MU_OFF)
		    && (DefAddr != REG_TU_DU_220)
		    && (DefAddr != REG_TU_DU_24)
		    && (DefAddr != REG_TU_DU_OFF)
		    && (DefAddr != REG_TU_RESETAL_220)
		    && (DefAddr != REG_TU_RESETAL_24)
		    && (DefAddr != REG_TU_RESETAL_OFF)
		    && (DefAddr != REG_CORR_CLOSE_30)
			&& (DefAddr != REG_CORR_CLOSE_40)
			&& (DefAddr != REG_CORR_CLOSE_60)
			&& (DefAddr != REG_CORR_CLOSE_80)
			&& (DefAddr != REG_CORR_CLOSE_110)
			&& (DefAddr != REG_CORR_OPEN_30)
			&& (DefAddr != REG_CORR_OPEN_40)
			&& (DefAddr != REG_CORR_OPEN_60)
			&& (DefAddr != REG_CORR_OPEN_80)
			&& (DefAddr != REG_CORR_OPEN_110)
		    && (DefAddr != REG_TASK_DATE))
		    || (DefAddr == REG_CODE) || (DefAddr == REG_FCODE)) //??? � ���� �� �������������� ������
	    {
		*(ToUnsPtr(&g_Ram) + DefAddr) = Dcr.Def;

#if NEW_RAZ
		g_Core.DefFlag = 2;
#endif
		ReadWriteEeprom(&Eeprom1, F_WRITE, DefAddr, ToUnsPtr(&g_Ram) + DefAddr, 1);
		while (!IsMemParReady())
		{
		    FM25V10_Update(&Eeprom1);
		}
		// ������������� ��������, ��������� � �.�.
		RefreshParams(DefAddr);

		DefAddr++;

	    }
	    else
	    {
		DefAddr++;
		return;
	    }
	}
	else
	{
	    a = 0;
#if NEW_RAZ
	    g_Core.DefFlag = 0;
#endif
	    *groupNumber = 0;
	    DefAddr = 0;
	    return;
	}
	


	/*if (IsMemParReady())
	{
		// ������ ���������� � Eeprom
		//ReadWriteAllParams(F_WRITE,p);
		//DefAddr = 0;
		*groupNumber = 0;
	}*/
}
//--------------------------------------------------------
void ReadWriteAllParams(Byte cmd, TCoreMenu *p)	// � Core_MenuInit()
{
	struct MENU_DCR Dcr;
	Uns DefAddr = 0, count;

	while (DefAddr < RAM_DATA_SIZE)
	{	
		count = 1;
		if (DefAddr == REG_CALIB_CLOSE) // ��� ���������� LgUns
			count = 5;
		Core_MenuReadDcr(p,&Dcr,DefAddr);
		// ��������� ����� ��������� ����������
		if ( !(Dcr.Config.all & M_NVM)
			&&(DefAddr != REG_TASK_TIME)
			&&(DefAddr != REG_TASK_DATE)
			&&(DefAddr != REG_VER_PO)
			&&(DefAddr != REG_SUBVER_PO) )
		{
			*(ToUnsPtr(&g_Ram) + DefAddr) = 0;
		}
		else {
			if (g_Ram.ramGroupC.DriveType!=0
					&& (DefAddr==REG_MAX_TRQE || DefAddr==REG_I_NOM || DefAddr==REG_GEAR_RATIO
						|| (DefAddr>=REG_TORQUE_CURR && DefAddr<REG_TORQUE_ANG_SF)
					)
				)
			{

			} else
			{
				ReadWriteEeprom(&Eeprom1,cmd,DefAddr,ToUnsPtr(&g_Ram) + DefAddr,count);
			}
		}
		while (!IsMemParReady()) {FM25V10_Update(&Eeprom1); DELAY_US(1000);}
		// ������������� ��������, ��������� � �.�.
		RefreshParams(DefAddr);
		DefAddr += count;
	}
}
//---------------------------------------------------
// ��������� �������� ModBus
void SetModBusParams(void)
{
	//��� ���
	/*
	if ((g_Ram.ramGroupB.RS_BAUD_RATE == 0xFFFF) & (g_Ram.ramGroupB.RS_STATION == 0xFFFF))
	{
		g_Ram.ramGroupB.RS_BAUD_RATE= br19200;
		g_Ram.ramGroupB.RS_STATION	= 1;
		g_Ram.ramGroupB.RS_MODE		= pmNone;
		ReadWriteEeprom(&Eeprom1,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3); // ������ ���������� � Eeprom
	}
	*/
	//��� ���
	if ((g_Ram.ramGroupB.RsBaudRate == 0xFFFF) & (g_Ram.ramGroupB.RsStation == 0xFFFF || g_Ram.ramGroupB.RsStation == 0))
	{
		g_Ram.ramGroupB.RsBaudRate= br19200;
		g_Ram.ramGroupB.RsStation	= 1;
		g_Ram.ramGroupB.RsMode		= pmNone;
		ReadWriteEeprom(&Eeprom1,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3); // ������ ���������� � Eeprom
	}
	/*
	//��� ���
	if ((g_Ram.ramGroupB.RS_BAUD_RATE == 0xFFFF) & (g_Ram.ramGroupB.RS_STATION == 0xFFFF))
	{
		g_Ram.ramGroupB.RS_BAUD_RATE= br19200;
		g_Ram.ramGroupB.RS_STATION	= 1;
		g_Ram.ramGroupB.RS_MODE		= pmNone;
		ReadWriteEeprom(&Eeprom1,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3); // ������ ���������� � Eeprom
	}
	//��� Bluetooth
	if ((g_Ram.ramGroupB.RS_BAUD_RATE == 0xFFFF) & (g_Ram.ramGroupB.RS_STATION == 0xFFFF))
	{
		g_Ram.ramGroupB.RS_BAUD_RATE= br19200;
		g_Ram.ramGroupB.RS_STATION	= 1;
		g_Ram.ramGroupB.RS_MODE		= pmNone;
		ReadWriteEeprom(&Eeprom1,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3); // ������ ���������� � Eeprom
	}
	*/
}

#if NEW_RAZ

static void PutAddData(Uns Addr, Uns *Value)
{
	PutAddHiStr(Addr);

	if (!Value)
		PutAddLoStr(Addr + 1);
	else
	{
		DISPL_ClrLoStr(&g_Peref.Display);
		DecToStr(*Value, &g_Peref.Display.LoStr[7], 0, 2, True, False);
		g_Peref.Display.LoStr[9] = '%';
	}
}

void AddControl(void)
{
	if(!g_Ram.ramGroupA.Status.bit.Test)
	{
		/*if (!g_Core.PowerEnable)				// ���� ����������
		 {
		 GrH->BusyValue = 0;	// 		������� ����������
		 Menu.State = MS_START;		// ���� � ��������� �����
		 }
		 else if (ClearLogFlag)
		 {
		 g_Ram.ramGroupH.BusyValue = GetWriteLogPercent();
		 PutAddData(BUSY_STR_ADR, &GrH->BusyValue);
		 if (IsMemLogReady())
		 {
		 g_Core.Mcu.EvLog.Source = g_Stat.ClearLogSource;
		 ClearLogFlag = False;
		 }
		 }
		 else */if(g_Core.DefFlag)	// ���� ���������� ����� ���������� � ��������
		{
			if(g_Core.DefFlag == 1) 									//
			g_Ram.ramGroupH.BusyValue = 100;
			else												//
			{
				//g_Ram.ramGroupH.BusyValue = GetWriteParPercent();
				//if (IsMemParReady())
				//	g_Core.DefFlag = 0;
			}
			PutAddData(BUSY_STR_ADR, &g_Ram.ramGroupH.BusyValue);
		}
		else if(g_Core.VlvDrvCtrl.Mpu.CancelFlag) //������ ������� ���
		{
			PutAddData(CMD_CANC_ADR, Null);
			if(++g_Core.CancelTimer >= (Uns) CANCEL_TOUT)
			{
				g_Core.VlvDrvCtrl.Mpu.CancelFlag = False;
				g_Core.CancelTimer = 0;
			}
		}
		/*else if (g_Core.VlvDrvCtrl.Mpu.MpuBlockedFlag)	// ������� �� �������������
		 {

		 CancelTimer++;
		 if (CancelTimer <= 20)			// 2 ������� ���������� "������� ��������"
		 {
		 PutAddData(CMD_CANC_ADR, Null);
		 }
		 else
		 {
		 PutAddData(MPU_BLOCKED_ADR, Null);
		 }
		 if (CancelTimer >= 40)		// 2 ������� ���������� "��� �������������"
		 {
		 g_Core.Mcu.Mpu.MpuBlockedFlag = False;
		 CancelTimer = 0;
		 }
		 }*/
		else
		{
			g_Ram.ramGroupH.BusyValue = 0;
			menu.Indication = True;
			g_Core.CancelTimer = 0;
			return;
		}
	}
	else if(!g_Ram.ramGroupG.DisplShow)
	{
		g_Ram.ramGroupH.BusyValue = 0;
		menu.Indication = True;
		return;
	}

	/*g_Peref.BtnStatus &= BTN_STOP;	// ���� ��� ���� �� �������� ����, ���� ���� ����� ������ �� ��������� ���� � ������� �� ��������������
	 g_Peref.Pult.Key = 0;			// �������� ������� � ���*/
	menu.Indication = False;		// �� ����� � ����
}
#endif

//---------����� �����------------------------------------
