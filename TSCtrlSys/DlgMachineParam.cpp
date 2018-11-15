// DlgMachineParam.cpp : 实现文件
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgMachineParam.h"
#include "afxdialogex.h"


// CDlgMachineParam 对话框

IMPLEMENT_DYNAMIC(CDlgMachineParam, CDialogEx)

CDlgMachineParam::CDlgMachineParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMachineParam::IDD, pParent)
{
	m_nRadioCamera = 0;
	m_nRadioHeight = 0;
	m_nRadioLdUld = 0;
	m_nRadioWorkMode = 0;
	m_nAutoClean = 0;
	m_nAutoRps = 0;
	m_nAutoRpsDots = 0;
	m_dAutoRpsIntv = 0.0;
	m_dCleanDownMM = 0.0;
	m_nCleanIntv = 0;
	m_nCleanNum = 0;
	m_dImageDotD = 0.0;
	m_nIfGlueWarning = 0;
	m_dProbeHSpd = 0.0;
	m_dProbeLSpd = 0.0;
	m_dProbeLHeight = 0.0;
	m_nTestLen = 0;
	m_nTestSpace = 0;
	m_nIfAutoTest = 0;
	m_dTestVel = 0.0;
	m_nIfSafeDoor = 0;
	m_nRadioGlueWarningMode = 0;
	m_nGlueWarningMin = 0;
	m_nGlueWarningPcs = 0;
	m_nIfUseCameraNeedle = 0;
	m_nIfRotateWorkpiece = 0;
	m_nValveNum = 0;
}

CDlgMachineParam::~CDlgMachineParam()
{
}

void CDlgMachineParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_CAMERA_DALSA, m_nRadioCamera);
	DDX_Radio(pDX, IDC_RADIO_HEIGHT_LASER, m_nRadioHeight);
	DDX_Radio(pDX, IDC_RADIO_LDULD_ONLINE, m_nRadioLdUld);
	DDX_Radio(pDX, IDC_RADIO_WORKMODE_JET, m_nRadioWorkMode);
	DDX_Check(pDX, IDC_CHECK_AUTO_CLEAN, m_nAutoClean);
	DDX_Check(pDX, IDC_CHECK_AUTO_RPS, m_nAutoRps);
	DDX_Text(pDX, IDC_EDIT_AUTO_RPS_DOTS2, m_nAutoRpsDots);
	DDX_Text(pDX, IDC_EDIT_AUTO_RPS_INV2, m_dAutoRpsIntv);
	DDX_Text(pDX, IDC_EDIT_CLEAN_DOWN_MM, m_dCleanDownMM);
	DDX_Text(pDX, IDC_EDIT_CLEAN_INTERVAL, m_nCleanIntv);
	DDX_Text(pDX, IDC_EDIT_CLEAN_NUM, m_nCleanNum);
	DDX_Text(pDX, IDC_EDIT_DOT_D2, m_dImageDotD);
	DDX_Check(pDX, IDC_CHK_GLUE_WARNING, m_nIfGlueWarning);
	DDX_Text(pDX, IDC_EDIT_PROBE_H_SPD, m_dProbeHSpd);
	DDX_Text(pDX, IDC_EDIT_PROBE_L_SPD, m_dProbeLSpd);
	DDX_Text(pDX, IDC_EDIT_PROBE_L_HEIGHT, m_dProbeLHeight);
	DDX_Text(pDX, IDC_EDIT_TEST__LEN, m_nTestLen);
	DDX_Text(pDX, IDC_EDIT_TEST_SPACE, m_nTestSpace);
	DDX_Check(pDX, IDC_CHECK_IF_TESTGLUE, m_nIfAutoTest);
	DDX_Text(pDX, IDC_EDIT_TEST__VEL, m_dTestVel);
	DDX_Control(pDX, IDC_BTN_MOV_BD1, m_btnMov1);
	DDX_Control(pDX, IDC_BTN_MOV_BD2, m_btnMov2);
	DDX_Control(pDX, IDC_BTN_SET_BD1, m_btnSet1);
	DDX_Control(pDX, IDC_BTN_SET_BD2, m_btnSet2);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Check(pDX, IDC_CHECK_SAFEDOOR, m_nIfSafeDoor);
	DDX_Radio(pDX, IDC_RADIO_GLUE_SNR, m_nRadioGlueWarningMode);
	DDX_Text(pDX, IDC_EDIT_GLUE_MINUTE, m_nGlueWarningMin);
	DDX_Text(pDX, IDC_EDIT_GLUE_PCS, m_nGlueWarningPcs);
	DDX_Check(pDX, IDC_CHECK_Camera_Needle, m_nIfUseCameraNeedle);
	DDX_Check(pDX, IDC_CHECK_Workpiece_Rotate, m_nIfRotateWorkpiece);
	DDX_Radio(pDX, IDC_RADIO_VALVE_1, m_nValveNum);
}


