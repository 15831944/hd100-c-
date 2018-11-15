#pragma once


// CDlgZUPLimit dialog

class CDlgDelay : public CDialog
{
	DECLARE_DYNAMIC(CDlgDelay)

public:
	CDlgDelay(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgDelay();

// Dialog Data
	enum { IDD = IDD_DLG_DELAY };
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
