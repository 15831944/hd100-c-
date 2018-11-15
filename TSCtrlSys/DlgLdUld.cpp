// DlgLdUld.cpp : 实现文件
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgLdUld.h"
#include "afxdialogex.h"


// CDlgLdUld 对话框

IMPLEMENT_DYNAMIC(CDlgLdUld, CDialogEx)

CDlgLdUld::CDlgLdUld(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLdUld::IDD, pParent)
{

	m_nLdLayerCount = 0;
	m_nUldLayerCount = 0;
}

CDlgLdUld::~CDlgLdUld()
{
}

void CDlgLdUld::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_2,	m_static_status[0]);
	DDX_Control(pDX, IDC_STATIC_3,	m_static_status[1]);
	DDX_Control(pDX, IDC_STATIC_5,	m_static_status[2]);
	DDX_Control(pDX, IDC_STATIC_6,	m_static_status[3]);
	DDX_Control(pDX, IDC_STATIC_1,	m_static_status[4]);
	DDX_Control(pDX, IDC_STATIC_4,	m_static_status[5]);	

	DDX_Control(pDX, IDC_COMBO1, m_cmbLdPitch);
	DDX_Control(pDX, IDC_COMBO2, m_cmbUldPitch);

	DDX_Control(pDX, IDC_GRID_LD_POS, m_GridLdPos);
	DDX_Control(pDX, IDC_GRID_ULD_POS, m_GridUldPos);
	DDX_Text(pDX, IDC_EDIT_LD_LAYERCOUNT, m_nLdLayerCount);
	DDV_MinMaxInt(pDX, m_nLdLayerCount, 2, 1000);
	DDX_Text(pDX, IDC_EDIT_ULD_LAYERCOUNT, m_nUldLayerCount);
	DDV_MinMaxInt(pDX, m_nUldLayerCount, 2, 1000);

	DDX_Control(pDX, IDC_BTN_LU, m_btnLoadUp);		
	DDX_Control(pDX, IDC_BTN_LD, m_btnLoadDown);	
	DDX_Control(pDX, IDC_BTN_LBOX_U, m_btnLoadBoxUp);	
	DDX_Control(pDX, IDC_BTN_LBOX_D, m_btnLoadBoxDown);	
	DDX_Control(pDX, IDC_BTN_LMET_U, m_btnLoadLayerUp);	
	DDX_Control(pDX, IDC_BTN_LMET_D, m_btnLoadLayerDown);	

	DDX_Control(pDX, IDC_BTN_ULU, m_btnUnloadUp);	
	DDX_Control(pDX, IDC_BTN_ULD, m_btnUnloadDown);	
	DDX_Control(pDX, IDC_BTN_ULBOX_U, m_btnUnloadBoxUp);	
	DDX_Control(pDX, IDC_BTN_ULBOX_D, m_btnUnloadBoxDown);	
	DDX_Control(pDX, IDC_BTN_ULMET_U, m_btnUnloadLayerUp);	
	DDX_Control(pDX, IDC_BTN_ULMET_D, m_btnUnloadLayerDown);	
	DDX_Control(pDX, IDC_BTN_HOME, m_btnHome);
	DDX_Control(pDX, IDC_BTN_LMOV, m_btnLMov);
	DDX_Control(pDX, IDC_BTN_LSET, m_btnLSet);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BTN_ULMOV, m_btnUlMov);
	DDX_Control(pDX, IDC_BTN_ULSET, m_btnUlSet);
}


