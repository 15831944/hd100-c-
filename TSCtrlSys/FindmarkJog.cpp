// FindmarkJog.cpp : implementation file
//

#include "stdafx.h"
#include "FindmarkJog.h"
#include "TSCtrlSys.h"

// CFindmarkJog dialog

IMPLEMENT_DYNAMIC(CFindmarkJog, CDialog)

CFindmarkJog::CFindmarkJog(CWnd* pParent /*=NULL*/)
	: CDialog(CFindmarkJog::IDD, pParent)
{

}

CFindmarkJog::~CFindmarkJog()
{
}

void CFindmarkJog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFindmarkJog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CFindmarkJog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CFindmarkJog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SET_LINE_YN, &CFindmarkJog::OnBnClickedSetLineYn)
	ON_BN_CLICKED(IDC_SET_LINE_YP, &CFindmarkJog::OnBnClickedSetLineYp)
	ON_BN_CLICKED(IDC_SET_LINE_XN, &CFindmarkJog::OnBnClickedSetLineXn)
	ON_BN_CLICKED(IDC_SET_LINE_XP, &CFindmarkJog::OnBnClickedSetLineXp)
END_MESSAGE_MAP()

// CFindmarkJog message handlers

BOOL CFindmarkJog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_EDIT_STEPNUM, _T("0.2"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFindmarkJog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CFindmarkJog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CFindmarkJog::OnBnClickedSetLineYn()
{
   CString strtemp;
  GetDlgItemText(IDC_EDIT_STEPNUM, strtemp);
  double nlong =0;
  nlong = atof(strtemp);

	double t_vel = 10;
	double t_acc = 0.2;
	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();
	double xx = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	double yy = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	double tempx=xx;
	double tempy=yy;

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx, tempy-nlong,t_vel,t_acc);

}

void CFindmarkJog::OnBnClickedSetLineYp()
{
	CString strtemp;
	GetDlgItemText(IDC_EDIT_STEPNUM, strtemp);
	double nlong =0;
	nlong = atof(strtemp);
	double t_vel = 10;
	double t_acc = 0.2;
	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();
	double xx = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	double yy = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	double tempx=xx;
	double tempy=yy;

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx, tempy+nlong,t_vel,t_acc);
}

void CFindmarkJog::OnBnClickedSetLineXn()
{
	CString strtemp;
	GetDlgItemText(IDC_EDIT_STEPNUM, strtemp);
	double nlong =0;
	nlong = atof(strtemp);
	double t_vel = 10;
	double t_acc = 0.2;
	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();
	double xx = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	double yy = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	double tempx=xx;
	double tempy=yy;

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx-nlong, tempy,t_vel,t_acc);
}

void CFindmarkJog::OnBnClickedSetLineXp()
{
	CString strtemp;
	GetDlgItemText(IDC_EDIT_STEPNUM, strtemp);
	double nlong =0;
	nlong = atof(strtemp);
	double t_vel = 10;
	double t_acc = 0.2;
	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();
	double xx = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	double yy = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	double tempx=xx;
	double tempy=yy;

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,tempx+nlong, tempy,t_vel,t_acc);
}
