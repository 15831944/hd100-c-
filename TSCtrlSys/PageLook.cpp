// PageLook.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "PageLook.h"


// CPageLook dialog

IMPLEMENT_DYNAMIC(CPageLook, CPropertyPage)

CPageLook::CPageLook()
	: CPropertyPage(CPageLook::IDD)
{

}

CPageLook::~CPageLook()
{
}

void CPageLook::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageLook, CPropertyPage)
	ON_CONTROL_RANGE(EN_CHANGE,IDC_EDIT_P1,IDC_EDIT_P5, &CPageLook::OnEnChangeEditP1)
END_MESSAGE_MAP()


// CPageLook message handlers

BOOL CPageLook::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strTemp;
	strTemp.Format("%.4f", g_pFrm->m_pSysParam->dLook[0]);
	SetDlgItemText(IDC_EDIT_P1, strTemp);
	strTemp.Format("%.4f", g_pFrm->m_pSysParam->dLook[1]);
	SetDlgItemText(IDC_EDIT_P2, strTemp);
	strTemp.Format("%.4f", g_pFrm->m_pSysParam->dLook[2]);
	SetDlgItemText(IDC_EDIT_P3, strTemp);
	strTemp.Format("%.4f", g_pFrm->m_pSysParam->dLook[3]);
	SetDlgItemText(IDC_EDIT_P5, strTemp);
	strTemp.Format("%.4f", g_pFrm->m_pSysParam->dLook[4]);
	SetDlgItemText(IDC_EDIT_P6, strTemp);

	SetDlgItemInt(IDC_EDIT_P4, g_pFrm->m_pSysParam->iLookNum);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageLook::OnApply()
{
	// TODO: Add your specialized code here and/or call the base class
	CString strTemp;
	GetDlgItemText(IDC_EDIT_P1, strTemp);
	g_pFrm->m_pSysParam->dLook[0] = atof(strTemp);

	GetDlgItemText(IDC_EDIT_P2, strTemp);
	g_pFrm->m_pSysParam->dLook[1] = atof(strTemp);

	GetDlgItemText(IDC_EDIT_P3, strTemp);
	g_pFrm->m_pSysParam->dLook[2] = atof(strTemp);

	GetDlgItemText(IDC_EDIT_P5, strTemp);
	g_pFrm->m_pSysParam->dLook[3] = atof(strTemp);
	GetDlgItemText(IDC_EDIT_P6, strTemp);
	g_pFrm->m_pSysParam->dLook[4] = atof(strTemp);


	g_pFrm->m_pSysParam->iLookNum = GetDlgItemInt(IDC_EDIT_P4);
	g_pFrm->SysParam(FALSE);

	return CPropertyPage::OnApply();
}

void CPageLook::OnEnChangeEditP1(UINT Id)
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UNREFERENCED_PARAMETER(Id);
	SetModified();
}
