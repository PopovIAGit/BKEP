/*======================================================================
��� �����:
�����:
������ �����:
���� ���������:
======================================================================*/

#include "g_Ram.h"
#include "stat.h"
#include "core.h"
#include "comm.h"

TStat			g_Stat;
Uns 			prevCmd = 0, delayStart = 2.0 * Prd50HZ;

char LogEvBufIndex = 0;
bool LogEvMainDataFlag = false;
char LogEvBufCurIndex = 0;
Byte LogEvBufSeconds = 0;
Bool ReadLogFlag = False;

//ma LogSIM 4 ���� 3
Uns  PrevLogAddr[4] = {0,0,0,0};
Uns  PrevLogCount[4] = {0,0,0,0};

void InitInfoModule(TInfoModule *);
void InitLogEvent(pTLogEvent);
void InitLogCmd(pTLogCmd);
void InitLogParam(pTLogParam);
//ma LogSIM
void InitLogSim(pTLogSim);
void InitTables(void);

//---------------------------------------------------
// ������������� �������
void Stat_Init(TStat *s)
{
	int i=0;

	InitLogEvent(&s->LogEvent);
	InitInfoModule(&s->Im);

	memset(&s->LogEventBuffer[0],	0, sizeof(LOG_EV_BUF_CELL_COUNT));//???		// ����� ������� �������

	InitLogCmd(&s->LogCmd);
	InitLogParam(&s->LogParam);
	//ma LogSIM
	InitLogSim(&s->LogSim);


	PrevLogAddr[PREV_LEV_INDEX]   = g_Ram.ramGroupH.LogEvAddr;
	PrevLogAddr[PREV_LCMD_INDEX]  = g_Ram.ramGroupH.LogCmdAddr;
	PrevLogAddr[PREV_LPAR_INDEX]  = g_Ram.ramGroupH.LogParamAddr;
	PrevLogAddr[PREV_LSIM_INDEX]  = g_Ram.ramGroupH.LogSimAddr;

	PrevLogCount[PREV_LEV_INDEX]  = g_Ram.ramGroupH.LogEvCount;
	PrevLogCount[PREV_LCMD_INDEX] = g_Ram.ramGroupH.LogCmdCount;
	PrevLogCount[PREV_LPAR_INDEX] = g_Ram.ramGroupH.LogParamCount;
	PrevLogCount[PREV_LSIM_INDEX] = g_Ram.ramGroupH.LogSimCount;

	memset(&s->LogParam.MbBuffer[0], 0, MB_PARAM_BUF_COUNT);//???
	memset(&s->Im.ImReadBuf[0], 0, (IM_READ_BUF_SIZE * 2));//???

	for (i= 0; i < LOG_EV_BUF_CELL_COUNT; i++)
	{
		s->LogEventBuffer[i].LogStatus.all = 0;
		s->LogEventBuffer[i].LogPositionPr = 0;
		s->LogEventBuffer[i].LogTorque	 = 0;
		s->LogEventBuffer[i].LogUr		 = 0;
		s->LogEventBuffer[i].LogUs		 = 0;
		s->LogEventBuffer[i].LogUt		 = 0;
		s->LogEventBuffer[i].LogIu		 = 0;
		s->LogEventBuffer[i].LogIv		 = 0;
		s->LogEventBuffer[i].LogIw		 = 0;
		s->LogEventBuffer[i].LogTemper	 = 0;
		s->LogEventBuffer[i].LogInputs	 = 0;
		s->LogEventBuffer[i].LogOutputs	 = 0;
	}
	g_Ram.ramGroupH.LogReset = 0;
	InitTables();
	// ������������� �������� ����
	//memset(&g_MainFlash, 0, sizeof(TAT25DF041A));
	//g_MainFlash.CSAddr = CS_MAIN_FLASH;

	//AT25DF041A_Init(&g_MainFlash);
	// ������������� ��������� ����
	//memset(&g_ReservFlash, 0, sizeof(TAT25DF041A));

	//AT25DF041A_Init(&g_ReservFlash);
	// ������������� �����
	//AT25XXX_Init(&g_Eeprom);

	// ������������� �����
	/*Eeprom1.CsFunc = &Eeprom1CsSet;
	FM25V10_Init(&Eeprom1);
	Eeprom2.CsFunc = &Eeprom2CsSet;
	FM25V10_Init(&Eeprom2);*/
}
//---------------------------------------------------
void InitTables(void)
{
	ImEvLogMainAddrsTable[0]	= GetAdr(ramGroupB.DevTime);
	ImEvLogMainAddrsTable[1]	= GetAdr(ramGroupB.DevDate);
	ImEvLogMainAddrsTable[2]	= GetAdr(ramGroupA.Status);
	ImEvLogMainAddrsTable[3]	= GetAdr(ramGroupA.Faults.Proc);
	ImEvLogMainAddrsTable[4]	= GetAdr(ramGroupA.Faults.Net);
	ImEvLogMainAddrsTable[5]	= GetAdr(ramGroupA.Faults.Load);
	ImEvLogMainAddrsTable[6]	= GetAdr(ramGroupA.Faults.Dev);
	ImEvLogMainAddrsTable[7]	= GetAdr(ramGroupA.PositionPr);
	ImEvLogMainAddrsTable[8]	= GetAdr(ramGroupA.Torque);
	ImEvLogMainAddrsTable[9]	= GetAdr(ramGroupA.Ur);
	ImEvLogMainAddrsTable[10]	= GetAdr(ramGroupA.Us);
	ImEvLogMainAddrsTable[11]	= GetAdr(ramGroupA.Ut);
	ImEvLogMainAddrsTable[12]	= GetAdr(ramGroupA.Iu);
	ImEvLogMainAddrsTable[13]	= GetAdr(ramGroupA.Iv);
	ImEvLogMainAddrsTable[14]	= GetAdr(ramGroupA.Iw);
	ImEvLogMainAddrsTable[15]	= GetAdr(ramGroupA.TemperBKP);
	ImEvLogMainAddrsTable[16]	= GetAdr(ramGroupA.StateTu);
	ImEvLogMainAddrsTable[17]	= GetAdr(ramGroupA.StateTs);
	ImEvLogMainAddrsTable[18]	= GetAdr(ramGroupH.Seconds);
	ImEvLogMainAddrsTable[19]	= FIRST_RECORD_FLAG_ADDR;

	ImEvLogBufAddrsTable[0]  = GetAdr(ramGroupA.Status);
	ImEvLogBufAddrsTable[1]  = GetAdr(ramGroupA.PositionPr);
	ImEvLogBufAddrsTable[2]  = GetAdr(ramGroupA.Torque);
	ImEvLogBufAddrsTable[3]  = GetAdr(ramGroupA.Ur);
	ImEvLogBufAddrsTable[4]  = GetAdr(ramGroupA.Us);
	ImEvLogBufAddrsTable[5]  = GetAdr(ramGroupA.Ut);
	ImEvLogBufAddrsTable[6]  = GetAdr(ramGroupA.Iu);
	ImEvLogBufAddrsTable[7]  = GetAdr(ramGroupA.Iv);
	ImEvLogBufAddrsTable[8]  = GetAdr(ramGroupA.Iw);
	ImEvLogBufAddrsTable[9]  = GetAdr(ramGroupA.TemperBKP);
	ImEvLogBufAddrsTable[10] = GetAdr(ramGroupA.StateTu);
	ImEvLogBufAddrsTable[11] = GetAdr(ramGroupA.StateTs);

	ImCmdLogAddrsTable[0] = GetAdr(ramGroupB.DevTime);
	ImCmdLogAddrsTable[1] = GetAdr(ramGroupB.DevDate);
	ImCmdLogAddrsTable[2] = GetAdr(ramGroupH.Seconds);
	ImCmdLogAddrsTable[3] = GetAdr(ramGroupH.LogControlWord);
	ImCmdLogAddrsTable[4] = GetAdr(ramGroupA.Status);
	ImCmdLogAddrsTable[5] = GetAdr(ramGroupA.StateTs);
	ImCmdLogAddrsTable[6] = GetAdr(ramGroupH.PosFix);

	ImParamLogAddrsTable[0] = GetAdr(ramGroupB.DevTime);
	ImParamLogAddrsTable[1] = GetAdr(ramGroupB.DevDate);
	ImParamLogAddrsTable[2] = GetAdr(ramGroupH.Seconds);
	ImParamLogAddrsTable[3] = NEW_PARAM_ADDR;
	ImParamLogAddrsTable[4] = NEW_PARAM_VALUE_ADDR;

	ImSimLogAddrsTable[0] = GetAdr(ramGroupB.DevTime);
	ImSimLogAddrsTable[1] = GetAdr(ramGroupB.DevDate);
	ImSimLogAddrsTable[2] = GetAdr(ramGroupH.Seconds);
	ImSimLogAddrsTable[3]  = WORD_0_SIMID;  //��� �� ����� � ����� ������ �����
	ImSimLogAddrsTable[4]  = WORD_1_SIMID;
	ImSimLogAddrsTable[5]  = WORD_2_SIMID;
	ImSimLogAddrsTable[6]  = WORD_3_SIMID;
	ImSimLogAddrsTable[7]  = WORD_4_SIMID;
	ImSimLogAddrsTable[8]  = WORD_5_SIMID;
	ImSimLogAddrsTable[9]  = WORD_6_SIMID;
	ImSimLogAddrsTable[10] = WORD_7_SIMID;
	ImSimLogAddrsTable[11] = WORD_8_SIMID;
	ImSimLogAddrsTable[12] = WORD_9_SIMID;
}
//---------------------------------------------------

