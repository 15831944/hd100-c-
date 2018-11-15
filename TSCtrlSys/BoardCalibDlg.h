#pragma once
#include "resource.h"
#include "paramdef.h"

class CBoardCalibDlg : public CDialog
{
	DECLARE_DYNAMIC(CBoardCalibDlg)
public:
	CBoardCalibDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBoardCalibDlg();

	tgCmdLine *m_pCmdLine; // ÷∏¡Óƒ⁄»›
	int		m_iSelIndex;   // ÷∏¡ÓID 
	void RefreshDate();
	// Dialog Data
	enum { IDD = IDD_DLG_BOARDCALIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboBmark();
	afx_msg void OnBnClickedButtonBlearn();
	afx_msg void OnBnClickedBtnBmove();
	afx_msg void OnBnClickedBtnMoveCalca();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeMarkEditBa();
	afx_msg void OnBnClickedOk();
};
