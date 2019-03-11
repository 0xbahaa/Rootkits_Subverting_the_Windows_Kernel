#include "ntddk.h"


typedef unsigned char BOOL, *PBOOL;
typedef unsigned char BYTE, *PBYTE;
typedef unsigned long DWORD, *PDWORD;
typedef unsigned short WORD, *PWORD;

DWORD HANDLELISTOFFSET = 0x1c; // ????
DWORD HANDLETABLEOFFSET = 0xc4;// ????
DWORD EPROCPIDOFFSET = 0x08;   // ????
		
void ListProcessesByHandleTable(void);

VOID OnUnload( IN PDRIVER_OBJECT DriverObject )
{
	DbgPrint("OnUnload called\n");
}

NTSTATUS DriverEntry( IN PDRIVER_OBJECT theDriverObject, IN PUNICODE_STRING theRegistryPath )
{
	DbgPrint("I loaded!");

	theDriverObject->DriverUnload  = OnUnload; 

	ListProcessesByHandleTable();
	
	return STATUS_SUCCESS;
}

void ListProcessesByHandleTable()
{
   PEPROCESS eproc;
   PLIST_ENTRY start_plist, plist_hTable = NULL;
   PDWORD d_pid;

   // Get the current EPROCESS block
   eproc = PsGetCurrentProcess();
 
   plist_hTable = (PLIST_ENTRY)((*(PDWORD)((DWORD) eproc + HANDLETABLEOFFSET)) + HANDLELISTOFFSET);

   start_plist = plist_hTable;
   do
   {
      d_pid = (PDWORD)(((DWORD)plist_hTable + EPROCPIDOFFSET) - HANDLELISTOFFSET);

	  // Print the Process ID as a debug message.
	  // You could store it to compare to API calls.
	  DbgPrint("Process ID: %d\n", *d_pid);

	  // Advance
	  plist_hTable = plist_hTable->Flink;
   }while (start_plist != plist_hTable); 
}
