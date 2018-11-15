#pragma once

// CDlgAxisPos dialog
class CDlgAxisPos : public CDialog
{
	DECLARE_DYNAMIC(CDlgAxisPos)

public:
	CDlgAxisPos(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAxisPos();

// Dialog Data
	enum { IDD = IDD_DLG_AXISPOS };
	
	tgCmdLine *m_pCmdLine;
	int	m_iSelIndex;//µ±Ç°ÐÐºÅ
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonReadAxispos();
	afx_msg void OnBnClickedButtonMove();
	afx_msg void OnBnClickedCheckVel();
	afx_msg void OnBnClickedCheckDelay();
	afx_msg void OnCbnSelchangeComboCmdType();
public:
	void RefreshAxisPos(void);
	void EnablePassAngle(BOOL bEnable);
	void EnableCheckVel(BOOL bEnable);
	void EnableCheckDelay(BOOL bEnable);
	afx_msg void OnCbnSelchangeRadioHead();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedAxisPosCheckDispenseOnoff();
	afx_msg void OnEnChangeAxisPosEditX();
	afx_msg void OnBnClickedCancel();
};
