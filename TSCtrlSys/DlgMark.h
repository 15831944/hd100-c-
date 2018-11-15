#pragma once
#include "paramdef.h"

// CDlgMark dialog

class CDlgMark : public CDialog
{
	DECLARE_DYNAMIC(CDlgMark)

public:
	CDlgMark(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMark();
    BOOL ShowMarkPat(int nGid,int nMarkID);

// Dialog Data
	enum { IDD = IDD_DLG_MARK };
	tgCmdLine *m_pCmdLine; // ÷∏¡Óƒ⁄»›
	int		m_iSelIndex;   // ÷∏¡ÓID 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedOk();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonLearn();
	afx_msg void OnBnClickedButtonPatTest();
	afx_msg void OnBnClickedButtonMove();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboMark();
	afx_msg void OnBnClickedButtonPatSave();
protected:
	virtual void OnOK();
public:
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboMarkmodeid();
	bool m_bIsEditMod;
	bool m_bLBtnDown;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnEdit();
};