void InitInfoModule(TInfoModule *im)
{
	memset(im,	0, sizeof(TInfoModule));

	im->ControlMode=RECEIVE_FUNC_MODE;

	im->FuncState		= imInit;
	im->Index			= 0;
	im->TxIndex			= 0;
	im->CanSendData		= False;
	im->IsTxBusy		= &g_Comm.Bluetooth.TxBusy;
	im->Timer			= 0;
	im->Period			= IM_TIMEOUT;
	im->TimerIndex		= 0;
	im->PeriodIndex		= 30; //300��

	im->CanReadNextRec	= False;
	im->IsReadRecBusy	= False;
	im->IsBufReady		= False;
	im->IsLogTransmit	= False;

	im->HardwareSrc		= imSrcBluetooth;
	im->HardwareSrc		= imSrcModbus;
	im->HardwareSrc		= imSrcNone;

	im->DeviceDataPtr   = (Uns *)&g_Ram;
	// ��� ��������������� ������
	im->EnableReceive	 = g_Comm.Bluetooth.EnableRx;
	im->EnableTransmit	 = g_Comm.Bluetooth.EnableTx;
	im->TransmitByte	 = g_Comm.Bluetooth.TransmitByteIM;

}
//---------------------------------------------------
void InitLogEvent(TLogEvent *le)
{
	le->Enable		= false;						// ���������� ������ ������� �������
	le->ExecFlag	= false;						// ���� ������������ ������ �������
	le->WriteFlag	= false;						// ���� ���������� ������ ������� � ������
	le->EventFlag	= false;						// ���� ������������� �������
	le->FirstEvent	= true;							// ���� ������� ������� ����� ��������� �������
	le->FaultsCount	= 4;							// ���������� ��������� ��������������

	memset(&le->FaultsState[0],	0, sizeof(FAULTS_COUNT_DEF));		// ����� ��������� ��������������
	memset(&le->FaultsFiltr[0],	0, sizeof(FAULTS_COUNT_DEF));		// ����� ���������� ��������� ��������������
	memset(&le->FaultsFiltrPrev[0],	0, sizeof(FAULTS_COUNT_DEF));	// ����� ���������� ��������� ��������������

	le->Time		= &g_Ram.ramGroupB.DevTime.all;
	le->Date		= &g_Ram.ramGroupB.DevDate.all;
	le->Status		= &g_Ram.ramGroupA.Status.all;
	le->PositionPr	= &g_Ram.ramGroupA.PositionPr;
	le->Torque		= &g_Ram.ramGroupA.Torque;
	le->Ur			= &g_Ram.ramGroupA.Ur;
	le->Us			= &g_Ram.ramGroupA.Ur;
	le->Ut			= &g_Ram.ramGroupA.Ur;
	le->Iu			= &g_Ram.ramGroupA.Iu;
	le->Iv			= &g_Ram.ramGroupA.Iu;
	le->Iw			= &g_Ram.ramGroupA.Iu;
	le->Temper		= &g_Ram.ramGroupA.TemperBKP;
	le->Inputs		= &g_Ram.ramGroupA.StateTu.all;
	le->Outputs		= &g_Ram.ramGroupA.StateTs.all;
	le->Seconds		= &g_Ram.ramGroupH.Seconds;

	memset(&le->Data[0],	0, sizeof(LOG_EV_BUF_DATA_COUNT));		// ����� ������� �������

}
//---------------------------------------------------
void InitLogCmd(TLogCmd *lc)
{
	lc->Enable 		= false;							// ���� ���������� ������ �������
	lc->ExecFlag 	= false;							// ���� ������������ ������ �������
	lc->WriteFlag 	= false;							// ���� ���������� ������ �������
	lc->Time		= &g_Ram.ramGroupB.DevTime.all;		// ��������� �� �����
	lc->Date		= &g_Ram.ramGroupB.DevDate.all;		// ��������� �� ����
	lc->Seconds		= &g_Ram.ramGroupH.Seconds;			// ��������� �� �������
	lc->StatusReg	= &g_Ram.ramGroupA.Status.all;		// ��������� �� ��������� ������� �����
	lc->StatDigOut	= &g_Ram.ramGroupA.StateTs.all;		// ��������� �� ������� ���������� �������
	lc->CmdReg		= 0;								// ������� ������
	lc->CmdRegPrev	= 0;								// ���������� �������� �������� ������
	lc->Position	= &g_Ram.ramGroupH.PosFix;
	memset(&lc->Data[0],	0, sizeof(LOG_CMD_BUF_DATA_COUNT));	// ����� ������� ������

}
//---------------------------------------------------
void InitLogParam(TLogParam *lp)
{

	lp->Enable		= false;							// ���� ���������� ������ �������
	lp->ExecFlag 	= false;							// ���� ������������ ������ �������
	lp->WriteFlag	= false;							// ���� ���������� ������
	lp->Time		= &g_Ram.ramGroupB.DevTime.all;		// ��������� �� �����
	lp->Date		= &g_Ram.ramGroupB.DevDate.all;		// ��������� �� ����
	lp->Seconds		= &g_Ram.ramGroupH.Seconds;			// ��������� �� �������
	lp->Addr		= 0;								// ����� ���������
	lp->NewValue	= 0;								// ����� �������� ���������
	lp->MbIndex		= 0;

	memset(&lp->Data[0],		0, sizeof(LOG_PARAM_BUF_DATA_COUNT)); 	// ����� �������
	memset(&lp->MbBuffer[0],	0, sizeof(MB_PARAM_BUF_COUNT));			// ����� ���������� ������
}
//---------------------------------------------------
//ma LogSim
void InitLogSim(TLogSim *ls)
{
	ls->Enable 		= false;							// ���� ���������� ������ �������
	ls->ExecFlag 	= false;							// ���� ������������ ������ �������
	ls->WriteFlag 	= false;							// ���� ���������� ������ �������
	ls->Time		= &g_Ram.ramGroupB.DevTime.all;		// ��������� �� �����
	ls->Date		= &g_Ram.ramGroupB.DevDate.all;		// ��������� �� ����
	ls->Seconds		= &g_Ram.ramGroupH.Seconds;			// ��������� �� �������								// ������� ������
	memset(&ls->NewSimID[0],	0, sizeof(ls->NewSimID)); 	// ����� �������
	memset(&ls->Data[0],	    0, sizeof(LOG_SIM_BUF_DATA_COUNT));	// ����� ������� ������

}
//---------------------------------------------------
// ���������� �������
void Stat_Update(TStat *p)	// 50 ��
{
	
}
//---------------------------------------------------
// ������ � Flash
/*Bool ReadWriteClearFlash(pFM25V10 flash, Byte func, Uns addr, Uns *pData, Uns count)
{
	Byte currentSector, prevSector;
	LgUns flashLAddr;
		
//	static LgUns FlashLAddr = 0;

	// �������� �� ��������� ����
//	if ((g_Flash.Func)||(Count < 1)||(FlashLAddr)) return;
	if ((flash->func)||(flash->error)||(count < 1)) return false;

	flashLAddr = addr << 1;

	// ���� ������, ����������� �� ������� �� � ����� ������
	if (func & F_WRITE)
	{
		prevSector = AT25DF041A_GetSector4KB(flashLAddr); // ��
		// ������� ������ �� ���� ��� �������
		currentSector = AT25DF041A_GetSector4KB(flashLAddr + (EVLOG_DATA_CNT << 1));
		// ������� ����� ������ ����� �������
		if (currentSector != prevSector)
		{	
			SPI_init(flash->spiId, SPI_MASTER, 0, flash->spiBaud, 8);
			AT25DF041A_EraseSector(flash,flashLAddr + (EVLOG_DATA_CNT << 1),4);
			AT25DF041A_WaitBusy(flash);
		}
	}
	flash->addr = flashLAddr;
	flash->pBuffer = pData;
	flash->func   = func;
	flash->retryTimer = 0;
	// ���� ������� ������ ��� ��� ����, �� ��������� ���������� ���� �� �����
	if (func & (F_ERASE_4KB|F_ERASE_ALL))
		flash->count = count;
	else flash->count = count << 1;

	return true;
	
//	FlashLAddr = 0; // ��� ���������� ��������� ����� ������


}*/
//---------------------------------------------------
// ������ � Eeprom
Bool ReadWriteEeprom(pFM25V10 eeprom, Byte func, Uns addr, Uns *pData, Uns count)
{
	unsigned long CastAddr = (unsigned long)addr;

	if ((eeprom->Func)||(eeprom->Error)||(count < 1)) return false;

	eeprom->Addr	= CastAddr << 1;	// �������� ����� (�������� �� 2), �.�. ������ � ��������� 2 �����, � � ������ ������� ��������
	eeprom->Buffer	= pData;			// ��������� �� ������
	eeprom->Count	= count << 1;
	eeprom->Func	= func;

	return true;
}
//----------------------------------------------------
// -- ������ � �������� ------------------------------

