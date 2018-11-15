// DlgPosInfo.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgPosInfo.h"
#pragma warning(disable: 4100)

#include "DlgTrackDraw.h"
// CDlgPosInfo dialog
IMPLEMENT_DYNAMIC(CDlgPosInfo, CDialog)

CDlgPosInfo::CDlgPosInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPosInfo::IDD, pParent)
{
   bNeedShowInfo  =FALSE;
   m_bWasteGlueAlarm = false;
}

CDlgPosInfo::~CDlgPosInfo()
{
}
void CDlgPosInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPosInfo)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DLG_INFO_MODE, m_BtnNetMode);
	DDX_Control(pDX, IDC_BTN_WASTEGLUE__CLEAR, m_btn_test);

	DDX_Control(pDX, IDC_BTN_RUN, m_btnRun);
	DDX_Control(pDX, IDC_BTN_PAUSE, m_btnPause);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
	//DDX_Control(pDX, IDC_PROGRESS_FEEDCURID, m_Info_FeedBoxCurID);
	//DDX_Control(pDX, IDC_PROGRESS_UNFEEDCURID, m_Info_UnFeedBoxCurID);
	// 	DDX_Control(pDX, IDC_BTN_PATH, m_btnPath);
	// 	DDX_Control(pDX, IDC_BTN_MACHINE_SET, m_btnMachineSet);
	// 	DDX_Control(pDX, IDC_BTN_USER, m_btnUser);
	// 	DDX_Control(pDX, IDC_BTN_JETADJUST, m_btnJetAdjust);
	DDX_Control(pDX, IDC_STATIC_PROGNAME, m_staticProgName);
}

BEGIN_MESSAGE_MAP(CDlgPosInfo, CDialog)
	//ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CCD_TRIGGER, &CDlgPosInfo::OnBnClickedBtnCcdTrigger)
	ON_BN_CLICKED(IDC_BTN_LOAD_IMAGE, &CDlgPosInfo::OnBnClickedBtnLoadImage)
	ON_BN_CLICKED(IDC_DLG_INFO_MODE, &CDlgPosInfo::OnBnClickedDlgInfoMode)

//	ON_BN_CLICKED(IDC_INFO_CHECK_BYPASS, &CDlgPosInfo::OnBnClickedInfoCheckBypass)
//	ON_BN_CLICKED(IDC_BUTTON_HANDSELIMT2, &CDlgPosInfo::OnBnClickedButtonHandselimtRESET)
//	ON_BN_CLICKED(IDC_BUTTON_HANDSELIMT3, &CDlgPosInfo::OnBnClickedButtonHandselimt3)
//	ON_BN_CLICKED(IDC_INFO_CHECK_ENABLEAPFS, &CDlgPosInfo::OnBnClickedInfoCheckEnableapfs)
//	ON_BN_CLICKED(IDC_BUTTON1, &CDlgPosInfo::OnBnClickedButton1)
// 	ON_BN_CLICKED(IDC_BUTTON_HANDSELIMT, &CDlgPosInfo::OnBnClickedButtonHandselimt)
// 	ON_BN_CLICKED(IDC_BUTTON_HANDSELIMT4, &CDlgPosInfo::OnBnClickedButtonHandselimt4)
// 	ON_BN_CLICKED(IDC_BUTTON_HANDSELIMT5, &CDlgPosInfo::OnBnClickedButtonHandselimt5)
// 	ON_BN_CLICKED(IDC_BUTTON_HANDSELIMT6, &CDlgPosInfo::OnBnClickedButtonHandselimt6)
ON_WM_PAINT()
ON_BN_CLICKED(IDC_BTN_WASTEGLUE__CLEAR, &CDlgPosInfo::OnBnClickedBtnWasteglue)
ON_BN_CLICKED(IDC_BTN_PATH, &CDlgPosInfo::OnBnClickedBtnPath)
ON_BN_CLICKED(IDC_BTN_MOV, &CDlgPosInfo::OnBnClickedBtnMov)
ON_BN_CLICKED(IDC_BTN_RUN, &CDlgPosInfo::OnBnClickedBtnRun)
ON_BN_CLICKED(IDC_BTN_PAUSE, &CDlgPosInfo::OnBnClickedBtnPause)
ON_BN_CLICKED(IDC_BTN_STOP, &CDlgPosInfo::OnBnClickedBtnStop)
ON_BN_CLICKED(IDC_BTN_MultiValve_Set, &CDlgPosInfo::OnBnClickedBtnMultivalveSet)
ON_BN_CLICKED(IDC_Btn_Load_Board, &CDlgPosInfo::OnBnClickedBtnLoadBoard)
ON_BN_CLICKED(IDC_Btn_Unload_Board, &CDlgPosInfo::OnBnClickedBtnUnloadBoard)
END_MESSAGE_MAP()


