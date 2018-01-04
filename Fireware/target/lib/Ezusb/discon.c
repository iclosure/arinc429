#include <EZUSB.H>
#include <EZRegs.h>
#include <Fx.h>

void EZUSB_Discon(BOOL renum)
{

   USBCS |=bmDISCON;       // added for FX first silicon keeper problem
   WRITEDELAY();           // allow for register write recovery time
   USBCS &=~bmDISCOE;      // Tristate the disconnect pin
   WRITEDELAY();           // allow for register write recovery time
   USBCS |=bmDISCON;       // Set this high to disconnect USB
   if(renum)               // If renumerate
      USBCS |=bmRENUM;     // then set renumerate bit
		
   EZUSB_Delay(1500);      // Wait 1500 ms  (SHK -- If you don't wait this long, some hubs and hosts will not see the disconnect.)

   if (CPUCS & CPUCS_48MHZ)
      EZUSB_Delay(1500);   // Wait 1500 ms  (SHK -- If you don't wait this long, some hubs and hosts will not see the disconnect.)

   USBIRQ = 0xff;          // Clear any pending USB interrupt requests.  They're for our old life.
   IN07IRQ = 0xff;         // clear old activity
   OUT07IRQ = 0xff;
   EZUSB_IRQ_CLEAR();

   USBCS &=~bmDISCON;      // reconnect USB
   WRITEDELAY();           // allow for register write recovery time
   USBCS |=bmDISCOE;       // Release Tristated disconnect pin
}

