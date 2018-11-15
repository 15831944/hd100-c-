// DlgOffset.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgNeedleAdjust.h"
#include "MainFrm.h"

#include "DlgJogXYZ.h"


// CDlgOffset dialog

IMPLEMENT_DYNAMIC(CDlgNeedleAdjust, CDialog)

CDlgNeedleAdjust::CDlgNeedleAdjust(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNeedleAdjust::IDD, pParent)
{

}

CDlgNeedleAdjust::~CDlgNeedleAdjust()
{
}

void CDlgNeedleAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CDlgNeedleAdjust, CDialog)
	ON_BN_CLICKED(IDC_BTN_READ_AXISPOS, &CDlgNeedleAdjust::OnBnClickedButtonReadAxispos)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDlgNeedleAdjust::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDOK, &CDlgNeedleAdjust::OnBnClickedOk)
	ON_CBN_SELENDOK(IDC_DLG_NEEDLE_ADJUST_COMBO, &CDlgNeedleAdjust::OnCbnSelendokComboPos)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgNeedleAdjust::OnBnClickedButtonApply)
	ON_WM_ACTIVATE()

	ON_BN_CLICKED(IDC_DLG_NEEDLE_ADJUST_DISP, &CDlgNeedleAdjust::OnBnClickedDlgNeedleAdjustDisp)
	ON_BN_CLICKED(IDC_DLG_NEEDLE_ADJUST_JOG, &CDlgNeedleAdjust::OnBnClickedDlgNeedleAdjustJog)
END_MESSAGE_MAP()

// CDlgOffset message handlers
BOOL CDlgNeedleAdjust::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString str;

	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_DLG_NEEDLE_ADJUST_COMBO);

	pBox->AddString("针头参考点位置");
	pBox->AddString("图像参考点位置");
		
	pBox->SetCurSel(0);

	OnCbnSelendokComboPos();

	CString strTemp; 
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tOffset[1].x);
	SetDlgItemText(IDC_DLG_NEEDLE_ADJUST_OFFX, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tOffset[1].y);
	SetDlgItemText(IDC_DLG_NEEDLE_ADJUST_OFFY, strTemp);

	strTemp.Format("%.3f", g_pFrm->m_pSysParam->dNeedleAdjustDispTime);
	SetDlgItemText(IDC_DLG_NEEDLE_ADJUST_DISPTIME, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->dNeedleAdjustWaitTime);
	SetDlgItemText(IDC_DLG_NEEDLE_ADJUST_OFFTIME, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->dNeedleAdjustUpHeight);
	SetDlgItemText(IDC_DLG_NEEDLE_ADJUST_DELTAH, strTemp);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgNeedleAdjust::OnBnClickedButtonReadAxispos()
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_DLG_NEEDLE_ADJUST_COMBO);
	int n = pBox->GetCurSel();

	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();

	g_pFrm->m_pSysParam->tNeedleAdjustPos[n].x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pSysParam->tNeedleAdjustPos[n].y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	g_pFrm->m_pSysParam->tNeedleAdjustPos[n].z = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	OnCbnSelendokComboPos();

}
void CDlgNeedleAdjust::OnBnClickedButtonMove()
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_DLG_NEEDLE_ADJUST_COMBO);
	int n = pBox->GetCurSel();

	double x,y,z;

	x = g_pFrm->m_pSysParam->tNeedleAdjustPos[n].x;
	y = g_pFrm->m_pSysParam->tNeedleAdjustPos[n].y;

	if(n==1)
	{//图像中心位置，用CCD标定高度
		z = g_pFrm->m_pSysParam->tPixelAdjustPos.z;

	}
	else
	{
		z = g_pFrm->m_pSysParam->tNeedleAdjustPos[n].z;
	}


	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
}
void CDlgNeedleAdjust::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnBnClickedButtonApply();

	OnOK();
}
void CDlgNeedleAdjust::OnCbnSelendokComboPos()
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_DLG_NEEDLE_ADJUST_COMBO);
	int n = pBox->GetCurSel();
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleAdjustPos[n].x);
	SetDlgItemText(IDC_DLG_NEEDLE_ADJUST_EDIT_X, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleAdjustPos[n].y);
	SetDlgItemText(IDC_DLG_NEEDLE_ADJUST_EDIT_Y, strTemp);

	if(n==0)
	{
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleAdjustPos[n].z);
		SetDlgItemText(IDC_DLG_NEEDLE_ADJUST_EDIT_Z, strTemp);
		GetDlgItem(IDC_DLG_NEEDLE_ADJUST_DISP)->EnableWindow(TRUE);
	}
	else
	{
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tPixelAdjustPos.z);
		SetDlgItemText(IDC_DLG_NEEDLE_ADJUST_EDIT_Z, strTemp);
		GetDlgItem(IDC_DLG_NEEDLE_ADJUST_DISP)->EnableWindow(FALSE);
	}

}
void CDlgNeedleAdjust::OnBnClickedButtonApply()
{//当选择针头时，所有的工具偏移为0，当选择图像时，偏移才有效
	// TODO: Add your control notification handler code here
	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();
	g_pFrm->m_pSysParam->tNeedleAdjustPos[1].x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pSysParam->tNeedleAdjustPos[1].y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];

	//tOffset[i]中存储针头与图像坐标之间偏移
	g_pFrm->m_pSysParam->tOffset[1].x = g_pFrm->m_pSysParam->tNeedleAdjustPos[0].x - g_pFrm->m_pSysParam->tNeedleAdjustPos[1].x;
	g_pFrm->m_pSysParam->tOffset[1].y = g_pFrm->m_pSysParam->tNeedleAdjustPos[0].y - g_pFrm->m_pSysParam->tNeedleAdjustPos[1].y;
	////////////////////////////////
    CString strTemp; 
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tOffset[1].x);
	SetDlgItemText(IDC_DLG_NEEDLE_ADJUST_OFFX, strTemp);
	strTemp.Format("%.3f", g_pFrm->m_pSysParam->tOffset[1].y);
	SetDlgItemText(IDC_DLG_NEEDLE_ADJUST_OFFY, strTemp);

	g_pFrm->SysParam(FALSE);//写参数
	g_pView->Invalidate();
}