BEGIN_MESSAGE_MAP(CDlgMachineParam, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_CAMERA_DALSA, &CDlgMachineParam::OnRadioCamera)
	ON_BN_CLICKED(IDC_RADIO_CAMERA_HK, &CDlgMachineParam::OnRadioCamera)
	ON_BN_CLICKED(IDC_RADIO_HEIGHT_LASER, &CDlgMachineParam::OnRadioHeight)
	ON_BN_CLICKED(IDC_RADIO_HEIGHT_PROBE, &CDlgMachineParam::OnRadioHeight)
	ON_BN_CLICKED(IDC_RADIO_LDULD_ONLINE, &CDlgMachineParam::OnRadioLduld)
	ON_BN_CLICKED(IDC_RADIO_LDULD_OFLINE, &CDlgMachineParam::OnRadioLduld)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgMachineParam::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_CLEAN, &CDlgMachineParam::OnBnClickedBtnClean)
	ON_BN_CLICKED(IDC_BTN_MOV_BD1, &CDlgMachineParam::OnBnClickedBtnMovBd1)
	ON_BN_CLICKED(IDC_BTN_SET_BD1, &CDlgMachineParam::OnBnClickedBtnSetBd1)
	ON_BN_CLICKED(IDC_BTN_MOV_BD2, &CDlgMachineParam::OnBnClickedBtnMovBd2)
	ON_BN_CLICKED(IDC_BTN_SET_BD2, &CDlgMachineParam::OnBnClickedBtnSetBd2)
	ON_BN_CLICKED(IDC_BTN_TEST, &CDlgMachineParam::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_POS_CLEAN, &CDlgMachineParam::OnBnClickedBtnPosClean)
	ON_BN_CLICKED(IDC_BTN_NEW_GLUE, &CDlgMachineParam::OnBnClickedBtnNewGlue)
END_MESSAGE_MAP()


// CDlgMachineParam 消息处理程序


BOOL CDlgMachineParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateCtrl(TRUE);

	m_btnMov1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnMov2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnSet1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnSet2.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnSave.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	m_btnMov1.SetIcon(IDI_ICON_MOVETO);
	m_btnMov2.SetIcon(IDI_ICON_MOVETO);
	m_btnSet1.SetIcon(IDI_ICON_SINGLE_ADJUST);
	m_btnSet2.SetIcon(IDI_ICON_SINGLE_ADJUST);
	m_btnSave.SetIcon(IDI_ICON_SAVE_S);
// 	m_btnMov1.SetFlat(FALSE);
// 	m_btnMov2.SetFlat(FALSE);
// 	m_btnSet1.SetFlat(FALSE);
// 	m_btnSet2.SetFlat(FALSE);
// 	m_btnSave.SetFlat(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgMachineParam::OnRadioCamera()
{
	// TODO: 在此添加命令处理程序代码
}

void CDlgMachineParam::OnRadioHeight()
{
	// TODO: 在此添加命令处理程序代码
}

void CDlgMachineParam::OnRadioLduld()
{
	// TODO: 在此添加命令处理程序代码
}

