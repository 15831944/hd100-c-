
// TSCtrlSys.cpp : ����Ӧ�ó��������Ϊ��
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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, &CTSCtrlSysApp::OnFileOpen)
END_MESSAGE_MAP()


// CTSCtrlSysApp ����

CTSCtrlSysApp::CTSCtrlSysApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CTSCtrlSysApp ����

CTSCtrlSysApp theApp;
CMainFrame *g_pFrm = NULL;
CTSCtrlSysDoc *g_pDoc = NULL;
CTSCtrlSysView *g_pView = NULL;
// CTSCtrlSysApp ��ʼ��

BOOL CTSCtrlSysApp::InitInstance()
{
	////////////////////////////////////////////////////
	//���������ű����
	HANDLE hSem=CreateSemaphore(NULL,1,1,m_pszAppName);
	if(hSem)  //�ű���󴴽��ɹ���
	{
		//�ű�����Ѿ����ڣ����������һ��ʵ�������С�
		if(ERROR_ALREADY_EXISTS==GetLastError())
		{                   
			CloseHandle(hSem);      //�ر��ź��������
			//��ȡ���洰�ڵ�һ���Ӵ��ڡ�
			HWND hWndPrev=::GetWindow(::GetDesktopWindow(),GW_CHILD);  
			while(::IsWindow(hWndPrev))
			{
				//�жϴ����Ƿ�������Ԥ�����õı�ǣ����У���������Ѱ�ҵĴ��ڣ����������
				if(::GetProp(hWndPrev,m_pszAppName))    
				{
					//�������������С������ָ����С��
					if (::IsIconic(hWndPrev))      
						::ShowWindow(hWndPrev,SW_RESTORE);
					//��Ӧ�ó���������ڼ��
					::SetForegroundWindow(hWndPrev);
					return FALSE;                      //�˳�ʵ����
				}
				//����Ѱ����һ�����ڡ�
				hWndPrev = ::GetWindow(hWndPrev,GW_HWNDNEXT); 
			}
			AfxMessageBox("����һ��ʵ�������У����Ҳ������������ڣ�");
			return FALSE;   
		}
	}
	else
	{
		AfxMessageBox("�����ű����ʧ�ܣ������˳���");
		return FALSE;
	}
	///////////////////////////////////////////////////////////////

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("USOFT"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	/////////////////////

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTSCtrlSysDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CTSCtrlSysView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	///////////////////////////////////////
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	// �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	// ������/��
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
		if (IDYES == AfxMessageBox("�Ƿ���Ҫ���㣿", MB_YESNO))
		{
			g_pFrm->m_Robot->Home();   // �� ��
			g_pFrm->AddMsg("��ԭ�����");
		}
	}

    /////////////  ���ڳ�ʼ�� ////////////////////////////
	g_pFrm->InitComPort();
	g_pFrm->m_comDisp[0].AttachDispenser(0, (DispenserType)g_pFrm->m_pRobotParam->m_nValveType, g_pFrm->GetComPort("�㽺��1"));	// ���õ㽺��1
	g_pFrm->m_comLaser.SetComPort(g_pFrm->GetComPort("������"));
	g_pFrm->m_comLight.SetComPort(g_pFrm->GetComPort("��Դ������"));
	g_pFrm->m_comWeight.SetComPort(g_pFrm->GetComPort("����"));

	// �ӵ㽺����ȡ����
	g_pFrm->m_comDisp[0].HandleParamDispenser(TRUE);

	g_pFrm->m_pSysParam->m_SystemBeginTime = CTime::GetCurrentTime();//��¼�������ʱ��
	if (g_pFrm->m_wndRightBar.m_DlgPosInfo.GetSafeHwnd()!=NULL)
		g_pFrm->m_wndRightBar.m_DlgPosInfo.UpsysWorkMode(); 

	//////////////////////////////////////////////////////////////////////////
	// ���ص�ǰ��Ʒ��Ϣ
	for (int i=0; i<NUM_CHANNEL; i++)
	{
		g_AllProgInfo[0].HandleAllProg(TRUE);
	}

	// ���ò�Ʒ��Ϣ
	((CMainFrame*)m_pMainWnd)->SetWindowTextEx();

	return TRUE;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CTSCtrlSysApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTSCtrlSysApp �Զ������/���淽��

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

	//CleanState("ApplicationLook");//�������ɾ��������������̻����е����ﱨ��
	return CWinAppEx::ExitInstance();
}
// CTSCtrlSysApp ��Ϣ�������
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
	// TODO: �ڴ���������������
	CDlgProgSel dlg;
	dlg.DoModal();
}
