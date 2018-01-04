
#include "precomp.h"
#include "pnp.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, EvtDeviceD0Entry)
#pragma alloc_text(PAGE, EvtDeviceD0EntryPostInterruptsEnabled)
#pragma alloc_text(PAGE, EvtDeviceD0Exit)
#pragma alloc_text(PAGE, EvtDeviceD0ExitPreInterruptsDisabled)
#pragma alloc_text(PAGE, EvtDevicePrepareHardware)
#pragma alloc_text(PAGE, EvtDeviceReleaseHardware)
#pragma alloc_text(PAGE, EvtDeviceSelfManagedIoCleanup)
#pragma alloc_text(PAGE, EvtDeviceSelfManagedIoFlush)
#pragma alloc_text(PAGE, EvtDeviceSelfManagedIoInit)
#pragma alloc_text(PAGE, EvtDeviceSelfManagedIoSuspend)
#pragma alloc_text(PAGE, EvtDeviceSelfManagedIoRestart)
#pragma alloc_text(PAGE, EvtDeviceSurpriseRemoval)
#pragma alloc_text(PAGE, EvtDeviceQueryRemove)
#pragma alloc_text(PAGE, EvtDeviceQueryStop)
#pragma alloc_text(PAGE, EvtDeviceUsageNotification)
#pragma alloc_text(PAGE, EvtDeviceRelationsQuery)
#endif

NTSTATUS EvtDeviceD0Entry(
	IN  WDFDEVICE Device,
	IN WDF_POWER_DEVICE_STATE PreviousState)
{
	NTSTATUS status = STATUS_SUCCESS;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();

	return status;
}

NTSTATUS EvtDeviceD0EntryPostInterruptsEnabled(
	IN WDFDEVICE Device,
	IN WDF_POWER_DEVICE_STATE PreviousState)
{
	NTSTATUS status = STATUS_SUCCESS;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();

	return status;
}

NTSTATUS EvtDeviceD0Exit(
	IN WDFDEVICE Device,
	IN WDF_POWER_DEVICE_STATE TargetState)
{
	NTSTATUS status = STATUS_SUCCESS;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();

	return status;
}

NTSTATUS EvtDeviceD0ExitPreInterruptsDisabled(
	IN WDFDEVICE Device,
	IN WDF_POWER_DEVICE_STATE TargetState)
{
	NTSTATUS status = STATUS_SUCCESS;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();

	return status;
}

NTSTATUS EvtDevicePrepareHardware(
	IN WDFDEVICE Device,
	IN WDFCMRESLIST ResourcesRaw,
	IN WDFCMRESLIST ResourcesTranslated)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_CONTEXT_EXTENSION deviceContext;
	PCM_PARTIAL_RESOURCE_DESCRIPTOR descriptor;
	ULONG numDesc;
	ULONG i, j = 0;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(Device);

	//////////////////////////////////////////////////////////////////////////

	RtlZeroMemory(&deviceContext->Bar, sizeof(deviceContext->Bar));

	//////////////////////////////////////////////////////////////////////////

	numDesc = WdfCmResourceListGetCount(ResourcesTranslated);

	HrKdPrint(("numDesc: %ld\n", numDesc));

	for (i = 0; i < numDesc; i++)
	{
		descriptor = WdfCmResourceListGetDescriptor(ResourcesTranslated, i);
		if (NULL == descriptor)
		{
			HrKdPrint(("WdfCmResourceListGetDescriptor() failure\n"));
			return STATUS_DEVICE_CONFIGURATION_ERROR;
		}

		switch (descriptor->Type)
		{
		case CmResourceTypePort:
		case CmResourceTypeMemory:
		{
			deviceContext->Bar[j].Addr = (PUCHAR)MmMapIoSpace(descriptor->u.Memory.Start,
				descriptor->u.Memory.Length, MmNonCached);
			if (NULL == deviceContext->Bar[j].Addr)
			{
				HrKdPrint(("MmMapIoSpace(%08I64X, %08X) Failed\n",
					descriptor->u.Memory.Start, descriptor->u.Memory.Length));
				return STATUS_UNSUCCESSFUL;
			}
			else
			{
				deviceContext->Bar[j].Length = descriptor->u.Memory.Length;
				HrKdPrint(("BAR%d -> MAP: [(%08I64X, %08I64X) --> (%08X, %08X)], size: %d\n", j,
					descriptor->u.Memory.Start.QuadPart, descriptor->u.Memory.Start.QuadPart + descriptor->u.Memory.Length,
					(ULONG)deviceContext->Bar[j].Addr, ((ULONG)deviceContext->Bar[j].Addr) + deviceContext->Bar[j].Length, deviceContext->Bar[j].Length));
				j++;
			}

			break;
		}
		case CmResourceTypeInterrupt:
		{
			break;
		}
		case CmResourceTypeDma:
		{
			break;
		}
		default:
			HrKdPrint(("Unexpected I/O resource type %d", descriptor->Type));
			break;
		}
	}

	/////////////////////////////////////////////////////////////////////////
	//! 初始化通道数

	deviceContext->RxChNum = READ_REGISTER_ULONG(ARINC_BUS_REG_ADDR(deviceContext, ARINC_USER_REG_RCHNUM));
	deviceContext->TxChNum = READ_REGISTER_ULONG(ARINC_BUS_REG_ADDR(deviceContext, ARINC_USER_REG_TCHNUM));

	if (deviceContext->RxChNum > ARINC_MAX_CH_NUM)		// 最大通道数允许值
	{
		HrKdPrint(("Get rx channel number failed, (rx channel number: %d)", deviceContext->RxChNum));

		for (i = 0; (i < PCI_TYPE0_ADDRESSES) && (deviceContext->Bar[i].Addr != NULL); i++)
		{
			HrKdPrint(("BAR%d: MmUnmapIoSpace(%08X, %08X), size: %d\n", i,
				(ULONG)deviceContext->Bar[i].Addr, ((ULONG)deviceContext->Bar[i].Addr) + deviceContext->Bar[i].Length,
				deviceContext->Bar[i].Length));

			MmUnmapIoSpace(deviceContext->Bar[i].Addr, deviceContext->Bar[i].Length);
			deviceContext->Bar[i].Addr = NULL;
			deviceContext->Bar[i].Length = 0;
		}

		return STATUS_UNSUCCESSFUL;
	}
	else
	{
		deviceContext->RxChMask = 1;

		for (i = 0; i < deviceContext->RxChNum; i++)
		{
			deviceContext->RxChMask *= 2;
			deviceContext->EnableLabel0[i] = TRUE;
		}

		deviceContext->RxChMask--;

		HrKdPrint(("rx channel number : %d, mask: %08X\n", deviceContext->RxChNum, deviceContext->RxChMask));

		deviceContext->RegPci = (PPCI9054_REGS)deviceContext->Bar[0].Addr;
		deviceContext->RegIntCsr = (PULONG)&deviceContext->RegPci->Int_Csr;
		deviceContext->RegIntStat = ARINC_BUS_REG_ADDR(deviceContext, ARINC_USER_REG_INTSTAT);
	}

	/////////////////////////////////////////////////////////////////////////

	//! create circular buffer
	status = CircFifoCreate(&deviceContext->CircFifo, 2048*deviceContext->RxChNum, sizeof(ATOM_RDATA_INFO));
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("CircFifoCreate() failed as %08X\n", status));
		return status;
	}

	HR_PRINT_METHOD_END();

	return status;
}

