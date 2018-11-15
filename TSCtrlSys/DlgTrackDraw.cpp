// DlgTrackDraw.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "RedrawTrack.h"
#include "DlgTrackDraw.h"
#include "afxdialogex.h"


// CDlgTrackDraw �Ի���

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


// CDlgTrackDraw ��Ϣ�������


BOOL CDlgTrackDraw::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

// 	//���ô���ΪLayeredWindow
// 	LONG para = GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE);
// 	para |= WS_EX_LAYERED;
// 	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, para);
// 
// 	//���ô���͸��ɫΪRGB(255,0,0)
// 	//SetLayeredWindowAttributes(RGB(255, 0, 0), 128, LWA_COLORKEY|LWA_ALPHA);
// 	SetLayeredWindowAttributes(RGB(255, 0, 0), 255, LWA_COLORKEY);

	// ��ȡ���ڿͻ���rect��������ͼ�õ�
	GetClientRect(m_rtClient);

	// ����Ĭ����ʾ����㽺·��
	m_radio1 = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgTrackDraw::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	RECT rect;
 	GetClientRect(&rect);
// 	dc.FillSolidRect(&rect, RGB(255, 0, 0));

	m_Track.LoadTrack();
	m_Track.Paint(&dc,rect,m_bFront);
	SetBkColor(GetDlgItem(IDC_RADIO1)->GetDC()->m_hDC, RGB(192, 255, 192));
}

void CDlgTrackDraw::OnRadio2()
{
	// TODO: �ڴ���������������
	m_bFront = FALSE;
	Invalidate();
	UpdateWindow();
}


void CDlgTrackDraw::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bFront = TRUE;
	Invalidate();
	UpdateWindow();
}
