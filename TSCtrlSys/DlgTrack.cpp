// DlgTrack.cpp : 实现文件
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgTrack.h"
#include "afxdialogex.h"


// CDlgTrack 对话框

IMPLEMENT_DYNAMIC(CDlgTrack, CDialogEx)

CDlgTrack::CDlgTrack(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTrack::IDD, pParent)
	, m_btnRadioAdd(0)
	, m_btnRadioFix(0)
{

	m_dDotRadius = 0.0;
	m_nAutoRpsDots = 0;
	m_dAutoRpsInterval = 0.0;
	m_nCleanInerval = 0;
}

CDlgTrack::~CDlgTrack()
{
}

void CDlgTrack::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_1,	m_static_pre);	
	DDX_Control(pDX, IDC_STATIC_2,	m_static_next);	
	DDX_Control(pDX, IDC_STATIC_3,	m_static_11);	
	DDX_Control(pDX, IDC_STATIC_4,	m_static_12);	
	DDX_Control(pDX, IDC_STATIC_5,	m_static_13);	
	DDX_Control(pDX, IDC_STATIC_6,	m_static_14);	
	DDX_Control(pDX, IDC_STATIC_7,	m_static_stop_1);
	DDX_Control(pDX, IDC_STATIC_8,	m_static_stop_2);
	DDX_Control(pDX, IDC_STATIC_9,	m_static_lift_1);
	DDX_Control(pDX, IDC_STATIC_10, m_static_lift_2);
	DDX_Control(pDX, IDC_STATIC_11, m_static_clamp);	

	DDX_Control(pDX, IDC_BTN_ASK_PRE,	m_btn_ask_pre);		
	DDX_Control(pDX, IDC_BTN_ASK_NEXT,	m_btn_ask_next);		
	DDX_Control(pDX, IDC_BTN_STOP_11,	m_btn_stop_1);		
	DDX_Control(pDX, IDC_BTN_STOP_12,	m_btn_stop_2);		
	DDX_Control(pDX, IDC_BTN_LIFT,		m_btn_lift);			
	DDX_Control(pDX, IDC_BTN_CLAMP,		m_btn_clamp);			
	DDX_Control(pDX, IDC_BTN_LOAD,		m_btn_board_load);	
	DDX_Control(pDX, IDC_BTN_FIX,		m_btn_board_fix);		
	DDX_Control(pDX, IDC_BTN_UNLOAD,	m_btn_board_unload);	
	DDX_Control(pDX, IDC_BTN_UNFIX,		m_btn_board_unfix);	
	DDX_Control(pDX, IDC_BTN_TARCK_R,	m_btn_track_right);	
	DDX_Control(pDX, IDC_BTN_TRACK_L,	m_btn_track_left);	

	DDX_Radio(pDX, IDC_RADIO1, m_btnRadioAdd);
	DDX_Radio(pDX, IDC_RADIO4, m_btnRadioFix);
	DDX_Control(pDX, IDC_BTN_REV_HOME, m_btn_rev_home);
	DDX_Control(pDX, IDC_BTN_REV_BOARD, m_btn_rev_board);
	DDX_Control(pDX, IDC_BTN_WORK2REV, m_btn_work2rev);
	DDX_Control(pDX, IDC_BTN_REV2WORK, m_btn_rev2work);
	DDX_Control(pDX, IDC_CHK_REV_STOP_1, m_btn_rev_stop1);
	DDX_Control(pDX, IDC_CHK_REV_STOP_2, m_btn_rev_stop2);
	DDX_Control(pDX, IDC_CHK_REV_CLAMP, m_btn_rev_clamp);
	DDX_Control(pDX, IDC_BTN_TRACK_L2, m_btn_rev_track_l);
	DDX_Control(pDX, IDC_BTN_TARCK_R2, m_btn_rev_track_r);

	DDX_Control(pDX, IDC_STATIC_12,	m_static_rev_stop1);	
	DDX_Control(pDX, IDC_STATIC_13,	m_static_rev_stop2);	
	DDX_Control(pDX, IDC_STATIC_14,	m_static_rev_clamp1);	
	DDX_Control(pDX, IDC_STATIC_15,	m_static_rev_clamp2);	
	DDX_Control(pDX, IDC_STATIC_16,	m_static_rev_dec1);	
	DDX_Control(pDX, IDC_STATIC_17,	m_static_rev_dec2);	
	DDX_Control(pDX, IDC_STATIC_18,	m_static_rev_revChk1);
	DDX_Control(pDX, IDC_STATIC_19,	m_static_rev_revChk2);

	DDX_Text(pDX, IDC_EDIT_DOT_D, m_dDotRadius);
	DDV_MinMaxDouble(pDX, m_dDotRadius, 0, 4);
	DDX_Text(pDX, IDC_EDIT_AUTO_RPS_DOTS, m_nAutoRpsDots);
	DDV_MinMaxInt(pDX, m_nAutoRpsDots, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_AUTO_RPS_INV, m_dAutoRpsInterval);
	DDV_MinMaxDouble(pDX, m_dAutoRpsInterval, 0, 99999);
	DDX_Text(pDX, IDC_EDIT_CLEAN_INV, m_nCleanInerval);
}