void CDlgMachineParam::UpdateCtrl(BOOL bShow)
{
	if (bShow)
	{
		m_nRadioCamera = g_pFrm->m_pRobotParam->m_nCameraType;
		m_nRadioHeight = g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode;
		m_nRadioLdUld = g_pFrm->m_pRobotParam->m_nLoadUnloadMode;
		m_nAutoClean = g_pFrm->m_pRobotParam->m_structClean.m_nIfAutoClean;
		m_nAutoRps = g_pFrm->m_pRobotParam->m_nIfAutoRps;
		m_nAutoRpsDots = g_pFrm->m_pRobotParam->m_nAutoRpsDots;
		m_dAutoRpsIntv = g_pFrm->m_pRobotParam->m_dAutoRpsIntervalMin;
		m_dCleanDownMM = g_pFrm->m_pRobotParam->m_structClean.m_dCleanDownMM;
		m_nCleanIntv = g_pFrm->m_pRobotParam->m_structClean.m_nCleanInterval;
		m_nCleanNum = g_pFrm->m_pRobotParam->m_structClean.m_nCleanTimes;
		m_dImageDotD = g_pFrm->m_pSysParam->dDrawCircleR1;
		m_nRadioWorkMode = g_pFrm->m_pRobotParam->m_nWorkMode;

		m_nIfGlueWarning = g_pFrm->m_pRobotParam->m_structGlueWarning.m_nIfGlueWarning;
		m_nRadioGlueWarningMode = g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode;
		m_nGlueWarningPcs = g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningPcs;
		m_nGlueWarningMin = g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMin;

		m_dProbeHSpd = g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeHSpd;
		m_dProbeLSpd = g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeLSpd;
		m_dProbeLHeight = g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeLHeight;

		m_nIfAutoTest = g_pFrm->m_pRobotParam->m_structTestGlue.m_nIfTestGlue;
		m_nTestLen = g_pFrm->m_pRobotParam->m_structTestGlue.m_nGlueLen;
		m_nTestSpace = g_pFrm->m_pRobotParam->m_structTestGlue.m_nGLueSpace;
		m_dTestVel = g_pFrm->m_pRobotParam->m_structTestGlue.m_dWorkVel;

		m_nIfSafeDoor = g_pFrm->m_pRobotParam->m_nIfCheckSafeDoor;

		m_nIfUseCameraNeedle = g_pFrm->m_pRobotParam->m_nIfUseCameraNeedle;
		m_nIfRotateWorkpiece = g_pFrm->m_pRobotParam->m_nIfRotateWorkpiece;
		m_nValveNum = g_pFrm->m_pRobotParam->m_nValveNum;

		UpdateData(FALSE);
	}
	else
	{
		UpdateData(TRUE);

		g_pFrm->m_pRobotParam->m_nCameraType					  = m_nRadioCamera		;
		g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode	  = m_nRadioHeight		;
		g_pFrm->m_pRobotParam->m_nLoadUnloadMode				  = m_nRadioLdUld		;
		g_pFrm->m_pRobotParam->m_structClean.m_nIfAutoClean		  = m_nAutoClean		;
		g_pFrm->m_pRobotParam->m_nIfAutoRps						  = m_nAutoRps			;
		g_pFrm->m_pRobotParam->m_nAutoRpsDots					  = m_nAutoRpsDots		;
		g_pFrm->m_pRobotParam->m_dAutoRpsIntervalMin			  = m_dAutoRpsIntv		;
		g_pFrm->m_pRobotParam->m_structClean.m_dCleanDownMM		  = m_dCleanDownMM		;
		g_pFrm->m_pRobotParam->m_structClean.m_nCleanInterval	  = m_nCleanIntv		;
		g_pFrm->m_pRobotParam->m_structClean.m_nCleanTimes		  = m_nCleanNum			;
		g_pFrm->m_pSysParam->dDrawCircleR1						  = m_dImageDotD		;
		g_pFrm->m_pRobotParam->m_nWorkMode						  = m_nRadioWorkMode 	;

		g_pFrm->m_pRobotParam->m_structGlueWarning.m_nIfGlueWarning	  = m_nIfGlueWarning ;
		g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode = m_nRadioGlueWarningMode;
		g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningPcs  = m_nGlueWarningPcs;
		g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMin  = m_nGlueWarningMin;

		g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeHSpd	  = m_dProbeHSpd		;
		g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeLSpd	  = m_dProbeLSpd		;
		g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeLHeight = m_dProbeLHeight		;

		g_pFrm->m_pRobotParam->m_structTestGlue.m_nIfTestGlue	  = m_nIfAutoTest		;
		g_pFrm->m_pRobotParam->m_structTestGlue.m_nGlueLen	  	  = m_nTestLen			;	 
		g_pFrm->m_pRobotParam->m_structTestGlue.m_nGLueSpace 	  = m_nTestSpace		;
		g_pFrm->m_pRobotParam->m_structTestGlue.m_dWorkVel		  = m_dTestVel			;

		g_pFrm->m_pRobotParam->m_nIfUseCameraNeedle				  = m_nIfUseCameraNeedle;
		g_pFrm->m_pRobotParam->m_nIfRotateWorkpiece				  = m_nIfRotateWorkpiece;
		g_pFrm->m_pRobotParam->m_nValveNum						  = m_nValveNum			;

		g_pFrm->m_pRobotParam->m_nIfCheckSafeDoor				  = m_nIfSafeDoor		;
	}
}

