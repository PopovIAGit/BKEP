/*======================================================================
Имя файла:          g_Ram.c
Автор:
Версия файла:
Дата изменения:
Описание:
Обновление переменной g_Ram
======================================================================*/

#include "g_Ram.h"
#include "core.h"
//#include "drive.h"
#include "comm.h"
#include "peref.h"
#include "stat.h"

void ReadParams(void);
void RefreshData(void);
//__inline void CheckParams(void);
void WriteAllParams(void);
void ReadAllParams(void);
void CubRefresh(TCubStr *v, TCubArray *Array);


//---------------------------------------------------
void g_Ram_Init(TRam *p)
{
	p->ramGroupD.SetDefaults = 0;
	p->ramGroupC.SetDefaults = 0;

	p->ramGroupA.Status.all = 0x01;
	p->ramGroupA.CalibState = p->ramGroupH.CalibState;
	p->ramGroupA.CycleCnt =  p->ramGroupH.CycleCnt;
	//PrevCycle = p->ramGroupH.CycleCnt;
	g_Core.PrevCycle = p->ramGroupH.CycleCnt;
	p->ramGroupA.VersionPO = VERSION;
	p->ramGroupC.SubVersionPO = SUBVERSION;

	//if (p->ramGroupH.ScFaults) LowPowerReset |= BIT0;

}

/*void ReadWriteAllParams(Byte cmd)
{
	struct MENU_DCR Dcr;
	Uns DefAddr = 0, count;

	while (DefAddr < RAM_DATA_SIZE)
	{
		count = 1;
		// Для считывания LgUns
		if (DefAddr == REG_CALIB_CLOSE) count = 5;
		Core_MenuReadDcr(p,&Dcr,DefAddr);

		EEPROM_Func(MEM_PAR, cmd, DefAddr, ToUnsPtr(&g_Ram) + DefAddr, count);
		//ReadWriteEeprom(&g_Eeprom,cmd,DefAddr,ToUnsPtr(&g_Ram) + DefAddr,count);

		while (!IsMemParReady()) { FM25V10_Update(&Eeprom1);}				// Ждем пока прочтет все параметры из памяти
																		// Функция вызывается при инициализации, поэтому необходимо зациклить
		RefreshParams(DefAddr);
		DefAddr += count;
	}
}*/

/*void ReadAllParams(void)
{
	EEPROM_Func(MEM_PAR, F_READ, RAM_ADR, &g_Ram, RAM_DATA_SIZE);
}*/
// Чтение всех параметров
//#define ReadAllParams() 			ReadPar(RAM_ADR, &Ram, RAM_DATA_SIZE)
//#define ReadPar(Adr, Buf, Cnt)  	ParFunc(MEM_PAR,     F_READ,  Adr, (Ptr)(Buf), Cnt)
//void EEPROM_Func(Byte Memory, Byte Func, Uns Addr, Uns *Data, Uns Count)
//-------------------------------------------------------

/*__inline void CheckParams(void)
{
	struct MENU_DCR Dcr;
	register Int  i;
	Uns *Data = ToUnsPtr(&g_Ram);

	for (i=RAM_DATA_SIZE-1; i >= 0; i--)
	{
		GetDcr(i, &Dcr);
		if (!Dcr.Config.Val.Memory) Data[i] = 0;
		else
		#ifndef USE_DEFPAR_VALUES
		if (!CheckRange(Data[i], Dcr.Min, Dcr.Max))
		#endif
		{
			Data[i] = Dcr.Def;
			DataError = True;
		}
	}

	if (DataError) WriteAllParams();
}*/

/*void WriteAllParams(void)
{
	EEPROM_Func(MEM_PAR, F_WRITE, RAM_ADR, &g_Ram, RAM_DATA_SIZE);
}*/
// Запись всех параметров
//#define WriteAllParams()			WritePar(RAM_ADR, &Ram, RAM_DATA_SIZE)
//#define WritePar(Adr, Buf, Cnt) 	ParFunc(MEM_PAR,     F_WRITE, Adr, (Ptr)(Buf), Cnt)
//#define ParFunc	EEPROM_Func
//--------------------------------------------------------


