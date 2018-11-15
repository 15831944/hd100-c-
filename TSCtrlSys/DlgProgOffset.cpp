// DlgProgOffset.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgProgOffset.h"


// CDlgProgOffset dialog
   
IMPLEMENT_DYNAMIC(CDlgProgOffset, CDialog)

CDlgProgOffset::CDlgProgOffset(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgOffset::IDD, pParent)
{
	m_pCmdLine = NULL;
	m_iStartIndex = 0;
	m_iEndIndex = 0;
}

CDlgProgOffset::~CDlgProgOffset()
{

}

void CDlgProgOffset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgProgOffset, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgProgOffset::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgProgOffset::OnBnClickedBtnApply)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDCANCEL, &CDlgProgOffset::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgProgOffset message handlers

void CDlgProgOffset::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnBnClickedBtnApply();
	OnOK();
}

void CDlgProgOffset::OnBnClickedBtnApply()
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	tgPos tTempOffset;
	GetDlgItemText(IDC_PROG_EDIT_X_MDI_OFFSET, strTemp);
	tTempOffset.x = atof(strTemp);
	GetDlgItemText(IDC_PROG_EDIT_Y_MDI_OFFSET, strTemp);
	tTempOffset.y = atof(strTemp);
	GetDlgItemText(IDC_PROG_EDIT_Z_MDI_OFFSET, strTemp);
	tTempOffset.z = atof(strTemp);

	GetDlgItemText(IDC_PROG_EDIT_STARTINDEX, strTemp);
	m_iStartIndex = atoi(strTemp);
	GetDlgItemText(IDC_PROG_EDIT_ENDINDEX, strTemp);
	m_iEndIndex = atoi(strTemp);

	//这里将偏移更新到程序列表中
	if(g_pFrm->m_Robot->RunListOffsetChange(m_iStartIndex,m_iEndIndex,tTempOffset.x,tTempOffset.y,tTempOffset.z))//更新成功
	{
		g_pFrm->m_wndEditBar.UpdateCmdListBox(g_pFrm->m_wndEditBar.m_wndList.GetItemCount()-1);
	}

	////////////////////////////////
	g_pView->Invalidate();

	tTempOffset.x = 0.0;
	tTempOffset.y = 0.0;
	tTempOffset.z = 0.0;
	
	strTemp.Format("%.3f", tTempOffset.x);
	SetDlgItemText(IDC_PROG_EDIT_X_MDI_OFFSET, strTemp);
	strTemp.Format("%.3f", tTempOffset.y);
	SetDlgItemText(IDC_PROG_EDIT_Y_MDI_OFFSET, strTemp);
	strTemp.Format("%.3f", tTempOffset.z);
	SetDlgItemText(IDC_PROG_EDIT_Z_MDI_OFFSET, strTemp);
}

BOOL CDlgProgOffset::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgOffset,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgOffset,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgOffset,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgOffset,"Tag_3"));

	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgOffset,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgOffset,"Btn_2"));
	GetDlgItem(IDC_BTN_APPLY)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgOffset,"Btn_3"));

	CString strTemp;

	strTemp.Format("%.3f", 0.0);
	SetDlgItemText(IDC_PROG_EDIT_X_MDI_OFFSET, strTemp);
	strTemp.Format("%.3f", 0.0);
	SetDlgItemText(IDC_PROG_EDIT_Y_MDI_OFFSET, strTemp);
	strTemp.Format("%.3f", 0.0);
	SetDlgItemText(IDC_PROG_EDIT_Z_MDI_OFFSET, strTemp);

	strTemp.Format("%d", m_iStartIndex);
	SetDlgItemText(IDC_PROG_EDIT_STARTINDEX, strTemp);

	if (g_pFrm->m_wndEditBar.m_rawList.GetCount()>0)
		strTemp.Format("%d", g_pFrm->m_wndEditBar.m_rawList.GetCount()-1);
	else
		strTemp.Format("%d", 0);
	SetDlgItemText(IDC_PROG_EDIT_ENDINDEX, strTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProgOffset::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::OnOK();
}

void CDlgProgOffset::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
	if(nState==WA_ACTIVE)
	{
		CString strTemp;
		strTemp.Format("%d", m_iStartIndex);
		SetDlgItemText(IDC_PROG_EDIT_STARTINDEX, strTemp);

		if (g_pFrm->m_wndEditBar.m_rawList.GetCount()>0)
			strTemp.Format("%d", g_pFrm->m_wndEditBar.m_rawList.GetCount()-1);
		else
			strTemp.Format("%d", 0);
		SetDlgItemText(IDC_PROG_EDIT_ENDINDEX, strTemp);
		////
		strTemp.Format("%.3f", 0.0);
		SetDlgItemText(IDC_PROG_EDIT_X_MDI_OFFSET, strTemp);
		strTemp.Format("%.3f", 0.0);
		SetDlgItemText(IDC_PROG_EDIT_Y_MDI_OFFSET, strTemp);
		strTemp.Format("%.3f", 0.0);
		SetDlgItemText(IDC_PROG_EDIT_Z_MDI_OFFSET, strTemp);
	}	
}
void CDlgProgOffset::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
