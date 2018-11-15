
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "TSCtrlSysView.h"

#include "MainFrm.h"
#include "PageSys.h"
#include "PageApp.h"
#include "PageJet.h"
#include "PagePos.h"
#include "PageImg.h"
#include "DlgGoto.h"
#include "DlgLogin.h"
#include "DlgCom.h"
//#include "PageBanleceSet.h"
#include "DlgTrackDraw.h"
//#include "TSPropertySheet.h"
#include "DlgBalanceOp.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

static const double g_dHandMovSpdLow = 0.1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)

	ON_COMMAND_RANGE(IDS_TOOL_SETTINGS,IDS_TOOL_CHIPAGE, &CMainFrame::OnToolCmd)

	ON_UPDATE_COMMAND_UI_RANGE(IDS_TOOL_SETTINGS , IDS_TOOL_CHIPAGE , &CMainFrame::OnUpdateToolCmd)
	ON_COMMAND_RANGE(IDS_TOOL_DOT,IDS_TOOL_LISTEDIT, &CMainFrame::OnToolCmd) //画图工具栏按钮响应
    ON_COMMAND_RANGE(IDS_TOOLS_SUPERSTART,IDS_TOOLS_SUPEREND, &CMainFrame::OnToolCmd)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN,&CMainFrame::OnUpdateToolOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE,&CMainFrame::OnUpdateToolSave)
	ON_COMMAND(ID_CMD_REOPEN_FILE, &CMainFrame::OnCmdReopenFile)

	ON_COMMAND(ID_VIEW_CMD_EDIT, &CMainFrame::OnViewCmdEdit)
	ON_COMMAND(ID_LOGON, &CMainFrame::OnLogon)
	ON_COMMAND(ID_LOGOFF, &CMainFrame::OnLogoff)

	ON_UPDATE_COMMAND_UI(ID_LOGON, &CMainFrame::OnUpdateLogon)

	ON_COMMAND(ID_RS232_CONFIG, &CMainFrame::OnRS232Config)
	ON_COMMAND(ID_ONLINE_ENABLE, &CMainFrame::OnSocketEnable)

	ON_UPDATE_COMMAND_UI(ID_RS232_CONFIG, &CMainFrame::OnUpdateSocketEnable)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_ENABLE, &CMainFrame::OnUpdateRS232Config)

	ON_UPDATE_COMMAND_UI(IDS_RUN_STATUS, &CMainFrame::OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(IDS_RUN_COUNT, &CMainFrame::OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(IDS_CYCLE_TIME, &CMainFrame::OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(IDS_CUR_CLOCK, &CMainFrame::OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(IDS_ONLINE_MODE, &CMainFrame::OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(IDS_TOOL_STATUS, &CMainFrame::OnUpdateStatusBar)

	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CLOSE()
	ON_WM_DESTROY()

	ON_MESSAGE(WM_MSG_INPUT_EVENT, &CMainFrame::OnMessageInputEvent)
	ON_MESSAGE(WM_MSG_POS_EVENT, &CMainFrame::OnMessagePosEvent)
	ON_MESSAGE(WM_MSG_PANE_STS, &CMainFrame::OnMessagePaneStatus)
	ON_WM_HOTKEY()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

static UINT indicators[] =
{
//	ID_SEPARATOR,           // 状态行指示器
	IDS_TOOL_VER,			// 版本号
	IDS_TOOL_STATUS,
	IDS_RUN_STATUS,
	IDS_RUN_COUNT,			//程序运行生产次数
	IDS_CYCLE_TIME,			//单次运行时间
	IDS_APP_STATUS,			//卡状态
	IDS_ONLINE_MODE,		//网络状态
	IDS_CUR_CLOCK,			//当前程序运行累计时间
};
// CMainFrame 构造/析构
extern CEvent m_EvtAskSeparaRun;
CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = ID_VIEW_APPLOOK_OFF_2007_AQUA;
	m_bSocketEnable = FALSE;
	m_Robot = new CCmdRun;
	m_pSysParam = new tgSysParam;
	m_pRobotParam = new tgRobotParam;
    bisWeightError = FALSE;
	LanguageMgr = NULL;
//	m_strVer = "版本号: [1.0.0.1]";
}
CMainFrame::~CMainFrame()
{
	if(m_Robot!=NULL)
		delete m_Robot;

	m_Robot = NULL;

	if (m_pSysParam!=NULL) 
	{
		delete m_pSysParam;
		m_pSysParam = NULL;
	}

	if (m_pRobotParam != NULL)
	{
		delete m_pRobotParam;
		m_pRobotParam = NULL;
	}

	if (LanguageMgr!=NULL) 
		delete LanguageMgr;
}

void CMainFrame::OnUpdateStatusBar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	m_wndToolBar.CreateEx(this, TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1),IDR_MAINFRAME_256);
	m_wndToolBar.LoadToolBar(IDR_MAINFRAME_256, 0, 0, FALSE,0,0,theApp.m_bHiColorIcons ? IDB_BITMAP_TOOLBAR:0);
	m_wndToolBar.EnableTextLabels();

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

// 	m_DrawToolBar.CreateEx(this, TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1),IDR_MAINFRAME_COMMAND);
// 	m_DrawToolBar.LoadToolBar(IDR_MAINFRAME_COMMAND, 0, 0, FALSE,0,0,theApp.m_bHiColorIcons ? IDB_BITMAP_COMMAND:0);
// 	m_DrawToolBar.EnableTextLabels();
// 	m_DrawToolBar.SetWindowText(_T("CMD TOOLS"));

	// 允许用户定义的工具栏操作:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	//状态图标初始化
	if (m_BitmapRun.GetSafeHandle () == NULL)
		m_BitmapRun.LoadBitmap (IDB_BITMAP_RUN);
	if (m_BitmapPause.GetSafeHandle () == NULL)
		m_BitmapPause.LoadBitmap (IDB_BITMAP_PAUSE);
	if (m_BitmapStop.GetSafeHandle () == NULL)
		m_BitmapStop.LoadBitmap (IDB_BITMAP_STOP);
	if (m_BitmapEStop.GetSafeHandle () == NULL)
		m_BitmapEStop.LoadBitmap (IDB_BITMAP_ESTOP);
	if (m_BitmapReady.GetSafeHandle () == NULL)
		m_BitmapReady.LoadBitmap (IDB_BITMAP_SMILE);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建7
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneWidth(m_wndStatusBar.CommandToIndex(IDS_TOOL_VER),150); 
	m_wndStatusBar.SetTipText(m_wndStatusBar.CommandToIndex(IDS_TOOL_VER),_T("版本号"));
	SetPanelTextEx(IDS_TOOL_VER, CFunction::GetVerion());
	m_wndStatusBar.SetPaneWidth(m_wndStatusBar.CommandToIndex(IDS_TOOL_STATUS),100); 
	m_wndStatusBar.SetTipText(m_wndStatusBar.CommandToIndex(IDS_TOOL_STATUS),_T("阀ID"));
	m_wndStatusBar.SetPaneWidth(m_wndStatusBar.CommandToIndex(IDS_ONLINE_MODE),100); 
	m_wndStatusBar.SetPaneWidth(m_wndStatusBar.CommandToIndex(IDS_APP_STATUS),220); 
	m_wndStatusBar.SetTipText(m_wndStatusBar.CommandToIndex(IDS_APP_STATUS),_T("板卡初始化状态"));
	m_wndStatusBar.SetPaneWidth(m_wndStatusBar.CommandToIndex(IDS_RUN_STATUS),100); 
	m_wndStatusBar.SetTipText(m_wndStatusBar.CommandToIndex(IDS_RUN_STATUS),_T("RunSts"));
	m_wndStatusBar.SetPaneTextColor(m_wndStatusBar.CommandToIndex(IDS_RUN_STATUS),RGB(255,0,0));
	m_wndStatusBar.SetPaneWidth(m_wndStatusBar.CommandToIndex(IDS_RUN_COUNT),150); 
	m_wndStatusBar.SetTipText(m_wndStatusBar.CommandToIndex(IDS_RUN_COUNT),_T("累计生产计数"));
	m_wndStatusBar.SetPaneWidth(m_wndStatusBar.CommandToIndex(IDS_CYCLE_TIME),150); 	
	m_wndStatusBar.SetTipText(m_wndStatusBar.CommandToIndex(IDS_CYCLE_TIME),_T("周期时间"));
	m_wndStatusBar.SetPaneWidth(m_wndStatusBar.CommandToIndex(IDS_CUR_CLOCK),190); 
	m_wndStatusBar.SetTipText(m_wndStatusBar.CommandToIndex(IDS_CUR_CLOCK),_T("当前时间"));

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	m_DrawToolBar.EnableDocking(CBRS_ALIGN_LEFT);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);
//	DockPane(&m_DrawToolBar);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	//// 导航窗格将创建在左侧，因此将暂时禁用左侧和右侧的停靠:
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);

	////////
	g_pFrm = this;// 创建停靠条初始化需要用到此指针

	CKFTimer tm;
	// 系统参数
	SysParam(TRUE); 
	TRACE("sys:[%f]\n", tm.TimerGetMs());
	tm.TimerStart();
	// 马达参数
	MotorParam(TRUE);
	TRACE("mtr:[%f]\n", tm.TimerGetMs());
	tm.TimerStart();
	// 串口配置
	ComPortParam(TRUE);
	TRACE("com:[%f]\n", tm.TimerGetMs());
	tm.TimerStart();
	// 位置参数
	RobotParam(TRUE);
	TRACE("robot:[%f]\n", tm.TimerGetMs());
	tm.TimerStart();

	// 点参数
	DotParam(TRUE);
	TRACE("mtr:[%f]\n", tm.TimerGetMs());
	tm.TimerStart();
	// 线参数
	LineParam(TRUE);
	TRACE("mtr:[%f]\n", tm.TimerGetMs());
	tm.TimerStart();

	// 语言文件
	OnReadLanguageFile();  
	theApp.m_SysUser.LoadUserData();
	if(m_pSysParam->bEnablePrecFun == TRUE)
	{
       m_Precision.PrecisionFile(TRUE);
	}

	//////////////////////////////////////////////////////////////////////////
	//	m_wndTitleBar	创建标题栏											//
	//////////////////////////////////////////////////////////////////////////
// 	DWORD style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP| CBRS_SIZE_DYNAMIC;
// 	style &= ~WS_CAPTION;
// 	if (!m_wndTitleBar.Create("", this, CRect(0, 0, 1280, 56), FALSE, IDD_DLG_TITLE, style))
// 	{
// 		TRACE0("未能创建标题栏窗口/n");
// 		return -1;
// 	}
// 	m_wndTitleBar.EnableDocking(CBRS_ALIGN_TOP);
// 	m_wndTitleBar.SetControlBarStyle(AFX_CBRS_AUTOHIDE | AFX_CBRS_CLOSE);
// 	m_wndTitleBar.SetResizeMode(FALSE);
// 	DockPane(&m_wndTitleBar);
	
	//////////////////////////////////////////////////////////////////////////
	//					创建系统信息工具条									//
	//////////////////////////////////////////////////////////////////////////
	CString strtemp =  LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_3");
	if (!m_wndRightBar.Create(strtemp, this, CRect(0, 0, 300, 1000), FALSE, IDD_DLG_INFO,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_SIZE_FIXED ))
	{
		TRACE0("未能创建系统信息窗口/n");
		return -1;
	}

	m_wndRightBar.EnableDocking(CBRS_ALIGN_RIGHT);
	m_wndRightBar.SetControlBarStyle(AFX_CBRS_AUTOHIDE | AFX_CBRS_CLOSE);
	m_wndRightBar.SetResizeMode(FALSE);
	DockPane(&m_wndRightBar);
	
	// 创建并设置“Outlook”导航栏:

// 	if (NULL == m_wndEditBar.GetSafeHwnd())
// 	{
// 		m_wndEditBar.Create(IDD_DLG_EDIT, this);
// 		m_wndEditBar.m_pMsgList = &m_wndOutput.m_wndOutputBuild;
// 		m_wndEditBar.EnableWindow(TRUE);
// 	}
// 	m_wndEditBar.SetForegroundWindow();
// 	m_wndEditBar.ShowWindow(SW_SHOW);
// 	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION,m_wndEditBar,350))
// 	{
// 		TRACE0("未能创建导航窗格1\n");
// 		return -1;// 未能创建
// 	}

	//创建输出窗口
	CString strOutputWnd = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_1");
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI,AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	{
		TRACE0("Creat OutPut Windows Error\n");
		return FALSE; // 未能创建
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);

// 	if (CMFCToolBar::GetUserImages() == NULL)
// 	{
// 		// 加载用户定义的工具栏图像
// 		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
// 		{
// 			m_UserImages.SetImageSize(CSize(32, 32), FALSE);
// 			CMFCToolBar::SetUserImages(&m_UserImages);
// 		}
// 	}

	//////////////////////////////////////
	//设置查找标记。
	::SetProp(m_hWnd,theApp.m_pszAppName,(HANDLE)1);
	//////////////////////////////////////
	SetMenu(NULL);   
	// 设置 CTRL+R 热键
	::RegisterHotKey(this->GetSafeHwnd(),1001,MOD_CONTROL,'R');

	// 隐藏标题栏
//	ModifyStyle(WS_CAPTION,  0,  SWP_FRAMECHANGED);  
// 	MoveWindow(0, 0, 1280, 1024);
// 	CenterWindow();

	// 获取CPU内核数目
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	m_nProcesserNum = sysInfo.dwNumberOfProcessors;
	m_nUIRunProcesser = 0;
	for (short s=0; s<m_nProcesserNum-1; s++)
	{
		m_nUIRunProcesser |= 1<<s;
	}

	return 0;
}
void CMainFrame::OnReadLanguageFile() 
{
	CString strFile;

	if(g_pFrm->m_pSysParam->nLanguageType == 0)
	{
		strFile = CFunction::GetDirectory() + "Languages\\Chinese.ini";
	}
	if(g_pFrm->m_pSysParam->nLanguageType == 1)
	{
		strFile = CFunction::GetDirectory() + "Languages\\English.ini";
	}
	if(g_pFrm->m_pSysParam->nLanguageType == 2)
	{
		strFile = CFunction::GetDirectory() + "Languages\\Korean.ini";
	}
	LanguageMgr = new CConfigOp(strFile,MODE_UNICODE);

}
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION// | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;
	
	cs.style  &=   ~WS_THICKFRAME; 


	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

}

// BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID,CCmdEditBar& editBar, int nInitialWidth)
// {
// 	CWindowDC dc(NULL);
// 
// 	BOOL bNameValid;
// 	CString strTemp;
// 	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
// 	ASSERT(bNameValid);
// 
// 	if (!bar.Create(strTemp, this, CRect(0, 0,nInitialWidth,32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
// 	{
// 		return FALSE; // 未能创建
// 	}
// 
// 	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();
// 	if (pOutlookBar == NULL)
// 	{
// 		ASSERT(FALSE);
// 		return FALSE;
// 	}
// 
// 	pOutlookBar->EnableInPlaceEdit(FALSE);
// 
// 	// 可浮动，可自动隐藏，可调整大小，但不能关闭
// 	DWORD dwStyle =  AFX_CBRS_AUTOHIDE /*| AFX_CBRS_RESIZE */| AFX_CBRS_CLOSE;
// 
// 	CRect rectDummy(0, 0, 0, 0);
// 	editBar.Create(rectDummy, &bar, 1200);
// 
// 	strTemp = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_2");
// 	pOutlookBar->AddControl(&editBar, strTemp, 3, TRUE, dwStyle);
// 
// 	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC);
// 	pOutlookBar->RecalcLayout();
// 
// 	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
// 	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);
// 
// 	bar.SetButtonsFont(&afxGlobalData.fontBold);
// 	return TRUE;
// }

