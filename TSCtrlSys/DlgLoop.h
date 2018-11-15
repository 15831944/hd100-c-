#pragma once


// CDlgLoop dialog

class CDlgLoop : public CDialog
{
	DECLARE_DYNAMIC(CDlgLoop)

public:
	CDlgLoop(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLoop();

// Dialog Data
	enum { IDD = IDD_DLG_LOOP };

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