BEGIN_MESSAGE_MAP(CDlgTrack, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ASK_PRE, &CDlgTrack::OnBnClickedBtnAskPre)
	ON_BN_CLICKED(IDC_BTN_ASK_NEXT, &CDlgTrack::OnBnClickedBtnAskNext)
	ON_BN_CLICKED(IDC_BTN_STOP_11, &CDlgTrack::OnBnClickedBtnStop11)
	ON_BN_CLICKED(IDC_BTN_STOP_12, &CDlgTrack::OnBnClickedBtnStop12)
	ON_BN_CLICKED(IDC_BTN_LIFT, &CDlgTrack::OnBnClickedBtnLift)
	ON_BN_CLICKED(IDC_BTN_CLAMP, &CDlgTrack::OnBnClickedBtnClamp1)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CDlgTrack::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_FIX, &CDlgTrack::OnBnClickedBtnFix)
	ON_BN_CLICKED(IDC_BTN_UNFIX, &CDlgTrack::OnBnClickedBtnUnfix)
	ON_BN_CLICKED(IDC_BTN_UNLOAD, &CDlgTrack::OnBnClickedBtnUnload)
	ON_BN_CLICKED(IDC_BTN_TRACK_L, &CDlgTrack::OnBnClickedBtnTrackL)
	ON_BN_CLICKED(IDC_BTN_TARCK_R, &CDlgTrack::OnBnClickedBtnTarckR)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgTrack::OnBnClickedRadioAdd)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgTrack::OnBnClickedRadioAdd)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgTrack::OnBnClickedRadioAdd)
	ON_BN_CLICKED(IDC_RADIO4, &CDlgTrack::OnBnClickedRadioFix)
	ON_BN_CLICKED(IDC_RADIO5, &CDlgTrack::OnBnClickedRadioFix)
	ON_BN_CLICKED(IDC_RADIO6, &CDlgTrack::OnBnClickedRadioFix)
	ON_BN_CLICKED(IDC_CHK_REV_STOP_1, &CDlgTrack::OnBnClickedChkRevStop1)
	ON_BN_CLICKED(IDC_CHK_REV_STOP_2, &CDlgTrack::OnBnClickedChkRevStop2)
	ON_BN_CLICKED(IDC_CHK_REV_CLAMP, &CDlgTrack::OnBnClickedChkRevClamp)
	ON_BN_CLICKED(IDC_BTN_REV_BOARD, &CDlgTrack::OnBnClickedBtnRevBoard)
	ON_BN_CLICKED(IDC_BTN_WORK2REV, &CDlgTrack::OnBnClickedBtnWork2rev)
	ON_BN_CLICKED(IDC_BTN_REV2WORK, &CDlgTrack::OnBnClickedBtnRev2work)
	ON_BN_CLICKED(IDC_BTN_TRACK_L2, &CDlgTrack::OnBnClickedBtnTrackL2)
	ON_BN_CLICKED(IDC_BTN_TARCK_R2, &CDlgTrack::OnBnClickedBtnTarckR2)
	ON_BN_CLICKED(IDC_BTN_REV_HOME, &CDlgTrack::OnBnClickedBtnRevHome)

	ON_BN_CLICKED(ID_BTN_SAVE, &CDlgTrack::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CDlgTrack 消息处理程序
BOOL CDlgTrack::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
// 	CFont m_FontBC;
// 	m_FontBC.CreateFont(20, // nHeight 
// 		0,					// nWidth 
// 		0,					// nEscapement 
// 		0,					// nOrientation 
// 		FW_BOLD,			// nWeight 
// 		TRUE,				// bItalic 
// 		FALSE,				// bUnderline 
// 		0,					// cStrikeOut 
// 		ANSI_CHARSET,		// nCharSet 
// 		OUT_DEFAULT_PRECIS, // nOutPrecision 
// 		CLIP_DEFAULT_PRECIS,// nClipPrecision 
// 		DEFAULT_QUALITY,	// nQuality 
// 		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
// 		_T("Arail"));	// lpszFac 微软雅黑
	m_hIcon_On = ::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_LED_ON));
	m_hIcon_Off= ::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_LED_OFF));

	g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
	InitCtrl();

	long outputSts;
	g_pFrm->m_Robot->m_pController->GetOutputSts(&outputSts);       // 获取板载输出口状态
	UpdaOutputStatus(outputSts);

	m_btnRadioAdd = g_pFrm->m_pRobotParam->m_nAddOn;
	m_btnRadioFix = g_pFrm->m_pRobotParam->m_nFixMode;
	m_nCleanInerval = g_pFrm->m_pRobotParam->m_structClean.m_nCleanInterval;
	m_nAutoRpsDots = g_pFrm->m_pRobotParam->m_nAutoRpsDots;
	m_dAutoRpsInterval = g_pFrm->m_pRobotParam->m_dAutoRpsIntervalMin;
	m_dDotRadius = g_pFrm->m_pSysParam->dDrawCircleR1;	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgTrack::InitCtrl()	// 初始化控件
{
	m_static_pre.ModifyStyle(0, SS_ICON);	
	m_static_next.ModifyStyle(0, SS_ICON);		
	m_static_11.ModifyStyle(0, SS_ICON);	
	m_static_12.ModifyStyle(0, SS_ICON);	
	m_static_13.ModifyStyle(0, SS_ICON);			
	m_static_14.ModifyStyle(0, SS_ICON);	
	m_static_stop_1.ModifyStyle(0, SS_ICON);
	m_static_stop_2.ModifyStyle(0, SS_ICON);
	m_static_lift_1.ModifyStyle(0, SS_ICON);
	m_static_lift_2.ModifyStyle(0, SS_ICON);
	m_static_clamp.ModifyStyle(0, SS_ICON);

	m_static_pre.SetIcon(m_hIcon_Off);	
	m_static_next.SetIcon(m_hIcon_Off);		
	m_static_11.SetIcon(m_hIcon_Off);		
	m_static_12.SetIcon(m_hIcon_Off);
	m_static_13.SetIcon(m_hIcon_Off);
	m_static_14.SetIcon(m_hIcon_Off);	
	m_static_stop_1.SetIcon(m_hIcon_Off);	
	m_static_stop_2.SetIcon(m_hIcon_Off);
	m_static_lift_1.SetIcon(m_hIcon_Off);
	m_static_lift_2.SetIcon(m_hIcon_Off);
	m_static_clamp.SetIcon(m_hIcon_Off);

	m_btn_ask_pre.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_ask_next.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_stop_1.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_stop_2.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_lift.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_clamp.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_board_load.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_board_fix.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_board_unload.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_board_unfix.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_track_right.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_track_left.SetShade(CShadeButtonST::SHS_METAL);

	m_btn_ask_pre.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
	m_btn_ask_next.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
	m_btn_stop_1.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
	m_btn_stop_2.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
	m_btn_lift.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
	m_btn_clamp.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
	m_btn_board_load.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btn_board_fix.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btn_board_unload.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btn_board_unfix.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
// 	m_btn_board_load.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
// 	m_btn_board_fix.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
// 	m_btn_board_unload.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
// 	m_btn_board_unfix.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
	m_btn_track_right.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
	m_btn_track_left.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);

	//////////////////////////////////////////////////////////////////////////
	//		翻板机
	//////////////////////////////////////////////////////////////////////////
	m_static_rev_stop1.ModifyStyle(0, SS_ICON);
	m_static_rev_stop2.ModifyStyle(0, SS_ICON);
	m_static_rev_clamp1.ModifyStyle(0, SS_ICON);
	m_static_rev_clamp2.ModifyStyle(0, SS_ICON);
	m_static_rev_dec1.ModifyStyle(0, SS_ICON);
	m_static_rev_dec2.ModifyStyle(0, SS_ICON);
	m_static_rev_revChk1.ModifyStyle(0, SS_ICON);
	m_static_rev_revChk2.ModifyStyle(0, SS_ICON);

	m_static_rev_stop1.SetIcon(m_hIcon_Off);	
	m_static_rev_stop2.SetIcon(m_hIcon_Off);	
	m_static_rev_clamp1.SetIcon(m_hIcon_Off);	
	m_static_rev_clamp2.SetIcon(m_hIcon_Off);	
	m_static_rev_dec1.SetIcon(m_hIcon_Off);	
	m_static_rev_dec2.SetIcon(m_hIcon_Off);	
	m_static_rev_revChk1.SetIcon(m_hIcon_Off);	
	m_static_rev_revChk2.SetIcon(m_hIcon_Off);	

	m_btn_rev_home.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_rev_board.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_work2rev.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_rev2work.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_rev_stop1.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_rev_stop2.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_rev_clamp.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_rev_track_l.SetShade(CShadeButtonST::SHS_METAL);
	m_btn_rev_track_r.SetShade(CShadeButtonST::SHS_METAL);

