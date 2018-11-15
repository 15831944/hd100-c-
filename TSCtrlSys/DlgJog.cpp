// DlgJog.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgJog.h"
#pragma warning(disable: 4800 4244)

// CDlgJog dialog

IMPLEMENT_DYNAMIC(CDlgJog, CDialog)

CDlgJog::CDlgJog(CWnd* pParent/*=NULL*/)
	: CDialog(CDlgJog::IDD, pParent)
	, m_fJogStep(0)
	, m_fXYSpeed(0)
	, m_fZSpeed(0)
{
	m_dSpdRatio = 1;
}

CDlgJog::~CDlgJog()
{
}

void CDlgJog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_JOG_BTN_DISP_TRIPPER, m_chkTripperCheckbox);
	DDX_Control(pDX, IDC_JOG_BTN_DISP_WASH, m_chkRotateCheckbox);
	DDX_Control(pDX, IDC_JOG_BTN_DISP_QIYA, m_chkUpdownCheckbox);

	DDX_Text(pDX, IDC_JOG_EDIT_STEP_LENGTH, m_fJogStep);
	DDX_Text(pDX, IDC_SPEED_Z, m_fZSpeed);
	DDX_Text(pDX, IDC_XY_SPEED, m_fXYSpeed);
	DDV_MinMaxFloat(pDX, m_fXYSpeed, 0, 200);
}

BEGIN_MESSAGE_MAP(CDlgJog, CDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_JOG_BTN_DISP_WASH, &CDlgJog::OnBnClickedJogBtnDispWash)
	ON_BN_CLICKED(IDC_JOG_BTN_DISP_QIYA, &CDlgJog::OnBnClickedJogBtnDispQiya)
	ON_CBN_SELCHANGE(IDC_JOG_OPER_RADIO_HEAD, &CDlgJog::OnCbnSelchangeJogOperRadioHead)
	ON_BN_CLICKED(IDC_JOG_BTN_DISP_TRIPPER, &CDlgJog::OnBnClickedJogBtnDispTripper)
	ON_WM_ACTIVATE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUT_ZERO, &CDlgJog::OnBnClickedButZero)
	ON_BN_CLICKED(IDC_JOG_CHECK_STEP, &CDlgJog::OnBnJogCheckStep)
	ON_EN_CHANGE(IDC_JOG_EDIT_STEP_LENGTH, &CDlgJog::OnEnChangeJogEditStepLength)
	ON_EN_CHANGE(IDC_XY_SPEED, &CDlgJog::OnEnChangeXYSpeed)
END_MESSAGE_MAP()


// CDlgJog message handlers

