// DlgTitle.cpp : 实现文件
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "Resource.h"
#include "DlgTitle.h"
#include "afxdialogex.h"


// CDlgTitle 对话框

IMPLEMENT_DYNAMIC(CDlgTitle, CDialogEx)

CDlgTitle::CDlgTitle(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTitle::IDD, pParent)
{

}

CDlgTitle::~CDlgTitle()
{
}

void CDlgTitle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_BTN_LOG, m_btnKFLog);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDlgTitle, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CDlgTitle::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// CDlgTitle 消息处理程序


BOOL CDlgTitle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
//	SetBackgroundColor(RGB(255, 255, 255));
	SetBackgroundImage(IDB_BMP_LOG, BACKGR_TOPLEFT);	
	
	m_btnClose.MoveWindow(1280-52, 4, 48, 48);
	m_btnClose.SetIcon(IDI_ICON_SHUTDOWN);
	m_btnClose.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(192, 192, 192)); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgTitle::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->OnClose();
}