// 	m_btn_rev_board;
// 	m_btn_work2rev;
// 	m_btn_rev2work;
	m_btn_rev_stop1.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
	m_btn_rev_stop2.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
	m_btn_rev_clamp.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
	m_btn_rev_track_l.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
	m_btn_rev_track_r.SetIcon(IDI_ICON_OK, IDI_ICON_CANCEL);
}

void CDlgTrack::UpdaOutputStatus(long outPutStatus)
{
	//////////////////////////////////////////////////////////////////////////
	CString strTemp = "本工位要板信号1";
	int ibit = g_pFrm->m_Robot->ExOutputBitNameToNo(strTemp);
	if((ibit<(OUT_POINT+EX_OUT_POINT))&&(ibit>=OUT_POINT))
	{
		if(g_pFrm->m_Robot->m_OutPutCtrl[ibit].m_IsOpen==FALSE)
			m_btn_ask_pre.SetCheck(0);
		else
			m_btn_ask_pre.SetCheck(1);
	}
	strTemp = "本工位出板信号1";
	ibit = g_pFrm->m_Robot->ExOutputBitNameToNo(strTemp);
	if((ibit<(OUT_POINT+EX_OUT_POINT))&&(ibit>=OUT_POINT))
	{
		if(g_pFrm->m_Robot->m_OutPutCtrl[ibit].m_IsOpen==FALSE)
			m_btn_ask_next.SetCheck(0);
		else
			m_btn_ask_next.SetCheck(1);
	}

	//////////////////////////////////////////////////////////////////////////
	//
// 	if ((outPutStatus&(0x01<<10))==(0x01<<10))
// 		m_btn_stop_1.SetCheck(0);
// 	else
// 		m_btn_stop_1.SetCheck(1);

	if ((outPutStatus&(0x01<<10))==(0x01<<10))
		m_btn_stop_1.SetCheck(0);
	else
		m_btn_stop_1.SetCheck(1);

	if ((outPutStatus&(0x01<<11))==(0x01<<11))
		m_btn_stop_2.SetCheck(0);
	else
		m_btn_stop_2.SetCheck(1);

	if ((outPutStatus&(0x01<<12))==(0x01<<12))
		m_btn_lift.SetCheck(0);
	else
		m_btn_lift.SetCheck(1);

	if ((outPutStatus&(0x01<<13))==(0x01<<13))
		m_btn_clamp.SetCheck(0);
	else
		m_btn_clamp.SetCheck(1);

	if (AddOn_Reverse == g_pFrm->m_pRobotParam->m_nAddOn)
	{
		DWORD outStatus = g_pFrm->m_Robot->m_lsCtrl.ReadDoPort();

		m_btn_rev_stop1.SetCheck((outStatus&1<<smc_do_stop_1) != (1<<smc_do_stop_1));
		m_btn_rev_stop2.SetCheck((outStatus&1<<smc_do_stop_2) != (1<<smc_do_stop_2));
		m_btn_rev_clamp.SetCheck((outStatus&1<<smc_do_clamp_1) != (1<<smc_do_clamp_1));		
	}
}

