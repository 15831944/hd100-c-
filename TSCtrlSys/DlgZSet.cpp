// DlgZSet.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgZSet.h"


// CDlgZSet dialog

IMPLEMENT_DYNAMIC(CDlgZSet, CDialog)

CDlgZSet::CDlgZSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgZSet::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgZSet::~CDlgZSet()
{
}

void CDlgZSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgZSet, CDialog)

	ON_BN_CLICKED(IDOK, &CDlgZSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgZSet message handlers

BOOL CDlgZSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetHight,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetHight,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetHight,"Tag_2"));

	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetHight,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetHight,"Btn_2"));
	GetDlgItem(IDC_RADIO_TYPE_DELTA_HEIGHT)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetHight,"Btn_3"));
	GetDlgItem(IDC_RADIO_TYPE_ABS_HEIGHT)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetHight,"Btn_4"));

	// TODO:  Add extra initialization here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		CWnd::CheckRadioButton(IDC_RADIO_TYPE_DELTA_HEIGHT, IDC_RADIO_TYPE_ABS_HEIGHT, m_pCmdLine->pIntValue[0]==0 ? IDC_RADIO_TYPE_ABS_HEIGHT:IDC_RADIO_TYPE_DELTA_HEIGHT);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_EDIT_ZSET, strTemp);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgZSet::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		if(CWnd::GetCheckedRadioButton(IDC_RADIO_TYPE_DELTA_HEIGHT,IDC_RADIO_TYPE_ABS_HEIGHT)==IDC_RADIO_TYPE_ABS_HEIGHT)
			m_pCmdLine->pIntValue[0] = 0;//¾ø¶Ô
		else
			m_pCmdLine->pIntValue[0] = 1;//Ïà¶Ô

		GetDlgItemText(IDC_EDIT_ZSET, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

	}
	CDialog::OnOK();
}

void CDlgZSet::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
}
void CDlgZSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
