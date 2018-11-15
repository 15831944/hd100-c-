#pragma once
#include "DlgJetAdjust.h"
#include "DlgMachineParam.h"
#include "DlgLdUld.h"

// CDlgMachineSet 对话框

class CDlgMachineSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMachineSet)

public:
	CDlgMachineSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMachineSet();

// 对话框数据
	enum { IDD = IDD_DLG_MACHINE_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDlgJetAdjust		m_pageJetAdjust;
	CDlgMachineParam	m_pageMachineParam;
	CDlgLdUld			m_pageLoadUnload;
	CTabCtrl m_tabCtrl;
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	void InitTableCtrl();
};
