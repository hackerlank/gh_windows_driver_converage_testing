#pragma once
#include "afxwin.h"

class CConverageDlg : public CDialog
{
public:
	CConverageDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_CONVERAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CButton m_LoadDriver;
	CButton m_UnloadDriver;
	CButton m_RunDLL;
	CButton m_ModuleBtn;
	CButton m_StartDriver;
	CComboBox m_Module;

public:
	afx_msg void OnBnClickedListModule();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedUnload();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedStart();
};
