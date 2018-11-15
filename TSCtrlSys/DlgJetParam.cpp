// DlgJetParam.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgJetParam.h"


// CDlgJetParam dialog

IMPLEMENT_DYNAMIC(CDlgJetParam, CDialog)

CDlgJetParam::CDlgJetParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgJetParam::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgJetParam::~CDlgJetParam()
{
}

void CDlgJetParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgJetParam, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgJetParam::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgJetParam message handlers

BOOL CDlgJetParam::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetPara,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetPara,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetPara,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetPara,"Tag_3"));
	
	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetPara,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJetPara,"Btn_2"));
	
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_JET_PARAM_PULSE_NUM, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_JET_PARAM_OPEN_TIME, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_JET_PARAM_CLOSE_TIME, strTemp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgJetParam::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		CString strTemp;

		GetDlgItemText(IDC_JET_PARAM_PULSE_NUM, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

		GetDlgItemText(IDC_JET_PARAM_OPEN_TIME, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

		GetDlgItemText(IDC_JET_PARAM_CLOSE_TIME, strTemp);
		m_pCmdLine->pDblValue[1] = atof(strTemp);

	}

	CDialog::OnOK();
}

void CDlgJetParam::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