void DataBufferPre(TStat *s)
{
	static Uns PreTimer = 0;

	if (g_Ram.ramGroupH.LogReset > 0)							// �������� ������ � ������ ��������
	{
		g_Core.VlvDrvCtrl.EvLog.Value = CMD_CLR_LOG;
		g_Ram.ramGroupH.LogReset 	  = 0;
		g_Ram.ramGroupH.LogEvAddr 	  = 0;
		g_Ram.ramGroupH.LogCmdAddr    = 0;
		g_Ram.ramGroupH.LogParamAddr  = 0;
		g_Ram.ramGroupH.LogSimAddr    = 0; //ma LogSIM
		g_Ram.ramGroupH.LogEvCount    = 0;
		g_Ram.ramGroupH.LogCmdCount   = 0;
		g_Ram.ramGroupH.LogParamCount = 0;
		g_Ram.ramGroupH.LogSimCount = 0; //ma LogSIM
	}

	if ((++PreTimer >= 200) && !s->LogEvent.EventFlag)						// ������ �� �������
	{
		PreTimer = 0;

		s->LogEventBuffer[0] = s->LogEventBuffer[1];
		s->LogEventBuffer[1] = s->LogEventBuffer[2];
		s->LogEventBuffer[2] = s->LogEventBuffer[3];
		s->LogEventBuffer[3] = s->LogEventBuffer[4];

		s->LogEventBuffer[4].LogStatus 	 	= g_Ram.ramGroupA.Status;
		s->LogEventBuffer[4].LogPositionPr	= g_Ram.ramGroupA.PositionPr;
		s->LogEventBuffer[4].LogTorque	 	= g_Ram.ramGroupA.Torque;
		s->LogEventBuffer[4].LogUr		 	= g_Ram.ramGroupA.Ur;
		s->LogEventBuffer[4].LogUs		 	= g_Ram.ramGroupA.Us;
		s->LogEventBuffer[4].LogUt		 	= g_Ram.ramGroupA.Ut;
		s->LogEventBuffer[4].LogIu		 	= g_Ram.ramGroupA.Iu;
		s->LogEventBuffer[4].LogIv		 	= g_Ram.ramGroupA.Iv;
		s->LogEventBuffer[4].LogIw		 	= g_Ram.ramGroupA.Iw;
		s->LogEventBuffer[4].LogTemper	 	= g_Ram.ramGroupA.TemperBKP;
		s->LogEventBuffer[4].LogInputs	 	= g_Ram.ramGroupA.StateTu.all;
		s->LogEventBuffer[4].LogOutputs 	= g_Ram.ramGroupA.StateTs.all;
	}
}

