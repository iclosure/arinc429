#include "..\inc\ezusb.h"
#include "..\inc\ezregs.h"

void EZUSB_Resume(void)
{
	if(USBCS & bmRWAKEUP)
	{
		USBCS |= bmSIGRESUME;
		EZUSB_Delay(20);
		USBCS &= ~bmSIGRESUME;
	}
}
