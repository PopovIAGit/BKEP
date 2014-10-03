/*======================================================================
Имя файла:          core_Menu.c
Автор:
Версия файла:
Дата изменения:
======================================================================*/

#include "add_ValueConv.h"
#include "core_MenuParams.h" 	// Файл параметров
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
	p->Data = ToPtr(&g_Ram);      	// Указатель на буфер данных
	p->Group.Count = GROUP_COUNT;    // Количество групп
	p->Params = params;   			// Указатель на структуры параметров
	p->Values = values;   			// Указатель на структуры строковых значений
	p->EnableEdit = EnableEdit;
	p->WriteValue = WriteValue;

	// Чтение всех параметров из Eeprom
	ReadWriteAllParams(F_READ,p);
	//RefreshParams(REG_DRIVE_TYPE);
	//RefreshParams(REG_SIN_FILTER_TF);
	//RefreshParams(REG_RMS_FILTER_TF);
	// Проверяем настроен ли ModBus
	SetModBusParams();
	// Номер версии
	//g_Ram.ramGroupA.VER_PO = SVN_REVISION/100;
	//g_Ram.ramGroupA.SUBVER_PO = SVN_REVISION;
	//while (g_Ram.ramGroupA.SUBVER_PO >= 100) g_Ram.ramGroupA.SUBVER_PO -= 100;

	p->MinMaxGain = &g_Ram.ramGroupC.MaxTorque;

}
//---------------------------------------------------

