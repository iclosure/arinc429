
#ifndef _PCI429_IOCTL_H_
#define _PCI429_IOCTL_H_

EVT_WDF_IO_QUEUE_CONTEXT_CLEANUP_CALLBACK EvtIoQueueContextCleanup;

EVT_WDF_IO_QUEUE_IO_READ                  EvtIoRead;
EVT_WDF_IO_QUEUE_IO_WRITE                 EvtIoWrite;
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL        EvtIoDeviceControl;

#endif	// _PCI429_IOCTL_H_
