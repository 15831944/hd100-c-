#pragma once
#include "afxwin.h"


// CDlgWCL dialog

class CDlgWCL : public CDialog
{
	DECLARE_DYNAMIC(CDlgWCL)

public:
	CDlgWCL(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgWCL();
	tgCmdLine *m_pCmdLine;
	int m_iSelIndex;
// Dialog Data
	enum { IDD = IDD_DLG_WCL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void RefreshAxisPos();
	CComboBox m_Combox;
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnCbnSelchangeWclCombo();
	afx_msg void OnBnClickedBtnReadAxispos();
	afx_msg void OnBnClickedBtnMove();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedWorkmode2();
	afx_msg void OnBnClickedWorkmode1();
	afx_msg void OnEnChangeWclEditW();
};
