// DlgDualValve.cpp : 实现文件
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgDualValve.h"
#include "afxdialogex.h"


// CDlgDualValve 对话框

IMPLEMENT_DYNAMIC(CDlgDualValve, CDialogEx)

CDlgDualValve::CDlgDualValve(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDualValve::IDD, pParent)
{
	m_dRotateMarkX1 = 0.0;
	m_dRotateMarkX2 = 0.0;
	m_dRotateMarkY1 = 0.0;
	m_dRotateMarkY2 = 0.0;
	m_dValveSpace = 0.0;
	m_dValvePosX1 = 0.0;
	m_dValvePosY1 = 0.0;

	m_pImageBuffer = NULL;
	MilImage = M_NULL;
	MilDisplay = M_NULL;   
	m_dNeedleRad = 100.0;
	m_dZStep = 0.1;
	m_dZCurPos = 0.0;
}					 

CDlgDualValve::~CDlgDualValve()
{
}

void CDlgDualValve::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Rotate_Mark_x1, m_dRotateMarkX1);
	DDX_Text(pDX, IDC_EDIT_Rotate_Mark_x2, m_dRotateMarkX2);
	DDX_Text(pDX, IDC_EDIT_Rotate_Mark_y1, m_dRotateMarkY1);
	DDX_Text(pDX, IDC_EDIT_Rotate_Mark_y2, m_dRotateMarkY2);
	DDX_Text(pDX, IDC_EDIT_Valve_Space, m_dValveSpace);
	DDX_Text(pDX, IDC_EDIT_Valve_x1, m_dValvePosX1);
	DDX_Text(pDX, IDC_EDIT_Valve_y1, m_dValvePosY1);
	DDX_Text(pDX, IDC_EDIT_Valve_z1, m_dValvePosZ1);
	DDX_Text(pDX, IDC_EDIT_Needle_Rad, m_dNeedleRad);
	DDX_Text(pDX, IDC_EDIT_Z_Step, m_dZStep);
	DDV_MinMaxDouble(pDX, m_dZStep, 0.001, 1);
	DDX_Control(pDX, IDC_STATIC_ToolDetection, m_staicToolDetection);
	DDX_Text(pDX, IDC_EDIT_Z_CurPos, m_dZCurPos);
}


BEGIN_MESSAGE_MAP(CDlgDualValve, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_Teach_Valve_1, &CDlgDualValve::OnBnClickedBtnTeachValve1)
	ON_BN_CLICKED(IDC_BTN_Move_Valve_1, &CDlgDualValve::OnBnClickedBtnMoveValve1)
	ON_BN_CLICKED(IDC_BTN_Teach_Rotate_1, &CDlgDualValve::OnBnClickedBtnTeachRotate1)
	ON_BN_CLICKED(IDC_BTN_Teach_Rotate_2, &CDlgDualValve::OnBnClickedBtnTeachRotate2)
	ON_BN_CLICKED(IDC_BTN_Move_Rotate_1, &CDlgDualValve::OnBnClickedBtnMoveRotate1)
	ON_BN_CLICKED(IDC_BTN_Move_Rotate_2, &CDlgDualValve::OnBnClickedBtnMoveRotate2)
	ON_BN_CLICKED(IDC_BTN_Rotate, &CDlgDualValve::OnBnClickedBtnRotate)
	ON_BN_CLICKED(IDC_BTN_Move_Valve_2, &CDlgDualValve::OnBnClickedBtnMoveValve2)
	ON_BN_CLICKED(IDOK, &CDlgDualValve::OnBnClickedOk)

	ON_MESSAGE(WM_REDRAWIMAGEFROMCCD, &CDlgDualValve::OnMessageRedrawCCDEvent)
	ON_EN_CHANGE(IDC_EDIT_Needle_Rad, &CDlgDualValve::OnEnChangeEditNeedleRad)
	ON_EN_CHANGE(IDC_EDIT_Z_Step, &CDlgDualValve::OnEnChangeEditZStep)
	ON_BN_CLICKED(IDC_BTN_Move_Valve_TD2, &CDlgDualValve::OnBnClickedBtnMoveValveTd2)
	ON_BN_CLICKED(IDC_BTN_Move_Valve_TD1, &CDlgDualValve::OnBnClickedBtnMoveValveTd1)
	ON_BN_CLICKED(IDC_BTN_ZDown, &CDlgDualValve::OnBnClickedBtnZdown)
	ON_BN_CLICKED(IDC_BTN_ZUp, &CDlgDualValve::OnBnClickedBtnZup)
	ON_BN_CLICKED(IDC_Save, &CDlgDualValve::OnBnClickedSave)