NTSTATUS EvtDeviceReleaseHardware(
	IN WDFDEVICE Device,
	IN WDFCMRESLIST ResourcesTranslated)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_CONTEXT_EXTENSION deviceContext;
	ULONG i = 0;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(Device);

	//! free map space
	for (i = 0; (i < PCI_TYPE0_ADDRESSES) && (deviceContext->Bar[i].Addr != NULL); i++)
	{
		HrKdPrint(("BAR%d: MmUnmapIoSpace(%08X, %08X), size: %d\n", i,
			(ULONG)deviceContext->Bar[i].Addr, ((ULONG)deviceContext->Bar[i].Addr) + deviceContext->Bar[i].Length,
			deviceContext->Bar[i].Length));

		MmUnmapIoSpace(deviceContext->Bar[i].Addr, deviceContext->Bar[i].Length);
		deviceContext->Bar[i].Addr = NULL;
		deviceContext->Bar[i].Length = 0;
	}

	// 释放其他已分配的资源
	CircFifoFree(&deviceContext->CircFifo);

	HR_PRINT_METHOD_END();

	return status;
}

VOID EvtDeviceSelfManagedIoCleanup(
	IN WDFDEVICE Device)
{
	//NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_CONTEXT_EXTENSION deviceContext;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(Device);

	HR_PRINT_METHOD_END();
}

VOID EvtDeviceSelfManagedIoFlush(
	IN WDFDEVICE Device)
{
	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();
}

NTSTATUS EvtDeviceSelfManagedIoInit(
	IN WDFDEVICE Device)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_CONTEXT_EXTENSION deviceContext;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(Device);

	HR_PRINT_METHOD_END();

	return status;
}

NTSTATUS EvtDeviceSelfManagedIoSuspend(
	IN WDFDEVICE Device)
{
	NTSTATUS status = STATUS_SUCCESS;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();

	return status;
}

NTSTATUS EvtDeviceSelfManagedIoRestart(
	IN WDFDEVICE Device)
{
	NTSTATUS status = STATUS_SUCCESS;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();

	return status;
}

VOID EvtDeviceSurpriseRemoval(
	IN WDFDEVICE Device)
{
	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();
}

NTSTATUS EvtDeviceQueryRemove(
	IN WDFDEVICE Device)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_CONTEXT_EXTENSION deviceContext;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(Device);

	if (deviceContext->IsOpen)
	{
		return STATUS_UNSUCCESSFUL;
	}

	HR_PRINT_METHOD_END();

	return status;
}

NTSTATUS EvtDeviceQueryStop(
	IN WDFDEVICE Device)
{
	NTSTATUS status = STATUS_SUCCESS;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();

	return status;
}

VOID EvtDeviceUsageNotification(
	IN WDFDEVICE Device,
	IN WDF_SPECIAL_FILE_TYPE NotificationType,
	IN BOOLEAN IsInNotificationPath)
{
	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();
}

VOID EvtDeviceRelationsQuery(
	IN WDFDEVICE Device,
	IN DEVICE_RELATION_TYPE RelationType)
{
	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();
}
