#pragma once


// CFeedModelCtrlDlg dialog

class CFeedModelCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CFeedModelCtrlDlg)

public:
	CFeedModelCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFeedModelCtrlDlg();

// Dialog Data
	enum { IDD = IDD_DLG_FEED_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeFeed();
	afx_msg void OnEnChangePageSysFeedDownvel();
	afx_msg void OnBnClickedDlgFeedctrlVelset();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDlgFeedctrlFeedup();
	afx_msg void OnBnClickedDlgFeedctrlFeeddown();
	afx_msg void OnBnClickedDlgFeedctrlUnfeedup();
	afx_msg void OnBnClickedDlgFeedctrlUnfeeddown();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedOk3();
};
