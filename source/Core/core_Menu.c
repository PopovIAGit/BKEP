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

Byte 	showLog = 0, 
		startReadLog = 0, 
		TimeOutReadLog = 0, 
		taskStartShow = 0;
Uns grcGear = 0;

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
	p->Data = ToPtr(&g_Ram);      	// ��������� �� ����� ������
	p->Group.Count = GROUP_COUNT;    // ���������� �����
	p->Params = params;   			// ��������� �� ��������� ����������
	p->Values = values;   			// ��������� �� ��������� ��������� ��������
	p->EnableEdit = EnableEdit;
	p->WriteValue = WriteValue;

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

//------------------------------------------------------------------
// ������������� ��� ��������� �������
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
			writeFlag = true;
		}
	}
	else if (value != 0)
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

	return false;
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

	PFUNC_blkRead((Ptr)&p->Params[Addr].Dcr, (Ptr)Dcr, sizeof(struct MENU_DCR));
	
	if ((Dcr->Config.Val.Type == MT_DEC) && (Dcr->Config.Val.MinMax))
	{
		Dcr->Min = Dcr->Min * Kmm;
		Dcr->Max = Dcr->Max * Kmm;
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
		p->setDefaultGroupNumber = 1;
		g_Ram.ramGroupD.SetDefaults = 0;
	}
	if (g_Ram.ramGroupC.SetDefaults)
	{
		p->setDefaultGroupNumber = 2;
		g_Ram.ramGroupC.SetDefaults = 0;
	}

	if (p->setDefaultGroupNumber)
		SetDefaultValues(p, &p->setDefaultGroupNumber);
}
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
		Core_MenuReadDcr(p,&Dcr,DefAddr);

		// ��������� ����� ��������� ����������
		// ���� �� ����� � �� ���� ���
		// ���� ��� ������� ����� B � C
		if ((((Dcr.Config.all & DefCode) == DefCode)
				&&(DefAddr != REG_DRIVE_TYPE) // �� ��� �������
				&&(DefAddr != REG_GEAR_RATIO) // �� ��� ���������
				&&(DefAddr != REG_FACTORY_NUMBER) // �� �����
				&&(DefAddr != REG_PRODUCT_DATE) // �� ���� ������������
				&&(DefAddr != REG_TASK_TIME)
				&&(DefAddr != REG_MAX_TRQE)
				&&(DefAddr != REG_I_NOM)
				&&(DefAddr != REG_TASK_DATE))
			||(DefAddr == REG_CODE)||(DefAddr == REG_FCODE))//??? � ���� �� �������������� ������
		{
			*(ToUnsPtr(&g_Ram) + DefAddr) = Dcr.Def;

			ReadWriteEeprom(&Eeprom1,F_WRITE,DefAddr,ToUnsPtr(&g_Ram) + DefAddr,1);
			while (!IsMemParReady()) {FM25V10_Update(&Eeprom1); }
			// ������������� ��������, ��������� � �.�.
			RefreshParams(DefAddr);

			DefAddr++;

		} else {DefAddr++; return;}
	} else {
		*groupNumber =0;
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
		while (!IsMemParReady()) {FM25V10_Update(&Eeprom1); DelayUs(1000);}
		// ������������� ��������, ��������� � �.�.
		RefreshParams(DefAddr);
		DefAddr += count;
	}
}
//---------------------------------------------------
// ��������� �������� ModBus
void SetModBusParams()
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

//---------����� �����------------------------------------
