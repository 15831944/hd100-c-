// DlgShotCut.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
//#include "CmdEditBar.h"
#include "DlgPathEdit.h"
#include "DlgShotCut.h"

// CDlgShotCut dialog

IMPLEMENT_DYNAMIC(CDlgShotCut, CDialog)

CDlgShotCut::CDlgShotCut(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShotCut::IDD, pParent)
	, m_fJogStep(0)
{
}

CDlgShotCut::~CDlgShotCut()
{
}

void CDlgShotCut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_JOG_EDIT_STEP_LENGTH, m_fJogStep);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_scrollBar);
}
BEGIN_MESSAGE_MAP(CDlgShotCut, CDialog)
	ON_BN_CLICKED(IDC_JOG_BUTTON_EXT, &CDlgShotCut::OnBnClickedJogExt)
	ON_BN_CLICKED(IDC_JOG_CHECK_STEP, &CDlgShotCut::OnBnClickedJogCheckStep)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_JOG_EDIT_STEP_LENGTH, &CDlgShotCut::OnEnChangeJogEditStepLength)
	ON_BN_CLICKED(IDC_TESTDISP, &CDlgShotCut::OnBnClickedTestdisp)
	ON_BN_CLICKED(IDC_CHECK_CONVeyor, &CDlgShotCut::OnClickedCheckConveyor)
END_MESSAGE_MAP()

// CDlgShotCut message handlers
BOOL CDlgShotCut::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(int i=0; i<8; i++)
	{//JOG手动图标
		m_JogButton[i].m_iIndex = i;
		m_JogButton[i].SubclassDlgItem(IDC_JOG_BUTTON_MV10+i, this);
		m_JogButton[i].SetBitmapEx(IDB_BITMAP_MOVE);
	}

// 	((CSliderCtrl*)GetDlgItem(IDC_SHOTCUT_JOG_SLIDER_SPEED))->SetRange(0,200);
// 	((CSliderCtrl*)GetDlgItem(IDC_SHOTCUT_JOG_SLIDER_SPEED))->SetPos(50);
// 	int n = ((CSliderCtrl*)GetDlgItem(IDC_SHOTCUT_JOG_SLIDER_SPEED))->GetPos();
// 	CJogButton::m_dJogSpdRatio = n/200.0;

	m_scrollBar.SetScrollRange(0, 200);
	m_scrollBar.SetScrollPos(50);
	int n = m_scrollBar.GetScrollPos();
	CJogButton::m_dJogSpdRatio = n/200.0;

	CString strTemp;
	strTemp.Format("%.1f", (g_pFrm->m_pSysParam->dSpeedMaxJogXY * (n/200.0)));
	SetDlgItemText(IDC_SHOTCUT_JOG_XY_SPEED, strTemp);
	strTemp.Format("%.1f", g_pFrm->m_pSysParam->dSpeedMaxJogZ * (n/200.0));
	SetDlgItemText(IDC_SHOTCUT_JOG_Z_SPEED, strTemp);
	//////////////////////////////////

	if (g_pFrm->m_Robot->m_pController->g_ExMtnPara->axisNum>4)
	{
		GetDlgItem(IDC_JOG_BUTTON_V)->ShowWindow(SW_SHOW);
	}

	GetDlgItem(S_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefShotCut,_T("Tag_1"))); 
	GetDlgItem(IDC_JOG_CHECK_STEP)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefShotCut,_T("Btn_1")));
	//GetDlgItem(IDC_TESTDISP)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefShotCut,_T("Btn_2")));

	return TRUE;  
}

void CDlgShotCut::OnBnClickedJogExt()
{
	CString str;
	if(theApp.m_SysUser.m_CurUD.level<=GM_PERSONNEL)
	{
	    str = g_pFrm->LanguageMgr->ReadStringFromIni(DefShotCut,_T("Msg_1"));
		AfxMessageBox(str);
		return;
	}

	CMenu menu;
	menu.LoadMenu(IDR_MENU_NEW);
// 	if(g_pFrm->m_pSysParam->nLanguageType == 0)
// 	{// 中文
// 	   menu.LoadMenu(IDR_MENU_CMD_CN);
// 	}
// 	if(g_pFrm->m_pSysParam->nLanguageType == 1)
// 	{// 英文
// 		menu.LoadMenu(IDR_MENU_CMD_EN);
// 	}
	CPoint point;
	GetCursorPos(&point);
	CDlgPathEdit *p = (CDlgPathEdit*)GetParent();
	p->m_bInsertCmd = FALSE;

	//if((!g_pView->m_ImgStatic.m_bImgInit))
	//	menu.EnableMenuItem(ID_CMD_MARK,MF_DISABLED|MF_GRAYED);
    
// 	if(0==g_pFrm->m_pSysParam->bBalanceEnable)
// 	{
// 		menu.EnableMenuItem(IDC_BUTTON_EDIT6,MF_DISABLED|MF_GRAYED);
// 		menu.EnableMenuItem(IDC_BUTTON_EDIT7,MF_DISABLED|MF_GRAYED);
// 		menu.EnableMenuItem(IDC_BUTTON_EDIT8,MF_DISABLED|MF_GRAYED);
// 	}

	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_BOTTOMALIGN, point.x, point.y-10, p);
}
void CDlgShotCut::OnBnClickedJogCheckStep()
{
	// TODO: Add your control notification handler code here
	CButton *pBnt = (CButton*)GetDlgItem(IDC_JOG_CHECK_STEP);
	CJogButton::s_bEnableStep = static_cast<BOOL>(pBnt->GetCheck());
	if(TRUE == CJogButton::s_bEnableStep )
	{
		GetDlgItem(IDC_JOG_EDIT_STEP_LENGTH)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_JOG_EDIT_STEP_LENGTH)->EnableWindow(FALSE);
	}
}