END_MESSAGE_MAP()


// CDlgDualValve 消息处理程序


BOOL CDlgDualValve::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_hIcon_On = ::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_LED_ON));
	m_hIcon_Off= ::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_LED_OFF));
	m_staicToolDetection.ModifyStyle(0, SS_ICON);
	m_staicToolDetection.SetIcon(m_hIcon_Off);


	InitBottomCamera();
	UpdateCtrl(true);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDualValve::UpdateCtrl(bool bShow)
{
	CString strTemp;
	if (bShow)
	{
		m_dRotateMarkX1 = g_pFrm->m_pRobotParam->m_posRotateMark1.x;
		m_dRotateMarkY1 = g_pFrm->m_pRobotParam->m_posRotateMark1.y;
		m_dRotateMarkX2 = g_pFrm->m_pRobotParam->m_posRotateMark2.x;
		m_dRotateMarkY2 = g_pFrm->m_pRobotParam->m_posRotateMark2.y;
		m_dValveSpace = g_pFrm->m_pRobotParam->m_dValveSpace;
		m_dValvePosX1 = g_pFrm->m_pRobotParam->m_posCCDValve1.x;
		m_dValvePosY1 = g_pFrm->m_pRobotParam->m_posCCDValve1.y;
		m_dValvePosZ1 = g_pFrm->m_pRobotParam->m_posCCDValve1.z;

		UpdateData(FALSE);
	}
	else
	{
		UpdateData(TRUE);

		g_pFrm->m_pRobotParam->m_posRotateMark1.x = 	m_dRotateMarkX1	;
		g_pFrm->m_pRobotParam->m_posRotateMark1.y = 	m_dRotateMarkY1	;
		g_pFrm->m_pRobotParam->m_posRotateMark2.x = 	m_dRotateMarkX2	;
		g_pFrm->m_pRobotParam->m_posRotateMark2.y = 	m_dRotateMarkY2	;
		g_pFrm->m_pRobotParam->m_dValveSpace	  = 	m_dValveSpace  ;
		g_pFrm->m_pRobotParam->m_posCCDValve1.x	  = 	m_dValvePosX1  ;
		g_pFrm->m_pRobotParam->m_posCCDValve1.y	  = 	m_dValvePosY1  ;
		g_pFrm->m_pRobotParam->m_posCCDValve1.z	  = 	m_dValvePosZ1  ;
	}
}


void CDlgDualValve::OnBnClickedBtnTeachValve1()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Precision.GetCurPosMM();
	g_pFrm->m_pRobotParam->m_posCCDValve1.x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pRobotParam->m_posCCDValve1.y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	g_pFrm->m_pRobotParam->m_posCCDValve1.z = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	UpdateCtrl(true);
}


void CDlgDualValve::OnBnClickedBtnMoveValve1()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->GotoSafeZpos();
	double x = g_pFrm->m_pRobotParam->m_posCCDValve1.x,
		y = g_pFrm->m_pRobotParam->m_posCCDValve1.y,
		z = g_pFrm->m_pRobotParam->m_posCCDValve1.z;
	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, x, y, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	Sleep(100);

	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc);
	Sleep(100);
}


void CDlgDualValve::OnBnClickedBtnTeachRotate1()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Precision.GetCurPosMM();
	g_pFrm->m_pRobotParam->m_posRotateMark1.x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pRobotParam->m_posRotateMark1.y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	UpdateCtrl(true);
}


void CDlgDualValve::OnBnClickedBtnTeachRotate2()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Precision.GetCurPosMM();
	g_pFrm->m_pRobotParam->m_posRotateMark2.x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pRobotParam->m_posRotateMark2.y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	UpdateCtrl(true);
}