void LogEvControl(TStat *s)
{
	static Uns Timer = (Uns)600;
	static Uns Addr;
	//static Uns MemFlag = false;

/*
	if (IsMemParReady())
	{
		GrH->FaultsNet.all = 0;
		WritePar(GetAdr(GroupH.FaultsNet.all), GrH->FaultsNet.all, 1);
	}
*/

	if (Timer > 0) Timer--;													// �������� ������ �������

	s->LogEvent.Enable = !Timer;

	//g_Ram.ramGroupA.Faults.Dev.bit.LowPower = 1;//!PowerEnable;		// ???						// ���������� ��� �����, �.�. �� ����� ������� �� ������, � �� ���� �� ����� ������ ��� ������

	s->LogEvent.FaultsState[0] = g_Ram.ramGroupA.Faults.Proc.all & PROCESS_EVLOG_MASK;		// �������� �������� ��������� �������
	s->LogEvent.FaultsState[1] = g_Ram.ramGroupA.Faults.Net.all  & NET_EVLOG_MASK;
	s->LogEvent.FaultsState[2] = g_Ram.ramGroupA.Faults.Load.all & LOAD_EVLOG_MASK;
	s->LogEvent.FaultsState[3] = g_Ram.ramGroupA.Faults.Dev.all  & DEV_EVLOG_MASK;

//--------------------------------------------------------------------------------
	LogEventUpdate(&s->LogEvent);													// �������� ������� ������������ ������ ������ ������� �������

	//g_Ram.ramGroupA.Faults.Dev.bit.LowPower = 0;	//???										// ���������� ���, ����� �� ������������ ��� �����
//--------------------------------------------------------------------------------
	if (IsMemParReady())
	{
		if (g_Ram.ramGroupH.LogEvAddr != PrevLogAddr[PREV_LEV_INDEX])					// ��������� ������� ����� ������ ������ � ����������
		{
			WritePar(GetAdr(ramGroupH.LogEvAddr), &g_Ram.ramGroupH.LogEvAddr, 1);			// ���������� ����� ����� ������ ������ � ������
			PrevLogAddr[PREV_LEV_INDEX] = g_Ram.ramGroupH.LogEvAddr;					// ���������� �����
		}
		else if (g_Ram.ramGroupH.LogEvCount != PrevLogCount[PREV_LEV_INDEX])
		{
			WritePar(GetAdr(ramGroupH.LogEvCount), &g_Ram.ramGroupH.LogEvCount, 1);
			PrevLogCount[PREV_LEV_INDEX] = g_Ram.ramGroupH.LogEvCount;
		}
		else if (s->LogEvent.WriteFlag)													// ��������� ���������� �������� ������ � ���� ���������� ������
		{
			Addr = LOG_EV_START_ADDR + g_Ram.ramGroupH.LogEvAddr * LOG_EV_DATA_CNT * LOG_EV_DATA_CELL;		// ��������� �����
			WritePar(Addr, s->LogEvent.Data, LOG_EV_DATA_CNT);										// ���������� ��������� � ������ ������������� �������

			LogEvMainDataFlag = true;
			LogEvBufCurIndex = LOG_EV_BUF_CELL_COUNT;
			s->LogEvent.WriteFlag = false;															// ���������� ���� ���������� ������. �� ����� ����� ������������ ������ ��������� � ������ �������

			// ����� ������ �������� ������, ��������� ��������� ����� ������
			Addr = LOG_EV_BUF_START_ADDR + g_Ram.ramGroupH.LogEvAddr * LOG_EV_BUF_DATA_CNT * LOG_EV_BUF_DATA_CELL;
			return;
		}

	}

	if ((IsMemLogReady()) && (LogEvMainDataFlag))
	{
		if (LogEvBufCurIndex > 0)
		{
			WriteLog(Addr, ToUnsPtr(&s->LogEventBuffer[LogEvBufCurIndex - 1]), LOG_EV_BUF_DATA_CNT);		// ����� � ������

			LogEvBufCurIndex--;

			Addr += LOG_EV_BUF_DATA_CNT;							// ����������� ����� �� ���������� ���������� �� ������ ������
		}

		if (LogEvBufCurIndex == 0)					// ���� ������� ����������, ������ �� ����� �� ������ ���������� (5-� �������)
		{
			LogEvMainDataFlag = false;							// �������� ��� ������ �� ������ ���������� ���� ������ ��������� ������� (0-� �������)
			if (++g_Ram.ramGroupH.LogEvAddr >= LOG_EV_CNT)					// �������������� ��������� ����� ������ � ��������� ��� ����� �� ������� ������������ �������
				g_Ram.ramGroupH.LogEvAddr = 0;								// ���� ����� �� ������������ �������, �� ����������� ����� ������ �����

			if (g_Ram.ramGroupH.LogEvCount < LOG_EV_CNT)					// ���� �� ��������� ������������ ����� �������
				g_Ram.ramGroupH.LogEvCount++;								// �������������� ����� ������� �������� ������

			s->LogEvent.EventFlag = false;							// ���������� �������� ���� �������, ����� �������� ������ ������ ��������� 5-���. �����
		}
	}
}

