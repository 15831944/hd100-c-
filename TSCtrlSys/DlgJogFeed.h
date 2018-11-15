#pragma once
#include "JogButton.h"

// CDlgJogFeed dialog

class CDlgJogFeed : public CDialog
{
	DECLARE_DYNAMIC(CDlgJogFeed)

public:
	CDlgJogFeed(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgJogFeed();

// Dialog Data
	enum { IDD = IDD_DLG_JOG_FEED };
	CJogButton	m_VNBtn;
	CJogButton	m_VPBtn;
	CJogButton	m_WNBtn;
	CJogButton	m_WPBtn;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
};
