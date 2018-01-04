
#pragma once

#include <stdio.h>
#include "mmtimer.h"

PreciTimer _g_preciTImer;
LONGLONG _g_interval = 0;

void WINAPI mmTimerEvent(void* dwUser)
{
	LONGLONG interval = _g_preciTImer.intervalUS();
	printf("period: %6.3f ms\n", (interval - _g_interval) / 1000.);
	_g_interval = interval;
}

int test_mmtimer()
{
	printf("\n--------  begin to test mmtimer --------\n\n");

	MmTimer mmTimer;
	mmTimer.setMode(TIME_PERIODIC);
	mmTimer.setPeriod(1);
	mmTimer.setTimerEvent(mmTimerEvent, NULL);

	_g_preciTImer.initialize();
	mmTimer.start();

	while(true) { 
		if (_g_interval > 0xFFFFFUL) {
			mmTimer.stop();
			break;
		}
	}

	printf("\n--------  finish to test mmtimer --------\n\n");

	getchar();

	return 0;
}
