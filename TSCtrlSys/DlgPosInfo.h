#pragma once

#include "./CButtonST/BtnST.h"
#include "GradientProgressCtrl/GradientProgressCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"
// CDlgPosInfo dialog

class CDlgPosInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgPosInfo)

public:
	CDlgPosInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPosInfo();

// Dialog Data
	enum { IDD = IDD_DLG_INFO };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CBrush m_brushBk;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void UpdateMotionState();
    void UpdateFileShowState(BOOL bshow);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

   BOOL bNeedShowInfo;
public:
	CShadeButtonST m_BtnNetMode;
	CShadeButtonST m_btn_test;
	afx_msg void OnBnClickedDlgInfoMode();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnCcdTrigger();
	afx_msg void OnBnClickedBtnLoadImage();
	void UpsysWorkMode(void);

	void SetLoadStatus(CString strTemp);		// 显示进料状态
	void SetUnloadStatus(CString strTemp);		// 显示下料状态
	void SetCurWasteGlue(CString strTemp);		// 显示废胶盒胶量
	CFont m_Font; 
	CWEdit m_edit_pos_x;
	CWEdit m_edit_pos_y;
	CWEdit m_edit_pos_z;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnWasteglue();
	bool m_bWasteGlueAlarm;
	CButtonST m_btnPath;
	CButtonST m_btnMachineSet;
	CButtonST m_btnUser;
	CButtonST m_btnJetAdjust;
	afx_msg void OnBnClickedBtnPath();
	afx_msg void OnBnClickedBtnMov();


	CButtonST m_btnRun;
	CButtonST m_btnStop;
	CButtonST m_btnPause;
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnMultivalveSet();

	CStatic m_staticProgName;
	void SetCurProgName();
	afx_msg void OnBnClickedBtnLoadBoard();
	afx_msg void OnBnClickedBtnUnloadBoard();
};
