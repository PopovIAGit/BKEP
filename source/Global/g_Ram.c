/*======================================================================
Имя файла:          g_Ram.c
Автор:
Версия файла:
Дата изменения:
Описание:
Обновление переменной g_Ram
======================================================================*/
//
#include "g_Ram.h"
#include "core.h"
#include "comm.h"
#include "peref.h"
#include "stat.h"

void ReadParams(void);
void RefreshData(void);
//__inline void CheckParams(void);
void WriteAllParams(void);
void ReadAllParams(void);
void CubRefresh(TCubStr *v, TCubArray *Array);
void ReWriteParams(void);

Uns RefreshCub=0;
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
	p->ramGroupA.VersionPO = VERSION_MAJOR*1000+VERSION_MINOR;
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
    p->ramGroupA.Temper = g_Peref.TSens.Temper;

    //------ Core -> RAM ------------------------------------
    p->ramGroupA.Status = g_Core.Status;
    p->ramGroupA.Faults.Net.all  = (g_Core.Protections.outFaults.Net.all  | g_Core.Protections.outDefects.Net.all);
    p->ramGroupA.Faults.Load.all = (g_Core.Protections.outFaults.Load.all | g_Core.Protections.outDefects.Load.all);
    p->ramGroupA.Faults.Proc.all = (g_Core.Protections.outFaults.Proc.all | g_Core.Protections.outDefects.Proc.all);
    p->ramGroupA.Faults.Dev.all  = (g_Core.Protections.outFaults.Dev.all  | g_Core.Protections.outDefects.Dev.all);

    //----- Peref -> RAM -----------------------------------

    p->ramGroupA.Ur = g_Peref.sinObserver.UR.Output;
    p->ramGroupA.Us = g_Peref.sinObserver.US.Output;
    p->ramGroupA.Ut = g_Peref.sinObserver.UT.Output;
    p->ramGroupH.Umid = g_Peref.Umid;
    p->ramGroupH.VSkValue = SkewCalc(p->ramGroupA.Ur, p->ramGroupA.Us, p->ramGroupA.Ut, p->ramGroupH.Umid);

    if (!g_Core.Status.bit.Stop)
	{
	    p->ramGroupH.IuPr = (g_Peref.sinObserver.IU.Output * 1000) / p->ramGroupC.Inom;
	    p->ramGroupH.IvPr = (g_Peref.sinObserver.IV.Output * 1000) / p->ramGroupC.Inom;
	    p->ramGroupH.IwPr = (g_Peref.sinObserver.IW.Output * 1000) / p->ramGroupC.Inom;
	    p->ramGroupH.Imidpr = (g_Peref.Imid * 1000) / p->ramGroupC.Inom;
	    p->ramGroupH.Imid 	= g_Peref.Imid;
	    p->ramGroupA.AngleUI = g_Peref.AngleUI;
	    p->ramGroupH.ISkewValue = SkewCalc(g_Peref.sinObserver.IU.Output, g_Peref.sinObserver.IV.Output, g_Peref.sinObserver.IW.Output,
		    g_Peref.Imid);
	    if (p->ramGroupB.IIndicMode == imRms)
		{
		    p->ramGroupA.Iu = g_Peref.sinObserver.IU.Output;
		    p->ramGroupA.Iv = g_Peref.sinObserver.IV.Output;
		    p->ramGroupA.Iw = g_Peref.sinObserver.IW.Output;
		}
	    if (p->ramGroupB.IIndicMode == imPercent)
		{
		    p->ramGroupA.Iu = p->ramGroupH.IuPr;
		    p->ramGroupA.Iv = p->ramGroupH.IvPr;
		    p->ramGroupA.Iw = p->ramGroupH.IwPr;
		}
	}
    else
	{
	    p->ramGroupA.Iu = 0;
	    p->ramGroupA.Iv = 0;
	    p->ramGroupA.Iw = 0;
	    p->ramGroupA.AngleUI = 0;
	    p->ramGroupH.Imid = 0;
	    p->ramGroupH.ISkewValue = 0;
	}

    //p->ramGroupA.Speed = g_Peref.Position.speedRPM;
    p->ramGroupA.Speed = LVS_flag;
    p->ramGroupA.CycleCnt = p->ramGroupH.CycleCnt;

    p->ramGroupC.Position 	= p->ramGroupH.Position >> p->ramGroupC.PosPrecision;
    p->ramGroupA.Position 	= p->ramGroupC.Position;
    p->ramGroupH.FullStep 	= g_Peref.Position.FullStep;
    p->ramGroupC.ClosePosition 	= p->ramGroupH.ClosePosition >> p->ramGroupC.PosPrecision;
    p->ramGroupC.OpenPosition 	= p->ramGroupH.OpenPosition >> p->ramGroupC.PosPrecision;
    p->ramGroupA.StateTu.all 	= g_Comm.digitInterface.Inputs.all;
    p->ramGroupA.StateTs.all 	= g_Comm.digitInterface.Outputs.all;
    p->ramGroupH.ReverseType 	= rvtNone;

    p->ramGroupB.MOD_FAULT = GpioDataRegs.GPBDAT.bit.GPIO39;
    GpioDataRegs.GPBDAT.bit.GPIO48 = p->ramGroupB.RES_ERR;

    if (STATE_TU24)
	{p->ramGroupB.InputType = it24;}
    else if (!STATE_TU24)
	{p->ramGroupB.InputType = it220;}

    ReWriteParams();

}
//---------------------------------------------------

