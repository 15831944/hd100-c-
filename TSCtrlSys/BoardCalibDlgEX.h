#pragma once
#include "resource.h"
#include "paramdef.h"

class CBoardCalibDlgEx : public CDialog
{
	DECLARE_DYNAMIC(CBoardCalibDlgEx)
public:
	CBoardCalibDlgEx(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBoardCalibDlgEx();

	tgCmdLine *m_pCmdLine; // ÷∏¡Óƒ⁄»›
	int		m_iSelIndex;   // ÷∏¡ÓID 
	void RefreshDate();
	// Dialog Data
	enum { IDD = IDD_DLG_BOARDCALIBEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboBmark();
	afx_msg void OnBnClickedButtonBlearn();
	afx_msg void OnBnClickedBtnBmove();
	afx_msg void OnBnClickedButtonPatBtest();
	afx_msg void OnBnClickedButtonPatBsave();
	afx_msg void OnBnClickedBtnMoveCalca();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeMarkEditBa();
};
