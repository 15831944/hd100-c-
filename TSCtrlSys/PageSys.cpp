// PageSys.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "PageSys.h"

#include <math.h>
// CPageSys dialog

IMPLEMENT_DYNAMIC(CPageSys, CPropertyPage)

CPageSys::CPageSys()
	: CPropertyPage(CPageSys::IDD)
{

}

CPageSys::~CPageSys()
{
}

void CPageSys::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageSys, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_PAGE_SYS_COMBO_AXIS, &CPageSys::OnCbnSelchangePageSysComboAxis)
	ON_EN_CHANGE(IDC_PAGE_SYS_EDIT_ACC, &CPageSys::OnEnChangePageSysEditAcc)
	ON_EN_CHANGE(IDC_PAGE_SYS_MOVETO_EDIT_VELXY, &CPageSys::OnEnChangePageSysEditVelxy)	
	ON_EN_CHANGE(IDC_PAGE_SYS_MOVETO_EDIT_VELZ, &CPageSys::OnEnChangePageSysEditVelz)
	ON_EN_CHANGE(IDC_PAGE_SYS_MOVETO_EDIT_VELU, &CPageSys::OnEnChangePageSysEditVelu)
	ON_EN_CHANGE(IDC_PAGE_SYS_JOG_MAX_VELXY, &CPageSys::OnEnChangePageSysJogMaxVelxy)
	ON_EN_CHANGE(IDC_PAGE_SYS_JOG_MAX_VELZ, &CPageSys::OnEnChangePageSysJogMaxVelz)
	ON_EN_CHANGE(IDC_PAGE_SYS_JOG_MAX_VELU, &CPageSys::OnEnChangePageSysJogMaxVelu)
	ON_EN_CHANGE(IDC_PAGE_SYS_DISP_VEL, &CPageSys::OnEnChangePageSysDispVel)
	ON_EN_CHANGE(IDC_PAGE_SYS_BELT_VEL, &CPageSys::OnEnChangePageSysBeltVel)
	ON_EN_CHANGE(IDC_PAGE_SYS_LOCA_VELXY, &CPageSys::OnEnChangePageSysLocaVelxy)
	ON_EN_CHANGE(IDC_PAGE_SYS_LOCA_VELZ, &CPageSys::OnEnChangePageSysLocaVelz)
	ON_EN_CHANGE(IDC_PAGE_SYS_HOME_VELH1, &CPageSys::OnEnChangePageSysHomeVelh1)
	ON_EN_CHANGE(IDC_PAGE_SYS_HOME_VELH2, &CPageSys::OnEnChangePageSysHomeVelh2)
	ON_EN_CHANGE(IDC_PAGE_SYS_HOME_VELL, &CPageSys::OnEnChangePageSysHomeVell)
	ON_EN_CHANGE(IDC_PAGE_SYS_SEARCH_NUM, &CPageSys::OnEnChangePageSysSearchNum)
	ON_EN_CHANGE(IDC_PAGE_SYS_XY_DIST, &CPageSys::OnEnChangePageSysXyDist)
	ON_EN_CHANGE(IDC_PAGE_SYS_VELXY, &CPageSys::OnEnChangePageSysVelxy)
	ON_EN_CHANGE(IDC_PAGE_SYS_VELZ, &CPageSys::OnEnChangePageSysVelz)
	ON_EN_CHANGE(IDC_PAGE_SYS_VELV, &CPageSys::OnEnChangePageSysVelv)
	ON_EN_CHANGE(IDC_PAGE_SYS_JOG_MAX_VELV, &CPageSys::OnEnChangePageSysJogMaxVelv)
	ON_EN_CHANGE(IDC_PAGE_SYS_MOVETO_EDIT_VELV, &CPageSys::OnEnChangePageSysMovetoEditVelv)
	ON_EN_CHANGE(IDC_PAGE_SYS_JOG_MAX_VELM, &CPageSys::OnEnChangePageSysJogMaxVelm)
	ON_EN_CHANGE(IDC_PAGE_SYS_HOME_VELLL, &CPageSys::OnEnChangePageSysHomeVelll)
	ON_EN_CHANGE(IDC_PAGE_SYS_FEEDHOME_VELH, &CPageSys::OnEnChangePageSysFeedhomeVelh)
	ON_EN_CHANGE(IDC_PAGE_SYS_FEEDHOME_VELM, &CPageSys::OnEnChangePageSysFeedhomeVelm)
	ON_EN_CHANGE(IDC_PAGE_SYS_FEEDHOME_VELLL, &CPageSys::OnEnChangePageSysFeedhomeVelll)
	ON_EN_CHANGE(IDC_PAGE_SYS_MOVETO_EDIT_VELM, &CPageSys::OnEnChangePageSysMovetoEditVelm)
	ON_EN_CHANGE(IDC_PAGE_SYS_VELM, &CPageSys::OnEnChangePageSysVelm)
	ON_EN_CHANGE(IDC_PAGE_SYS_BELT_UNLOADDELAY, &CPageSys::OnEnChangePageSysBeltUnloaddelay)
	ON_EN_CHANGE(IDC_PAGE_SYS_LOOK_ACC, &CPageSys::OnEnChangePageSysLookAcc)
	ON_EN_CHANGE(IDC_PAGE_SYS_EDIT_KP, &CPageSys::OnEnChangePageSysEditKp)
	ON_EN_CHANGE(IDC_PAGE_SYS_EDIT_KI, &CPageSys::OnEnChangePageSysEditKi)
	ON_EN_CHANGE(IDC_PAGE_SYS_EDIT_KD, &CPageSys::OnEnChangePageSysEditKd)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_PID, &CPageSys::OnBnClickedButtonUpdatePid)
