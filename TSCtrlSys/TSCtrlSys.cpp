
// TSCtrlSys.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "TSCtrlSysDoc.h"
#include "TSCtrlSysView.h"
#include "TSCtrlSys.h"
#include "MainFrm.h"

#include "DlgProgSel.h"

#include "vn_define.h"
#include <psapi.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma    comment(lib,   "psapi.lib ")

int Round(double val)
{
	int t;
	__asm
	{
		fld val;
		fistp t;
	}
	return t;
}

// CTSCtrlSysApp

BEGIN_MESSAGE_MAP(CTSCtrlSysApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CTSCtrlSysApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, &CTSCtrlSysApp::OnFileOpen)
END_MESSAGE_MAP()


// CTSCtrlSysApp 构造

CTSCtrlSysApp::CTSCtrlSysApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CTSCtrlSysApp 对象

CTSCtrlSysApp theApp;
CMainFrame *g_pFrm = NULL;
CTSCtrlSysDoc *g_pDoc = NULL;
CTSCtrlSysView *g_pView = NULL;
// CTSCtrlSysApp 初始化

BOOL CTSCtrlSysApp::InitInstance()
{
	////////////////////////////////////////////////////
	//创建命名信标对象。
	HANDLE hSem=CreateSemaphore(NULL,1,1,m_pszAppName);
	if(hSem)  //信标对象创建成功。
	{
		//信标对象已经存在，则程序已有一个实例在运行。
		if(ERROR_ALREADY_EXISTS==GetLastError())
		{                   
			CloseHandle(hSem);      //关闭信号量句柄。
			//获取桌面窗口的一个子窗口。
			HWND hWndPrev=::GetWindow(::GetDesktopWindow(),GW_CHILD);  
			while(::IsWindow(hWndPrev))
			{
				//判断窗口是否有我们预先设置的标记，如有，则是我们寻找的窗口，并将它激活。
				if(::GetProp(hWndPrev,m_pszAppName))    
				{
					//如果主窗口已最小化，则恢复其大小。
					if (::IsIconic(hWndPrev))      
						::ShowWindow(hWndPrev,SW_RESTORE);
					//将应用程序的主窗口激活。
					::SetForegroundWindow(hWndPrev);
					return FALSE;                      //退出实例。
				}
				//继续寻找下一个窗口。
				hWndPrev = ::GetWindow(hWndPrev,GW_HWNDNEXT); 
			}
			AfxMessageBox("已有一个实例在运行，但找不到它的主窗口！");
			return FALSE;   
		}
	}
	else
	{
		AfxMessageBox("创建信标对象失败，程序退出！");
		return FALSE;
	}
	///////////////////////////////////////////////////////////////

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("USOFT"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	/////////////////////

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTSCtrlSysDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CTSCtrlSysView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	///////////////////////////////////////
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	// 在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	// 启用拖/放
	// m_pMainWnd->DragAcceptFiles();
	//////////////////////////////////////////////////////////////////////////
	CRect rect;
	g_pView->GetClientRect(&rect);
	g_pView->SendMessage(WM_SIZE, SIZE_MAXIMIZED, MAKELPARAM(rect.Width(), rect.Height()));

	//////////////////////////////////////////////////////////////////////////
	CString strTemp, strText;
	strTemp = g_pView->m_bImgInit ? "CCD:OK|":"CCD:NG|";	
	strText = strTemp;
	strTemp = (g_pFrm->m_Robot->Init()==RTN_NONE) ? "Controller:OK" : "Controller:NG";
	strText += strTemp;	
	((CMainFrame*)m_pMainWnd)->SetPanelTextEx(IDS_APP_STATUS, strText);
	if (g_pView->m_bImgInit)
	{
		g_pView->PaintShape(CRect(0, 0, IMG_WIDTH, IMG_HEIGHT));
	}
    
	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		if (IDYES == AfxMessageBox("是否需要回零？", MB_YESNO))
		{
			g_pFrm->m_Robot->Home();   // 回 零
			g_pFrm->AddMsg("回原点完成");
		}
	}

    /////////////  串口初始化 ////////////////////////////
	g_pFrm->InitComPort();
	g_pFrm->m_comDisp[0].AttachDispenser(0, (DispenserType)g_pFrm->m_pRobotParam->m_nValveType, g_pFrm->GetComPort("点胶阀1"));	// 配置点胶阀1
	g_pFrm->m_comLaser.SetComPort(g_pFrm->GetComPort("激光测高"));
	g_pFrm->m_comLight.SetComPort(g_pFrm->GetComPort("光源控制器"));
	g_pFrm->m_comWeight.SetComPort(g_pFrm->GetComPort("称重"));

	// 从点胶阀读取参数
	g_pFrm->m_comDisp[0].HandleParamDispenser(TRUE);

	g_pFrm->m_pSysParam->m_SystemBeginTime = CTime::GetCurrentTime();//记录软件启动时间
	if (g_pFrm->m_wndRightBar.m_DlgPosInfo.GetSafeHwnd()!=NULL)
		g_pFrm->m_wndRightBar.m_DlgPosInfo.UpsysWorkMode(); 

	//////////////////////////////////////////////////////////////////////////
	// 加载当前产品信息
	for (int i=0; i<NUM_CHANNEL; i++)
	{
		g_AllProgInfo[0].HandleAllProg(TRUE);
	}

	// 设置产品信息
	((CMainFrame*)m_pMainWnd)->SetWindowTextEx();

	return TRUE;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CTSCtrlSysApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTSCtrlSysApp 自定义加载/保存方法

void CTSCtrlSysApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	if(0 == g_pFrm->m_pSysParam->nLanguageType)
	  GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT_CN);

	if(1 == g_pFrm->m_pSysParam->nLanguageType)
	  GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT_EN);
}

void CTSCtrlSysApp::LoadCustomState()
{
}

void CTSCtrlSysApp::SaveCustomState()
{
}
int CTSCtrlSysApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class

	//CleanState("ApplicationLook");//这里如果删除，启动多个进程会运行到这里报错
	return CWinAppEx::ExitInstance();
}
// CTSCtrlSysApp 消息处理程序
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	SetDlgItemText(IDC_STATIC_VERSION, CString(g_pFrm->m_pSysParam->szApp) +CString("   ")+ CString(STR_Version));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTSCtrlSysApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message == WM_KEYDOWN)
	{
		//g_pFrm->SetFocus();
		g_pFrm->OnKeyDown(pMsg->wParam, 1, 0);
	}
	else if(pMsg->message == WM_KEYUP)
		g_pFrm->OnKeyUp(pMsg->wParam, 1, 0);

	return CWinAppEx::PreTranslateMessage(pMsg);
}


void CTSCtrlSysApp::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CDlgProgSel dlg;
	dlg.DoModal();
}
