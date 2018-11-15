// DlgJogXYZ.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgJogXYZ.h"



// CDlgJogXYZ dialog

IMPLEMENT_DYNAMIC(CDlgJogXYZ, CDialog)

CDlgJogXYZ::CDlgJogXYZ(const CString& str,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgJogXYZ::IDD, pParent)
	, m_fJogStep(0)
{
	m_strCaption = str;

}

CDlgJogXYZ::~CDlgJogXYZ()
{
	
}

void CDlgJogXYZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SET_JOGXYZ_EDIT_STEP, m_fJogStep);
}


BEGIN_MESSAGE_MAP(CDlgJogXYZ, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgJogXYZ::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgJogXYZ::OnBnClickedCancel)
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SET_JOGXYZ_CHECK_STEP, &CDlgJogXYZ::OnBnClickedSetJogxyzCheckStep)
	ON_EN_CHANGE(IDC_SET_JOGXYZ_EDIT_STEP, &CDlgJogXYZ::OnEnChangeSetJogxyzEditStep)
END_MESSAGE_MAP()


// CDlgJogXYZ message handlers

BOOL CDlgJogXYZ::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_XNBtn.m_iIndex = 0;
	m_XNBtn.SubclassDlgItem(IDC_SET_JOGXYZ_XN, this);
	m_XNBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	m_XPBtn.m_iIndex = 1;
	m_XPBtn.SubclassDlgItem(IDC_SET_JOGXYZ_XP, this);
	m_XPBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	m_YNBtn.m_iIndex = 2;
	m_YNBtn.SubclassDlgItem(IDC_SET_JOGXYZ_YN, this);
	m_YNBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	m_YPBtn.m_iIndex = 3;
	m_YPBtn.SubclassDlgItem(IDC_SET_JOGXYZ_YP, this);
	m_YPBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	m_ZNBtn.m_iIndex = 4;
	m_ZNBtn.SubclassDlgItem(IDC_SET_JOGXYZ_ZN, this);
	m_ZNBtn.SetBitmapEx(IDB_BITMAP_MOVE);

	m_ZPBtn.m_iIndex = 5;
	m_ZPBtn.SubclassDlgItem(IDC_SET_JOGXYZ_ZP, this);
	m_ZPBtn.SetBitmapEx(IDB_BITMAP_MOVE);


	((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED))->SetRange(0,200);
	((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED))->SetPos(1);
	int n = ((CSliderCtrl*)GetDlgItem(IDC_SET_JOGXYZ_SLIDER_SPEED))->GetPos();

	CJogButton::m_dJogSpdRatio = n/200.0;

	double m_dSpdRatio = n/200.0;

	if(n==0) 
	{
		m_dSpdRatio = 1/2000.0;
		CJogButton::m_dJogSpdRatio = 1/2000.0;
	}

	CString strTemp;
	strTemp.Format("XY:%.2f,Z:%.2f", g_pFrm->m_pSysParam->dSpeedMaxJogXY * m_dSpdRatio,g_pFrm->m_pSysParam->dSpeedMaxJogZ * m_dSpdRatio);
	SetDlgItemText(IDC_SET_JOGXYZ_EDIT_SPEED, strTemp);

	SetWindowText(m_strCaption);

	CString str;
	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITX,str);
	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITY,str);
	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITZ,str);

	SetTimer(3,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgJogXYZ::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgJogXYZ::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDlgJogXYZ::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
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
	strTemp.Format("XY:%.2f,Z:%.2f", g_pFrm->m_pSysParam->dSpeedMaxJogXY * m_dSpdRatio,g_pFrm->m_pSysParam->dSpeedMaxJogZ * m_dSpdRatio);
	SetDlgItemText(IDC_SET_JOGXYZ_EDIT_SPEED, strTemp);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CDlgJogXYZ::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID()==IDC_SET_JOGXYZ_EDIT_SPEED)
		pDC->SetTextColor(RGB(0,0,255));  //设置字体颜色

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CDlgJogXYZ::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITX,str);
	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITY,str);
	str.Format(_T("%.3f"),g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS]);
	SetDlgItemText(IDC_SET_JOGXYZ_EDITZ,str);


	CDialog::OnTimer(nIDEvent);
}

void CDlgJogXYZ::OnBnClickedSetJogxyzCheckStep()
{
	// TODO: Add your control notification handler code here
	CButton *pBnt = (CButton*)GetDlgItem(IDC_SET_JOGXYZ_CHECK_STEP);
	CJogButton::s_bEnableStep = static_cast<BOOL>(pBnt->GetCheck());
	if(TRUE == CJogButton::s_bEnableStep )
	{
		GetDlgItem(IDC_SET_JOGXYZ_EDIT_STEP)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_SET_JOGXYZ_EDIT_STEP)->EnableWindow(FALSE);
	}
}

void CDlgJogXYZ::OnEnChangeSetJogxyzEditStep()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strText;

	GetDlgItemText(IDC_SET_JOGXYZ_EDIT_STEP, strText);
	if( true == CFunction::IsNumber(strText) )
	{
		UpdateData(TRUE);
		CJogButton::s_fStepDist = m_fJogStep;
	}
	else
	{
		UpdateData(FALSE);
		AfxMessageBox(_T("只能输入数字！"));
	}
}