// CMainFrame 诊断
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序
void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	//theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}
void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnUpdateToolCmd(CCmdUI* pCmdUI)
{
	/////////
	if(g_pFrm->m_pSysParam->nAutoMode==1) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(!theApp.m_EncryptOp.m_bLockSystem)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if (pCmdUI->m_nID == IDS_TOOL_DISPSET)
	{
		pCmdUI->Enable((m_Robot->m_tSysStatus != K_RUN_STS_RUN) && (theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL));
	}
	if(pCmdUI->m_nID == IDS_TOOL_PAUSE)
	{
		pCmdUI->Enable((m_Robot->m_tSysStatus ==K_RUN_STS_RUN));
	}
	else if(pCmdUI->m_nID == IDS_TOOL_RUN)
	{
		pCmdUI->Enable((m_Robot->m_tSysStatus ==K_RUN_STS_STOP) || (m_Robot->m_tSysStatus ==K_RUN_STS_PAUSE)/* && !g_pFrm->m_pSysParam->nAutoMode*/);
	}
	else if(pCmdUI->m_nID == IDS_TOOL_STEP)
	{
		pCmdUI->Enable((m_Robot->m_tSysStatus ==K_RUN_STS_STOP));
	}
	else if(pCmdUI->m_nID == IDS_TOOL_HOME)
	{
		pCmdUI->Enable((m_Robot->m_tSysStatus == K_RUN_STS_STOP || m_Robot->m_tSysStatus == K_RUN_STS_EMERGENCY));
	}

	if(pCmdUI->m_nID == IDS_TOOL_LIVE)
	{
		pCmdUI->Enable(g_pView->m_bImgInit);
	}
	else if(pCmdUI->m_nID == IDS_TOOL_STOPLIVE)
	{
		pCmdUI->Enable(g_pView->m_bImgInit);
	}
	else if(pCmdUI->m_nID == IDS_TOOL_MARK)
	{
	
	}
	else if(pCmdUI->m_nID == IDS_TOOL_PARAM)
	{
		pCmdUI->Enable((theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL));
	}
	else if(pCmdUI->m_nID == IDS_TOOL_PIXEL)
	{
		pCmdUI->Enable((theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL));
	}	
	else if(pCmdUI->m_nID == IDS_TOOL_LISTEDIT)
	{
		pCmdUI->Enable((theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL));
	}
	else if(pCmdUI->m_nID == IDS_TOOL_DOT)
	{
		pCmdUI->Enable(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL);
	}
	else if(pCmdUI->m_nID == IDS_TOOL_START)
	{
		pCmdUI->Enable((theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL));
	}
	else if(pCmdUI->m_nID == IDS_TOOL_PASS)
	{
		pCmdUI->Enable(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL);
	}
	else if(pCmdUI->m_nID == IDS_TOOL_END)
	{
		pCmdUI->Enable(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL);
	}
	else if(pCmdUI->m_nID == IDS_TOOL_ARC)
	{
		pCmdUI->Enable(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL);
	}
	else if(pCmdUI->m_nID == IDS_TOOL_CIRCLE)
	{
		pCmdUI->Enable(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL);
	}
	else if(pCmdUI->m_nID == IDS_TOOL_ZHEIGHT)
	{
		pCmdUI->Enable((theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL));
	}
	else if(pCmdUI->m_nID == IDS_TOOL_IO)
	{
		pCmdUI->Enable(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL);
	}
	
	else if(pCmdUI->m_nID == IDS_TOOL_COATLINE)
	{
		pCmdUI->Enable(FALSE);
	}
	else if(pCmdUI->m_nID != IDS_TOOL_STOPRUN)
	{
		pCmdUI->Enable((m_Robot->m_tSysStatus !=K_RUN_STS_RUN) && (m_Robot->m_tSysStatus !=K_RUN_STS_PAUSE));
	}

	if(pCmdUI->m_nID == IDS_TOOLS_BLINE)
	{
		if( 0 == g_pFrm->m_pSysParam->bBalanceEnable)
			pCmdUI->Enable(FALSE);
		else
			pCmdUI->Enable(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL);
	}
	else if(pCmdUI->m_nID == IDS_TOOLS_BDROP)
	{
		//
		if( 0 == g_pFrm->m_pSysParam->bBalanceEnable)
		     pCmdUI->Enable(FALSE);
		else
		    pCmdUI->Enable(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL);
	}
	else if(pCmdUI->m_nID == IDS_TOOL_COATDOT)
	{
		//   
		if( 0 == g_pFrm->m_pSysParam->bBalanceEnable)
			pCmdUI->Enable(FALSE);
		else
			pCmdUI->Enable(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL);
	}

	if(pCmdUI->m_nID == IDS_TOOL_RECT)
	{
		pCmdUI->Enable(FALSE);
	}

	if(pCmdUI->m_nID == IDS_TOOL_SETTINGS)
	{
		pCmdUI->Enable((m_Robot->m_tSysStatus !=K_RUN_STS_RUN)/* && (theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)*/);
	}   
}

void CMainFrame::OnUpdateToolOpen(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_Robot->m_tSysStatus == K_RUN_STS_STOP)&&(!theApp.m_EncryptOp.m_bLockSystem));
};
void CMainFrame::OnUpdateToolSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_Robot->m_tSysStatus == K_RUN_STS_STOP)&&(!theApp.m_EncryptOp.m_bLockSystem));
};

void CMainFrame::OnUpdateLogon(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_Robot->m_tSysStatus == K_RUN_STS_STOP)&&!theApp.m_EncryptOp.m_bLockSystem);
};
BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基类将执行真正的工作
	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}
	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnToolCmd(UINT ID)
{
	CString str;
	switch(ID)
	{
	case IDS_TOOL_HOME:     // 三轴回零
		m_Robot->Home();  
		{
			// 打开背压电磁阀
			short iRet = m_Robot->ExOutput("1号备压电磁阀",TRUE);
			if (iRet!=RTN_NONE)	
			{
				MessageBox("打开背压出错!!!");
			} 
		}  
		break;
	case IDS_TOOL_SETTINGS: // 参数界面
		//ShowSysDialog();
		ShowMachineSetDlg();
		break;
	case IDS_TOOL_LIVE:     // 相机采集
		g_pView->ImgLive();
		break; 
	case IDS_TOOL_STOPLIVE: // 相机关闭
		g_pView->ImgStop();
		break;

	case IDS_TOOL_PIXEL:     // 图像标定界面
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			ShowPixelDialog();
			g_pView->PaintShape(CRect(0, 0, IMG_WIDTH, IMG_HEIGHT));
		}
		else
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}
		break;
	case IDS_TOOL_REFPOS:    // 图像与喷射阀位置校准界面
		if(m_pSysParam->nToolType==0)
		{
			ShowMachineSetDlg();
		}
		else if(m_pSysParam->nToolType==1)
		{
			ShowNeedleAdjustDialog();
		}
		else
		{
			AfxMessageBox("当前为图像模式,请切换到其他模式");
		}
		break;	
	case IDS_TOOL_DISPSET:   // 点胶阀控制
		ShowDispParam();
	    //ShowFeedMdlCtrlDlg();
		break;	

	case IDS_TOOL_CHIPAGE:	// 轨道功能调试界面
		ShowTrackDialog();
		break;

	case IDS_TOOL_LISTEDIT:  // 程序整体偏移界面
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
			ShowProgOffsetDialog();
		else
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}
		break;
	case IDS_TOOL_WASH:    
		if(m_pSysParam->nToolType==0)
		{ 
			if(1 == m_pSysParam->iWashType)
			{  // 擦拭式清洗
				m_Robot->WashJet();   
			}
			else
			{  // 单吸式清洗
				m_Robot->WashNeedle();  
			}
		}
		break;
	case IDS_TOOL_RUN:      // 自动执行
		if (m_Robot->m_tSysStatus != K_RUN_STS_NONE)
		{
			m_Robot->Run();
		}   
		break;
	case IDS_TOOL_PAUSE:   // 暂停执行
		m_Robot->m_bIsHandPause = true;
		m_Robot->Pause();
		break;
	case IDS_TOOL_STEP:    // 单步执行
		//m_Robot->Step();
		{
			CDlgTrackDraw dlg;
			dlg.DoModal();
		}
		break;
	case IDS_TOOL_STOPRUN: // 停止运行
		m_Robot->Stop();        
		break;
	case IDS_TOOL_PARAM:   // 程序参数工具条
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
			m_wndEditBar.ShowTrackPopupMenu(0);
		else
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}
		break;

	case IDS_TOOL_IO:      // IO界面
		ShowIODialog();
		break;

		//////////////////////////////////////////////////////////////////////////
		////////////////////			编程指令工具				   ///////////
		//////////////////////////////////////////////////////////////////////////
	case IDS_TOOL_DOT:    // 单点指令
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			m_wndEditBar.m_bInsertCmd = FALSE;
			m_wndEditBar.OnCmdType(IDS_TOOL_DOT);
		}
		else
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}
		break;
	case IDS_TOOL_START:  // 线段起点指令
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			m_wndEditBar.m_bInsertCmd = FALSE;
			m_wndEditBar.OnCmdType(IDS_TOOL_START);
		}
		else
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}
		break;
	case IDS_TOOL_PASS: // 线段中点
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			m_wndEditBar.m_bInsertCmd = FALSE;
			m_wndEditBar.OnCmdType(IDS_TOOL_PASS);
		}
		else 
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}
		break;
	case IDS_TOOL_ARC:  // 圆弧中点
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			m_wndEditBar.m_bInsertCmd = FALSE;
			m_wndEditBar.OnCmdType(IDS_TOOL_ARC);
		}
		else 
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}
		break;
	case IDS_TOOL_END:  // 线段结束
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			m_wndEditBar.m_bInsertCmd = FALSE;
			m_wndEditBar.OnCmdType(IDS_TOOL_END);
		}
		else
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}

		break;
	case IDS_TOOL_CIRCLE: // 圆形点胶
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			m_wndEditBar.m_bInsertCmd = FALSE;
			m_wndEditBar.OnCmdType(IDS_TOOL_CIRCLE);
		}
		else
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}

		break;

	case IDS_TOOL_RECT: // 矩形点胶
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			m_wndEditBar.m_bInsertCmd = FALSE;
			m_wndEditBar.OnCmdType(IDS_TOOL_RECT);
		}
		else
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}
		break;
	case IDS_TOOLS_BDROP:   // 称重打点指令
		m_wndEditBar.m_bInsertCmd = FALSE;
		m_wndEditBar.OnCmdType(IDS_TOOLS_BDROP);
		break;
	case IDS_TOOLS_BLINE:   // 称重划线指令
		m_wndEditBar.m_bInsertCmd = FALSE;
		m_wndEditBar.OnCmdType(IDS_TOOLS_BLINE);
		break;
	case IDS_TOOL_COATDOT:   // 称重矩形
		m_wndEditBar.m_bInsertCmd = FALSE;
		m_wndEditBar.OnCmdType(IDS_TOOL_COATDOT);
		break;
	case IDS_TOOL_ZHEIGHT: // Z轴测高指令
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			m_wndEditBar.m_bInsertCmd = FALSE;
			m_wndEditBar.OnCmdType(IDS_TOOL_ZHEIGHT);
		}
		else
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}
		break;
	case IDS_TOOLS_SUPERSTART:
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			m_wndEditBar.m_bInsertCmd = FALSE;
			m_wndEditBar.OnCmdType(IDS_TOOLS_SUPERSTART);
		}
		else
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}
		break;
	case IDS_TOOLS_SUPERPOINT:
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			m_wndEditBar.m_bInsertCmd = FALSE;
			m_wndEditBar.OnCmdType(IDS_TOOLS_SUPERPOINT);
		}
		else
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}
		break;
	case IDS_TOOLS_SUPEREND:
		if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
		{
			m_wndEditBar.m_bInsertCmd = FALSE;
			m_wndEditBar.OnCmdType(IDS_TOOLS_SUPEREND);
		}
		else
		{
			str = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_4");
			AfxMessageBox(str);
		}
		break;
	default:break;
	}
}
void CMainFrame::ShowNeedleZAdjustDialog()
{
	if(m_dlgNeedleZAdjust.GetSafeHwnd() == NULL)
	{
		m_dlgNeedleZAdjust.Create(IDD_DLG_NEEDLE_ZH, this);
	}
	m_dlgNeedleZAdjust.CenterWindow();
	m_dlgNeedleZAdjust.ShowWindow( SW_SHOW);
}


void CMainFrame::ShowDualValveDlg()
{
	m_DlgDualValve.DoModal();
}


void CMainFrame::ShowDispParam()
{
	if(m_DlgDispParam.GetSafeHwnd() == NULL)
	{
		m_DlgDispParam.Create(IDD_DLG_DISPPARAM, this);
	}
	m_DlgDispParam.CenterWindow();
	m_DlgDispParam.ShowWindow(SW_SHOW);
}

void CMainFrame::ShowFeedMdlCtrlDlg()
{
	if(m_DlgFeedCtrlMdl.GetSafeHwnd() == NULL)
	{
		m_DlgFeedCtrlMdl.Create(IDD_DLG_FEED_CTRL, this);
	}
	m_DlgFeedCtrlMdl.CenterWindow();
	m_DlgFeedCtrlMdl.ShowWindow( SW_SHOW);
}

void CMainFrame::ShowZBaseSetDlg()
{
	if(m_DlgZBaseSet.GetSafeHwnd() == NULL)
	{
		m_DlgZBaseSet.Create(IDD_DLG_INFO_ZBASESET, this);
	}
	m_DlgZBaseSet.CenterWindow();
	m_DlgZBaseSet.ShowWindow( SW_SHOW);
}

void CMainFrame::ShowPathEditDialog()
{
	if(m_wndEditBar.GetSafeHwnd() == NULL)
	{
		m_wndEditBar.Create(IDD_DLG_EDIT, this);////????
	}

	m_wndEditBar.ShowWindow(SW_HIDE);
	m_wndEditBar.SetWindowText(g_AllProgInfo[0].m_curProgName);
	CRect rc;
	GetWindowRect(rc);

	CRect rc1;
	m_wndEditBar.GetWindowRect(rc1);
	m_wndEditBar.MoveWindow((rc.Width()-rc1.Width())/2, rc.bottom-rc1.Height(), rc1.Width(), rc1.Height());
	m_wndEditBar.ShowWindow(SW_SHOW);
}

void CMainFrame::ShowMachineSetDlg()
{
	if(m_dlgMachineSet.GetSafeHwnd() == NULL)
	{
		m_dlgMachineSet.Create(IDD_DLG_MACHINE_SET, this);////????
	}

	m_dlgMachineSet.ShowWindow(SW_HIDE);
	m_dlgMachineSet.m_pageJetAdjust.EnableWindow((m_Robot->m_tSysStatus != K_RUN_STS_RUN) && (theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL));
	m_dlgMachineSet.m_pageMachineParam.EnableWindow((m_Robot->m_tSysStatus != K_RUN_STS_RUN) && (theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL));
	m_dlgMachineSet.m_pageLoadUnload.EnableWindow((m_Robot->m_tSysStatus != K_RUN_STS_RUN) && (theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL));
	
	m_dlgMachineSet.InitTableCtrl();
	m_dlgMachineSet.ShowWindow(SW_SHOW);
}

void CMainFrame::ShowProgOffsetDialog()
{
	if(m_dlgProgOffset.GetSafeHwnd() == NULL)
	{
		m_dlgProgOffset.Create(IDD_DLG_PROG_OFFSET, this);
	}
	m_dlgProgOffset.CenterWindow();
	m_dlgProgOffset.ShowWindow( SW_SHOW);
}

