#pragma once

// CDlgProgSel 对话框

class CDlgProgSel : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProgSel)

public:
	CDlgProgSel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProgSel();

	CGridCtrl m_progListGrid;
// 对话框数据
	enum { IDD = IDD_DLG_PROG_SEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
