// PageImg.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "PageImg.h"


// CPageImg dialog

IMPLEMENT_DYNAMIC(CPageImg, CPropertyPage)

CPageImg::CPageImg()
	: CPropertyPage(CPageImg::IDD)
{

}

CPageImg::~CPageImg()
{
}

void CPageImg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageImg, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CPageImg::OnBnClickedBtnApply)
	ON_EN_CHANGE(IDC_PAGE_IMG_R1, &CPageImg::OnEnChangePageImgR1)
	ON_EN_CHANGE(IDC_PAGE_IMG_R2, &CPageImg::OnEnChangePageImgR2)
	ON_BN_CLICKED(IDC_PAGE_IMG_CHECK_XDIR, &CPageImg::OnBnClickedPageImgCheckXdir)
	ON_BN_CLICKED(IDC_PAGE_IMG_CHECK_YDIR, &CPageImg::OnBnClickedPageImgCheckYdir)
	ON_CBN_SELCHANGE(IDC_COMBO_MARKNUM, &CPageImg::OnCbnSelchangeComboMarknum)
END_MESSAGE_MAP()


// CPageImg message handlers

void CPageImg::OnBnClickedBtnApply()
{
	// TODO: Add your control notification handler code here
}

BOOL CPageImg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strTemp;

	strTemp.Format("%.3f", g_pFrm->m_pSysParam->dDrawCircleR1);
	SetDlgItemText(IDC_PAGE_IMG_R1, strTemp);
	strTemp.Format("%d", g_pFrm->m_pSysParam->dDrawCircleR2);
	SetDlgItemText(IDC_PAGE_IMG_R2, strTemp);


	if(theApp.m_SysUser.m_CurUD.level>GM_ADMIN)
	{
		GetDlgItem(IDC_PAGE_IMG_CHECK_XDIR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PAGE_IMG_CHECK_YDIR)->ShowWindow(SW_SHOW);

		((CButton*)GetDlgItem(IDC_PAGE_IMG_CHECK_XDIR))->SetCheck(g_pFrm->m_pSysParam->iXDir);
		((CButton*)GetDlgItem(IDC_PAGE_IMG_CHECK_YDIR))->SetCheck(g_pFrm->m_pSysParam->iYDir);
	}
    
	CString str= g_pFrm->LanguageMgr->ReadStringFromIni(DefPageOther,"Tag_1");
	GetDlgItem(Tag_1222)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageOther,"Tag_1"));
	GetDlgItem(Tag_1223)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageOther,"Tag_2"));
	GetDlgItem(IDC_PAGE_IMG_CHECK_XDIR)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageOther,"Btn_1"));
	GetDlgItem(IDC_PAGE_IMG_CHECK_YDIR)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageOther,"Btn_2"));
   
	// Markµã¸öÊý 
	CComboBox *pBox =(CComboBox*)GetDlgItem(IDC_COMBO_MARKNUM);
	pBox->AddString("1");
	pBox->AddString("2");
	pBox->AddString("3");
    int n =  g_pFrm->m_pSysParam->iMarkNum -1;
	pBox->SetCurSel(n);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CPageImg::OnApply()
{
	// TODO: Add your specialized code here and/or call the base class
	CString strTemp;

	GetDlgItemText(IDC_PAGE_IMG_R1, strTemp);
	g_pFrm->m_pSysParam->dDrawCircleR1 = atof(strTemp);
	GetDlgItemText(IDC_PAGE_IMG_R2, strTemp);
	g_pFrm->m_pSysParam->dDrawCircleR2 = atoi(strTemp);

	if(theApp.m_SysUser.m_CurUD.level>GM_ADMIN)
	{
		g_pFrm->m_pSysParam->iXDir = ((CButton*)GetDlgItem(IDC_PAGE_IMG_CHECK_XDIR))->GetCheck();
		g_pFrm->m_pSysParam->iYDir = ((CButton*)GetDlgItem(IDC_PAGE_IMG_CHECK_YDIR))->GetCheck();
	}

	CComboBox *pBox =(CComboBox*)GetDlgItem(IDC_COMBO_MARKNUM);
	
	int n=pBox->GetCurSel();
	if (n<0)   n=0; 

    g_pFrm->m_pSysParam->iMarkNum = n+1;

	g_pView->Invalidate();
	return CPropertyPage::OnApply();
}

void CPageImg::OnEnChangePageImgR1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	SetModified();
}

void CPageImg::OnEnChangePageImgR2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	SetModified();
}

void CPageImg::OnBnClickedPageImgCheckXdir()
{
	SetModified();
}

void CPageImg::OnBnClickedPageImgCheckYdir()
{
	SetModified();
}

void CPageImg::OnCbnSelchangeComboMarknum()
{
	SetModified();
}
