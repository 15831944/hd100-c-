#pragma once


// DlgHDot 对话框

class CDlgHDot : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHDot)

public:
	CDlgHDot(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHDot();

// 对话框数据
	enum { IDD = IDD_DLG_HPOINT_DOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	tgCmdLine *m_pCmdLine;
	int	m_iSelIndex;//当前行号
	void RefreshAxisPos(void);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnReadAxispos();
	afx_msg void OnBnClickedBtnMove();
	virtual BOOL OnInitDialog();
};
