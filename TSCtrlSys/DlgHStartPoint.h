#pragma once
#include "Resource.h"
#include "paramdef.h" 

// CDlgHStartPoint dialog

class CDlgHStartPoint : public CDialog
{
	DECLARE_DYNAMIC(CDlgHStartPoint)

public:
	CDlgHStartPoint(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgHStartPoint();

// Dialog Data
	enum { IDD = IDD_DLG_HPOINT_START };

	tgCmdLine *m_pCmdLine;
	int	m_iSelIndex;//µ±Ç°ÐÐºÅ
	void RefreshAxisPos(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnReadAxispos();
	afx_msg void OnBnClickedBtnMove();
	afx_msg void OnBnClickedCancel();
};