void CDlgShotCut::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if(pScrollBar-> GetDlgCtrlID() != IDC_SCROLLBAR1) return;
	int iTmpPos = m_scrollBar.GetScrollPos();
	CString strTemp;

	switch (nSBCode)
	{
	case SB_THUMBPOSITION:
		iTmpPos = nPos;
		m_scrollBar.SetScrollPos(iTmpPos);
		break;
	case SB_LINERIGHT:
		iTmpPos = m_scrollBar.GetScrollPos();
		if (iTmpPos < 200)	
			iTmpPos++;
		m_scrollBar.SetScrollPos(iTmpPos);
		break;
	case SB_LINELEFT:
		iTmpPos = m_scrollBar.GetScrollPos();
		if (iTmpPos > 0)	
			iTmpPos--;
		m_scrollBar.SetScrollPos(iTmpPos);
		break;
	case SB_PAGELEFT:
		iTmpPos = m_scrollBar.GetScrollPos();
		if (iTmpPos > 10)
			iTmpPos -= 10;
		m_scrollBar.SetScrollPos(iTmpPos);
		break;
	case SB_PAGERIGHT:
		iTmpPos = m_scrollBar.GetScrollPos();
		if (iTmpPos < 190)
			iTmpPos += 10;
		m_scrollBar.SetScrollPos(iTmpPos);
		break;
	}

// 	if(pScrollBar-> GetDlgCtrlID() != IDC_SHOTCUT_JOG_SLIDER_SPEED) return;
// 	if(nSBCode == SB_PAGEDOWN) 
// 	{
// 		((CSliderCtrl*)GetDlgItem(IDC_SHOTCUT_JOG_SLIDER_SPEED))->SetPos((int)(CJogButton::m_dJogSpdRatio*200));
// 		return;
// 	}
// 	int n = ((CSliderCtrl*)GetDlgItem(IDC_SHOTCUT_JOG_SLIDER_SPEED))->GetPos();

	CJogButton::m_dJogSpdRatio = iTmpPos/200.0;
	double m_dSpdRatio = iTmpPos/200.0;
	if(iTmpPos==0) 
	{
		m_dSpdRatio = 1/2000.0;
		CJogButton::m_dJogSpdRatio = 1/2000.0;
	}

	strTemp.Format("%.1f", (g_pFrm->m_pSysParam->dSpeedMaxJogXY * m_dSpdRatio));
	SetDlgItemText(IDC_SHOTCUT_JOG_XY_SPEED, strTemp);
	strTemp.Format("%.1f", g_pFrm->m_pSysParam->dSpeedMaxJogZ * m_dSpdRatio);
	SetDlgItemText(IDC_SHOTCUT_JOG_Z_SPEED, strTemp);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgShotCut::OnEnChangeJogEditStepLength()
{
	CString strText;
    CString strMsg;
	GetDlgItemText(IDC_JOG_EDIT_STEP_LENGTH, strText);
	if( true == CFunction::IsNumber(strText) )
	{
		UpdateData(TRUE);
		CJogButton::s_fStepDist = m_fJogStep;
	}
	else
	{
		UpdateData(FALSE);
		strMsg = g_pFrm->LanguageMgr->ReadStringFromIni(DefShotCut,_T("Msg_2"));
		AfxMessageBox(strMsg);
	}
}

void CDlgShotCut::OnOK()
{
	return;
}

void CDlgShotCut::OnBnClickedTestdisp()
{
	if(g_pFrm->m_pSysParam->nToolType == 1)
	{ 
		// 针筒控制
        g_pFrm->m_Robot->NeedleTestDisp();   // 试胶动作

   	}else if(g_pFrm->m_pSysParam->nToolType == 0)
	{
		// 喷射动作
	  g_pFrm->m_Robot->TestDisp();   // 试胶动作 
	}
}



void CDlgShotCut::OnClickedCheckConveyor()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton *pBnt = (CButton*)GetDlgItem(IDC_CHECK_CONVeyor);
	CJogButton::m_bEnableTrack = static_cast<BOOL>(pBnt->GetCheck());
// 	if(TRUE == CJogButton::m_bEnableTrack )
// 	{
// 		GetDlgItem(IDC_JOG_BUTTON_MV10)->SetWindowText(_T("T-"));
// 		GetDlgItem(IDC_JOG_BUTTON_MV11)->SetWindowText(_T("T+"));
// 		GetDlgItem(IDC_JOG_BUTTON_MV12)->SetWindowText(_T("W+"));
// 		GetDlgItem(IDC_JOG_BUTTON_MV13)->SetWindowText(_T("W-"));
// 		GetDlgItem(IDC_JOG_BUTTON_MV14)->SetWindowText(_T("R-"));
// 		GetDlgItem(IDC_JOG_BUTTON_MV15)->SetWindowText(_T("R+"));
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_JOG_BUTTON_MV10)->SetWindowText(_T("X-"));
// 		GetDlgItem(IDC_JOG_BUTTON_MV11)->SetWindowText(_T("X+"));
// 		GetDlgItem(IDC_JOG_BUTTON_MV12)->SetWindowText(_T("Y-"));
// 		GetDlgItem(IDC_JOG_BUTTON_MV13)->SetWindowText(_T("Y+"));
// 		GetDlgItem(IDC_JOG_BUTTON_MV14)->SetWindowText(_T("Z-"));
// 		GetDlgItem(IDC_JOG_BUTTON_MV15)->SetWindowText(_T("Z+"));
// 	}
}
