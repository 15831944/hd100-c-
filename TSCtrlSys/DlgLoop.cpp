// DlgLoop.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgLoop.h"


// CDlgLoop dialog

IMPLEMENT_DYNAMIC(CDlgLoop, CDialog)

CDlgLoop::CDlgLoop(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLoop::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgLoop::~CDlgLoop()
{

}

void CDlgLoop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLoop, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgLoop::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLoop message handlers

BOOL CDlgLoop::OnInitDialog()
{
	CDialog::OnInitDialog();
   
	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLOOP,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLOOP,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLOOP,"Tag_2"));

	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLOOP,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLOOP,"Btn_2"));

	// TODO:  Add extra initialization here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("ÐòºÅ·¶Î§:0~%d", g_pFrm->m_wndEditBar.GetCmdCurSel()-1);
		//SetDlgItemText(IDC_STATIC_NOTE, strTemp);

		SetDlgItemInt(IDC_LOOP_EDIT_STARTINDEX, m_pCmdLine->pIntValue[1]);
		SetDlgItemInt(IDC_LOOP_EDIT_LOOPNUM,m_pCmdLine->pIntValue[0]);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgLoop::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		m_pCmdLine->pIntValue[1] = GetDlgItemInt(IDC_LOOP_EDIT_STARTINDEX);
		m_pCmdLine->pIntValue[0] = GetDlgItemInt(IDC_LOOP_EDIT_LOOPNUM);
	}
	CDialog::OnOK();
}
void CDlgLoop::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