BOOL CDlgJog::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	m_chkTripperCheckbox.SetIcon(IDI_LED_ON,IDI_LED_OFF);
	m_chkTripperCheckbox.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	m_chkRotateCheckbox.SetIcon(IDI_LED_ON,IDI_LED_OFF);
	m_chkRotateCheckbox.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	
	m_chkUpdownCheckbox.SetIcon(IDI_LED_ON,IDI_LED_OFF);
	m_chkUpdownCheckbox.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);


	GetDlgItem(IDC_JOG_EDIT_STEP_LENGTH)->EnableWindow(FALSE);

	CFont m_Font; 
	m_Font.CreateFont(20, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("Arail")); // lpszFac 微软雅黑

 
	GetDlgItem(IDC_XY_SPEED)->SetFont(&m_Font);
	GetDlgItem(IDC_SPEED_Z)->SetFont(&m_Font);
	m_Font.Detach();	

	CString str;
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_JOG_OPER_RADIO_HEAD);
	for (int j=0;j<TOOL_NUM;j++)
	{
		str.Empty();
		str.Format("#%d",j);
		pBox->AddString(str);
	}
	pBox->SetCurSel(0);
	
	OnCbnSelchangeJogOperRadioHead();

	//for(int i=0; i<8; i++)
	//{//JOG手动图标
	//	m_JogButton[i].m_iIndex = i;
	//	m_JogButton[i].SubclassDlgItem(IDC_JOG_BUTTON_MV10+i, this);
	//	m_JogButton[i].SetBitmapEx(IDB_BITMAP_MOVE);
	//}
	//
    ((CSliderCtrl*)GetDlgItem(IDC_JOG_SLIDER_SPEED))->SetRange(0,200);
	((CSliderCtrl*)GetDlgItem(IDC_JOG_SLIDER_SPEED))->SetPos(50);

	int n = ((CSliderCtrl*)GetDlgItem(IDC_JOG_SLIDER_SPEED))->GetPos();
	CJogButton::s_dJogSpdRatio = n/200.0;
	m_dSpdRatio = n/200.0;

	
	/*CString strTemp;
	strTemp.Format("XY:%.1f,Z:%.1f", g_pFrm->m_Robot->m_pSysParam->dVelEditXY[2] * m_dSpdRatio,g_pFrm->m_Robot->m_pSysParam->dVelEditZ[2] * m_dSpdRatio);
	SetDlgItemText(IDC_JOG_EDIT_KEYCTRL, strTemp);*/

	m_fXYSpeed = g_pFrm->m_Robot->m_pSysParam->dVelEditXY[2] * m_dSpdRatio;
	m_fZSpeed = g_pFrm->m_Robot->m_pSysParam->dVelEditZ[2] * m_dSpdRatio;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgJog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if(pScrollBar-> GetDlgCtrlID() != IDC_JOG_SLIDER_SPEED) return;
	//int n2 = ((CSliderCtrl*)GetDlgItem(IDC_JOG_SLIDER_SPEED))->GetPos();
	if(nSBCode == SB_PAGEDOWN) 
	{
		((CSliderCtrl*)GetDlgItem(IDC_JOG_SLIDER_SPEED))->SetPos((int)(CJogButton::s_dJogSpdRatio*200));
		return;
	}
	int n = ((CSliderCtrl*)GetDlgItem(IDC_JOG_SLIDER_SPEED))->GetPos();
	CJogButton::s_dJogSpdRatio = n/200.0;
	m_dSpdRatio = n/200.0;

	//CString strTemp;
	//strTemp.Format("XY:%.2f,Z:%.2f", g_pFrm->m_Robot->m_pSysParam->dVelEditXY[2] * m_dSpdRatio,g_pFrm->m_Robot->m_pSysParam->dVelEditZ[2] * m_dSpdRatio);
	//SetDlgItemText(IDC_JOG_EDIT_KEYCTRL, strTemp);
	m_fXYSpeed = g_pFrm->m_Robot->m_pSysParam->dVelEditXY[2] * m_dSpdRatio;
	m_fZSpeed = g_pFrm->m_Robot->m_pSysParam->dVelEditZ[2] * m_dSpdRatio;
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgJog::OnBnClickedJogBtnDispTripper()
{
	// TODO: Add your control notification handler code here
	//CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_JOG_OPER_RADIO_HEAD);
	static bool bOn = true;
	m_chkTripperCheckbox.SetCheck(0);
	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		//if(bOn==true)
		//{
		//	if (((g_pFrm->m_Robot->m_outputSts &(0x0001<<g_pFrm->m_Robot->dcp[pBox->GetCurSel()].outBit))==(0x0001<<g_pFrm->m_Robot->dcp[pBox->GetCurSel()].outBit))
		//		||((g_pFrm->m_Robot->m_outputSts &(0x0001<<g_pFrm->m_Robot->dcp[pBox->GetCurSel()].outAtom))==(0x0001<<g_pFrm->m_Robot->dcp[pBox->GetCurSel()].outAtom)))
		//	{//未触发
		//		g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->dcp[pBox->GetCurSel()].outBit,TRUE);
		//		g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->dcp[pBox->GetCurSel()].outAtom,TRUE);
		//		m_chkTripperCheckbox.SetCheck(1);
		//	}		
		//}
		//else
		//{
		//	if (((g_pFrm->m_Robot->m_outputSts &(0x0001<<g_pFrm->m_Robot->dcp[pBox->GetCurSel()].outBit))==0x0000)
		//		||((g_pFrm->m_Robot->m_outputSts &(0x0001<<g_pFrm->m_Robot->dcp[pBox->GetCurSel()].outAtom))==0x0000))
		//	{//已触发
		//		g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->dcp[pBox->GetCurSel()].outAtom,FALSE);
		//		g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->dcp[pBox->GetCurSel()].outBit,FALSE);
		//		m_chkTripperCheckbox.SetCheck(0);
		//	}		
		//}
		//bOn=!bOn;
		g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->dcp[0].outBit[0],TRUE);
		Sleep(200);
		g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->dcp[0].outBit[0],false);





	}
	else
	{
		AfxMessageBox(_T("运动控制卡未正常初始化！"));
	}
}