void CDlgDualValve::OnBnClickedBtnMoveRotate1()
{
	// TODO: 在此添加控件通知处理程序代码	
	g_pFrm->m_Robot->GotoSafeZpos();
	double x = g_pFrm->m_pRobotParam->m_posRotateMark1.x,
		y = g_pFrm->m_pRobotParam->m_posRotateMark1.y;
	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, x, y, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	Sleep(100);
}


void CDlgDualValve::OnBnClickedBtnMoveRotate2()
{
	// TODO: 在此添加控件通知处理程序代码	
	g_pFrm->m_Robot->GotoSafeZpos();
	double x = g_pFrm->m_pRobotParam->m_posRotateMark1.x,
		y = g_pFrm->m_pRobotParam->m_posRotateMark1.y;
	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, x, y, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	Sleep(100);
}


void CDlgDualValve::OnBnClickedBtnRotate()
{
	// TODO: 在此添加控件通知处理程序代码
	double dY = g_pFrm->m_pRobotParam->m_posRotateMark1.y - g_pFrm->m_pRobotParam->m_posRotateMark2.y;
	double dX = g_pFrm->m_pRobotParam->m_posRotateMark1.x - g_pFrm->m_pRobotParam->m_posRotateMark2.x;

	double dAngle = atan2(dY, dX);
	dAngle = dAngle*180/PI;
	if (dAngle > 90)
		dAngle -= 360;
	else if (dAngle < -90)
		dAngle += 360;

	g_pFrm->m_Robot->m_pController->AxisMove(R_AXIS, dAngle, 1, 0.1);
}


void CDlgDualValve::OnBnClickedBtnMoveValve2()
{
	// TODO: 在此添加控件通知处理程序代码	
	g_pFrm->m_Robot->GotoSafeZpos();
	double x = g_pFrm->m_pRobotParam->m_posCCDValve1.x - g_pFrm->m_pRobotParam->m_dValveSpace,
		y = g_pFrm->m_pRobotParam->m_posCCDValve1.y,
		z = g_pFrm->m_pRobotParam->m_posCCDValve1.z;
	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, x, y, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	Sleep(100);

	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc);
	Sleep(100);
}


void CDlgDualValve::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateCtrl(false);

	CDialogEx::OnOK();
}


void CDlgDualValve::PaintShape(CRect rect)
{	
	if(g_pFrm == NULL)
		return;
	
	/* Clear the overlay image. */
	MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

// 	CString strTemp;
// 	CPoint point;
// 	double dmm;
// 	CPoint t;

	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0));
	
	MgraRect(M_DEFAULT, MilOverlayImage, rect.left, rect.top, rect.right, rect.bottom);
	MgraLine(M_DEFAULT, MilOverlayImage, 0, rect.CenterPoint().y, rect.right, rect.CenterPoint().y);
	MgraLine(M_DEFAULT, MilOverlayImage, rect.CenterPoint().x, 0, rect.CenterPoint().x, rect.bottom);

	MgraArc(M_DEFAULT, MilOverlayImage, rect.CenterPoint().x, rect.CenterPoint().y, (long)m_dNeedleRad, (long)m_dNeedleRad, 0, 360);
}

int CDlgDualValve::InitBottomCamera()
{
	/* Allocate a MIL display. */
	MdispAlloc(g_pView->MilSystem, M_DEFAULT, "M_DEFAULT", M_WINDOWED, &MilDisplay);

	MbufAlloc2d(g_pView->MilSystem, IMG_WIDTH, IMG_HEIGHT,M_DEF_IMAGE_TYPE,M_IMAGE+M_DISP+M_PROC,&MilImage);
	MbufClear(MilImage, 128);

	MdispSelectWindow(MilDisplay, MilImage, GetDlgItem(IDC_STATIC_Image)->GetSafeHwnd());
	/* Enable the display of overlay annotations. */
	MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
	/* Inquire the overlay buffer associated with the display. */
	MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
	/* Clear the overlay to transparent. */
	MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
	MdispZoom(MilDisplay, 0.5, 0.5);

	PaintShape(CRect(0, 0, IMG_WIDTH, IMG_HEIGHT));
	BOOL bRtn = m_camBottomHK.OpenCamera("相机2", GetSafeHwnd(), m_pImageBuffer);
	return bRtn ? 1:0;
}

