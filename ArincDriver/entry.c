
#include "precomp.h"
#include "adddev.h"

DRIVER_INITIALIZE		       DriverEntry;
EVT_WDF_DRIVER_UNLOAD	       EvtDriverUnload;
EVT_WDF_OBJECT_CONTEXT_CLEANUP EvtDriverContextCleanup;

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, EvtDriverUnload)
#pragma alloc_text(PAGE, EvtDriverContextCleanup)
#endif	// ALLOC_PRAGMA

EXTERN_C NTSTATUS DriverEntry(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDRIVER_CONTEXT_EXTENSION driverContext;
	WDFDRIVER driver;
	WDF_DRIVER_CONFIG driverConfig;
	WDF_OBJECT_ATTRIBUTES objAttributes;

	HR_PRINT_SET_PREFIX("#> ");

	HR_PRINT_METHOD_BEGIN();

	HrKdPrint(("Compiled time: %s (%s)\n", __DATE__, __TIME__));

	// 初始化驱动扩展对象
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&objAttributes, DRIVER_CONTEXT_EXTENSION);
	objAttributes.EvtCleanupCallback = EvtDriverContextCleanup;

	// 初始化驱动事件
	WDF_DRIVER_CONFIG_INIT(&driverConfig, EvtDriverDeviceAdd);
	driverConfig.EvtDriverUnload = EvtDriverUnload;
	driverConfig.DriverPoolTag = HR_MEM_TAG;

	// 创建驱动对象
	status = WdfDriverCreate(DriverObject, RegistryPath, &objAttributes, &driverConfig, &driver);
	if (!NT_SUCCESS(status))
	{
		HrKdPrint(("WdfDriverCreate() failed with status %08X\n", status));
	}

	// 初始化驱动扩展对象属性
	driverContext = WdfDriverGetContext(driver);
	driverContext->Instance = 0;	// 

	HR_PRINT_METHOD_END();

	return status;
}

VOID EvtDriverUnload(
	IN WDFDRIVER Driver)
{
	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	HR_PRINT_METHOD_END();
}

VOID EvtDriverContextCleanup(
	IN WDFDRIVER Driver)
{
	PDRIVER_CONTEXT_EXTENSION driverContext;

	PAGED_CODE();

	HR_PRINT_METHOD_BEGIN();

	driverContext = WdfDriverGetContext(Driver);

	HR_PRINT_METHOD_END();
}