void CMainFrame::ShowSysDialog()
{
	CString strmsg = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_7");

	CPropertySheet   Sheet(strmsg);

	CPageApp     pageApp;
	//CPageBanleceSet pageBalance;      // 称重测试界面
	CDlgBalanceOp     pageBalance;      // 电子称界面
	CPageJet pageJet;
	CPagePos pagePos;
	CPageSys pageSys;
	CPageImg pageImg;

	Sheet.AddPage(&pageApp);

	Sheet.AddPage(&pageJet);

	Sheet.AddPage(&pageImg);

	if(1 == g_pFrm->m_pSysParam->bBalanceEnable)
       Sheet.AddPage(&pageBalance);

	if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
	{
		Sheet.AddPage(&pageSys);
	}
	if(theApp.m_SysUser.m_CurUD.level>GM_ADMIN)
	{
		Sheet.AddPage(&pagePos);
	}

	Sheet.DoModal();

}
LRESULT CMainFrame::OnMessagePaneStatus(WPARAM wParam, LPARAM lParam)
{
	char *p = (char *)lParam;
	UINT nIDFind = int(wParam);
	CString str = CString(p);

	if(IDS_RUN_STATUS==nIDFind)
	{
		if(str==_T("Running..."))
			m_wndStatusBar.SetPaneIcon(m_wndStatusBar.CommandToIndex(nIDFind),m_BitmapRun,RGB(0,0,0));
		else if(str==_T("Pause..."))
			m_wndStatusBar.SetPaneIcon(m_wndStatusBar.CommandToIndex(nIDFind),m_BitmapPause,RGB(0,0,0));
		else if(str==_T("Emergency"))
			m_wndStatusBar.SetPaneIcon(m_wndStatusBar.CommandToIndex(nIDFind),m_BitmapEStop,RGB(0,0,0));
		else if(str==_T("StopRun"))
			m_wndStatusBar.SetPaneIcon(m_wndStatusBar.CommandToIndex(nIDFind),m_BitmapStop,RGB(0,0,0));
		else if(str==_T("GoHome"))
			m_wndStatusBar.SetPaneIcon(m_wndStatusBar.CommandToIndex(nIDFind),m_BitmapReady);
		else if(str==_T("System Ready"))
			m_wndStatusBar.SetPaneIcon(m_wndStatusBar.CommandToIndex(nIDFind),m_BitmapReady);
	}

	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(int(wParam)), str);
	return 0;
}

void CMainFrame::SetPanelTextEx(UINT ID, CString strText)
{
	SendMessage(WM_MSG_PANE_STS, WPARAM(ID), LPARAM(strText.GetBuffer()));
	strText.ReleaseBuffer();
}

void CMainFrame::ShowBoardCheckDlgEx(tgCmdLine *pCmdLine, int iSelIndex)
{
	if(m_dlgBoardCheckEx.GetSafeHwnd() == NULL)
	{
		m_dlgBoardCheckEx.Create(IDD_DLG_BOARDCALIBEX, this);
	}
	m_dlgBoardCheckEx.m_iSelIndex = iSelIndex;   // 指令ID
	m_dlgBoardCheckEx.m_pCmdLine = pCmdLine;     // 指令内容

	m_dlgBoardCheckEx.RefreshDate();

	////
	CRect rect,rect1,rect2;
	g_pView->GetWindowRect(&rect1);
	m_dlgBoardCheckEx.GetWindowRect(&rect2);
	rect.top=rect1.bottom-rect2.Height(); 
	rect.left=rect1.left; 
	rect.right=rect.left+rect2.Width(); 
	rect.bottom=rect1.bottom; 
	m_dlgBoardCheckEx.MoveWindow(&rect);
	//////
	m_dlgBoardCheckEx.ShowWindow(SW_SHOW);
}

void CMainFrame::ShowPixelDialog()
{
	if(m_dlgPixel.GetSafeHwnd() == NULL)
	{
		m_dlgPixel.Create(IDD_DLG_PIXEL, this);
	}
	m_dlgPixel.ShowWindow(SW_HIDE);

	CRect rect, rect2;
	m_wndRightBar.GetWindowRect(&rect);
	m_dlgPixel.GetWindowRect(&rect2);
	m_dlgPixel.MoveWindow(rect.right - rect2.Width(), rect.top, rect2.Width(), rect2.Height());

	m_dlgPixel.ShowWindow(SW_SHOW);
}
// 模板匹配界面
void CMainFrame::ShowMarkDialog(tgCmdLine *pCmdLine, int iSelIndex)
{
	if(m_dlgMark.GetSafeHwnd() == NULL)
	{
		m_dlgMark.Create(IDD_DLG_MARK, this);
	}
	m_dlgMark.ShowWindow(SW_HIDE);
	m_dlgMark.m_iSelIndex = iSelIndex;   // 指令ID
	m_dlgMark.m_pCmdLine = pCmdLine;     // 指令内容

	CRect rect, rect2;
	m_wndRightBar.GetWindowRect(&rect);
	m_dlgMark.GetWindowRect(&rect2);
	m_dlgMark.MoveWindow(rect.right - rect2.Width(), rect.top, rect2.Width(), rect2.Height());
	
	m_dlgMark.ShowWindow(SW_SHOW);
}

void CMainFrame::ShowAxisPosDialog(tgCmdLine *pCmdLine, int iSelIndex)
{
	if(m_dlgAxisPos.GetSafeHwnd() == NULL)
	{
		m_dlgAxisPos.Create(IDD_DLG_AXISPOS, this);
	}
	m_dlgAxisPos.m_iSelIndex = iSelIndex;
	m_dlgAxisPos.m_pCmdLine = pCmdLine;
	m_dlgAxisPos.RefreshAxisPos();

	m_dlgAxisPos.ShowWindow(SW_HIDE);	
	CRect rect, rect2;
	m_wndRightBar.GetWindowRect(&rect);
	m_dlgAxisPos.GetWindowRect(&rect2);
	m_dlgAxisPos.MoveWindow(rect.right - rect2.Width(), rect.top, rect2.Width(), rect2.Height());

	m_dlgAxisPos.ShowWindow(SW_SHOW);
}

// 画圆指令修改界面
void CMainFrame::ShowCircleDialog(tgCmdLine *pCmdLine, int iSelIndex)
{
	if(m_dlgCircle.GetSafeHwnd() == NULL)
	{
		m_dlgCircle.Create(IDD_DLG_CIRCLE, this);
	}

	m_dlgCircle.m_iSelIndex = iSelIndex;
	m_dlgCircle.m_pCmdLine = pCmdLine;
	m_dlgCircle.RefreshAxisPos();  // 更新轴位置

	////
	CRect rect,rect1,rect2;
	g_pView->GetWindowRect(&rect1);
	m_dlgCircle.GetWindowRect(&rect2);
	rect.top=rect1.bottom-rect2.Height(); 
	rect.left=rect1.left; 
	rect.right=rect.left+rect2.Width(); 
	rect.bottom=rect1.bottom; 
	m_dlgCircle.MoveWindow(&rect);
	//////

	m_dlgCircle.ShowWindow(SW_SHOW);
}
// 激光测高界面
void CMainFrame::ShowZHEIGHTDialog(tgCmdLine *pCmdLine, int iSelIndex)
{
	if(m_dlgZHeight.GetSafeHwnd() == NULL)
	{
		m_dlgZHeight.Create(IDD_DLG_ZHEIGHT, this);
	}
	m_dlgZHeight.m_iSelIndex = iSelIndex;
	m_dlgZHeight.m_pCmdLine = pCmdLine;
	m_dlgZHeight.RefreshAxisPos();

	////
// 	CRect rect,rect1,rect2;
// 	g_pView->GetWindowRect(&rect1);
// 	m_dlgZHeight.GetWindowRect(&rect2);
// 	rect.top=rect1.bottom-rect2.Height(); 
// 	rect.left=rect1.left; 
// 	rect.right=rect.left+rect2.Width(); 
// 	rect.bottom=rect1.bottom; 
// 	m_dlgZHeight.MoveWindow(&rect);
	//////
// 	if (m_wndEditBar.GetSafeHwnd() && m_wndEditBar.IsWindowVisible())
// 		m_dlgZHeight.CenterWindow(&m_wndEditBar);
// 	else
		
	m_dlgZHeight.CenterWindow();

	m_dlgZHeight.ShowWindow(SW_SHOW);
}

void CMainFrame::ShowRepeatZHDialog(tgCmdLine *pCmdLine, int iSelIndex)
{
	if(m_dlgRepeatZHeight.GetSafeHwnd() == NULL)
	{
		m_dlgRepeatZHeight.Create(IDD_DLG_REPEAT_ZMETER, this);
	}
	m_dlgRepeatZHeight.m_iSelIndex = iSelIndex;
	m_dlgRepeatZHeight.m_pCmdLine = pCmdLine;
	m_dlgRepeatZHeight.RefreshAxisPos();

	////
	CRect rect,rect1,rect2;
	g_pView->GetWindowRect(&rect1);
	m_dlgRepeatZHeight.GetWindowRect(&rect2);
	rect.top=rect1.bottom-rect2.Height(); 
	rect.left=rect1.left; 
	rect.right=rect.left+rect2.Width(); 
	rect.bottom=rect1.bottom; 
	m_dlgRepeatZHeight.MoveWindow(&rect);
	//////

	m_dlgRepeatZHeight.ShowWindow(SW_SHOW);
}

void CMainFrame::ShowDotParamDialog(tgCmdLine *pCmdLine, int iSelIndex/* =-1 */)
{
	if(m_dlgDotParam.GetSafeHwnd() == NULL)
	{
		m_dlgDotParam.Create(IDD_DLG_PARAM_DOT, this);
	}
// 	m_dlgDotParam.m_iSelIndex = iSelIndex;
// 	m_dlgDotParam.m_pCmdLine = pCmdLine;
// 	m_dlgDotParam.RefreshAxisPos();

	////
	CRect rect,rect1,rect2;
	g_pView->GetWindowRect(&rect1);
	m_dlgDotParam.GetWindowRect(&rect2);
	rect.top=rect1.bottom-rect2.Height(); 
	rect.left=rect1.left; 
	rect.right=rect.left+rect2.Width(); 
	rect.bottom=rect1.bottom; 
	m_dlgDotParam.MoveWindow(&rect);
	//////

	m_dlgDotParam.ShowWindow(SW_SHOW);
}

void CMainFrame::ShowLineParamDialog(tgCmdLine *pCmdLine, int iSelIndex/* =-1 */)
{
	if(m_dlgLintParam.GetSafeHwnd() == NULL)
	{
		m_dlgLintParam.Create(IDD_DLG_PARAM_LINE, this);
	}
	// 	m_dlgLintParam.m_iSelIndex = iSelIndex;
	// 	m_dlgLintParam.m_pCmdLine = pCmdLine;
	// 	m_dlgLintParam.RefreshAxisPos();


	CRect rect,rect1,rect2;
	g_pView->GetWindowRect(&rect1);
	m_dlgLintParam.GetWindowRect(&rect2);
	rect.top=rect1.bottom-rect2.Height(); 
	rect.left=rect1.left; 
	rect.right=rect.left+rect2.Width(); 
	rect.bottom=rect1.bottom; 
	m_dlgLintParam.MoveWindow(&rect);


	m_dlgLintParam.ShowWindow(SW_SHOW);
}

void CMainFrame::SetWindowTextEx()
{
	CString strTemp;
	strTemp.Format("%s [当前产品:%s]", m_pSysParam->szApp, g_AllProgInfo->m_curProgName/*g_pDoc->GetPathName()*/);
// 	if(g_pDoc != NULL)
// 	{
// 		if(g_pDoc->IsModified())
// 		{
// 			strTemp += " *";
// 		}
// 	}
	SetWindowText(strTemp);

//	strTemp.Format("%s",g_pDoc->GetPathName());
    Invalidate();
}
void CMainFrame::ShowPathDrawDlg()
{
	if(m_dlgTrackDraw.GetSafeHwnd() == NULL)
	{
		m_dlgTrackDraw.Create(IDD_DLG_TRACKDRAW, this);
	}

	m_dlgTrackDraw.CenterWindow();
	m_dlgTrackDraw.ShowWindow(SW_SHOW);
}

void CMainFrame::ShowTrackDialog()
{
// 	CDlgTrackDraw dlg;
// 	dlg.DoModal();

	if(m_dlgTrack.GetSafeHwnd() == NULL)
	{
		m_dlgTrack.Create(IDD_DLG_TRACK, this);
	}

	m_dlgTrack.CenterWindow();
	m_dlgTrack.ShowWindow(SW_SHOW);
}