void ReWriteParams(void)
{
	Drive_ReWrite_Update();

	if (RefreshCub==1){
		if (IsMemParReady())
		{
			RefreshCub = 0;
			WriteToEeprom(GetAdr(ramGroupH.TqCurr.Data[0][0])-1, &g_Ram.ramGroupH.TransCurr, 41);
		}
	}

}

// Обновление значение по требованию
void RefreshParams(Uns addr)
{

	TPerefPosition *pPosition 		= &g_Peref.Position;

	if (addr == REG_GEAR_RATIO)	{

		pPosition->GearRatio = g_Ram.ramGroupC.GearRatio;
		pPosition->GearInv = CalcClbGearInv(&g_Peref.Position);

	}else if (addr == REG_OVERWAY_ZONE) { g_Core.VlvDrvCtrl.Valve.BreakDelta = (((LgUns)pPosition->GearRatio * (LgUns)g_Ram.ramGroupB.OverwayZone) << *pPosition->PosSensPow)/10; //CalcClbAbsRev(&Calib, g_Ram.ramGroupB.OverwayZone);

	} else if (addr == REG_DRIVE_TYPE) {

		Core_Drive_Update();
		CubRefresh(&g_Core.TorqObs.Cub1, &g_Ram.ramGroupH.TqCurr);
		CubRefresh(&g_Core.TorqObs.Cub2, &g_Ram.ramGroupH.TqAngUI);
		RefreshCub = 1;

	}	else if (addr >= REG_TORQUE_CURR && addr < REG_TORQUE_ANG_SF)
	{
		CubRefresh(&g_Core.TorqObs.Cub1, &g_Ram.ramGroupH.TqCurr);
		CubRefresh(&g_Core.TorqObs.Cub2, &g_Ram.ramGroupH.TqAngUI);
		RefreshCub = 1;
	}	else if (addr == REG_MAX_TRQE)
	{
		 g_Core.TorqObs.TorqueMax = g_Ram.ramGroupC.MaxTorque * 10; //??? убрать в обновление параметров
	}	else if (addr == REG_SIN_FILTER_TF){

			peref_ApFilter1Init(&g_Peref.URfltr, (Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);		// Инициализируем фильтры
			peref_ApFilter1Init(&g_Peref.USfltr, (Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.UTfltr, (Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.IUfltr, (Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.IVfltr, (Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.IWfltr, (Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);


			peref_ApFilter1Init(&g_Peref.UfltrOpen, 	 (Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);		// Инициализируем фильтры
			peref_ApFilter1Init(&g_Peref.UfltrClose, 	 (Uns)Prd18kHZ, g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.UfltrStop, 	 (Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.UfltrMu, 		 (Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.UfltrResetAlarm,(Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.UfltrReadyTU, 	 (Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);
			peref_ApFilter1Init(&g_Peref.UfltrDU, 		 (Uns)Prd18kHZ,  g_Ram.ramGroupC.SinTf);

	} else if (addr == REG_RMS_FILTER_TF) {

			peref_ApFilter3Init(&g_Peref.UR3fltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);		// Инициализируем фильтры
			peref_ApFilter3Init(&g_Peref.US3fltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.UT3fltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.IU3fltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.IV3fltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.IW3fltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);

			peref_ApFilter1Init(&g_Peref.Phifltr, (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);
			peref_ApFilter1Init(&g_Peref.Umfltr,  (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.Imfltr,  (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);

			peref_ApFilter3Init(&g_Peref.U3fltrOpen, 	  (Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);		// Инициализируем фильтры
			peref_ApFilter3Init(&g_Peref.U3fltrClose, 	  (Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.U3fltrStop, 	  (Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.U3fltrMu, 		  (Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.U3fltrResetAlarm,(Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.U3fltrReadyTU,   (Uns)Prd50HZ,  g_Ram.ramGroupC.RmsTf);
			peref_ApFilter3Init(&g_Peref.U3fltrDU, 		  (Uns)Prd50HZ, g_Ram.ramGroupC.RmsTf);

	} else if (addr == REG_TU_TYPE) {

		if (g_Ram.ramGroupB.InputType==it24)
		{
			g_Peref.InDigSignalObserver.parSensors.p_UOpen_Mpy		= &g_Ram.ramGroupB.UOpen_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UClose_Mpy		= &g_Ram.ramGroupB.p_UClose_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UStop_Mpy		= &g_Ram.ramGroupB.p_UStop_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UMu_Mpy		= &g_Ram.ramGroupB.p_UMu_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UStop_Mpy		= &g_Ram.ramGroupB.p_UStop_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UResetAlarm_Mpy= &g_Ram.ramGroupB.p_UResetAlarm_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UReadyTu_Mpy	= &g_Ram.ramGroupB.p_UReadyTu_Mpy24;
			g_Peref.InDigSignalObserver.parSensors.p_UDu_Mpy		= &g_Ram.ramGroupB.p_UDu_Mpy24;
		} else
		{
			g_Peref.InDigSignalObserver.parSensors.p_UOpen_Mpy		= &g_Ram.ramGroupB.UOpen_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UClose_Mpy		= &g_Ram.ramGroupB.p_UClose_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UStop_Mpy		= &g_Ram.ramGroupB.p_UStop_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UMu_Mpy		= &g_Ram.ramGroupB.p_UMu_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UStop_Mpy		= &g_Ram.ramGroupB.p_UStop_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UResetAlarm_Mpy= &g_Ram.ramGroupB.p_UResetAlarm_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UReadyTu_Mpy	= &g_Ram.ramGroupB.p_UReadyTu_Mpy220;
			g_Peref.InDigSignalObserver.parSensors.p_UDu_Mpy		= &g_Ram.ramGroupB.p_UDu_Mpy220;
		}
	}


}
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