void CDlgJog::OnBnClickedJogBtnDispWash()
{
   
   	m_chkRotateCheckbox.SetCheck(0);
    g_pFrm->m_Robot->ClearnNeedle();

}

void CDlgJog::OnBnClickedJogBtnDispQiya()
{
	// TODO: Add your control notification handler code here
	CString str;
    static long t_JetPress;
    static long t_DisPress;
	//m_chkUpdownCheckbox.SetCheck(0);
	m_chkUpdownCheckbox.GetWindowText(str);
   if (str=="关闭气压")
   {
      m_chkUpdownCheckbox.SetWindowText("开启气压");

	  t_JetPress=theApp.m_Serial.ReadDispPress(TRUE);
	  Sleep(50);
	  t_DisPress=theApp.m_Serial.ReadDispPress(false);
	  Sleep(50);
       theApp.m_Serial.SetDispPress(1,TRUE);
	   Sleep(100);
     theApp.m_Serial.SetDispPress(1,false);
   }
   else
   {
	   if (t_JetPress<=1||t_JetPress>500)
	   {
		   t_JetPress=250;
	   }
	   if (t_DisPress<=1||t_DisPress>500)
	   {
		   t_DisPress=250;
	   }
	   m_chkUpdownCheckbox.SetWindowText("关闭气压");
	   theApp.m_Serial.SetDispPress(t_JetPress,TRUE);
	   Sleep(100);
	   theApp.m_Serial.SetDispPress(t_DisPress,false);
   }
}

void CDlgJog::OnCbnSelchangeJogOperRadioHead()
{
	//// TODO: Add your control notification handler code here
	//CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_JOG_OPER_RADIO_HEAD);

	//if (g_pFrm->m_Robot->dcp[pBox->GetCurSel()].bRotateEnable==FALSE)
	//{
	//	GetDlgItem(IDC_JOG_BTN_DISP_WASH)->ShowWindow(SW_HIDE);
	//}
	//else
	//{
	//	if ((g_pFrm->m_Robot->m_outputSts &&(0x01<<g_pFrm->m_Robot->dcp[pBox->GetCurSel()].outRC))==0x00)
	//		((CButton*)GetDlgItem(IDC_JOG_BTN_DISP_WASH))->SetCheck(BST_CHECKED);
	//	else//未触发
	//		((CButton*)GetDlgItem(IDC_JOG_BTN_DISP_WASH))->SetCheck(BST_UNCHECKED);

	//	GetDlgItem(IDC_JOG_BTN_DISP_WASH)->ShowWindow(SW_SHOW);
	//}
	//////////////////
	//if (g_pFrm->m_Robot->dcp[pBox->GetCurSel()].bUpDownEnable==FALSE)
	//{
	//	GetDlgItem(IDC_JOG_BTN_DISP_QIYA)->ShowWindow(SW_HIDE);
	//}
	//else
	//{
	//	if ((g_pFrm->m_Robot->m_outputSts &&(0x01<<g_pFrm->m_Robot->dcp[pBox->GetCurSel()].outLC))==0x00)
	//		((CButton*)GetDlgItem(IDC_JOG_BTN_DISP_QIYA))->SetCheck(BST_CHECKED);
	//	else
	//		((CButton*)GetDlgItem(IDC_JOG_BTN_DISP_QIYA))->SetCheck(BST_UNCHECKED);
	//	GetDlgItem(IDC_JOG_BTN_DISP_QIYA)->ShowWindow(SW_SHOW);
	//}
}