END_MESSAGE_MAP()


// CPageSys message handlers

BOOL CPageSys::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CComboBox *pBox =(CComboBox*)GetDlgItem(IDC_PAGE_SYS_COMBO_AXIS);
	pBox->AddString("X Axis");
	pBox->AddString("Y Axis");
	pBox->AddString("Z Axis");
	pBox->AddString("U Axis");
	pBox->SetCurSel(0);

	OnCbnSelchangePageSysComboAxis();

	////////////
	CString strTemp;
	strTemp.Format("%.3f",g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax);
	SetDlgItemText(IDC_PAGE_SYS_MOVETO_EDIT_VELXY, strTemp);
	
	strTemp.Format("%.3f",g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax);
	SetDlgItemText(IDC_PAGE_SYS_MOVETO_EDIT_VELZ, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedMovetoU);
	SetDlgItemText(IDC_PAGE_SYS_MOVETO_EDIT_VELU, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedMovetoVW);
	SetDlgItemText(IDC_PAGE_SYS_MOVETO_EDIT_VELV, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedMovetoMN);
	SetDlgItemText(IDC_PAGE_SYS_MOVETO_EDIT_VELM, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedMaxJogXY);
	SetDlgItemText(IDC_PAGE_SYS_JOG_MAX_VELXY, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedMaxJogZ);
	SetDlgItemText(IDC_PAGE_SYS_JOG_MAX_VELZ, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedMaxJogU);
	SetDlgItemText(IDC_PAGE_SYS_JOG_MAX_VELU, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedMaxJogVW);
	SetDlgItemText(IDC_PAGE_SYS_JOG_MAX_VELV, strTemp);
 
	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedMaxJogMN);
	SetDlgItemText(IDC_PAGE_SYS_JOG_MAX_VELM, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedDisp);
	SetDlgItemText(IDC_PAGE_SYS_DISP_VEL, strTemp);
	//////
	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedBelt);
	SetDlgItemText(IDC_PAGE_SYS_BELT_VEL, strTemp);	

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dBeltUnloadDelay);
	SetDlgItemText(IDC_PAGE_SYS_BELT_UNLOADDELAY, strTemp);	

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedMoveXY);
	SetDlgItemText(IDC_PAGE_SYS_VELXY, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedMoveZ);
	SetDlgItemText(IDC_PAGE_SYS_VELZ, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedMoveVW);
	SetDlgItemText(IDC_PAGE_SYS_VELV, strTemp);
   
	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedMoveMN);
	SetDlgItemText(IDC_PAGE_SYS_VELM, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedImageMoveXY);
	SetDlgItemText(IDC_PAGE_SYS_LOCA_VELXY, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedImageMoveZ);
	SetDlgItemText(IDC_PAGE_SYS_LOCA_VELZ, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedHomeH1);
	SetDlgItemText(IDC_PAGE_SYS_HOME_VELH1, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedHomeH2);
	SetDlgItemText(IDC_PAGE_SYS_HOME_VELH2, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedHomeL);
	SetDlgItemText(IDC_PAGE_SYS_HOME_VELL, strTemp);

	strTemp.Format("%d",g_pFrm->m_pSysParam->m_ccdMoveCnt);
	SetDlgItemText(IDC_PAGE_SYS_SEARCH_NUM, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->m_ccdMoveDis);
	SetDlgItemText(IDC_PAGE_SYS_XY_DIST, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedHomeLoadH);
	SetDlgItemText(IDC_PAGE_SYS_FEEDHOME_VELH, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedHomeLoadM);
	SetDlgItemText(IDC_PAGE_SYS_FEEDHOME_VELM, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedHomeLoadL);
	SetDlgItemText(IDC_PAGE_SYS_FEEDHOME_VELLL, strTemp);
    
	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dLookAcc);
	SetDlgItemText(IDC_PAGE_SYS_LOOK_ACC, strTemp);
	
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_3"));
	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_4"));
	GetDlgItem(Tag_5)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_5"));
	GetDlgItem(Tag_6)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_6"));
	GetDlgItem(Tag_7)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_7"));
	GetDlgItem(Tag_8)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_8"));
	GetDlgItem(Tag_9)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_9"));//
	//GetDlgItem(Tag_10)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_10"));
	GetDlgItem(Tag_11)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_11"));
	GetDlgItem(Tag_12)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_12"));
	GetDlgItem(Tag_13)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_13"));
