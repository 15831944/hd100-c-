// DlgIOCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgIOCtrl.h"



// CDlgIOCtrl dialog

IMPLEMENT_DYNAMIC(CDlgIOCtrl, CDialog)

CDlgIOCtrl::CDlgIOCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIOCtrl::IDD, pParent)
{

}

CDlgIOCtrl::~CDlgIOCtrl()
{

	
}

void CDlgIOCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_IO_TAB, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CDlgIOCtrl, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DLG_IO_TAB, &CDlgIOCtrl::OnTcnSelchangeDlgIoTab)
//	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgIOCtrl message handlers

BOOL CDlgIOCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_tabCtrl.InsertItem(0,_T("信号输入端口"));  
	m_tabCtrl.InsertItem(1,_T("信号输出端口"));	
	m_tabCtrl.InsertItem(2,_T("轨道功能调试"));

	if(theApp.m_SysUser.m_CurUD.level>=GM_PERSONNEL)
	{
		m_tabCtrl.InsertItem(3,_T("马达参数设置"));
		m_tabCtrl.InsertItem(4,_T("Com口配置"));
	}

	pageInput.Create(IDD_PAGE_IN,&m_tabCtrl);  
	pageOutput.Create(IDD_PAGE_OUT,&m_tabCtrl); 
	pageTrack.Create(IDD_DLG_TRACK, &m_tabCtrl);
	pageMtrParam.Create(IDD_PAGE_MOTOR, &m_tabCtrl);
	pageComPort.Create(IDD_PAGE_COMPORT, &m_tabCtrl);

	//获得IDC_TABTEST客户区大小
	CRect rs;
	m_tabCtrl.GetClientRect(&rs);
	rs.top+=25; 
	rs.left+=5; 
	rs.right-=5; 
	rs.bottom-=10; 

	pageInput.MoveWindow(&rs);
	pageOutput.MoveWindow(&rs);
	pageMtrParam.MoveWindow(&rs);
	pageComPort.MoveWindow(&rs);
	pageTrack.MoveWindow(&rs);

	pageInput.EnableWindow(TRUE);
	pageOutput.EnableWindow(TRUE);
	pageMtrParam.EnableWindow(TRUE);
	pageComPort.EnableWindow(TRUE);
	pageTrack.EnableWindow(TRUE);

	pageInput.ShowWindow(SW_SHOW); 
	pageOutput.ShowWindow(SW_HIDE); 
	pageMtrParam.ShowWindow(SW_HIDE);
	pageComPort.ShowWindow(SW_HIDE);
	pageTrack.ShowWindow(SW_HIDE);

//	m_TabCtrl.SetCurSel(0); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgIOCtrl::InitTableCtrl()
{
	int nIdx = m_tabCtrl.GetCurSel();

	m_tabCtrl.DeleteAllItems();
	m_tabCtrl.InsertItem(0,_T("信号输入端口"));  
	m_tabCtrl.InsertItem(1,_T("信号输出端口"));	
	m_tabCtrl.InsertItem(2,_T("轨道功能测试"));

	if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
	{
		m_tabCtrl.InsertItem(3,_T("马达参数设置"));
		m_tabCtrl.InsertItem(4,_T("Com口配置"));
	}

	m_tabCtrl.SetCurSel(nIdx);
}


void CDlgIOCtrl::OnTcnSelchangeDlgIoTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 0;

	switch(m_tabCtrl.GetCurSel())   
	{   
	case 0: 
		pageInput.ShowWindow(SW_SHOW);
		pageOutput.ShowWindow(SW_HIDE);
		pageTrack.ShowWindow(SW_HIDE);
		pageMtrParam.ShowWindow(SW_HIDE);
		pageComPort.ShowWindow(SW_HIDE);
		break;   
	case 1:   
		pageInput.ShowWindow(SW_HIDE);
		pageOutput.ShowWindow(SW_SHOW);
		pageTrack.ShowWindow(SW_HIDE);
		pageMtrParam.ShowWindow(SW_HIDE);
		pageComPort.ShowWindow(SW_HIDE);
		break;
	case 2:
		pageInput.ShowWindow(SW_HIDE);
		pageOutput.ShowWindow(SW_HIDE);
		pageTrack.ShowWindow(SW_SHOW);
		pageMtrParam.ShowWindow(SW_HIDE);
		pageComPort.ShowWindow(SW_HIDE);
		break;
	case 3:
		pageInput.ShowWindow(SW_HIDE);
		pageOutput.ShowWindow(SW_HIDE);
		pageTrack.ShowWindow(SW_HIDE);
		pageMtrParam.ShowWindow(SW_SHOW);
		pageComPort.ShowWindow(SW_HIDE);
		break;
	case 4:
		pageInput.ShowWindow(SW_HIDE);
		pageOutput.ShowWindow(SW_HIDE);
		pageTrack.ShowWindow(SW_HIDE);
		pageMtrParam.ShowWindow(SW_HIDE);
		pageComPort.ShowWindow(SW_SHOW);
		g_pFrm->m_comLight.GetLightPower(pageComPort.nValCH1, pageComPort.nValCH2);
		pageComPort.UpdateData(FALSE);
		break;
	default:
		break;
	}
}