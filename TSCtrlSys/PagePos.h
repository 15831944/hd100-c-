#pragma once


// CPagePos dialog

class CPagePos : public CPropertyPage
{
	DECLARE_DYNAMIC(CPagePos)

public:
	CPagePos();
	virtual ~CPagePos();

// Dialog Data
	enum { IDD = IDD_PAGE_POS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPagePosWashMoveto();
	afx_msg void OnBnClickedPagePosWashSave();
	afx_msg void OnBnClickedPagePosHomeMoveto();
	afx_msg void OnBnClickedPagePosHomeSave();
	afx_msg void OnBnClickedPagePosZadjustMoveto();
	afx_msg void OnBnClickedPagePosZadjustSave();
	afx_msg void OnBnClickedPagePosJetMoveto();
	afx_msg void OnBnClickedPagePosJetSave();
	afx_msg void OnBnClickedPagePosNeedleMoveto();
	afx_msg void OnBnClickedPagePosNeedleSave();
	virtual BOOL OnApply();
	afx_msg void OnBnClickedPagePosJetZadjustMoveto();
	afx_msg void OnBnClickedPagePosJetZadjustSave();
	afx_msg void OnBnClickedPagePosTestjetMoveto();
	afx_msg void OnBnClickedPagtestjetSave();
	afx_msg void OnBnClickedPagePosAfpsMoveto();
	afx_msg void OnBnClickedPagePosAfpsCalib();
};
