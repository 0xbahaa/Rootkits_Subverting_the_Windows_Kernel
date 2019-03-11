// --------------------
// Listing OS version info from Kernel-Mode
//		by 0xbahaa
// --------------------

#include "ntddk.h"
#include <stdio.h>

typedef unsigned char BOOL, *PBOOL;
typedef unsigned char BYTE, *PBYTE;
typedef unsigned long DWORD, *PDWORD;
typedef unsigned short WORD, *PWORD;

/*	Definition of OSVersionInfo struct (winnt.h) 
typedef struct _OSVERSIONINFOA {
  DWORD dwOSVersionInfoSize;
  DWORD dwMajorVersion;
  DWORD dwMinorVersion;
  DWORD dwBuildNumber;
  DWORD dwPlatformId;
  CHAR  szCSDVersion[128];
} OSVERSIONINFOA, *POSVERSIONINFOA, *LPOSVERSIONINFOA;
*/
OSVERSIONINFOW my_osversion_struct;

VOID OnUnload( IN PDRIVER_OBJECT DriverObject )
{	
	DbgPrint("ROOTKIT: OnUnload called\n");
}

NTSTATUS DriverEntry( IN PDRIVER_OBJECT theDriverObject, IN PUNICODE_STRING theRegistryPath )
{
	theDriverObject->DriverUnload  = OnUnload; 

	
	RtlGetVersion( &my_osversion_struct );
	
	DbgPrint("dwOSVersionInfoSize: %u", my_osversion_struct.dwOSVersionInfoSize);
	DbgPrint("dwMajorVersion: %u", my_osversion_struct.dwMajorVersion);
	DbgPrint("dwMinorVersion: %u", my_osversion_struct.dwMinorVersion);
	DbgPrint("dwBuildNumber: %u", my_osversion_struct.dwBuildNumber);
	DbgPrint("dwPlatformId: %u", my_osversion_struct.dwPlatformId);
	DbgPrint("szCSDVersion: %s", my_osversion_struct.szCSDVersion);
	
	 
	return STATUS_SUCCESS;
}