BEGIN_MESSAGE_MAP(CDlgLdUld, CDialogEx)
// 	ON_NOTIFY(NM_CLICK, IDC_GRID_LD_POS, OnLdGridClick)
// 	ON_NOTIFY(NM_CLICK, IDC_GRID_ULD_POS, OnUldGridClick)

	ON_BN_CLICKED(IDC_BTN_LOAD1, &CDlgLdUld::OnBnClickedBtnLoad1)
	ON_BN_CLICKED(IDC_BTN_LOAD_OUT, &CDlgLdUld::OnBnClickedBtnLoadOut)
	ON_BN_CLICKED(IDC_BTN_LOAD_BACK, &CDlgLdUld::OnBnClickedBtnLoadBack)
	ON_BN_CLICKED(IDC_BTN_LU, &CDlgLdUld::OnBnClickedBtnLu)
	ON_BN_CLICKED(IDC_BTN_LD, &CDlgLdUld::OnBnClickedBtnLd)
	ON_BN_CLICKED(IDC_BTN_LBOX_U, &CDlgLdUld::OnBnClickedBtnLboxU)
	ON_BN_CLICKED(IDC_BTN_LBOX_D, &CDlgLdUld::OnBnClickedBtnLboxD)
	ON_BN_CLICKED(IDC_BTN_LMET_U, &CDlgLdUld::OnBnClickedBtnLmetU)
	ON_BN_CLICKED(IDC_BTN_LMET_D, &CDlgLdUld::OnBnClickedBtnLmetD)
	ON_BN_CLICKED(IDC_BTN_LMOV, &CDlgLdUld::OnBnClickedBtnLmov)
	ON_BN_CLICKED(IDC_BTN_LSET, &CDlgLdUld::OnBnClickedBtnLset)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgLdUld::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_UNLOAD1, &CDlgLdUld::OnBnClickedBtnUnload1)
	ON_BN_CLICKED(IDC_BTN_UNLOAD_OUT, &CDlgLdUld::OnBnClickedBtnUnloadOut)
	ON_BN_CLICKED(IDC_BTN_UNLOAD_BACK, &CDlgLdUld::OnBnClickedBtnUnloadBack)
	ON_BN_CLICKED(IDC_BTN_ULU, &CDlgLdUld::OnBnClickedBtnUlu)
	ON_BN_CLICKED(IDC_BTN_ULD, &CDlgLdUld::OnBnClickedBtnUld)
	ON_BN_CLICKED(IDC_BTN_ULBOX_U, &CDlgLdUld::OnBnClickedBtnUlboxU)
	ON_BN_CLICKED(IDC_BTN_ULBOX_D, &CDlgLdUld::OnBnClickedBtnUlboxD)
	ON_BN_CLICKED(IDC_BTN_ULMET_U, &CDlgLdUld::OnBnClickedBtnUlmetU)
	ON_BN_CLICKED(IDC_BTN_ULMET_D, &CDlgLdUld::OnBnClickedBtnUlmetD)
	ON_BN_CLICKED(IDC_BTN_ULMOV, &CDlgLdUld::OnBnClickedBtnUlmov)
	ON_BN_CLICKED(IDC_BTN_ULSET, &CDlgLdUld::OnBnClickedBtnUlset)
	ON_BN_CLICKED(IDC_BTN_HOME, &CDlgLdUld::OnBnClickedBtnHome)
END_MESSAGE_MAP()


