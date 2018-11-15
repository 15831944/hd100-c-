#pragma once


// CDlgMachineParam 对话框

class CDlgMachineParam : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMachineParam)

public:
	CDlgMachineParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMachineParam();

// 对话框数据
	enum { IDD = IDD_DLG_MACHINE_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nRadioCamera;
	int m_nRadioHeight;
	int m_nRadioLdUld;
	int m_nAutoClean;
	int m_nAutoRps;
	int m_nAutoRpsDots;
	double m_dAutoRpsIntv;
	double m_dCleanDownMM;
	int m_nCleanIntv;
	int m_nCleanNum;
	double m_dImageDotD;
	int m_nRadioWorkMode;
	int m_nIfGlueWarning;
	double m_dProbeHSpd;
	double m_dProbeLSpd;
	double m_dProbeLHeight;
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioCamera();
	afx_msg void OnRadioHeight();
	afx_msg void OnRadioLduld();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnClean();
	void UpdateCtrl(BOOL bShow);


	int m_nTestLen;
	int m_nTestSpace;
	int m_nIfAutoTest;
	double m_dTestVel;
	afx_msg void OnBnClickedBtnMovBd1();
	afx_msg void OnBnClickedBtnSetBd1();
	afx_msg void OnBnClickedBtnMovBd2();
	afx_msg void OnBnClickedBtnSetBd2();
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedBtnPosClean();
	CButtonST m_btnMov1;
	CButtonST m_btnMov2;
	CButtonST m_btnSet1;
	CButtonST m_btnSet2;
	CButtonST m_btnSave;
	afx_msg void OnBnClickedBtnNewGlue();
	int m_nIfSafeDoor;
	int m_nRadioGlueWarningMode;
	int m_nGlueWarningMin;
	int m_nGlueWarningPcs;
	int m_nIfUseCameraNeedle;
	int m_nIfRotateWorkpiece;
	int m_nValveNum;
};
