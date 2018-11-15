// DlgPointDispense.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgPointParam.h"


// CDlgPointParam dialog

IMPLEMENT_DYNAMIC(CDlgPointParam, CDialog)

CDlgPointParam::CDlgPointParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPointParam::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgPointParam::~CDlgPointParam()
{

}

void CDlgPointParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPointParam, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPointParam::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgPointDispense message handlers

BOOL CDlgPointParam::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDPointPara,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDPointPara,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDPointPara,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDPointPara,"Tag_3"));

	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDPointPara,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDPointPara,"Btn_2"));

	// TODO:  Add extra initialization here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_POINT_PARAM_EDIT1, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_POINT_PARAM_EDIT2, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_POINT_PARAM_EDIT3, strTemp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPointParam::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		CString strTemp;

		GetDlgItemText(IDC_POINT_PARAM_EDIT1, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

		GetDlgItemText(IDC_POINT_PARAM_EDIT2, strTemp);
		m_pCmdLine->pDblValue[1] = atof(strTemp);

		GetDlgItemText(IDC_POINT_PARAM_EDIT3, strTemp);
		m_pCmdLine->pDblValue[2] = atof(strTemp);

	}

	CDialog::OnOK();
}
void CDlgPointParam::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
