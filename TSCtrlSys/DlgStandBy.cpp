// DlgStandBy.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgStandBy.h"


// CDlgStandBy dialog

IMPLEMENT_DYNAMIC(CDlgStandBy, CDialog)

CDlgStandBy::CDlgStandBy(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStandBy::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgStandBy::~CDlgStandBy()
{
}

void CDlgStandBy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgStandBy, CDialog)
	ON_BN_CLICKED(IDC_BTN_READ_AXISPOS, &CDlgStandBy::OnBnClickedBtnReadAxispos)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDlgStandBy::OnBnClickedBtnMove)
	ON_BN_CLICKED(IDOK, &CDlgStandBy::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgStandBy message handlers

void CDlgStandBy::OnBnClickedBtnReadAxispos()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, 0);
		RefreshAxisPos();
	}
}

void CDlgStandBy::RefreshAxisPos(void)
{
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_STANDBY_EDIT_X, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_STANDBY_EDIT_Y, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_STANDBY_EDIT_Z, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[3]);
		SetDlgItemText(IDC_STANDBY_EDIT_U, strTemp);
	}
}
void CDlgStandBy::OnBnClickedBtnMove()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		g_pFrm->m_Robot->GotoSafeZpos();
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,m_pCmdLine->pDblValue[0],m_pCmdLine->pDblValue[1],g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,m_pCmdLine->pDblValue[2], g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
	}
}

void CDlgStandBy::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (m_pCmdLine!=NULL)
	{
		CString strTemp;
		GetDlgItemText(IDC_STANDBY_EDIT_X, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

		GetDlgItemText(IDC_STANDBY_EDIT_Y, strTemp);
		m_pCmdLine->pDblValue[1] = atof(strTemp);

		GetDlgItemText(IDC_STANDBY_EDIT_Z, strTemp);
		m_pCmdLine->pDblValue[2] = atof(strTemp);

		GetDlgItemText(IDC_STANDBY_EDIT_U, strTemp);
		m_pCmdLine->pDblValue[3] = atof(strTemp);
	}
	OnOK();
}
BOOL CDlgStandBy::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDStandBy,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDStandBy,"Tag_1"));

	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDStandBy,"Btn_3"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDStandBy,"Btn_4"));
	GetDlgItem(IDC_BTN_READ_AXISPOS)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDStandBy,"Btn_1"));
	GetDlgItem(IDC_BTN_MOVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDStandBy,"Btn_2"));
	
	if(m_pCmdLine != NULL)
	{
		SetDlgItemText(IDC_STANDBY_CMD_TYPE, szCmdline[m_pCmdLine->tType]);
		RefreshAxisPos();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
