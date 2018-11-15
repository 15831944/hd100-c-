#pragma once


// CDlgZHeight dialog

class CDlgZHeight : public CDialog
{
	DECLARE_DYNAMIC(CDlgZHeight)

public:
	CDlgZHeight(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgZHeight();
	tgCmdLine *m_pCmdLine;
	int m_iSelIndex;
// Dialog Data
	enum { IDD = IDD_DLG_ZHEIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void RefreshAxisPos();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedZheightApply();
	afx_msg void OnBnClickedZheightReadAxispos();
	afx_msg void OnBnClickedZheightMove();
	afx_msg void OnBnClickedZheightSave();
	afx_msg void OnBnClickedCancel();
};