void CDlgNeedleAdjust::OnBnClickedDlgNeedleAdjustDisp()
{
	// TODO: Add your control notification handler code here
	double x,y,z;
	CString str;
	GetDlgItemText(IDC_DLG_NEEDLE_ADJUST_DELTAH,str);
	double zHeight = atof(str);
	g_pFrm->m_pSysParam->dNeedleAdjustUpHeight = zHeight;

	GetDlgItemText(IDC_DLG_NEEDLE_ADJUST_DISPTIME,str);
	double dDispTime = atof(str);
	g_pFrm->m_pSysParam->dNeedleAdjustDispTime = dDispTime;

	GetDlgItemText(IDC_DLG_NEEDLE_ADJUST_OFFTIME,str);
	double dWaitTime = atof(str);
	g_pFrm->m_pSysParam->dNeedleAdjustWaitTime = dWaitTime;

	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();
	g_pFrm->m_pSysParam->tNeedleAdjustPos[0].x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pSysParam->tNeedleAdjustPos[0].y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];

	z= g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS]-zHeight;
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

	g_pFrm->m_Robot->ExOutput(_T("Disp1"),TRUE);
	CFunction::DelaySec(dDispTime);
	g_pFrm->m_Robot->ExOutput(_T("Disp1"),FALSE);
	CFunction::DelaySec(dWaitTime);

	g_pFrm->m_Robot->GotoSafeZpos();

	x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS] - g_pFrm->m_pSysParam->tOffset[1].x;
	y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS] - g_pFrm->m_pSysParam->tOffset[1].y;
	z= g_pFrm->m_pSysParam->tPixelAdjustPos.z;

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

}

void CDlgNeedleAdjust::OnBnClickedDlgNeedleAdjustJog()
{
	// TODO: Add your control notification handler code here
	CDlgJogXYZ m_JogXYZ(_T("手动JOG"));
	m_JogXYZ.DoModal();
}