void CMainFrame::ShowIODialog()
{
// 	CDlgIOCtrl dlg;
// 	dlg.DoModal();
	if(m_dlgIOCtrl.GetSafeHwnd() == NULL)
	{
		m_dlgIOCtrl.Create(IDD_DLG_IOCTRL, this);
	}
	m_dlgIOCtrl.InitTableCtrl();
	m_dlgIOCtrl.CenterWindow();
	m_dlgIOCtrl.ShowWindow(SW_SHOW);
}
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	return CFrameWndEx::PreTranslateMessage(pMsg);
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	if (K_RUN_STS_STOP != m_Robot->m_tSysStatus)	return;

	CString strtemp;
	switch(nChar)
	{
	case VK_ESCAPE:
		return;
	case VK_RETURN:	//屏蔽回车键消息
		return;
	case VK_DELETE:
		break;

	case VK_LEFT:
		if (GetAsyncKeyState(VK_LCONTROL))
		{
			if (GetAsyncKeyState(VK_LSHIFT))
				m_Robot->m_pController->AxisJog(X_AXIS, -m_mtrParamGts[X_AXIS].m_VhandMove, m_mtrParamGts[X_AXIS].m_Acc);
			else
				m_Robot->m_pController->AxisJog(X_AXIS, -g_dHandMovSpdLow, m_mtrParamGts[X_AXIS].m_Acc);
		}
		break;
	case VK_RIGHT:
		if (GetAsyncKeyState(VK_LCONTROL))
		{
			if (GetAsyncKeyState(VK_LSHIFT))
				m_Robot->m_pController->AxisJog(X_AXIS, m_mtrParamGts[X_AXIS].m_VhandMove, m_mtrParamGts[X_AXIS].m_Acc);
			else
				m_Robot->m_pController->AxisJog(X_AXIS, g_dHandMovSpdLow, m_mtrParamGts[X_AXIS].m_Acc);
		}
		break;
	case VK_UP:
		if (GetAsyncKeyState(VK_LCONTROL))
		{
			if (GetAsyncKeyState(VK_LSHIFT))
			{
				if (GetAsyncKeyState('Z'))
					m_Robot->m_pController->AxisJog(Z_AXIS, -m_mtrParamGts[Z_AXIS].m_VhandMove, m_mtrParamGts[Z_AXIS].m_Acc);
				else if (GetAsyncKeyState('W'))
					m_Robot->m_pController->AxisJog(W1_AXIS, m_mtrParamGts[W1_AXIS].m_VhandMove, m_mtrParamGts[W1_AXIS].m_Acc);
				else
					m_Robot->m_pController->AxisJog(Y_AXIS, -m_mtrParamGts[Y_AXIS].m_VhandMove, m_mtrParamGts[Y_AXIS].m_Acc);
			}
			else
			{
				if (GetAsyncKeyState('Z'))
					m_Robot->m_pController->AxisJog(Z_AXIS, -g_dHandMovSpdLow, m_mtrParamGts[Z_AXIS].m_Acc);
				else if (GetAsyncKeyState('W'))
					m_Robot->m_pController->AxisJog(W1_AXIS, g_dHandMovSpdLow, m_mtrParamGts[W1_AXIS].m_Acc);
				else
					m_Robot->m_pController->AxisJog(Y_AXIS, -g_dHandMovSpdLow, m_mtrParamGts[Y_AXIS].m_Acc);
			}
		}
		break;
	case VK_DOWN:
		if (GetAsyncKeyState(VK_LCONTROL))
		{
			if (GetAsyncKeyState(VK_LSHIFT))
			{
				if (GetAsyncKeyState('Z'))
					m_Robot->m_pController->AxisJog(Z_AXIS, m_mtrParamGts[Z_AXIS].m_VhandMove, m_mtrParamGts[Z_AXIS].m_Acc);
				else if (GetAsyncKeyState('W'))
					m_Robot->m_pController->AxisJog(W1_AXIS, -m_mtrParamGts[W1_AXIS].m_VhandMove, m_mtrParamGts[W1_AXIS].m_Acc);
				else
					m_Robot->m_pController->AxisJog(Y_AXIS, m_mtrParamGts[Y_AXIS].m_VhandMove, m_mtrParamGts[Y_AXIS].m_Acc);
			}
			else
			{
				if (GetAsyncKeyState('Z'))
					m_Robot->m_pController->AxisJog(Z_AXIS, g_dHandMovSpdLow, m_mtrParamGts[Z_AXIS].m_Acc);
				else if (GetAsyncKeyState('W'))
					m_Robot->m_pController->AxisJog(W1_AXIS, -g_dHandMovSpdLow, m_mtrParamGts[W1_AXIS].m_Acc);
				else
					m_Robot->m_pController->AxisJog(Y_AXIS, g_dHandMovSpdLow, m_mtrParamGts[Y_AXIS].m_Acc);
			}
		}
		break;
// 	case VK_PRIOR:
// 		m_Robot->Jog(Z_AXIS, CJogButton::m_dJogSpdRatio, -1,FALSE /*CJogButton::s_bEnableStep*/, CJogButton::s_fStepDist);
// 		break;
// 	case VK_NEXT:
// 		m_Robot->Jog(Z_AXIS, CJogButton::m_dJogSpdRatio, 1, FALSE/*JogButton::s_bEnableStep*/, CJogButton::s_fStepDist);
// 		break;
	case VK_F6:
		if (g_pFrm->m_pSysParam->nToolType != 0)
		{
			if(::GetKeyState(VK_SHIFT) < 0)
			{
				g_pFrm->m_pSysParam->nToolType = 0;
				strtemp = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_8");
				AfxMessageBox(strtemp);
			}
		}
		break;
	case VK_F7:
		break;
	case VK_F8:
		if (g_pFrm->m_pSysParam->nToolType != 2)
		{
			if(::GetKeyState(VK_SHIFT) < 0)
			{
				g_pFrm->m_pSysParam->nToolType = 2;
				strtemp = LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_10");
				AfxMessageBox(strtemp);
			}
		}

	default:
		return	CFrameWndEx::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	CFrameWndEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_DELETE:
		g_pFrm->m_wndEditBar.DeleteCurSel();
		break;
	case VK_LEFT:
	case VK_RIGHT:
	case VK_UP:
	case VK_DOWN:
	case VK_PRIOR:
	case VK_NEXT:
		m_Robot->m_pController->AxisStop(0x3FF);
		break;
	default:
		return CFrameWndEx::OnKeyUp(nChar, nRepCnt, nFlags);
	}
	CFrameWndEx::OnKeyUp(nChar, nRepCnt, nFlags);
}
void CMainFrame::OnCmdReopenFile()
{
	// TODO: Add your command handler code here
	m_wndEditBar.ReadCmdLine(g_pDoc->GetPathName());
	g_pDoc->SetModifiedFlag(FALSE);
}
void CMainFrame::OnClose()
{
	m_wndRightBar.m_DlgPosInfo.KillTimer(0);
	m_Robot->m_pController->SetExOutport(0, -1, FALSE);

	m_Robot->Free();
	CString str;
	str=LanguageMgr->ReadStringFromIni(DefMainFrm,"Msg_11");
	if( IDOK != AfxMessageBox(str, MB_OKCANCEL) )
	{
		return;
	}
	g_pView->ImgStop();
	CFunction::DelayExSec(0.5);

	for (int i=0; i<NUM_CHANNEL; i++)
	{
		g_AllProgInfo[i].HandleAllProg(FALSE);
	}
	
	CFrameWndEx::OnClose();
}
void CMainFrame::OnViewCmdEdit()
{
	// TODO: Add your command handler code here
	m_wndNavigationBar.ShowTab(&m_wndEditBar, TRUE, FALSE, FALSE);
}

void CMainFrame::OnLogon()
{
	// TODO: Add your command handler code here
	CDlgLogin log;
	log.DoModal();

	if(theApp.m_SysUser.m_CurUD.level > 0)
	{
		m_wndEditBar.EnableWindow(TRUE);
	}
}
void CMainFrame::OnLogoff()
{
	// TODO: Add your command handler code here
	theApp.m_SysUser.m_CurUD.level = GM_PERSONNEL;
	theApp.m_SysUser.m_CurUD.uID = 0;
}
void CMainFrame::OnRS232Config()
{
	// TODO: Add your command handler code here
	CDlgComConfig comConfig;
	if(comConfig.DoModal() == IDOK)
	{

	}
}
void CMainFrame::OnUpdateRS232Config(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
	pCmdUI->SetCheck(m_bSocketEnable);	
}

void CMainFrame::OnSocketEnable() 
{
	m_bSocketEnable=!m_bSocketEnable;
}
void CMainFrame::OnUpdateSocketEnable(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
	pCmdUI->SetCheck(m_bSocketEnable);	
}

void CMainFrame::OnDestroy()
{
	CFrameWndEx::OnDestroy();

	// TODO: Add your message handler code here

	SysParam(FALSE);
	MotorParam(FALSE);
	RobotParam(FALSE);

	DotParam(FALSE);
	LineParam(FALSE);
	//删除所设置的标记。
	::RemoveProp(m_hWnd,theApp.m_pszAppName);
}
void CMainFrame::ShowNeedleAdjustDialog()
{
	if(m_dlgNeedleAdjust.GetSafeHwnd() == NULL)
	{
		m_dlgNeedleAdjust.Create(IDD_DLG_NEEDLE_ADJUST, this);
	}
	m_dlgNeedleAdjust.CenterWindow();
	m_dlgNeedleAdjust.ShowWindow(SW_SHOW);
}

void CMainFrame::MotorParam(BOOL bRead)		// 马达参数操作
{
	bool bRtn = false;
	CString strFile = CFunction::GetDirectory() + "system\\MotorParam.csv";
	CCSVOperator CSVOperator;
	bRtn = CSVOperator.LoadCSV(strFile);
	CHECK_bool_NO_RTN_VAL("打开马达参数文件出错!", bRtn);	

	// 读取和写入马达参数时的行索引号
	int nRowIdx = 0;

	if (bRead)
	{
		MOTOR_PARAM tmpMtrParam;

		// 固高卡参数加载
		for (int i=0; i<MAX_AXIS_NUM; i++)
		{
			nRowIdx= i+2;
			std::string* pString = CSVOperator.GetString(nRowIdx, 1);
			tmpMtrParam.m_MotorName.Format(pString->c_str());	
			bRtn = CSVOperator.GetInt(nRowIdx, 2, tmpMtrParam.m_MotorID);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 3, tmpMtrParam.m_VhandMove);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 4, tmpMtrParam.m_Vmax);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 5, tmpMtrParam.m_Acc);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 6, tmpMtrParam.m_Vhome);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 7, tmpMtrParam.m_VhomeLow);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);
			bRtn = CSVOperator.GetDouble(nRowIdx, 8, tmpMtrParam.m_AccHome);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 9, tmpMtrParam.m_HomeReturn);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetInt(nRowIdx, 10, tmpMtrParam.m_HomeDir);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 11, tmpMtrParam.m_MaxTravelNegative);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 12, tmpMtrParam.m_MaxTravelPositive);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 13, tmpMtrParam.m_Resolution);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 17, tmpMtrParam.m_HomeStep);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);
			tmpMtrParam.m_MoveMode = CSVOperator.GetString(nRowIdx, 18)->c_str();	
			tmpMtrParam.m_CardType = CSVOperator.GetString(nRowIdx, 19)->c_str();		

			m_mtrParamGts[tmpMtrParam.m_MotorID] = tmpMtrParam;
		}
		// 雷赛卡参数加载
		for (int i=MAX_AXIS_NUM; i<MAX_AXIS_NUM+LS_AXIS_NUM; i++)
		{
			nRowIdx= i+2;
			std::string* pString = CSVOperator.GetString(nRowIdx, 1);
			tmpMtrParam.m_MotorName.Format(pString->c_str());	
			bRtn = CSVOperator.GetInt(nRowIdx, 2, tmpMtrParam.m_MotorID);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 3, tmpMtrParam.m_VhandMove);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 4, tmpMtrParam.m_Vmax);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 5, tmpMtrParam.m_Acc);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 6, tmpMtrParam.m_Vhome);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 7, tmpMtrParam.m_VhomeLow);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);
			bRtn = CSVOperator.GetDouble(nRowIdx, 8, tmpMtrParam.m_AccHome);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 9, tmpMtrParam.m_HomeReturn);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetInt(nRowIdx, 10, tmpMtrParam.m_HomeDir);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 11, tmpMtrParam.m_MaxTravelNegative);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 12, tmpMtrParam.m_MaxTravelPositive);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 13, tmpMtrParam.m_Resolution);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 17, tmpMtrParam.m_HomeStep);
			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);
			tmpMtrParam.m_MoveMode = CSVOperator.GetString(nRowIdx, 18)->c_str();	
			tmpMtrParam.m_CardType = CSVOperator.GetString(nRowIdx, 19)->c_str();		

			m_mtrParamLs[tmpMtrParam.m_MotorID] = tmpMtrParam;
		}
	}
	else
	{
		// 固高卡参数保存
		for (int i=0; i<MAX_AXIS_NUM; i++)
		{
			nRowIdx= i+2;
			bRtn = CSVOperator.SetString(nRowIdx, 1,m_mtrParamGts[i].m_MotorName);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 2,m_mtrParamGts[i].m_MotorID);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 3,m_mtrParamGts[i].m_VhandMove);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 4,m_mtrParamGts[i].m_Vmax);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 5,m_mtrParamGts[i].m_Acc);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 6,m_mtrParamGts[i].m_Vhome);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 7,m_mtrParamGts[i].m_VhomeLow);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 8,m_mtrParamGts[i].m_AccHome);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 9,m_mtrParamGts[i].m_HomeReturn);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 10,m_mtrParamGts[i].m_HomeDir);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 11,m_mtrParamGts[i].m_MaxTravelNegative);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 12,m_mtrParamGts[i].m_MaxTravelPositive);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 13,m_mtrParamGts[i].m_Resolution);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 17,m_mtrParamGts[i].m_HomeStep);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetString(nRowIdx, 18,m_mtrParamGts[i].m_MoveMode);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetString(nRowIdx, 19,m_mtrParamGts[i].m_CardType);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
		}
		// 雷赛卡参数保存
		for (int i=0; i<LS_AXIS_NUM; i++)
		{
			nRowIdx= i+2+MAX_AXIS_NUM;
			bRtn = CSVOperator.SetString(nRowIdx, 1,m_mtrParamLs[i].m_MotorName);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 2,m_mtrParamLs[i].m_MotorID);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 3,m_mtrParamLs[i].m_VhandMove);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 4,m_mtrParamLs[i].m_Vmax);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 5,m_mtrParamLs[i].m_Acc);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 6,m_mtrParamLs[i].m_Vhome);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 7,m_mtrParamLs[i].m_VhomeLow);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 8,m_mtrParamLs[i].m_AccHome);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 9,m_mtrParamLs[i].m_HomeReturn);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 10,m_mtrParamLs[i].m_HomeDir);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 11,m_mtrParamLs[i].m_MaxTravelNegative);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 12,m_mtrParamLs[i].m_MaxTravelPositive);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 13,m_mtrParamLs[i].m_Resolution);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 17,m_mtrParamLs[i].m_HomeStep);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetString(nRowIdx, 18,m_mtrParamLs[i].m_MoveMode);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetString(nRowIdx, 19,m_mtrParamLs[i].m_CardType);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
		}
		CSVOperator.SaveCSV(strFile);
	}
}

