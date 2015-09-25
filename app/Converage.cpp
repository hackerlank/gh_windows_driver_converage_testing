#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>

#include "Converage.h"
#include "ConverageDlg.h"
#include "CVGDriver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define dbg(_x_) _tprintf(_T("[CVG] ")_x_);
#define info(_x_) _tprintf(_T("")_x_);

BEGIN_MESSAGE_MAP(CConverageApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CConverageApp::CConverageApp()
{
}

CConverageApp theApp;

using namespace std;
vector<CString> gModuleList;

BOOLEAN ExtractFile(DWORD dwID, TCHAR* pResType, TCHAR* pFileName)
{
    HMODULE hModule = GetModuleHandle(NULL);
    HRSRC resource = FindResource(hModule, MAKEINTRESOURCE(dwID), pResType);
    HGLOBAL hRSC = LoadResource(hModule, resource);
    LPVOID lpResBuffer = (unsigned char*)::LockResource(hRSC);
    DWORD dwFileSize = SizeofResource(hModule, resource);
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
    LPVOID pvData = GlobalLock(hGlobal);
    memcpy(pvData, lpResBuffer, dwFileSize);
    HANDLE hFile = CreateFile(pFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile == INVALID_HANDLE_VALUE){
		return FALSE;
	}

	DWORD dwRet;
	WriteFile(hFile, pvData, dwFileSize, &dwRet, NULL);
	CloseHandle(hFile);
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	return TRUE;
}

BOOLEAN ListModule(void)
{
	int i=0;
	TCHAR *ptr=NULL;
	TCHAR pBuffer[4096]={0};

	gModuleList.clear();
	if(CVG_GetModuleList(pBuffer, sizeof(pBuffer)) == FALSE){
		dbg(L"Can't open cvg driver !\n");
		return FALSE;
	}

	_tprintf(_T("Module Name: "));
	ptr = pBuffer;
	do{
		if(pBuffer[i] == L''){
			break;
		}

		if(pBuffer[i] == L','){
			pBuffer[i] = L'';
			gModuleList.push_back(ptr);
			_tprintf(_T("%s "), ptr);
			ptr = &pBuffer[i + 1];
		}
		i+= 1;
	}while(1);
	dbg(_T("\nAction complete !\n"));
	return TRUE;
}

BOOLEAN LoadDriver(void)
{
	if(ExtractFile(IDR_FILE1, _T("FILE"), _T("C:\\Windows\\System32\\cvg.dll")) == FALSE){
		dbg(_T("Can't copy cvg.sys to c:\\windows\\system32\\ folder !\n"));
		return FALSE;
	}
	system("sc create cvg binPath= \"c:\\windows\\system32\\cvg.sys\" type= \"kernel\" start= \"demand\" error= \"normal\" Displayname= \"cvg\"");
	if(system("sc start cvg") != 0){
		dbg(_T("Can't start cvg driver !\n"));
	}

	if(ExtractFile(IDR_FILE2, _T("FILE"), _T("C:\\Windows\\System32\\cvg.sys")) == FALSE){
		dbg(_T("Can't copy cvg.dll to c:\\windows\\system32\\ folder !\n"));
		return FALSE;
	}
	dbg(_T("Action complete !\n"));
	return TRUE;
}

BOOLEAN StartDriver(void)
{
	if(system("sc start cvg") != 0){
		dbg(_T("Can't start cvg driver !\n"));
		return FALSE;
	}
	dbg(_T("Action complete !\n"));
	return TRUE;
}

BOOLEAN UnloadDriver(void)
{
	system("sc stop cvg");
	system("sc delete cvg");
	DeleteFile(_T("C:\\Windows\\System32\\cvg.sys"));
	DeleteFile(_T("C:\\Windows\\System32\\cvg.dll"));
	dbg(_T("Action complete !\n"));
	return TRUE;
}

BOOLEAN RunDLL(void)
{
	typedef void (*RunDll)(void);
	HMODULE hDLL=NULL;
	RunDll ptrRunFunc=NULL;

	hDLL = LoadLibrary(_T("cvg.dll"));
	if(hDLL == NULL){
		dbg(_T("Load cvg.dll failed\n"));
		return FALSE;
	}

	ptrRunFunc = (RunDll)GetProcAddress(hDLL, "RunDll");
	if(ptrRunFunc == NULL){
		dbg(_T("Can't get \"RunDll\" function pointer\n"));
		return FALSE;
	}
	
	_tprintf(_T("RunDll address: 0x%X\n"), (DWORD)ptrRunFunc);
	ptrRunFunc();
	return TRUE;
}

void ParseCmdLine(TCHAR *szCmdLine)
{
	int index=0;
	TCHAR *szParam=_tcstok(szCmdLine, _T(" "));
	while(szParam != NULL){
		//_tprintf(_T("Param%d: %s\n"), index++, szParam);
		if((_tcscmp(szParam, _T("help")) == 0) ||
			(_tcscmp(szParam, _T("-help")) == 0) ||
			(_tcscmp(szParam, _T("--help")) == 0) ||
			(_tcscmp(szParam, _T("/?")) == 0))
		{
			info(_T("Usage: cvg.exe [list|load|start|unload|run]\n"));
		}
		else if(_tcscmp(szParam, _T("list")) == 0){
			ListModule();
		}
		else if(_tcscmp(szParam, _T("load")) == 0){
			LoadDriver();
		}
		else if(_tcscmp(szParam, _T("start")) == 0){
			StartDriver();
		}
		else if(_tcscmp(szParam, _T("unload")) == 0){
			UnloadDriver();
		}
		else if(_tcscmp(szParam, _T("run")) == 0){
			RunDLL();
		}
		else{
			dbg(_T("Invalid command !\n"));
		}
		szParam = _tcstok(NULL, _T(" "));
	}
}

BOOL CConverageApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	if(!AfxSocketInit()){
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	if(!AfxOleInit()){
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	BOOLEAN bAttachToConsole = FALSE;
	if(m_lpCmdLine[0] != 0){
		bAttachToConsole = TRUE;
		AttachConsole(ATTACH_PARENT_PROCESS);
		_tfreopen(_T("CONIN$"), _T("r+t"), stdin);
		_tfreopen(_T("CONOUT$"), _T("w+t"), stdout);
		SetConsoleTitle(_T("Converage Testing"));
		info(_T("\n=== Converage Testing by Lucas and Steward ===\n"));
		ParseCmdLine(m_lpCmdLine);
	}

	if(bAttachToConsole == FALSE){
		CConverageDlg dlg;
		m_pMainWnd = &dlg;
		dlg.DoModal();
	}

	if(bAttachToConsole == TRUE){
        FreeConsole();
    }
	return FALSE;
}
