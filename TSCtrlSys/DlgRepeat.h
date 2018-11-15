#pragma once


// CDlgRepeat dialog

class CDlgRepeat : public CDialog
{
	DECLARE_DYNAMIC(CDlgRepeat)

public:
	CDlgRepeat(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRepeat();
	tgCmdLine *m_pCmdLine;
	int m_iSelIndex;

	CButtonST m_btn[6];

	BOOL m_bExpand; // 阵列是否展开
// Dialog Data
	enum { IDD = IDD_DLG_REPEAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void UpdateCtrl(BOOL bIfShow);	// 更新控件数值		bIfShow= TRUE:将数值显示到控件    FALSE:从控件读取数值到内存
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonExpand();

// 	afx_msg void OnEnChangeRepeatEditXspace();
// 	afx_msg void OnEnChangeRepeatEditXnum();
// 	afx_msg void OnEnChangeRepeatEditIndex();
// 	afx_msg void OnEnChangeRepeatEditReparc();
//	afx_msg void OnBnClickedButtonArcmark();
	afx_msg void OnBnClickedBtnSet1();
	afx_msg void OnBnClickedBtnMov1();
	afx_msg void OnBnClickedBtnSet2();
	afx_msg void OnBnClickedBtnMov2();
	afx_msg void OnBnClickedBtnSet3();
	afx_msg void OnBnClickedBtnMov3();
	double m_dLTx;
	double m_dLTy;
	double m_dRBx;
	double m_dRBy;
	double m_dRTx;
	double m_dRTy;
	int m_nXnum;
	double m_dXSpace;
	int m_nYnum;
	double m_dYSpace;
	int m_nIndex;
	int m_nIndexEnd;
	int m_nXStart;
	int m_nYStart;
	int m_nDispUnitNum;
};
