#include <ntddk.h>
#include "Trigger.h"

NTSTATUS DriverUnload(_In_ PDRIVER_OBJECT driverObject)
{
	UNREFERENCED_PARAMETER(driverObject);
	Cleanup();
	KdPrint(("Goodbye world \n"));
	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT driverObject, PUNICODE_STRING registryPath)
{
	KdPrint(("Hello"));
	UNREFERENCED_PARAMETER(registryPath);
	KdPrint(("Hello, world"));
	NTSTATUS status;
	status = WfpInit(driverObject);
	driverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}



