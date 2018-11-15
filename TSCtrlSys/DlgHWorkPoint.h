#pragma once
#include "Resource.h"
#include "paramdef.h" 

// CDlgHWorkPoint dialog

class CDlgHWorkPoint : public CDialog
{
	DECLARE_DYNAMIC(CDlgHWorkPoint)

public:
	CDlgHWorkPoint(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgHWorkPoint();

    tgCmdLine *m_pCmdLine;
    int	m_iSelIndex;//µ±Ç°ÐÐºÅ
    void RefreshAxisPos(void);

// Dialog Data
	enum { IDD = IDD_DLG_HPOINT_POINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnReadAxispos();
	afx_msg void OnBnClickedBtnMove();
	virtual BOOL OnInitDialog();
};