// CDlgLdUld 消息处理程序
BOOL CDlgLdUld::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_hIcon_On = ::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_LED_ON));
	m_hIcon_Off= ::LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_LED_OFF));

	CreateLdGrid();
	CreateUldGrid();

	InitCtrl();

	m_nLdLayerCount = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount;
	m_nUldLayerCount = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nUldLayerCount;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgLdUld::InitCtrl()
{
	for (int i=0; i<6; i++)
	{
		m_static_status[i].ModifyStyle(0, SS_ICON);
		m_static_status[i].SetIcon(m_hIcon_Off);
	}

	m_cmbLdPitch.ResetContent();
	m_cmbLdPitch.AddString("0.001");
	m_cmbLdPitch.AddString("0.01");
	m_cmbLdPitch.AddString("0.1");
	m_cmbLdPitch.AddString("1");
	m_cmbLdPitch.AddString("10");
	m_cmbLdPitch.AddString("50");
	m_cmbLdPitch.SetCurSel(3);

	m_cmbUldPitch.ResetContent();
	m_cmbUldPitch.AddString("0.001");
	m_cmbUldPitch.AddString("0.01");
	m_cmbUldPitch.AddString("0.1");
	m_cmbUldPitch.AddString("1");
	m_cmbUldPitch.AddString("10");
	m_cmbUldPitch.AddString("50");
	m_cmbUldPitch.SetCurSel(3);

	m_btnLoadUp.SetIcon(IDI_ICON_ARROW_U);
	m_btnLoadUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, -30);
	m_btnLoadBoxUp.SetIcon(IDI_ICON_ARROW_U);
	m_btnLoadBoxUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, -30);
	m_btnLoadLayerUp.SetIcon(IDI_ICON_ARROW_U);
	m_btnLoadLayerUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, -30);
	m_btnUnloadUp.SetIcon(IDI_ICON_ARROW_U);
	m_btnUnloadUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, -30);
	m_btnUnloadBoxUp.SetIcon(IDI_ICON_ARROW_U);
	m_btnUnloadBoxUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, -30);
	m_btnUnloadLayerUp.SetIcon(IDI_ICON_ARROW_U);
	m_btnUnloadLayerUp.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, -30);

	m_btnLoadDown.SetIcon(IDI_ICON_ARROW_D);
	m_btnLoadDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnLoadBoxDown.SetIcon(IDI_ICON_ARROW_D);
	m_btnLoadBoxDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnLoadLayerDown.SetIcon(IDI_ICON_ARROW_D);
	m_btnLoadLayerDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnUnloadDown.SetIcon(IDI_ICON_ARROW_D);
	m_btnUnloadDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnUnloadBoxDown.SetIcon(IDI_ICON_ARROW_D);
	m_btnUnloadBoxDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnUnloadLayerDown.SetIcon(IDI_ICON_ARROW_D);
	m_btnUnloadLayerDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	m_btnHome.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnLMov.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnLSet.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnSave.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnUlMov.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnUlSet.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	m_btnHome.SetIcon(IDI_ICON_HOME);
	m_btnLMov.SetIcon(IDI_ICON_MOVETO);
	m_btnLSet.SetIcon(IDI_ICON_SINGLE_ADJUST);
	m_btnSave.SetIcon(IDI_ICON_SAVE_S);
	m_btnUlMov.SetIcon(IDI_ICON_MOVETO);
	m_btnUlSet.SetIcon(IDI_ICON_SINGLE_ADJUST);
}


void CDlgLdUld::UpdataIOStatus(long exInput)	// 更新状态
{
	//////////////////////////////////////////////////////////////////////////
	// input
	for (int i=0; i<6; i++)
	{
		if ((exInput&(0x01<<(i+6)))==(0x01<<(i+6)))
			m_static_status[i].SetIcon(m_hIcon_Off);
		else
			m_static_status[i].SetIcon(m_hIcon_On);
	}
}

void CDlgLdUld::CreateLdGrid()
{
	m_GridLdPos.EnableDragAndDrop(FALSE);
	m_GridLdPos.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_GridLdPos.SetEditable(TRUE);
	m_GridLdPos.SetListMode(TRUE);
	m_GridLdPos.SetTrackFocusCell(FALSE);
	m_GridLdPos.SetFrameFocusCell(FALSE);		//去除鼠标点选时的边框线  
	m_GridLdPos.SetColumnCount(LdUld_pos_col);	//初始列数,按顺序分别为：点位名称,数值
	m_GridLdPos.SetRowCount(LdUld_pos_num + 4);			//设置初始行数量，初始化只有一行

	m_GridLdPos.SetFixedRowCount(1);	
	m_GridLdPos.SetFixedBkColor(RGB(128,128,192));
	m_GridLdPos.SetGridLines(GVL_BOTH);
	CString l_strTitle[] = {"  参  数  名  称  ","   数   值   "};
	//序号头宽度
	for (int i=0; i<LdUld_pos_col; i++)
	{
		m_GridLdPos.SetGridItemText(0,i,l_strTitle[i]);
		m_GridLdPos.ExpandColumnsToFit();
	}
	m_GridLdPos.AutoSizeColumns();
	m_GridLdPos.ExpandLastColumn();

	m_GridLdPos.SetColumnResize(TRUE);
	m_GridLdPos.SetRowResize(FALSE);

	//调整行高
	for(UINT i=0;i<LdUld_pos_num+4;i++)
	{
		m_GridLdPos.SetRowHeight(i,28);	
	}

	//调整风格
	for (UINT i=1;i<LdUld_pos_num+4;i++)
	{
		for (UINT j=0;j<LdUld_pos_col;j++)
		{	
			m_GridLdPos.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
			if (0==j)
			{	
				m_GridLdPos.GetCell(i, j)->SetBackClr(RGB(128,192,128));
				m_GridLdPos.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
			}
		}
	}	

	UpdateGrid();
}

