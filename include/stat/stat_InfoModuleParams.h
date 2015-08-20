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


//Uns *DeviceDataPtr 			= ((Uns *)&Ram);


extern Uns ImEvLogMainAddrsTable[20];
extern Uns ImEvLogBufAddrsTable[12];
extern Uns ImCmdLogAddrsTable[5];
extern Uns ImParamLogAddrsTable[5];

#endif

