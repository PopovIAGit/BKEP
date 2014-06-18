/*======================================================================
Имя файла:          modbus_crc.h
Автор:              
Версия файла:       01.00
Описание:
Модуль расчета контрольной суммы кадра
======================================================================*/

#ifndef MODBUS_CRC_
#define MODBUS_CRC_

#define INIT_CRC    0xFFFF
#define GOOD_CRC    0x0000
#define GENER_CRC   0xA001

Uns CrcTable[256];

//-------------------------------------------------------------------------------
void GenerateCrcTable(void)
{
	Uns i, j, Crc;
	for (i=0; i < 256; i++)
	{
		Crc = i;
		for (j=0; j < 8; j++)
		{
			if (Crc & 1) Crc = (Crc >> 1) ^ GENER_CRC;
			else Crc = (Crc >> 1);
		}
		CrcTable[i] = Crc;
	}
}

//-------------------------------------------------------------------------------
static Uns CalcFrameCrc(Byte *Buf, Uns Count)
{
	/*Uns Crc = INIT_CRC;
	do {Crc = (Crc >> 8) ^ CrcTable[(Crc ^ (Uns)*Buf++) & 0x00FF];}
	while (--Count);
	return Crc;*/
	Uns Crc = INIT_CRC;
	do {Crc = (Crc >> 8) ^ CrcTable[(Crc ^ (Uns)*Buf++) & 0x00FF];}
	while (--Count);
	return Crc;
}

#endif

