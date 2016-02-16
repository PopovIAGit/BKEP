
#include "core.h"

	Uns FirstUpdate = 0;
	Int InomDef[15]  	 = {13,11,18,52,52,47,56,110,85,148,240,294,358,840,840};						// default �������� ��� Inom ��� ������ ��������
	Int MomMaxDef[15]  	 = {10,10,40,40,80,100,400,400,1000,1000,1000,1500,2000,5000,5000};				//				��� Mmax
	Int TransCurrDef[10] = {1500,1000,1000,1000,1000,1000,1100,1100,1100,600};					//				��� TransCur �������
	Int GearRatioDef[15] = {5250,5250,7360,7360,7360,7360,16720,16720,16016,16016,17400,14400,14400,15600,15600};						//��� ������������� ����� ���������


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
				PFUNC_blkRead(&TransCurrDef[9], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
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
				PFUNC_blkRead(&TransCurrDef[9], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
				break;
			case dt35000_F48:
							PFUNC_blkRead(&drive14,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
							PFUNC_blkRead(&TransCurrDef[9], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
							break;
			case dt50000_F48:
							PFUNC_blkRead(&drive15,  	 	(Int *)(&g_Ram.ramGroupH.TqCurr),   LENGTH_TRQ);
							PFUNC_blkRead(&TransCurrDef[9], (Int *)(&g_Ram.ramGroupH.TransCurr),		  1);
							break;
		}

		if ((g_Ram.ramGroupC.DriveType < 16)&&(g_Ram.ramGroupC.DriveType != 0))
		{
			if ((g_Ram.ramGroupC.GearRatio != GearRatioDef[g_Ram.ramGroupC.DriveType - 1])
					|| (g_Ram.ramGroupC.Inom!= InomDef[g_Ram.ramGroupC.DriveType - 1])
					|| (g_Ram.ramGroupC.MaxTorque != MomMaxDef[g_Ram.ramGroupC.DriveType - 1]))
			{
				/*if (FirstUpdate==1)
				{
					WAIT_FOR_EEPROM_READY();
					if (IsMemParReady())
					{
						g_Ram.ramGroupC.GearRatio = GearRatioDef[g_Ram.ramGroupC.DriveType - 1];
						g_Ram.ramGroupC.Inom = InomDef[g_Ram.ramGroupC.DriveType - 1];
						g_Ram.ramGroupC.MaxTorque = MomMaxDef[g_Ram.ramGroupC.DriveType - 1];
						WriteToEeprom(GetAdr(ramGroupC.MaxTorque), &g_Ram.ramGroupC.MaxTorque, 3);
					}
				} else*/
				if (FirstUpdate==0){
					g_Ram.ramGroupC.GearRatio = GearRatioDef[g_Ram.ramGroupC.DriveType - 1];
					g_Ram.ramGroupC.Inom = InomDef[g_Ram.ramGroupC.DriveType - 1];
					g_Ram.ramGroupC.MaxTorque = MomMaxDef[g_Ram.ramGroupC.DriveType - 1];
				}
			}
		} FirstUpdate=1;
	}

	void Drive_ReWrite_Update(void)
	{
		if ((g_Ram.ramGroupC.DriveType < 16)&&(g_Ram.ramGroupC.DriveType != 0))
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
					WriteToEeprom(REG_MAX_TRQE, &g_Ram.ramGroupC.MaxTorque, 3);
				}
			}
		}
	}


