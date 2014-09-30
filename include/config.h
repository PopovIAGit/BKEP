/*======================================================================
��� �����:          config.h
�����:
������ �����:
���� ���������:
�������������:      TMS320F28022
��������:
�������� ������ ������������ ������� ���������� �� ����� ���
======================================================================*/

#ifndef CONFIG_
#define CONFIG_

// ���������� ���������
#define SYSCLK				150e6	// ��������� ������� (���-�� �������� � ���)
#define CLKIN				30e6	// ������� ������

#define DEF_USER_PASS		2387	// ������ ��-��������� (����������������)
#define DEF_FACT_PASS		43072	// ������ ��-��������� (���������)

// ������������ ������������ �������
#define Prd18kHZ			18000	// ������� ��������� ���������� RTOS
#define Prd9kHZ				9000	// ������������� ������ 9000 ��
#define Prd2kHZ				2000	// ������������� ������ 2000 ��
#define Prd200HZ			200		// ������������� ������ 200 ��
#define Prd50HZ				50		// ������������� ������ 50 ��
#define Prd10HZ				10		// ������������� ������ 10 ��

// �������� ������
#define TORQ_TEST			0		// ���� ������� �������
#define LOWPOW_TEST			0		// ���� ���������� ��� ������� ����������

// �������� �������� ������
//#define HSPCLK_PRESC		1			// �������� ������� HSPCLK
//#define LSPCLK_PRESC		0			// �������� ������� LSPCLK

#define HSPCLK_DIV			1
#define LSPCLK_DIV			12

#define F_HSPCLK			(SYSCLK / HSPCLK_DIV)
#define F_LSPCLK			(SYSCLK / LSPCLK_DIV)

// ������ ����������� ��������, ������� ��������� ���������� ��������� "expr"
// ���� ��������� �����, ������ ������� ��������� �� ������ �� ����� ����������
#define CONCAT2(First, Second) (First ## Second)
#define CONCAT(First, Second) CONCAT2(First, Second)
#define STATIC_ASSERT(expr) typedef char CONCAT(static_assert_failed_at_line_, __LINE__) [(expr) ? 1 : -1]

// ����� ������ ��� �������� ������
#define UART_TYPE			0
#define MCBSP_TYPE			1


// ����������� ������������ ������
//#include "g_InitHardware.h"
#include "project_Version.h"
#include "g_DeviceModification.h"		// ����������� �����
#include "csl\csl_spi.h"
#include "csl\csl_sci.h"
#include "csl\csl_mcbsp.h"
#include "chip\DSP2833x_Device.h"	// ���������� ����� ��������� ����
#include "std.h"					// ���������� ��������� ����
#include "IQmathLib.h"


#endif

