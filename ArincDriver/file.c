
#include "precomp.h"
#include "file.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, EvtDeviceFileCreate)
#pragma alloc_text(PAGE, EvtFileClose)
#pragma alloc_text(PAGE, EvtFileCleanup)
#pragma alloc_text(PAGE, EvtFileContextCleanup)
#endif

VOID EvtDeviceFileCreate(
	IN WDFDEVICE Device,
	IN WDFREQUEST Request,
	IN WDFFILEOBJECT FileObject)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_CONTEXT_EXTENSION deviceContext;
	LARGE_INTEGER systemTime, localTime, cpuFreq;
#if DBG
	TIME_FIELDS timeFields;
#endif  // DBG

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(Device);

	// 初始化系统开机时间变量
	KeQuerySystemTime(&systemTime);
	deviceContext->SysStartupCounter = KeQueryPerformanceCounter(&cpuFreq).QuadPart;
	ExSystemTimeToLocalTime(&systemTime, &localTime);
	deviceContext->SysStartupTime = localTime.QuadPart;
	deviceContext->CpuFrequency = cpuFreq.QuadPart;
#if DBG
	// 把时间转换为容易理解的形式
	RtlTimeToTimeFields(&localTime, &timeFields);
	HrKdPrint(("Startup time: %04d-%02d-%02d %02d:%02d:%02d:%03d (Week: %d)\n",
		timeFields.Year, timeFields.Month, timeFields.Day,
		timeFields.Hour, timeFields.Minute, timeFields.Second, timeFields.Milliseconds,
		timeFields.Weekday));
#endif  // DBG

	deviceContext->IsOpen = TRUE;

	{	// for debug information
		PUNICODE_STRING fileName = WdfFileObjectGetFileName(FileObject);

		HrKdPrint(("FileName: %wZ\n", fileName));
	}

	WdfRequestComplete(Request, status);

	HR_PRINT_METHOD_END();
}

VOID EvtFileClose(
	IN WDFFILEOBJECT FileObject)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_CONTEXT_EXTENSION deviceContext;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	deviceContext = WdfDeviceGetContext(WdfFileObjectGetDevice(FileObject));

	deviceContext->IsOpen = FALSE;

	HR_PRINT_METHOD_END();
}

VOID EvtFileCleanup(
	IN WDFFILEOBJECT FileObject)
{
	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();
}

VOID EvtFileContextCleanup(
	IN WDFOBJECT Object)
{
	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();
}
