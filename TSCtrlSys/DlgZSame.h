#pragma once


// CDlgZUPLimit dialog

class CDlgZSame : public CDialog
{
	DECLARE_DYNAMIC(CDlgZSame)

public:
	CDlgZSame(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgZSame();

// Dialog Data
	enum { IDD = IDD_DLG_ZSAME };
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
