
#include "precomp.h"
#include "ioctl.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, EvtIoRead)
#endif

VOID EvtIoRead(
	IN WDFQUEUE Queue,
	IN WDFREQUEST Request,
	IN size_t Length)
{
	ULONG status = STATUS_SUCCESS;
	PDEVICE_CONTEXT_EXTENSION deviceContext;
	PATOM_RDATA_INFO outBuffer;
	size_t outCount;
	ULONG readCount = 0;

	//PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(WdfIoQueueGetDevice(Queue));

	status = WdfRequestRetrieveOutputBuffer(Request, Length, (PVOID*)&outBuffer, &outCount);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("WdfRequestRetrieveOutputBuffer() Failed : %08X\n", status));
		WdfRequestCompleteWithInformation(Request, STATUS_UNSUCCESSFUL, 0);
		return;
	}

	outCount /= sizeof(ATOM_RDATA_INFO);

	while (outCount > 0)
	{
		if (readCount >= outCount)
		{
			break;
		}

		if (CircFifoRead(&deviceContext->CircFifo, &outBuffer[readCount]))
		{
			HrKdPrint(("ReadCount: %ld, Channel: %ld, Data: %08X, Timestamp: %I6408X", readCount,
				outBuffer[readCount].channel, outBuffer[readCount].data, outBuffer[readCount].timestamp));
			readCount++;
		}
		else
		{
			break;
		}
	}

	HrKdPrint(("ReadCount: %ld", readCount));

	WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, readCount * sizeof(ATOM_RDATA_INFO));

	HR_PRINT_METHOD_END();
}
