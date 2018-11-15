// DlgLineDispense.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgLineParam.h"


// CDlgLineDispense dialog
IMPLEMENT_DYNAMIC(CDlgLineParam, CDialog)

CDlgLineParam::CDlgLineParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLineParam::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgLineParam::~CDlgLineParam()
{

}
void CDlgLineParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CDlgLineParam, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgLineParam::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLineParam message handlers
BOOL CDlgLineParam::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLinePara,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLinePara,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLinePara,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLinePara,"Tag_3"));
	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLinePara,"Tag_4"));

	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLinePara,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLinePara,"Btn_2"));

	// TODO:  Add extra initialization here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_LINE_PARAM_EDIT1, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_LINE_PARAM_EDIT2, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_LINE_PARAM_EDIT3, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[3]);
		SetDlgItemText(IDC_LINE_PARAM_EDIT4, strTemp);		
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[4]);
		SetDlgItemText(IDC_LINE_PARAM_EDIT5, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[5]);
		SetDlgItemText(IDC_LINE_PARAM_EDIT6, strTemp);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgLineParam::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		GetDlgItemText(IDC_LINE_PARAM_EDIT1, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

		GetDlgItemText(IDC_LINE_PARAM_EDIT2, strTemp);
		m_pCmdLine->pDblValue[1] = atof(strTemp);

		GetDlgItemText(IDC_LINE_PARAM_EDIT3, strTemp);
		m_pCmdLine->pDblValue[2] = atof(strTemp);

		GetDlgItemText(IDC_LINE_PARAM_EDIT4, strTemp);
		m_pCmdLine->pDblValue[3] = atof(strTemp);
		GetDlgItemText(IDC_LINE_PARAM_EDIT5, strTemp);
		m_pCmdLine->pDblValue[4] = atof(strTemp);
		GetDlgItemText(IDC_LINE_PARAM_EDIT6, strTemp);
		m_pCmdLine->pDblValue[5] = atof(strTemp);
	}
	CDialog::OnOK();
}
void CDlgLineParam::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
