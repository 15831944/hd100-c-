// PagePos.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "PagePos.h"

#include "DlgJogXYZ.h"


// CPagePos dialog

IMPLEMENT_DYNAMIC(CPagePos, CPropertyPage)

CPagePos::CPagePos()
	: CPropertyPage(CPagePos::IDD)
{

}

CPagePos::~CPagePos()
{
}

void CPagePos::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPagePos, CPropertyPage)
	ON_BN_CLICKED(IDC_PAGE_POS_WASH_MOVETO, &CPagePos::OnBnClickedPagePosWashMoveto)
	ON_BN_CLICKED(IDC_PAGE_POS_WASH_SAVE, &CPagePos::OnBnClickedPagePosWashSave)
	ON_BN_CLICKED(IDC_PAGE_POS_HOME_MOVETO, &CPagePos::OnBnClickedPagePosHomeMoveto)
	ON_BN_CLICKED(IDC_PAGE_POS_HOME_SAVE, &CPagePos::OnBnClickedPagePosHomeSave)
	ON_BN_CLICKED(IDC_PAGE_POS_ZADJUST_MOVETO, &CPagePos::OnBnClickedPagePosZadjustMoveto)
	ON_BN_CLICKED(IDC_PAGE_POS_ZADJUST_SAVE, &CPagePos::OnBnClickedPagePosZadjustSave)
	ON_BN_CLICKED(IDC_PAGE_POS_JET_MOVETO, &CPagePos::OnBnClickedPagePosJetMoveto)
	ON_BN_CLICKED(IDC_PAGE_POS_JET_SAVE, &CPagePos::OnBnClickedPagePosJetSave)
	ON_BN_CLICKED(IDC_PAGE_POS_NEEDLE_MOVETO, &CPagePos::OnBnClickedPagePosNeedleMoveto)
	ON_BN_CLICKED(IDC_PAGE_POS_NEEDLE_SAVE, &CPagePos::OnBnClickedPagePosNeedleSave)
	ON_BN_CLICKED(IDC_PAGE_POS_JET_ZADJUST_MOVETO, &CPagePos::OnBnClickedPagePosJetZadjustMoveto)
	ON_BN_CLICKED(IDC_PAGE_POS_JET_ZADJUST_SAVE, &CPagePos::OnBnClickedPagePosJetZadjustSave)
	ON_BN_CLICKED(IDC_PAGE_POS_TESTJET_MOVETO, &CPagePos::OnBnClickedPagePosTestjetMoveto)
	ON_BN_CLICKED(IDC_PAGTESTJET_SAVE, &CPagePos::OnBnClickedPagtestjetSave)
	ON_BN_CLICKED(IDC_PAGE_POS_AFPS_MOVETO, &CPagePos::OnBnClickedPagePosAfpsMoveto)
	ON_BN_CLICKED(IDC_PAGE_POS_AFPS_CALIB, &CPagePos::OnBnClickedPagePosAfpsCalib)
END_MESSAGE_MAP()


// CPagePos message handlers

BOOL CPagePos::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strTemp;

