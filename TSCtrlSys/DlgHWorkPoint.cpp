// DlgHWorkPoint.cpp : implementation file
//

#include "stdafx.h"
#include "DlgHWorkPoint.h"
#include "TSCtrlSys.h"

// CDlgHWorkPoint dialog

IMPLEMENT_DYNAMIC(CDlgHWorkPoint, CDialog)

CDlgHWorkPoint::CDlgHWorkPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHWorkPoint::IDD, pParent)
{

}

CDlgHWorkPoint::~CDlgHWorkPoint()
{
}

void CDlgHWorkPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHWorkPoint, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgHWorkPoint::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgHWorkPoint::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_READ_AXISPOS, &CDlgHWorkPoint::OnBnClickedBtnReadAxispos)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDlgHWorkPoint::OnBnClickedBtnMove)
END_MESSAGE_MAP()


// CDlgHWorkPoint message handlers

void CDlgHWorkPoint::OnBnClickedOk()
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

		GetDlgItemText(IDC_AXIS_HPOS_EDIT_Z2, strTemp);
		m_pCmdLine->pDblValue[3] = atof(strTemp);

		// X微调距离
        GetDlgItemText(IDC_AXIS_HPOS_EDIT_X2, strTemp);
		m_pCmdLine->pDblValue[6] = atof(strTemp);
		
		// Y微调距离
		GetDlgItemText(IDC_AXIS_HPOS_EDIT_Y2, strTemp);
		m_pCmdLine->pDblValue[7] = atof(strTemp);
		
	}
    
	g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);

	OnOK();
}

void CDlgHWorkPoint::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDlgHWorkPoint::OnBnClickedBtnReadAxispos()
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

void CDlgHWorkPoint::OnBnClickedBtnMove()
{
	g_pFrm->m_Robot->GotoSafeZpos();

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,m_pCmdLine->pDblValue[0],m_pCmdLine->pDblValue[1],g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
//	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,m_pCmdLine->pDblValue[2], g_pFrm->m_mtrParam[Z_AXIS].m_Vmax,g_pFrm->m_mtrParam[Z_AXIS].m_Acc,TRUE);

}

BOOL CDlgHWorkPoint::OnInitDialog()
{
	CDialog::OnInitDialog();
// 
// 	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_1"));
// 	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_2"));
// 	GetDlgItem(IDC_BTN_READ_AXISPOS)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_3"));
// 	GetDlgItem(IDC_BTN_MOVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_4"));
// 	
	RefreshAxisPos();

	// // X微调距离
	CString strTemp;
	strTemp.Format("%.3f",m_pCmdLine->pDblValue[6]);
	SetDlgItemText(IDC_AXIS_HPOS_EDIT_X2, strTemp);
    // Y微调距离
	strTemp.Format("%.3f",m_pCmdLine->pDblValue[7]);
	SetDlgItemText(IDC_AXIS_HPOS_EDIT_Y2, strTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHWorkPoint::RefreshAxisPos(void)
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
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_Z2, strTemp);
	}
}
