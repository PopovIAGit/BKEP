/*======================================================================
Имя файла:
Автор:
Версия файла:
Дата изменения:
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

//ma LogSIM 4 БЫЛО 3
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
// Инициализация журнала
void Stat_Init(TStat *s)
{
	int i=0;

	InitLogEvent(&s->LogEvent);
	InitInfoModule(&s->Im);

	memset(&s->LogEventBuffer[0],	0, sizeof(LOG_EV_BUF_CELL_COUNT));//???		// Буфер журнала событий

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
	// Инициализация основной флеш
	//memset(&g_MainFlash, 0, sizeof(TAT25DF041A));
	//g_MainFlash.CSAddr = CS_MAIN_FLASH;

	//AT25DF041A_Init(&g_MainFlash);
	// Инициализация резервной флеш
	//memset(&g_ReservFlash, 0, sizeof(TAT25DF041A));

	//AT25DF041A_Init(&g_ReservFlash);
	// Инициализация епром
	//AT25XXX_Init(&g_Eeprom);

	// Инициализация епром
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
	ImSimLogAddrsTable[3]  = WORD_0_SIMID;  //что за адрес и зачем именно такой
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
	im->PeriodIndex		= 30; //300мс

	im->CanReadNextRec	= False;
	im->IsReadRecBusy	= False;
	im->IsBufReady		= False;
	im->IsLogTransmit	= False;

	im->HardwareSrc		= imSrcBluetooth;
	im->HardwareSrc		= imSrcModbus;
	im->HardwareSrc		= imSrcNone;

	im->DeviceDataPtr   = (Uns *)&g_Ram;
	// Для информационного модуля
	im->EnableReceive	 = g_Comm.Bluetooth.EnableRx;
	im->EnableTransmit	 = g_Comm.Bluetooth.EnableTx;
	im->TransmitByte	 = g_Comm.Bluetooth.TransmitByteIM;

}
//---------------------------------------------------
void InitLogEvent(TLogEvent *le)
{
	le->Enable		= false;						// Разрешение работы журнала событий
	le->ExecFlag	= false;						// Флаг формирования буфера журнала
	le->WriteFlag	= false;						// Флаг разрешения записи журнала в память
	le->EventFlag	= false;						// Флаг возникновения события
	le->FirstEvent	= true;							// Флаг первого события после включения питания
	le->FaultsCount	= 4;							// Количество регистров неисправностей

	memset(&le->FaultsState[0],	0, sizeof(FAULTS_COUNT_DEF));		// Буфер регистров неисправностей
	memset(&le->FaultsFiltr[0],	0, sizeof(FAULTS_COUNT_DEF));		// Буфер фильтрации регистров неисправностей
	memset(&le->FaultsFiltrPrev[0],	0, sizeof(FAULTS_COUNT_DEF));	// Буфер фильтрации регистров неисправностей

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

	memset(&le->Data[0],	0, sizeof(LOG_EV_BUF_DATA_COUNT));		// Буфер журнала событий

}
//---------------------------------------------------
void InitLogCmd(TLogCmd *lc)
{
	lc->Enable 		= false;							// Флаг разрешения работы журнала
	lc->ExecFlag 	= false;							// Флаг формирования буфера журнала
	lc->WriteFlag 	= false;							// Флаг разрешения записи журнала
	lc->Time		= &g_Ram.ramGroupB.DevTime.all;		// Указатель на время
	lc->Date		= &g_Ram.ramGroupB.DevDate.all;		// Указатель на дату
	lc->Seconds		= &g_Ram.ramGroupH.Seconds;			// Указатель на секунды
	lc->StatusReg	= &g_Ram.ramGroupA.Status.all;		// Указатель на статусный регистр блока
	lc->StatDigOut	= &g_Ram.ramGroupA.StateTs.all;		// Указатель на регистр дискретных выходов
	lc->CmdReg		= 0;								// Регистр команд
	lc->CmdRegPrev	= 0;								// Предыдущее значение регистра команд
	lc->Position	= &g_Ram.ramGroupH.PosFix;
	memset(&lc->Data[0],	0, sizeof(LOG_CMD_BUF_DATA_COUNT));	// Буфер журнала команд

}
//---------------------------------------------------
void InitLogParam(TLogParam *lp)
{

	lp->Enable		= false;							// Флаг разрешения работы журнала
	lp->ExecFlag 	= false;							// Флаг формирования буфера журнала
	lp->WriteFlag	= false;							// Флаг разрешения записи
	lp->Time		= &g_Ram.ramGroupB.DevTime.all;		// Указатель на время
	lp->Date		= &g_Ram.ramGroupB.DevDate.all;		// Указатель на дату
	lp->Seconds		= &g_Ram.ramGroupH.Seconds;			// Указатель на секунды
	lp->Addr		= 0;								// Адрес параметра
	lp->NewValue	= 0;								// Новое значение параметра
	lp->MbIndex		= 0;

	memset(&lp->Data[0],		0, sizeof(LOG_PARAM_BUF_DATA_COUNT)); 	// Буфер журнала
	memset(&lp->MbBuffer[0],	0, sizeof(MB_PARAM_BUF_COUNT));			// Буфер параметров Модбас
}
//---------------------------------------------------
//ma LogSim
void InitLogSim(TLogSim *ls)
{
	ls->Enable 		= false;							// Флаг разрешения работы журнала
	ls->ExecFlag 	= false;							// Флаг формирования буфера журнала
	ls->WriteFlag 	= false;							// Флаг разрешения записи журнала
	ls->Time		= &g_Ram.ramGroupB.DevTime.all;		// Указатель на время
	ls->Date		= &g_Ram.ramGroupB.DevDate.all;		// Указатель на дату
	ls->Seconds		= &g_Ram.ramGroupH.Seconds;			// Указатель на секунды								// Регистр команд
	memset(&ls->NewSimID[0],	0, sizeof(ls->NewSimID)); 	// Буфер журнала
	memset(&ls->Data[0],	    0, sizeof(LOG_SIM_BUF_DATA_COUNT));	// Буфер журнала команд

}
//---------------------------------------------------
// Обновление журнала
void Stat_Update(TStat *p)	// 50 Гц
{
	
}
//---------------------------------------------------
// Работа с Flash
/*Bool ReadWriteClearFlash(pFM25V10 flash, Byte func, Uns addr, Uns *pData, Uns count)
{
	Byte currentSector, prevSector;
	LgUns flashLAddr;
		
//	static LgUns FlashLAddr = 0;

	// Проверка на занятость флеш
//	if ((g_Flash.Func)||(Count < 1)||(FlashLAddr)) return;
	if ((flash->func)||(flash->error)||(count < 1)) return false;

	flashLAddr = addr << 1;

	// Если запись, топроверяем не перешли ли в новый сектор
	if (func & F_WRITE)
	{
		prevSector = AT25DF041A_GetSector4KB(flashLAddr); // ИК
		// Текущий сектор во флеш для журнала
		currentSector = AT25DF041A_GetSector4KB(flashLAddr + (EVLOG_DATA_CNT << 1));
		// Очищаем новый сектор перед записью
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
	// Если стираем сектор или всю флеш, то удваивать количество байт не нужно
	if (func & (F_ERASE_4KB|F_ERASE_ALL))
		flash->count = count;
	else flash->count = count << 1;

	return true;
	
//	FlashLAddr = 0; // Для исключения ошибочной смены адреса


}*/
//---------------------------------------------------
// Работа с Eeprom
Bool ReadWriteEeprom(pFM25V10 eeprom, Byte func, Uns addr, Uns *pData, Uns count)
{
	unsigned long CastAddr = (unsigned long)addr;

	if ((eeprom->Func)||(eeprom->Error)||(count < 1)) return false;

	eeprom->Addr	= CastAddr << 1;	// Сдвигаем адрес (умножаем на 2), т.к. данные в структуре 2 байта, а в память пишется побайтно
	eeprom->Buffer	= pData;			// Указатель на данные
	eeprom->Count	= count << 1;
	eeprom->Func	= func;

	return true;
}
//----------------------------------------------------
// -- РАБОТА С ЖУРНАЛОМ ------------------------------

