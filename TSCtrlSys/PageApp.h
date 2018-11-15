#pragma once


// CPageApp dialog

class CPageApp : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageApp)

public:
	CPageApp();
	virtual ~CPageApp();

// Dialog Data
	enum { IDD = IDD_PAGE_APP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

public:
	 afx_msg void OnBnClickedPageAppRadisp1();
	 int m_iWashCount;
	 int m_iWashTime;
	 afx_msg void OnEnChangePageAppWashCount();
	 afx_msg void OnBnClickedPageAppRadisp3();
	 afx_msg void OnEnChangePageAppWashTimer();

	 afx_msg void OnBnClickedPageAppRadist1();
	 afx_msg void OnBnClickedPageAppRadisidcPageAppRadist2();
	 afx_msg void OnBnClickedCheckGostandbypos();
	 afx_msg void OnBnClickedPageAppRadist2();
	 afx_msg void OnBnClickedPageAppRadistWorkmode1();
	 afx_msg void OnBnClickedPageAppRadistWorkmode2();
	 afx_msg void OnEnChangeEditFlyjetdelay();
	// afx_msg void OnBnClickedCheckFilesapter();
	 afx_msg void OnBnClickedCheckEnableworktegether();
	 afx_msg void OnEnChangeEditMakeupnum();
	 afx_msg void OnBnClickedCheckEnablesqerteanle();
	 afx_msg void OnBnClickedPageAppRadisp2();
	 afx_msg void OnEnChangeEditAutorpsdelay();
	 double m_nPRSDelay;
	 double m_nPRSTimes;
	 double m_nSPRSTime;
	 double m_nSPRSDelay;
	 afx_msg void OnEnChangeEditAutorpstimes();
	 afx_msg void OnEnChangeEditAutosrpstime();
	 afx_msg void OnEnChangeEditAutosrpsdelay();
	 double m_dWeightMax;
	 double m_dWeightMin;
	 afx_msg void OnEnChangePageAppWeghtMaxlmt();
	 afx_msg void OnEnChangePageAppWeghtMinlmt();
	 afx_msg void OnBnClickedCheckGostandbypos2();
};
