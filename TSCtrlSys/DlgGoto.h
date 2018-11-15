#pragma once


// CDlgGoto dialog
class CDlgGoto : public CDialog
{
	DECLARE_DYNAMIC(CDlgGoto)

public:
	CDlgGoto(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGoto();

// Dialog Data
	enum { IDD = IDD_DLG_GOTO };
	tgCmdLine *m_pCmdLine;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	//int m_iStartIndex;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGotoRadio1();
	afx_msg void OnBnClickedGotoRadio2();
	afx_msg void OnCbnSelchangeGotoComboLabel();
	afx_msg void OnEnChangeGotoEditStartCmd();
};