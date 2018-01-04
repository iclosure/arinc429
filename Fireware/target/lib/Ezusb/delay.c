#include <ezusb.h>
#include <ezregs.h>
#include <fx.h>

void EZUSB_Delay(WORD ms)
{
   //
   // if we are running at 48MHz, we need to double the delay time since
   // EZUSB_Delay1ms() assumes a 24MHz clock
   //
   if (CPUCS & CPUCS_48MHZ)
      ms = ms * 2;

	while(ms--)
		EZUSB_Delay1ms();
}