void CDlgTrack::UpdataIOState(long inPutState, long exInput, long outPutStatus)	// 初始化控件
{
	UNREFERENCED_PARAMETER(outPutStatus);
	//////////////////////////////////////////////////////////////////////////
	// input
	if ((exInput&(0x01<<0))==(0x01<<0))
		m_static_pre.SetIcon(m_hIcon_Off);
	else
		m_static_pre.SetIcon(m_hIcon_On);

	if ((exInput&(0x01<<1))==(0x01<<1))			
		m_static_next.SetIcon(m_hIcon_Off);		
	else										
		m_static_next.SetIcon(m_hIcon_On);		

	if ((inPutState&(0x01<<7))==(0x01<<7))			
		m_static_11.SetIcon(m_hIcon_Off);		
	else										
		m_static_11.SetIcon(m_hIcon_On);		

	if ((inPutState&(0x01<<8))==(0x01<<8))			
		m_static_12.SetIcon(m_hIcon_Off);		
	else
		m_static_12.SetIcon(m_hIcon_On);

	if ((inPutState&(0x01<<9))==(0x01<<9))
		m_static_13.SetIcon(m_hIcon_Off);
	else
		m_static_13.SetIcon(m_hIcon_On);

	if ((inPutState&(0x01<<10))==(0x01<<10))
		m_static_14.SetIcon(m_hIcon_Off);
	else
		m_static_14.SetIcon(m_hIcon_On);

	if ((inPutState&(0x01<<11))==(0x01<<11))
		m_static_stop_1.SetIcon(m_hIcon_Off);
	else
		m_static_stop_1.SetIcon(m_hIcon_On);

	if ((inPutState&(0x01<<12))==(0x01<<12))
		m_static_stop_2.SetIcon(m_hIcon_Off);
	else
		m_static_stop_2.SetIcon(m_hIcon_On);

	if ((inPutState&(0x01<<13))==(0x01<<13))
		m_static_lift_1.SetIcon(m_hIcon_Off);
	else
		m_static_lift_1.SetIcon(m_hIcon_On);

	if ((inPutState&(0x01<<14))==(0x01<<14))
		m_static_lift_2.SetIcon(m_hIcon_Off);
	else
		m_static_lift_2.SetIcon(m_hIcon_On);

	if ((inPutState&(0x01<<15))==(0x01<<15))
		m_static_clamp.SetIcon(m_hIcon_Off);
	else
		m_static_clamp.SetIcon(m_hIcon_On);

	//////////////////////////////////////////////////////////////////////////
	// 翻板机
	//////////////////////////////////////////////////////////////////////////
	if (AddOn_Reverse == g_pFrm->m_pRobotParam->m_nAddOn)
	{
		DWORD inpStatus = g_pFrm->m_Robot->m_lsCtrl.ReadDiPort();

		if ((inpStatus&1<<smc_di_decSpd_1) == (1<<smc_di_decSpd_1))
			m_static_rev_dec1.SetIcon(m_hIcon_Off);
		else
			m_static_rev_dec1.SetIcon(m_hIcon_On);

		if ((inpStatus&1<<smc_di_decSpd_2) == (1<<smc_di_decSpd_2))
			m_static_rev_dec2.SetIcon(m_hIcon_Off);
		else
			m_static_rev_dec2.SetIcon(m_hIcon_On);

		if ((inpStatus&1<<smc_di_revChk_1) == (1<<smc_di_revChk_1))
			m_static_rev_revChk1.SetIcon(m_hIcon_Off);
		else
			m_static_rev_revChk1.SetIcon(m_hIcon_On);

		if ((inpStatus&1<<smc_di_revChk_2) == (1<<smc_di_revChk_2))
			m_static_rev_revChk2.SetIcon(m_hIcon_Off);
		else
			m_static_rev_revChk2.SetIcon(m_hIcon_On);

		if ((inpStatus&1<<smc_di_stopChk_1) == (1<<smc_di_stopChk_1))
			m_static_rev_stop1.SetIcon(m_hIcon_Off);
		else
			m_static_rev_stop1.SetIcon(m_hIcon_On);

		if ((inpStatus&1<<smc_di_stopChk_2) == (1<<smc_di_stopChk_2))
			m_static_rev_stop2.SetIcon(m_hIcon_Off);
		else
			m_static_rev_stop2.SetIcon(m_hIcon_On);

		if ((inpStatus&1<<smc_di_clampChk_1) == (1<<smc_di_clampChk_1))
			m_static_rev_clamp1.SetIcon(m_hIcon_Off);
		else
			m_static_rev_clamp1.SetIcon(m_hIcon_On);

		if ((inpStatus&1<<smc_di_clampChk_2) == (1<<smc_di_clampChk_2))
			m_static_rev_clamp2.SetIcon(m_hIcon_Off);
		else
			m_static_rev_clamp2.SetIcon(m_hIcon_On);
	}
}

