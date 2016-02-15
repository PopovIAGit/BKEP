#ifndef IM_PARAMS_H_
#define IM_PARAMS_H_


//#include "config.h"

#define IM_FIRMWARE_VERSION		DEVICE_GROUP*1000+VERSION			// Версия ПО
#define IM_DEVICE_ID			DEVICE_ID							// Идентификатор устройства
#define IM_PARAMS_CNT			RAM_DATA_SIZE						// Количество параметров устройства
#define IM_LOGS_CNT				LOGS_CNT							// Количество журналов устройства

#define IM_LOGEV_REC_ADDR		g_Ram.ramGroupH.LogEvAddr			// Текущий адрес журнала событий
#define IM_LOGCMD_REC_ADDR		g_Ram.ramGroupH.LogCmdAddr			// Текущий адрес журнала команд
#define IM_LOGPAR_REC_ADDR		g_Ram.ramGroupH.LogParamAddr			// Текущий адрес журнала изм. параметров
//ma LogSIM
#define IM_LOGSIM_REC_ADDR		g_Ram.ramGroupH.LogSimAddr			// Текущий адрес журнала SIM


#define IM_LOGEV_REC_CNT		g_Ram.ramGroupH.LogEvCount			// Количество записей журнала событий
#define IM_LOGCMD_REC_CNT		g_Ram.ramGroupH.LogCmdCount		// Количество записей журнала команд
#define IM_LOGPAR_REC_CNT		g_Ram.ramGroupH.LogParamCount		// Количество записей журнала изм. параметров
//ma LogSIM
#define IM_LOGSIM_REC_CNT		g_Ram.ramGroupH.LogSimCount		// Количество записей журнала SimID


#define IM_LOGEV_BUF_CELL_CNT			LOG_EV_BUF_CELL_COUNT		// Количество ячеек буфера журнала событий

#define IM_LOGEV_MAIN_CELL_DATA_CNT		LOG_EV_DATA_CNT				// Количество полей в основной ячейке журнала событий
#define IM_LOGEV_BUF_CELL_DATA_CNT		LOG_EV_BUF_DATA_CNT			// Количество полей в буферной ячейке журнала событий
#define IM_LOGCMD_CELL_DATA_CNT			LOG_CMD_DATA_CNT			// Количество полей в ячейке журнала команд
#define IM_LOGPAR_CELL_DATA_CNT			LOG_PARAM_DATA_CNT			// Количетсов полей в ячейке журнала изм. параметров
//maLogSIM
#define IM_LOGSIM_CELL_DATA_CNT			LOG_SIM_DATA_CNT			// Количетсов полей в ячейке журнала SimID

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

