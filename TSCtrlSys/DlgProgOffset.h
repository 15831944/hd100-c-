#pragma once


// CDlgProgOffset dialog

class CDlgProgOffset : public CDialog
{
	DECLARE_DYNAMIC(CDlgProgOffset)

public:
	CDlgProgOffset(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgProgOffset();

// Dialog Data
	enum { IDD = IDD_DLG_PROG_OFFSET };
	tgCmdLine *m_pCmdLine;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnApply();
	virtual BOOL OnInitDialog();
public:
	int m_iStartIndex;
	int m_iEndIndex;
protected:
	virtual void OnOK();
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedCancel();
};
