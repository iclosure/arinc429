
#include "precomp.h"
#include "adddev.h"
#include "interrupt.h"
#include "file.h"
#include "ioctl.h"
#include "pnp.h"
#include <devguid.h>

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, EvtDriverDeviceAdd)
#pragma alloc_text(PAGE, EvtDeviceContextCleanup)
#endif

NTSTATUS EvtDriverDeviceAdd(
	IN WDFDRIVER Driver,
	IN PWDFDEVICE_INIT DeviceInit)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDRIVER_CONTEXT_EXTENSION driverContext;
	PDEVICE_CONTEXT_EXTENSION deviceContext;
	WDFDEVICE device;
	WDF_PNPPOWER_EVENT_CALLBACKS pnpEvtCallbacks;
	WDF_DEVICE_PNP_CAPABILITIES pnpCaps;
	WDF_OBJECT_ATTRIBUTES attributes;
	WDF_FILEOBJECT_CONFIG fileConfig;
	PINTERRUPT_CONTEXT_EXTENSION interruptContext;
	WDF_INTERRUPT_CONFIG interruptConfig;
	WDF_IO_QUEUE_CONFIG queueConfig;
	DECLARE_UNICODE_STRING_SIZE(deviceName, 80);
	static ULONG instance = 0;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HrKdPrint(("instance = %d\n", instance));

	WdfDeviceInitSetIoType(DeviceInit, WdfDeviceIoDirect);
	WdfDeviceInitSetExclusive(DeviceInit, TRUE);
	WdfDeviceInitSetDeviceType(DeviceInit, FILE_DEVICE_BUS_EXTENDER);
	WdfDeviceInitSetCharacteristics(DeviceInit, FILE_DEVICE_SECURE_OPEN, TRUE);

	////////////////////////////////////////////////////////////////////

	// 
	driverContext = WdfDriverGetContext(Driver);

	// �豸����
	driverContext->Instance++;

	HrKdPrint(("Instance: %d\n", driverContext->Instance));

	// Set device name
	status = RtlUnicodeStringPrintf(&deviceName, L"\\Device\\CrdPci429B%03d", driverContext->Instance);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("RtlUnicodeStringPrintf() DeviceName failed as %08X\n", status));
		return status;
	}

	HrKdPrint(("DeviceName: %wZ\n", &deviceName));

	WdfDeviceInitAssignName(DeviceInit, &deviceName);

	////////////////////////////////////////////////////////////////////////
	// ��ʼ�����弴���¼�
#if !DEBUG_FOR_VIRTUAL_DEVICE
	WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpEvtCallbacks);
	pnpEvtCallbacks.EvtDeviceD0Entry = EvtDeviceD0Entry;
	pnpEvtCallbacks.EvtDeviceD0EntryPostInterruptsEnabled = EvtDeviceD0EntryPostInterruptsEnabled;
	pnpEvtCallbacks.EvtDeviceD0Exit = EvtDeviceD0Exit;
	pnpEvtCallbacks.EvtDeviceD0ExitPreInterruptsDisabled = EvtDeviceD0ExitPreInterruptsDisabled;
	pnpEvtCallbacks.EvtDevicePrepareHardware = EvtDevicePrepareHardware;
	pnpEvtCallbacks.EvtDeviceReleaseHardware = EvtDeviceReleaseHardware;
	pnpEvtCallbacks.EvtDeviceSelfManagedIoCleanup = EvtDeviceSelfManagedIoCleanup;
	pnpEvtCallbacks.EvtDeviceSelfManagedIoFlush = EvtDeviceSelfManagedIoFlush;
	pnpEvtCallbacks.EvtDeviceSelfManagedIoInit = EvtDeviceSelfManagedIoInit;
	pnpEvtCallbacks.EvtDeviceSelfManagedIoSuspend = EvtDeviceSelfManagedIoSuspend;
	pnpEvtCallbacks.EvtDeviceSelfManagedIoRestart = EvtDeviceSelfManagedIoRestart;
	pnpEvtCallbacks.EvtDeviceSurpriseRemoval = EvtDeviceSurpriseRemoval;
	pnpEvtCallbacks.EvtDeviceQueryRemove = EvtDeviceQueryRemove;
	pnpEvtCallbacks.EvtDeviceQueryStop = EvtDeviceQueryStop;
	pnpEvtCallbacks.EvtDeviceUsageNotification = EvtDeviceUsageNotification;
	pnpEvtCallbacks.EvtDeviceRelationsQuery = EvtDeviceRelationsQuery;
	WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpEvtCallbacks);
