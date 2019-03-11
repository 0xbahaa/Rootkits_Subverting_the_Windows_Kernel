// BASIC DEVICE DRIVER
//     Just loads and supports unloading

#include "ntddk.h"

VOID OnUnload( IN PDRIVER_OBJECT DriverObject )
{
	DbgPrint("==============> OnUnload called\n");
}

NTSTATUS DriverEntry( IN PDRIVER_OBJECT theDriverObject, IN PUNICODE_STRING theRegistryPath )
{
	DbgPrint("==============> I loaded!");
		
		//setting the unload routine for this driver object
	theDriverObject->DriverUnload  = OnUnload; 

	return STATUS_SUCCESS;
}