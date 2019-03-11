
//----------------------------------------------------------------
// build a .sys file on disk from a resource
//----------------------------------------------------------------
bool decompress_sysfile(char *theResourceName)
{
	HRSRC aResourceH;
	HGLOBAL aResourceHGlobal;
	unsigned char * aFilePtr;
	unsigned long aFileSize;
	HANDLE file_handle;
	
	//////////////////////////////////////////////////////////
	// locate a named resource in the current binary EXE
	//////////////////////////////////////////////////////////
	aResourceH = FindResource(NULL, theResourceName, "BINARY");
	if(!aResourceH)
	{
		return false;
	}
	
	aResourceHGlobal = LoadResource(NULL, aResourceH);
	if(!aResourceHGlobal)
	{
		return false;
	}

	aFileSize = SizeofResource(NULL, aResourceH);
	aFilePtr = (unsigned char *)LockResource(aResourceHGlobal);
	if(!aFilePtr)
	{
		return false;
	}

	
	char filename[64];
	snprintf(filename, 62, "%s.sys", theResourceName);
	
	file_handle = 
		CreateFile( 
				filename,
				FILE_ALL_ACCESS,
				0,
				NULL,
				CREATE_ALWAYS,
				0,
				NULL);

	if(INVALID_HANDLE_VALUE == file_handle)
	{
		return false;
	}

	while(aFileSize--)
	{
		unsigned long numWritten;
		WriteFile(file_handle, aFilePtr, 1, &numWritten, NULL);
		aFilePtr++;
	}
	CloseHandle(file_handle);

	return true;
}



