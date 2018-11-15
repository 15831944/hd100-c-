#pragma once
#include "Resource.h"
// CPageBanleceSet dialog

class CPageBanleceSet : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageBanleceSet)

public:
	CPageBanleceSet();
	virtual ~CPageBanleceSet();
    BOOL  bNeedBC;
	CString m_strCaption;
// Dialog Data
	enum { IDD = IDD_PAGE_BATET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtSetbancelepos();
	afx_msg void OnBnClickedBtMovetobcpos();
	afx_msg void OnBnClickedBtBanlenceZero();
	double m_dBCPosX;
	double m_dBCPosY;
	double m_dBCPosZ;
	double m_dOpenDispTime;
	afx_msg void OnBnClickedButtonDobc();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonClacbcpd();
	double m_dDispDis;
	afx_msg void OnEnChangeEditDispdistime();
	afx_msg void OnEnChangeEditXposBanlece();
	afx_msg void OnEnChangeEditYposBanlece();
	afx_msg void OnEnChangeEditZposBanlece();
	double m_BCDispNum;
	double m_BCMProDot;
	double m_BCMCurData;
	afx_msg void OnEnChangeEditOpendisptime();
	afx_msg void OnEnChangeEditBcdispnum();
	afx_msg void OnEnChangeEditBcggpb();
	virtual BOOL OnApply();
};