//	GetDlgItem(Tag_14)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_14"));
//  GetDlgItem(Tag_15)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_15"));
	GetDlgItem(Tag_16)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_16"));
	GetDlgItem(Tag_17)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_17"));
	GetDlgItem(Tag_18)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_18"));
	GetDlgItem(Tag_19)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_19"));
	//GetDlgItem(Tag_20)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_20"));
	//GetDlgItem(Tag_21)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_21"));
	//GetDlgItem(Tag_22)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_22"));
	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_0"));
	//GetDlgItem(Tag_23)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageSys,"Tag_23"));
   
	if(g_pFrm->m_pSysParam->nRobotCtrlMode == DefCtrlMode_Analog)
	{
		GetDlgItem(IDC_PAGE_SYS_EDIT_KP)->ShowWindow(TRUE);
		GetDlgItem(IDC_PAGE_SYS_EDIT_KI)->ShowWindow(TRUE);
		GetDlgItem(IDC_PAGE_SYS_EDIT_KD)->ShowWindow(TRUE);
		GetDlgItem(IDC_PAGE_SYS_EDIT_KAFF)->ShowWindow(TRUE);
		GetDlgItem(IDC_PAGE_SYS_EDIT_KVFF)->ShowWindow(TRUE);
		GetDlgItem(IDC_PAGE_SYS_EDIT_PVF)->ShowWindow(TRUE);
	    GetDlgItem(IDC_BUTTON_UPDATE_PID)->ShowWindow(TRUE);  

		GetDlgItem(IDC_STATIC_KP)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_KI)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_KD)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_KAFF)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_KVFF)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_SPEEDLOAD)->ShowWindow(TRUE);
		
	}else
	{
		GetDlgItem(IDC_PAGE_SYS_EDIT_KP)->ShowWindow(FALSE);
		GetDlgItem(IDC_PAGE_SYS_EDIT_KI)->ShowWindow(FALSE);
		GetDlgItem(IDC_PAGE_SYS_EDIT_KD)->ShowWindow(FALSE);
		GetDlgItem(IDC_PAGE_SYS_EDIT_KAFF)->ShowWindow(FALSE);
		GetDlgItem(IDC_PAGE_SYS_EDIT_KVFF)->ShowWindow(FALSE);
		GetDlgItem(IDC_PAGE_SYS_EDIT_PVF)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPDATE_PID)->ShowWindow(FALSE);

		GetDlgItem(IDC_STATIC_KP)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_KI)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_KD)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_KAFF)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_KVFF)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_SPEEDLOAD)->ShowWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageSys::OnApply()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CComboBox *pBox =(CComboBox*)GetDlgItem(IDC_PAGE_SYS_COMBO_AXIS);
	CString strTemp;
	int n=pBox->GetCurSel();
	if (n<0)  {n=0;}

	GetDlgItemText(IDC_PAGE_SYS_EDIT_AXIS_SCALE, strTemp);
	g_pFrm->m_mtrParamGts[n].m_Resolution = atof(strTemp);
	
	GetDlgItemText(IDC_PAGE_SYS_EDIT_MAXPOS, strTemp);
	g_pFrm->m_mtrParamGts[n].m_MaxTravelPositive = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_EDIT_MINPOS, strTemp);
	g_pFrm->m_mtrParamGts[n].m_MaxTravelNegative = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_EDIT_ACC, strTemp);
	g_pFrm->m_mtrParamGts[n].m_Acc = atof(strTemp);
	
	if (g_pFrm->m_mtrParamGts[n].m_Acc>10000)
	{
		g_pFrm->m_mtrParamGts[n].m_Acc = 10000;
	}

	
	GetDlgItemText(IDC_PAGE_SYS_MOVETO_EDIT_VELXY, strTemp);
	g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax = atof(strTemp);
  
	GetDlgItemText(IDC_PAGE_SYS_LOOK_ACC, strTemp);
	g_pFrm->m_pSysParam->dLookAcc = atof(strTemp);
    if (g_pFrm->m_pSysParam->dLookAcc>18000)
    {
        g_pFrm->m_pSysParam->dLookAcc = 18000;
    }

	// ÖØÐÂÉèÖÃLOOkACC
	g_pFrm->m_Robot->m_pController->SetListRunACC(g_pFrm->m_pSysParam->dLookAcc,10000);

	GetDlgItemText(IDC_PAGE_SYS_MOVETO_EDIT_VELZ, strTemp);
	g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_MOVETO_EDIT_VELU, strTemp);
	g_pFrm->m_pSysParam->dSpeedMovetoU = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_MOVETO_EDIT_VELV, strTemp);
	g_pFrm->m_pSysParam->dSpeedMovetoVW = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_MOVETO_EDIT_VELM, strTemp);
	g_pFrm->m_pSysParam->dSpeedMovetoMN = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_JOG_MAX_VELXY, strTemp);
	g_pFrm->m_pSysParam->dSpeedMaxJogXY = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_JOG_MAX_VELZ, strTemp);
	g_pFrm->m_pSysParam->dSpeedMaxJogZ= atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_JOG_MAX_VELU, strTemp);
	g_pFrm->m_pSysParam->dSpeedMaxJogU = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_JOG_MAX_VELV, strTemp);
	g_pFrm->m_pSysParam->dSpeedMaxJogVW = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_JOG_MAX_VELM, strTemp);
	g_pFrm->m_pSysParam->dSpeedMaxJogMN =atof(strTemp);
	
	GetDlgItemText(IDC_PAGE_SYS_DISP_VEL, strTemp);
	g_pFrm->m_pSysParam->dSpeedDisp = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_BELT_VEL, strTemp);
	g_pFrm->m_pSysParam->dSpeedBelt = atof(strTemp);

	if(fabs(g_pFrm->m_pSysParam->dSpeedBelt)>500)
		g_pFrm->m_pSysParam->dSpeedBelt = 500;

