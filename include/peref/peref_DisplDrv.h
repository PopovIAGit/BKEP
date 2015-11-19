#ifndef PEREF_DISPL_
#define PEREF_DISPL_

#include "config.h"

typedef struct _TPerefDisplay {

	Uns data;
	Uns dataHi;
	Uns dataLow;

	Uns displTimer;

}TPerefDisplay;

void DisplDrvInit(TPerefDisplay *);
void DisplDrvUpdate(TPerefDisplay *);

#endif
