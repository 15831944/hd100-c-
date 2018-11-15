// DlgTrackDraw.cpp : 实现文件
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "RedrawTrack.h"
#include "DlgTrackDraw.h"
#include "afxdialogex.h"


// CDlgTrackDraw 对话框

IMPLEMENT_DYNAMIC(CDlgTrackDraw, CDialogEx)

CDlgTrackDraw::CDlgTrackDraw(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTrackDraw::IDD, pParent)
{
	m_bFront = TRUE;
	m_radio1 = 0;
}

CDlgTrackDraw::~CDlgTrackDraw()
{
}

void CDlgTrackDraw::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_radio1);
}


BEGIN_MESSAGE_MAP(CDlgTrackDraw, CDialogEx)
	ON_WM_PAINT()
	ON_COMMAND(IDC_RADIO2, &CDlgTrackDraw::OnRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgTrackDraw::OnBnClickedRadio1)
END_MESSAGE_MAP()


// CDlgTrackDraw 消息处理程序


BOOL CDlgTrackDraw::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

// 	//设置窗体为LayeredWindow
// 	LONG para = GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE);
// 	para |= WS_EX_LAYERED;
// 	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, para);
// 
// 	//设置窗体透明色为RGB(255,0,0)
// 	//SetLayeredWindowAttributes(RGB(255, 0, 0), 128, LWA_COLORKEY|LWA_ALPHA);
// 	SetLayeredWindowAttributes(RGB(255, 0, 0), 255, LWA_COLORKEY);

	// 获取窗口客户区rect，后续绘图用到
	GetClientRect(m_rtClient);

	// 设置默认显示正面点胶路径
	m_radio1 = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgTrackDraw::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	RECT rect;
 	GetClientRect(&rect);
// 	dc.FillSolidRect(&rect, RGB(255, 0, 0));

	m_Track.LoadTrack();
	m_Track.Paint(&dc,rect,m_bFront);
	SetBkColor(GetDlgItem(IDC_RADIO1)->GetDC()->m_hDC, RGB(192, 255, 192));
}

void CDlgTrackDraw::OnRadio2()
{
	// TODO: 在此添加命令处理程序代码
	m_bFront = FALSE;
	Invalidate();
	UpdateWindow();
}


void CDlgTrackDraw::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bFront = TRUE;
	Invalidate();
	UpdateWindow();
}
