// DlgLineEnd.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgLineEnd.h"


// CDlgLineEnd dialog

IMPLEMENT_DYNAMIC(CDlgLineEnd, CDialog)

CDlgLineEnd::CDlgLineEnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLineEnd::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgLineEnd::~CDlgLineEnd()
{

}

void CDlgLineEnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLineEnd, CDialog)

	ON_BN_CLICKED(IDOK, &CDlgLineEnd::OnBnClickedOk)
	ON_BN_CLICKED(IDC_LINE_END_RADIO5, &CDlgLineEnd::OnBnClickedRadio5)
END_MESSAGE_MAP()


// CDlgLineEnd message handlers
BOOL CDlgLineEnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLineEnd,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLineEnd,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLineEnd,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLineEnd,"Tag_3"));
	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLineEnd,"Tag_4"));

	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLineEnd,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDLineEnd,"Btn_2"));

	// TODO:  Add extra initialization here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_LINE_END_EDIT1, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_LINE_END_EDIT2, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_LINE_END_EDIT3, strTemp);

		CWnd::CheckRadioButton(IDC_LINE_END_RADIO1, IDC_LINE_END_RADIO5, IDC_LINE_END_RADIO1+m_pCmdLine->pIntValue[0]);

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLineEnd::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		GetDlgItemText(IDC_LINE_END_EDIT1, strTemp);
		m_pCmdLine->pDblValue[0] =atof(strTemp);

		GetDlgItemText(IDC_LINE_END_EDIT2, strTemp);
		m_pCmdLine->pDblValue[1] =atof(strTemp);

		GetDlgItemText(IDC_LINE_END_EDIT3, strTemp);
		m_pCmdLine->pDblValue[2] =atof(strTemp);

		switch (CWnd::GetCheckedRadioButton(IDC_LINE_END_RADIO1, IDC_LINE_END_RADIO5))
		{
		case IDC_LINE_END_RADIO1:
			m_pCmdLine->pIntValue[0] = 0;
			break;
		case IDC_LINE_END_RADIO2:
			m_pCmdLine->pIntValue[0] = 1;
			break;
		case IDC_LINE_END_RADIO3:
			m_pCmdLine->pIntValue[0] = 2;
			break;
		case IDC_LINE_END_RADIO4:
			m_pCmdLine->pIntValue[0] = 3;
			break;
		case IDC_LINE_END_RADIO5:
			m_pCmdLine->pIntValue[0] = 4;
			break;
		default:break;
		}
	}
	CDialog::OnOK();
}
void CDlgLineEnd::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgLineEnd::OnBnClickedRadio5()
{
	// TODO: Add your control notification handler code here
}
