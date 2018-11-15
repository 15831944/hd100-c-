#pragma once


// CPageLook dialog

class CPageLook : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageLook)

public:
	CPageLook();
	virtual ~CPageLook();

// Dialog Data
	enum { IDD = IDD_PAGE_LOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnEnChangeEditP1(UINT Id);
};