void CDlgJog::UpdateJogBarWindow()
{
	///////////////////
	/*
	if (g_pFrm->m_Robot->m_tSysStatus!=K_RUN_STS_STOP)
	{
		if (m_chkTripperCheckbox.IsWindowEnabled()==TRUE)
		{
			m_chkTripperCheckbox.EnableWindow(FALSE);
		}
		if (m_chkRotateCheckbox.IsWindowEnabled()==TRUE)
		{
			m_chkRotateCheckbox.EnableWindow(FALSE);
		}
		if (m_chkUpdownCheckbox.IsWindowEnabled()==TRUE)
		{
			m_chkUpdownCheckbox.EnableWindow(FALSE);
		}
		for (int i=0;i<8;i++)
		{
			if (m_JogButton[i].IsWindowEnabled()==TRUE)
				m_JogButton[i].EnableWindow(FALSE);
		}
	}
	else
	{
		if (m_chkTripperCheckbox.IsWindowEnabled()==FALSE)
		{
			m_chkTripperCheckbox.EnableWindow(TRUE);
		}
		if (m_chkRotateCheckbox.IsWindowEnabled()==FALSE)
		{
			m_chkRotateCheckbox.EnableWindow(TRUE);
		}
		if (m_chkUpdownCheckbox.IsWindowEnabled()==FALSE)
		{
			m_chkUpdownCheckbox.EnableWindow(TRUE);
		}
		for (int i=0;i<8;i++)
		{
			if (m_JogButton[i].IsWindowEnabled()==FALSE)
				m_JogButton[i].EnableWindow(TRUE);
		}
	}
	*/
	////////////////////
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_JOG_OPER_RADIO_HEAD);
	int iCurIndex = pBox->GetCurSel();
	if(((g_pFrm->m_Robot->m_outputSts &(0x01<<g_pFrm->m_Robot->dcp[iCurIndex].outBit[0]))==(0x01<<g_pFrm->m_Robot->dcp[iCurIndex].outBit[0])))
	{
		g_pFrm->m_Robot->m_drawTrack=FALSE;
		g_pFrm->m_Robot->m_bStartOpenDisp = false;
		if (m_chkTripperCheckbox.GetCheck()==BST_CHECKED)

			m_chkTripperCheckbox.SetCheck(BST_UNCHECKED);
	}
	else
	{
		g_pFrm->m_Robot->m_drawTrack=TRUE;

		if (m_chkTripperCheckbox.GetCheck()==BST_UNCHECKED)
			m_chkTripperCheckbox.SetCheck(BST_CHECKED);
	}
	//if((g_pFrm->m_Robot->m_outputSts &(0x01<<g_pFrm->m_Robot->dcp[iCurIndex].outRC))==(0x01<<g_pFrm->m_Robot->dcp[iCurIndex].outRC))
	//{
	//	if (m_chkRotateCheckbox.GetCheck()==BST_CHECKED)
	//		m_chkRotateCheckbox.SetCheck(BST_UNCHECKED);
	//}
	//else
	//{
	//	if (m_chkRotateCheckbox.GetCheck()==BST_UNCHECKED)
	//		m_chkRotateCheckbox.SetCheck(BST_CHECKED);
	//}
	//if((g_pFrm->m_Robot->m_outputSts &(0x01<<g_pFrm->m_Robot->dcp[iCurIndex].outLC))==(0x01<<g_pFrm->m_Robot->dcp[iCurIndex].outLC))
	//{
	//	if (m_chkUpdownCheckbox.GetCheck()==BST_CHECKED)
	//		m_chkUpdownCheckbox.SetCheck(BST_UNCHECKED);
	//}
	//else
	//{
	//	if (m_chkUpdownCheckbox.GetCheck()==BST_UNCHECKED)
	//		m_chkUpdownCheckbox.SetCheck(BST_CHECKED);
	//}
}