void DataBufferPre(TStat *s)
{
	static Uns PreTimer = 0;

	if (g_Ram.ramGroupH.LogReset > 0)							// Обнуляем записи и адреса журналов
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

	if ((++PreTimer >= 200) && !s->LogEvent.EventFlag)						// Запись по секунде
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

	if (Timer > 0) Timer--;													// Задержка работы журнала

	s->LogEvent.Enable = !Timer;

	//g_Ram.ramGroupA.Faults.Dev.bit.LowPower = 1;//!PowerEnable;		// ???						// Выставляем бит здесь, т.к. он может сказать на работе, а по сути он нужен только для записи

	s->LogEvent.FaultsState[0] = g_Ram.ramGroupA.Faults.Proc.all & PROCESS_EVLOG_MASK;		// Забираем значения регистров событий
	s->LogEvent.FaultsState[1] = g_Ram.ramGroupA.Faults.Net.all  & NET_EVLOG_MASK;
	s->LogEvent.FaultsState[2] = g_Ram.ramGroupA.Faults.Load.all & LOAD_EVLOG_MASK;
	s->LogEvent.FaultsState[3] = g_Ram.ramGroupA.Faults.Dev.all  & DEV_EVLOG_MASK;

//--------------------------------------------------------------------------------
	LogEventUpdate(&s->LogEvent);													// Вызываем функцию формирования первой ячейки журнала событий

	//g_Ram.ramGroupA.Faults.Dev.bit.LowPower = 0;	//???										// Сбрасываем бит, чтобы он отсутствовал при пуске
//--------------------------------------------------------------------------------
	if (IsMemParReady())
	{
		if (g_Ram.ramGroupH.LogEvAddr != PrevLogAddr[PREV_LEV_INDEX])					// Проверяем текущий адрес начала записи и предыдущий
		{
			WritePar(GetAdr(ramGroupH.LogEvAddr), &g_Ram.ramGroupH.LogEvAddr, 1);			// Записываем новый адрес начала записи в память
			PrevLogAddr[PREV_LEV_INDEX] = g_Ram.ramGroupH.LogEvAddr;					// Запоминаем адрес
		}
		else if (g_Ram.ramGroupH.LogEvCount != PrevLogCount[PREV_LEV_INDEX])
		{
			WritePar(GetAdr(ramGroupH.LogEvCount), &g_Ram.ramGroupH.LogEvCount, 1);
			PrevLogCount[PREV_LEV_INDEX] = g_Ram.ramGroupH.LogEvCount;
		}
		else if (s->LogEvent.WriteFlag)													// Проверяем готовность драйвера памяти и флаг разрешения записи
		{
			Addr = LOG_EV_START_ADDR + g_Ram.ramGroupH.LogEvAddr * LOG_EV_DATA_CNT * LOG_EV_DATA_CELL;		// Формируем адрес
			WritePar(Addr, s->LogEvent.Data, LOG_EV_DATA_CNT);										// Записываем параметры в момент возникновения события

			LogEvMainDataFlag = true;
			LogEvBufCurIndex = LOG_EV_BUF_CELL_COUNT;
			s->LogEvent.WriteFlag = false;															// Сбрасываем флаг разрешения записи. По этому флагу записываются только параметры в момент события

			// После записи основной ячейки, формируем начальный адрес буфера
			Addr = LOG_EV_BUF_START_ADDR + g_Ram.ramGroupH.LogEvAddr * LOG_EV_BUF_DATA_CNT * LOG_EV_BUF_DATA_CELL;
			return;
		}

	}

	if ((IsMemLogReady()) && (LogEvMainDataFlag))
	{
		if (LogEvBufCurIndex > 0)
		{
			WriteLog(Addr, ToUnsPtr(&s->LogEventBuffer[LogEvBufCurIndex - 1]), LOG_EV_BUF_DATA_CNT);		// Пишем в память

			LogEvBufCurIndex--;

			Addr += LOG_EV_BUF_DATA_CNT;							// Увеличиваем адрес на количество записанный из буфера данных
		}

		if (LogEvBufCurIndex == 0)					// Если индексы сравнялись, значит мы дошли до самого последнего (5-я секунда)
		{
			LogEvMainDataFlag = false;							// Записали все ячейки из буфера сбрасываем флаг записи основного события (0-я секунда)
			if (++g_Ram.ramGroupH.LogEvAddr >= LOG_EV_CNT)					// Инкрементируем начальный адрес записи и проверяем его выход за пределы установленой области
				g_Ram.ramGroupH.LogEvAddr = 0;								// Если вышел за установленую область, то присваиваем самый первый адрес

			if (g_Ram.ramGroupH.LogEvCount < LOG_EV_CNT)					// Если не превышено максимальное число записей
				g_Ram.ramGroupH.LogEvCount++;								// Инкрементируем число записей журанала команд

			s->LogEvent.EventFlag = false;							// Сбрасываем основной флаг события, после которого сможем заново заполнять 5-сек. буфер
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

	// Отсекаем повторяющуся команду Стоп, открыть, закрыть
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

			// Если самая первая из команд - Стоп, то убираем ее
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
	LogCmdUpdate(&s->LogCmd);																// Вызываем функцию формирования журнала
//--------------------------------------------------------------------------------
	// Записываем адрес ячейки журнала
	if (IsMemParReady())
	{
		if (g_Ram.ramGroupH.LogCmdAddr != PrevLogAddr[PREV_LCMD_INDEX])							// Проверяем готовность ПЗУ
		{																				// Проверяем изменение адреса текущей ячейки журнала
			WritePar(GetAdr(ramGroupH.LogCmdAddr), &g_Ram.ramGroupH.LogCmdAddr, 1);					// Записываем новый адрес в ПЗУ
			PrevLogAddr[PREV_LCMD_INDEX] = g_Ram.ramGroupH.LogCmdAddr;								// Обновляем предыдущий адрес
		}
	//--------------------------------------------------------------------------------
		else if (s->LogCmd.WriteFlag)														// Проверяем готовность ПЗУ
		{																				// Проверяем наличие флага разрешения записи журнала

			Addr = LOG_CMD_START_ADDR + g_Ram.ramGroupH.LogCmdAddr * LOG_CMD_DATA_CNT;				// Формируем начальный адрес записи
			WritePar(Addr, s->LogCmd.Data, LOG_CMD_DATA_CNT);								// Отправляем на запись драйверу Eeprom1

			s->LogCmd.WriteFlag = false;													// Сбрасываем флаг разрешения записи журнала

			if (g_Ram.ramGroupH.LogCmdCount < LOG_CMD_CNT)											// Если не превышено максимальное число записей
				g_Ram.ramGroupH.LogCmdCount++;														// Инкрементируем число записей журанала команд

			if (++g_Ram.ramGroupH.LogCmdAddr >= LOG_CMD_CNT)										// Инкрементируем адрес текущей ячейки журнала
				g_Ram.ramGroupH.LogCmdAddr = 0;													// Проверяем выход за пределы (превышено допустимое количество записей). Если превышено - обнуляем
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

// Формируем ячейку, когда нет флагов формирования и записи, чтобы не было конфликта с другими
// источниками изменения параметров. Так же, если индекс не нулевой, т.е. когда параметры по модбас есть
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
	LogParamUpdate(&s->LogParam);															// Вызываем функцию формирования журнала
//--------------------------------------------------------------------------------
	if (IsMemParReady())
	{
		if (g_Ram.ramGroupH.LogParamAddr != PrevLogAddr[PREV_LPAR_INDEX])							// Проверяем условаие готовности ПЗУ
		{																				// Проверяем изменение адреса текущей ячейки
			WritePar(GetAdr(ramGroupH.LogParamAddr), &g_Ram.ramGroupH.LogParamAddr, 1);				// Записываем новый адрес в ПЗУ
			PrevLogAddr[PREV_LPAR_INDEX] = g_Ram.ramGroupH.LogParamAddr; 							// Обновляем предыдущий адрес ячейки журнала
		}
	//--------------------------------------------------------------------------------
		else if (s->LogParam.WriteFlag)													// Проверяем готовность памяти
		{																				// Проверяем наличие флага разрешения записи журнала
			Addr = LOG_PARAM_START_ADDR + g_Ram.ramGroupH.LogParamAddr * LOG_PARAM_DATA_CNT;		// Формируем начальный адрес записи в ПЗУ
			WritePar(Addr, s->LogParam.Data, LOG_PARAM_DATA_CNT);							// Отправляем драйверу для записи
			s->LogParam.WriteFlag = false; 												// Сбрасываем флаг разрешения записи

			if (g_Ram.ramGroupH.LogParamCount < LOG_PARAM_CNT)										// Если не превышено максимальное число записей
				g_Ram.ramGroupH.LogParamCount++;													// Инкрементируем число записей журанала изменения параметров

			if (++g_Ram.ramGroupH.LogParamAddr >= LOG_PARAM_CNT)									// Инкрементируем адрес ячейки журнала
				g_Ram.ramGroupH.LogParamAddr = 0;													// Если он вышел за границы (превышено количество допустимых записей), обнуляем
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
	LogSimUpdate(&s->LogSim);															// Вызываем функцию формирования журнала
//--------------------------------------------------------------------------------
	if (IsMemParReady())
	{
		if (g_Ram.ramGroupH.LogSimAddr != PrevLogAddr[PREV_LSIM_INDEX])							// Проверяем условаие готовности ПЗУ
		{																				// Проверяем изменение адреса текущей ячейки
			WritePar(GetAdr(ramGroupH.LogSimAddr), &g_Ram.ramGroupH.LogSimAddr, 1);				// Записываем новый адрес в ПЗУ
			PrevLogAddr[PREV_LSIM_INDEX] = g_Ram.ramGroupH.LogSimAddr; 							// Обновляем предыдущий адрес ячейки журнала
		}
	//--------------------------------------------------------------------------------
		else if (s->LogSim.WriteFlag)													// Проверяем готовность памяти
		{																				// Проверяем наличие флага разрешения записи журнала
			Addr = LOG_SIM_START_ADDR + g_Ram.ramGroupH.LogSimAddr * LOG_SIM_DATA_CNT;		// Формируем начальный адрес записи в ПЗУ
			WritePar(Addr, s->LogSim.Data, LOG_SIM_DATA_CNT);							// Отправляем драйверу для записи
			s->LogSim.WriteFlag = false; 												// Сбрасываем флаг разрешения записи

			if (g_Ram.ramGroupH.LogSimCount < LOG_SIM_CNT)										// Если не превышено максимальное число записей
				g_Ram.ramGroupH.LogSimCount++;													// Инкрементируем число записей журанала изменения параметров

			if (++g_Ram.ramGroupH.LogSimAddr >= LOG_SIM_CNT)									// Инкрементируем адрес ячейки журнала
				g_Ram.ramGroupH.LogSimAddr = 0;													// Если он вышел за границы (превышено количество допустимых записей), обнуляем
		}
	//--------------------------------------------------------------------------------
		else if (g_Ram.ramGroupH.LogSimCount != PrevLogCount[PREV_LSIM_INDEX])
		{
			WritePar(GetAdr(ramGroupH.LogSimCount), &g_Ram.ramGroupH.LogSimCount, 1);
			PrevLogCount[PREV_LSIM_INDEX] = g_Ram.ramGroupH.LogSimCount;
		}
	}
}


//---------Конец файла------------------------------------
