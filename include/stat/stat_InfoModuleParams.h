#ifndef IM_PARAMS_H_
#define IM_PARAMS_H_


//#include "config.h"

#define IM_FIRMWARE_VERSION		DEVICE_GROUP*1000+VERSION			// ������ ��
#define IM_DEVICE_ID			DEVICE_ID							// ������������� ����������
#define IM_PARAMS_CNT			RAM_DATA_SIZE						// ���������� ���������� ����������
#define IM_LOGS_CNT				LOGS_CNT							// ���������� �������� ����������

#define IM_LOGEV_REC_ADDR		g_Ram.ramGroupH.LogEvAddr			// ������� ����� ������� �������
#define IM_LOGCMD_REC_ADDR		g_Ram.ramGroupH.LogCmdAddr			// ������� ����� ������� ������
#define IM_LOGPAR_REC_ADDR		g_Ram.ramGroupH.LogParamAddr			// ������� ����� ������� ���. ����������
//ma LogSIM
#define IM_LOGSIM_REC_ADDR		g_Ram.ramGroupH.LogSimAddr			// ������� ����� ������� SIM


#define IM_LOGEV_REC_CNT		g_Ram.ramGroupH.LogEvCount			// ���������� ������� ������� �������
#define IM_LOGCMD_REC_CNT		g_Ram.ramGroupH.LogCmdCount		// ���������� ������� ������� ������
#define IM_LOGPAR_REC_CNT		g_Ram.ramGroupH.LogParamCount		// ���������� ������� ������� ���. ����������
//ma LogSIM
#define IM_LOGSIM_REC_CNT		g_Ram.ramGroupH.LogSimCount		// ���������� ������� ������� SimID


#define IM_LOGEV_BUF_CELL_CNT			LOG_EV_BUF_CELL_COUNT		// ���������� ����� ������ ������� �������

#define IM_LOGEV_MAIN_CELL_DATA_CNT		LOG_EV_DATA_CNT				// ���������� ����� � �������� ������ ������� �������
#define IM_LOGEV_BUF_CELL_DATA_CNT		LOG_EV_BUF_DATA_CNT			// ���������� ����� � �������� ������ ������� �������
#define IM_LOGCMD_CELL_DATA_CNT			LOG_CMD_DATA_CNT			// ���������� ����� � ������ ������� ������
#define IM_LOGPAR_CELL_DATA_CNT			LOG_PARAM_DATA_CNT			// ���������� ����� � ������ ������� ���. ����������
//maLogSIM
#define IM_LOGSIM_CELL_DATA_CNT			LOG_SIM_DATA_CNT			// ���������� ����� � ������ ������� SimID

#define IM_DOWNLOAD_BUFFER_SIZE			IM_READ_BUF_SIZE

#define IM_PARAM_DOWNLOAD_NUM			20

#define NEW_PARAM_ADDR					60000
#define NEW_PARAM_VALUE_ADDR			60001
#define FIRST_RECORD_FLAG_ADDR			60002

#define WORD_0_SIMID					60003
#define WORD_1_SIMID					60004
#define WORD_2_SIMID					60005
#define WORD_3_SIMID					60006
#define WORD_4_SIMID					60007
#define WORD_5_SIMID					60008
#define WORD_6_SIMID					60009
#define WORD_7_SIMID					60010
#define WORD_8_SIMID					60011
#define WORD_9_SIMID					60012


//Uns *DeviceDataPtr 			= ((Uns *)&Ram);


extern Uns ImEvLogMainAddrsTable[20];
extern Uns ImEvLogBufAddrsTable[12];
extern Uns ImCmdLogAddrsTable[5];
extern Uns ImParamLogAddrsTable[5];
extern Uns ImSimLogAddrsTable[13];

#endif