// CDlgPosInfo message handlers

BOOL CDlgPosInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_brushBk.CreateSolidBrush(RGB(32, 32, 32));
	//m_brushBk = CreateHatchBrush(HS_BDIAGONAL, RGB(32, 32, 32));
	m_btn_test.SetShade(CShadeButtonST::SHS_METAL);
	m_BtnNetMode.SetIcon(IDI_ICON_NET);
	m_BtnNetMode.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	m_btnRun.SetIcon(IDI_ICON_EZ_RUN);
	m_btnRun.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnRun.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnPause.SetIcon(IDI_ICON_EZ_PAUSE);
	m_btnPause.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnPause.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnStop.SetIcon(IDI_ICON_EZ_STOP);
	m_btnStop.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnStop.SetAlign(CButtonST::ST_ALIGN_VERT);


// 	m_Font1.CreateFont(22,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,
// 		CLIP_STROKE_PRECIS,DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("UnidreamLED"));
// 	CFont m_FontBC;
	m_Font.CreateFont(24, // nHeight 
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
		_T("Arail")); // lpszFac ΢���ź�
	//_T("UnidreamLED")); // lpszFac ΢���ź�

	m_staticProgName.SetFont(&m_Font);

// 	m_btnPath.SetIcon(IDI_ICON_PATH);
// 	m_btnPath.SetAlign(CButtonST::ST_ALIGN_VERT);
// 	m_btnPath.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
// 	m_btnPath.SetFlat(FALSE);
// 
// 	m_btnMachineSet.SetIcon(IDI_ICON_SET);
// //	m_btnMachineSet.SetAlign(CButtonST::ST_ALIGN_VERT);
// 	m_btnMachineSet.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
// 	m_btnMachineSet.SetFlat(FALSE);
// 
// 	m_btnUser.SetIcon(IDI_ICON_USER);
// 	m_btnUser.SetAlign(CButtonST::ST_ALIGN_VERT);
// 	m_btnUser.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
// 	m_btnUser.SetFlat(FALSE);
// 	m_edit_pos_x.SubclassDlgItem(IDC_INFO_EDIT1, this);
// 	m_edit_pos_x.SetFont(&m_Font1);
// 	m_edit_pos_y.SubclassDlgItem(IDC_INFO_EDIT2, this);
// 	m_edit_pos_z.SubclassDlgItem(IDC_INFO_EDIT3, this);
	GetDlgItem(IDC_INFO_EDIT1)->SetFont(&m_Font);     
	GetDlgItem(IDC_INFO_EDIT2)->SetFont(&m_Font);      
	GetDlgItem(IDC_INFO_EDIT3)->SetFont(&m_Font);    

// 	GetDlgItem(IDC_INFO_STATIC_MOTION)->SetFont(&m_Font);
// 	GetDlgItem(IDC_INFO_STATIC_LOAD)->SetFont(&m_Font);
// 	GetDlgItem(IDC_INFO_STATIC_UNLOAD)->SetFont(&m_Font);
	GetDlgItem(IDC_INFO_EDIT_WASTEGLUE)->SetFont(&m_Font);

	CString strTemp;
	strTemp.Format("%.3f", g_pFrm->m_pRobotParam->m_dWasteGlueCur/1000);
	SetCurWasteGlue(strTemp);
// 	m_FontBC.CreateFont(25, // nHeight 
// 		0, // nWidth 
// 		0, // nEscapement 
// 		0, // nOrientation 
// 		FW_BOLD, // nWeight 
// 		FALSE, // bItalic 
// 		FALSE, // bUnderline 
// 		0, // cStrikeOut 
// 		ANSI_CHARSET, // nCharSet 
// 		OUT_DEFAULT_PRECIS, // nOutPrecision 
// 		CLIP_DEFAULT_PRECIS, // nClipPrecision 
// 		DEFAULT_QUALITY, // nQuality 
// 		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
// 		_T("Arail")); // lpszFac ΢���ź�

