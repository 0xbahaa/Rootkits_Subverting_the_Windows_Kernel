// loader.cpp
//	Loading our driver the correct way
//	 by opening the ServiceManager 

#include "windows.h"

bool _util_load_sysfile()
{
	TCHAR aPath[1024];
	TCHAR aCurrentDirectory[515];
	TCHAR drvr_name[20] = L"test";

	SC_HANDLE sh = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(!sh)
	{
		printf("Failure to open SVCmanager.\nYou should run this as admin ...");
		return false;
	}

	GetCurrentDirectory( 512, aCurrentDirectory);

	_sntprintf(aPath,1022,L"%ls\\%ls.sys",aCurrentDirectory,drvr_name);
	printf("loading\"%ls\"", aPath);

	SC_HANDLE rh = CreateService(sh,
			(LPCWSTR)drvr_name,
			(LPCWSTR)drvr_name,
			SERVICE_ALL_ACCESS,
			SERVICE_KERNEL_DRIVER,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,
			aPath,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);

	if(!rh)
	{
		printf("Failure to create the new service...\n");

		if (GetLastError() == ERROR_SERVICE_EXISTS)
		{
			printf("Guess why... the service already exists :) \n");
			
			// service exists
			rh = OpenService(sh,(LPCWSTR)drvr_name,SERVICE_ALL_ACCESS);
			if(!rh)
			{
				printf("Failed to open the service :(\n");
				
				CloseServiceHandle(sh);
				return false;
			}
		}
		else
		{
			printf("Unknown error\n");
			
			CloseServiceHandle(sh);
			return false;
		}
	}

	printf("Service installed successfully\n");

	// start the drivers
	if(rh)
	{
		if(0 == StartService(rh, 0, NULL))
		{
			if(ERROR_SERVICE_ALREADY_RUNNING == GetLastError())
			{
				printf("Service already running\n");

				// no real problem
			}
			else
			{
				printf("Failed to start service\n");

				CloseServiceHandle(sh);
				CloseServiceHandle(rh);
				return false;
			}
		}

		printf("Service started successfully\n");
		
		CloseServiceHandle(sh);
		CloseServiceHandle(rh);
	}

	return true;
}


int main(int argc, _TCHAR* argv[])
{
	_util_load_sysfile();

	//system("pause");
	getchar();

	return 0;
}

