#pragma once


// CDlgPointDispense dialog

class CDlgPointParam : public CDialog
{
	DECLARE_DYNAMIC(CDlgPointParam)

public:
	CDlgPointParam(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPointParam();

// Dialog Data
	enum { IDD = IDD_DLG_POINT_PARAM };

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
