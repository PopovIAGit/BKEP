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
//__inline void ReadWriteAllParams(Byte, TCoreMenu *);
//---------------------------------------------------
void Core_MenuInit(TCoreMenu *p)
{
	p->Data = ToPtr(&g_Ram);      	// Указатель на буфер данных
	
	p->Group.Count = GROUP_COUNT;    // Количество групп

	p->EvLogSelected = false;		// Флаг выбора записи
	p->EvLogGroup = p->Group.Count - 1;	// Номер группы журнала
	//p->EvLogNumber = REG_LOG_NUMBER;// Адрес номера последней записи в журнал
	//p->EvLogTime = REG_LOG_TIME;    // Адрес параметра времени записи журнала
	//p->EvLogDate = REG_LOG_DATE;    // Адрес параметра даты записи журнала

	//p->EvLog.Count = EVLOG_CNT;     // Количество записей журнала
	
	//p->Groups = groups;   			// Указатель на структуры групп
	//p->SubGroups = subgroups;		// Указатель на структуры подгрупп
	p->Params = params;   			// Указатель на структуры параметров
	p->Values = values;   			// Указатель на структуры строковых значений
	
	p->EnableEdit = EnableEdit;
	p->WriteValue = WriteValue;

	// Чтение всех параметров из Eeprom
	//ReadWriteAllParams(F_READ,p);
	// Проверяем настроен ли ModBus
	SetModBusParams();
	// Номер версии
	//g_Ram.ramGroupA.VER_PO = SVN_REVISION/100;
	//g_Ram.ramGroupA.SUBVER_PO = SVN_REVISION;
	//while (g_Ram.ramGroupA.SUBVER_PO >= 100) g_Ram.ramGroupA.SUBVER_PO -= 100;
}
//---------------------------------------------------
// Чтение журнала
/*__inline Bool ReadLog(TCoreMenu *p)	// в SelectGroup()
{
	LgUns addr;

	addr = EVLOG_DATA_START_ADR + p->EvLog.Position * EVLOG_DATA_CNT;
	if ( ReadWriteClearFlash(&g_MainFlash, F_READ, addr, (Uns *)&g_Ram.logRecord, EVLOG_DATA_CNT) )
		{WaitForMainFlash();}
	else return false;
	// Обнулим неиспользуемые записи
	if ((g_Ram.logRecord.LogDate.all == 0xFFFF)&&(g_Ram.logRecord.LogPositionPr < 0))
		memset(&g_Ram.logRecord, 0,sizeof(TStatRecord));
	return true;
}*/
//---------------------------------------------------

//------------------------------------------------------------------
// Подтверждение или изменение паролей
Bool UpdateCode(Uns addrPassw, Uns addrCode, Uns value, Uns def)	// в WriteValue()
{
	/*Uns *password = ToUnsPtr(&g_Ram) + addrPassw;
	Uns *code = ToUnsPtr(&g_Ram) + addrCode;
	Bool writeFlag = false;

	//if (!IsEepromReady()) return false;
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
		if ( WriteToEeprom(addrPassw, password, 1)) return true;
	*/
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
	/*
	if (memory && !IsEepromReady()) return false;
	
	if (param == REG_CODE)
	{
		if ( UpdateCode(REG_PASSW1_NEW, param, *value, g_Ram.ramGroupA.VER_PO) )
			return true;
	}
	else if (param == REG_FCODE)
	{
		if ( UpdateCode(REG_PASSW2_NEW, param, *value, g_Ram.ramGroupA.VER_PO) )
			return true;
	}

	*(ToUnsPtr(&g_Ram) + param) = *value;
	if (memory)
	{
		if ( WriteToEeprom(param, value, 1) )
		{ 
			RefreshParams(param);
			return true;
		}
//		Mcu.EvLog.Value = CMD_PAR_CHANGE;
	}
	else return true;
*/
	return false;
}
//---------------------------------------------------
// Разрешение редактирования
Bool EnableEdit(Uns password)
{
	/*switch (password)
	{
		case 1: if (IsPassword1()) return false; break;
		case 2: if (IsPassword2()) return false; break;
	}*/
	return true;
}
//---------------------------------------------------
// Установка значений по умолчанию
void SetDefaultValues(TCoreMenu *p, Byte *groupNumber) // в Core_MenuDisplay()
{
	/*struct MENU_DCR Dcr;
	static LgUns DefAddr = 0; 
	Byte DefCode = 0;

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
				&&(DefAddr != REG_INDICATOR_TYPE) // Не тип индикатора
				&&(DefAddr != REG_FACTORY_NUMBER) // Не номер
				&&(DefAddr != REG_PRODUCT_DATE) // Не дата изготовления
				&&(DefAddr != REG_TASK_TIME)
				&&(DefAddr != REG_TASK_DATE))
			||(DefAddr == REG_CODE)||(DefAddr == REG_FCODE))
	 		*(ToUnsPtr(&g_Ram) + DefAddr) = Dcr.Def;
		DefAddr++;
		// Пишем "УСТРОЙСТВО ЗАНЯТО ...%"
		ShowAddStrings(p, 0, _IQmpy(_IQdiv(DefAddr,RAM_DATA_SIZE),100LU));
		return;
	}
	
	if (IsEepromReady())
	{
		// Запись всех параметров в Eeprom
		ReadWriteAllParams(F_WRITE,p);
		DefAddr = 0;
		*groupNumber = 0;

		UpdateStringsDisplay(p);
	}*/
}
//--------------------------------------------------------
/*void ReadWriteAllParams(Byte cmd, TCoreMenu *p)	// в Core_MenuInit()
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
		else ReadWriteEeprom(&g_Eeprom,cmd,DefAddr,ToUnsPtr(&g_Ram) + DefAddr,count);
		while (!IsEepromReady()) {AT25XXX_Update(&g_Eeprom); DelayUs(1000);}
		// Инициализация фильтров, масштабов и т.д.
		RefreshParams(DefAddr);
		DefAddr += count;
	}
}*/
//---------------------------------------------------
// Установка значений ModBus
void SetModBusParams()
{
	/* надо будет вставить
	 * if ((g_Ram.ramGroupB.RS_BAUD_RATE == 0xFFFF) & (g_Ram.ramGroupB.RS_STATION == 0xFFFF))
	{
		g_Ram.ramGroupB.RS_BAUD_RATE= br19200;
		g_Ram.ramGroupB.RS_STATION	= 1;
		g_Ram.ramGroupB.RS_MODE		= pmNone;
		ReadWriteEeprom(&g_Eeprom,F_WRITE,REG_RS_BAUD_RATE,ToUnsPtr(&g_Ram) + REG_RS_BAUD_RATE,3);
	}*/
}
//---------------------------------------------------
// Обновление строк дисплея
void UpdateStringsDisplay(pCoreMenu p)
{
	if (p->State == MS_START) p->logoIsShown = false;
	// Обновление верхней и нижней строки дисплея
	p->updateHiStr = p->updateLowStr = p->updateMainValues = true;
}
//---------Конец файла------------------------------------
