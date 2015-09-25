#ifndef __CVG_DRIVER_H__
#define __CVG_DRIVER_H__

	#include <windows.h>
	#include <Winbase.h>
	#include <winioctl.h>
	#include "../sys/main.h"
	#define DRIVER_NAME _T("\\\\.\\cvg")
	
	VOID CVG_FreeMemory(DWORD dwAddr);
	DWORD CVG_GetModuleBaseAddress(TCHAR *pBuffer);
	DWORD CVG_GetModuleEntryAddress(TCHAR *pBuffer);
	DWORD CVG_GetModuleSectionAddress(TCHAR *pBuffer);
	DWORD CVG_GetModuleSectionCount(TCHAR *pBuffer);
	DWORD CVG_NewMemory(DWORD dwSize);
	BOOLEAN CVG_GetModuleList(TCHAR *pBuffer, DWORD dwLen);
	BOOLEAN CVG_PatchModule(TCHAR *pBuffer);
	DWORD CVG_ReadMemory(DWORD dwAddr, BYTE *pRetBuffer, DWORD dwLen);
	DWORD CVG_WriteMemory(DWORD dwAddr, BYTE *pRetBuffer, DWORD dwLen);

#endif