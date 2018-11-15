// DlgHStartPoint.cpp : implementation file
//

#include "stdafx.h"
#include "DlgHStartPoint.h"
#include "TSCtrlSys.h"

// CDlgHStartPoint dialog

IMPLEMENT_DYNAMIC(CDlgHStartPoint, CDialog)

CDlgHStartPoint::CDlgHStartPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHStartPoint::IDD, pParent)
{

}

CDlgHStartPoint::~CDlgHStartPoint()
{
}

void CDlgHStartPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHStartPoint, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgHStartPoint::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_READ_AXISPOS, &CDlgHStartPoint::OnBnClickedBtnReadAxispos)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDlgHStartPoint::OnBnClickedBtnMove)
	ON_BN_CLICKED(IDCANCEL, &CDlgHStartPoint::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgHStartPoint message handlers

void CDlgHStartPoint::OnBnClickedOk()
{
	CString strTemp;
	if(m_pCmdLine != NULL)
	{
		GetDlgItemText(IDC_AXIS_HPOS_EDIT_X, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_HPOS_EDIT_Y, strTemp);
		m_pCmdLine->pDblValue[1] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_HPOS_EDIT_Z, strTemp);
		m_pCmdLine->pDblValue[2] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_HPOS_EDIT_PREDOT, strTemp);
		m_pCmdLine->pDblValue[4] = atof(strTemp);
	}

	g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);

	OnOK();
}

BOOL CDlgHStartPoint::OnInitDialog()
{
	CDialog::OnInitDialog();
// 	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_1"));
// 	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_2"));
// 	GetDlgItem(IDC_BTN_READ_AXISPOS)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_3"));
// 	GetDlgItem(IDC_BTN_MOVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_4"));

	RefreshAxisPos();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHStartPoint::OnBnClickedBtnReadAxispos()
{
	if(m_pCmdLine != NULL)
	{
		if (IDYES == AfxMessageBox("是否要更新当前点位!", MB_YESNO))
		{
			g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, 0);
			RefreshAxisPos();
		}
	}
}

void CDlgHStartPoint::OnBnClickedBtnMove()
{
	g_pFrm->m_Robot->GotoSafeZpos();

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,m_pCmdLine->pDblValue[0],m_pCmdLine->pDblValue[1],g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
//	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,m_pCmdLine->pDblValue[2], g_pFrm->m_mtrParam[Z_AXIS].m_Vmax,g_pFrm->m_mtrParam[Z_AXIS].m_Acc,TRUE);
}

void CDlgHStartPoint::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
void CDlgHStartPoint::RefreshAxisPos(void)
{
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_X, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_Y, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_Z, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[4]);
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_PREDOT, strTemp);
	}
}