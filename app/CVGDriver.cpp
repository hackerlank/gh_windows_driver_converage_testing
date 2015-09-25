#include "stdafx.h"
#include "CVGDriver.h"

#define DRIVER_NAME _T("\\\\.\\cvg")
BOOLEAN CVG_GetModuleList(TCHAR *pBuffer, DWORD dwLen)
{
	HANDLE hFile=NULL;
	DWORD dwRet=0;

	hFile = CreateFile(DRIVER_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		return FALSE;
	}
	memset(pBuffer, 0, dwLen);
	DeviceIoControl(hFile, IOCTL_CVG_GET_MODULE_LIST, pBuffer, dwLen, pBuffer, dwLen, &dwRet, NULL);
	CloseHandle(hFile);
	return TRUE;
}

BOOLEAN CVG_PatchModule(TCHAR *pBuffer)
{
	HANDLE hFile=NULL;
	DWORD dwRet=0;
	DWORD dwLen=0;
	BOOLEAN bRet=0;

	hFile = CreateFile(_T("\\\\.\\cvg"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		return FALSE;
	}
	dwLen = ((DWORD)_tcsclen(pBuffer) + 1) * 2;
	bRet = DeviceIoControl(hFile, IOCTL_CVG_PATCH_MODULE, pBuffer, dwLen, NULL, 0, &dwRet, NULL);
	CloseHandle(hFile);
	return TRUE;
}

DWORD CVG_GetModuleBaseAddress(TCHAR *pBuffer)
{
	HANDLE hFile=NULL;
	DWORD dwRet=0, dwOutLen=0, dwInLen=0, dwData=0;
	BOOLEAN bRet=0;

	hFile = CreateFile(_T("\\\\.\\cvg"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		return -1;
	}
	dwInLen = (DWORD)_tcsclen(pBuffer) + 1;
	dwInLen*= 2;
	dwOutLen = sizeof(DWORD);
	bRet = DeviceIoControl(hFile, IOCTL_CVG_GET_BASE_ADDRESS, pBuffer, dwInLen, &dwData, dwOutLen, &dwRet, NULL);
	CloseHandle(hFile);
	return dwData;
}

DWORD CVG_GetModuleEntryAddress(TCHAR *pBuffer)
{
	HANDLE hFile=NULL;
	DWORD dwRet=0, dwOutLen=0, dwInLen=0, dwData=0;
	BOOLEAN bRet=0;

	hFile = CreateFile(_T("\\\\.\\cvg"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		return -1;
	}
	dwInLen = (DWORD)_tcsclen(pBuffer) + 1;
	dwInLen*= 2;
	dwOutLen = sizeof(DWORD);
	bRet = DeviceIoControl(hFile, IOCTL_CVG_GET_ENTRY_ADDRESS, pBuffer, dwInLen, &dwData, dwOutLen, &dwRet, NULL);
	CloseHandle(hFile);
	return dwData;
}

DWORD CVG_GetModuleSectionAddress(TCHAR *pBuffer)
{
	HANDLE hFile=NULL;
	DWORD dwRet=0, dwOutLen=0, dwInLen=0, dwData=0;
	BOOLEAN bRet=0;

	hFile = CreateFile(_T("\\\\.\\cvg"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		return -1;
	}
	dwInLen = (DWORD)_tcsclen(pBuffer) + 1;
	dwInLen*= 2;
	dwOutLen = sizeof(DWORD);
	bRet = DeviceIoControl(hFile, IOCTL_CVG_GET_SECTION_ADDRESS, pBuffer, dwInLen, &dwData, dwOutLen, &dwRet, NULL);
	CloseHandle(hFile);
	return dwData;
}

DWORD CVG_GetModuleSectionCount(TCHAR *pBuffer)
{
	HANDLE hFile=NULL;
	DWORD dwRet=0, dwOutLen=0, dwInLen=0, dwData=0;
	BOOLEAN bRet=0;

	hFile = CreateFile(_T("\\\\.\\cvg"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		return -1;
	}
	dwInLen = (DWORD)_tcsclen(pBuffer) + 1;
	dwInLen*= 2;
	dwOutLen = sizeof(DWORD);
	bRet = DeviceIoControl(hFile, IOCTL_CVG_GET_SECTION_COUNT, pBuffer, dwInLen, &dwData, dwOutLen, &dwRet, NULL);
	CloseHandle(hFile);
	return dwData;
}

DWORD CVG_ReadMemory(DWORD dwAddr, BYTE *pRetBuffer, DWORD dwLen)
{
	HANDLE hFile=NULL;
	DWORD dwRet=0;
	BOOLEAN bRet=0;

	hFile = CreateFile(_T("\\\\.\\cvg"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		return -1;
	}
	bRet = DeviceIoControl(hFile, IOCTL_CVG_READ_MEMORY, &dwAddr, dwLen, pRetBuffer, dwLen, &dwRet, NULL);
	CloseHandle(hFile);
	return dwRet;
}

DWORD CVG_WriteMemory(DWORD dwAddr, BYTE *pRetBuffer, DWORD dwLen)
{
	HANDLE hFile=NULL;
	DWORD dwRet=0;
	BOOLEAN bRet=0;

	hFile = CreateFile(_T("\\\\.\\cvg"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		return -1;
	}
	bRet = DeviceIoControl(hFile, IOCTL_CVG_WRITE_MEMORY, &dwAddr, dwLen, pRetBuffer, dwLen, &dwRet, NULL);
	CloseHandle(hFile);
	return dwRet;
}

DWORD CVG_NewMemory(DWORD dwLen)
{
	HANDLE hFile=NULL;
	DWORD dwRet=0, dwData;
	BOOLEAN bRet=0;

	hFile = CreateFile(_T("\\\\.\\cvg"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		return -1;
	}
	bRet = DeviceIoControl(hFile, IOCTL_CVG_NEW_MEMORY, &dwLen, sizeof(DWORD), &dwData, sizeof(DWORD), &dwRet, NULL);
	CloseHandle(hFile);
	return dwData;
}

VOID CVG_FreeMemory(DWORD dwAddr)
{
	HANDLE hFile=NULL;
	DWORD dwRet=0;
	BOOLEAN bRet=0;

	hFile = CreateFile(_T("\\\\.\\cvg"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE){
		bRet = DeviceIoControl(hFile, IOCTL_CVG_FREE_MEMORY, &dwAddr, sizeof(DWORD), NULL, 0, &dwRet, NULL);
		CloseHandle(hFile);
	}
}
