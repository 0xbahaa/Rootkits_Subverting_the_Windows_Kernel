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
	
	switch (my_osversion_struct.dwMajorVersion) {
	
		case 4:
			DbgPrint("Windows NT 4");
			break;
		case 5:
			switch(my_osversion_struct.dwMinorVersion){
				case 0:
					DbgPrint("Windows 2000");
					break;
				case 1:
					DbgPrint("Win XP");
					break;
				case 2:
					DbgPrint("Win XP Pro x64 or Win Server 2003/HomeServer/2003-R2");
					break;
				default:
					DbgPrint("Unknown windows version");
					break;
			}
			
			break;
		case 6:
			switch(my_osversion_struct.dwMinorVersion){
				case 0:
					DbgPrint("Windows Vista or Windows Server 2008");
					break;
				case 1:
					DbgPrint("Windows 7 or Win Server 2008 R2");
					break;
				case 2:
					DbgPrint("Windows 8 or Win Server 2012");
					break;
				case 3:
					DbgPrint("Windows 8.1 or Win Server 2012 R2");
					break;
				default:
					DbgPrint("Unknown windows version");
					break;
			}
			
			break;
		case 10:
			DbgPrint("Windows 10 or Windows Server 2016");
			break;
		default:
			DbgPrint("Unknown windows version");
			break;
	}
	
	return STATUS_SUCCESS;
}
