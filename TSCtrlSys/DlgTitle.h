#pragma once
#include "afxwin.h"


// CDlgTitle �Ի���

class CDlgTitle : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTitle)

public:
	CDlgTitle(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTitle();

// �Ի�������
	enum { IDD = IDD_DLG_TITLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

//	CButtonST m_btnKFLog;
	CButtonST m_btnClose;
	afx_msg void OnBnClickedBtnClose();
};
