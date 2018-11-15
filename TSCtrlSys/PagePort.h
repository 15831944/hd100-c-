#pragma once


// CPagePort dialog

class CPagePort : public CPropertyPage
{
	DECLARE_DYNAMIC(CPagePort)

public:
	CPagePort();
	virtual ~CPagePort();

// Dialog Data
	enum { IDD = IDD_PAGE_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnEnChangeEditOutput(UINT ID);
	afx_msg void OnBnClickedBtread();
	afx_msg void OnBnClickedBtwrite();
	long m_WorkMode;
	long m_Count;
	long m_OpenTime;
	long m_CloseTime;
	long m_pressJet;
	long m_pressDisp;
	afx_msg void OnEnChangeEdit1(UINT ID);
};
