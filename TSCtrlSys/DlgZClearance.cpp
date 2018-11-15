// DlgZClearance.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgZClearance.h"


// CDlgZCLearance dialog

IMPLEMENT_DYNAMIC(CDlgZCLearance, CDialog)

CDlgZCLearance::CDlgZCLearance(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgZCLearance::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgZCLearance::~CDlgZCLearance()
{

}

void CDlgZCLearance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgZCLearance, CDialog)

	ON_BN_CLICKED(IDOK, &CDlgZCLearance::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgZUPLimit message handlers
BOOL CDlgZCLearance::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDZClear,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDZClear,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDZClear,"Tag_2"));
	
	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDZClear,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDZClear,"Btn_2"));
	GetDlgItem(IDC_RADIO_TYPE_DELTA_HEIGHT)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDZClear,"Btn_3"));
	GetDlgItem(IDC_RADIO_TYPE_ABS_HEIGHT)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDZClear,"Btn_4"));

	if(m_pCmdLine != NULL)
	{
		CString strTemp;

		CWnd::CheckRadioButton(IDC_RADIO_TYPE_DELTA_HEIGHT, IDC_RADIO_TYPE_ABS_HEIGHT, m_pCmdLine->pIntValue[0]==0 ? IDC_RADIO_TYPE_ABS_HEIGHT:IDC_RADIO_TYPE_DELTA_HEIGHT);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_EDIT_ZCLEARANCE, strTemp);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgZCLearance::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		CString strTemp;

		if(CWnd::GetCheckedRadioButton(IDC_RADIO_TYPE_DELTA_HEIGHT,IDC_RADIO_TYPE_ABS_HEIGHT)==IDC_RADIO_TYPE_ABS_HEIGHT)
			m_pCmdLine->pIntValue[0] = 0;//¾ø¶Ô
		else
			m_pCmdLine->pIntValue[0] = 1;//Ïà¶Ô

		GetDlgItemText(IDC_EDIT_ZCLEARANCE, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

	}
	CDialog::OnOK();
}

void CDlgZCLearance::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
