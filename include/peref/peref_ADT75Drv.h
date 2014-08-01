
#ifndef __DRV_ADT75__
#define __DRV_ADT75__

typedef struct {
	int Slave;
	int Temper;
	int Error;
	int Busy;
} ADT75;


void ADT75_Init(ADT75 *);
void ADT75_Update(ADT75 *);

#endif





