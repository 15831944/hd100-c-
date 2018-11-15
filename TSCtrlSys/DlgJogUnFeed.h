#pragma once
#include "JogButton.h"

// CDlgJogUnFeed dialog

class CDlgJogUnFeed : public CDialog
{
	DECLARE_DYNAMIC(CDlgJogUnFeed)

public:
	CDlgJogUnFeed(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgJogUnFeed();
	CJogButton	m_MNBtn;
	CJogButton	m_MPBtn;
	CJogButton	m_NNBtn;
	CJogButton	m_NPBtn;
// Dialog Data
	enum { IDD = IDD_DLG_JOG_UNFEED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedSetJogxyzEditSpeed();
};