void CDlgTrack::OnBnClickedBtnAskPre()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp = "本工位要板信号1";
	int ibit = g_pFrm->m_Robot->ExOutputBitNameToNo(strTemp);
	if((ibit<(OUT_POINT+EX_OUT_POINT))&&(ibit>=OUT_POINT))
	{
		if(g_pFrm->m_Robot->m_OutPutCtrl[ibit].m_IsOpen==FALSE)
		{
			short iRet = g_pFrm->m_Robot->ExOutput(strTemp,TRUE);
			if (iRet != RTN_NONE)
			{
				AfxMessageBox("打开端口出错!");
			}
		}
		else
		{
			short iRet = g_pFrm->m_Robot->ExOutput(strTemp,FALSE);
			if (iRet != RTN_NONE)
			{
				AfxMessageBox("关闭端口出错!");
			}
		}
	}
}

void CDlgTrack::OnBnClickedBtnAskNext()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp = "本工位出板信号1";
	int ibit = g_pFrm->m_Robot->ExOutputBitNameToNo(strTemp);
	if((ibit<(OUT_POINT+EX_OUT_POINT))&&(ibit>=OUT_POINT))
	{
		if(g_pFrm->m_Robot->m_OutPutCtrl[ibit].m_IsOpen==FALSE)
		{
			short iRet = g_pFrm->m_Robot->ExOutput(strTemp,TRUE);
			if (iRet != RTN_NONE)
			{
				AfxMessageBox("打开端口出错!");
			}
		}
		else
		{
			short iRet = g_pFrm->m_Robot->ExOutput(strTemp,FALSE);
			if (iRet != RTN_NONE)
			{
				AfxMessageBox("关闭端口出错!");
			}
		}
	}
}


