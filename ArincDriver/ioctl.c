
#include "precomp.h"
#include "ioctl.h"
#include "interrupt.h"

VOID SetHardwareFifoToEmpty(
	IN WDFDEVICE Device);

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, SetHardwareFifoToEmpty)
#pragma alloc_text(PAGE, EvtIoDeviceControl)
#endif

VOID SetHardwareFifoToEmpty(
	IN WDFDEVICE Device)
{
	PDEVICE_CONTEXT_EXTENSION deviceContext;
#if DBG
	ULONG i, regVal;
#endif

	//PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(Device);

	WRITE_REGISTER_ULONG(ARINC_BUS_REG_ADDR(deviceContext, ARINC_USER_REG_RESETFIFO), 0x00);  // ������н��ն�ͨ��Ӳ��FIFO����ֹ����
	CircFifoClear(&deviceContext->CircFifo);

#if DBG
	for (i = 0; i < deviceContext->RxChNum; i++)
	{
		regVal = READ_REGISTER_ULONG(ARINC_BUS_RXREG_ADDR(deviceContext, i, ARINC_REG_STAT_ID));
		if (GET_REG_BIT(regVal, ARINC_RSTAT_FIFO_EMPTY) == 0)
		{
			HrKdPrint(("Reset RX (CH%02d) FIFO empty faield!!!\n", i));
		}
	}
#endif

	HR_PRINT_METHOD_END();
}

VOID EvtIoDeviceControl(
	IN WDFQUEUE Queue,
	IN WDFREQUEST Request,
	IN size_t OutputBufferLength,
	IN size_t InputBufferLength,
	IN ULONG IoControlCode)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_CONTEXT_EXTENSION deviceContext;
	PVOID pInBuffer = NULL, pOutBuffer = NULL;
	size_t length;
	ULONG bytesResolved = 0;

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(WdfIoQueueGetDevice(Queue));

	// ������뻺�������ڣ���ȡ�����뻺���������ж�����Ч��
	if (InputBufferLength > 0)
	{
		status = WdfRequestRetrieveInputBuffer(Request, InputBufferLength, &pInBuffer, &length);
		if ((!NT_SUCCESS(status)) || (NULL == pInBuffer) || (length < InputBufferLength))
		{
			HrKdPrint(("WdfRequestRetrieveInputBuffer() failed as %08X\n", status));
			WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
			return;
		}
	}

	// ���������������ڣ���ȡ����������������ж�����Ч��
	if (OutputBufferLength > 0)
	{
		status = WdfRequestRetrieveOutputBuffer(Request, OutputBufferLength, &pOutBuffer, &length);
		if ((!NT_SUCCESS(status)) || (NULL == pOutBuffer) || (length < OutputBufferLength))
		{
			HrKdPrint(("WdfRequestRetrieveOutputBuffer() failed as %08X\n", status));
			WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
			return;
		}
	}

	// IO������
	switch (IoControlCode)
	{
	case IOCTL_PCI429_SET_EVENT:       // �����¼�
	{
		HrKdPrint(("IOCTL_PCI429_SET_EVENT\n"));

		//! 
		status = ObReferenceObjectByHandle(*(PHANDLE)pInBuffer, SYNCHRONIZE,
			*ExEventObjectType, UserMode, (PVOID*)&deviceContext->PkEvent, NULL);
		if (!NT_SUCCESS(status))
		{
			HrKdPrint(("ObReferenceObjectByHandle() Failed : %08X\n", status));
		}
		//ObDereferenceObject(deviceContext->PkEvent);

		bytesResolved = sizeof(PHANDLE);

		break;
	}
	case IOCTL_PCI429_SET_INTERRUPT:   // �����ж�
	{
		HrKdPrint(("IOCTL_PCI429_SET_INTERRUPT\n"));

		WdfSpinLockAcquire(deviceContext->IoctlSpinLock);

		if (AR_ON == *(PULONG)pInBuffer)
		{
			SetHardwareFifoToEmpty(WdfIoQueueGetDevice(Queue));
			EnableInterrupt(WdfIoQueueGetDevice(Queue), TRUE);
			WRITE_REGISTER_ULONG(ARINC_BUS_REG_ADDR(deviceContext, ARINC_USER_REG_RESETFIFO), deviceContext->RxChMask);  // ʹ�����н��ն�ͨ����������
		}
		else
		{
			EnableInterrupt(WdfIoQueueGetDevice(Queue), FALSE);
			SetHardwareFifoToEmpty(WdfIoQueueGetDevice(Queue));
		}

		WdfSpinLockRelease(deviceContext->IoctlSpinLock);

		bytesResolved = sizeof(ULONG);

		break;
	}
	case IOCTL_PCI429_READ_REGISTER:  // ���Ĵ���
	{
		ULONG inBuffer = *(PULONG)pInBuffer;

		HrKdPrint(("IOCTL_PCI429_READ_REGISTER\n"));

		*(PULONG)pOutBuffer = READ_REGISTER_ULONG(ARINC_BUS_REG_ADDR(deviceContext, inBuffer));

		HrKdPrint(("Read register -> ADDR: %08X, VALUE: %08X\n", inBuffer, *(PULONG)pOutBuffer));

		bytesResolved = sizeof(ULONG);

		break;
	}
	case IOCTL_PCI429_WRITE_REGISTER: // д�Ĵ���
	{
		PULONG lpInBuffer = (PULONG)pInBuffer;

		HrKdPrint(("IOCTL_PCI429_WRITE_REGISTER\n"));

		HrKdPrint(("Write register -> ADDR: %08X, VALUE: %08X\n", *lpInBuffer, *(lpInBuffer + 1)));

		WRITE_REGISTER_ULONG(ARINC_BUS_REG_ADDR(deviceContext, *lpInBuffer), *(lpInBuffer + 1));

		bytesResolved = sizeof(ULONG)* 2;

		break;
	}
	case IOCTL_PCI429_SET_LABEL0:  // ʹ�ܻ��ֹ��ʶ��Ϊ0�����ݵĽ���
	{
		PULONG lpInBuffer = (PULONG)pInBuffer;

		HrKdPrint(("IOCTL_PCI429_SET_LABEL0\n"));

		WdfSpinLockAcquire(deviceContext->IoctlSpinLock);

		deviceContext->EnableLabel0[*lpInBuffer] = (BOOLEAN)(*(lpInBuffer + 1));

		WdfSpinLockRelease(deviceContext->IoctlSpinLock);

		bytesResolved = sizeof(ULONG)* 2;

		break;
	}
	default:
		HrKdPrint(("(Error) STATUS_NOT_SUPPORTED\n"));
		status = STATUS_NOT_SUPPORTED;
		break;
	}

	WdfRequestCompleteWithInformation(Request, status, bytesResolved);

	HR_PRINT_METHOD_END();
}