void GetCurrentCmd(TStat *s)
{
	TBurCmd LogControlWord = bcmNone;
	static Uns PrevEvLogValue = 0;
	static Bool FirstCmd = true;

	if (g_Core.VlvDrvCtrl.EvLog.Value != 0)
		LogControlWord = bcmNone;

	// �������� ������������ ������� ����, �������, �������
	if (g_Core.VlvDrvCtrl.EvLog.Value <= CMD_OPEN)
	{
		if (g_Core.VlvDrvCtrl.EvLog.Value == PrevEvLogValue)
		{
			g_Core.VlvDrvCtrl.EvLog.Value = 0;
			return;
		}
	}

	switch(g_Core.VlvDrvCtrl.EvLog.Value)
	{
		case CMD_STOP: 			LogControlWord = bcmStop;				break;
		case CMD_CLOSE: 		LogControlWord = bcmClose;				break;
		case CMD_OPEN: 			LogControlWord = bcmOpen;				break;
		case CMD_DEFAULTS_USER: LogControlWord = bcmSetDefaultsUser; 	break;
		case CMD_RES_CLB:		LogControlWord = bcmCalibReset;			break;
		case CMD_RES_FAULT: 	LogControlWord = bcmPrtReset;			break;
		case CMD_CLR_LOG: 		LogControlWord = bcmLogClear; 			break;
		case CMD_RES_CYCLE:		LogControlWord = bcmCycleReset;			break;
		case CMD_DEFAULTS_FACT: LogControlWord = bcmSetDefaultsFact;	break;
		case CMD_DEFSTOP:		LogControlWord = bcmDefStop;	g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_BLOCK;	break;
		case CDM_DISCROUT_TEST: LogControlWord = bcmDiscrOutTest;		break;
		case CMD_DISCRIN_TEST: 	LogControlWord = bcmDiscrInTest; 		break;
		case CMD_ON_BLT: 		LogControlWord = bcmBlueOn; 			break;
		case CMD_OFF_BLT: 		LogControlWord = bcmBlueOff; 			break;
		case CMD_FIX_POS:	    LogControlWord = bcmFixPos; 			break;
		default: LogControlWord = bcmNone; break;
	}

	if (g_Core.VlvDrvCtrl.EvLog.Value)
	{
		PrevEvLogValue = g_Core.VlvDrvCtrl.EvLog.Value;

		if (FirstCmd)
		{
			FirstCmd = false;

			// ���� ����� ������ �� ������ - ����, �� ������� ��
			if (g_Core.VlvDrvCtrl.EvLog.Value == CMD_STOP)
				LogControlWord = bcmNone;
		}
		if (!g_Core.VlvDrvCtrl.EvLog.Source) g_Core.VlvDrvCtrl.EvLog.Source = CMD_SRC_BLOCK;

		LogControlWord = LogControlWord | g_Core.VlvDrvCtrl.EvLog.Source;
		g_Core.VlvDrvCtrl.EvLog.Source = 0;
	}
	//g_Core.VlvDrvCtrl.EvLog.Value = 0;


	if (LogControlWord != bcmNone)
	{
		g_Core.VlvDrvCtrl.EvLog.Value = 0;
	}

	s->LogCmd.CmdReg = LogControlWord;
}

