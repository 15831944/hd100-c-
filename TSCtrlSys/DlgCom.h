#pragma once


// CDlgComConfig dialog

class CDlgComConfig : public CDialog
{
	DECLARE_DYNAMIC(CDlgComConfig)

public:
	CDlgComConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgComConfig();

// Dialog Data
	enum { IDD = IDD_DLG_SYNC_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