// 	strTemp.Format("%d", PLC_SEAD_PORT_BELT_VEL);
// 	theApp.m_Serial.WritePLCData(strTemp,(int)g_pFrm->m_pSysParam->dSpeedBelt);
   
	GetDlgItemText(IDC_PAGE_SYS_BELT_UNLOADDELAY, strTemp);	
    g_pFrm->m_pSysParam->dBeltUnloadDelay = atof(strTemp);

	if(fabs(g_pFrm->m_pSysParam->dBeltUnloadDelay)>50)
		g_pFrm->m_pSysParam->dBeltUnloadDelay = 50;

	GetDlgItemText(IDC_PAGE_SYS_LOCA_VELXY, strTemp);
	g_pFrm->m_pSysParam->dSpeedImageMoveXY = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_LOCA_VELZ, strTemp);
	g_pFrm->m_pSysParam->dSpeedImageMoveZ = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_VELXY, strTemp);
	g_pFrm->m_pSysParam->dSpeedMoveXY = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_VELZ, strTemp);
	g_pFrm->m_pSysParam->dSpeedMoveZ = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_VELV, strTemp);
	g_pFrm->m_pSysParam->dSpeedMoveVW = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_VELM, strTemp);
	g_pFrm->m_pSysParam->dSpeedMoveMN = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_HOME_VELH1, strTemp);
	g_pFrm->m_pSysParam->dSpeedHomeH1 = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_HOME_VELH2, strTemp);
	g_pFrm->m_pSysParam->dSpeedHomeH2 = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_HOME_VELL, strTemp);
	g_pFrm->m_pSysParam->dSpeedHomeL = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_FEEDHOME_VELH, strTemp);
	g_pFrm->m_pSysParam->dSpeedHomeLoadH = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_FEEDHOME_VELM, strTemp);
	g_pFrm->m_pSysParam->dSpeedHomeLoadM = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_FEEDHOME_VELLL, strTemp);
	g_pFrm->m_pSysParam->dSpeedHomeLoadL = atof(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_SEARCH_NUM, strTemp);
	g_pFrm->m_pSysParam->m_ccdMoveCnt = atoi(strTemp);

	GetDlgItemText(IDC_PAGE_SYS_XY_DIST, strTemp);
	g_pFrm->m_pSysParam->m_ccdMoveDis = atof(strTemp);
 
	g_pFrm->SysParam(FALSE);

	return CPropertyPage::OnApply();
}