void LogCmdControl(TStat *s)
{
	static Uns Timer = 600;//(Uns)LOG_START_TOUT;
	Uns Addr;

	if (Timer > 0)	Timer--;

	s->LogCmd.Enable = !Timer;

//--------------------------------------------------------------------------------
	LogCmdUpdate(&s->LogCmd);																// �������� ������� ������������ �������
//--------------------------------------------------------------------------------
	// ���������� ����� ������ �������
	if (IsMemParReady())
	{
		if (g_Ram.ramGroupH.LogCmdAddr != PrevLogAddr[PREV_LCMD_INDEX])							// ��������� ���������� ���
		{																				// ��������� ��������� ������ ������� ������ �������
			WritePar(GetAdr(ramGroupH.LogCmdAddr), &g_Ram.ramGroupH.LogCmdAddr, 1);					// ���������� ����� ����� � ���
			PrevLogAddr[PREV_LCMD_INDEX] = g_Ram.ramGroupH.LogCmdAddr;								// ��������� ���������� �����
		}
	//--------------------------------------------------------------------------------
		else if (s->LogCmd.WriteFlag)														// ��������� ���������� ���
		{																				// ��������� ������� ����� ���������� ������ �������

			Addr = LOG_CMD_START_ADDR + g_Ram.ramGroupH.LogCmdAddr * LOG_CMD_DATA_CNT;				// ��������� ��������� ����� ������
			WritePar(Addr, s->LogCmd.Data, LOG_CMD_DATA_CNT);								// ���������� �� ������ �������� Eeprom1

			s->LogCmd.WriteFlag = false;													// ���������� ���� ���������� ������ �������

			if (g_Ram.ramGroupH.LogCmdCount < LOG_CMD_CNT)											// ���� �� ��������� ������������ ����� �������
				g_Ram.ramGroupH.LogCmdCount++;														// �������������� ����� ������� �������� ������

			if (++g_Ram.ramGroupH.LogCmdAddr >= LOG_CMD_CNT)										// �������������� ����� ������� ������ �������
				g_Ram.ramGroupH.LogCmdAddr = 0;													// ��������� ����� �� ������� (��������� ���������� ���������� �������). ���� ��������� - ��������
		}
		else if (g_Ram.ramGroupH.LogCmdCount != PrevLogCount[PREV_LCMD_INDEX])
		{
			WritePar(GetAdr(ramGroupH.LogCmdCount), &g_Ram.ramGroupH.LogCmdCount, 1);
			PrevLogCount[PREV_LCMD_INDEX] = g_Ram.ramGroupH.LogCmdCount;
		}
	}
}