//---------------------------------------------------
void g_Ram_Update(TRam *p)
{
	g_Ram.ramGroupA.Temper 	  = g_Peref.TSens.Temper;

	//------ Core -> RAM ------------------------------------
	p->ramGroupA.Status = g_Core.Status;
	//InterfIndication(p);

	//----- Peref -> RAM -----------------------------------

	 p->ramGroupA.Ur 			= g_Peref.sinObserver.UR.Output;
	 p->ramGroupA.Us 			= g_Peref.sinObserver.US.Output;
	 p->ramGroupA.Ut 			= g_Peref.sinObserver.UT.Output;
	 p->ramGroupH.Umid 			= g_Peref.Umid;
	 p->ramGroupH.VSkValue 		= SkewCalc(p->ramGroupA.Ur, p->ramGroupA.Us, p->ramGroupA.Ut, p->ramGroupH.Umid);

	 if(!g_Core.Status.bit.Stop)
	 {
		 p->ramGroupA.Iu 		= g_Peref.sinObserver.IU.Output;
		 p->ramGroupA.Iv 		= g_Peref.sinObserver.IV.Output;
		 p->ramGroupA.Iw 		= g_Peref.sinObserver.IW.Output;
		 p->ramGroupA.AngleUI 	= g_Peref.AngleUI;
		 p->ramGroupH.Imid 		= g_Peref.Imid;
		 p->ramGroupH.ISkewValue= SkewCalc(p->ramGroupA.Iu, p->ramGroupA.Iv, p->ramGroupA.Iw, p->ramGroupH.Imid);
	 }
	 else
	 {
		 p->ramGroupA.Iu 		= 0;
		 p->ramGroupA.Iv 		= 0;
		 p->ramGroupA.Iw 		= 0;
		 p->ramGroupA.AngleUI 	= 0;
		 p->ramGroupH.Imid 		= 0;
		 p->ramGroupH.ISkewValue= 0;
	 }

	 p->ramGroupA.Speed = g_Peref.Position.speedRPM;
	 // ------------------------------------------
	 p->ramGroupA.Position = p->ramGroupC.Position;

}

