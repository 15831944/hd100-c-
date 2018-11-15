#pragma once


// CDlgZSet dialog

class CDlgZSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgZSet)

public:
	CDlgZSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgZSet();

// Dialog Data
	enum { IDD = IDD_DLG_ZSET };
	tgCmdLine *m_pCmdLine;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditZset();
};
