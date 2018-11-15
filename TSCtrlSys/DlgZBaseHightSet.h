#pragma once
#include "Resource.h"

// CDlgZBaseHightSet dialog

class CDlgZBaseHightSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgZBaseHightSet)

public:
	CDlgZBaseHightSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgZBaseHightSet();

// Dialog Data
	enum { IDD = IDD_DLG_INFO_ZBASESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedDlgBasesetSetbase();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeBaseJethight();
};