void CMainFrame::DotParam(BOOL bRead)		// 点参数操作
{
	bool bRtn = false;
	CString strFile = CFunction::GetDirectory() + "system\\DotParam.csv";
	CCSVOperator CSVOperator;
	bRtn = CSVOperator.LoadCSV(strFile);
	CHECK_bool_NO_RTN_VAL("打开点参数文件出错!", bRtn);	

	// 读取和写入点参数时的行索引号
	int nRowIdx = 0;

	if (bRead)
	{
		DOT_PARAM tmpDotParam;

		// 点参数加载
		for (int i=0; i<MAX_DOT_PARAM_NUM; i++)
		{
			nRowIdx= i+2;
			std::string* pString = CSVOperator.GetString(nRowIdx, 1);
			tmpDotParam.strName.Format(pString->c_str());	
			bRtn = CSVOperator.GetInt(nRowIdx, 2, tmpDotParam.nTypeID);
			CHECK_bool_NO_RTN_VAL("读取点参数ID出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 3, tmpDotParam.dfSettlingTime);
			CHECK_bool_NO_RTN_VAL("读取点参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 4, tmpDotParam.dfDownSpeed);
			CHECK_bool_NO_RTN_VAL("读取点参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 5, tmpDotParam.dfDownAccel);
			CHECK_bool_NO_RTN_VAL("读取点参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 6, tmpDotParam.dfOnTime);
			CHECK_bool_NO_RTN_VAL("读取点参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 7, tmpDotParam.dfDispenseGap);
			CHECK_bool_NO_RTN_VAL("读取点参数出错", bRtn);
			bRtn = CSVOperator.GetDouble(nRowIdx, 8, tmpDotParam.dfNumShots);
			CHECK_bool_NO_RTN_VAL("读取点参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 9, tmpDotParam.dfDwellSecs);
			CHECK_bool_NO_RTN_VAL("读取点参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 10, tmpDotParam.dfRetractDistance);
			CHECK_bool_NO_RTN_VAL("读取点参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 11, tmpDotParam.dfRetractSpeed);
			CHECK_bool_NO_RTN_VAL("读取点参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 12, tmpDotParam.dfRetractAccel);
			CHECK_bool_NO_RTN_VAL("读取点参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 13, tmpDotParam.dfSuckback);
			CHECK_bool_NO_RTN_VAL("读取点参数出错", bRtn);	
// 			bRtn = CSVOperator.GetDouble(nRowIdx, 17, tmpMtrParam.m_HomeStep);
// 			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);
// 			tmpMtrParam.m_MoveMode = CSVOperator.GetString(nRowIdx, 18)->c_str();	
// 			tmpMtrParam.m_CardType = CSVOperator.GetString(nRowIdx, 19)->c_str();		

			m_DotParam[tmpDotParam.nTypeID] = tmpDotParam;
		}
	}
	else
	{
		// 点参数保存
		for (int i=0; i<MAX_DOT_PARAM_NUM; i++)
		{
			nRowIdx= i+2;
			bRtn = CSVOperator.SetString(nRowIdx, 1,m_DotParam[i].strName);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 2,m_DotParam[i].nTypeID);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 3,m_DotParam[i].dfSettlingTime);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 4,m_DotParam[i].dfDownSpeed);
			CHECK_bool_NO_RTN_VAL("写入马达参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 5,m_DotParam[i].dfDownAccel);
			CHECK_bool_NO_RTN_VAL("写入点参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 6,m_DotParam[i].dfOnTime);
			CHECK_bool_NO_RTN_VAL("写入点参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 7,m_DotParam[i].dfDispenseGap);
			CHECK_bool_NO_RTN_VAL("写入点参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 8,m_DotParam[i].dfNumShots);
			CHECK_bool_NO_RTN_VAL("写入点参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 9,m_DotParam[i].dfDwellSecs);
			CHECK_bool_NO_RTN_VAL("写入点参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 10,m_DotParam[i].dfRetractDistance);
			CHECK_bool_NO_RTN_VAL("写入点参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 11,m_DotParam[i].dfRetractSpeed);
			CHECK_bool_NO_RTN_VAL("写入点参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 12,m_DotParam[i].dfRetractAccel);
			CHECK_bool_NO_RTN_VAL("写入点参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 13,m_DotParam[i].dfSuckback);
			CHECK_bool_NO_RTN_VAL("写入点参数出错", bRtn);
		}

		CSVOperator.SaveCSV(strFile);
	}
}

void CMainFrame::LineParam(BOOL bRead)		// 线参数操作
{
	bool bRtn = false;
	CString strFile = CFunction::GetDirectory() + "system\\LineParam.csv";
	CCSVOperator CSVOperator;
	bRtn = CSVOperator.LoadCSV(strFile);
	CHECK_bool_NO_RTN_VAL("打开线参数文件出错!", bRtn);	

	// 读取和写入线参数时的行索引号
	int nRowIdx = 0;

	if (bRead)
	{
		LINE_PARAM tmpLineParam;

		// 点参数加载
		for (int i=0; i<MAX_LINE_PARAM_NUM; i++)
		{
			nRowIdx= i+2;
			std::string* pString = CSVOperator.GetString(nRowIdx, 1);
			tmpLineParam.strName.Format(pString->c_str());	
			bRtn = CSVOperator.GetInt(nRowIdx, 2, tmpLineParam.nTypeID);
			CHECK_bool_NO_RTN_VAL("读取线参数ID出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 3, tmpLineParam.dfPremoveDelay);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 4, tmpLineParam.dfDownSpeed);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 5, tmpLineParam.dfDownAccel);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 6, tmpLineParam.dfDispenseGap);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 7, tmpLineParam.dfSpeed);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);
			bRtn = CSVOperator.GetDouble(nRowIdx, 8, tmpLineParam.dfShutOffDistance);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 9, tmpLineParam.dfSuckback);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 10, tmpLineParam.dfDwellSecs);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 11, tmpLineParam.dfRetractDistance);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 12, tmpLineParam.dfBacktrackGap);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);	
			bRtn = CSVOperator.GetDouble(nRowIdx, 13, tmpLineParam.dfBacktrackLength);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);
			bRtn = CSVOperator.GetDouble(nRowIdx, 14, tmpLineParam.dfRetractSpeed);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);
			bRtn = CSVOperator.GetDouble(nRowIdx, 15, tmpLineParam.dfRetractAccel);
			CHECK_bool_NO_RTN_VAL("读取线参数出错", bRtn);
			// 			bRtn = CSVOperator.GetDouble(nRowIdx, 17, tmpMtrParam.m_HomeStep);
			// 			CHECK_bool_NO_RTN_VAL("读取马达参数出错", bRtn);
			// 			tmpMtrParam.m_MoveMode = CSVOperator.GetString(nRowIdx, 18)->c_str();	
			// 			tmpMtrParam.m_CardType = CSVOperator.GetString(nRowIdx, 19)->c_str();		

			m_LineParm[tmpLineParam.nTypeID] = tmpLineParam;
		}
	}
	else
	{
		// 点参数保存
		for (int i=0; i<MAX_LINE_PARAM_NUM; i++)
		{
			nRowIdx= i+2;
			bRtn = CSVOperator.SetString(nRowIdx, 1,m_LineParm[i].strName);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 2,m_LineParm[i].nTypeID);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 3,m_LineParm[i].dfPremoveDelay);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 4,m_LineParm[i].dfDownSpeed);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 5,m_LineParm[i].dfDownAccel);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 6,m_LineParm[i].dfDispenseGap);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 7,m_LineParm[i].dfSpeed);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 8,m_LineParm[i].dfShutOffDistance);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 9,m_LineParm[i].dfSuckback);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 10,m_LineParm[i].dfDwellSecs);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 11,m_LineParm[i].dfRetractDistance);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 12,m_LineParm[i].dfBacktrackGap);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 13,m_LineParm[i].dfBacktrackLength);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 14,m_LineParm[i].dfRetractSpeed);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 15,m_LineParm[i].dfRetractAccel);
			CHECK_bool_NO_RTN_VAL("写入线参数出错", bRtn);
		}

		CSVOperator.SaveCSV(strFile);
	}
}
void CMainFrame::InitComPort()
{
	BOOL bRtn = FALSE;
	CString csInfo = "";

	for (int i=0; i<COM_PORT_USED; i++)
	{
		bRtn = m_ComPort[i].Init(m_ComPort[i].m_nComNo,
			m_ComPort[i].m_nBaud,
			m_ComPort[i].m_nDataBits,
			m_ComPort[i].m_nParity,
			m_ComPort[i].m_nStopBits);

		if (!bRtn)
		{
			csInfo.Format("初始化串口:[%s]失败！", m_ComPort[i].m_szPortName);
			AfxMessageBox(csInfo);
			return;	
		}
	}
}


SerialPort*	CMainFrame::GetComPort(const CString strPorName)	// 串口定义： 点胶阀1	点胶阀2	激光测高	光源控制器	称重
{
	for (int i=0; i<COM_PORT_USED; i++)
	{
		if (strPorName == m_ComPort[i].m_szPortName)
		{
			return	&m_ComPort[i];
		}
	}

	AfxMessageBox("串口: "+strPorName + " 没有定义");
	return NULL;
}