void LogParamMbExec(TStat *s)
{
	Uns *Data = (Uns *)&g_Ram;
	static Uns LogParamIndex = 0;

// ��������� ������, ����� ��� ������ ������������ � ������, ����� �� ���� ��������� � �������
// ����������� ��������� ����������. ��� ��, ���� ������ �� �������, �.�. ����� ��������� �� ������ ����
	if ((s->LogParam.ExecFlag) || (!s->LogParam.MbIndex) || (s->LogParam.WriteFlag))
		{	return;	}

	s->LogParam.Addr 	  = s->LogParam.MbBuffer[LogParamIndex];
	s->LogParam.NewValue = Data[s->LogParam.Addr];
	s->LogParam.ExecFlag = true;

	if (++LogParamIndex == s->LogParam.MbIndex)
	{
		LogParamIndex = 0;
		s->LogParam.MbIndex = 0;
	}
}

void LogParamControl(TStat *s)
{
	static Uns Timer = 600;//(Uns)LOG_START_TOUT;
	Uns Addr;

	if (Timer > 0)	Timer--;

	s->LogParam.Enable = !Timer;

//--------------------------------------------------------------------------------
	LogParamUpdate(&s->LogParam);															// �������� ������� ������������ �������
//--------------------------------------------------------------------------------
	if (IsMemParReady())
	{
		if (g_Ram.ramGroupH.LogParamAddr != PrevLogAddr[PREV_LPAR_INDEX])							// ��������� �������� ���������� ���
		{																				// ��������� ��������� ������ ������� ������
			WritePar(GetAdr(ramGroupH.LogParamAddr), &g_Ram.ramGroupH.LogParamAddr, 1);				// ���������� ����� ����� � ���
			PrevLogAddr[PREV_LPAR_INDEX] = g_Ram.ramGroupH.LogParamAddr; 							// ��������� ���������� ����� ������ �������
		}
	//--------------------------------------------------------------------------------
		else if (s->LogParam.WriteFlag)													// ��������� ���������� ������
		{																				// ��������� ������� ����� ���������� ������ �������
			Addr = LOG_PARAM_START_ADDR + g_Ram.ramGroupH.LogParamAddr * LOG_PARAM_DATA_CNT;		// ��������� ��������� ����� ������ � ���
			WritePar(Addr, s->LogParam.Data, LOG_PARAM_DATA_CNT);							// ���������� �������� ��� ������
			s->LogParam.WriteFlag = false; 												// ���������� ���� ���������� ������

			if (g_Ram.ramGroupH.LogParamCount < LOG_PARAM_CNT)										// ���� �� ��������� ������������ ����� �������
				g_Ram.ramGroupH.LogParamCount++;													// �������������� ����� ������� �������� ��������� ����������

			if (++g_Ram.ramGroupH.LogParamAddr >= LOG_PARAM_CNT)									// �������������� ����� ������ �������
				g_Ram.ramGroupH.LogParamAddr = 0;													// ���� �� ����� �� ������� (��������� ���������� ���������� �������), ��������
		}
	//--------------------------------------------------------------------------------
		else if (g_Ram.ramGroupH.LogParamCount != PrevLogCount[PREV_LPAR_INDEX])
		{
			WritePar(GetAdr(ramGroupH.LogParamCount), &g_Ram.ramGroupH.LogParamCount, 1);
			PrevLogCount[PREV_LPAR_INDEX] = g_Ram.ramGroupH.LogParamCount;
		}
	}
}

