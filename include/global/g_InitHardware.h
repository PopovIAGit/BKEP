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
#define ECN_DATA0			GpioDataRegs.GPADAT.bit.GPIO0
#define ECN_DATA1			GpioDataRegs.GPADAT.bit.GPIO1
#define ECN_DATA2			GpioDataRegs.GPADAT.bit.GPIO2

#define ECN_ADDR0			GpioDataRegs.GPADAT.bit.GPIO3
#define ECN_ADDR1			GpioDataRegs.GPADAT.bit.GPIO4
#define ECN_ADDR2			GpioDataRegs.GPADAT.bit.GPIO5

//--- LED's ------------------------------------------
#define LED0				GpioDataRegs.GPADAT.bit.GPIO7
#define LED1				GpioDataRegs.GPADAT.bit.GPIO12
#define LED2				GpioDataRegs.GPADAT.bit.GPIO16
#define LED3				GpioDataRegs.GPADAT.bit.GPIO17
#define LED4				GpioDataRegs.GPADAT.bit.GPIO18

//--- FUNCTION ---------------------------------------
#define WORK_DSP_LED		GpioDataRegs.GPATOGGLE.bit.GPIO29 // GpioDataRegs.GPBDAT.bit.GPIO43
#define OFF_TEN				GpioDataRegs.GPADAT.bit.GPIO6

//--- MODBUS -----------------------------------------

#define RS485_DIR_BKD		GpioDataRegs.GPBDAT.bit.GPIO52
#define RS485_DIR_ASU		GpioDataRegs.GPBDAT.bit.GPIO51
#define RS485_DIR_SHN		GpioDataRegs.GPBDAT.bit.GPIO50
#define RS485_DIR_BT		GpioDataRegs.GPBDAT.bit.GPIO53

//----CONTACTORS -------------------------------------

#define CONTACTOR_1_STATUS GpioDataRegs.GPBDAT.bit.GPIO44
#define CONTACTOR_2_STATUS GpioDataRegs.GPBDAT.bit.GPIO45

#define CONTACTOR_1_ENABLE  GpioDataRegs.GPADAT.bit.GPIO10
#define CONTACTOR_2_ENABLE  GpioDataRegs.GPADAT.bit.GPIO11

//--- BUTTON's ---------------------------------------
#define HALL_OPEN			AdcRegs.ADCRESULT0
#define HALL_STOP_MU		AdcRegs.ADCRESULT1
#define HALL_CLOSE			AdcRegs.ADCRESULT2
#define HALL_STOP_DU		AdcRegs.ADCRESULT3

//--- SENSORS ---------------------------------------- PIA 03.04.14
#define ADC_UR				AdcRegs.ADCRESULT2
#define ADC_US				AdcRegs.ADCRESULT4
#define ADC_UT				AdcRegs.ADCRESULT5

#define ADC_IU				AdcRegs.ADCRESULT0
#define ADC_IV				AdcRegs.ADCRESULT1
#define ADC_IW				AdcRegs.ADCRESULT3

//--- FRAM --------------------------------------------
#define SC_EEPROM1 			GpioDataRegs.GPCDAT.bit.GPIO79
#define SC_EEPROM2 			GpioDataRegs.GPCDAT.bit.GPIO78

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