void CDlgMachineParam::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateCtrl(FALSE);

	g_pFrm->RobotParam(FALSE);
	g_pView->PaintShape(CRect(0,0,IMG_WIDTH, IMG_HEIGHT));
	if (2 == g_pFrm->m_pRobotParam->m_nWorkMode)
		g_pFrm->m_Robot->SetRunMode(RunMode_Dry);
	else
		g_pFrm->m_Robot->SetRunMode(RunMode_Wet);
}


void CDlgMachineParam::OnBnClickedBtnClean()
{
	// TODO: 在此添加控件通知处理程序代码
	if(0 == g_pFrm->m_pRobotParam->m_nWorkMode)
	{	// 擦拭式清洗  
		g_pFrm->m_Robot->WashJet(); 
	}
	else if (1 == g_pFrm->m_pRobotParam->m_nWorkMode)
	{	// 单吸式清洗
		g_pFrm->m_Robot->WashNeedle();  
	}
}


void CDlgMachineParam::OnBnClickedBtnMovBd1()
{
	// TODO: 在此添加控件通知处理程序代码
	double dPosX = g_pFrm->m_pRobotParam->m_structTestGlue.m_posTopLeft.x;
	double dPosY = g_pFrm->m_pRobotParam->m_structTestGlue.m_posTopLeft.y;

	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Robot->m_pController->LineMoveXY(CRD1, 0, dPosX, dPosY, 100, 10);
}


void CDlgMachineParam::OnBnClickedBtnSetBd1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK == g_pFrm->m_Robot->AfxMessageBox(_T("是否要更新点位位置"), MB_OKCANCEL))
	{
		g_pFrm->m_Precision.GetCurPosMM();
		g_pFrm->m_pRobotParam->m_structTestGlue.m_posTopLeft.x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
		g_pFrm->m_pRobotParam->m_structTestGlue.m_posTopLeft.y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	}
}


void CDlgMachineParam::OnBnClickedBtnMovBd2()
{
	// TODO: 在此添加控件通知处理程序代码
	double dPosX = g_pFrm->m_pRobotParam->m_structTestGlue.m_posButRight.x;
	double dPosY = g_pFrm->m_pRobotParam->m_structTestGlue.m_posButRight.y;

	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Robot->m_pController->LineMoveXY(CRD1, 0, dPosX, dPosY, 100, 10);
}


void CDlgMachineParam::OnBnClickedBtnSetBd2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK == g_pFrm->m_Robot->AfxMessageBox(_T("是否要更新点位位置"), MB_OKCANCEL))
	{
		g_pFrm->m_Precision.GetCurPosMM();
		g_pFrm->m_pRobotParam->m_structTestGlue.m_posButRight.x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
		g_pFrm->m_pRobotParam->m_structTestGlue.m_posButRight.y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	}
}


void CDlgMachineParam::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->TestDisp();
}


void CDlgMachineParam::OnBnClickedBtnPosClean()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_pRobotParam->m_structTestGlue.m_nCurIndex = 0;
}


void CDlgMachineParam::OnBnClickedBtnNewGlue()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_pRobotParam->m_structGlueWarning.InitGlue();
}