void CDlgLdUld::CreateUldGrid()
{
	m_GridUldPos.EnableDragAndDrop(FALSE);
	m_GridUldPos.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_GridUldPos.SetEditable(TRUE);
	m_GridUldPos.SetListMode(TRUE);
	m_GridUldPos.SetTrackFocusCell(FALSE);
	m_GridUldPos.SetFrameFocusCell(FALSE);		//去除鼠标点选时的边框线  
	m_GridUldPos.SetColumnCount(LdUld_pos_col);	//初始列数,按顺序分别为：点位名称,数值
	m_GridUldPos.SetRowCount(LdUld_pos_num + 2);			//设置初始行数量，初始化只有一行

	m_GridUldPos.SetFixedRowCount(1);	
	m_GridUldPos.SetFixedBkColor(RGB(128,128,192));
	m_GridUldPos.SetGridLines(GVL_BOTH);
	CString l_strTitle[] = {"  参  数  名  称  ","   数   值   "};
	//序号头宽度
	for (int i=0; i<LdUld_pos_col; i++)
	{
		m_GridUldPos.SetGridItemText(0,i,l_strTitle[i]);
		m_GridUldPos.ExpandColumnsToFit();
	}
	m_GridUldPos.AutoSizeColumns();
	m_GridUldPos.ExpandLastColumn();

	m_GridUldPos.SetColumnResize(TRUE);
	m_GridUldPos.SetRowResize(FALSE);

	//调整行高
	for(UINT i=0;i<LdUld_pos_num+2;i++)
	{
		m_GridUldPos.SetRowHeight(i,28);	
	}

	//调整风格
	for (UINT i=1;i<LdUld_pos_num+2;i++)
	{
		for (UINT j=0;j<LdUld_pos_col;j++)
		{	
			m_GridUldPos.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
			if (0==j)
			{	
				m_GridUldPos.GetCell(i, j)->SetBackClr(RGB(128,192,128));
				m_GridUldPos.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
			}
		}
	}	

	UpdateGrid();
}

void CDlgLdUld::UpdateGrid()
{
	CString str;
	for(UINT i=1;i<LdUld_pos_num;i++)
	{
		m_GridLdPos.SetGridItemText(i, 0, g_strLdUldName[i]);
		m_GridUldPos.SetGridItemText(i, 0, g_strLdUldName[i]);

		str.Format("%.3f", g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[i-1]);
		m_GridLdPos.SetGridItemText(i, 1, str);
		str.Format("%.3f", g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[i-1]);
		m_GridUldPos.SetGridItemText(i, 1, str);
	}

	{
		m_GridLdPos.SetGridItemText(LdUld_pos_num+1, 0, _T("重复推料次数"));
		str.Format("%d", g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdRePushout);
		m_GridLdPos.SetGridItemText(LdUld_pos_num+1, 1, str);
		m_GridLdPos.SetGridItemText(LdUld_pos_num+2, 0, _T("推料报警层数"));
		str.Format("%d", g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdEmptyWarning);
		m_GridLdPos.SetGridItemText(LdUld_pos_num+2, 1, str);
		m_GridLdPos.SetGridItemText(LdUld_pos_num+3, 0, _T("循环进料(0~1)"));
		str.Format("%d", g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdContinue);
		m_GridLdPos.SetGridItemText(LdUld_pos_num+3, 1, str);
	}

	{
		m_GridUldPos.SetGridItemText(LdUld_pos_num+1, 0, _T("推出料延时(s)"));
		str.Format("%.3f", g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldDelay);
		m_GridUldPos.SetGridItemText(LdUld_pos_num+1, 1, str);
	}
	EnableWindow(1 == g_pFrm->m_pRobotParam->m_nLoadUnloadMode);
}