void CDlgTrack::OnBnClickedBtnStop11()
{
	// TODO: 在此添加控件通知处理程序代码
	short iRet = g_pFrm->m_Robot->ExOutput("11号阻挡气缸电磁阀", m_btn_stop_1.GetCheck());
	if (iRet != RTN_NONE)
	{
		AfxMessageBox("关闭端口出错!");
		return ;
	}
}


void CDlgTrack::OnBnClickedBtnStop12()
{
	// TODO: 在此添加控件通知处理程序代码
	short iRet = g_pFrm->m_Robot->ExOutput("12号阻挡气缸电磁阀", m_btn_stop_2.GetCheck());
	if (iRet != RTN_NONE)
	{
		AfxMessageBox("关闭端口出错!");
		return ;
	}
}


void CDlgTrack::OnBnClickedBtnLift()
{
	// TODO: 在此添加控件通知处理程序代码
	short iRet = g_pFrm->m_Robot->ExOutput("11号顶升气缸电磁阀", m_btn_lift.GetCheck());
	if (iRet != RTN_NONE)
	{
		AfxMessageBox("关闭端口出错!");
		return ;
	}
}


void CDlgTrack::OnBnClickedBtnClamp1()
{
	// TODO: 在此添加控件通知处理程序代码
	short iRet = g_pFrm->m_Robot->ExOutput("11号侧夹气缸电磁阀", m_btn_clamp.GetCheck());
	if (iRet != RTN_NONE)
	{
		AfxMessageBox("关闭端口出错!");
		return ;
	}
}


