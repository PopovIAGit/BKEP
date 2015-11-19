/*======================================================================
Имя файла:          g_InitHardware.h
Автор:
Версия файла:
Дата изменения:
Применяемость:      Совместно с библиотекой IQmath
Описание:
======================================================================*/

#ifndef INITHARDWARE_
#define INITHARDWARE_

//#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

//--- ENCODER ----------------------------------------
/*
#define ECN_DATA0			GpioDataRegs.GPADAT.bit.GPIO0
#define ECN_DATA1			GpioDataRegs.GPADAT.bit.GPIO1
#define ECN_DATA2			GpioDataRegs.GPADAT.bit.GPIO2

#define ECN_ADDR0			GpioDataRegs.GPADAT.bit.GPIO3
#define ECN_ADDR1			GpioDataRegs.GPADAT.bit.GPIO4
#define ECN_ADDR2			GpioDataRegs.GPADAT.bit.GPIO5
*/
//--- LED's ------------------------------------------
#define LED_OPEN			GpioDataRegs.GPADAT.bit.GPIO22
#define LED_MUFTA			GpioDataRegs.GPADAT.bit.GPIO23
#define LED_DEFECT			GpioDataRegs.GPADAT.bit.GPIO24
#define LED_CLOSE		    GpioDataRegs.GPADAT.bit.GPIO25
#define LED_CONNECT			GpioDataRegs.GPADAT.bit.GPIO26
#define ENABLE_BLUETOOTH    GpioDataRegs.GPADAT.bit.GPIO27
#define WORK_DSP_LED		GpioDataRegs.GPADAT.bit.GPIO29
#define LED_FAULT			GpioDataRegs.GPBDAT.bit.GPIO34
#define LED_MUDU		    GpioDataRegs.GPCDAT.bit.GPIO64
#define LED_MPZ			    GpioDataRegs.GPCDAT.bit.GPIO65
#define LED_MPO			    GpioDataRegs.GPCDAT.bit.GPIO66


//----Display----------------------------------

#define BCD_A				GpioDataRegs.GPADAT.bit.GPIO1
#define BCD_B				GpioDataRegs.GPADAT.bit.GPIO2
#define BCD_C				GpioDataRegs.GPADAT.bit.GPIO3
#define BCD_D				GpioDataRegs.GPADAT.bit.GPIO4

#define BCD_FIX				GpioDataRegs.GPADAT.bit.GPIO5

#define DIG1				GpioDataRegs.GPADAT.bit.GPIO7
#define DIG2				GpioDataRegs.GPADAT.bit.GPIO8

//--- MODBUS -----------------------------------------

#define RS485_DIR_BKD		GpioDataRegs.GPBDAT.bit.GPIO52
#define RS485_DIR_ASU		GpioDataRegs.GPBDAT.bit.GPIO51
#define RS485_DIR_SHN		GpioDataRegs.GPBDAT.bit.GPIO50
#define RS485_DIR_BT		GpioDataRegs.GPBDAT.bit.GPIO53

//----CONTACTORS -------------------------------------

#define CONTACTOR_1_STATUS GpioDataRegs.GPBDAT.bit.GPIO44
#define CONTACTOR_2_STATUS GpioDataRegs.GPBDAT.bit.GPIO45

#define CONTACTOR_1_CONTROL  GpioDataRegs.GPADAT.bit.GPIO10
#define CONTACTOR_2_CONTROL  GpioDataRegs.GPADAT.bit.GPIO11

//--- BUTTON's ---------------------------------------
#define TY_24				GpioDataRegs.GPBDAT.bit.GPIO40 // 24 или 220 ТУ

#define BUTTON_BLUE			GpioDataRegs.GPBDAT.bit.GPIO46 // активация bluetooth канала

//--- SENSORS ---------------------------------------- PIA 03.04.14

#define ADC_IU				AdcRegs.ADCRESULT0
#define ADC_IV				AdcRegs.ADCRESULT1
#define ADC_IW				AdcRegs.ADCRESULT2

#define ADC_UR				AdcRegs.ADCRESULT3
#define ADC_US				AdcRegs.ADCRESULT4
#define ADC_UT				AdcRegs.ADCRESULT5

#define TU_SIG_OPEN			AdcRegs.ADCRESULT8
#define TU_SIG_CLOSE		AdcRegs.ADCRESULT9
#define TU_SIG_STOP_OPEN	AdcRegs.ADCRESULT10
#define TU_SIG_STOP_CLOSE	AdcRegs.ADCRESULT11
#define TU_SIG_MU			AdcRegs.ADCRESULT12
#define TU_SIG_DU			AdcRegs.ADCRESULT13
#define TU_SIG_RESET_ALARM	AdcRegs.ADCRESULT14
#define TU_SIG_RZVD			AdcRegs.ADCRESULT15

#define CUR_SENSOR_GAIN GpioDataRegs.GPCDAT.bit.GPIO69  //0 - большие тока. 1 - малые токи

//--- FRAM --------------------------------------------
#define SC_EEPROM1 			GpioDataRegs.GPCDAT.bit.GPIO79
#define SC_EEPROM2 			GpioDataRegs.GPCDAT.bit.GPIO78

//----- Shift Reg 74hc595 (управление ТС)-------------------------------

#define ENB_RELE GpioDataRegs.GPADAT.bit.GPIO0
#define CS_RELE  GpioDataRegs.GPADAT.bit.GPIO9
//----------------------------------------
// Toggle GPIO32 once per 500 milliseconds
//----------------------------------------
//---system config------------
#define ADC_INT_ENABLE		1
#define CPU0_INT_ENABLE		1
#define SCIRX_INT_ENABLE	1
#define SCITX_INT_ENABLE	1
//---common ------------------
#define GPIO_ON				0
#define GPIO_OFF			1

extern void InitHardware(void);
extern interrupt void SciaRxIsrHandler(void);
extern interrupt void SciaTxIsrHandler(void);

#ifdef __cplusplus
}
#endif // extern "C"

#endif
