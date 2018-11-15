// DlgJogUnFeed.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgJogUnFeed.h"


// CDlgJogUnFeed dialog

IMPLEMENT_DYNAMIC(CDlgJogUnFeed, CDialog)

CDlgJogUnFeed::CDlgJogUnFeed(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgJogUnFeed::IDD, pParent)
{

}

CDlgJogUnFeed::~CDlgJogUnFeed()
{
}

void CDlgJogUnFeed::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgJogUnFeed, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgJogUnFeed::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgJogUnFeed message handlers

void CDlgJogUnFeed::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CDlgJogUnFeed::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_MNBtn.m_iIndex = 14;
	m_MNBtn.SubclassDlgItem(IDC_SET_JOGXYZ_MN, this);
	m_MNBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	m_MPBtn.m_iIndex = 15;
	m_MPBtn.SubclassDlgItem(IDC_SET_JOGXYZ_MP, this);
	m_MPBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	m_NNBtn.m_iIndex = 13;
	m_NNBtn.SubclassDlgItem(IDC_SET_JOGXYZ_NN, this);
	m_NNBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	m_NPBtn.m_iIndex = 12;
	m_NPBtn.SubclassDlgItem(IDC_SET_JOGXYZ_NP, this);
	m_NPBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED1))->SetRange(0,200);
	((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED1))->SetPos(50);
	int n = ((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED1))->GetPos();

	CJogButton::m_dJogSpdRatio = n/200.0;

	CString strTemp;
	strTemp.Format("M:%.2f   N:%.2f", g_pFrm->m_pSysParam->dSpeedMaxJogVW * (n/200.0),g_pFrm->m_pSysParam->dSpeedMaxJogVW * (n/200.0));
	SetDlgItemText(IDC_SET_JOGXYZ_EDIT_SPEED1, strTemp);

	CString str;
	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[LOAD_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITM,str);

	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[UNLOAD_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITN,str);

	SetTimer(5,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgJogUnFeed::OnTimer(UINT_PTR nIDEvent)
{
	CString str;
	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[LOAD_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITM,str);
	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[UNLOAD_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITN,str);

	CDialog::OnTimer(nIDEvent);
}

void CDlgJogUnFeed::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if(pScrollBar-> GetDlgCtrlID() != IDC_SET_JOGXYZ_SLIDER_SPEED1) return;

	if(nSBCode == SB_PAGEDOWN) 
	{
		((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED1))->SetPos((int)(CJogButton::m_dJogSpdRatio*200));
		return;
	}
	int n = ((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED1))->GetPos();

	CJogButton::m_dJogSpdRatio = n/200.0;

	double m_dSpdRatio = n/200.0;

	if(n==0) 
	{
		m_dSpdRatio = 1/2000.0;
		CJogButton::m_dJogSpdRatio = 1/2000.0;
	}

	CString strTemp;
	strTemp.Format("M:%.2f   N:%.2f", g_pFrm->m_pSysParam->dSpeedMaxJogMN * m_dSpdRatio,g_pFrm->m_pSysParam->dSpeedMaxJogMN * m_dSpdRatio);
	SetDlgItemText(IDC_SET_JOGXYZ_EDIT_SPEED1, strTemp);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CDlgJogUnFeed::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID()==IDC_SET_JOGXYZ_EDIT_SPEED1)
		pDC->SetTextColor(RGB(0,0,255));  //ÉèÖÃ×ÖÌåÑÕÉ«

	return hbr;
}
void CDlgJogUnFeed::OnStnClickedSetJogxyzEditSpeed()
{
	// TODO: Add your control notification handler code here
}
