// DlgMachineSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgMachineSet.h"
#include "afxdialogex.h"


// CDlgMachineSet �Ի���

IMPLEMENT_DYNAMIC(CDlgMachineSet, CDialogEx)

CDlgMachineSet::CDlgMachineSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMachineSet::IDD, pParent)
{

}

CDlgMachineSet::~CDlgMachineSet()
{
}

void CDlgMachineSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CDlgMachineSet, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgMachineSet::OnBnClickedBtnSave)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgMachineSet::OnSelchangeTab1)
END_MESSAGE_MAP()


// CDlgMachineSet ��Ϣ�������


void CDlgMachineSet::OnBnClickedBtnSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

BOOL CDlgMachineSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//////////////////////////////////////////////////////////////////////////
	CRect rc;
	g_pFrm->m_wndRightBar.GetWindowRect(rc);

	CRect rc1;
	GetWindowRect(rc1);
	MoveWindow(rc.right-rc1.Width(), rc.top, rc1.Width(),rc1.Height());

	//////////////////////////////////////////////////////////////////////////
	m_tabCtrl.InsertItem(0,_T("λ������&����У��"));  
	m_tabCtrl.InsertItem(1,_T("������������")); 
	if (1 == g_pFrm->m_pRobotParam->m_nLoadUnloadMode)
	{
		m_tabCtrl.InsertItem(2,_T("�����ϲ���"));
	}

	m_pageJetAdjust.Create(IDD_DLG_JET_ADJUST, &m_tabCtrl);
	m_pageMachineParam.Create(IDD_DLG_MACHINE_PARAM, &m_tabCtrl);
	m_pageLoadUnload.Create(IDD_DLG_LD_ULD, &m_tabCtrl); 

	//���IDC_TABTEST�ͻ�����С
	CRect rs;
	m_tabCtrl.GetClientRect(&rs);
	rs.top+=25; 
	rs.left+=5; 
	rs.right-=5; 
	rs.bottom-=10; 

	m_pageJetAdjust.MoveWindow(&rs);
	m_pageMachineParam.MoveWindow(&rs);
	m_pageLoadUnload.MoveWindow(&rs);

	m_pageJetAdjust.EnableWindow(TRUE);
	m_pageMachineParam.EnableWindow(TRUE);
	m_pageLoadUnload.EnableWindow(TRUE);

	m_pageJetAdjust.ShowWindow(SW_SHOW);
	m_pageMachineParam.ShowWindow(SW_HIDE);
	m_pageLoadUnload.ShowWindow(SW_HIDE);

//	m_TabCtrl.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgMachineSet::InitTableCtrl()
{
	int nIdx = m_tabCtrl.GetCurSel();
	m_tabCtrl.DeleteAllItems();

	m_tabCtrl.InsertItem(0,_T("λ������&����У��"));  
	m_tabCtrl.InsertItem(1,_T("������������")); 
	if (1 == g_pFrm->m_pRobotParam->m_nLoadUnloadMode)
	{
		m_tabCtrl.InsertItem(2,_T("�����ϵ���"));
	}

	m_pageJetAdjust.UpdateCtrl();

	m_tabCtrl.SetCurSel(nIdx);
}

void CDlgMachineSet::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	UNREFERENCED_PARAMETER(pNMHDR);

	switch(m_tabCtrl.GetCurSel())   
	{   
	case 0: 
		m_pageJetAdjust.ShowWindow(SW_SHOW);
		m_pageMachineParam.ShowWindow(SW_HIDE);
		m_pageLoadUnload.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pageJetAdjust.ShowWindow(SW_HIDE);
		m_pageMachineParam.ShowWindow(SW_SHOW);
		m_pageLoadUnload.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pageJetAdjust.ShowWindow(SW_HIDE);
		m_pageMachineParam.ShowWindow(SW_HIDE);
		m_pageLoadUnload.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
}