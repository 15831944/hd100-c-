#pragma once


// CDlgNeedleAdjust dialog

class CDlgNeedleAdjust : public CDialog
{
	DECLARE_DYNAMIC(CDlgNeedleAdjust)

public:

	CDlgNeedleAdjust(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNeedleAdjust();

// Dialog Data
	enum { IDD = IDD_DLG_NEEDLE_ADJUST };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonReadAxispos();
	afx_msg void OnBnClickedButtonMove();
	afx_msg void OnCbnSelendokComboPos();	
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonApply();
	

	afx_msg void OnBnClickedDlgNeedleAdjustDisp();
	afx_msg void OnBnClickedDlgNeedleAdjustJog();
};