/*======================================================================
��� �����:          g_InitHardware.h
�����:
������ �����:
���� ���������:
�������������:      ��������� � ����������� IQmath
��������:
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
#define LED_OPEN			GpioDataRegs.GPADAT.bit.GPIO22
#define LED_MUFTA			GpioDataRegs.GPADAT.bit.GPIO23
#define LED_DEFECT			GpioDataRegs.GPADAT.bit.GPIO24
#define LED_FAULT			GpioDataRegs.GPBDAT.bit.GPIO34
#define LED_CLOSE		    GpioDataRegs.GPADAT.bit.GPIO25

#define LED_OPENING		    GpioDataRegs.GPCDAT.bit.GPIO66
#define LED_CLOSING		    GpioDataRegs.GPCDAT.bit.GPIO65
#define LED_MUDU		    GpioDataRegs.GPCDAT.bit.GPIO64

//#define LED_????		    GpioDataRegs.GPADAT.bit.GPIO25
#define LED_CONNECT			GpioDataRegs.GPADAT.bit.GPIO26

#define WORK_DSP_LED		GpioDataRegs.GPADAT.bit.GPIO29 // GpioDataRegs.GPBDAT.bit.GPIO43
#define ENABLE_BLUETOOTH    GpioDataRegs.GPADAT.bit.GPIO27
//----Function----------------------------------
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
#define TS_1	GpioDataRegs.GPADAT.bit.GPIO0
#define TS_2	GpioDataRegs.GPADAT.bit.GPIO1
#define TS_3	GpioDataRegs.GPADAT.bit.GPIO2
#define TS_4	GpioDataRegs.GPADAT.bit.GPIO3
#define TS_5	GpioDataRegs.GPADAT.bit.GPIO4
#define TS_6	GpioDataRegs.GPADAT.bit.GPIO5
#define TS_7	GpioDataRegs.GPADAT.bit.GPIO6
#define TS_8	GpioDataRegs.GPADAT.bit.GPIO7
#define TS_9	GpioDataRegs.GPADAT.bit.GPIO8
#define TS_10	GpioDataRegs.GPADAT.bit.GPIO9

#define TY_24				GpioDataRegs.GPBDAT.bit.GPIO40 // 24 ��� 220 ��
#define KEY_1				GpioDataRegs.GPBDAT.bit.GPIO41 // ������ ����� ������ (���. = 1)
#define KEY_2				GpioDataRegs.GPBDAT.bit.GPIO42 // ������ ����� ����� (���. = 1)
#define KEY_3				GpioDataRegs.GPBDAT.bit.GPIO43 // ������ ����� ���� (���. = 1)

#define BUTTON_BLUE			GpioDataRegs.GPBDAT.bit.GPIO46 // ��������� bluetooth ������

//--- SENSORS ---------------------------------------- PIA 03.04.14
#define ADC_UR				AdcRegs.ADCRESULT3
#define ADC_US				AdcRegs.ADCRESULT4
#define ADC_UT				AdcRegs.ADCRESULT5

#define ADC_IU				AdcRegs.ADCRESULT0
#define ADC_IV				AdcRegs.ADCRESULT1
#define ADC_IW				AdcRegs.ADCRESULT2

#define TU_SIG_OPEN			AdcRegs.ADCRESULT8
#define TU_SIG_CLOSE		AdcRegs.ADCRESULT9
#define TU_SIG_STOP			AdcRegs.ADCRESULT10
#define TU_SIG_MU			AdcRegs.ADCRESULT11
#define TU_SIG_RESETALARM	AdcRegs.ADCRESULT12
#define TU_SIG_READYTU		AdcRegs.ADCRESULT13
#define TU_SIG_DU			AdcRegs.ADCRESULT14
#define TU_SIG_RZVD			AdcRegs.ADCRESULT15


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