void InterfIndication(TRam *p)
{
	p->ramGroupA.Faults.Net.all  = p->ramGroupH.FaultsNet.all;
	p->ramGroupA.Faults.Load.all = p->ramGroupH.FaultsLoad.all;
	p->ramGroupA.Faults.Dev.all  = p->ramGroupH.FaultsDev.all;

	//p->ramGroupA.StateTu.all     = g_Comm.digitInterface.Inputs.all;
	p->ramGroupA.StateTs.all	 = g_Comm.digitInterface.Outputs.all;
}
//---------------------------------------------------
void g_Ram_SetTorque(void)
{

}
//---------------------------------------------------
// Обновление значение по требованию
void RefreshParams(Uns addr)
{
	//TCoreProtections *pProtections 	= &g_Core.protections;
	TPeref *pPeref 					= &g_Peref;
	TPerefPosition *pPosition 		= &g_Peref.Position;

	if (addr == REG_GEAR_RATIO)	{

		pPosition->GearRatio = g_Ram.ramGroupC.GearRatio;
		pPosition->GearInv = CalcClbGearInv(&g_Peref.Position);

	}else if (addr == REG_OVERWAY_ZONE) { g_Core.VlvDrvCtrl.Valve.BreakDelta = (((LgUns)pPosition->GearRatio * (LgUns)g_Ram.ramGroupB.OverwayZone) << *pPosition->PosSensPow)/10; //CalcClbAbsRev(&Calib, g_Ram.ramGroupB.OverwayZone);

	}else if (addr == REG_TU_INVERT){

		// Все входа не реверсивные
		// хуйню заменить на нормальный код
		//TuOpen.Level  = DIN_LEVEL(SBEXT_OPEN,  (Uns)g_Ram.ramGroupB.InputMask.bit.Open);
		//TuClose.Level = DIN_LEVEL(SBEXT_CLOSE, (Uns)g_Ram.ramGroupB.InputMask.bit.Close);
		//TuStop.Level  = DIN_LEVEL(SBEXT_STOP,  (Uns)g_Ram.ramGroupB.InputMask.bit.Stop);
		//TuMu.Level    = DIN_LEVEL(SBEXT_MU,    (Uns)g_Ram.ramGroupB.InputMask.bit.Mu);
		//TuDu.Level    = DIN_LEVEL(SBEXT_DU,    (Uns)g_Ram.ramGroupB.InputMask.bit.Du);

	} else if (addr == REG_DRIVE_TYPE) {

		//TorqueObsInit();
		//TorqueMax = g_Ram.ramGroupC.MaxTorque * 10; // Перевод максимального момента в *10
		//CubRefresh(&Torq.Cub1, &g_Ram.ramGroupGrH->TqCurr);
		//CubRefresh(&Torq.Cub2, &g_Ram.ramGroupGrH->TqAngUI);
		//CubRefresh(&Torq.Cub3, &g_Ram.ramGroupGrH->TqAngSf);

	} else if (addr == REG_COEF_VOLT_FILTER){

		pPeref->URfltr.Tf = _IQdiv(g_Ram.ramGroupC.CoefVoltFltr, 1E08); 		//peref_ApFilter3Init(&pPeref->URfltr);
		pPeref->USfltr.Tf = pPeref->URfltr.Tf; 								  	//peref_ApFilter3Init(&pPeref->USfltr);
		pPeref->UTfltr.Tf = pPeref->URfltr.Tf; 								  	//peref_ApFilter3Init(&pPeref->UTfltr);

		peref_ApFilter1Init(&pPeref->URfltr, (LgUns)Prd18kHZ, g_Ram.ramGroupC.CoefVoltFltr);
		peref_ApFilter1Init(&pPeref->USfltr, (LgUns)Prd18kHZ, g_Ram.ramGroupC.CoefVoltFltr);
		peref_ApFilter1Init(&pPeref->UTfltr, (LgUns)Prd18kHZ, g_Ram.ramGroupC.CoefVoltFltr);



	} else if (addr == REG_CURRENT_FILTER) {

		pPeref->IUfltr.Tf = _IQdiv(g_Ram.ramGroupC.CoefCurrFltr, 1E08); 		//peref_ApFilter3Init(&pPeref->IUfltr);
		pPeref->IVfltr.Tf = pPeref->IUfltr.Tf; 									//peref_ApFilter3Init(&pPeref->IVfltr);
		pPeref->IWfltr.Tf = pPeref->IUfltr.Tf; 									//peref_ApFilter3Init(&pPeref->IWfltr);

		peref_ApFilter1Init(&pPeref->IUfltr, (LgUns)Prd18kHZ, g_Ram.ramGroupC.CoefCurrFltr);
		peref_ApFilter1Init(&pPeref->IVfltr, (LgUns)Prd18kHZ, g_Ram.ramGroupC.CoefCurrFltr);
		peref_ApFilter1Init(&pPeref->IWfltr, (LgUns)Prd18kHZ, g_Ram.ramGroupC.CoefCurrFltr);

	} else if (addr >= REG_TORQUE_CURR && addr < REG_TORQUE_CURR+20) {
		//CubRefresh(&Torq.Cub1, &g_Ram.ramGroupGrH->TqCurr);

	} else if (addr >= REG_TORQUE_ANGLE_UI && addr < REG_TORQUE_ANGLE_UI+20){
		// CubRefresh(&Torq.Cub2, &g_Ram.ramGroupGrH->TqAngUI);

	}else if (addr >= REG_TORQUE_ANG_SF && addr < REG_TORQUE_ANG_SF+20){
		//CubRefresh(&Torq.Cub3, &g_Ram.ramGroupGrH->TqAngSf);
	}

}
//---------------------------------------------------
/*void CubRefresh(TCubStr *v, TCubArray *Array)	//
{
	register TCubPoint *Pt;
	register Uns i, j;

	for (i=0; i < CUB_COUNT1; i++)
	{
		for (j=0; j < CUB_COUNT2; j++)
		{
			Pt = &v->Points[i][j];
			Pt->Z = Array->Data[i][j];
		}
	}
}*/
//---------------------------------------------------
Int MinMax3IntValue (Int val1, Int val2, Int val3)
{
	Int minMax = 0;
	if (((val1 <= val2)&&(val1 >= val3))||((val1 >= val2)&&(val1 <= val3)))
		minMax = val1;
	if (((val2 <= val1)&&(val2 >= val3))||((val2 >= val1)&&(val2 <= val3)))
		minMax = val2;
	if (((val3 <= val1)&&(val3 >= val2))||((val3 >= val1)&&(val3 <= val2)))
		minMax = val3;
	return minMax;
}
//---------------------------------------------------
Int Max3Int (Int val1, Int val2, Int val3)
{
	Int Max = 0;
	if ((val1 >= val2)&&(val1 >= val3))
		Max = val1;
	if ((val2 >= val1)&&(val2 >= val3))
		Max = val2;
	if ((val3 >= val2)&&(val3 >= val1))
		Max = val3;
	return Max;
}
//--------------------------------------------------------

