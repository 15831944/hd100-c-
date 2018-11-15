// DlgJogFeed.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgJogFeed.h"


// CDlgJogFeed dialog

IMPLEMENT_DYNAMIC(CDlgJogFeed, CDialog)

CDlgJogFeed::CDlgJogFeed(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgJogFeed::IDD, pParent)
{

}

CDlgJogFeed::~CDlgJogFeed()
{
}

void CDlgJogFeed::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgJogFeed, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgJogFeed::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDlgJogFeed message handlers

void CDlgJogFeed::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgJogFeed::OnTimer(UINT_PTR nIDEvent)
{
	CString str;
	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[T1_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITV,str);
	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[W1_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITW,str);
	
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CDlgJogFeed::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID()==IDC_SET_JOGXYZ_EDIT_SPEED)
		pDC->SetTextColor(RGB(0,0,255));  //ÉèÖÃ×ÖÌåÑÕÉ«

	return hbr;
}

void CDlgJogFeed::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(pScrollBar-> GetDlgCtrlID() != IDC_SET_JOGXYZ_SLIDER_SPEED) return;

	if(nSBCode == SB_PAGEDOWN) 
	{
		((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED))->SetPos((int)(CJogButton::m_dJogSpdRatio*200));
		return;
	}
	int n = ((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED))->GetPos();

	CJogButton::m_dJogSpdRatio = n/200.0;

	double m_dSpdRatio = n/200.0;

	if(n==0) 
	{
		m_dSpdRatio = 1/2000.0;
		CJogButton::m_dJogSpdRatio = 1/2000.0;
	}

	CString strTemp;
	strTemp.Format("V:%.2f   W:%.2f", g_pFrm->m_pSysParam->dSpeedMaxJogVW * m_dSpdRatio,g_pFrm->m_pSysParam->dSpeedMaxJogVW * m_dSpdRatio);
	SetDlgItemText(IDC_SET_JOGXYZ_EDIT_SPEED, strTemp);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDlgJogFeed::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_VNBtn.m_iIndex = 10;
	m_VNBtn.SubclassDlgItem(IDC_SET_JOGXYZ_VN, this);
	m_VNBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	m_VPBtn.m_iIndex = 11;
	m_VPBtn.SubclassDlgItem(IDC_SET_JOGXYZ_VP, this);
	m_VPBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	m_WNBtn.m_iIndex = 9;
	m_WNBtn.SubclassDlgItem(IDC_SET_JOGXYZ_WN, this);
	m_WNBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	m_WPBtn.m_iIndex = 8;
	m_WPBtn.SubclassDlgItem(IDC_SET_JOGXYZ_WP, this);
	m_WPBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED))->SetRange(0,200);
	((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED))->SetPos(50);
	int n = ((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED))->GetPos();

	CJogButton::m_dJogSpdRatio = n/200.0;

	CString strTemp;
	strTemp.Format("M:%.2f   N:%.2f", g_pFrm->m_pSysParam->dSpeedMaxJogU * (n/200.0),g_pFrm->m_pSysParam->dSpeedMaxJogVW * (n/200.0));
	SetDlgItemText(IDC_SET_JOGXYZ_EDIT_SPEED, strTemp);

	CString str;
	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[T1_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITV,str);

	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[W1_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITW,str);

	SetTimer(5,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
