
#ifndef _PCI429_INTERRUPT_H_
#define _PCI429_INTERRUPT_H_

EVT_WDF_INTERRUPT_ISR       EvtInterruptIsr;
EVT_WDF_INTERRUPT_DPC       EvtInterruptDpc;
EVT_WDF_INTERRUPT_ENABLE    EvtInterruptEnable;
EVT_WDF_INTERRUPT_DISABLE   EvtInterruptDisable;

VOID EnableInterrupt(
	IN WDFDEVICE device,
	IN BOOLEAN bEnable);

#endif	// _PCI429_INTERRUPT_H_
