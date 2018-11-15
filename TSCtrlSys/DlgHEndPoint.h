#pragma once
#include "Resource.h"
#include "paramdef.h" 
// CDlgHEndPoint dialog

class CDlgHEndPoint : public CDialog
{
	DECLARE_DYNAMIC(CDlgHEndPoint)

public:
	CDlgHEndPoint(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgHEndPoint();
   
// Dialog Data
	enum { IDD = IDD_DLG_HPOINT_END };

	tgCmdLine *m_pCmdLine;
	int	m_iSelIndex;//µ±Ç°ÐÐºÅ
	void RefreshAxisPos(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnReadAxispos();
	afx_msg void OnBnClickedBtnMove();
};
