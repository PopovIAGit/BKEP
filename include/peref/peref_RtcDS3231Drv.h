
#ifndef __DRV_DS3231__
#define __DRV_DS3231__

typedef struct DS3231 {
	char *Data;
	int   Slave;
	int   Flag;
	int   Busy;
	int   Error;
} DS3231;

void DS3231_Init(DS3231 *);
void DS3231_Update(DS3231 *);

#endif