int CDlgDualValve::FreeMilResource()
{
	m_camBottomHK.CloseCamera();
	CFunction::DelaySec(0.5);
	if (M_NULL != MilImage) {
		MbufFree(MilImage);
		MilImage = M_NULL;
	}
	if (M_NULL != MilDisplay) {
		MdispFree(MilDisplay);
		MilDisplay = M_NULL;
	}

	return 1;
}

LRESULT CDlgDualValve::OnMessageRedrawCCDEvent(WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(wparam);
	UNREFERENCED_PARAMETER(lparam);

	if (!IsWindowVisible() || !GetSafeHwnd())		return 0;
	if (NULL == m_pImageBuffer)						return 0;
	if (M_NULL == MilImage)							return 0;

	MbufPut(MilImage, m_pImageBuffer);
	GetDlgItem(IDC_STATIC_Image)->Invalidate();

	return 0;
}

void CDlgDualValve::OnEnChangeEditNeedleRad()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	PaintShape(CRect(0, 0, IMG_WIDTH, IMG_HEIGHT));
}


void CDlgDualValve::OnEnChangeEditZStep()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}


void CDlgDualValve::OnBnClickedBtnMoveValveTd2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g_pFrm->m_Robot->GotoSafeZpos();
	if (IDYES != g_pFrm->m_Robot->AfxMessageBox(_T("进行双阀高度调整前，请确保已经测试过阀的高度"), MB_YESNO))
	{
		return;
	}

	double xPos, yPos, zPos;
	xPos = g_pFrm->m_pRobotParam->m_posContactTest.x;
	yPos = g_pFrm->m_pRobotParam->m_posContactTest.y;
	xPos -= m_dValveSpace;
	zPos = g_pFrm->m_pRobotParam->m_zContactPos;

	g_pFrm->m_Robot->m_pController->LineMoveXY(CRD1, 0, xPos, yPos, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, 10);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, zPos, g_pFrm->m_mtrParamGts[Z_AXIS].m_VhandMove, 10);
}


void CDlgDualValve::OnBnClickedBtnMoveValveTd1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g_pFrm->m_Robot->GotoSafeZpos();
	if (IDYES != g_pFrm->m_Robot->AfxMessageBox(_T("进行双阀高度调整前，请确保已经测试过阀的高度"), MB_YESNO))
	{
		return;
	}

	double xPos, yPos, zPos;
	xPos = g_pFrm->m_pRobotParam->m_posContactTest.x;
	yPos = g_pFrm->m_pRobotParam->m_posContactTest.y;
	zPos = g_pFrm->m_pRobotParam->m_zContactPos;

	g_pFrm->m_Robot->m_pController->LineMoveXY(CRD1, 0, xPos, yPos, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, 10);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, zPos, g_pFrm->m_mtrParamGts[Z_AXIS].m_VhandMove, 10);
}


void CDlgDualValve::OnBnClickedBtnZdown()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_dZStep<0 || m_dZStep>1)
	{
		AfxMessageBox("微调时Z轴步长应在0~1之间");
		return;
	}

	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, m_dZCurPos+m_dZStep, g_pFrm->m_mtrParamGts[Z_AXIS].m_VhandMove, 10);
}


void CDlgDualValve::OnBnClickedBtnZup()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_dZStep<0 || m_dZStep>1)
	{
		AfxMessageBox("微调时Z轴步长应在0~1之间");
		return;
	}

	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS, m_dZCurPos-m_dZStep, g_pFrm->m_mtrParamGts[Z_AXIS].m_VhandMove, 10);
}

void CDlgDualValve::UpdateInputStatus(int inport)
{
	if ((1<<g_pFrm->m_Robot->inSenseCheckZ) == (inport&(1<<g_pFrm->m_Robot->inSenseCheckZ)))
		m_staicToolDetection.SetIcon(m_hIcon_Off);
	else
		m_staicToolDetection.SetIcon(m_hIcon_On);
}

void CDlgDualValve::UpdateZPos(double dZPos)
{
	m_dZCurPos = dZPos;
	CString strTemp;
	strTemp.Format("%.3f", dZPos);
	SetDlgItemText(IDC_EDIT_Z_CurPos,strTemp);
}

void CDlgDualValve::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->RobotParam(FALSE);
}