// 	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tWashPos.x);
// 	SetDlgItemText(IDC_PAGE_POS_WASH_X, strTemp);
// 	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tWashPos.y);
// 	SetDlgItemText(IDC_PAGE_POS_WASH_Y, strTemp);
// 	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tWashPos.z);
// 	SetDlgItemText(IDC_PAGE_POS_WASH_Z, strTemp);

	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tAutoPfsPos.x);
	SetDlgItemText(IDC_PAGE_POS_AFPSX, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tAutoPfsPos.y);
	SetDlgItemText(IDC_PAGE_POS_AFPSY, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tAutoPfsPos.z);
	SetDlgItemText(IDC_PAGE_POS_AFPSZ, strTemp);


	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tDefHomePos.x);
	SetDlgItemText(IDC_PAGE_POS_HOME_X, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tDefHomePos.y);
	SetDlgItemText(IDC_PAGE_POS_HOME_Y, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tDefHomePos.z);
	SetDlgItemText(IDC_PAGE_POS_HOME_Z, strTemp);

	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tDispTestPos.x);
	SetDlgItemText(IDC_PAGE_POS_TESTJET_X, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tDispTestPos.y);
	SetDlgItemText(IDC_PAGE_POS_TESTJET_Y, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tDispTestPos.z);
	SetDlgItemText(IDC_PAGE_POS_TESTJET_Z, strTemp);

	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleAdjustPos[0].x);
	SetDlgItemText(IDC_PAGE_POS_NEEDLE_X, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleAdjustPos[0].y);
	SetDlgItemText(IDC_PAGE_POS_NEEDLE_Y, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleAdjustPos[0].z);
	SetDlgItemText(IDC_PAGE_POS_NEEDLE_Z, strTemp);

	
 	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tJetZAdjustPos.x);
 	SetDlgItemText(IDC_PAGE_POS_JET_ZADJUST_X, strTemp);
 	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tJetZAdjustPos.y);
 	SetDlgItemText(IDC_PAGE_POS_JET_ZADJUST_Y, strTemp);
 	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tJetZAdjustPos.z);
 	SetDlgItemText(IDC_PAGE_POS_JET_ZADJUST_Z, strTemp);
    
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleZAdjustPos.x);
	SetDlgItemText(IDC_PAGE_POS_ZADJUST_X, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleZAdjustPos.y);
	SetDlgItemText(IDC_PAGE_POS_ZADJUST_Y, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleZAdjustPos.z);
	SetDlgItemText(IDC_PAGE_POS_ZADJUST_Z, strTemp);

	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Tag_3"));
	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Tag_4"));
	GetDlgItem(Tag_5)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Tag_5"));
	GetDlgItem(Tag_6)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Tag_6"));
	GetDlgItem(Tag_7)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Tag_7"));

	GetDlgItem(IDC_PAGE_POS_ZADJUST_MOVETO)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_1"));
	GetDlgItem(IDC_PAGE_POS_ZADJUST_SAVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_2"));
	GetDlgItem(IDC_PAGE_POS_NEEDLE_MOVETO)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_1"));
	GetDlgItem(IDC_PAGE_POS_NEEDLE_SAVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_2"));
	GetDlgItem(IDC_PAGE_POS_WASH_MOVETO)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_1"));
	GetDlgItem(IDC_PAGE_POS_WASH_SAVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_2"));
	GetDlgItem(IDC_PAGE_POS_TESTJET_MOVETO)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_1"));
	GetDlgItem(IDC_PAGTESTJET_SAVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_2"));
	GetDlgItem(IDC_PAGE_POS_JET_ZADJUST_MOVETO)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_1"));
	GetDlgItem(IDC_PAGE_POS_JET_ZADJUST_SAVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_2"));
	GetDlgItem(IDC_PAGE_POS_JET_MOVETO)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_1"));
	GetDlgItem(IDC_PAGE_POS_JET_SAVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_2"));
	GetDlgItem(IDC_PAGE_POS_HOME_MOVETO)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_1"));
	GetDlgItem(IDC_PAGE_POS_HOME_SAVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_2"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPagePos::OnBnClickedPagePosWashMoveto()
{
	// TODO: Add your control notification handler code here
// 	double x=0,y=0,z=0;
// 
// 	if(g_pFrm->m_Robot->m_pController->IsInitOk())
// 	{
// 		x = g_pFrm->m_pSysParam->tWashPos.x;
// 		y = g_pFrm->m_pSysParam->tWashPos.y;
// 		z = g_pFrm->m_pSysParam->tWashPos.z;
// 
// 		g_pFrm->m_Robot->GotoSafeZpos();
// 		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_Vmin,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
// 		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
// 	}
}

void CPagePos::OnBnClickedPagePosWashSave()
{
	// TODO: Add your control notification handler code here
	CString str=g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_5");
	CDlgJogXYZ m_JogXYZ(str);
	if(IDOK!=m_JogXYZ.DoModal()) return;

// 	g_pFrm->m_pSysParam->tWashPos.x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
// 	g_pFrm->m_pSysParam->tWashPos.y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
// 	g_pFrm->m_pSysParam->tWashPos.z = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];
// 
// 	CString strTemp;
// 	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tWashPos.x);
// 	SetDlgItemText(IDC_PAGE_POS_WASH_X, strTemp);
// 	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tWashPos.y);
// 	SetDlgItemText(IDC_PAGE_POS_WASH_Y, strTemp);
// 	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tWashPos.z);
// 	SetDlgItemText(IDC_PAGE_POS_WASH_Z, strTemp);

	SetModified();
}

void CPagePos::OnBnClickedPagePosHomeMoveto()
{
	// TODO: Add your control notification handler code here
	double x=0,y=0,z=0;

	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		x = g_pFrm->m_pSysParam->tDefHomePos.x;
		y = g_pFrm->m_pSysParam->tDefHomePos.y;
		z = g_pFrm->m_pSysParam->tDefHomePos.z;

		g_pFrm->m_Robot->GotoSafeZpos();
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
	}
}

void CPagePos::OnBnClickedPagePosHomeSave()
{
	// TODO: Add your control notification handler code here
	CString str=g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_9");
	CDlgJogXYZ m_JogXYZ(str);

	if(IDOK!=m_JogXYZ.DoModal()) return;

	g_pFrm->m_pSysParam->tDefHomePos.x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pSysParam->tDefHomePos.y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	g_pFrm->m_pSysParam->tDefHomePos.z = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	CString strTemp;
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tDefHomePos.x);
	SetDlgItemText(IDC_PAGE_POS_HOME_X, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tDefHomePos.y);
	SetDlgItemText(IDC_PAGE_POS_HOME_Y, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tDefHomePos.z);
	SetDlgItemText(IDC_PAGE_POS_HOME_Z, strTemp);

	SetModified();
}
// 针头Z高度校正位
void CPagePos::OnBnClickedPagePosZadjustMoveto()
{
	double x=0,y=0,z=0;

	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		x = g_pFrm->m_pSysParam->tNeedleZAdjustPos.x;
		y = g_pFrm->m_pSysParam->tNeedleZAdjustPos.y;
		z = g_pFrm->m_pSysParam->tNeedleZAdjustPos.z;

		g_pFrm->m_Robot->GotoSafeZpos();
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
	}
}
//Z 向针头矫正位设置
void CPagePos::OnBnClickedPagePosZadjustSave()
{
	CString str=g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_3");
	CDlgJogXYZ m_JogXYZ(str);

	if(IDOK!=m_JogXYZ.DoModal()) 
		return;

	g_pFrm->m_pSysParam->tNeedleZAdjustPos.x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pSysParam->tNeedleZAdjustPos.y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	g_pFrm->m_pSysParam->tNeedleZAdjustPos.z = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	CString strTemp;
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleZAdjustPos.x);
	SetDlgItemText(IDC_PAGE_POS_ZADJUST_X, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleZAdjustPos.y);
	SetDlgItemText(IDC_PAGE_POS_ZADJUST_Y, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleZAdjustPos.z);
	SetDlgItemText(IDC_PAGE_POS_ZADJUST_Z, strTemp);

	SetModified();
}

void CPagePos::OnBnClickedPagePosJetMoveto()
{
	// TODO: Add your control notification handler code here
	double x=0,y=0;

	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		g_pFrm->m_Robot->GotoSafeZpos();
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	//	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
	}
}

void CPagePos::OnBnClickedPagePosJetSave()
{
	// TODO: Add your control notification handler code here
	CString str=g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_8");
	CDlgJogXYZ m_JogXYZ(str);
	if(IDOK!=m_JogXYZ.DoModal()) return;

//	g_pFrm->m_pSysParam->tJetAdjustPos.z = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	SetModified();
}

void CPagePos::OnBnClickedPagePosNeedleMoveto()
{
	double x=0,y=0,z=0;

	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		x = g_pFrm->m_pSysParam->tNeedleAdjustPos[0].x;
		y = g_pFrm->m_pSysParam->tNeedleAdjustPos[0].y;
		z = g_pFrm->m_pSysParam->tNeedleAdjustPos[0].z;

		g_pFrm->m_Robot->GotoSafeZpos();
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
	}
}

void CPagePos::OnBnClickedPagePosNeedleSave()
{
	CString str=g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_4");
	CDlgJogXYZ m_JogXYZ(str);
	if(IDOK!=m_JogXYZ.DoModal()) return;

	g_pFrm->m_pSysParam->tNeedleAdjustPos[0].x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pSysParam->tNeedleAdjustPos[0].y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	g_pFrm->m_pSysParam->tNeedleAdjustPos[0].z = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	CString strTemp;
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleAdjustPos[0].x);
	SetDlgItemText(IDC_PAGE_POS_NEEDLE_X, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleAdjustPos[0].y);
	SetDlgItemText(IDC_PAGE_POS_NEEDLE_Y, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleAdjustPos[0].z);
	SetDlgItemText(IDC_PAGE_POS_NEEDLE_Z, strTemp);

	SetModified();
}

