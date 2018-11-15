#pragma once


// DlgRepeatZheight dialog

class CDlgRepeatZheight : public CDialog
{
	DECLARE_DYNAMIC(CDlgRepeatZheight)

public:
	CDlgRepeatZheight(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRepeatZheight();

// Dialog Data
	enum { IDD = IDD_DLG_REPEAT_ZMETER };
	tgCmdLine *m_pCmdLine;
	int m_iSelIndex;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRepeatZheightApply();
	afx_msg void OnBnClickedRepeatZheightSave();
	afx_msg void OnBnClickedRepeatZheightMove();
	afx_msg void OnBnClickedRepeatZheightReadAxispos();
	virtual BOOL OnInitDialog();
	void RefreshAxisPos();
protected:
	virtual void OnOK();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedOk();
};
