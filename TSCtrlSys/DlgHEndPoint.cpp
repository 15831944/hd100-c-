// DlgHEndPoint.cpp : implementation file
//

#include "stdafx.h"
#include "DlgHEndPoint.h"
#include "TSCtrlSys.h"

// CDlgHEndPoint dialog

IMPLEMENT_DYNAMIC(CDlgHEndPoint, CDialog)

CDlgHEndPoint::CDlgHEndPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHEndPoint::IDD, pParent)
{

}

CDlgHEndPoint::~CDlgHEndPoint()
{
}

void CDlgHEndPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHEndPoint, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgHEndPoint::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgHEndPoint::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_READ_AXISPOS, &CDlgHEndPoint::OnBnClickedBtnReadAxispos)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDlgHEndPoint::OnBnClickedBtnMove)
END_MESSAGE_MAP()


// CDlgHEndPoint message handlers

void CDlgHEndPoint::OnBnClickedOk()
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

		GetDlgItemText(IDC_AXIS_HPOS_EDIT_WEIGHT, strTemp);
		m_pCmdLine->pDblValue[3] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_HPOS_END_PREDOT, strTemp);
		m_pCmdLine->pDblValue[4] = atof(strTemp);
	}

	g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);

	OnOK();
}

BOOL CDlgHEndPoint::OnInitDialog()
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

void CDlgHEndPoint::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDlgHEndPoint::OnBnClickedBtnReadAxispos()
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

void CDlgHEndPoint::OnBnClickedBtnMove()
{
	g_pFrm->m_Robot->GotoSafeZpos();

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,m_pCmdLine->pDblValue[0],m_pCmdLine->pDblValue[1],g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
//	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,m_pCmdLine->pDblValue[2], g_pFrm->m_mtrParam[Z_AXIS].m_Vmax,g_pFrm->m_mtrParam[Z_AXIS].m_Acc,TRUE);

}
void CDlgHEndPoint::RefreshAxisPos(void)
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

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[3]);
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_WEIGHT, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[4]);
		SetDlgItemText(IDC_AXIS_HPOS_END_PREDOT, strTemp);
	}
}