#include "stdafx.h"
#include <windows.h>
#include "../app/CVGDriver.h"

void dbg(TCHAR *fmt, ...)
{
	TCHAR szBuf[MAX_PATH]={0};

	va_list ap; 
   	va_start(ap, fmt); 
	_vstprintf(szBuf, fmt, ap);
    va_end(ap);
	OutputDebugString(szBuf);
}

void _stdcall RunDll(void)
{
    dbg(_T("Yes, I am DLL !\n"));

	TCHAR *MODULE_NAME=_T("Ntfs.sys");
	DWORD i=0, dwRet=0;
	DWORD dwBaseAddr=0, dwEntryAddr=0, dwSectionAddr=0, dwSectionCnt=0, dwTextAddr=0, dwTextSize=0;
	IMAGE_SECTION_HEADER sectionHeader[10]={0};
	BYTE aOPCode[4]={0};

	dwBaseAddr = CVG_GetModuleBaseAddress(MODULE_NAME);
	dwEntryAddr = CVG_GetModuleEntryAddress(MODULE_NAME);
	dwSectionAddr = CVG_GetModuleSectionAddress(MODULE_NAME);
	dwSectionCnt = CVG_GetModuleSectionCount(MODULE_NAME);
	dbg(_T("Base address: 0x%X\n"), dwBaseAddr);
	dbg(_T("Entry address: 0x%X\n"), dwEntryAddr);
	dbg(_T("Section address: 0x%X\n"), dwSectionAddr);
	dbg(_T("Section count: %d\n"), dwSectionCnt);

	if(dwEntryAddr != -1){
		// parse section header
		if(CVG_ReadMemory(dwSectionAddr, (BYTE*)sectionHeader, sizeof(IMAGE_SECTION_HEADER) * dwSectionCnt) != -1){
			for(i=0; i<dwSectionCnt; i++){
				if(strcmp((char*)sectionHeader[i].Name, ".text") == 0){
					dwTextAddr = sectionHeader[i].VirtualAddress;
					dwTextSize = sectionHeader[i].Misc.VirtualSize;

					// read data of ".text" section
					if(CVG_ReadMemory(dwBaseAddr + dwTextAddr, (BYTE*)aOPCode, sizeof(aOPCode)) != -1){
						for(i=0; i<sizeof(aOPCode); i++){
							dbg(_T("Original Data-%d: 0x%X"), i, aOPCode[i]);
						}
						dbg(_T("\n"));
					}
					break;
				}
			}
		}

		// modify data
		aOPCode[0] = 0x11;
		aOPCode[1] = 0x22;
		aOPCode[2] = 0x33;
		aOPCode[3] = 0x44;
		if(CVG_WriteMemory(dwBaseAddr + dwTextAddr, (BYTE*)aOPCode, sizeof(aOPCode)) != -1){

			// read modified data
			memset(aOPCode, 0, sizeof(aOPCode));
			if(CVG_ReadMemory(dwBaseAddr + dwTextAddr, (BYTE*)aOPCode, sizeof(aOPCode)) != -1){
				for(i=0; i<sizeof(aOPCode); i++){
					dbg(_T("Modified Data-%d: 0x%X"), i, aOPCode[i]);
				}
				dbg(_T("\n"));
			}
		}

		// allocate kernel memory
		if((dwBaseAddr = CVG_NewMemory(4)) != -1){
			dbg(_T("New memory address: 0x%X\n"), dwBaseAddr);

			// write memory
			aOPCode[0] = 0x55;
			aOPCode[1] = 0x66;
			aOPCode[2] = 0x77;
			aOPCode[3] = 0x88;
			if(CVG_WriteMemory(dwBaseAddr, (BYTE*)aOPCode, sizeof(aOPCode)) != -1){

				// read modified memory
				memset(aOPCode, 0, sizeof(aOPCode));
				if(CVG_ReadMemory(dwBaseAddr, (BYTE*)aOPCode, sizeof(aOPCode)) != -1){
					for(i=0; i<sizeof(aOPCode); i++){
						dbg(_T("Modified Memory-%d: 0x%X"), i, aOPCode[i]);
					}
					dbg(_T("\n"));
				}
			}
			CVG_FreeMemory(dwBaseAddr);
		}
	}
}
