#pragma once
#include "Resource.h"
#include "paramdef.h"
// CDlgWcPoint dialog

class CDlgWcPoint : public CDialog
{
	DECLARE_DYNAMIC(CDlgWcPoint)

public:
	CDlgWcPoint(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgWcPoint();

	tgCmdLine *m_pCmdLine;
	int m_iSelIndex;
	void RefreshAxisPos();
// Dialog Data
	enum { IDD = IDD_DLG_WCPOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClicked2();
	afx_msg void OnBnClicked3();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int m_WLPNum;                // 称重出胶次数
	double  m_WPLDrop;           // 单点重量
	double m_dWCLPointTDate;     // 总重量
};