void CMainFrame::ComPortParam(BOOL bRead)	// 串口定义操作
{
	bool bRtn = false;
	CString strFile = CFunction::GetDirectory() + "system\\ComPortDefine.csv";
	CCSVOperator CSVOperator;
	bRtn = CSVOperator.LoadCSV(strFile);
	CHECK_bool_NO_RTN_VAL("打开串口配置文件出错!", bRtn);	

	// 读取和写入串口配置时的行索引号
	int nRowIdx = 0;

	if (bRead)	// 加载
	{
		CString strPortName = "";
		int nPortNo = -1;
		int nBaud = 0;
		for (int i=0; i<COM_PORT_USED; i++)
		{
			nRowIdx= i+2;
			std::string* pString = CSVOperator.GetString(nRowIdx, 1);
			strPortName.Format(pString->c_str());	
			bRtn = CSVOperator.GetInt(nRowIdx, 2, nPortNo);
			CHECK_bool_NO_RTN_VAL("读取串口参数出错", bRtn);
			bRtn = CSVOperator.GetInt(nRowIdx, 3, nBaud);
			CHECK_bool_NO_RTN_VAL("读取串口参数出错", bRtn);

			m_ComPort[i].m_szPortName = strPortName;
			m_ComPort[i].m_nComNo = nPortNo;
			m_ComPort[i].m_nBaud = nBaud;
		}
	}
	else	// Save
	{
		nRowIdx= 2;
		for (int i=0; i<COM_PORT_USED; i++)
		{
			bRtn = CSVOperator.SetString(nRowIdx, 1, m_ComPort[i].m_szPortName);
			CHECK_bool_NO_RTN_VAL("写入串口参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 2, m_ComPort[i].m_nComNo);
			CHECK_bool_NO_RTN_VAL("写入串口参数出错", bRtn);
			bRtn = CSVOperator.SetNumber(nRowIdx, 3, (int)m_ComPort[i].m_nBaud);
			CHECK_bool_NO_RTN_VAL("写入串口参数出错", bRtn);
			nRowIdx++;
		}

		CSVOperator.SaveCSV(strFile);
	}
}

void CMainFrame::RobotParam(BOOL bRead)
{
	CString strFile = CFunction::GetDirectory() + "system\\robotparam.dat";
	CString strTemp, strKey;
//	int i=0;

	// 胶量校准位置
	CFunction::HandleDouble(bRead, "WeightAdjPos", "x", g_pFrm->m_pRobotParam->m_posWeightAdj.x, strFile);
	CFunction::HandleDouble(bRead, "WeightAdjPos", "y", g_pFrm->m_pRobotParam->m_posWeightAdj.y, strFile);
	CFunction::HandleDouble(bRead, "WeightAdjPos", "z", g_pFrm->m_pRobotParam->m_posWeightAdj.z, strFile);

	// 接触测高位置
	CFunction::HandleDouble(bRead, "ContactTest", "x", g_pFrm->m_pRobotParam->m_posContactTest.x, strFile);
	CFunction::HandleDouble(bRead, "ContactTest", "y", g_pFrm->m_pRobotParam->m_posContactTest.y, strFile);
	CFunction::HandleDouble(bRead, "ContactTest", "z", g_pFrm->m_pRobotParam->m_posContactTest.z, strFile);
	
	// 激光测量接触传感器位置
	CFunction::HandleDouble(bRead, "LaserBase", "x", g_pFrm->m_pRobotParam->m_posLaserBasePos.x, strFile);
	CFunction::HandleDouble(bRead, "LaserBase", "y", g_pFrm->m_pRobotParam->m_posLaserBasePos.y, strFile);
	CFunction::HandleDouble(bRead, "LaserBase", "z", g_pFrm->m_pRobotParam->m_posLaserBasePos.z, strFile);

	// 探针测量接触传感器高度位置
	CFunction::HandleDouble(bRead, "ProbeBase", "x", g_pFrm->m_pRobotParam->m_posProbeBasePos.x, strFile);
	CFunction::HandleDouble(bRead, "ProbeBase", "y", g_pFrm->m_pRobotParam->m_posProbeBasePos.y, strFile);
	CFunction::HandleDouble(bRead, "ProbeBase", "z", g_pFrm->m_pRobotParam->m_posProbeBasePos.z, strFile);

	// 镜头观察十字线位置
	CFunction::HandleDouble(bRead, "CamereCross", "x", g_pFrm->m_pRobotParam->m_posCamereCross.x, strFile);
	CFunction::HandleDouble(bRead, "CamereCross", "y", g_pFrm->m_pRobotParam->m_posCamereCross.y, strFile);
	CFunction::HandleDouble(bRead, "CamereCross", "z", g_pFrm->m_pRobotParam->m_posCamereCross.z, strFile);

	// 激光光斑在十字线中心的位置
	CFunction::HandleDouble(bRead, "LaserCross", "x", g_pFrm->m_pRobotParam->m_posLaserCross.x, strFile);
	CFunction::HandleDouble(bRead, "LaserCross", "y", g_pFrm->m_pRobotParam->m_posLaserCross.y, strFile);
	CFunction::HandleDouble(bRead, "LaserCross", "z", g_pFrm->m_pRobotParam->m_posLaserCross.z, strFile);

	// 探针针头在十字线中心的位置
	CFunction::HandleDouble(bRead, "ProbeCross", "x", g_pFrm->m_pRobotParam->m_posProbeCross.x, strFile);
	CFunction::HandleDouble(bRead, "ProbeCross", "y", g_pFrm->m_pRobotParam->m_posProbeCross.y, strFile);
	CFunction::HandleDouble(bRead, "ProbeCross", "z", g_pFrm->m_pRobotParam->m_posProbeCross.z, strFile);

	// 清洗喷嘴位置
	CFunction::HandleDouble(bRead, "Clean", "x", g_pFrm->m_pRobotParam->m_structClean.m_posClean.x, strFile);
	CFunction::HandleDouble(bRead, "Clean", "y", g_pFrm->m_pRobotParam->m_structClean.m_posClean.y, strFile);
	CFunction::HandleDouble(bRead, "Clean", "z", g_pFrm->m_pRobotParam->m_structClean.m_posClean.z, strFile);
	// 点胶时自动清洗间隔
	CFunction::HandleInt(bRead, "AuotClean", "Interval", g_pFrm->m_pRobotParam->m_structClean.m_nCleanInterval, strFile);
	// 点胶过程中是否自动清洗	
	CFunction::HandleInt(bRead, "AuotClean", "IfAutoClean", g_pFrm->m_pRobotParam->m_structClean.m_nIfAutoClean, strFile);
	// 清洗次数
	CFunction::HandleInt(bRead, "AuotClean", "CleanTimes", g_pFrm->m_pRobotParam->m_structClean.m_nCleanTimes, strFile);
	// 针嘴清洗时,下降高度
	CFunction::HandleDouble(bRead, "AuotClean", "CleanDownMM", g_pFrm->m_pRobotParam->m_structClean.m_dCleanDownMM, strFile);
	
	// 4点校正位置
	CFunction::HandleDouble(bRead, "JetAdjust", "x", g_pFrm->m_pRobotParam->m_posJetAdjust.x, strFile);
	CFunction::HandleDouble(bRead, "JetAdjust", "y", g_pFrm->m_pRobotParam->m_posJetAdjust.y, strFile);
	CFunction::HandleDouble(bRead, "JetAdjust", "z", g_pFrm->m_pRobotParam->m_posJetAdjust.z, strFile);
	
	// 激光测量十字线的读数
	CFunction::HandleDouble(bRead, "LaserCrossVal", "LaserCrossVal", g_pFrm->m_pRobotParam->m_zLaserCrossVal, strFile);

	// 探针测量十字线的读数
	CFunction::HandleDouble(bRead, "LaserCrossVal", "LaserCrossVal", g_pFrm->m_pRobotParam->m_zProbeCrossVal, strFile);

	// 接触测高的系统偏差
	CFunction::HandleDouble(bRead, "Z_SysHeightOffset", "ZSysHeightOffset", g_pFrm->m_pRobotParam->m_zSysHeightOffset, strFile);
	// Z轴安全高度
	CFunction::HandleDouble(bRead, "Z_SafePos", "zSafepos", g_pFrm->m_pRobotParam->m_zSafePos, strFile);

	// Z轴接触高度
	CFunction::HandleDouble(bRead, "Z_ContactPos", "zContactpos", g_pFrm->m_pRobotParam->m_zContactPos, strFile);
	
	// 激光测高标准高度值
	CFunction::HandleDouble(bRead, "LaserBaseVal", "LaserBaseVal", g_pFrm->m_pRobotParam->m_zLaserBaseVal, strFile);

	// 探针测量接触传感器的高度.
	CFunction::HandleDouble(bRead, "ProbeBaseVal", "ProbeBaseVal", g_pFrm->m_pRobotParam->m_zProbeBaseVal, strFile);

	// 胶桶背压
	CFunction::HandleDouble(bRead, "BackPress", "BackPress", g_pFrm->m_pRobotParam->m_dBackPress, strFile);

	// 当前使用的胶阀类型	m_eValveType
	CFunction::HandleInt(bRead, "CurDispType", "DispType", g_pFrm->m_pRobotParam->m_nValveType, strFile);

	// 点胶阀控制方式
	CFunction::HandleInt(bRead, "CurDispCtrl", "DispCtrl", g_pFrm->m_pRobotParam->m_nValveCtrlMode, strFile);

	// 称重胶滴数
	CFunction::HandleInt(bRead, "WeightAdjDotNum", "WeightAdjDotNum", g_pFrm->m_pRobotParam->m_nWeightAdjDotNum, strFile);
	
	// 单点胶重
	CFunction::HandleDouble(bRead, "AvgWeight", "AvgWeight", g_pFrm->m_pRobotParam->m_dAvgWeight, strFile);	

	// 要板时间
	CFunction::HandleDouble(bRead, "TimeAskPre", "TimeAskPre", g_pFrm->m_pRobotParam->m_dTimeAskPre, strFile);	

	// 外挂辅助配置
	CFunction::HandleInt(bRead, "AddOn", "AddOn", g_pFrm->m_pRobotParam->m_nAddOn, strFile);
	// 物料定位方式
	CFunction::HandleInt(bRead, "FixMode", "FixMode", g_pFrm->m_pRobotParam->m_nFixMode, strFile);

	// 是否启用胶量报警
	CFunction::HandleInt(bRead, "GlueWarning", "IfGlueWarning", g_pFrm->m_pRobotParam->m_structGlueWarning.m_nIfGlueWarning, strFile);
	// 胶量报警模式		0:传感器检测胶量	1:预设好板数	2:预设好过期时间
	CFunction::HandleInt(bRead, "GlueWarning", "WarningMode", g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMode, strFile);
	// 胶量报警:可使用的板数目
	CFunction::HandleInt(bRead, "GlueWarning", "WarningPcs", g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningPcs, strFile);
	// 胶量报警:胶水有效时间
	CFunction::HandleInt(bRead, "GlueWarning", "WarningMin", g_pFrm->m_pRobotParam->m_structGlueWarning.m_nGlueWarningMin, strFile);
	
	// 废胶报警重量
	CFunction::HandleDouble(bRead, "WasteGlue", "AlarmWt", g_pFrm->m_pRobotParam->m_dWasteGlueAlarm, strFile);	
	// 当前废胶盒中胶重
	CFunction::HandleDouble(bRead, "WasteGlue", "CurWt", g_pFrm->m_pRobotParam->m_dWasteGlueCur, strFile);		

	// 进出料方式	0:在线式自动进出料		1:离线式3层料盒
	CFunction::HandleInt(bRead, "LoadUnloadMode", "Mode", g_pFrm->m_pRobotParam->m_nLoadUnloadMode, strFile);

	// 测高方式，修改后需要重启程序		 测高方式	0:激光测高		1:探针测高
	CFunction::HandleInt(bRead, "HeightReadMode", "Mode", g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode, strFile);
	// 探针高速
	CFunction::HandleDouble(bRead, "HeightReadMode", "ProbeHSpeed", g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeHSpd, strFile);
	// 探针低速
	CFunction::HandleDouble(bRead, "HeightReadMode", "ProbeLSpeed", g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeLSpd, strFile);
	// 低速高度
	CFunction::HandleDouble(bRead, "HeightReadMode", "ProbeLHeight", g_pFrm->m_pRobotParam->m_structHeightRead.m_dProbeLHeight, strFile);

	//////////////////////////////////////////////////////////////////////////
	// 是否启用自动试胶
	CFunction::HandleInt(bRead, "TestGlue", "Mode", g_pFrm->m_pRobotParam->m_structTestGlue.m_nIfTestGlue, strFile);
	// 胶线长度
	CFunction::HandleInt(bRead, "TestGlue", "Length", g_pFrm->m_pRobotParam->m_structTestGlue.m_nGlueLen, strFile);
	// 胶线距离
	CFunction::HandleInt(bRead, "TestGlue", "Space", g_pFrm->m_pRobotParam->m_structTestGlue.m_nGLueSpace, strFile);
	// 试胶线速度
	CFunction::HandleDouble(bRead, "TestGlue", "WorkVel", g_pFrm->m_pRobotParam->m_structTestGlue.m_dWorkVel, strFile);
	// 陶瓷板左上角坐标
	CFunction::HandleDouble(bRead, "TestGlue", "posTopLeft_x", g_pFrm->m_pRobotParam->m_structTestGlue.m_posTopLeft.x, strFile);
	CFunction::HandleDouble(bRead, "TestGlue", "posTopLeft_y", g_pFrm->m_pRobotParam->m_structTestGlue.m_posTopLeft.y, strFile);
	// 陶瓷板左上角坐标
	CFunction::HandleDouble(bRead, "TestGlue", "posButRight_x", g_pFrm->m_pRobotParam->m_structTestGlue.m_posButRight.x, strFile);
	CFunction::HandleDouble(bRead, "TestGlue", "posButRight_y", g_pFrm->m_pRobotParam->m_structTestGlue.m_posButRight.y, strFile);
	//////////////////////////////////////////////////////////////////////////

	// 相机配置
	CFunction::HandleInt(bRead, "Camera_Type", "Type", g_pFrm->m_pRobotParam->m_nCameraType, strFile);
	
	// 工作模式
	CFunction::HandleInt(bRead, "WorkMode", "Mode", g_pFrm->m_pRobotParam->m_nWorkMode, strFile);

	// 料盒层数
	CFunction::HandleInt(bRead, "LoadUnloadMode", "LoadLayerCount", g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdLayerCount, strFile);
	CFunction::HandleInt(bRead, "LoadUnloadMode", "UnloadLayerCount", g_pFrm->m_pRobotParam->m_structLdUldParam.m_nUldLayerCount, strFile);
	CFunction::HandleDouble(bRead, "LoadUnloadMode", "UnloadDelay", g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldDelay, strFile);	

	// 推料没感应到,重复推料次数
	CFunction::HandleInt(bRead, "LoadUnloadMode", "LoadRePush", g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdRePushout, strFile);
	// 连续几层没推到料片报警
	CFunction::HandleInt(bRead, "LoadUnloadMode", "LoadEmptyWarning", g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdEmptyWarning, strFile);
	// 进料是否3层循环不报警
	CFunction::HandleInt(bRead, "LoadUnloadMode", "LoadContinue", g_pFrm->m_pRobotParam->m_structLdUldParam.m_nLdContinue, strFile);
	for (int i=0; i<6; i++)
	{ 
		CString str;
		str.Format("LoadBoxPos_%d", i);
		CFunction::HandleDouble(bRead, "LoadUnloadMode", str, g_pFrm->m_pRobotParam->m_structLdUldParam.m_dLdPos[i], strFile);
		str.Format("UnloadBoxPos_%d", i);
		CFunction::HandleDouble(bRead, "LoadUnloadMode", str, g_pFrm->m_pRobotParam->m_structLdUldParam.m_dUldPos[i], strFile);
	}

	// 是否自动称重
	CFunction::HandleInt(bRead, "WeightParam", "IfAutoWeight", g_pFrm->m_pRobotParam->m_nIfAutoWeight, strFile);

	// 自动称重间隔类型
	CFunction::HandleInt(bRead, "WeightParam", "WeightIntervalType", (int)g_pFrm->m_pRobotParam->m_nWeightIntervalType, strFile);

	// 自动称重间隔(时间:分钟  板数:pcs)
	CFunction::HandleInt(bRead, "WeightParam", "WeightIntervalValue", g_pFrm->m_pRobotParam->m_nWeightIntervalValue, strFile);

	// 是否启用防固化自动喷胶
	CFunction::HandleInt(bRead, "AuotRps", "EnableAutoRps", g_pFrm->m_pRobotParam->m_nIfAutoRps, strFile);
	// 防固化自动喷胶间隔
	CFunction::HandleDouble(bRead, "AuotRps", "Interval", g_pFrm->m_pRobotParam->m_dAutoRpsIntervalMin, strFile);	
	// 防固化自动喷胶胶点数目
	CFunction::HandleInt(bRead, "AuotRps", "Dots", g_pFrm->m_pRobotParam->m_nAutoRpsDots, strFile);

	// 是否检测安全门
	CFunction::HandleInt(bRead, "SafeDoor", "CheckSafeDoor", g_pFrm->m_pRobotParam->m_nIfCheckSafeDoor, strFile);

	// 使用的阀数目
	CFunction::HandleInt(bRead, "ValveNum", "ValveNum", g_pFrm->m_pRobotParam->m_nValveNum, strFile);
	// 是否使用底部相机
	CFunction::HandleInt(bRead, "IfUseCameraNeedle", "IfUseCameraNeedle", g_pFrm->m_pRobotParam->m_nIfUseCameraNeedle, strFile);
	// 是否使用底部旋转
	CFunction::HandleInt(bRead, "IfRotateWorkpiece", "IfRotateWorkpiece", g_pFrm->m_pRobotParam->m_nIfRotateWorkpiece, strFile);

	// 工件调平Mark1位置
	CFunction::HandleDouble(bRead, "posRotateMark1", "x", g_pFrm->m_pRobotParam->m_posRotateMark1.x, strFile);
	CFunction::HandleDouble(bRead, "posRotateMark1", "y", g_pFrm->m_pRobotParam->m_posRotateMark1.y, strFile);
	CFunction::HandleDouble(bRead, "posRotateMark1", "z", g_pFrm->m_pRobotParam->m_posRotateMark1.z, strFile);
	// 工件调平Mark2位置
	CFunction::HandleDouble(bRead, "posRotateMark2", "x", g_pFrm->m_pRobotParam->m_posRotateMark2.x, strFile);
	CFunction::HandleDouble(bRead, "posRotateMark2", "y", g_pFrm->m_pRobotParam->m_posRotateMark2.y, strFile);
	CFunction::HandleDouble(bRead, "posRotateMark2", "z", g_pFrm->m_pRobotParam->m_posRotateMark2.z, strFile);
	// 下相机阀1位置
	CFunction::HandleDouble(bRead, "posCCDValve1", "x", g_pFrm->m_pRobotParam->m_posCCDValve1.x, strFile);
	CFunction::HandleDouble(bRead, "posCCDValve1", "y", g_pFrm->m_pRobotParam->m_posCCDValve1.y, strFile);
	CFunction::HandleDouble(bRead, "posCCDValve1", "z", g_pFrm->m_pRobotParam->m_posCCDValve1.z, strFile);
	// 阀间距离
	CFunction::HandleDouble(bRead, "ValveSpace", "ValveSpace", g_pFrm->m_pRobotParam->m_dValveSpace, strFile);
}

void CMainFrame::SysParam(BOOL bRead)
{
	CString strFile = CFunction::GetDirectory() + "system\\sysparam.dat";
	CString strTemp, strKey;
	int i=0;

    CFunction::HandleInt(bRead, "SYSTEM", "bBalanceEnable", g_pFrm->m_pSysParam->bBalanceEnable, strFile);
	CFunction::HandleString(bRead, "APPLICATION", "NAME", g_pFrm->m_pSysParam->szApp, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "LANGUAGE", g_pFrm->m_pSysParam->nLanguageType, strFile);
	CFunction::HandleDouble(bRead,"SYSTEM","UnloadHeartTime",g_pFrm->m_pSysParam->dBeltUnloadDelay,strFile);
// 	for(int i=0; i<MAX_AXIS_NUM; i++)
// 	{
// 		strKey.Format("SCALE%d",i);
// 		CFunction::HandleDouble(bRead, "SYSTEM", strKey, g_pFrm->m_pSysParam->tAxis[i].scale, strFile);
// 
// 		strKey.Format("MAXPOS%d",i);
// 		CFunction::HandleDouble(bRead, "SYSTEM", strKey, g_pFrm->m_mtrParam[i].m_MaxTravelPositive, strFile);
// 
// 		strKey.Format("MINPOS%d", i);
// 		CFunction::HandleDouble(bRead, "SYSTEM", strKey, g_pFrm->m_mtrParam[i].m_MaxTravelNegative, strFile);
// 
// 		strKey.Format("ACC%d", i);
// 		CFunction::HandleDouble(bRead, "SPEED", strKey, g_pFrm->m_mtrParam[i].m_Acc, strFile);
// 	}
    
	for(i=0; i<TOOL_NUM; i++)
	{//针头与图像中心偏移
		strKey.Format("tDeltaOffPosX%d", i);
		CFunction::HandleDouble(bRead, "OFFSET", strKey, g_pFrm->m_pSysParam->tOffset[i].x, strFile);
		strKey.Format("tDeltaOffPosY%d", i);
		CFunction::HandleDouble(bRead, "OFFSET", strKey, g_pFrm->m_pSysParam->tOffset[i].y, strFile);
		strKey.Format("tDeltaOffPosZ%d", i);
		CFunction::HandleDouble(bRead, "OFFSET", strKey, g_pFrm->m_pSysParam->tOffset[i].z, strFile);
	}

	for(i=0; i<2; i++)
	{
		strKey.Format("tNeedleAdjustPosX%d", i);
		CFunction::HandleDouble(bRead, "CaliPos", strKey, g_pFrm->m_pSysParam->tNeedleAdjustPos[i].x, strFile);
		strKey.Format("tNeedleAdjustPosY%d", i);
		CFunction::HandleDouble(bRead, "CaliPos", strKey, g_pFrm->m_pSysParam->tNeedleAdjustPos[i].y, strFile);
		strKey.Format("tNeedleAdjustPosZ%d", i);
		CFunction::HandleDouble(bRead, "CaliPos", strKey, g_pFrm->m_pSysParam->tNeedleAdjustPos[i].z, strFile);
	}
	CFunction::HandleDouble(bRead, "CaliPos", "tDefHomePosX", g_pFrm->m_pSysParam->tDefHomePos.x, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tDefHomePosY", g_pFrm->m_pSysParam->tDefHomePos.y, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tDefHomePosZ", g_pFrm->m_pSysParam->tDefHomePos.z, strFile);

// 	CFunction::HandleDouble(bRead, "CaliPos", "tWashPosX", g_pFrm->m_pSysParam->tWashPos.x, strFile);
// 	CFunction::HandleDouble(bRead, "CaliPos", "tWashPosY", g_pFrm->m_pSysParam->tWashPos.y, strFile);
// 	CFunction::HandleDouble(bRead, "CaliPos", "tWashPosZ", g_pFrm->m_pSysParam->tWashPos.z, strFile);
	
    CFunction::HandleDouble(bRead, "CaliPos", "tDispTestPosX", g_pFrm->m_pSysParam->tDispTestPos.x, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tDispTestPosY", g_pFrm->m_pSysParam->tDispTestPos.y, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tDispTestPosZ", g_pFrm->m_pSysParam->tDispTestPos.z, strFile);
	
	CFunction::HandleDouble(bRead, "CaliPos", "tNeedleZAdjustPosX", g_pFrm->m_pSysParam->tNeedleZAdjustPos.x, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tNeedleZAdjustPosY", g_pFrm->m_pSysParam->tNeedleZAdjustPos.y, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tNeedleZAdjustPosZ", g_pFrm->m_pSysParam->tNeedleZAdjustPos.z, strFile);

	CFunction::HandleDouble(bRead, "CaliPos", "tJetZAdjustPosX", g_pFrm->m_pSysParam->tJetZAdjustPos.x, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tJetZAdjustPosY", g_pFrm->m_pSysParam->tJetZAdjustPos.y, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tJetZAdjustPosZ", g_pFrm->m_pSysParam->tJetZAdjustPos.z, strFile);

	CFunction::HandleDouble(bRead, "CaliPos", "tBoardCalibAX", g_pFrm->m_pSysParam->tBoardCalib_A.x, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tBoardCalibAY", g_pFrm->m_pSysParam->tBoardCalib_A.y, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tBoardCalibAZ", g_pFrm->m_pSysParam->tBoardCalib_A.z, strFile);

	CFunction::HandleDouble(bRead, "CaliPos", "tBoardCalibBX", g_pFrm->m_pSysParam->tBoardCalib_B.x, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tBoardCalibBY", g_pFrm->m_pSysParam->tBoardCalib_B.y, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tBoardCalibBZ", g_pFrm->m_pSysParam->tBoardCalib_B.z, strFile);

	//CFunction::HandleDouble(bRead, "CaliPos", "dBoardArc", g_pFrm->m_pSysParam->m_BoardArc, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tPixelAdjustPosX", g_pFrm->m_pSysParam->tPixelAdjustPos.x, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tPixelAdjustPosY", g_pFrm->m_pSysParam->tPixelAdjustPos.y, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "tPixelAdjustPosZ", g_pFrm->m_pSysParam->tPixelAdjustPos.z, strFile);

	CFunction::HandleDouble(bRead, "CaliPos", "dCaliJetDelta", g_pFrm->m_pSysParam->dCaliJetDelta, strFile);
	CFunction::HandleDouble(bRead, "CaliPos", "dCaliNeedleDelta", g_pFrm->m_pSysParam->dCaliNeedleDelta, strFile);

// 	CFunction::HandleDouble(bRead, "CaliPos", "tJetAdjustPosX", g_pFrm->m_pSysParam->tJetAdjustPos.x, strFile);
// 	CFunction::HandleDouble(bRead, "CaliPos", "tJetAdjustPosY", g_pFrm->m_pSysParam->tJetAdjustPos.y, strFile);
// 	CFunction::HandleDouble(bRead, "CaliPos", "tJetAdjustPosZ", g_pFrm->m_pSysParam->tJetAdjustPos.z, strFile);

	CFunction::HandleDouble(bRead, "SYSTEM", "PIXELSCALE",g_pFrm->m_pSysParam->dPixelScale, strFile);
	CFunction::HandleDouble(bRead, "SYSTEM", "PIXELANGLE",g_pFrm->m_pSysParam->dPixelAngle, strFile);
	CFunction::HandleLong(bRead, "SYSTEM", "nJetPulse",g_pFrm->m_pSysParam->nJetPulse, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "nOpenTime",g_pFrm->m_pSysParam->nOpenTime, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "nCloseTime",g_pFrm->m_pSysParam->nCloseTime, strFile);
	CFunction::HandleDouble(bRead, "SYSTEM", "dPressJet",g_pFrm->m_pSysParam->dPressJet, strFile);
	CFunction::HandleDouble(bRead, "SYSTEM", "dPressDisp",g_pFrm->m_pSysParam->dPressDisp, strFile);
	CFunction::HandleDouble(bRead, "SYSTEM", "dJetSetTemperature",g_pFrm->m_pSysParam->dJetSetTemperature, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "nAutoMode",g_pFrm->m_pSysParam->nAutoMode, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "WashPinCycle",g_pFrm->m_pSysParam->uiWashPinCycle, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "nToolType",g_pFrm->m_pSysParam->nToolType, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "nGoStandByPos",g_pFrm->m_pSysParam->nGoStandByPos, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "nCtrlMode",g_pFrm->m_pSysParam->nRobotCtrlMode, strFile);
	
	CFunction::HandleDouble(bRead, "SYSTEM", "dJetZBaseHightValue",g_pFrm->m_pSysParam->dJetZBaseHightValue, strFile);
	
	CFunction::HandleDouble(bRead, "SYSTEM", "dWeightLmtMin",g_pFrm->m_pSysParam->dWeightLmtMin, strFile);
	CFunction::HandleDouble(bRead, "SYSTEM", "dWeightLmtMax",g_pFrm->m_pSysParam->dWeightLmtMax, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "bEableWeightAlm",g_pFrm->m_pSysParam->bEableWeightAlm, strFile);

	CFunction::HandleInt(bRead, "SYSTEM", "WashPinNum",g_pFrm->m_pSysParam->iWashNum,strFile); 
	CFunction::HandleInt(bRead, "SYSTEM", "WashType",g_pFrm->m_pSysParam->iWashType,strFile); 
	
	CFunction::HandleDouble(bRead, "QInjectPara", "dAutoPRSdelay",g_pFrm->m_pSysParam->dAutoPRSdelay,strFile); 
	CFunction::HandleDouble(bRead, "QInjectPara", "dPRSTimes",g_pFrm->m_pSysParam->dPRSTimes, strFile);
	CFunction::HandleDouble(bRead, "QInjectPara", "dSPRSDelay",g_pFrm->m_pSysParam->dSPRSDelay, strFile);
	CFunction::HandleDouble(bRead, "QInjectPara", "dSPRSTime",g_pFrm->m_pSysParam->dSPRSTime, strFile);
   
	CFunction::HandleDouble(bRead, "QInjectPara", "tAutoPfsPosX", g_pFrm->m_pSysParam->tAutoPfsPos.x, strFile);
	CFunction::HandleDouble(bRead, "QInjectPara", "tAutoPfsPosY", g_pFrm->m_pSysParam->tAutoPfsPos.y, strFile);
	CFunction::HandleDouble(bRead, "QInjectPara", "tAutoPfsPosZ", g_pFrm->m_pSysParam->tAutoPfsPos.z, strFile);


	CFunction::HandleInt(bRead, "SYSTEM", "iXDir",g_pFrm->m_pSysParam->iXDir, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "iYDir",g_pFrm->m_pSysParam->iYDir, strFile);

	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMaxJogXY",g_pFrm->m_pSysParam->dSpeedMaxJogXY, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMaxJogZ",g_pFrm->m_pSysParam->dSpeedMaxJogZ, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMaxJogU",g_pFrm->m_pSysParam->dSpeedMaxJogU, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMaxJogVW",g_pFrm->m_pSysParam->dSpeedMaxJogVW, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMaxJogMN",g_pFrm->m_pSysParam->dSpeedMaxJogMN, strFile);

	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMoveXY",g_pFrm->m_pSysParam->dSpeedMoveXY, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMoveZ",g_pFrm->m_pSysParam->dSpeedMoveZ, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMoveU",g_pFrm->m_pSysParam->dSpeedMoveU, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMoveVW",g_pFrm->m_pSysParam->dSpeedMoveVW, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMoveMN",g_pFrm->m_pSysParam->dSpeedMoveMN, strFile);

	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMovetoXY",g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMovetoZ",g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMovetoU",g_pFrm->m_pSysParam->dSpeedMovetoU, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMovetoVW",g_pFrm->m_pSysParam->dSpeedMovetoVW, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedMovetoMN",g_pFrm->m_pSysParam->dSpeedMovetoMN, strFile);

	CFunction::HandleDouble(bRead, "SPEED", "dSpeedImageMoveXY",g_pFrm->m_pSysParam->dSpeedImageMoveXY, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedImageMoveZ",g_pFrm->m_pSysParam->dSpeedImageMoveZ, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedDisp",g_pFrm->m_pSysParam->dSpeedDisp, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedHomeH1",g_pFrm->m_pSysParam->dSpeedHomeH1, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedHomeH2",g_pFrm->m_pSysParam->dSpeedHomeH2, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedHomeL",g_pFrm->m_pSysParam->dSpeedHomeL, strFile);

	CFunction::HandleDouble(bRead, "SPEED", "dSpeedFeedHomeH",g_pFrm->m_pSysParam->dSpeedHomeLoadH, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedFeedHomeM",g_pFrm->m_pSysParam->dSpeedHomeLoadM, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedFeedHomeL",g_pFrm->m_pSysParam->dSpeedHomeLoadL, strFile);

	CFunction::HandleDouble(bRead, "SPEED", "dSpeedFeedUpVel",g_pFrm->m_pSysParam->dSpeedFeedUpVel, strFile);
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedFeedDownVel",g_pFrm->m_pSysParam->dSpeedFeedDownVel, strFile);
	
	CFunction::HandleDouble(bRead, "SPEED", "dSpeedBelt",g_pFrm->m_pSysParam->dSpeedBelt, strFile);
	CFunction::HandleDouble(bRead, "POS", "dCaliNeedleStdHeight",g_pFrm->m_pSysParam->dCaliNeedleStdHeight, strFile);
	CFunction::HandleDouble(bRead, "POS", "dCaliJetStdHeight",g_pFrm->m_pSysParam->dCaliJetStdHeight, strFile);

	CFunction::HandleInt(bRead, "LOOK", "NUM", g_pFrm->m_pSysParam->iLookNum, strFile);
	CFunction::HandleDouble(bRead, "LOOK", "LOOKACC", g_pFrm->m_pSysParam->dLookAcc, strFile);
	CFunction::HandleDouble(bRead, "LOOK", "LOOKMAXACC", g_pFrm->m_pSysParam->dLookMaxAcc, strFile);
	CFunction::HandleDouble(bRead, "LOOK", "LOOKTUNSCALE", g_pFrm->m_pSysParam->m_LookTunScale, strFile);
	CFunction::HandleInt(bRead, "LOOK", "LOOKEVENTIME", g_pFrm->m_pSysParam->m_LookEvenTime, strFile);

	CFunction::HandleDouble(bRead, "CCDParam", "CCDMOVEDIS", g_pFrm->m_pSysParam->m_ccdMoveDis, strFile);
	CFunction::HandleInt(bRead, "CCDParam", "CCDMOVECNT", g_pFrm->m_pSysParam->m_ccdMoveCnt, strFile);
	CFunction::HandleInt(bRead, "CCDParam", "iMarkNum", g_pFrm->m_pSysParam->iMarkNum, strFile);
	CFunction::HandleDouble(bRead, "CCDParam", "dDrawCircleR1", g_pFrm->m_pSysParam->dDrawCircleR1, strFile);
	CFunction::HandleInt(bRead, "CCDParam", "dDrawCircleR2", g_pFrm->m_pSysParam->dDrawCircleR2, strFile);
	CFunction::HandleInt(bRead, "CCDParam", "nCCDType",g_pFrm->m_pSysParam->nCCDType, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "nFindMarkJogMove",g_pFrm->m_pSysParam->nFindMarkJogMove, strFile);
//	CFunction::HandleInt(bRead, "SYSTEM", "nWorkMode",g_pFrm->m_pSysParam->nWorkMode, strFile);
	CFunction::HandleDouble(bRead, "SYSTEM", "nJetDelay",g_pFrm->m_pSysParam->dJetOpenDelay, strFile);
	
	CFunction::HandleInt(bRead, "SYSTEM", "nSFDoorEnable",g_pFrm->m_pSysParam->nSFDoorEnable, strFile);
	
	CFunction::HandleInt(bRead, "SYSTEM", "nLaserReadPortID",g_pFrm->m_pSysParam->nLaserReadPortID, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "nFileSeparaEnable",g_pFrm->m_pSysParam->nFileSeparaEnable, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "nFileScrapeEnable",g_pFrm->m_pSysParam->nFileScrapeEnable, strFile);
	CFunction::HandleInt(bRead, "SYSTEM", "nFileScrapeNum",g_pFrm->m_pSysParam->nFileScrapeNum, strFile);
	CFunction::HandleDouble(bRead, "SYSTEM", "dNeedleAdjustUpHeight",g_pFrm->m_pSysParam->dNeedleAdjustUpHeight, strFile);
	CFunction::HandleDouble(bRead, "SYSTEM", "dNeedleAdjustDispTime",g_pFrm->m_pSysParam->dNeedleAdjustDispTime, strFile);
	CFunction::HandleDouble(bRead, "SYSTEM", "dNeedleAdjustWaitTime",g_pFrm->m_pSysParam->dNeedleAdjustWaitTime, strFile);
	 
	CFunction::HandleInt(bRead, "SYSTEM", "bEnablePrecFun",g_pFrm->m_pSysParam->bEnablePrecFun, strFile);

	for (int i=0;i<3;i++)//pid参数
	{
		strKey.Format("PID_P%d", i);
		CFunction::HandleDouble(bRead, "PID", strKey, m_pSysParam->PIDParam[i].PID_P, strFile);
		strKey.Format("PID_I%d", i);
		CFunction::HandleDouble(bRead, "PID", strKey, m_pSysParam->PIDParam[i].PID_I, strFile);
		strKey.Format("PID_D%d", i);
		CFunction::HandleDouble(bRead, "PID", strKey, m_pSysParam->PIDParam[i].PID_D, strFile);
		
		strKey.Format("PVF_%d", i);
		CFunction::HandleDouble(bRead, "PID", strKey, m_pSysParam->PIDParam[i].PrfVelFeilter, strFile);

		strKey.Format("KAF_%d", i);
		CFunction::HandleDouble(bRead, "PID", strKey, m_pSysParam->PIDParam[i].Kaff, strFile);

		strKey.Format("KVF_%d", i);
		CFunction::HandleDouble(bRead, "PID", strKey, m_pSysParam->PIDParam[i].Kvff, strFile);

	}

	// 上下料机构使能
	CFunction::HandleInt(bRead, "FEEDLOAD", "EnableMDL", g_pFrm->m_pSysParam->bLoadSysEnaLoad, strFile);
	// 天平参数
	CFunction::HandleDouble(bRead, "BANLACESET", "BCDISPPOSX", g_pFrm->m_pSysParam->tBCTestPos.x, strFile);
	CFunction::HandleDouble(bRead, "BANLACESET", "BCDISPPOSY", g_pFrm->m_pSysParam->tBCTestPos.y, strFile);
	CFunction::HandleDouble(bRead, "BANLACESET", "BCDISPPOSZ", g_pFrm->m_pSysParam->tBCTestPos.z, strFile);
	CFunction::HandleDouble(bRead, "BANLACESET", "BCDISPTIME", g_pFrm->m_pSysParam->dBCDispTime, strFile);
	CFunction::HandleDouble(bRead, "BANLACESET", "BCDISPDIS", g_pFrm->m_pSysParam->dBCDispDis, strFile);
	CFunction::HandleDouble(bRead, "BANLACESET", "BCDISPNUM", g_pFrm->m_pSysParam->dBCDispNum, strFile);
	CFunction::HandleDouble(bRead, "BANLACESET", "BCMGPORDOT", g_pFrm->m_pSysParam->dBCMGPorDOT, strFile);
}

// 输入事件
LRESULT CMainFrame::OnMessageInputEvent(WPARAM wparam, LPARAM lparam)
{	
	int inNo = (int)(wparam);
	int sts  = (int)(lparam);

    CString strtemp;
	if((inNo<0)||(inNo>=(IN_POINT+EX_IN_POINT)))
		return 0;

	CString str = m_Robot->m_InPutCtrl[inNo].m_strIOName;

	if(str == _T("急停按钮"))           // 系统急停
	{
		m_Robot->Emergency();
	}	
	else if((_T("前门报警")==str) || (_T("后门报警")==str))   // 安全门信号
	{	//Stop
		if(1 == g_pFrm->m_pRobotParam->m_nIfCheckSafeDoor)
		{
			if(sts == -2) 
			{
				if (m_Robot->m_tSysStatus==K_RUN_STS_RUN)
				{
					m_Robot->m_bIsHandPause = true;
					m_Robot->Pause();
				}
			}else if (sts == 2)
			{
				if (m_Robot->m_tSysStatus==K_RUN_STS_PAUSE)
				{
					m_Robot->Run();
				}
			}else if (sts == 0)
			{
				// 门打开状态
				m_Robot->m_bDoorSFSts=FALSE;
			}else if(sts == 1)
			{  
				// 门关闭状态
				m_Robot->m_bDoorSFSts=TRUE;
			}
		}
	}
//	else if(str == _T("自动运行"))      // 启动运行按钮
// 	{
// 		if(g_pFrm->m_pSysParam->nAutoMode == 0)
// 		{//手动模式有效
// 			if(sts == -2) 
// 			{
// 				if (m_Robot->m_tSysStatus==K_RUN_STS_RUN)
// 				{
// 					m_Robot->m_bIsHandPause = true;
// 					m_Robot->Pause();
// 				}
// 				else if ((m_Robot->m_tSysStatus==K_RUN_STS_STOP)||(m_Robot->m_tSysStatus==K_RUN_STS_PAUSE))
// 				{
// 					;
// 				}
// 			}
// 		}
// 		else
// 		{//自动情况响应启动暂停
// 			if(sts == -2) 
// 			{
// 				if (m_Robot->m_tSysStatus==K_RUN_STS_RUN)
// 				{
// 					m_Robot->m_bIsHandPause = true;
// 					m_Robot->Pause();
// 				}
// 				else if (m_Robot->m_tSysStatus==K_RUN_STS_PAUSE)
// 				{
// 					if ((m_Robot->m_inputSts&(0x0001<<m_Robot->inPLCError))==(0x0001<<m_Robot->inPLCError))
// 					{
// 						m_Robot->Run();
// 					}
// 					else
// 					{
// 						m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,_T("气缸故障,禁止启动!"));
// 					}
// 				}
// 			}
// 		}
// 	}
//	else if(str == _T("请求加工"))   // 工件到位，PLC请求开始加工
// 	{//Remote
// 		if((sts == 0)&&(g_pFrm->m_pSysParam->nAutoMode==1)
// 			&&(m_Robot->m_tSysStatus==K_RUN_STS_STOP)
// 			&&(m_Robot->m_bFlagRunning==FALSE)
// 			&&(m_Robot->bRobotIsMoving==FALSE)
// 			&&((m_Robot->m_inputSts&(0x0001<<m_Robot->inPLCError))==(0x0001<<m_Robot->inPLCError)))
// 		{
// 			if(g_pFrm->m_pSysParam->nSFDoorEnable ==TRUE)
// 			{
// 				// 判断安全门状态
// 				if(g_pFrm->m_Robot->m_bDoorSFSts == FALSE)
// 				{
// 					// 打开状态
// 					m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,_T("安全门打开,禁止启动!"));
// 				}else
// 				{
// 					m_Robot->Run();
// 				}
// 			}else
// 			{  // 安全门未使能
//                  m_Robot->Run();
// 			}
// 			
// 		}
// 	}	
//	else if(str == _T("请求空板称重"))   // PCL 请求空板称重
// 	{
// 		if((sts == 0)&&(g_pFrm->m_pSysParam->nAutoMode==1))
// 		{
// 			date = -1;
// 			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"空板称重中…");
// 			
// 			g_pFrm->m_BatetWCDate[g_pFrm->m_Robot->m_WCBatetID].ID =
// 				g_pFrm->m_Robot->m_WCBatetID;  
// 			// 获取重量
// 			//bIsS = theApp.m_Serial.ReadBSMBalance(date);
// // 			if(bIsS == TRUE)
// // 			{
// // 				g_pFrm->m_BatetWCDate[g_pFrm->m_Robot->m_WCBatetID].dWCDate_0
// // 					= date;
// // 			}
//       
// 			strtemp.Format(_T("%d#"),g_pFrm->m_BatetWCDate[g_pFrm->m_Robot->m_WCBatetID].ID);
// 			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_BCID,strtemp); // ID
// 
// 			strtemp.Format(_T("%.3f"),g_pFrm->m_BatetWCDate[g_pFrm->m_Robot->m_WCBatetID].dWCDate_0);
// 			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_BCDATA,strtemp);// 状态
// 			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"空板称重完成！");
//             g_pFrm->m_Robot->m_WCBatetID++;
//            
//             CFunction::DelaySec(0.5);  
// 		    // 发送完成信号                
// 			g_pFrm->m_Robot->ExOutput(_T("上料称重完成"),TRUE);
// 			CFunction::DelaySec(0.2);                   //s
// 			g_pFrm->m_Robot->ExOutput(_T("上料称重完成"),FALSE);
// 
// 			g_pFrm->m_wndRightBar.m_DlgPosInfo.UpdateMotionState();
// 		}
// 	}
//	else if(str == _T("请求满板称重"))   // PLC 请求满板称重
// 	{   
// 		//Remote
// 		if((sts == 0)&&(g_pFrm->m_pSysParam->nAutoMode==1))
// 		{
// 			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"满板称重中…");
//              
// 			date= -1;
// 			// 获取重量
// 			//bIsS = theApp.m_Serial.ReadBSMBalanceEx(date);
// 			if(bIsS == TRUE)
// 			{
// 				g_pFrm->m_BatetWCDate[g_pFrm->m_Robot->m_WCBatetID2].dWCDate_1
// 					= date;
// 			}
// 
// 			strtemp.Format(_T("%d#"),g_pFrm->m_BatetWCDate[g_pFrm->m_Robot->m_WCBatetID2].ID);
// 			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_BCID2,strtemp); // ID
// 
// 			strtemp.Format(_T("%.3f"),g_pFrm->m_BatetWCDate[g_pFrm->m_Robot->m_WCBatetID2].dWCDate_1);
// 			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_BCDATA2,strtemp);// 状态
// 			g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,"满板称重完成！");
// 		
//            
// 			 // 检测是否超过上下限
//             int rtn = g_pFrm->m_Robot->checkWeightLMT(g_pFrm->m_Robot->m_WCBatetID2);
//             if(rtn == -1)
// 			{  
// 				bisWeightError =TRUE;
//                // 下限
//                 m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,_T("单板胶量超出设置下限!"));
// 			  //  g_pFrm->m_Robot->m_tSysStatus = K_RUN_STS_WEIGHTERROR;
//                 m_wndRightBar.m_DlgPosInfo.GetDlgItem(IDC_BUTTON_HANDSELIMT)->EnableWindow(TRUE);
// 			    g_pFrm->m_Robot->SetLamp();
// 			}
// 			if(rtn == 1)
// 			{
//                // 上限
// 				bisWeightError =TRUE;
// 				//g_pFrm->m_Robot->m_tSysStatus = K_RUN_STS_WEIGHTERROR;
//                 m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_STATIC_MOTION,_T("单板胶量超出设置上限!"));
// 			    m_wndRightBar.m_DlgPosInfo.GetDlgItem(IDC_BUTTON_HANDSELIMT)->EnableWindow(TRUE);
// 			     g_pFrm->m_Robot->SetLamp();
// 			}
// 
//              g_pFrm->m_Robot->m_WCBatetID2++;
// 			 CFunction::DelaySec(0.5);
// 
// 			// 发送完成信号
// 			g_pFrm->m_Robot->ExOutput(_T("下料称重完成"),TRUE);
// 			CFunction::DelaySec(0.2);                   //s
// 			g_pFrm->m_Robot->ExOutput(_T("下料称重完成"),FALSE);
//             g_pFrm->m_wndRightBar.m_DlgPosInfo.UpdateMotionState();
// 		}
// 	}
// 	else if(str == _T("请求分段加工"))   // 分段加工，工件到位，PLC请求开始
// 	{//Remote
// 		if((sts == 0)&&(m_Robot->m_tSysStatus==K_RUN_STS_RUN))
// 		{
// 			g_pFrm->AddMsg("askdisp2");
// 	        m_EvtAskSeparaRun.SetEvent();  // 请求加工
// 		}
// 	}
	else if(str == _T("GasError"))   // 气压异常
	{
		if(sts == -2)
		{
			;
		}
	}
	else if(str == _T("停止运行"))   // 停止运行
	{//Stop
		if(sts == -2) 
		{
			m_Robot->Stop();
		}
	}
// 	else if(str == _T("输送线故障"))
// 	{//Stop
// 		if(sts == -2)
// 		{
// 			;
// 		}
// 	}
// 	else if(str == _T("系统复位"))  // 系统回零复位
// 	{//Home
// 		if(sts == -2)
// 		{
// 			m_wndToolBar.EnableWindow(FALSE);
// 			m_Robot->Home(); 
// 			m_wndToolBar.EnableWindow(TRUE);
// 		}
// 	}
// 	else if(str == _T("顶升气缸到位"))  //顶升气缸上限
// 	{
// 		if(sts == -2)
// 		{ // 触发  
// 			m_Robot->m_bUpStopCYSign1= TRUE;  
// 		}else if(sts == 2)
// 		{
// 			m_Robot->m_bUpStopCYSign1= FALSE;
// 		}
// 	}
// 	else if(str == _T("顶升气缸原位"))  //顶升气缸下线
// 	{
// 		if(sts == -2)
// 		{ // 触发  
// 			m_Robot->m_bUpStopCYSign2= TRUE;  
// 		}else if(sts == 2)
// 		{
//             m_Robot->m_bUpStopCYSign2= FALSE;
// 		}
// 	}
// 	else if(str == _T("加工阻挡气缸到位"))  //加工阻挡气缸上位
// 	{
// 		if(sts == -2)
// 		{
// 			m_Robot->m_bWorkStopCYSign = TRUE; 
// 		}
// 		else if(sts == 2)
// 		{
// 			m_Robot->m_bWorkStopCYSign = FALSE;  
// 		}
// 	}
// 	else if(str == _T("分版阻挡气缸到位"))  //分段阻挡气缸到位信号
// 	{
// 		if(sts == -2)
// 		{
// 			m_Robot->m_bSqeStopCYSign = TRUE;  
// 		}
// 		else if(sts == 2)
// 		{
// 			m_Robot->m_bSqeStopCYSign = FALSE;  
// 		}
// 	}
	return 0;
}
// 
LRESULT CMainFrame::OnMessagePosEvent(WPARAM wparam, LPARAM lparam)
{
	double *p = (double *)wparam;
	long *axis_stat = (long *)lparam;
//	unsigned short pValue=0;
    
	CString strTemp;

	K_RUN_STATUS kTmpStat = m_Robot->m_tSysStatus;

	for(int i=0; i<3; i++)
	{
		strTemp.Format("%.3f", p[i]);
		g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_EDIT1+i,strTemp);
		if(i!=3)
		{	//检查驱动器报警信号
			if((axis_stat[i] & 0x02) == 0x02)
			{
				switch(i)
				{
				case 0:
					m_Robot->m_tSysStatus = K_AXIS_STS_ALM_X;
					break;
				case 1:
					m_Robot->m_tSysStatus = K_AXIS_STS_ALM_Y;
					break;
				case 2:
					m_Robot->m_tSysStatus = K_AXIS_STS_ALM_Z;
					break;
				default:
					break;
				}
				if(m_Robot->bRobotIsMoving == TRUE)
					m_Robot->Stop();
				g_pFrm->m_Robot->SetLamp();
			}
		}
	}

//     //喷射阀高度值 = 基准高度 - 当前位置  
// 	double dis = g_pFrm->m_pSysParam->dJetZBaseHightValue - p[2];
// 	strTemp.Format("%.3f",dis); 
// 	g_pFrm->m_wndRightBar.m_DlgPosInfo.SetDlgItemText(IDC_INFO_EDIT5,strTemp);
//      
// 	// 基准高度设置界面
// 	if ((g_pFrm->m_DlgZBaseSet.GetSafeHwnd()!=NULL)&&g_pFrm->m_DlgZBaseSet.IsWindowVisible())
// 	{
// 		//喷射阀高度值 = 基准高度 - 当前位置  
// 		double dis = g_pFrm->m_pSysParam->dJetZBaseHightValue - p[2];
// 		strTemp.Format("%.3f",dis); 
// 		g_pFrm->m_DlgZBaseSet.SetDlgItemText(IDC_BASE_JETHIGHT,strTemp);
// 
// 	}

// 	if ((g_pFrm->m_dlgZHeight.GetSafeHwnd()!=NULL)&&g_pFrm->m_dlgZHeight.IsWindowVisible())
// 	{
// 		// 获取模拟量数据 Z轴高度数据
// 		g_pFrm->m_Robot->m_pController->GetExtADDAVal(0,(short)g_pFrm->m_pSysParam->nLaserReadPortID,&pValue);  
// 		strTemp.Format("%.3f",(-5.0+(pValue/65535.0)*10.0)*2);
// 		g_pFrm->m_dlgZHeight.SetDlgItemText(IDC_ZHEIGHT_EDIT_M1,strTemp);
// 	}

// 	if ((g_pFrm->m_dlgRepeatZHeight.GetSafeHwnd()!=NULL)&&g_pFrm->m_dlgRepeatZHeight.IsWindowVisible())
// 	{
// 		// 获取模拟量数据 Z轴高度数据
// 		g_pFrm->m_Robot->m_pController->GetExtADDAVal(0,(short)g_pFrm->m_pSysParam->nLaserReadPortID,&pValue);
// 		strTemp.Format("%.3f",(-5.0+(pValue/65535.0)*10.0)*2);
// 		g_pFrm->m_dlgRepeatZHeight.SetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_M1,strTemp);
// 	}

	// 更新状态显示
	if(kTmpStat != m_Robot->m_tSysStatus)
	{
		m_wndRightBar.m_DlgPosInfo.UpdateMotionState();
		m_Robot->SetLamp();
	}
	//////////////////////

	return 0;
}
void CMainFrame::AddMsg(CString strText)
{
	if( m_wndOutput.m_wndOutputBuild.m_hWnd != NULL)
	{
		if(m_wndOutput.m_wndOutputBuild.GetCount()>3000)
		{
			m_wndOutput.m_wndOutputBuild.ResetContent();
		}
		CString strTemp;
		strTemp.Format("%04d: ", m_wndOutput.m_wndOutputBuild.GetCount());
		m_wndOutput.m_wndOutputBuild.AddString(strTemp + strText);
		m_wndOutput.m_wndOutputBuild.SetCurSel(m_wndOutput.m_wndOutputBuild.GetCount()-1);
	}
}
void CMainFrame::ShowWCPDialog(tgCmdLine *pCmdLine, int iSelIndex)
{
	if(m_DlgWCP.GetSafeHwnd() == NULL)
	{
		m_DlgWCP.Create(IDD_DLG_WCPOT, this);
	}
	m_DlgWCP.m_iSelIndex = iSelIndex;
	m_DlgWCP.m_pCmdLine = pCmdLine;
	m_DlgWCP.RefreshAxisPos();
	m_DlgWCP.ShowWindow(SW_SHOW);

}
void CMainFrame::ShowWCRectDialog(tgCmdLine *pCmdLine, int iSelIndex)
{
	if(m_dlgWcRect.GetSafeHwnd() == NULL)
	{
		m_dlgWcRect.Create(IDD_DLG_WCRECT, this);
	}
	m_dlgWcRect.m_iSelIndex = iSelIndex;
    m_dlgWcRect.m_pCmdLine = pCmdLine;
	m_dlgWcRect.RefreshAxisPos();

	CRect rect,rect1,rect2;
	g_pView->GetWindowRect(&rect1);
	m_dlgWcRect.GetWindowRect(&rect2);
	rect.top=rect1.bottom-rect2.Height(); 
	rect.left=rect1.left; 
	rect.right=rect.left+rect2.Width(); 
	rect.bottom=rect1.bottom; 
	m_dlgWcRect.MoveWindow(&rect);

	//////
	m_dlgWcRect.ShowWindow(SW_SHOW);
}
void CMainFrame::ShowWCLDialog(tgCmdLine *pCmdLine, int iSelIndex)
{
	if(m_dlgWCL.GetSafeHwnd() == NULL)
	{
		m_dlgWCL.Create(IDD_DLG_WCL, this);
	}
	m_dlgWCL.m_iSelIndex = iSelIndex;
	m_dlgWCL.m_pCmdLine = pCmdLine;
	m_dlgWCL.RefreshAxisPos();
	////
	CRect rect,rect1,rect2;
	g_pView->GetWindowRect(&rect1);
	m_dlgWCL.GetWindowRect(&rect2);
	rect.top=rect1.bottom-rect2.Height(); 
	rect.left=rect1.left; 
	rect.right=rect.left+rect2.Width(); 
	rect.bottom=rect1.bottom; 
	m_dlgWCL.MoveWindow(&rect);
	//////
	m_dlgWCL.ShowWindow(SW_SHOW);
}
// 热键响应函数
void CMainFrame::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: Add your message handler code here and/or call default
	if (nHotKeyId == 1001)
	{
		CUserRegister regidter;  
		regidter.DoModal();
	}
	CFrameWndEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CMainFrame::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
}
