// DlgTitle.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "Resource.h"
#include "DlgTitle.h"
#include "afxdialogex.h"


// CDlgTitle �Ի���

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


// CDlgTitle ��Ϣ�������


BOOL CDlgTitle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
//	SetBackgroundColor(RGB(255, 255, 255));
	SetBackgroundImage(IDB_BMP_LOG, BACKGR_TOPLEFT);	
	
	m_btnClose.MoveWindow(1280-52, 4, 48, 48);
	m_btnClose.SetIcon(IDI_ICON_SHUTDOWN);
	m_btnClose.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(192, 192, 192)); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgTitle::OnBnClickedBtnClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pFrm->OnClose();
}
