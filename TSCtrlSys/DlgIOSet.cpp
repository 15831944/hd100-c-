// DlgIO.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgIOSet.h"


// CDlgIOCtrl dialog
IMPLEMENT_DYNAMIC(CDlgIOSet, CDialog)

CDlgIOSet::CDlgIOSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIOSet::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgIOSet::~CDlgIOSet()
{

}

void CDlgIOSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgIOSet, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgIOSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgIOCtrl message handlers

BOOL CDlgIOSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDIO,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDIO,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDIO,"Tag_2"));
	
	GetDlgItem(IDC_RADIO_TYPEINPUT)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDIO,"Btn_1"));
	GetDlgItem(IDC_RADIO_TYPEOUTPUT)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDIO,"Btn_2"));
	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDIO,"Btn_3"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDIO,"Btn_4"));

	// TODO:  Add extra initialization here
	if(m_pCmdLine != NULL)
	{
		// ¶Ë¿ÚÀàÐÍ 
		CWnd::CheckRadioButton(IDC_RADIO_TYPEINPUT, IDC_RADIO_TYPEOUTPUT, IDC_RADIO_TYPEINPUT+m_pCmdLine->pIntValue[0]);

		SetDlgItemInt(IDC_IO_EDIT_NO,m_pCmdLine->pIntValue[1]);
		CWnd::CheckRadioButton(IDC_IO_RADIO_0, IDC_IO_RADIO_1, IDC_IO_RADIO_0+m_pCmdLine->pIntValue[2]);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgIOSet::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		m_pCmdLine->pIntValue[0] = CWnd::GetCheckedRadioButton(IDC_RADIO_TYPEINPUT,IDC_RADIO_TYPEOUTPUT)-IDC_RADIO_TYPEINPUT;
		m_pCmdLine->pIntValue[1] = GetDlgItemInt(IDC_IO_EDIT_NO);
		m_pCmdLine->pIntValue[2] = CWnd::GetCheckedRadioButton(IDC_IO_RADIO_0,IDC_IO_RADIO_1)-IDC_IO_RADIO_0;
	}
	CDialog::OnOK();
}
void CDlgIOSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
