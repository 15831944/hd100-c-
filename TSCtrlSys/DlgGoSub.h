#pragma once


// CDlgGoSub dialog

class CDlgGoSub : public CDialog
{
	DECLARE_DYNAMIC(CDlgGoSub)

public:
	CDlgGoSub(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGoSub();

// Dialog Data
	enum { IDD = IDD_DLG_GOSUB };
	tgCmdLine *m_pCmdLine;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonMove();
	afx_msg void OnBnClickedButtonReadAxispos();
	afx_msg void OnBnClickedGosubRadio1();
	afx_msg void OnBnClickedGosubRadio2();
public:
	void RefreshAxisPos(void);
};
