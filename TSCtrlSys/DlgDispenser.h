#pragma once


// CDispenseDlg dialog

class CDlgDispenser : public CDialog
{
	DECLARE_DYNAMIC(CDlgDispenser)

public:
	CDlgDispenser(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgDispenser();

// Dialog Data
	enum { IDD = IDD_DLG_DISPENSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	tgCmdLine *m_pCmdLine;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeRadioHead();
	afx_msg void OnBnClickedCheckSideUpdown();
};
