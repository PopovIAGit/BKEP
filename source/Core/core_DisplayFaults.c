/*
 * core_DisplayFaults.c
 *
 *  Created on: 14 окт. 2015 г.
 *      Author: PopovIA
 */

#include "core.h"

void Core_DisplayFaultsInit(TCoreDislpayFaults *p)
{
	p->DisplFaulstTimer = 0;
	p->DisplFault = 0;
	p->TestData = 0;
	p->DisplFaultFlag = 0;
	p->DisplFaultUnion.Dev.all = 0;
	p->DisplFaultUnion.Net.all = 0;
	p->DisplFaultUnion.Proc.all = 0;
	p->DisplFaultUnion.Load.all = 0;
}


void Core_DisplayFaultsUpdate(TCoreDislpayFaults *p)
{

	if(g_Ram.ramGroupG.Mode)
	{
		g_Peref.Display.data = g_Ram.ramGroupG.DisplShow;
		return;
	}

	// если нет не одной аварии гасим дисплей
	if (!g_Ram.ramGroupA.Faults.Dev.all && !g_Ram.ramGroupA.Faults.Proc.all && !g_Ram.ramGroupA.Faults.Net.all && !g_Ram.ramGroupA.Faults.Load.all)
	{
		g_Peref.Display.data = 999;
		return;
	}
//--------------------- если показали все коды и время обнулено то готовы к приему нового пакета данных

	if(p->DisplFaultUnion.Proc.all==0 && p->DisplFaultUnion.Net.all==0 && p->DisplFaultUnion.Dev.all==0 && p->DisplFaultUnion.Load.all==0 && p->DisplFaulstTimer==0)
		p->DisplFaultFlag = 0;

//------------------------------------------------------ если готовы к приему то копируем все аварии
	if (!p->DisplFaultFlag)
	{
		p->DisplFaultUnion = g_Ram.ramGroupA.Faults;
		p->DisplFaultFlag = 1;
		p->DisplFaulstTimer = 0;
	}
//---------------------------------------------------------- отображаем скопированные аварии

		if (p->DisplFaulstTimer)
		{
			p->DisplFaulstTimer--;
			g_Peref.Display.data = p->DisplFault;
		}
		else if(p->DisplFaultUnion.Proc.bit.NoMove)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Proc.bit.NoMove = 0;
			p->DisplFault = NoMove_CODE;
		}
		else if (p->DisplFaultUnion.Proc.bit.PhOrd)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Proc.bit.PhOrd = 0;
			p->DisplFault = PhOrd_CODE;
		}
		else if (p->DisplFaultUnion.Proc.bit.NoCalib || p->DisplFaultUnion.Proc.bit.NoClose || p->DisplFaultUnion.Proc.bit.NoOpen)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Proc.bit.NoCalib = 0;
			p->DisplFaultUnion.Proc.bit.NoClose = 0;
			p->DisplFaultUnion.Proc.bit.NoOpen = 0;
			p->DisplFault = NoCalib_CODE;
		}
		else if (p->DisplFaultUnion.Proc.bit.Overway)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Proc.bit.Overway = 0;
			p->DisplFault = Overway_CODE;
		}
		else if (p->DisplFaultUnion.Proc.bit.Mufta)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Proc.bit.Mufta = 0;
			p->DisplFault = Mufta_CODE;
		}
		else if (p->DisplFaultUnion.Proc.bit.MuDuDef)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Proc.bit.MuDuDef = 0;
			p->DisplFault = MuDuDef_CODE;
		}
		else if (p->DisplFaultUnion.Net.bit.UvR || p->DisplFaultUnion.Net.bit.UvS || p->DisplFaultUnion.Net.bit.UvT)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Net.bit.UvR = 0;
			p->DisplFaultUnion.Net.bit.UvS = 0;
			p->DisplFaultUnion.Net.bit.UvT = 0;
			p->DisplFault = Uv_CODE;
		}
		else if (p->DisplFaultUnion.Net.bit.OvR || p->DisplFaultUnion.Net.bit.OvS || p->DisplFaultUnion.Net.bit.OvT)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Net.bit.OvR = 0;
			p->DisplFaultUnion.Net.bit.OvS = 0;
			p->DisplFaultUnion.Net.bit.OvT = 0;
			p->DisplFault = Ov_CODE;
		}
		else if (p->DisplFaultUnion.Net.bit.VSk)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Net.bit.VSk = 0;
			p->DisplFault = VSk_CODE;
		}
		else if (p->DisplFaultUnion.Net.bit.BvR || p->DisplFaultUnion.Net.bit.BvS || p->DisplFaultUnion.Net.bit.BvT)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Net.bit.BvR = 0;
			p->DisplFaultUnion.Net.bit.BvS = 0;
			p->DisplFaultUnion.Net.bit.BvT = 0;
			p->DisplFault = Bv_CODE;
		}
		else if (p->DisplFaultUnion.Net.bit.OvR_max || p->DisplFaultUnion.Net.bit.OvS_max || p->DisplFaultUnion.Net.bit.OvT_max)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Net.bit.OvR_max = 0;
			p->DisplFaultUnion.Net.bit.OvS_max = 0;
			p->DisplFaultUnion.Net.bit.OvT_max = 0;
			p->DisplFault = Ov_max_CODE;
		}
		else if (p->DisplFaultUnion.Load.bit.PhlU || p->DisplFaultUnion.Load.bit.PhlV || p->DisplFaultUnion.Load.bit.PhlW)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Load.bit.PhlU = 0;
			p->DisplFaultUnion.Load.bit.PhlV = 0;
			p->DisplFaultUnion.Load.bit.PhlW = 0;
			p->DisplFault = PhlU_CODE;
		}
		else if (p->DisplFaultUnion.Load.bit.I2t)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Load.bit.PhlU = 0;
			p->DisplFault = I2t_CODE;
		}
		else if (p->DisplFaultUnion.Load.bit.ShCU || p->DisplFaultUnion.Load.bit.ShCV || p->DisplFaultUnion.Load.bit.ShCW)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Load.bit.ShCU = 0;
			p->DisplFaultUnion.Load.bit.ShCV = 0;
			p->DisplFaultUnion.Load.bit.ShCW = 0;
			p->DisplFault = ShCU_CODE;
		}
		else if (p->DisplFaultUnion.Load.bit.ISkew)
		{
			p->DisplFaultUnion.Load.bit.ISkew = 0;
		}
		else if (p->DisplFaultUnion.Dev.bit.PosSens)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Dev.bit.PosSens = 0;
			p->DisplFault = PosSens_CODE;
		}
		else if (p->DisplFaultUnion.Dev.bit.Memory1 || p->DisplFaultUnion.Dev.bit.Memory2)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Dev.bit.Memory1 = 0;
			p->DisplFaultUnion.Dev.bit.Memory2 = 0;
			p->DisplFault = Memory1_CODE;
		}
		else if (p->DisplFaultUnion.Dev.bit.Rtc)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Dev.bit.Rtc = 0;
			p->DisplFault = Rtc_CODE;
		}
		else if (p->DisplFaultUnion.Dev.bit.TSens)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Dev.bit.TSens = 0;
			p->DisplFault = TSens_CODE;
		}
		else if (p->DisplFaultUnion.Dev.bit.Th_BCD)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Dev.bit.Th_BCD = 0;
			p->DisplFault = Th_BCD_CODE;
		}
		else if (p->DisplFaultUnion.Dev.bit.Tl_BCD)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Dev.bit.Tl_BCD = 0;
			p->DisplFault = Tl_BCD_CODE;
		}
		else if (p->DisplFaultUnion.Dev.bit.Th_BCP)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Dev.bit.Th_BCP = 0;
			p->DisplFault = Th_BCP_CODE;
		}
		else if (p->DisplFaultUnion.Dev.bit.Tl_BCP)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Dev.bit.Tl_BCP = 0;
			p->DisplFault = Tl_BCP_CODE;
		}
		else if (p->DisplFaultUnion.Dev.bit.NoBCP_Connect)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Dev.bit.NoBCP_Connect = 0;
			p->DisplFault = NoBCP_Connect_CODE;
		}
		else if (p->DisplFaultUnion.Dev.bit.MpoMpzError)
		{
			p->DisplFaulstTimer = DISPL_FAULT_TIME;
			p->DisplFaultUnion.Dev.bit.NoBCP_Connect = 0;
			p->DisplFault = MpoMpzError_CODE;
		}

}