void CDlgTrack::OnBnClickedBtnLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_pFrm->m_Robot->m_bLoadFinish)
	{
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetLoadStatus("进板流程中");
		return;
	}

	g_pFrm->m_Robot->m_nLoadStep = 1;
	g_pFrm->m_Robot->LoadBoard();
}


void CDlgTrack::OnBnClickedBtnFix()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->FixBoard();
}


void CDlgTrack::OnBnClickedBtnUnfix()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->UnFixBoard();
}


void CDlgTrack::OnBnClickedBtnUnload()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->UnloadBoard();
}


void CDlgTrack::OnBnClickedBtnTrackL()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btn_track_right.SetCheck(0);
	if (!m_btn_track_left.GetCheck())
	{
		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
	}
	else
	{
		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, -g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
	}
// 	long axisStatus;
// 	g_pFrm->m_Robot->m_pController->GetAxisSts(T1_AXIS+1,&axisStatus);
// 	if (axisStatus & 0x400)
}


void CDlgTrack::OnBnClickedBtnTarckR()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btn_track_left.SetCheck(0);
	if (!m_btn_track_right.GetCheck())
	{
		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
	}
	else
	{
		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
	}
// 	long axisStatus;
// 	g_pFrm->m_Robot->m_pController->GetAxisSts(T1_AXIS+1,&axisStatus);
// 	if (axisStatus & 0x400)
// 	{
// 		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
// 	}
// 	else
// 	{
// 		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParam[T1_AXIS].m_Vmax, g_pFrm->m_mtrParam[T1_AXIS].m_Acc);
// 	}
}


void CDlgTrack::OnBnClickedRadioAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_btnRadioAdd > 2)
	{
		return;
	}

	CString strFile = CFunction::GetDirectory() + "system\\robotparam.dat";
	// 外挂辅助配置
	g_pFrm->m_pRobotParam->m_nAddOn = m_btnRadioAdd;
	CFunction::HandleInt(FALSE, "AddOn", "AddOn", g_pFrm->m_pRobotParam->m_nAddOn, strFile);

	EnableReverseBtn(AddOn_Reverse==m_btnRadioAdd);
}


void CDlgTrack::OnBnClickedRadioFix()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_btnRadioFix > 2)
	{
		return;
	}

	CString strFile = CFunction::GetDirectory() + "system\\robotparam.dat";
	// 外挂辅助配置
	g_pFrm->m_pRobotParam->m_nFixMode = m_btnRadioFix;
	CFunction::HandleInt(FALSE, "FixMode", "FixMode", g_pFrm->m_pRobotParam->m_nFixMode, strFile);
}

void CDlgTrack::OnBnClickedChkRevStop1()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->m_lsCtrl.SetDoBit(smc_do_stop_1, m_btn_rev_stop1.GetCheck());
}


void CDlgTrack::OnBnClickedChkRevStop2()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->m_lsCtrl.SetDoBit(smc_do_stop_2, m_btn_rev_stop2.GetCheck());
}


void CDlgTrack::OnBnClickedChkRevClamp()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->m_lsCtrl.SetDoBit(smc_do_clamp_1, m_btn_rev_clamp.GetCheck());
}


void CDlgTrack::OnBnClickedBtnRevBoard()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->m_lsCtrl.RevBoard();
}


void CDlgTrack::OnBnClickedBtnWork2rev()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bOn[4];
	g_pFrm->m_Robot->ExInput(("11号光开"), bOn[0]);
	g_pFrm->m_Robot->ExInput(("12号光开"), bOn[1]);
	g_pFrm->m_Robot->ExInput(("13号光开"), bOn[2]);
	g_pFrm->m_Robot->ExInput(("14号光开"), bOn[3]);
	if (bOn[0] || bOn[1] || bOn[2] || bOn[3])
	{
		g_pFrm->m_Robot->ExOutput("11号顶升气缸电磁阀", FALSE);
		g_pFrm->m_Robot->ExOutput("12号阻挡气缸电磁阀", FALSE);
		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
	
		CKFTimer tmTemp;
		tmTemp.TimerStart();
		while (1)
		{
			g_pFrm->m_Robot->ExInput("14号光开", bOn[0]);
			if (bOn[0])
			{
				g_pFrm->m_Robot->m_lsCtrl.TrackToBack();
				break;
			}
			if (tmTemp.TimerGetSecond() > 2.0)
			{
				AfxMessageBox("点胶机到翻板机时，在点胶工位超时！");
				g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
				return ;
			}
		}
		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
	}
	else
	{
		AfxMessageBox("点胶工位没有板");
		return;
	}
}


