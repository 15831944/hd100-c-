#pragma once


// CPageJet dialog

class CPageJet : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageJet)

public:
	CPageJet();
	virtual ~CPageJet();

// Dialog Data
	enum { IDD = IDD_PAGE_JET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_PageJetPulse;
	int m_PageJetOffTime;
	int m_PageJetOnTime;
	double m_PageJetPress1;
	double m_PageJetPress2;
	double m_PageJetTp;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangePageJetEditMode();
	afx_msg void OnEnChangePageJetEditPulse();
	afx_msg void OnEnChangePageJetEditOntime();
	afx_msg void OnEnChangePageJetEditOfftime();
	afx_msg void OnEnChangePageJetEditPress1();
	afx_msg void OnEnChangePageJetEditPress2();
	afx_msg void OnEnChangePageJetEditTp();
	virtual BOOL OnApply();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonClosepress();
	afx_msg void OnBnClickedButtonOpenpress();
	afx_msg void OnBnClickedButtonOpenhreat();
};
