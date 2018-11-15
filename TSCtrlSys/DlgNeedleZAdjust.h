#pragma once

#include "CButtonST/BtnST.h"
// CDlgNeedleZAdjust dialog

class CDlgNeedleZAdjust : public CDialog
{
	DECLARE_DYNAMIC(CDlgNeedleZAdjust)

public:
	CDlgNeedleZAdjust(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNeedleZAdjust();

// Dialog Data
	enum { IDD = IDD_DLG_NEEDLE_ZH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void UpdateZSensorSts(long sts);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDlgNeedleZhCali();
	afx_msg void OnClose();
	CButtonST m_inZSensor;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedDlgNeedleZhOffset();
	afx_msg void OnBnClickedDlgNeedleZhLed();
};
