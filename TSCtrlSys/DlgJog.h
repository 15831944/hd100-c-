#pragma once

#include "JogButton.h"
#include "./CButtonST/BtnST.h"
// CDlgJog dialog

class CDlgJog : public CDialog
{
	DECLARE_DYNAMIC(CDlgJog)

public:
	CDlgJog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgJog();

// Dialog Data
	enum { IDD = IDD_DLG_JOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	CButtonST m_chkTripperCheckbox;
	CButtonST m_chkRotateCheckbox;
	CButtonST m_chkUpdownCheckbox;


	DECLARE_MESSAGE_MAP()
public:
	CJogButton m_JogButton[8];
	double m_dSpdRatio;
	double m_fJogStep;
	double m_fXYSpeed;
	double m_fZSpeed;
public:
	virtual BOOL OnInitDialog();
	void UpdateJogBarWindow();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedJogBtnDispWash();
	afx_msg void OnBnClickedJogBtnDispQiya();
	afx_msg void OnCbnSelchangeJogOperRadioHead();
	afx_msg void OnBnClickedJogBtnDispTripper();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButZero();
	afx_msg void OnBnJogCheckStep();
	afx_msg void OnEnChangeJogEditStepLength();
	afx_msg void OnEnChangeXYSpeed();
};