#endif	// !DEBUG_FOR_VIRTUAL_DEVICE

	// ��ʼ���ļ������¼�
	WDF_FILEOBJECT_CONFIG_INIT(&fileConfig, EvtDeviceFileCreate, EvtFileClose, EvtFileCleanup);
	WdfDeviceInitSetFileObjectConfig(DeviceInit, &fileConfig, WDF_NO_OBJECT_ATTRIBUTES);

	// ��ʼ���豸��չ����
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, DEVICE_CONTEXT_EXTENSION);
	attributes.EvtCleanupCallback = EvtDeviceContextCleanup;
	attributes.SynchronizationScope = WdfSynchronizationScopeDevice;

	// �����豸����
	status = WdfDeviceCreate(&DeviceInit, &attributes, &device);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("WdfDeviceCreate() failed as %08X\n", status));
		return status;
	}

	// ��ʼ�����弴�ù���
	// PnP���ԡ����������PC���쳣�γ��������豸������ϵͳ�Ͳ��ᵯ�������
	WDF_DEVICE_PNP_CAPABILITIES_INIT(&pnpCaps);
	pnpCaps.Removable = WdfTrue;
	pnpCaps.SurpriseRemovalOK = WdfTrue;
	WdfDeviceSetPnpCapabilities(device, &pnpCaps);

	// ��ȡ�豸��չ������
	deviceContext = WdfDeviceGetContext(device);
	RtlZeroMemory(deviceContext, sizeof(DEVICE_CONTEXT_EXTENSION));

	// �����жϷ�������
	WDF_INTERRUPT_CONFIG_INIT(&interruptConfig, EvtInterruptIsr, EvtInterruptDpc);
	//interruptConfig.AutomaticSerialization = WdfTrue;		//  ���л��ж�DpcForIsr
	//interruptConfig.EvtInterruptEnable = EvtInterruptEnable;
	//interruptConfig.EvtInterruptDisable = EvtInterruptDisable;
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, INTERRUPT_CONTEXT_EXTENSION); 
	status = WdfInterruptCreate(device, &interruptConfig, &attributes, &deviceContext->Interrupt);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("WdfInterruptCreate() failed as %d\n", status));
	}

	// ��������������
	WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchSequential);
	queueConfig.EvtIoRead = EvtIoRead;
	status = WdfIoQueueCreate(device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &deviceContext->QueueRead);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("QueueRead -> WdfIoQueueCreate() failed as %08X\n", status));
		return status;
	}

	status = WdfDeviceConfigureRequestDispatching(device, deviceContext->QueueRead, WdfRequestTypeRead);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("QueueRead -> WdfDeviceConfigureRequestDispatching() failed as %08X\n", status));
		return status;
	}

	// ����д��������
	WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchSequential);
	queueConfig.EvtIoWrite = EvtIoWrite;
	status = WdfIoQueueCreate(device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &deviceContext->QueueWrite);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("QueueWrite -> WdfIoQueueCreate() failed as %08X\n", status));
		return status;
	}

	status = WdfDeviceConfigureRequestDispatching(device, deviceContext->QueueWrite, WdfRequestTypeWrite);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("QueueWrite -> WdfDeviceConfigureRequestDispatching() failed as %08X\n", status));
		return status;
	}

	// ����Ĭ�϶���
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchSequential);
	queueConfig.EvtIoDeviceControl = EvtIoDeviceControl;
	status = WdfIoQueueCreate(device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &deviceContext->QueueIoctl);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("QueueIoctl -> WdfIoQueueCreate() failed as %08X\n", status));
		return status;
	}

	// ����IOCTL������
	status = WdfSpinLockCreate(WDF_NO_OBJECT_ATTRIBUTES, &deviceContext->IoctlSpinLock);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("WdfSpinLockCreate() IoctlSpinLock failed as %08X\n", status));
	}

	// �����豸�ӿڣ���Ӧ�ò���
	status = WdfDeviceCreateDeviceInterface(device, (LPGUID)&GUID_DEVCLASS_PCI429, NULL);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("WdfDeviceCreateDeviceInterface() failed as %08X\n", status));
		return status;
	}

	// �豸����
	instance++;

	HR_PRINT_METHOD_END();

	return status;
}

VOID EvtDeviceContextCleanup(
	IN WDFOBJECT Object)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDRIVER_CONTEXT_EXTENSION driverContext;
	PDEVICE_CONTEXT_EXTENSION deviceContext;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	driverContext = WdfDriverGetContext(WdfGetDriver());
	deviceContext = WdfDeviceGetContext(Object);

	// 
	driverContext->Instance--;

	HR_PRINT_METHOD_END();
}