void CDlgJog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	// TODO: Add your message handler code here

/*
	CMainFrame *pMain=(CMainFrame *)(AfxGetApp()->m_pMainWnd);
	if(nState==WA_ACTIVE)
	{	
		CString strTemp;
		strTemp.Format("XY:%.1f,Z:%.1f", pMain->m_Robot->m_pSysParam->dVelEditXY[2] * m_dSpdRatio,pMain->m_Robot->m_pSysParam->dVelEditZ[2] * m_dSpdRatio);
		SetDlgItemText(IDC_JOG_EDIT_KEYCTRL, strTemp);

		OnCbnSelchangeJogOperRadioHead();
	}
*/
}

HBRUSH CDlgJog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	if (pWnd->GetDlgCtrlID()==IDC_ST_TEMP1)
		pDC->SetTextColor(RGB(0,0,255));  //设置字体颜色
	if (pWnd->GetDlgCtrlID()==IDC_ST_TEMP2)
		pDC->SetTextColor(RGB(0,0,255));  //设置字体颜色
	if (pWnd->GetDlgCtrlID()==IDC_ST_PRESS1)
		pDC->SetTextColor(RGB(0,0,255));  //设置字体颜色
	if (pWnd->GetDlgCtrlID()==IDC_ST_PRESS2)
		pDC->SetTextColor(RGB(0,0,255));  //设置字体颜色
	if (pWnd->GetDlgCtrlID()==IDC_ST_BALA)
		pDC->SetTextColor(RGB(0,0,255));  //设置字体颜色
	if (pWnd->GetDlgCtrlID()==IDC_XY_SPEED)
		pDC->SetTextColor(RGB(0,0,255));  //设置字体颜色
	if (pWnd->GetDlgCtrlID()==IDC_SPEED_Z)
		pDC->SetTextColor(RGB(0,0,255));  //设置字体颜色
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CDlgJog::OnBnClickedButZero()
{
	// TODO: Add your control notification handler code here
	theApp.m_Serial.BalanceZero();
}

void CDlgJog::OnBnJogCheckStep()
{
	// TODO: Add your control notification handler code here
	CButton *pBnt = (CButton*)GetDlgItem(IDC_JOG_CHECK_STEP);
	TRACE(_T("\nbEnable=%d"), pBnt->GetCheck());
	CJogButton::s_bEnableStep = static_cast<bool>(pBnt->GetCheck());
	if( true == CJogButton::s_bEnableStep )
	{
		GetDlgItem(IDC_JOG_EDIT_STEP_LENGTH)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_JOG_EDIT_STEP_LENGTH)->EnableWindow(FALSE);
	}
}

void CDlgJog::OnEnChangeJogEditStepLength()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strText;

	GetDlgItemText(IDC_JOG_EDIT_STEP_LENGTH, strText);
	if( true == CFunction::IsNumber(strText) )
	{
		UpdateData(TRUE);
		TRACE(_T("\nm_fJogStep=%f"), m_fJogStep);
		CJogButton::s_fStepDist = m_fJogStep;
	}
	else
	{
		UpdateData(FALSE);
		AfxMessageBox(_T("只能输入数字！"));
	}
}

void CDlgJog::OnEnChangeXYSpeed()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strText;
	GetDlgItemText(IDC_XY_SPEED, strText);
	if( true == CFunction::IsNumber(strText) )
	{
		UpdateData(TRUE);
		if( m_fXYSpeed > 200 ) m_fXYSpeed = 200;
		m_fZSpeed = m_fXYSpeed*0.5;
		UpdateData(FALSE);
		((CSliderCtrl*)GetDlgItem(IDC_JOG_SLIDER_SPEED))->SetPos(m_fXYSpeed);
		CJogButton::s_dJogSpdRatio = m_fXYSpeed/200.0;
	}
	else
	{
		UpdateData(FALSE);
		AfxMessageBox(_T("只能输入数字！"));
	}
}
