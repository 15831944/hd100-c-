#pragma once


// CDlgIOSet dialog

class CDlgIOSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgIOSet)

public:
	CDlgIOSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgIOSet();

// Dialog Data
	enum { IDD = IDD_DLG_SETIO };

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