// void CDlgLdUld::OnLdGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
// {
// 
// }
// 
// void CDlgLdUld::OnUldGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
// {
// 
// }


void CDlgLdUld::OnBnClickedBtnLoad1()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bStatus = TRUE;
	g_pFrm->m_Robot->ExInput("13号光开", bStatus);
	if (bStatus)
	{
		AfxMessageBox("点胶工位有板,取走后再测试进料动作！");
		return ;
	}

	short iRtn = 0;
	iRtn = g_pFrm->m_Robot->ExOutput("12号阻挡气缸电磁阀", TRUE);
	// 启动皮带	
	g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
	
	// 推料推出
	BOOL bBdDetect;
	iRtn = g_pFrm->m_Robot->LoadUnLoad_loadPushOut(bBdDetect);
	if (RTN_NONE != iRtn)
	{
		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
		iRtn = g_pFrm->m_Robot->LoadUnLoad_loadPullBack();
		AfxMessageBox("推料推出出错!");
		return;
	}
	iRtn = g_pFrm->m_Robot->LoadUnLoad_loadPullBack();
	
	if (!bBdDetect)
	{
		g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
		AfxMessageBox("入口未检测到进料");
		return;
	}

	CKFTimer tmTemp;
	tmTemp.TimerStart();
	while (!bStatus)
	{
		if (tmTemp.TimerGetSecond() > 4)
		{
			AfxMessageBox("物料到点胶位超时!");
			g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
			return;
		}

		g_pFrm->m_Robot->ExInput("13号光开", bStatus);
		Sleep(20);
	}

	g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);
	CFunction::DelaySec(0.2);

	// 进料完成
	g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);

	g_pFrm->m_Robot->LoadUnLoad_loadLayerMove(g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId+1);
}


void CDlgLdUld::OnBnClickedBtnLoadOut()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bBdDetect;
	g_pFrm->m_Robot->LoadUnLoad_loadPushOut(bBdDetect);
}


void CDlgLdUld::OnBnClickedBtnLoadBack()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->LoadUnLoad_loadPullBack();
}


void CDlgLdUld::OnBnClickedBtnLu()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;

	g_pFrm->m_Precision.GetCurPosMM();
	CString str;
	m_cmbLdPitch.GetWindowText(str);
	double dStepDist = atof(str);
	dStepDist = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[LOAD_AXIS]+dStepDist;
	short rtn = g_pFrm->m_Robot->m_pController->AxisMove(LOAD_AXIS,dStepDist,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Acc, TRUE);

	if (RTN_NONE != rtn)
	{
		AfxMessageBox("上料电机移动出错!");
	}
}


void CDlgLdUld::OnBnClickedBtnLd()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;

	g_pFrm->m_Precision.GetCurPosMM();
	CString str;
	m_cmbLdPitch.GetWindowText(str);
	double dStepDist = atof(str);
	dStepDist = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[LOAD_AXIS]-dStepDist;
	short rtn = g_pFrm->m_Robot->m_pController->AxisMove(LOAD_AXIS,dStepDist,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Acc, TRUE);

	if (RTN_NONE != rtn)
	{
		AfxMessageBox("上料电机移动出错!");
	}
}


void CDlgLdUld::OnBnClickedBtnLboxU()
{
	// TODO: 在此添加控件通知处理程序代码
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;
	int nCrtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId;
	g_pFrm->m_Robot->LoadUnLoad_loadBoxMove(nCrtBox+1);
}


