
#include "core.h"

	Uns FirstUpdate = 0;
	Int InomDef[26]  	     = {13,11,18,52,52,47 ,56 ,110,85  ,148 ,240 ,294 ,358 ,574 ,840 ,11,9 ,13,32,32,33 ,73 ,85 ,95  ,150, 294};		// default значения для Inom для разных приводов
	Int MomMaxDef[26]  	     = {10,10,40,40,80,100,400,400,1000,1000,1000,1500,2000,3500,5000,10,10,40,40,80,100,400,400,1000,1000,1500};	//				для Mmax
	Int TransCurrDef[11] 	 = {1500,1000,1000,1000,1000,1000,1100,1100,1100,700,300};			//				для TransCur править
	Int GearRatioDef[26] 	 = {5250,5250,7560,5320,7560,7560,16720,16720,25116,25116,14400,14400,14400,16800,15600,5250,5250,7560,5320,7560,7560,16720,16720,25116,25116,14400};		//для передаточного числа редуктора


	void Core_Drive_Update(void)
	{
		//WAIT_FOR_EEPROM_READY();
		switch(g_Ram.ramGroupC.DriveType)
		{
			case dt100_A25:
				PFUNC_blkRead(&drive1,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
			 	PFUNC_blkRead(&TransCurrDef[0], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt100_A50:
				PFUNC_blkRead(&drive2,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[1], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt400_B20:
				PFUNC_blkRead(&drive3,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[2], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt400_B50:
				PFUNC_blkRead(&drive4,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[3], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt800_V40:
				PFUNC_blkRead(&drive5,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[4], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt1000_V20:
				PFUNC_blkRead(&drive6,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[5], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt4000_G9:
				PFUNC_blkRead(&drive7,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[6], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt4000_G18:
				PFUNC_blkRead(&drive8,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[7], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt10000_D6:
				PFUNC_blkRead(&drive9,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[8], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt10000_D12:
				PFUNC_blkRead(&drive10,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[8], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt10000_D10:
				PFUNC_blkRead(&drive11,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[9], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt15000_D10:
				PFUNC_blkRead(&drive12,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[9], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt20000_F40:
				PFUNC_blkRead(&drive13,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[8], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt35000_F48:
				PFUNC_blkRead(&drive14,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[10], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt50000_F48:
				PFUNC_blkRead(&drive15,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[9], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt100_A25_S:
				PFUNC_blkRead(&drive16, (Int *) (&g_Ram.ramGroupH.TqCurr),		    LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[0],		    (Int *) (&g_Ram.ramGroupH.TransCurr), 1);
				break;
			case dt100_A50_S:
				PFUNC_blkRead(&drive17, (Int *) (&g_Ram.ramGroupH.TqCurr), LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[1], (Int *) (&g_Ram.ramGroupH.TransCurr), 1);
				break;
			case dt400_B20_S:
				PFUNC_blkRead(&drive18, (Int *) (&g_Ram.ramGroupH.TqCurr), LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[2], (Int *) (&g_Ram.ramGroupH.TransCurr), 1);
				break;
			case dt400_B50_S:
				PFUNC_blkRead(&drive19, (Int *) (&g_Ram.ramGroupH.TqCurr), LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[3], (Int *) (&g_Ram.ramGroupH.TransCurr), 1);
				break;
			case dt800_V40_S:
				PFUNC_blkRead(&drive20, (Int *) (&g_Ram.ramGroupH.TqCurr), LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[4], (Int *) (&g_Ram.ramGroupH.TransCurr), 1);
				break;
			case dt1000_V20_S:
				PFUNC_blkRead(&drive21, (Int *) (&g_Ram.ramGroupH.TqCurr), LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[5], (Int *) (&g_Ram.ramGroupH.TransCurr), 1);
				break;
			case dt4000_G9_S:
				PFUNC_blkRead(&drive22, (Int *) (&g_Ram.ramGroupH.TqCurr), LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[6], (Int *) (&g_Ram.ramGroupH.TransCurr), 1);
				break;
			case dt4000_G18_S:
				PFUNC_blkRead(&drive23, (Int *) (&g_Ram.ramGroupH.TqCurr), LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[7], (Int *) (&g_Ram.ramGroupH.TransCurr), 1);
				break;
			case dt10000_D6_S:
				PFUNC_blkRead(&drive24, (Int *) (&g_Ram.ramGroupH.TqCurr), LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[8], (Int *) (&g_Ram.ramGroupH.TransCurr), 1);
				break;
			case dt10000_D12_S:
				PFUNC_blkRead(&drive25, (Int *) (&g_Ram.ramGroupH.TqCurr), LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[8], (Int *) (&g_Ram.ramGroupH.TransCurr), 1);
				break;
			case dt15000_D10_T:
                PFUNC_blkRead(&drive26,         (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
                PFUNC_blkRead(&TransCurrDef[10], (Int *)(&g_Ram.ramGroupH.TransCurr),          1);
                break;
		    }

		if ((g_Ram.ramGroupC.DriveType < 27)&&(g_Ram.ramGroupC.DriveType != 0))
		{
			if ((g_Ram.ramGroupC.GearRatio != GearRatioDef[g_Ram.ramGroupC.DriveType - 1])
					|| (g_Ram.ramGroupC.Inom!= InomDef[g_Ram.ramGroupC.DriveType - 1])
					|| (g_Ram.ramGroupC.MaxTorque != MomMaxDef[g_Ram.ramGroupC.DriveType - 1]))
			{
				if (FirstUpdate==0)
				{
					g_Ram.ramGroupC.GearRatio = GearRatioDef[g_Ram.ramGroupC.DriveType - 1];
					g_Ram.ramGroupC.Inom = InomDef[g_Ram.ramGroupC.DriveType - 1];
					g_Ram.ramGroupC.MaxTorque = MomMaxDef[g_Ram.ramGroupC.DriveType - 1];
					Core_ProtectionI2TInit(&g_Core.Protections.I2t);
				}
			}
		}
		FirstUpdate=1;
	}

	void Drive_ReWrite_Update(void)
	{
		if ((g_Ram.ramGroupC.DriveType < 27)&&(g_Ram.ramGroupC.DriveType != 0))
		{
			if ((g_Ram.ramGroupC.GearRatio != GearRatioDef[g_Ram.ramGroupC.DriveType - 1])
					|| (g_Ram.ramGroupC.Inom!= InomDef[g_Ram.ramGroupC.DriveType - 1])
					|| (g_Ram.ramGroupC.MaxTorque != MomMaxDef[g_Ram.ramGroupC.DriveType - 1]))
			{
				if (IsMemParReady())
				{
					g_Ram.ramGroupC.GearRatio = GearRatioDef[g_Ram.ramGroupC.DriveType - 1];
					g_Ram.ramGroupC.Inom = InomDef[g_Ram.ramGroupC.DriveType - 1];
					g_Ram.ramGroupC.MaxTorque = MomMaxDef[g_Ram.ramGroupC.DriveType - 1];
					Core_ProtectionI2TInit(&g_Core.Protections.I2t);
					WriteToEeprom(REG_MAX_TRQE, &g_Ram.ramGroupC.MaxTorque, 3);
				}
			}
		}
	}


