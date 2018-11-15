#pragma once
#include "afxwin.h"


// CDlgTrack 对话框

class CDlgTrack : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTrack)

public:
	CDlgTrack(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTrack();

// 对话框数据
	enum { IDD = IDD_DLG_TRACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAskPre();
	virtual BOOL OnInitDialog();

	void InitCtrl();		// 初始化控件
	void UpdataIOState(long inPutStatus, long exInput, long outPutStatus);	// 初始化控件
	void UpdaOutputStatus(long outPutStatus);

	HICON m_hIcon_On;
	HICON m_hIcon_Off;

	CStatic m_static_pre;
	CStatic m_static_next;
	CStatic m_static_11;
	CStatic m_static_12;
	CStatic m_static_13;
	CStatic m_static_14;
	CStatic m_static_stop_1;
	CStatic m_static_stop_2;
	CStatic m_static_lift_1;
	CStatic m_static_lift_2;
	CStatic m_static_clamp;

	CShadeButtonST m_btn_ask_pre;
	CShadeButtonST m_btn_ask_next;
	CShadeButtonST m_btn_stop_1;
	CShadeButtonST m_btn_stop_2;
	CShadeButtonST m_btn_lift;
	CShadeButtonST m_btn_clamp;
	CShadeButtonST m_btn_board_load;
	CShadeButtonST m_btn_board_fix;
	CShadeButtonST m_btn_board_unload;
	CShadeButtonST m_btn_board_unfix;
	CShadeButtonST m_btn_track_right;
	CShadeButtonST m_btn_track_left;

	//////////////////////////////////////////////////////////////////////////

	afx_msg void OnBnClickedBtnAskNext();
	afx_msg void OnBnClickedBtnStop11();
	afx_msg void OnBnClickedBtnStop12();
	afx_msg void OnBnClickedBtnLift();
	afx_msg void OnBnClickedBtnClamp1();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnFix();
	afx_msg void OnBnClickedBtnUnfix();
	afx_msg void OnBnClickedBtnUnload();
	afx_msg void OnBnClickedBtnTrackL();
	afx_msg void OnBnClickedBtnTarckR();
	// 外挂配置
	int m_btnRadioAdd;
	// 物料定位方式
	int m_btnRadioFix;
	afx_msg void OnBnClickedRadioAdd();
	afx_msg void OnBnClickedRadioFix();

	//////////////////////////////////////////////////////////////////////////
	CStatic m_static_rev_stop1;
	CStatic m_static_rev_stop2;
	CStatic m_static_rev_clamp1;
	CStatic m_static_rev_clamp2;
	CStatic m_static_rev_dec1;
	CStatic m_static_rev_dec2;
	CStatic m_static_rev_revChk1;
	CStatic m_static_rev_revChk2;

	CShadeButtonST m_btn_rev_home;
	CShadeButtonST m_btn_rev_board;
	CShadeButtonST m_btn_work2rev;
	CShadeButtonST m_btn_rev2work;
	CShadeButtonST m_btn_rev_stop1;
	CShadeButtonST m_btn_rev_stop2;
	CShadeButtonST m_btn_rev_clamp;
	CShadeButtonST m_btn_rev_track_l;
	CShadeButtonST m_btn_rev_track_r;
	afx_msg void OnBnClickedChkRevStop1();
	afx_msg void OnBnClickedChkRevStop2();
	afx_msg void OnBnClickedChkRevClamp();
	afx_msg void OnBnClickedBtnRevBoard();
	afx_msg void OnBnClickedBtnWork2rev();
	afx_msg void OnBnClickedBtnRev2work();
	afx_msg void OnBnClickedBtnTrackL2();
	afx_msg void OnBnClickedBtnTarckR2();
	afx_msg void OnBnClickedBtnRevHome();
	void EnableReverseBtn(BOOL bEnable);
	double m_dDotRadius;
	afx_msg void OnBnClickedBtnSave();
	int m_nAutoRpsDots;
	double m_dAutoRpsInterval;
	int m_nCleanInerval;
};
