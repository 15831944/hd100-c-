#pragma once


// CDlgJetParam dialog

class CDlgJetParam : public CDialog
{
	DECLARE_DYNAMIC(CDlgJetParam)

public:
	CDlgJetParam(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgJetParam();

// Dialog Data
	enum { IDD = IDD_DLG_JET_PARAM };
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