//	SetTimer(0, 5000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCle m_Runtime;EPTION: OCX Property Pages should return FALSE
}

void  CDlgPosInfo::UpdateFileShowState(BOOL bshow)
{
	if(bshow)
	{
		GetDlgItem(IDC_BUTTON_HANDSELIMT)->ShowWindow(TRUE);
	}else
	{
		GetDlgItem(IDC_BUTTON_HANDSELIMT)->ShowWindow(TRUE);
	}
}
//����ϵͳ��Ϣ����״̬��
void CDlgPosInfo::UpdateMotionState()
{
	CString str;
	switch(g_pFrm->m_Robot->m_tSysStatus)
	{
	case K_RUN_STS_NONE:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_1");
		break;
	case K_RUN_STS_RUN:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_2");
		break;
	case K_RUN_STS_PAUSE:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_3");
		break;
	case K_RUN_STS_STOP:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_4");
		break;
	case K_RUN_STS_EMERGENCY:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_5");
		break;
	case K_RUN_STS_WEIGHTERROR:
		str = "�������ݳ���";
		break;
	case 	K_AXIS_STS_LIMIT_XP:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_6");
		break;
	case 	K_AXIS_STS_LIMIT_XS:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_7");
		break;
	case 	K_AXIS_STS_LIMIT_YP:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_8");
		break;
	case 	K_AXIS_STS_LIMIT_YS:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_9");
		break;
	case 	K_AXIS_STS_LIMIT_ZP:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_10");
		break;
	case 	K_AXIS_STS_LIMIT_ZS:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_11");
		break;
	case 	K_AXIS_STS_ALM_X:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_12");
		break;
	case 	K_AXIS_STS_ALM_Y:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_13");
		break;
	case 	K_AXIS_STS_ALM_Z:
		str = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Sts_14");
		break;
	default:
		break;
	}
	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,str);
     
// 	if(g_pFrm->bisWeightError == TRUE) 
// 	{
// 		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,_T("���س��ޱ�����"));
// 	}

    g_pFrm->SetPanelTextEx(IDS_RUN_STATUS, str);	
}

HBRUSH CDlgPosInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID()==IDC_INFO_EDIT1)
		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ
	if (pWnd->GetDlgCtrlID()==IDC_INFO_EDIT2)
		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ
	if (pWnd->GetDlgCtrlID()==IDC_INFO_EDIT3)
		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ
// 	if (pWnd->GetDlgCtrlID()==IDC_INFO_EDIT4)
// 	     pDC->SetTextColor(RGB(0,0,255));  //����������ɫ

	if (pWnd->GetDlgCtrlID()==IDC_INFO_EDIT5)
		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ

	if (pWnd->GetDlgCtrlID()==IDC_INFO_STATIC_MOTION)
		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ
	if (pWnd->GetDlgCtrlID()==IDC_INFO_STATIC_LOAD)
		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ
	if (pWnd->GetDlgCtrlID()==IDC_INFO_STATIC_UNLOAD)
		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ

	if (pWnd->GetDlgCtrlID()==IDC_INFO_EDIT_WASTEGLUE)
	{
		if (m_bWasteGlueAlarm)
		{
			pDC->SetTextColor(RGB(255,255,255));  //����������ɫ
			pDC->SetBkColor(RGB(255, 0, 0));  //����������ɫ
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,255));  //����������ɫ
			//pDC->SetBkColor(RGB(255, 0, 0));  //����������ɫ
		}
	}


// 	if (pWnd->GetDlgCtrlID()==IDC_INFO_STATIC_BCDATA)
// 		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ
// 
// 	if (pWnd->GetDlgCtrlID()==IDC_INFO_STATIC_BCDATA2)
// 		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ
// 
// 	if (pWnd->GetDlgCtrlID()==IDC_INFO_STATIC_BCID)
// 		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ
// 
// 	if (pWnd->GetDlgCtrlID()==IDC_INFO_STATIC_BCID2)
// 		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ

//     if (pWnd->GetDlgCtrlID()==S_11)
// 		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ
// 
// 	if (pWnd->GetDlgCtrlID()==S_12)
// 		pDC->SetTextColor(RGB(0,0,255));  //����������ɫ

	return hbr;
}

void CDlgPosInfo::SetLoadStatus(CString strTemp)
{
	GetDlgItem(IDC_INFO_STATIC_LOAD)->SetWindowText(strTemp);
}

