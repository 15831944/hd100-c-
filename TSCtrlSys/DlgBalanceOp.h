#pragma once
#include "Resource.h"
#include "afxwin.h"
#include "afxcmn.h"

// CDlgBalanceOp dialog

class CDlgBalanceOp : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgBalanceOp)

public:
	CDlgBalanceOp();   // standard constructor
	virtual ~CDlgBalanceOp();
   
	int fileID;
// Dialog Data
	enum { IDD = IDD_PAGE_BATET_BSW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtBcGetdata2();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtBanlenceZero2();
	afx_msg void OnBnClickedBtBanlenceZero();
	afx_msg void OnBnClickedBtBcGetdata3();
	void UpdateBClist(int num);
	CListCtrl m_listBC;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
};