void CPageSys::OnCbnSelchangePageSysComboAxis()
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox =(CComboBox*)GetDlgItem(IDC_PAGE_SYS_COMBO_AXIS);
	CString strTemp;
	int n=pBox->GetCurSel();

	strTemp.Format("%.6f", g_pFrm->m_mtrParamGts[n].m_Resolution);
	SetDlgItemText(IDC_PAGE_SYS_EDIT_AXIS_SCALE, strTemp);

	strTemp.Format("%.3f", g_pFrm->m_mtrParamGts[n].m_MaxTravelPositive);
	SetDlgItemText(IDC_PAGE_SYS_EDIT_MAXPOS, strTemp);

	strTemp.Format("%.3f", g_pFrm->m_mtrParamGts[n].m_MaxTravelNegative);
	SetDlgItemText(IDC_PAGE_SYS_EDIT_MINPOS, strTemp);

	strTemp.Format("%.3f", g_pFrm->m_mtrParamGts[n].m_Acc);
	SetDlgItemText(IDC_PAGE_SYS_EDIT_ACC, strTemp);

	if (n<3)
	{
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->PIDParam[n].PID_P);
		SetDlgItemText(IDC_PAGE_SYS_EDIT_KP, strTemp);

		strTemp.Format("%.3f", g_pFrm->m_pSysParam->PIDParam[n].PID_I);
		SetDlgItemText(IDC_PAGE_SYS_EDIT_KI, strTemp);
		
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->PIDParam[n].PID_D);
		SetDlgItemText(IDC_PAGE_SYS_EDIT_KD, strTemp);
   
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->PIDParam[n].Kaff);
		SetDlgItemText(IDC_PAGE_SYS_EDIT_KAFF, strTemp);
		
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->PIDParam[n].Kvff);
		SetDlgItemText(IDC_PAGE_SYS_EDIT_KVFF, strTemp);

		strTemp.Format("%.3f", g_pFrm->m_pSysParam->PIDParam[n].PrfVelFeilter);
		SetDlgItemText(IDC_PAGE_SYS_EDIT_PVF, strTemp);
	
	}
}

void CPageSys::OnEnChangePageSysEditAcc()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysEditVelxy()
{
	SetModified();
}
void CPageSys::OnEnChangePageSysEditVelz()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysEditVelu()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysJogMaxVelxy()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysJogMaxVelz()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysJogMaxVelu()
{

	SetModified();
}

void CPageSys::OnEnChangePageSysDispVel()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysBeltVel()
{

	SetModified();
}

void CPageSys::OnEnChangePageSysDummyVelxy()
{

	SetModified();
}

void CPageSys::OnEnChangePageSysDummyVelz()
{

	SetModified();
}

void CPageSys::OnEnChangePageSysLocaVelxy()
{

	SetModified();
}

void CPageSys::OnEnChangePageSysLocaVelz()
{

	SetModified();
}

