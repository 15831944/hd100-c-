#pragma once


// CDlgStandBy dialog

class CDlgStandBy : public CDialog
{
	DECLARE_DYNAMIC(CDlgStandBy)

public:
	CDlgStandBy(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgStandBy();

// Dialog Data
	enum { IDD = IDD_DLG_STANDBY };
	tgCmdLine *m_pCmdLine;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnReadAxispos();
	afx_msg void OnBnClickedBtnMove();
	afx_msg void OnBnClickedOk();
public:
	void RefreshAxisPos(void);
	virtual BOOL OnInitDialog();
};
