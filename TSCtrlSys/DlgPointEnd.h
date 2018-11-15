#pragma once


// CDlgLineEnd dialog

class CDlgPointEnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgPointEnd)

public:
	CDlgPointEnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPointEnd();

// Dialog Data
	enum { IDD = IDD_DLG_POINT_END };
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
