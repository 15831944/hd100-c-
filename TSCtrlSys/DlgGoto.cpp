// CDlgGoto.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgGoto.h"
#include "CmdEditBar.h"
#include "MainFrm.h"
// CDlgGoto dialog

IMPLEMENT_DYNAMIC(CDlgGoto, CDialog)

CDlgGoto::CDlgGoto(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGoto::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgGoto::~CDlgGoto()
{

}

void CDlgGoto::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CDlgGoto, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgGoto::OnBnClickedOk)
	ON_BN_CLICKED(IDC_GOTO_RADIO1, &CDlgGoto::OnBnClickedGotoRadio1)
	ON_BN_CLICKED(IDC_GOTO_RADIO2, &CDlgGoto::OnBnClickedGotoRadio2)
	ON_CBN_SELCHANGE(IDC_GOTO_COMBO_LABEL, &CDlgGoto::OnCbnSelchangeGotoComboLabel)
	ON_EN_CHANGE(IDC_GOTO_EDIT_START_CMD, &CDlgGoto::OnEnChangeGotoEditStartCmd)
END_MESSAGE_MAP()

// CDlgRunEx message handlers

void CDlgGoto::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString str;
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_GOTO_COMBO_LABEL);

	m_pCmdLine->pIntValue[0] = CWnd::GetCheckedRadioButton(IDC_GOTO_RADIO1,IDC_GOTO_RADIO2)-IDC_GOTO_RADIO1;

	//tgCmdLine *p = &g_pFrm->m_Robot->m_cmdRunList[m_iStartIndex];

	if(m_pCmdLine->pIntValue[0]!=0)
	{  
		int n = pBox->GetCurSel();
		if(n==-1)
		{
			OnOK();
			return ;
		}
		((CComboBox*)GetDlgItem(IDC_GOTO_COMBO_LABEL))->GetLBText(n,str);
		strcpy_s(m_pCmdLine->pszNote, (LPSTR)(LPCSTR)str);
		m_pCmdLine->pIntValue[1] = 0;//选择标号时 地址清零
	}
	else
	{
		GetDlgItemText(IDC_GOTO_EDIT_START_CMD,str);
		m_pCmdLine->pIntValue[1] =atoi(str);
		strcpy_s(m_pCmdLine->pszNote, "");
	}

	OnOK();
}

BOOL CDlgGoto::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDGOTO,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDGOTO,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDGOTO,"Tag_2"));
	
	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDGOTO,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDGOTO,"Btn_2"));
	GetDlgItem(IDC_GOTO_RADIO1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDGOTO,"Btn_3"));
	GetDlgItem(IDC_GOTO_RADIO2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDGOTO,"Btn_4"));
	
	if(m_pCmdLine != NULL)
	{
		CString str;
		str.Format("ID：0~%d",g_pFrm->m_wndEditBar.m_wndList.GetItemCount());//行号
		SetDlgItemText(Tag_1,str);

		if(m_pCmdLine->pIntValue[1]>=g_pFrm->m_wndEditBar.m_wndList.GetItemCount())
			m_pCmdLine->pIntValue[1]=0;

		str.Format("%d",m_pCmdLine->pIntValue[1]);
		SetDlgItemText(IDC_GOTO_EDIT_START_CMD,str);

		int index=0;
		for(int i=0;i<g_pFrm->m_wndEditBar.m_wndList.GetItemCount();i++)
		{
			if(g_pFrm->m_wndEditBar.m_rawList.GetAt(g_pFrm->m_wndEditBar.m_rawList.FindIndex(i)).tType == CMD_LABEL)
			{
				((CComboBox*)GetDlgItem(IDC_GOTO_COMBO_LABEL))->InsertString(index,g_pFrm->m_wndEditBar.m_rawList.GetAt(g_pFrm->m_wndEditBar.m_rawList.FindIndex(i)).pszNote);
				index++;
			}
		}
		for(int i=0;i<((CComboBox*)GetDlgItem(IDC_GOTO_COMBO_LABEL))->GetCount();i++)
		{
			((CComboBox*)GetDlgItem(IDC_GOTO_COMBO_LABEL))->GetLBText(i,str);
			if(str == m_pCmdLine->pszNote)
				((CComboBox*)GetDlgItem(IDC_GOTO_COMBO_LABEL))->SetCurSel(i);
		}

		CWnd::CheckRadioButton(IDC_GOTO_RADIO1, IDC_GOTO_RADIO2, m_pCmdLine->pIntValue[0]==0 ? IDC_GOTO_RADIO1:IDC_GOTO_RADIO2);
		if(m_pCmdLine->pIntValue[0]==0)//地址
		{
			GetDlgItem(Tag_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_GOTO_COMBO_LABEL)->ShowWindow(SW_HIDE);
			GetDlgItem(Tag_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_GOTO_EDIT_START_CMD)->ShowWindow(SW_SHOW);
		}
		else//标号
		{
			GetDlgItem(Tag_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_GOTO_EDIT_START_CMD)->ShowWindow(SW_HIDE);
			GetDlgItem(Tag_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_GOTO_COMBO_LABEL)->ShowWindow(SW_SHOW);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGoto::OnBnClickedGotoRadio1()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine!=NULL)
	{
		m_pCmdLine->pIntValue[0]=0;
		GetDlgItem(Tag_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GOTO_COMBO_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(Tag_1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_GOTO_EDIT_START_CMD)->ShowWindow(SW_SHOW);

		CString str;
		str.Format("ID：0~%d",g_pFrm->m_wndEditBar.m_wndList.GetItemCount());//行号
		SetDlgItemText(Tag_1,str);

		if(m_pCmdLine->pIntValue[1]>=g_pFrm->m_wndEditBar.m_wndList.GetItemCount())
		{
			m_pCmdLine->pIntValue[1]=0;
		}

		str.Format("%d",m_pCmdLine->pIntValue[1]);
		SetDlgItemText(IDC_GOTO_EDIT_START_CMD,str);
	}
}

void CDlgGoto::OnBnClickedGotoRadio2()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		CString str;
		CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_GOTO_COMBO_LABEL);
		pBox->SetCurSel(0);
		int index=0;
		for(int i=0;i<g_pFrm->m_wndEditBar.m_wndList.GetItemCount();i++)
		{
			if(g_pFrm->m_wndEditBar.m_rawList.GetAt(g_pFrm->m_wndEditBar.m_rawList.FindIndex(i)).tType == CMD_LABEL)
			{
				index++;
			}
		}
		for(int i=0;i<pBox->GetCount();i++)
		{
			pBox->GetLBText(i,str);
			if(str == m_pCmdLine->pszNote)
				pBox->SetCurSel(i);
		}
		GetDlgItem(Tag_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GOTO_EDIT_START_CMD)->ShowWindow(SW_HIDE);
		GetDlgItem(Tag_2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_GOTO_COMBO_LABEL)->ShowWindow(SW_SHOW);

	}
}

void CDlgGoto::OnCbnSelchangeGotoComboLabel()
{
	// TODO: Add your control notification handler code here
}

void CDlgGoto::OnEnChangeGotoEditStartCmd()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
