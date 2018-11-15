#pragma once


// CDlgTool dialog

class CDlgTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgTool)

public:
	CDlgTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTool();
    double  m_dJethightDis; 
// Dialog Data
	enum { IDD = IDD_DLG_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	tgCmdLine *m_pCmdLine;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeDlgToolRadioHead();
	afx_msg void OnEnChangeDlgToolZHeight2();
	afx_msg void OnBnClickedApp();
	afx_msg void OnBnClickedCancel();
};
