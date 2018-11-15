// DlgLogin.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgLogin.h"


// CDlgLogin dialog

IMPLEMENT_DYNAMIC(CDlgLogin, CDialog)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin::IDD, pParent)
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PWD, &CDlgLogin::OnBnClickedButtonPwd)
	ON_BN_CLICKED(IDOK, &CDlgLogin::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_USERTYPE, &CDlgLogin::OnCbnSelchangeUsertype)
	ON_BN_CLICKED(IDCANCEL, &CDlgLogin::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_USER_LOGOFF, &CDlgLogin::OnBnClickedUserLogoff)
END_MESSAGE_MAP()


// CDlgLogin message handlers

void CDlgLogin::OnBnClickedButtonPwd()
{
	CString str1, str2,str3;
	GetDlgItemText(IDC_ACCESS_EDIT1, str1);
	GetDlgItemText(IDC_ACCESS_EDIT2, str2);
	str3.Format("%s",theApp.m_SysUser.m_CurUD.Password);
	char *cNewCh = (LPSTR)(LPCSTR)str2;
     
	CString  strtemp;
	if(str2.GetLength() < 4)
	{ 
		strtemp=g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Msg_4");
		AfxMessageBox(strtemp);
	}
	else if(0==str3.Compare(str1))
	{
		memcpy(theApp.m_SysUser.m_CurUD.Password,cNewCh,sizeof(char)*20);
		if(theApp.m_SysUser.ChangeUser(theApp.m_SysUser.m_CurUD))
		{
			memcpy(m_LoginData.Password,cNewCh,sizeof(char)*20);
			theApp.m_SysUser.SaveUserData();
			strtemp=g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Msg_5");
			AfxMessageBox(strtemp);
		}
		else
		{   
			strtemp=g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Msg_6");
			AfxMessageBox(strtemp);
		}
	}
	else
	{  
		strtemp=g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Msg_7");
		AfxMessageBox(strtemp);
	}
	OnOK();
}

void CDlgLogin::OnBnClickedOk()
{
	CString str1, str2;
	CString strtemp;
	GetDlgItemText(IDC_ACCESS_EDIT1, str1);
	CComboBox *pUserBox = (CComboBox*)GetDlgItem(IDC_USERTYPE);
	if(pUserBox->GetCurSel() == 0)
	{
		OnBnClickedUserLogoff();
		g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
		g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
		return;
	}
	char user[20] = "default";
	m_LoginData.level = pUserBox->GetCurSel()+1;
	memcpy(m_LoginData.UserName,user,sizeof(char)*20);
	char *pCh = (LPSTR)(LPCSTR)str1;
	memset(m_LoginData.Password,0,sizeof(char)*20);
	memcpy(m_LoginData.Password,pCh,sizeof(char)*str1.GetLength());
	int val = theApp.m_SysUser.FindUser(m_LoginData);
	if(val >= 0)
	{
		theApp.m_SysUser.m_CurUD = m_LoginData;
		theApp.m_SysUser.m_CurUD.uID = val;
		if(theApp.m_SysUser.m_CurUD.level > GM_ADMIN)
		{
//			g_pFrm->m_wndToolBar.RestoreOriginalstate();
		}
		g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
		g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
	}
	else
	{
		strtemp=g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Msg_8");
		AfxMessageBox(strtemp);
		g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(FALSE);
		g_pFrm->m_wndEditBar.m_wndList.EnableWindow(FALSE);
	}
	g_pFrm->Invalidate();
	OnOK();
}

void CDlgLogin::OnCbnSelchangeUsertype()
{
	CComboBox *pUserBox = (CComboBox*)GetDlgItem(IDC_USERTYPE);
	if(theApp.m_SysUser.m_CurUD.uID == pUserBox->GetCurSel() &&
		theApp.m_SysUser.m_CurUD.level == pUserBox->GetCurSel()+1)
	{
		GetDlgItem(IDC_ACCESS_EDIT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PWD)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ACCESS_EDIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PWD)->EnableWindow(FALSE);
	}
	if(pUserBox->GetCurSel() < GM_PERSONNEL)
	{
		GetDlgItem(IDC_ACCESS_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_ACCESS_EDIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PWD)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_ACCESS_EDIT1)->EnableWindow(TRUE);
	}
}

BOOL CDlgLogin::OnInitDialog()
{
	CDialog::OnInitDialog();
	CComboBox *pUserBox = (CComboBox*)GetDlgItem(IDC_USERTYPE);
  
	CString str1 = g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Msg_1");
	CString str2 = g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Msg_2");
	CString str3 = g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Msg_3");

	pUserBox->InsertString(0,str3);
	pUserBox->InsertString(0,str2);
	pUserBox->InsertString(0,str1);
	pUserBox->SetCurSel(theApp.m_SysUser.m_CurUD.uID);
	if(theApp.m_SysUser.m_CurUD.level <= GM_PERSONNEL)
	{
		GetDlgItem(IDC_ACCESS_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_ACCESS_EDIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PWD)->EnableWindow(FALSE);
	}
	GetDlgItem(Tab_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Tag_1"));
	GetDlgItem(Tab_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Tag_2"));
	GetDlgItem(Tab_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Tag_3"));
	
	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Btn_2"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Btn_4"));
	GetDlgItem(IDC_BUTTON_PWD)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Btn_1"));
	GetDlgItem(IDC_USER_LOGOFF)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageLogin,"Btn_3"));

	return TRUE;
}

void CDlgLogin::OnBnClickedCancel()
{
	OnCancel();
}

void CDlgLogin::OnBnClickedUserLogoff()
{
	theApp.m_SysUser.m_CurUD.level = GM_PERSONNEL;
	theApp.m_SysUser.m_CurUD.uID = 0;

	g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(FALSE);
	g_pFrm->m_wndEditBar.m_wndList.EnableWindow(FALSE);
	OnCancel();
}