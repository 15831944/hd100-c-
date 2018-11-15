#pragma once

// CPageSys dialog

class CPageSys : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageSys)

public:
	CPageSys();
	virtual ~CPageSys();

// Dialog Data
	enum { IDD = IDD_PAGE_SYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnCbnSelchangePageSysComboAxis();
	afx_msg void OnEnChangePageSysEditVelxy();
	afx_msg void OnEnChangePageSysEditAcc();
	afx_msg void OnEnChangePageSysEditVelz();
	afx_msg void OnEnChangePageSysEditVelu();
	afx_msg void OnEnChangePageSysJogMaxVelxy();
	afx_msg void OnEnChangePageSysJogMaxVelz();
	afx_msg void OnEnChangePageSysJogMaxVelu();
	afx_msg void OnEnChangePageSysDispVel();
	afx_msg void OnEnChangePageSysBeltVel();
	afx_msg void OnEnChangePageSysDummyVelxy();
	afx_msg void OnEnChangePageSysDummyVelz();
	afx_msg void OnEnChangePageSysLocaVelxy();
	afx_msg void OnEnChangePageSysLocaVelz();
	afx_msg void OnEnChangePageSysHomeVelh1();
	afx_msg void OnEnChangePageSysHomeVelh2();
	afx_msg void OnEnChangePageSysHomeVell();
	afx_msg void OnEnChangePageSysSearchNum();
	afx_msg void OnEnChangePageSysXyDist();
	afx_msg void OnEnChangePageSysVelxy();
	afx_msg void OnEnChangePageSysVelz();
	afx_msg void OnEnChangePageSysVelv();
	afx_msg void OnEnChangePageSysJogMaxVelv();
	afx_msg void OnEnChangePageSysMovetoEditVelv();
	afx_msg void OnEnChangePageSysJogMaxVelm();
	afx_msg void OnEnChangePageSysHomeVelll();
	afx_msg void OnEnChangePageSysFeedhomeVelh();
	afx_msg void OnEnChangePageSysFeedhomeVelm();
	afx_msg void OnEnChangePageSysFeedhomeVelll();
	afx_msg void OnEnChangePageSysMovetoEditVelm();
	afx_msg void OnEnChangePageSysVelm();
	afx_msg void OnEnChangePageSysBeltUnloaddelay();
	afx_msg void OnEnChangePageSysLookAcc();
	afx_msg void OnEnChangePageSysEditKp();
	afx_msg void OnEnChangePageSysEditKi();
	afx_msg void OnEnChangePageSysEditKd();
	afx_msg void OnBnClickedButtonUpdatePid();
	afx_msg void OnEnChangePageSysBeltVel4();
};
