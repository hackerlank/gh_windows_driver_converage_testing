#include "stdafx.h"
#include "Converage.h"
#include "ConverageDlg.h"
#include "CVGDriver.h"
#include <string>
#include <vector>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
extern vector<CString> gModuleList;
BOOLEAN ListModule(void);
BOOLEAN StartDriver(void);
BOOLEAN LoadDriver(void);
BOOLEAN UnloadDriver(void);
BOOLEAN RunDLL(void);

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

CConverageDlg::CConverageDlg(CWnd* pParent /*=NULL*/) : CDialog(CConverageDlg::IDD, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConverageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODULE, m_Module);
	DDX_Control(pDX, IDC_LIST_MODULE, m_ModuleBtn);
	DDX_Control(pDX, IDC_LOAD, m_LoadDriver);
	DDX_Control(pDX, IDC_UNLOAD, m_UnloadDriver);
	DDX_Control(pDX, IDC_RUN, m_RunDLL);
	DDX_Control(pDX, IDC_START, m_StartDriver);
}

BEGIN_MESSAGE_MAP(CConverageDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LIST_MODULE, &CConverageDlg::OnBnClickedListModule)
	ON_BN_CLICKED(IDC_LOAD, &CConverageDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_UNLOAD, &CConverageDlg::OnBnClickedUnload)
	ON_BN_CLICKED(IDC_RUN, &CConverageDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_START, &CConverageDlg::OnBnClickedStart)
END_MESSAGE_MAP()

BOOL CConverageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if(pSysMenu != NULL){
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if(!strAboutMenu.IsEmpty()){
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	return TRUE;
}

void CConverageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if((nID & 0xFFF0) == IDM_ABOUTBOX){
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CConverageDlg::OnPaint()
{
	if(IsIconic()){
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else{
		CDialog::OnPaint();
	}
}

HCURSOR CConverageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CConverageDlg::OnBnClickedListModule()
{
	int i=0, size=0;
	m_Module.ResetContent();
	if(ListModule() == TRUE){
		size = (int)gModuleList.size();
		for(i=0; i<size; i++){
			m_Module.AddString(gModuleList[i]);
		}
		m_Module.SetCurSel(0);
	}
}

void CConverageDlg::OnBnClickedLoad()
{
	if(LoadDriver() == FALSE){
		MessageBox(_T("Load driver failed !"), _T("Error"), MB_OK | MB_ICONERROR);
	}
	else{
		MessageBox(_T("Load driver complete !"), _T("Info"), MB_OK | MB_ICONINFORMATION);
	}
}

void CConverageDlg::OnBnClickedUnload()
{
	UnloadDriver();
	MessageBox(_T("Unload driver complete !"), _T("Info"), MB_OK | MB_ICONINFORMATION);
}

void CConverageDlg::OnBnClickedRun()
{
	RunDLL();
}

void CConverageDlg::OnBnClickedStart()
{
	if(StartDriver() == FALSE){
		MessageBox(_T("Start driver failed !"), _T("Error"), MB_OK | MB_ICONERROR);
	}
	else{
		MessageBox(_T("Start driver complete !"), _T("Info"), MB_OK | MB_ICONINFORMATION);
	}
}
