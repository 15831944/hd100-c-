#pragma once


// CDlgCircle dialog

class CDlgCircle : public CDialog
{
	DECLARE_DYNAMIC(CDlgCircle)

public:
	CDlgCircle(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCircle();

// Dialog Data
	enum { IDD = IDD_DLG_CIRCLE };
	tgCmdLine *m_pCmdLine;
	int m_iSelIndex;
	bool disbool;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnCbnSelendokComboItem();
	afx_msg void OnBnClickedButtonReadAxispos();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonMove();
	afx_msg void OnBnClickedOk();

	void CaliCenter();
	void RefreshAxisPos(void);
	afx_msg void OnBnClickedCircleCheckDispenseOnoff();
	afx_msg void OnCbnSelchangeCircleRadioHead();
};