void CDlgLdUld::OnBnClickedBtnLboxD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;
	int nCrtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId;
	g_pFrm->m_Robot->LoadUnLoad_loadBoxMove(nCrtBox-1);
}


void CDlgLdUld::OnBnClickedBtnLmetU()
{
	// TODO: 在此添加控件通知处理程序代码
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;
	int nCrtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId;
	g_pFrm->m_Robot->LoadUnLoad_loadLayerMove(nCrtLayer+1);
}


void CDlgLdUld::OnBnClickedBtnLmetD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;
	int nCrtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId;
	g_pFrm->m_Robot->LoadUnLoad_loadLayerMove(nCrtLayer-1);
}


void CDlgLdUld::OnBnClickedBtnLmov()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_loadCheck())return;

	int row_click = -1;
	row_click = m_GridLdPos.GetFocusCell().row;
	if (row_click<1 || row_click>LdUld_pos_num)	return;

	CString strTemp;
	double ldPos=0;

	ldPos = g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[row_click-1];
	short rtn = g_pFrm->m_Robot->m_pController->AxisMove(LOAD_AXIS, ldPos,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Acc, TRUE);
	if (RTN_NONE != rtn)
	{
		AfxMessageBox("上料电机移动出错!");
		return ;
	}

	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdBoxId = (row_click-1)/ 2;
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtLdLayerId = (row_click&0x01) ? 0 : g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount-1;
}


void CDlgLdUld::OnBnClickedBtnLset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (IDOK != AfxMessageBox("是否更新点位!", MB_OKCANCEL))return;

	int row_click = -1;
	row_click = m_GridLdPos.GetFocusCell().row;
	if (row_click<1 || row_click>LdUld_pos_num)	return;

	g_pFrm->m_Precision.GetCurPosMM();
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[row_click-1] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[LOAD_AXIS];
	UpdateGrid();
}

void CDlgLdUld::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount = GetDlgItemInt(IDC_EDIT_LD_LAYERCOUNT);
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nUldLayerCount = GetDlgItemInt(IDC_EDIT_ULD_LAYERCOUNT);

	for (int i=1; i<LdUld_pos_num; i++)
	{
		g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[i-1] = m_GridLdPos.GetItemDataDouble(i, 1);
		g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[i-1] = m_GridUldPos.GetItemDataDouble(i, 1);
	}

	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdRePushout = m_GridLdPos.GetItemDataInt(LdUld_pos_num+1, 1);
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdEmptyWarning = m_GridLdPos.GetItemDataInt(LdUld_pos_num+2, 1);
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdContinue = m_GridLdPos.GetItemDataInt(LdUld_pos_num+3, 1);

	g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldDelay = m_GridUldPos.GetItemDataDouble(LdUld_pos_num+1, 1);
	g_pFrm->RobotParam(FALSE);
}


