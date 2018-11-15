#pragma once
#include "afxwin.h"
#include "ComDisp.h"

// CDlgDispenserParam 对话框
UINT ThreadDotDisp(LPVOID lParam);

class CDlgDispenserParam : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDispenserParam)

public:
	CDlgDispenserParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDispenserParam();

// 对话框数据
	enum { IDD = IDD_DLG_DISPPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLoadFromDisp();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void InitScrollBar();
	void InitStaticCtrl(int dispType);
	CScrollBar m_scrollRising;
	CScrollBar m_scrollOpen;
	CScrollBar m_scrollLift;
	CScrollBar m_scrollFalling;
	CScrollBar m_scrollDelay;
	CScrollBar m_scrollPulse;
	CScrollBar m_scrollTemp;

	// 点胶阀索引
	CComboBox m_comboDispIdx;
	// 点胶阀类型
	CComboBox m_comboDispType;
	// 控制方式		0:轴脉冲		1:普通IO		2:位置比较高速IO
	CComboBox m_comboDispCtrl;
	// 当前点胶参数
	DispenserParam m_dispParam;
	afx_msg void OnBnClickedBtnLoadFromFile();
	afx_msg void OnBnClickedBtnSetParam();
//	afx_msg void OnBnClickedBtnSetWtpos();
	afx_msg void OnBnClickedBtnReadPressure();
//	afx_msg void OnBnClickedBtnMvtoWtpos();
	void MvtoWtpos(void);
	afx_msg void OnBnClickedBtnWtAdj();
	afx_msg void OnBnClickedBtnValveOn();
	afx_msg void OnCbnSelchangeComboDispType();
	afx_msg void OnCbnSelchangeComboDispSel();

	void UpdataCtrlVal(bool bIfShow);	// 刷新界面空间值   bIfShow--true:显示保存值到界面    false:从界面读取值保存
	afx_msg void OnBnClickedBtnSetPressure();

//	CWEdit m_edit_wtPos_x;
	int m_nWeightType;
	afx_msg void OnBnClickedChkWeight();
	afx_msg void OnBnClickedRadioT();
	afx_msg void OnBnClickedRadioP();
	afx_msg void OnCbnSelchangeComboDispCtrl();
};
