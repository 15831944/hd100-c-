#pragma once

// CDlgProgSel �Ի���

class CDlgProgSel : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProgSel)

public:
	CDlgProgSel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgProgSel();

	CGridCtrl m_progListGrid;
// �Ի�������
	enum { IDD = IDD_DLG_PROG_SEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedBtnSel();
	virtual BOOL OnInitDialog();

	void CreateGrid();
	void UpdataGrid();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedOk();
};
