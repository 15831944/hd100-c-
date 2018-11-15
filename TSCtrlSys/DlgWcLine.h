#pragma once
#include "Resource.h"
#include "paramdef.h"
#include "afxwin.h"
// CDlgWcLine dialog

class CDlgWcLine : public CDialog
{
	DECLARE_DYNAMIC(CDlgWcLine)

public:
	CDlgWcLine(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgWcLine();
	tgCmdLine *m_pCmdLine;
	int m_iSelIndex;
	void RefreshAxisPos();
// Dialog Data
	enum { IDD = IDD_DLG_WCL };
    void CheckPara();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClicked3();
	afx_msg void OnCbnSelchangeDlgCombPos();
	CComboBox m_ComboSelPos;
	afx_msg void OnBnClicked2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int m_WcLineNum;
	double m_dWCLPointData;    // 单粒重量
	double m_dWCLineTDate;     // 线段总重
	afx_msg void OnEnChangeWclLinespeed();
	double m_dLineSpeed;
	afx_msg void OnEnChangeWclLineeight();
	
};
