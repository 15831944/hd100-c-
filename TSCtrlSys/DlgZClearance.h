#pragma once


// CDlgZUPLimit dialog

class CDlgZCLearance : public CDialog
{
	DECLARE_DYNAMIC(CDlgZCLearance)

public:
	CDlgZCLearance(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgZCLearance();

// Dialog Data
	enum { IDD = IDD_DLG_ZCLEARANCE };
	tgCmdLine *m_pCmdLine;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
};
