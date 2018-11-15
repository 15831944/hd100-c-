#pragma once


// CDLgSpeed dialog

class CDLgSpeed : public CDialog
{
	DECLARE_DYNAMIC(CDLgSpeed)

public:
	CDLgSpeed(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLgSpeed();
	tgCmdLine *m_pCmdLine;

// Dialog Data
	enum { IDD = IDD_DLG_SPEED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedRadio1(UINT ID);
	afx_msg void OnBnClickedOk();
};