void CDlgPosInfo::SetUnloadStatus(CString strTemp)
{
	GetDlgItem(IDC_INFO_STATIC_UNLOAD)->SetWindowText(strTemp);
}
// ����ģʽ��ʾ�л�
void CDlgPosInfo::UpsysWorkMode(void)
{
// 	if (g_pFrm->m_pSysParam->nAutoMode==1)
// 	{
// 		m_BtnNetMode.SetIcon(IDI_ICON_NET);
// 		g_pFrm->m_Robot->Net();
// 		m_BtnNetMode.SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Btn_51"));
// 		GetDlgItem(IDC_BUTTON_HANDSELIMT5)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_BUTTON_HANDSELIMT6)->EnableWindow(FALSE);
// 	}
// 	else
// 	{
// 		m_BtnNetMode.SetIcon(IDI_ICON_UNNET);
// 		g_pFrm->m_Robot->UnNet();
// 		m_BtnNetMode.SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Btn_52"));
// 		GetDlgItem(IDC_BUTTON_HANDSELIMT5)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_BUTTON_HANDSELIMT6)->EnableWindow(TRUE);
// 	}
// 	m_BtnNetMode.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
}
// �л�����ģʽ
void CDlgPosInfo::OnBnClickedDlgInfoMode()
{
	CString msg;
	// TODO: Add your control notification handler code here
	if ( g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_STOP ||g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_NONE )
	{
		if (g_pFrm->m_pSysParam->nAutoMode == 1 )
		{
			g_pFrm->m_pSysParam->nAutoMode=0;
			g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(TRUE);
			g_pFrm->m_wndEditBar.m_wndList.EnableWindow(TRUE);
		}
		else
		{
			g_pFrm->m_pSysParam->nAutoMode=1;
			g_pFrm->m_wndEditBar.m_wndShotCutEdit.EnableWindow(FALSE);
			g_pFrm->m_wndEditBar.m_wndList.EnableWindow(FALSE);
		
		}
		UpsysWorkMode();
	}
	else
	{
		TRACE(_T("\nm_tSysStatus=%d"),  g_pFrm->m_Robot->m_tSysStatus);
		msg = g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgInfo,"Btn_4");
		AfxMessageBox("����ֹͣ���У�");
		return;
	}
}
// by pass 
// void CDlgPosInfo::OnBnClickedInfoCheckBypass()
// {
// 	// TODO: Add your control notification handler code here
// 	if ( g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_STOP ||g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_NONE )
// 	{
// 		//if (g_pFrm->m_pSysParam->nAutoMode == 1 )
// 		//{
// 			g_pFrm->m_pSysParam->nByPassFlag = ((CButton *)GetDlgItem(IDC_INFO_CHECK_BYPASS))->GetCheck();
// 			g_pFrm->m_Robot->ExOutput("BYPASS",g_pFrm->m_pSysParam->nByPassFlag);
// 		//}
// 	}
// }

void CDlgPosInfo::OnTimer(UINT_PTR nIDEvent)
{
	CTime CurTime = CTime::GetCurrentTime();
	CString str;
	if (!theApp.m_EncryptOp.IsInstalled())
	{
		//û��ע�����Ϣ��д��
		theApp.m_EncryptOp.InitRegeditInfo(CurTime);
	}
	else if(!theApp.m_EncryptOp.GetRegisterFlag())
	{
		CTime LastTime;
		CTime LimitTime;
		theApp.m_EncryptOp.GetLastTime(LastTime);
		theApp.m_EncryptOp.GetLimitTime(LimitTime);
		if(CurTime>LastTime)
		{
			theApp.m_EncryptOp.SetLastTime(CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(),
				CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond());
		}
		if (CurTime>LimitTime||CurTime<LastTime)
		{   
			if(!theApp.m_EncryptOp.m_bLockSystem)
			{
				theApp.m_EncryptOp.m_bLockSystem = TRUE;

				AfxMessageBox(_T("���ð��ѵ��ڣ���ע���������!"),MB_OK);
			}
		}
		else
		{
			theApp.m_EncryptOp.m_bLockSystem = FALSE;
		}
		int day =-1;
		BOOL rtn =theApp.m_EncryptOp.CheckLMTTime(CurTime,day);
        if((rtn == FALSE)&&(bNeedShowInfo== FALSE))
		{
            bNeedShowInfo  = TRUE;
		    str.Format(_T("%d ��!"),day);
			str = _T("�������ڽ�������")+str;
			AfxMessageBox(str);
		}
	}
	else
	{
		if(theApp.m_EncryptOp.m_bLockSystem)
		{
			theApp.m_EncryptOp.m_bLockSystem = FALSE;
		}
	}

	CDialog::OnTimer(nIDEvent);
}


