
#ifndef _PCI429_COMMON_H_
#define _PCI429_COMMON_H_

///////////////////////////////////////////////////////////////////////

#define DEBUG_FOR_VIRTUAL_DEVICE	0	// for debug with virtual device

//////////////////////////////////////////////////////////////////////////

#define PCI429_SYMLINK_FORMAT      L"\\\\?\\CRDPCI429B%03d"  // 

//! driver context extension
typedef struct _DRIVER_CONTEXT_EXTENSION
{
	ULONG				Instance;	// 1 ~ ? 

} DRIVER_CONTEXT_EXTENSION, *PDRIVER_CONTEXT_EXTENSION;

//////////////////////////////////////////////////////////////////////////

//! 
typedef struct _DEVICE_CONTEXT_EXTENSION
{
	UNICODE_STRING		DeviceName;		// 
	BOOLEAN				IsOpen;			// 

	WDFINTERRUPT		Interrupt;		// 
	WDFSPINLOCK			IoctlSpinLock;	// 
	WDFQUEUE			QueueRead;		// 
	WDFQUEUE			QueueWrite;		// 
	WDFQUEUE			QueueIoctl;		// 

	PKEVENT				PkEvent;		// 

	CIRCFIFO_OBJECT		CircFifo;		// 

	ULONG				TxChNum;
	ULONG				RxChNum;
	ULONG				RxChMask;		// rx channel mask that is use for interrupt status

	BOOLEAN				EnableLabel0[ARINC_MAX_CH_NUM];	// Enable or disable label 0x00

	//////////////////////////////////////////////////////////////////////////

	//!
	struct {
		PUCHAR			Addr;			// 
		ULONG			Length;			// 
	} Bar[PCI_TYPE0_ADDRESSES];

	PPCI9054_REGS		RegPci;
	PULONG				RegIntCsr;
	PULONG				RegIntStat;

	//////////////////////////////////////////////////////////////////////////

	LONGLONG			SysStartupTime;	// System startup time
	LONGLONG			SysStartupCounter;	// System startup counter
	LONGLONG			CpuFrequency;	// 

} DEVICE_CONTEXT_EXTENSION, *PDEVICE_CONTEXT_EXTENSION;

//////////////////////////////////////////////////////////////////////////

//! request context extension
typedef struct _REQUEST_CONTEXT_EXTENSION
{
	ULONG				Temp;

} REQUEST_CONTEXT_EXTENSION, *PREQUEST_CONTEXT_EXTENSION;

//////////////////////////////////////////////////////////////////////////

//! file context extension
typedef struct _FILE_CONTEXT_EXTENSION
{
	ULONG				Temp;

} FILE_CONTEXT_EXTENSION, *PFILE_CONTEXT_EXTENSION;

//////////////////////////////////////////////////////////////////////////

//! interrupt context extension
typedef struct _INTERRUPT_CONTEXT_EXTENSION
{
	//
	// This boolean value indicates whether Interrupt is connected.
	//
	BOOLEAN IsInterruptConnected;

	//
	// This lock is used to synchronize the file close logic and
	// the Surprise Removal logic. When a surprise remove happens,
	// the device interrupts are disabled. When this occurs, the
	// file close logic should not attempt to use the interrupt
	// object.
	//
	WDFWAITLOCK InterruptStateLock;

	// 
	// interrupt data buffer
	// 
	//CIRCULAR_BUFFER		CircularBuffer;

} INTERRUPT_CONTEXT_EXTENSION, *PINTERRUPT_CONTEXT_EXTENSION;

//! Read context extension
typedef struct _SYNC_READ_CONTEXT_EXTENSION
{
	PULONG  ReadBuffer;		// 
	ULONG   InputCount;		// 
	ULONG   OutputCount;	// 

} SYNC_READ_CONTEXT_EXTENSION, *PSYNC_READ_CONTEXT_EXTENSION;

//////////////////////////////////////////////////////////////////////////

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DRIVER_CONTEXT_EXTENSION, WdfDriverGetContext);
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT_EXTENSION, WdfDeviceGetContext);
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(REQUEST_CONTEXT_EXTENSION, WdfRequestGetContext);
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(FILE_CONTEXT_EXTENSION, WdfFileGetContext);
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(INTERRUPT_CONTEXT_EXTENSION, WdfInterruptGetContext);

//////////////////////////////////////////////////////////////////////////

#endif	// _PCI429_COMMON_H_
