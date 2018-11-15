#pragma once


// CDlgNote dialog

class CDlgNote : public CDialog
{
	DECLARE_DYNAMIC(CDlgNote)

public:
	CDlgNote(CWnd* pParent = NULL);   // standard constructor
	CDlgNote(CString strText,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNote();

	tgCmdLine *m_pCmdLine;
// Dialog Data
	enum { IDD = IDD_DLG_NOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
private:
	CString m_strText;
public:
	int	m_iSelIndex;//µ±Ç°ÐÐºÅ
	virtual BOOL OnInitDialog();
	void RefreshAxisPos(void);
	afx_msg void OnBnClickedBtnReadAxispos();
	afx_msg void OnBnClickedBtnMove();
};