// void CDlgPosInfo::OnBnClickedButtonHandselimt()
// {
//   if(g_pFrm->bisWeightError == TRUE)
//   {
// 	  g_pFrm->bisWeightError =FALSE;
// 	 // g_pFrm->m_Robot->m_tSysStatus  = K_RUN_STS_STOP;
//       GetDlgItem(IDC_BUTTON_HANDSELIMT)->EnableWindow(FALSE);
//       SetDlgItemText(IDC_INFO_STATIC_MOTION,_T("ϵͳ����!"));
// 	   g_pFrm->m_Robot->SetLamp();
//   }
// }

// �ֶ���λ�ֶΣУ̣�
// void CDlgPosInfo::OnBnClickedButtonHandselimtRESET()
// {
// 	// �赲���׸�λ
// 	g_pFrm->m_Robot->ExOutput(_T("�ӹ��赲����"),TRUE);
// 	
// 	CFunction::DelaySec(0.3);                          // s
// //	g_pFrm->m_Robot->ExOutput(_T("�ӹ��赲����"),FALSE);
//     
// }

// void CDlgPosInfo::OnBnClickedButtonHandselimt3()
// {
// 	// �������׸�λ
// 	g_pFrm->m_Robot->ExOutput(_T("�ӹ���������"),TRUE);
// 		CFunction::DelaySec(0.3);                          // s
// }

// void CDlgPosInfo::OnBnClickedButtonHandselimt4()
// {
// 	// ���ʹ�����
// 	g_pFrm->m_Robot->ExOutput(_T("���ʹ��˶�"),TRUE);
// 	CFunction::DelaySec(0.3);   
// }

// ���̻�����ʹ��
// void CDlgPosInfo::OnBnClickedInfoCheckEnableapfs()
// {  
//     int STS= ((CButton *)GetDlgItem(IDC_INFO_CHECK_ENABLEAPFS))->GetCheck();
// 	if((STS == 1)&&(g_pFrm->m_Robot->m_bAutoPreFic == FALSE))
// 	{
// 		g_pFrm->m_Robot->m_bAutoPreFic = TRUE;
// 		g_pFrm->m_Robot->AutoRPSRun();  
// 	}
// 	else
// 	{
//         g_pFrm->m_Robot->m_bAutoPreFic = FALSE;
// 	}
// }
// ��׼�߶ȱ궨
// void CDlgPosInfo::OnBnClickedButton1()
// {	
// 	g_pFrm->ShowZBaseSetDlg();
// }

// //�հ��ֶ�����
// void CDlgPosInfo::OnBnClickedButtonHandselimt5()
// {
// 	CString str;
// 	double  date;
// 	BOOL bIsS;
// 
// 	bIsS = theApp.m_Serial.ReadBSMBalance(date);
// 	if(bIsS == TRUE)
// 	{
// 		str.Format(_T("%.3f"),date);
// 	}
// 	else
// 		str="-Error-";
// 	SetDlgItemText(IDC_INFO_STATIC_BCDATA,str);// ״̬
// 	SetDlgItemText(IDC_INFO_STATIC_MOTION,"�ֶ��հ����");
// }
// �����ֶ�����
// void CDlgPosInfo::OnBnClickedButtonHandselimt6()
// {
// // 	CString str;
// // 	double  date;
// // 	BOOL bIsS;
// 
// // 	bIsS = theApp.m_Serial.ReadBSMBalanceEx(date);
// // 	if(bIsS == TRUE)
// // 	{
// // 		str.Format(_T("%.3f"),date);
// // 	}
// // 	else
// // 		str="-Error-";
// 
// // 	SetDlgItemText(IDC_INFO_STATIC_BCDATA2,str);// ״̬
// // 	SetDlgItemText(IDC_INFO_STATIC_MOTION,"�ֶ��������");
// }

