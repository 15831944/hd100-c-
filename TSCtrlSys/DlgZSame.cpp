// DlgZUPLimit.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgZSame.h"


// CDlgZSame dialog

IMPLEMENT_DYNAMIC(CDlgZSame, CDialog)

CDlgZSame::CDlgZSame(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgZSame::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgZSame::~CDlgZSame()
{

}

void CDlgZSame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgZSame, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgZSame::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgZUPLimit message handlers
BOOL CDlgZSame::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDZlimit,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDZlimit,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDZlimit,"Tag_2"));

	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDZlimit,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDZlimit,"Btn_2"));
	
	if(m_pCmdLine != NULL)
	{
		CString strTemp;

		SetDlgItemText(IDC_ZSAME_EDIT_TYPE, szCmdline[m_pCmdLine->tType]);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_ZSAME_EDIT_ZHIGH, strTemp);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgZSame::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		CString strTemp;

		GetDlgItemText(IDC_ZSAME_EDIT_ZHIGH, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

	}
	CDialog::OnOK();
}
void CDlgZSame::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
