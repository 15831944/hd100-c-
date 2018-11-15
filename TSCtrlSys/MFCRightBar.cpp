// MFCRightBar.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "MFCRightBar.h"


// CMFCRightBarD

IMPLEMENT_DYNAMIC(CMFCRightBar, CDockablePane)

CMFCRightBar::CMFCRightBar()
{
}

CMFCRightBar::~CMFCRightBar()
{
}


BEGIN_MESSAGE_MAP(CMFCRightBar, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMFCRightBar message handlers

void CMFCRightBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here

	// ѡ��ؼ�Ӧ��������������
	m_DlgPosInfo.SetWindowPos (this, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	m_DlgPosInfo.ShowWindow(SW_SHOW);
}

int CMFCRightBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ����ѡ�����:
	if (!m_DlgPosInfo.Create(IDD_DLG_INFO,this))
	{
		TRACE0("δ�ܴ������ѡ�����/n");
		return -1;      // δ�ܴ���
	}
	m_DlgPosInfo.ShowWindow(SW_SHOW);

	return 0;
}

BOOL CMFCRightBar::OnShowControlBarMenu(CPoint point)
{ 
	UNREFERENCED_PARAMETER(point);
	return TRUE;
}
BOOL CMFCRightBar::CanBeClosed()const
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//																		//
//////////////////////////////////////////////////////////////////////////
// CMFCTitleBar

IMPLEMENT_DYNAMIC(CMFCTitleBar, CDockablePane)

CMFCTitleBar::CMFCTitleBar()
{
}

CMFCTitleBar::~CMFCTitleBar()
{
}


BEGIN_MESSAGE_MAP(CMFCTitleBar, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMFCTitleBar message handlers

void CMFCTitleBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here

	// ѡ��ؼ�Ӧ��������������
	m_DlgTitle.SetWindowPos (this, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	m_DlgTitle.ShowWindow(SW_SHOW);
}

int CMFCTitleBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ����ѡ�����:
	if (!m_DlgTitle.Create(IDD_DLG_TITLE,this))
	{
		TRACE0("δ�ܴ������ѡ�����/n");
		return -1;      // δ�ܴ���
	}
	m_DlgTitle.ModifyStyle(WS_CAPTION, 0);
	m_DlgTitle.ShowWindow(SW_SHOW);

	return 0;
}