void CPagePos::OnBnClickedPagePosJetZadjustMoveto()
{
	// TODO: Add your control notification handler code here
	double x=0,y=0,z=0;

	if(g_pFrm->m_Robot->m_pController->IsInitOk())
    {
 		x = g_pFrm->m_pSysParam->tJetZAdjustPos.x;
 		y = g_pFrm->m_pSysParam->tJetZAdjustPos.y;
 		z = g_pFrm->m_pSysParam->tJetZAdjustPos.z;

		g_pFrm->m_Robot->GotoSafeZpos();
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
	}
}

void CPagePos::OnBnClickedPagePosJetZadjustSave()
{
	// TODO: Add your control notification handler code here

	CString str=g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_7");
	CDlgJogXYZ m_JogXYZ(str);
	if(IDOK!=m_JogXYZ.DoModal()) return;

 	g_pFrm->m_pSysParam->tJetZAdjustPos.x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
 	g_pFrm->m_pSysParam->tJetZAdjustPos.y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
 	g_pFrm->m_pSysParam->tJetZAdjustPos.z = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];
 
 	CString strTemp;
 	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tJetZAdjustPos.x);
 	SetDlgItemText(IDC_PAGE_POS_JET_ZADJUST_X, strTemp);
 	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tJetZAdjustPos.y);
 	SetDlgItemText(IDC_PAGE_POS_JET_ZADJUST_Y, strTemp);
 	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tJetZAdjustPos.z);
 	SetDlgItemText(IDC_PAGE_POS_JET_ZADJUST_Z, strTemp);

	SetModified();
}
BOOL CPagePos::OnApply()
{
	// TODO: Add your specialized code here and/or call the base class
	g_pFrm->SysParam(FALSE);

	return CPropertyPage::OnApply();
}
// 试胶位
void CPagePos::OnBnClickedPagePosTestjetMoveto()
{
	double x=0,y=0,z=0;

	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		x = g_pFrm->m_pSysParam->tDispTestPos.x;
		y = g_pFrm->m_pSysParam->tDispTestPos.y;
		z = g_pFrm->m_pSysParam->tDispTestPos.z;

		g_pFrm->m_Robot->GotoSafeZpos();
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
	}
}
// 保存试胶位置
void CPagePos::OnBnClickedPagtestjetSave()
{
	CString str=g_pFrm->LanguageMgr->ReadStringFromIni(DefPagePos,"Btn_6");
	CDlgJogXYZ m_JogXYZ(str);
	if(IDOK!=m_JogXYZ.DoModal()) return;

	g_pFrm->m_pSysParam->tDispTestPos.x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pSysParam->tDispTestPos.y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	g_pFrm->m_pSysParam->tDispTestPos.z = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	CString strTemp;
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tDispTestPos.x);
	SetDlgItemText(IDC_PAGE_POS_TESTJET_X, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tDispTestPos.y);
	SetDlgItemText(IDC_PAGE_POS_TESTJET_Y, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tDispTestPos.z);
	SetDlgItemText(IDC_PAGE_POS_TESTJET_Z, strTemp);

	SetModified();
}

void CPagePos::OnBnClickedPagePosAfpsMoveto()
{
	double x=0,y=0,z=0;

	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		x = g_pFrm->m_pSysParam->tAutoPfsPos.x;
		y = g_pFrm->m_pSysParam->tAutoPfsPos.y;
		z = g_pFrm->m_pSysParam->tAutoPfsPos.z;

		g_pFrm->m_Robot->GotoSafeZpos();
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
	}
}

void CPagePos::OnBnClickedPagePosAfpsCalib()
{
	CString str=_T("防固化清喷位");
	CDlgJogXYZ m_JogXYZ(str);
	if(IDOK!=m_JogXYZ.DoModal()) return;

	g_pFrm->m_pSysParam->tAutoPfsPos.x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pSysParam->tAutoPfsPos.y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	g_pFrm->m_pSysParam->tAutoPfsPos.z = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	CString strTemp;
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tAutoPfsPos.x);
	SetDlgItemText(IDC_PAGE_POS_AFPSX, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tAutoPfsPos.y);
	SetDlgItemText(IDC_PAGE_POS_AFPSY, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tAutoPfsPos.z);
	SetDlgItemText(IDC_PAGE_POS_AFPSZ, strTemp);

	SetModified();
}
