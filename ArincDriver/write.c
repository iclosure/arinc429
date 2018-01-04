
#include "precomp.h"
#include "ioctl.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, EvtIoWrite)
#endif

VOID EvtIoWrite(
	IN WDFQUEUE Queue,
	IN WDFREQUEST Request,
	IN size_t Length)
{
	ULONG status = STATUS_SUCCESS;
	PDEVICE_CONTEXT_EXTENSION deviceContext;
	PULONG lpInBuffer;
	PATOM_TDATA_INFO atomInfo;
	PULONG regAddrTxStat, regAddrTxData;
	ULONG regVal;
	ULONG channel;
	size_t size;
	ULONG bytesResolved = 0;

	//PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(WdfIoQueueGetDevice(Queue));

	//! get input buffer
	status = WdfRequestRetrieveInputBuffer(Request, Length, (PVOID*)&lpInBuffer, &size);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("WdfRequestRetrieveInputBuffer() Failed: %08X\n", status));
		WdfRequestCompleteWithInformation(Request, status, 0);
		return;
	}

	if (size < sizeof(ATOM_TDATA_INFO))
	{
		HrKdPrint(("Buffer is too small!(size: %ld)\n", size));
		WdfRequestCompleteWithInformation(Request, STATUS_INVALID_PARAMETER, 0);
		return;
	}

	atomInfo = (PATOM_TDATA_INFO)lpInBuffer;

	channel = atomInfo->channel;
	regAddrTxStat = ARINC_BUS_TXREG_ADDR(deviceContext, channel, ARINC_REG_STAT_ID);
	regAddrTxData = ARINC_BUS_TXREG_ADDR(deviceContext, channel, ARINC_REG_DATA_ID);

	if (atomInfo->BufferSize == 1)  // write one atom
	{
		regVal = READ_REGISTER_ULONG(regAddrTxStat);

		HrKdPrint(("Status: CHANNEL: %d, DATA: %08X\n", channel, regVal));

		if (GET_REG_BIT(regVal, ARINC_TSTAT_FIFO_HALF))	// less than half full or programmed level
		{
			HrKdPrint(("ARS_XMITOVRFLO"));
			bytesResolved = ARS_XMITOVRFLO;
		}
		else
		{
			HrKdPrint(("Write: CHANNEL:%d, DATA: %08X\n", channel, atomInfo->Buffer[0]));
			WRITE_REGISTER_ULONG(regAddrTxData, atomInfo->Buffer[0]);
			bytesResolved = sizeof(ATOM_TDATA_INFO);

			//TEST
#ifdef TEST_ARINC429_000
			if (atomInfo->Buffer[0] == 0)
			{
				DbgPrint("#> EvtIoWrite: CHANNEL:%d, DATA: %08X\n", channel, atomInfo->Buffer[0]);
			}
#endif  // TEST_ARINC429_000
		}
	}
	else if (atomInfo->BufferSize > 1)
	{
		ULONG i;

		for (i = 0; i < atomInfo->BufferSize; i++)
		{
			regVal = READ_REGISTER_ULONG(regAddrTxStat);

			HrKdPrint(("Status: CHANNEL: %d, DATA: %08X\n", channel, regVal));

			if (GET_REG_BIT(regVal, ARINC_TSTAT_FIFO_HALF))	// less than half full or programmed level
			{
				break;
			}
			else
			{
				HrKdPrint(("Write: CHANNEL:%d, DATA: %08X\n", channel, atomInfo->Buffer[i]));
				WRITE_REGISTER_ULONG(regAddrTxData, atomInfo->Buffer[i]);
			}
		}

		bytesResolved = sizeof(ATOM_TDATA_INFO)+i - sizeof(ULONG);

		HrKdPrint(("Write Count: %ld\n", bytesResolved));
	}
	else
	{
		HrKdPrint(("BufferSize must be great than 1\n"));
		WdfRequestCompleteWithInformation(Request, STATUS_INVALID_PARAMETER, 0);
		return;
	}

	WdfRequestCompleteWithInformation(Request, status, bytesResolved);

	HR_PRINT_METHOD_END();
}
