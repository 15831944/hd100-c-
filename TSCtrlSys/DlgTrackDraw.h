#pragma once
#include "RedrawTrack.h"

// CDlgTrackDraw 对话框

class CDlgTrackDraw : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTrackDraw)

public:
	CDlgTrackDraw(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTrackDraw();

// 对话框数据
	enum { IDD = IDD_DLG_TRACKDRAW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CRedrawTrack m_Track;
	CRect m_rtClient;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	int m_radio1;
	afx_msg void OnRadio2();
	afx_msg void OnBnClickedRadio1();

	BOOL m_bFront;
};