void CDlgLdUld::OnBnClickedBtnUnload1()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bStatus1;
	BOOL bStatus2;
	BOOL bStatus = FALSE;
	short iRet = 0;
	g_pFrm->m_Robot->ExInput("13号光开", bStatus1);
	g_pFrm->m_Robot->ExInput("14号光开", bStatus2);

	if (!bStatus1 && !bStatus2)
	{
		return;
	}

	// 阻挡降下
	g_pFrm->m_Robot->ExOutput("12号阻挡气缸电磁阀", FALSE);
	// 启动皮带	
	g_pFrm->m_Robot->m_pController->AxisJog(T1_AXIS, g_pFrm->m_mtrParamGts[T1_AXIS].m_Vmax, g_pFrm->m_mtrParamGts[T1_AXIS].m_Acc);

	CKFTimer tmTemp;
	tmTemp.TimerStart();
	while (!bStatus2)
	{
		if (tmTemp.TimerGetSecond() > 4.0)
		{
			g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
			g_pFrm->m_Robot->AfxMessageBox("出料卡料,到出口检测光开超时!");
			return;
		}

		CFunction::DelaySec(0.02);
		g_pFrm->m_Robot->ExInput("14号光开", bStatus2);
	}
	g_pFrm->m_Robot->ExInput("14号光开", bStatus2);
	tmTemp.TimerStart();
	while (bStatus2)
	{
		if (tmTemp.TimerGetSecond() > 4.0)
		{
			g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
			g_pFrm->m_Robot->AfxMessageBox("出料口卡料!请手动处理");
		}

		iRet = g_pFrm->m_Robot->ExInput("14号光开", bStatus2);
		CFunction::DelaySec(0.02);
	}

	CFunction::DelaySec(g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldDelay);

	//////////////////////////////////////////////////////////////////////////
	iRet = g_pFrm->m_Robot->ExOutput("2号推出料气缸电磁阀",TRUE);
	if (iRet != RTN_NONE)	
	{
		g_pFrm->m_Robot->AfxMessageBox("气缸动作出错!");
	}

	iRet = g_pFrm->m_Robot->ExInput("推出料气缸-R", bStatus);
	tmTemp.TimerStart();
	while (!bStatus)
	{
		if (tmTemp.TimerGetSecond() > 4.0)
		{
			if (IDOK == AfxMessageBox("推出料气缸2 到位超时", MB_OKCANCEL))
			{
			}
		}

		iRet = g_pFrm->m_Robot->ExInput("推出料气缸-R", bStatus);
		CFunction::DelaySec(0.02);
	}

	iRet = g_pFrm->m_Robot->ExOutput("1号推出料气缸电磁阀",TRUE);
	iRet = g_pFrm->m_Robot->ExInput("推出料气缸-L", bStatus);
	tmTemp.TimerStart();
	while (!bStatus)
	{
		if (tmTemp.TimerGetSecond() > 4.0)
		{
			AfxMessageBox("推出料气缸1 到位超时");
		}

		iRet = g_pFrm->m_Robot->ExInput("推出料气缸-L", bStatus);
		CFunction::DelaySec(0.02);
	}
	// 推料退回
	g_pFrm->m_Robot->ExOutput("1号推出料气缸电磁阀", FALSE);
	g_pFrm->m_Robot->ExOutput("2号推出料气缸电磁阀", FALSE);

	g_pFrm->m_Robot->ExInput("14号光开", bStatus);
	if (bStatus)
	{
		AfxMessageBox("出口卡料,机器出口光开检测到有物料");
	}

	g_pFrm->m_Robot->m_pController->AxisStop(1<<T1_AXIS);
	g_pFrm->m_Robot->LoadUnLoad_unloadLayerMove(g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId+1);
}


void CDlgLdUld::OnBnClickedBtnUnloadOut()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->LoadUnLoad_unloadPushOut();
}


void CDlgLdUld::OnBnClickedBtnUnloadBack()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->LoadUnLoad_unloadPullBack();
}


void CDlgLdUld::OnBnClickedBtnUlu()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())			return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)			return;
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())	return;

	g_pFrm->m_Precision.GetCurPosMM();
	CString str;
	m_cmbUldPitch.GetWindowText(str);
	double dStepDist = atof(str);
	dStepDist = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[UNLOAD_AXIS]+dStepDist;
	short rtn = g_pFrm->m_Robot->m_pController->AxisMove(UNLOAD_AXIS,dStepDist,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Acc, TRUE);

	if (RTN_NONE != rtn)
	{
		AfxMessageBox("下料电机移动出错!");
	}
}


void CDlgLdUld::OnBnClickedBtnUld()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())	return;

	g_pFrm->m_Precision.GetCurPosMM();
	CString str;
	m_cmbUldPitch.GetWindowText(str);
	double dStepDist = atof(str);
	dStepDist = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[UNLOAD_AXIS]-dStepDist;
	short rtn = g_pFrm->m_Robot->m_pController->AxisMove(UNLOAD_AXIS,dStepDist,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Acc, TRUE);

	if (RTN_NONE != rtn)
	{
		AfxMessageBox("上料电机移动出错!");
	}
}


