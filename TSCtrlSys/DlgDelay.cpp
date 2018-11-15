// DlgZUPLimit.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgDelay.h"


// CDlgZUPLimit dialog

IMPLEMENT_DYNAMIC(CDlgDelay, CDialog)

CDlgDelay::CDlgDelay(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDelay::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgDelay::~CDlgDelay()
{

}

void CDlgDelay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDelay, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgDelay::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgDelay message handlers

BOOL CDlgDelay::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDDELAY,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDDELAY,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDDELAY,"Tag_2"));
	
	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDDELAY,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDDELAY,"Btn_2"));

	// TODO:  Add extra initialization here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;

		SetDlgItemText(IDC_DELAY_EDIT_TYPE, szCmdline[m_pCmdLine->tType]);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_DELAY_EDIT, strTemp);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDelay::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		CString strTemp;

		GetDlgItemText(IDC_DELAY_EDIT, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

	}
	CDialog::OnOK();
}
void CDlgDelay::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
