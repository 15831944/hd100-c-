// DlgPointEnd.cpp : implementation file
//
#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgPointEnd.h"


// CDlgPointEnd dialog

IMPLEMENT_DYNAMIC(CDlgPointEnd, CDialog)

CDlgPointEnd::CDlgPointEnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPointEnd::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgPointEnd::~CDlgPointEnd()
{

}

void CDlgPointEnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPointEnd, CDialog)

	ON_BN_CLICKED(IDOK, &CDlgPointEnd::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLineEnd message handlers
BOOL CDlgPointEnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDPointEnd,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDPointEnd,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDPointEnd,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDPointEnd,"Tag_3"));
	
	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDPointEnd,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDPointEnd,"Btn_2"));

	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_EDIT_POINT_END1, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_EDIT_POINT_END2, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_EDIT_POINT_END_LEN, strTemp);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPointEnd::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		GetDlgItemText(IDC_EDIT_POINT_END1, strTemp);
		m_pCmdLine->pDblValue[0] =atof(strTemp);
		GetDlgItemText(IDC_EDIT_POINT_END2, strTemp);
		m_pCmdLine->pDblValue[1] =atof(strTemp);
		GetDlgItemText(IDC_EDIT_POINT_END_LEN, strTemp);
		m_pCmdLine->pDblValue[2] =atof(strTemp);
	}
	CDialog::OnOK();
}
void CDlgPointEnd::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