//ma LogSIM
//---------------------------------------------------------------------------------
void LogSimControl(TStat *s)
{
	static Uns Timer = 600;//(Uns)LOG_START_TOUT;
	Uns Addr;

	if (Timer > 0)	Timer--;

	s->LogSim.Enable = !Timer;

//--------------------------------------------------------------------------------
	LogSimUpdate(&s->LogSim);															// �������� ������� ������������ �������
//--------------------------------------------------------------------------------
	if (IsMemParReady())
	{
		if (g_Ram.ramGroupH.LogSimAddr != PrevLogAddr[PREV_LSIM_INDEX])							// ��������� �������� ���������� ���
		{																				// ��������� ��������� ������ ������� ������
			WritePar(GetAdr(ramGroupH.LogSimAddr), &g_Ram.ramGroupH.LogSimAddr, 1);				// ���������� ����� ����� � ���
			PrevLogAddr[PREV_LSIM_INDEX] = g_Ram.ramGroupH.LogSimAddr; 							// ��������� ���������� ����� ������ �������
		}
	//--------------------------------------------------------------------------------
		else if (s->LogSim.WriteFlag)													// ��������� ���������� ������
		{																				// ��������� ������� ����� ���������� ������ �������
			Addr = LOG_SIM_START_ADDR + g_Ram.ramGroupH.LogSimAddr * LOG_SIM_DATA_CNT;		// ��������� ��������� ����� ������ � ���
			WritePar(Addr, s->LogSim.Data, LOG_SIM_DATA_CNT);							// ���������� �������� ��� ������
			s->LogSim.WriteFlag = false; 												// ���������� ���� ���������� ������

			if (g_Ram.ramGroupH.LogSimCount < LOG_SIM_CNT)										// ���� �� ��������� ������������ ����� �������
				g_Ram.ramGroupH.LogSimCount++;													// �������������� ����� ������� �������� ��������� ����������

			if (++g_Ram.ramGroupH.LogSimAddr >= LOG_SIM_CNT)									// �������������� ����� ������ �������
				g_Ram.ramGroupH.LogSimAddr = 0;													// ���� �� ����� �� ������� (��������� ���������� ���������� �������), ��������
		}
	//--------------------------------------------------------------------------------
		else if (g_Ram.ramGroupH.LogSimCount != PrevLogCount[PREV_LSIM_INDEX])
		{
			WritePar(GetAdr(ramGroupH.LogSimCount), &g_Ram.ramGroupH.LogSimCount, 1);
			PrevLogCount[PREV_LSIM_INDEX] = g_Ram.ramGroupH.LogSimCount;
		}
	}
}


//---------����� �����------------------------------------
