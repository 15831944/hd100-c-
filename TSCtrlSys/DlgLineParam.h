#pragma once


// CDlgLineDispense dialog

class CDlgLineParam : public CDialog
{
	DECLARE_DYNAMIC(CDlgLineParam)

public:
	CDlgLineParam(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLineParam();

// Dialog Data
	enum { IDD = IDD_DLG_LINE_PARAM };
	
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
