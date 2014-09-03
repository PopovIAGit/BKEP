
#include "core.h"

	Int InomDef[13]  	 = {13,11,18,52,52,47,56,110,85,148,240,294,358};						// default значения для Inom для разных приводов
	Int MomMaxDef[13]  	 = {10,10,40,40,80,100,400,400,1000,1000,1000,1500,2000};				//				для Mmax
	Int TransCurrDef[10] = {1000,1000,1000,1000,1000,1000,1100,1100,1100,1100};					//				для TransCur править
	Int GearRatioDef[13] = {5250,5250,7360,7360,7360,7360,16720,16720,16016,16016,17400,14400,14400};						//для передаточного числа редуктора


	void Core_Drive_Init(TCoreDrive *p)
	{
		p->DriveType 	= &g_Ram.ramGroupC.DriveType;
		p->Inom 		= &g_Ram.ramGroupC.Inom;
		p->GearRatio 	= &g_Ram.ramGroupC.GearRatio;
		p->MaxTorqe 	= &g_Ram.ramGroupC.MaxTorque;
	}

	void Core_Drive_Update(TCoreDrive *p)
	{
		switch(*p->DriveType)
		{
			case dt100_A25:
				PFUNC_blkRead(&drive1,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
			 	PFUNC_blkRead(&TransCurrDef[0], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;

			case dt100_A50:
				PFUNC_blkRead(&drive2,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[1], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt400_B20:
				PFUNC_blkRead(&drive3,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[2], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt400_B50:
				PFUNC_blkRead(&drive4,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[3], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt800_V40:
				PFUNC_blkRead(&drive5,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[4], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt1000_V20:
				PFUNC_blkRead(&drive6,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[5], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt4000_G9:
				PFUNC_blkRead(&drive7,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[6], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt4000_G18:
				PFUNC_blkRead(&drive8,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[7], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt10000_D6:
				PFUNC_blkRead(&drive9,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[8], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt10000_D12:
				PFUNC_blkRead(&drive10,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[9], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt10000_D10:
				PFUNC_blkRead(&drive11,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[9], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt15000_D10:
				PFUNC_blkRead(&drive12,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[9], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt20000_F40:
				PFUNC_blkRead(&drive13,  	 (Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
				PFUNC_blkRead(&TransCurrDef[9], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
		}

		if (*p->DriveType < 14)
		{
			if ((*p->GearRatio != GearRatioDef[*p->DriveType - 1])
					|| (*p->Inom != InomDef[*p->DriveType - 1])
					|| (*p->MaxTorqe != MomMaxDef[*p->DriveType - 1]))
			{
				if (IsMemParReady())
				{
					*p->GearRatio = GearRatioDef[*p->DriveType - 1];
					*p->Inom = InomDef[*p->DriveType - 1];
					*p->MaxTorqe = MomMaxDef[*p->DriveType - 1];
					WriteToEeprom(GetAdr(ramGroupC.MaxTorque), &g_Ram.ramGroupC.MaxTorque, 3);

				}
			}
		}
	}