#include <io.h>
void CDlgPosInfo::OnBnClickedBtnCcdTrigger()
{
// 	CDlgTrackDraw dlg;
// 	dlg.DoModal();
// 	return;


	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pView->m_pCamera->SetTriggerMode(false);
//	g_pView->m_ImgStatic.m_pCamera->GrabOneFrame();
// 	if (0 == _access("cap", 0))
// 	{
// 		CTime time = CTime::GetCurrentTime();
// 		CString str = "";
// 		str = time.Format("%Y-%m-%d %H:%M:%S");
// 		str = str + ".bmp";
// 		g_pView->m_ImgStatic.SaveImage(str);
// 	}
}


void CDlgPosInfo::OnBnClickedBtnLoadImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	g_pFrm->m_Robot->m_pController->SetExOutport(0, -1, TRUE);

// 	CFileDialog dlg(TRUE);
// 	CString strPath;
// 
// 	if (IDOK == dlg.DoModal())
// 	{
// 		strPath = dlg.GetPathName();
// 		g_pView->LoadImage(strPath);
// 	}	
}

void CDlgPosInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

// 	CRect rc;
// 	GetClientRect(rc);
// 
// 	FillRect(dc.m_hDC, rc, m_brushBk);
}

void CDlgPosInfo::SetCurWasteGlue(CString strTemp)		// ��ʾ�Ͻ��н���
{
	SetDlgItemText(IDC_INFO_EDIT_WASTEGLUE, strTemp);
// 	if ("�Ͻ�������" == strTemp)
// 	{
// 		GetDlgItem(IDC_INFO_EDIT_WASTEGLUE)->set
// 	}
}

void CDlgPosInfo::SetCurProgName()
{
	CString strTemp;
	strTemp.Format("��ǰ��Ʒ:[%s]", g_AllProgInfo[0].m_curProgName);
	SetDlgItemText(IDC_STATIC_PROGNAME, strTemp);
}

void CDlgPosInfo::OnBnClickedBtnWasteglue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pFrm->m_Robot->ExOutput(_T("��ɫ��.��"),FALSE);
	g_pFrm->m_Robot->ExOutput(_T("������"),FALSE);
	g_pFrm->m_pRobotParam->m_dWasteGlueCur = 0.0;
	SetCurWasteGlue("0.0");

	m_bWasteGlueAlarm = false;
	// ���浱ǰ�Ͻ����н���
	CString strFile = CFunction::GetDirectory() + "system\\robotparam.dat";
	CFunction::HandleDouble(FALSE, "WasteGlue", "CurWt", g_pFrm->m_pRobotParam->m_dWasteGlueCur, strFile);		
}


void CDlgPosInfo::OnBnClickedBtnPath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

UINT ThreadTestCPU(LPVOID lpParam)
{
	int cpuNo = *(int*)lpParam;

	CKFTimer tm;
	while (1)
	{
		CFunction::DelayExSec(1);
		CString str;
		str.Format("CPU[%d]  time: %.3f", cpuNo, tm.GetCurrentTimeEx());
		g_pFrm->AddMsg(str);
	}
	return 0;
}

void CDlgPosInfo::OnBnClickedBtnMov()
{ 
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i=0; i<8; i++)
	{
		CWinThread* pThread;
		pThread = AfxBeginThread(ThreadTestCPU, &i, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		SetThreadAffinityMask(pThread->m_hThread, 0x01<<i);
		pThread->ResumeThread();
	}
	return;

	CString str;
// 	str.Format("111111\r\n222222\r\n22222\r\n22222\r\n");
// 	AfxMessageBox(str);
	g_pFrm->m_Robot->m_pController->SetExOutport(0, -1, FALSE);
//	CFunction::WriteLog(__FILE__, __LINE__, "��תƤ�� ʧ��!", CFunction::LEV_ERROR);
}


void CDlgPosInfo::OnBnClickedBtnRun()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (g_pFrm->m_Robot->m_tSysStatus != K_RUN_STS_NONE)
	{
		g_pFrm->m_Robot->Run();
	}  
}


void CDlgPosInfo::OnBnClickedBtnPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pFrm->m_Robot->m_bIsHandPause = true;
	g_pFrm->m_Robot->Pause();
}


void CDlgPosInfo::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pFrm->m_Robot->Stop();  
}

#include "DlgDualValve.h"
void CDlgPosInfo::OnBnClickedBtnMultivalveSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgDualValve dlg;
	dlg.DoModal();
}


void CDlgPosInfo::OnBnClickedBtnLoadBoard()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDlgPosInfo::OnBnClickedBtnUnloadBoard()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
