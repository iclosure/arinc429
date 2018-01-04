
#include "precomp.h"
#include "interrupt.h"

VOID EnableInterrupt(
	IN WDFDEVICE Device,
	IN BOOLEAN bEnable)
{
	PDEVICE_CONTEXT_EXTENSION deviceContext;
	INT_CSR intCsr;

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(Device);

	intCsr.all = READ_REGISTER_ULONG(deviceContext->RegIntCsr);
	intCsr.bits.PciIntEnable = bEnable;
	intCsr.bits.LocalIntInputEnable = bEnable;
	WRITE_REGISTER_ULONG(deviceContext->RegIntCsr, intCsr.all);

	HrKdPrint(("EnableInterrupt: %s", bEnable ? "TRUE" : "FALSE"));

	HR_PRINT_METHOD_END();
}

BOOLEAN EvtInterruptIsr(
	IN WDFINTERRUPT Interrupt,
	IN ULONG MessageID)
{
	PDEVICE_CONTEXT_EXTENSION deviceContext;

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(WdfInterruptGetDevice(Interrupt));

	HrKdPrint(("StatReg: %08X\n", READ_REGISTER_ULONG(deviceContext->RegIntStat)));

	if ((~READ_REGISTER_ULONG(deviceContext->RegIntStat)) & deviceContext->RxChMask)
	{
		if (WdfInterruptQueueDpcForIsr(Interrupt))
		{
			// Disable PCI input interrupt
			INT_CSR intCsr;
			intCsr.all = READ_REGISTER_ULONG(deviceContext->RegIntCsr);
			intCsr.bits.PciIntEnable = FALSE;
			WRITE_REGISTER_ULONG(deviceContext->RegIntCsr, intCsr.all);
		}
		return TRUE;
	}

	HR_PRINT_METHOD_END();

	return FALSE;
}

VOID EvtInterruptDpc(
	IN WDFINTERRUPT Interrupt,
	IN WDFOBJECT AssociatedObject)
{
	PDEVICE_CONTEXT_EXTENSION deviceContext;
	ATOM_RDATA_INFO atomData;
	ULONG statReg;
	ULONG channel;
	ULONG count = 1;

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(WdfInterruptGetDevice(Interrupt));

	WdfSpinLockAcquire(deviceContext->IoctlSpinLock);

	while (count--)
	{
		//TEST
#ifdef TEST_ARINC429_000
		ULONG temp1 = READ_REGISTER_ULONG(deviceContext->RegIntStat);
		temp1 = READ_REGISTER_ULONG(deviceContext->RegIntStat);
		statReg = (~temp1) & deviceContext->RxChMask;
#else
		// 读取两次中断状态寄存器值，防止第一次读取错误的问题
		READ_REGISTER_ULONG(deviceContext->RegIntStat);
		statReg = (~READ_REGISTER_ULONG(deviceContext->RegIntStat)) & deviceContext->RxChMask;
#endif 
		if (statReg)
		{
			HrKdPrint(("StatReg: %08X, PortMask: %08X\n", statReg, deviceContext->RxChMask));

			for (channel = 0; channel < deviceContext->RxChNum; channel++)
			{
				if (statReg & (1UL << channel))
				{
					atomData.data = READ_REGISTER_ULONG(ARINC_BUS_RXREG_ADDR(deviceContext, channel, ARINC_REG_DATA_ID));

					HrKdPrint(("data: %08X, EnableLabel0: %d\n", atomData.data, deviceContext->EnableLabel0[channel]));

					//TEST
#ifdef TEST_ARINC429_000
					if (atomData.data == 0)
					{
						ULONG temp2;
						DbgPrint("#> DPC: temp1: %08X, statReg:%08X, CHANNEL:%d, DATA: %08X\n", temp1, statReg, channel, atomData.data);
						temp2 = READ_REGISTER_ULONG(deviceContext->RegIntStat);
						DbgPrint("#> DPC: temp2: %08X, statReg:%08X, CHANNEL:%d, DATA: %08X\n", temp2, statReg, channel, atomData.data);
					}
#endif  // TEST_ARINC429_000

					if ((0 == (atomData.data & 0xFF)) && (!deviceContext->EnableLabel0[channel]))	// 
					{
						DbgPrint("#> DPC: LABEL: 0x00n");
						continue;
					}

					atomData.channel = channel;
					atomData.timestamp = KeQueryPerformanceCounter(NULL).QuadPart;
					atomData.timestamp = deviceContext->SysStartupTime
						+ (atomData.timestamp - deviceContext->SysStartupCounter)
						* 10000000 / deviceContext->CpuFrequency;	// 时间戳，单位：100ns

					CircFifoWrite(&deviceContext->CircFifo, &atomData);

					HrKdPrint(("IRQ> Channel: %ld, Data: %08X\n", channel, atomData.data));
				}
			}

			KeSetEvent(deviceContext->PkEvent, 0, FALSE);
		}
		else
		{
			break;
		}
	}

	WdfSpinLockRelease(deviceContext->IoctlSpinLock);
	/*
	if (!CircFifoIsEmpty(&deviceContext->CircFifo))
	{
		KeSetEvent(deviceContext->PkEvent, 0, FALSE);
	}*/

	// Enable PCI input interrupt
	{
		INT_CSR intCsr;
		intCsr.all = READ_REGISTER_ULONG(deviceContext->RegIntCsr);
		intCsr.bits.PciIntEnable = TRUE;
		WRITE_REGISTER_ULONG(deviceContext->RegIntCsr, intCsr.all);
	}

	HR_PRINT_METHOD_END();
}

NTSTATUS EvtInterruptEnable(
	IN WDFINTERRUPT Interrupt,
	IN WDFDEVICE AssociatedDevice)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_CONTEXT_EXTENSION deviceContext;

	HR_PRINT_METHOD_BEGIN();

	//EnableInterrupt(WdfInterruptGetDevice(Interrupt), WdfTrue);

	HR_PRINT_METHOD_END();

	return status;
}

NTSTATUS EvtInterruptDisable(
	IN WDFINTERRUPT Interrupt,
	IN WDFDEVICE AssociatedDevice)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_CONTEXT_EXTENSION deviceContext;

	HR_PRINT_METHOD_BEGIN();

	//EnableInterrupt(WdfInterruptGetDevice(Interrupt), WdfFalse);

	HR_PRINT_METHOD_END();

	return status;
}