void CDlgTrack::OnBnClickedBtnRev2work()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bDiStatus = FALSE;
	CKFTimer tmTemp;
	g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, -g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);

	if (g_pFrm->m_Robot->m_lsCtrl.BackToTrack())
	{
		tmTemp.TimerStart();
		while (1)
		{
			g_pFrm->m_Robot->ExInput("13号光开", bDiStatus);
			if (bDiStatus)
			{
				g_pFrm->m_Robot->m_lsCtrl.AxisStop(smc_mtr_track);
				break;
			}
			if (tmTemp.TimerGetSecond() > 6.0)
			{
				g_pFrm->m_Robot->m_lsCtrl.AxisStop(smc_mtr_track);
				g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
				AfxMessageBox("翻板机到点胶位卡板!");
				return;
			}
		}
		tmTemp.TimerStart();
		while (1)
		{
			g_pFrm->m_Robot->ExInput("13号光开", bDiStatus);
			if (!bDiStatus)
			{
				break;
			}
			if (tmTemp.TimerGetSecond() > 4.0)
			{
				AfxMessageBox("翻板机到点胶位时，在点胶工位卡板");
			}
		}

		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, -g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax/4, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
		g_pFrm->m_Robot->ExOutput("12号阻挡气缸电磁阀", TRUE);
		tmTemp.TimerStart();
		while (tmTemp.TimerGetSecond() < 2.0)
		{
			g_pFrm->m_Robot->ExInput("12号阻挡气缸检测", bDiStatus);
			if (bDiStatus)
			{
				break;
			}
		}
		g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax/2, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);

		tmTemp.TimerStart();
		while (tmTemp.TimerGetSecond() < 2.0)
		{
			g_pFrm->m_Robot->ExInput("13号光开", bDiStatus);
			if (bDiStatus)
			{
				CFunction::DelaySec(0.2);
				g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
				break;
			}
		}
	}
}


void CDlgTrack::OnBnClickedBtnTrackL2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btn_rev_track_r.SetCheck(0);
	if (m_btn_rev_track_l.GetCheck())
	{
		g_pFrm->m_Robot->m_lsCtrl.AxisMoveJog(smc_mtr_track, -2000);
	}
	else
	{
		g_pFrm->m_Robot->m_lsCtrl.AxisStop(smc_mtr_track);
	}
}


void CDlgTrack::OnBnClickedBtnTarckR2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btn_rev_track_l.SetCheck(0);
	if (m_btn_rev_track_r.GetCheck())
	{
		g_pFrm->m_Robot->m_lsCtrl.AxisMoveJog(smc_mtr_track, 2000);
	}
	else
	{
		g_pFrm->m_Robot->m_lsCtrl.AxisStop(smc_mtr_track);
	}
}


void CDlgTrack::OnBnClickedBtnRevHome()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->m_lsCtrl.Home(smc_mtr_rev);
}

void CDlgTrack::EnableReverseBtn(BOOL bEnable)
{
	m_btn_rev_home.EnableWindow(bEnable);
	m_btn_rev_board.EnableWindow(bEnable);
	m_btn_work2rev.EnableWindow(bEnable);
	m_btn_rev2work.EnableWindow(bEnable);
	m_btn_rev_stop1.EnableWindow(bEnable);
	m_btn_rev_stop2.EnableWindow(bEnable);
	m_btn_rev_clamp.EnableWindow(bEnable);
	m_btn_rev_track_l.EnableWindow(bEnable);
	m_btn_rev_track_r.EnableWindow(bEnable);
}

void CDlgTrack::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g_pFrm->m_pSysParam->dDrawCircleR1 = m_dDotRadius;	
	
	g_pFrm->m_pRobotParam->m_structClean.m_nCleanInterval =	m_nCleanInerval	;
	g_pFrm->m_pRobotParam->m_nAutoRpsDots			=	m_nAutoRpsDots	;
	g_pFrm->m_pRobotParam->m_dAutoRpsIntervalMin	=	m_dAutoRpsInterval;
	g_pFrm->RobotParam(FALSE);
}