void CPageSys::OnEnChangePageSysHomeVelh1()
{

	SetModified();
}

void CPageSys::OnEnChangePageSysHomeVelh2()
{

	SetModified();
}

void CPageSys::OnEnChangePageSysHomeVell()
{

	SetModified();
}

void CPageSys::OnEnChangePageSysSearchNum()
{

	SetModified();
}

void CPageSys::OnEnChangePageSysXyDist()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysVelxy()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysVelz()
{

	SetModified();
}

void CPageSys::OnEnChangePageSysVelv()
{
   SetModified(); 
}

void CPageSys::OnEnChangePageSysJogMaxVelv()
{
    SetModified();
}

void CPageSys::OnEnChangePageSysMovetoEditVelv()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysJogMaxVelm()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysHomeVelll()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysFeedhomeVelh()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysFeedhomeVelm()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysFeedhomeVelll()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysMovetoEditVelm()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysVelm()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysBeltUnloaddelay()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysLookAcc()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysEditKp()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysEditKi()
{
	SetModified();
}

void CPageSys::OnEnChangePageSysEditKd()
{
	SetModified();
}

void CPageSys::OnBnClickedButtonUpdatePid()
{
	CComboBox *pBox =(CComboBox*)GetDlgItem(IDC_PAGE_SYS_COMBO_AXIS);
	CString strTemp;
	int n=pBox->GetCurSel();
	if (n<0)  {n=0;}

	if (n<3)
	{
		GetDlgItemText(IDC_PAGE_SYS_EDIT_KP, strTemp);
		g_pFrm->m_pSysParam->PIDParam[n].PID_P=atof(strTemp);
		g_pFrm->m_Robot->m_pController->g_ExMtnPara->PID_P[n]=g_pFrm->m_pSysParam->PIDParam[n].PID_P;

		GetDlgItemText(IDC_PAGE_SYS_EDIT_KI, strTemp);
		g_pFrm->m_pSysParam->PIDParam[n].PID_I=atof(strTemp);
		g_pFrm->m_Robot->m_pController->g_ExMtnPara->PID_I[n]=g_pFrm->m_pSysParam->PIDParam[n].PID_I;

		GetDlgItemText(IDC_PAGE_SYS_EDIT_KD, strTemp);
		g_pFrm->m_pSysParam->PIDParam[n].PID_D=atof(strTemp);
		g_pFrm->m_Robot->m_pController->g_ExMtnPara->PID_D[n]=g_pFrm->m_pSysParam->PIDParam[n].PID_D;
	    
		GetDlgItemText(IDC_PAGE_SYS_EDIT_KAFF, strTemp);
		g_pFrm->m_pSysParam->PIDParam[n].Kaff=atof(strTemp);
		g_pFrm->m_Robot->m_pController->g_ExMtnPara->Kaff[n]=g_pFrm->m_pSysParam->PIDParam[n].Kaff;

		GetDlgItemText(IDC_PAGE_SYS_EDIT_KVFF, strTemp);
		g_pFrm->m_pSysParam->PIDParam[n].Kvff=atof(strTemp);
		g_pFrm->m_Robot->m_pController->g_ExMtnPara->kvff[n]=g_pFrm->m_pSysParam->PIDParam[n].Kvff;

		GetDlgItemText(IDC_PAGE_SYS_EDIT_PVF, strTemp);
		g_pFrm->m_pSysParam->PIDParam[n].PrfVelFeilter=atof(strTemp);
		g_pFrm->m_Robot->m_pController->g_ExMtnPara->PrfVelFilter[n]=g_pFrm->m_pSysParam->PIDParam[n].PrfVelFeilter;

	}
    
	g_pFrm->m_Robot->m_pController->UpdatePIDPara(n,
		g_pFrm->m_pSysParam->PIDParam[n].PID_P,
		g_pFrm->m_pSysParam->PIDParam[n].PID_I,
		g_pFrm->m_pSysParam->PIDParam[n].PID_D,
		g_pFrm->m_pSysParam->PIDParam[n].Kaff,
		g_pFrm->m_pSysParam->PIDParam[n].Kvff,
		g_pFrm->m_pSysParam->PIDParam[n].PrfVelFeilter);

}
