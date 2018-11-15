#include "resource.h"
#include "JogButton.h"
#include "./CButtonST/BtnST.h"

#pragma once

// CDlgShotCut dialog

class CDlgShotCut : public CDialog
{
	DECLARE_DYNAMIC(CDlgShotCut)

public:
	CDlgShotCut(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgShotCut();

// Dialog Data
	enum { IDD = IDD_DLG_SHOTCUT };
	CJogButton m_JogButton[8];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	///////////////
	afx_msg void OnBnClickedJogExt();
	afx_msg void OnBnClickedJogCheckStep();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeJogEditStepLength();
	double m_fJogStep;
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedTestdisp();
	CScrollBar m_scrollBar;
	afx_msg void OnClickedCheckConveyor();
};
