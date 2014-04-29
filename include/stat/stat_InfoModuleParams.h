#ifndef IM_PARAMS_H_
#define IM_PARAMS_H_


//#include "config.h"

#define IM_FIRMWARE_VERSION		VERSION					// ������ ��
#define IM_DEVICE_ID			DEVICE_ID				// ������������� ����������
#define IM_PARAMS_CNT			RAM_DATA_SIZE			// ���������� ���������� ����������
#define IM_LOGS_CNT				LOGS_CNT				// ���������� �������� ����������

#define IM_LOGEV_REC_ADDR		g_Ram.ramGroupH.LogEvAddr			// ������� ����� ������� �������
#define IM_LOGCMD_REC_ADDR		g_Ram.ramGroupH.LogCmdAddr			// ������� ����� ������� ������
#define IM_LOGPAR_REC_ADDR		g_Ram.ramGroupH.LogParamAddr			// ������� ����� ������� ���. ����������

#define IM_LOGEV_REC_CNT		g_Ram.ramGroupH.LogEvCount			// ���������� ������� ������� �������
#define IM_LOGCMD_REC_CNT		g_Ram.ramGroupH.LogCmdCount		// ���������� ������� ������� ������
#define IM_LOGPAR_REC_CNT		g_Ram.ramGroupH.LogParamCount		// ���������� ������� ������� ���. ����������

#define IM_LOGEV_BUF_CELL_CNT			LOG_EV_BUF_CELL_COUNT		// ���������� ����� ������ ������� �������

#define IM_LOGEV_MAIN_CELL_DATA_CNT		LOG_EV_DATA_CNT				// ���������� ����� � �������� ������ ������� �������
#define IM_LOGEV_BUF_CELL_DATA_CNT		LOG_EV_BUF_DATA_CNT			// ���������� ����� � �������� ������ ������� �������
#define IM_LOGCMD_CELL_DATA_CNT			LOG_CMD_DATA_CNT			// ���������� ����� � ������ ������� ������
#define IM_LOGPAR_CELL_DATA_CNT			LOG_PARAM_DATA_CNT			// ���������� ����� � ������ ������� ���. ����������

#define IM_DOWNLOAD_BUFFER_SIZE			IM_READ_BUF_SIZE

#define IM_PARAM_DOWNLOAD_NUM			20

#define NEW_PARAM_ADDR					60000
#define NEW_PARAM_VALUE_ADDR			60001
#define FIRST_RECORD_FLAG_ADDR			60002

extern Uns ImEvLogMainAddrsTable[20];
extern Uns ImEvLogBufAddrsTable[12];
extern Uns ImCmdLogAddrsTable[5];
extern Uns ImParamLogAddrsTable[5];

//Uns *DeviceDataPtr 			= ((Uns *)&Ram);
/*
Uns ImEvLogMainAddrsTable[]	= {
								GetAdr(ramGroupB.DevTime),
								GetAdr(ramGroupB.DevDate),
								GetAdr(ramGroupA.Status),
								GetAdr(ramGroupA.Faults.Proc),
								GetAdr(ramGroupA.Faults.Net),
								GetAdr(ramGroupA.Faults.Load),
								GetAdr(ramGroupA.Faults.Dev),
								GetAdr(ramGroupA.PositionPr),
								GetAdr(ramGroupA.Torque),
								GetAdr(ramGroupA.Ur),
								GetAdr(ramGroupA.Us),
								GetAdr(ramGroupA.Ut),
								GetAdr(ramGroupA.Iu),
								GetAdr(ramGroupA.Iv),
								GetAdr(ramGroupA.Iw),
								GetAdr(ramGroupA.Temper),
								GetAdr(ramGroupA.Inputs),
								GetAdr(ramGroupA.Outputs),
								GetAdr(ramGroupH.Seconds),
								FIRST_RECORD_FLAG_ADDR
};

Uns ImEvLogBufAddrsTable[] = {
								GetAdr(ramGroupA.Status),
								GetAdr(ramGroupA.PositionPr),
								GetAdr(ramGroupA.Torque),
								GetAdr(ramGroupA.Ur),
								GetAdr(ramGroupA.Us),
								GetAdr(ramGroupA.Ut),
								GetAdr(ramGroupA.Iu),
								GetAdr(ramGroupA.Iv),
								GetAdr(ramGroupA.Iw),
								GetAdr(ramGroupA.Temper),
								GetAdr(ramGroupA.Inputs),
								GetAdr(ramGroupA.Outputs)
};

Uns ImCmdLogAddrsTable[] = {
								GetAdr(ramGroupB.DevTime),
								GetAdr(ramGroupB.DevDate),
								GetAdr(ramGroupH.Seconds),
								GetAdr(ramGroupH.LogControlWord),
								GetAdr(ramGroupA.Status)
};

Uns ImParamLogAddrsTable[] = {
								GetAdr(ramGroupB.DevTime),
								GetAdr(ramGroupB.DevDate),
								GetAdr(ramGroupH.Seconds),
								NEW_PARAM_ADDR,
								NEW_PARAM_VALUE_ADDR
};*/



#endif