//------------------------------------------------------------------
// Подтверждение или изменение паролей
Bool UpdateCode(Uns addrPassw, Uns addrCode, Uns value, Uns def)	// в WriteValue()
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
// Чтение настроек параметра
void ReadDcrFull(TCoreMenu *p, Uns Addr)	// в ChangeCoordinate()
{
	struct MENU_VAL_CFG *Val = &p->Dcr.Config.Val;
	
	Core_MenuReadDcr(p, &p->Dcr, Addr);

	p->Bit = 0xFF;

	// Для знакового значения увеличиваем диапазон
	p->EditRate = Val->RateMax;
	if (p->EditType && (Val->Type == MT_DEC) && Val->Signed) p->EditRate++;
}
//---------------------------------------------------
// Чтение и установка границ значения параметра
void Core_MenuReadDcr(TCoreMenu *p, struct MENU_DCR *Dcr, Uns Addr) // в ReadDcrFull()
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
// Запись параметра
Bool WriteValue(Uns memory, Uns param, Uns *value) // в EditParam()
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
// Разрешение редактирования
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
void Core_MenuDisplay(TCoreMenu *p)	// 50 Гц
{
	// Меняем заводские параметры (группа C), защищённые паролем 2
	if (g_Ram.ramGroupC.FactCode)
	{
		p->setDefaultGroupNumber = GROUP_FACT_PAR;
		g_Ram.ramGroupH.Password2 = 0;
		g_Ram.ramGroupC.FactCode = 0;
	}

	// По умолчанию
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
// Установка значений по умолчанию
void SetDefaultValues(TCoreMenu *p, Byte *groupNumber) // в Core_MenuDisplay()
{
	struct MENU_DCR Dcr;
	static LgUns DefAddr = 0; 
	Byte DefCode = 0;

	if (!IsMemParReady()) return;
	 // Меняем заводские параметры (группа B), защищённые паролем 1
	if (*groupNumber == GROUP_EDIT_PAR) DefCode = M_EDIT_PAR;
	 // Меняем заводские параметры (группа C), защищённые паролем 2
	else DefCode = M_FACT_PAR;

	if (DefAddr < RAM_DATA_SIZE)
	{	
		Core_MenuReadDcr(p,&Dcr,DefAddr);

		// Проверяем какие параметры записывать
		// Если не Время и не Дата или
		// Если Код доступа групп B и C
		if ((((Dcr.Config.all & DefCode) == DefCode)
				&&(DefAddr != REG_DRIVE_TYPE) // Не тип привода
				&&(DefAddr != REG_GEAR_RATIO) // Не тип редуктора
				&&(DefAddr != REG_FACTORY_NUMBER) // Не номер
				&&(DefAddr != REG_PRODUCT_DATE) // Не дата изготовления
				&&(DefAddr != REG_TASK_TIME)
				&&(DefAddr != REG_MAX_TRQE)
				&&(DefAddr != REG_I_NOM)
				&&(DefAddr != REG_TASK_DATE))
			||(DefAddr == REG_CODE)||(DefAddr == REG_FCODE))//??? а надо ли перезаписывать пароли
		{
			*(ToUnsPtr(&g_Ram) + DefAddr) = Dcr.Def;

			ReadWriteEeprom(&Eeprom1,F_WRITE,DefAddr,ToUnsPtr(&g_Ram) + DefAddr,1);
			while (!IsMemParReady()) {FM25V10_Update(&Eeprom1); }
			// Инициализация фильтров, масштабов и т.д.
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
		// Запись параметров в Eeprom
		//ReadWriteAllParams(F_WRITE,p);
		//DefAddr = 0;
		*groupNumber = 0;
	}*/
}
//--------------------------------------------------------
void ReadWriteAllParams(Byte cmd, TCoreMenu *p)	// в Core_MenuInit()
{
	struct MENU_DCR Dcr;
	Uns DefAddr = 0, count;

	while (DefAddr < RAM_DATA_SIZE)
	{	
		count = 1;
		if (DefAddr == REG_CALIB_CLOSE) // Для считывания LgUns
			count = 5;
		Core_MenuReadDcr(p,&Dcr,DefAddr);
		// Проверяем какие параметры записывать
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
		// Инициализация фильтров, масштабов и т.д.
		RefreshParams(DefAddr);
		DefAddr += count;
	}
}
//---------------------------------------------------
// Установка значений ModBus
void SetModBusParams()
{
	//для БКД
	/*
	if ((g_Ram.ramGroupB.RS_BAUD_RATE == 0xFFFF) & (g_Ram.ramGroupB.RS_STATION == 0xFFFF))
	{
		g_Ram.ramGroupB.RS_BAUD_RATE= br19200;
		g_Ram.ramGroupB.RS_STATION	= 1;
		g_Ram.ramGroupB.RS_MODE		= pmNone;
		ReadWriteEeprom(&Eeprom1,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3); // запись параметров в Eeprom
	}
	*/
	//для АСУ
	if ((g_Ram.ramGroupB.RsBaudRate == 0xFFFF) & (g_Ram.ramGroupB.RsStation == 0xFFFF || g_Ram.ramGroupB.RsStation == 0))
	{
		g_Ram.ramGroupB.RsBaudRate= br19200;
		g_Ram.ramGroupB.RsStation	= 1;
		g_Ram.ramGroupB.RsMode		= pmNone;
		ReadWriteEeprom(&Eeprom1,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3); // запись параметров в Eeprom
	}
	/*
	//для УПП
	if ((g_Ram.ramGroupB.RS_BAUD_RATE == 0xFFFF) & (g_Ram.ramGroupB.RS_STATION == 0xFFFF))
	{
		g_Ram.ramGroupB.RS_BAUD_RATE= br19200;
		g_Ram.ramGroupB.RS_STATION	= 1;
		g_Ram.ramGroupB.RS_MODE		= pmNone;
		ReadWriteEeprom(&Eeprom1,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3); // запись параметров в Eeprom
	}
	//для Bluetooth
	if ((g_Ram.ramGroupB.RS_BAUD_RATE == 0xFFFF) & (g_Ram.ramGroupB.RS_STATION == 0xFFFF))
	{
		g_Ram.ramGroupB.RS_BAUD_RATE= br19200;
		g_Ram.ramGroupB.RS_STATION	= 1;
		g_Ram.ramGroupB.RS_MODE		= pmNone;
		ReadWriteEeprom(&Eeprom1,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3); // запись параметров в Eeprom
	}
	*/
}

//---------Конец файла------------------------------------
