#pragma once


// CPageImg dialog

class CPageImg : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageImg)

public:
	CPageImg();
	virtual ~CPageImg();

// Dialog Data
	enum { IDD = IDD_PAGE_IMG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnApply();
	virtual BOOL OnInitDialog();

	virtual BOOL OnApply();
	afx_msg void OnEnChangePageImgR1();
	afx_msg void OnEnChangePageImgR2();
	afx_msg void OnBnClickedPageImgCheckXdir();
	afx_msg void OnBnClickedPageImgCheckYdir();
	afx_msg void OnCbnSelchangeComboMarknum();
};
