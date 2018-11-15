// DispenseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgDispenser.h"


// CDlgDispenser dialog
IMPLEMENT_DYNAMIC(CDlgDispenser, CDialog)

CDlgDispenser::CDlgDispenser(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDispenser::IDD, pParent)
{

}
CDlgDispenser::~CDlgDispenser()
{
}
void CDlgDispenser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CDlgDispenser, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgDispenser::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_OPER_RADIO_HEAD, &CDlgDispenser::OnCbnSelchangeRadioHead)
END_MESSAGE_MAP()


// CDispenseDlg message handlers
BOOL CDlgDispenser::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJETCTRL,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJETCTRL,"Tag_1"));

	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJETCTRL,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJETCTRL,"Btn_2"));
    	GetDlgItem(IDC_OPER_CHECK_DISPENSE_ONOFF)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDJETCTRL,"Btn_3"));
	CString str;
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_OPER_RADIO_HEAD);
	for (int i=0;i<TOOL_NUM;i++)
	{
		str.Empty();
		str.Format("#%d",i);
		pBox->AddString(str);
	}

	pBox->SetCurSel(m_pCmdLine->pIntValue[0]);
	((CButton*)GetDlgItem(IDC_OPER_CHECK_DISPENSE_ONOFF))->SetCheck(m_pCmdLine->pIntValue[1]);

	return TRUE;
}
void CDlgDispenser::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_OPER_RADIO_HEAD);
	m_pCmdLine->pIntValue[0] = pBox->GetCurSel();
	m_pCmdLine->pIntValue[1] = ((CButton*)GetDlgItem(IDC_OPER_CHECK_DISPENSE_ONOFF))->GetCheck();
	OnOK();
}
void CDlgDispenser::OnCbnSelchangeRadioHead()
{
	// TODO: Add your control notification handler code here
	//CString str;

}

void CDlgDispenser::OnBnClickedCheckSideUpdown()
{
	// TODO: Add your control notification handler code here
}
