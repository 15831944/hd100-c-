#pragma once


// DlgHDot �Ի���

class CDlgHDot : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHDot)

public:
	CDlgHDot(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHDot();

// �Ի�������
	enum { IDD = IDD_DLG_HPOINT_DOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	tgCmdLine *m_pCmdLine;
	int	m_iSelIndex;//��ǰ�к�
	void RefreshAxisPos(void);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnReadAxispos();
	afx_msg void OnBnClickedBtnMove();
	virtual BOOL OnInitDialog();
};