void CDlgLdUld::OnBnClickedBtnUlboxU()
{
	// TODO: 在此添加控件通知处理程序代码
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())	return;
	int nCrtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId;
	g_pFrm->m_Robot->LoadUnLoad_unloadBoxMove(nCrtBox+1);
}


void CDlgLdUld::OnBnClickedBtnUlboxD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())	return;
	int nCrtBox = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId;
	g_pFrm->m_Robot->LoadUnLoad_unloadBoxMove(nCrtBox-1);
}


void CDlgLdUld::OnBnClickedBtnUlmetU()
{
	// TODO: 在此添加控件通知处理程序代码
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())	return;
	int nCrtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId;
	g_pFrm->m_Robot->LoadUnLoad_unloadLayerMove(nCrtLayer+1);
}


void CDlgLdUld::OnBnClickedBtnUlmetD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())	return;
	int nCrtLayer = g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId;
	g_pFrm->m_Robot->LoadUnLoad_unloadLayerMove(nCrtLayer-1);
}


void CDlgLdUld::OnBnClickedBtnUlmov()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (RTN_NONE != g_pFrm->m_Robot->LoadUnLoad_unloadCheck())return;

	int row_click = -1;
	row_click = m_GridUldPos.GetFocusCell().row;
	if (row_click<1 || row_click>LdUld_pos_num+1)	return;

	CString strTemp;
	double ldPos=0;

	if (row_click <= LdUld_pos_num)
	{
		ldPos = g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[row_click-1];
		short rtn = g_pFrm->m_Robot->m_pController->AxisMove(UNLOAD_AXIS, ldPos,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Acc, TRUE);
		if (RTN_NONE != rtn)
		{
			AfxMessageBox("上料电机移动出错!");
		}
	}
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldBoxId = (row_click-1)/ 2;
	g_pFrm->m_pRobotParam->m_structLdUldParam.m_nCrtUldLayerId = (row_click&0x01) ? 0 : g_pFrm->m_pRobotParam->m_structLdUldParam.m_nUldLayerCount-1;
}


void CDlgLdUld::OnBnClickedBtnUlset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_pFrm->m_Robot->m_pController->IsInitOk())		return;
	if (1 != g_pFrm->m_pRobotParam->m_nLoadUnloadMode)		return;
	if (IDOK != AfxMessageBox("是否更新点位!", MB_OKCANCEL))return;

	int row_click = -1;
	row_click = m_GridUldPos.GetFocusCell().row;
	if (row_click<1 || row_click>LdUld_pos_num+1)	return;

	if (row_click <= LdUld_pos_num)
	{
		g_pFrm->m_Precision.GetCurPosMM();
		g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[row_click-1] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[UNLOAD_AXIS];
	}
	else
	{		
		g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldDelay = m_GridUldPos.GetItemDataDouble(LdUld_pos_num+1, 1);
	}
	UpdateGrid();
}


void CDlgLdUld::OnBnClickedBtnHome()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_pFrm->m_Robot->m_pController->IsInitOk()==FALSE) 		return;

	g_pFrm->m_Robot->LoadUnLoad_Home(0);
	g_pFrm->m_Robot->LoadUnLoad_Home(1);
// 	g_pFrm->m_Robot->m_pController->AxisHome(LOAD_AXIS, g_pFrm->m_mtrParamGts[LOAD_AXIS].m_Vhome, g_pFrm->m_mtrParamGts[LOAD_AXIS].m_VhomeLow,
// 										 g_pFrm->m_mtrParamGts[LOAD_AXIS].m_AccHome,  g_pFrm->m_mtrParamGts[LOAD_AXIS].m_HomeReturn);
// 	g_pFrm->m_Robot->m_pController->AxisHome(UNLOAD_AXIS, g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_Vhome, g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_VhomeLow,
// 		g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_AccHome,  g_pFrm->m_mtrParamGts[UNLOAD_AXIS].m_HomeReturn);

	g_pFrm->m_pRobotParam->m_structLdUldParam.Init();
}
