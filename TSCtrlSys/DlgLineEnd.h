#pragma once


// CDlgLineEnd dialog

class CDlgLineEnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgLineEnd)

public:
	CDlgLineEnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLineEnd();

// Dialog Data
	enum { IDD = IDD_DLG_LINE_END };
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
	afx_msg void OnBnClickedRadio5();
};
