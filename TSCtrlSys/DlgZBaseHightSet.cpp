// DlgZBaseHightSet.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgZBaseHightSet.h"


// CDlgZBaseHightSet dialog

IMPLEMENT_DYNAMIC(CDlgZBaseHightSet, CDialog)

CDlgZBaseHightSet::CDlgZBaseHightSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgZBaseHightSet::IDD, pParent)
{

}

CDlgZBaseHightSet::~CDlgZBaseHightSet()
{
}

void CDlgZBaseHightSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgZBaseHightSet, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgZBaseHightSet::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CDlgZBaseHightSet::OnBnClickedOk2)
	ON_BN_CLICKED(IDC_DLG_BASESET_SETBASE, &CDlgZBaseHightSet::OnBnClickedDlgBasesetSetbase)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_BASE_JETHIGHT, &CDlgZBaseHightSet::OnEnChangeBaseJethight)
END_MESSAGE_MAP()


// CDlgZBaseHightSet message handlers

void CDlgZBaseHightSet::OnBnClickedOk()
{
    g_pFrm->SysParam(FALSE);
	OnOK();
}

void CDlgZBaseHightSet::OnBnClickedOk2()
{
	OnCancel();
}
// 设置当前Z轴位置为基准高度面
void CDlgZBaseHightSet::OnBnClickedDlgBasesetSetbase()
{
  	CString strTemp;  
	
	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();

    g_pFrm->m_pSysParam->dJetZBaseHightValue = 
		g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dJetZBaseHightValue);
	SetDlgItemText(IDC_BASE_BASEDATE, strTemp);

}

BOOL CDlgZBaseHightSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString strTemp;

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dJetZBaseHightValue);
	SetDlgItemText(IDC_BASE_BASEDATE, strTemp);

	// SetTimer(5,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgZBaseHightSet::OnTimer(UINT_PTR nIDEvent)
{
// 	CString strTemp; 
// 
//     // 基准高度
// 	double database = g_pFrm->m_pSysParam->dJetZBaseHightValue;
// 	
// 	// 当前Z轴高度
// 	g_pFrm->m_Robot->m_pController->GetCurPosMM();
// 	double curpos = 
// 		g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];
//      
// 	double dis = database - curpos ;   // 当前位置与标准界面高度差
//    
// 	strTemp.Format(_T("%.3f"),dis);
// 	SetDlgItemText(IDC_BASE_JETHIGHT,strTemp);
//      

	CDialog::OnTimer(nIDEvent);
}

void CDlgZBaseHightSet::OnEnChangeBaseJethight()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
