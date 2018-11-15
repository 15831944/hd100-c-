// DLgSpeed.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DLgSpeed.h"


// CDLgSpeed dialog

IMPLEMENT_DYNAMIC(CDLgSpeed, CDialog)

CDLgSpeed::CDLgSpeed(CWnd* pParent /*=NULL*/)
	: CDialog(CDLgSpeed::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDLgSpeed::~CDLgSpeed()
{

}

void CDLgSpeed::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLgSpeed, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_SPEED_RADIO1, IDC_SPEED_RADIO3, &CDLgSpeed::OnBnClickedRadio1)
	ON_BN_CLICKED(IDOK, &CDLgSpeed::OnBnClickedOk)
END_MESSAGE_MAP()


// CDLgSpeed message handlers

BOOL CDLgSpeed::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSpeed,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSpeed,"Tag_1"));
	
	GetDlgItem(IDC_SPEED_RADIO1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSpeed,"Btn_1"));
	GetDlgItem(IDC_SPEED_RADIO2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSpeed,"Btn_2"));
	GetDlgItem(IDC_SPEED_RADIO3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSpeed,"Btn_3"));
	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSpeed,"Btn_4"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSpeed,"Btn_5"));

	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		SetDlgItemText(IDC_SPEED_EDIT_TYPE, szCmdline[m_pCmdLine->tType]);
	
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_SPEED_EDIT_VEL, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_SPEED_EDIT_VEL2, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_SPEED_EDIT_VEL3, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[3]);
		SetDlgItemText(IDC_SPEED_EDIT_VEL4, strTemp);
		
		UINT id[] = {/*IDC_SPEED_STATIC_Z, IDC_SPEED_EDIT_VEL2, */IDC_SPEED_RADIO1, IDC_SPEED_RADIO2, IDC_SPEED_RADIO3};
		for(int i=0; i<3; i++)
		{
			GetDlgItem(id[i])->ShowWindow(SW_HIDE);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDLgSpeed::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		{
			GetDlgItemText(IDC_SPEED_EDIT_VEL, strTemp);
			m_pCmdLine->pDblValue[0] = atof(strTemp);
			GetDlgItemText(IDC_SPEED_EDIT_VEL2, strTemp);
			m_pCmdLine->pDblValue[1] = atof(strTemp);
			GetDlgItemText(IDC_SPEED_EDIT_VEL3, strTemp);
			m_pCmdLine->pDblValue[2] = atof(strTemp);
			GetDlgItemText(IDC_SPEED_EDIT_VEL4, strTemp);
			m_pCmdLine->pDblValue[3] = atof(strTemp);
		}
	}
	CDialog::OnOK();
}

void CDLgSpeed::OnBnClickedRadio1(UINT ID)
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		int n=ID-IDC_SPEED_RADIO1;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*2+0]);
		SetDlgItemText(IDC_SPEED_EDIT_VEL, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*2+1]);
		SetDlgItemText(IDC_SPEED_EDIT_VEL2, strTemp);
	}
}

void CDLgSpeed::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
