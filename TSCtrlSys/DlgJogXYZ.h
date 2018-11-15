#pragma once

//#include "TriangleButton.h"
#include "JogButton.h"

// CDlgJogXYZ dialog

class CDlgJogXYZ : public CDialog
{
	DECLARE_DYNAMIC(CDlgJogXYZ)

public:
	CDlgJogXYZ(const CString& str,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgJogXYZ();

// Dialog Data
	enum { IDD = IDD_DLG_JOG_XYZ };

	CJogButton	m_XNBtn;
	CJogButton	m_XPBtn;
	CJogButton	m_YNBtn;
	CJogButton	m_YPBtn;
	CJogButton	m_ZNBtn;
	CJogButton	m_ZPBtn;
	
	//CJogButton m_JogButton[6];

private:
	CString m_strCaption;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSetJogxyzCheckStep();
	double m_fJogStep;
	afx_msg void OnEnChangeSetJogxyzEditStep();
};
