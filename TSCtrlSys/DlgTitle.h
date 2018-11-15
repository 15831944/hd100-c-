#pragma once
#include "afxwin.h"


// CDlgTitle 对话框

class CDlgTitle : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTitle)

public:
	CDlgTitle(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTitle();

// 对话框数据
	enum { IDD = IDD_DLG_TITLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

//	CButtonST m_btnKFLog;
	CButtonST m_btnClose;
	afx_msg void OnBnClickedBtnClose();
};
