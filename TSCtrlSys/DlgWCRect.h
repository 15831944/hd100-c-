#pragma once
#include "Resource.h"
#include "paramdef.h"
#include "afxwin.h"

// CDlgWCRect dialog

class CDlgWCRect : public CDialog
{
	DECLARE_DYNAMIC(CDlgWCRect)

public:
	CDlgWCRect(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgWCRect();
	tgCmdLine *m_pCmdLine;
	int m_iSelIndex;
	void RefreshAxisPos();
	void CheckPara();
// Dialog Data
	enum { IDD = IDD_DLG_WCRECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked3();
	afx_msg void OnCbnSelchangeDlgCombWcrpos();
	afx_msg void OnBnClicked1();
	virtual BOOL OnInitDialog();
	CComboBox m_ComboxCtrl;
	int m_dWcPointNum;      // 出点个数
	double m_dWCRectTatolW;    // 总重量
	double m_dWcRectVel;       // 运动速度
	double m_dWCLPointData;    // 单粒重量
	afx_msg void OnBnClicked2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
