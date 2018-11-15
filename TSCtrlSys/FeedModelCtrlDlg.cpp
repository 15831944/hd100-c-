// FeedModelCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "FeedModelCtrlDlg.h"
#include <math.h>

// CFeedModelCtrlDlg dialog

IMPLEMENT_DYNAMIC(CFeedModelCtrlDlg, CDialog)

CFeedModelCtrlDlg::CFeedModelCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFeedModelCtrlDlg::IDD, pParent)
{

}

CFeedModelCtrlDlg::~CFeedModelCtrlDlg()
{
}

void CFeedModelCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFeedModelCtrlDlg, CDialog)
	ON_EN_CHANGE(IDC_FEED_FEED_UPVEL, &CFeedModelCtrlDlg::OnEnChangeFeed)
	ON_EN_CHANGE(IDC_FEED_FEED_DOWNVEL, &CFeedModelCtrlDlg::OnEnChangePageSysFeedDownvel)
	ON_BN_CLICKED(IDC_DLG_FEEDCTRL_VELSET, &CFeedModelCtrlDlg::OnBnClickedDlgFeedctrlVelset)
	ON_BN_CLICKED(IDOK, &CFeedModelCtrlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DLG_FEEDCTRL_FEEDUP, &CFeedModelCtrlDlg::OnBnClickedDlgFeedctrlFeedup)
	ON_BN_CLICKED(IDC_DLG_FEEDCTRL_FEEDDOWN, &CFeedModelCtrlDlg::OnBnClickedDlgFeedctrlFeeddown)
	ON_BN_CLICKED(IDC_DLG_FEEDCTRL_UNFEEDUP, &CFeedModelCtrlDlg::OnBnClickedDlgFeedctrlUnfeedup)
	ON_BN_CLICKED(IDC_DLG_FEEDCTRL_UNFEEDDOWN, &CFeedModelCtrlDlg::OnBnClickedDlgFeedctrlUnfeeddown)
	ON_BN_CLICKED(IDOK2, &CFeedModelCtrlDlg::OnBnClickedOk2)
	ON_BN_CLICKED(IDOK3, &CFeedModelCtrlDlg::OnBnClickedOk3)
END_MESSAGE_MAP()


// CFeedModelCtrlDlg message handlers

void CFeedModelCtrlDlg::OnEnChangeFeed()
{
   
}

void CFeedModelCtrlDlg::OnEnChangePageSysFeedDownvel()
{
   
}

void CFeedModelCtrlDlg::OnBnClickedDlgFeedctrlVelset()
{
	CString strTemp;
	GetDlgItemText(IDC_FEED_FEED_UPVEL, strTemp);
	g_pFrm->m_pSysParam->dSpeedFeedUpVel = atof(strTemp);

	GetDlgItemText(IDC_FEED_FEED_DOWNVEL, strTemp);
	g_pFrm->m_pSysParam->dSpeedFeedDownVel = atof(strTemp);

	if(fabs(g_pFrm->m_pSysParam->dSpeedFeedUpVel)>500)
		g_pFrm->m_pSysParam->dSpeedFeedUpVel = 500;

// 	strTemp.Format("%d", PLC_SEAD_PORT_FEEDUP_VEL);
// 	theApp.m_Serial.WritePLCData(strTemp,(int)g_pFrm->m_pSysParam->dSpeedFeedUpVel);
// 	strTemp.Format("%d", PLC_SEAD_PORT_UNFEEDUP_VEL);
// 	theApp.m_Serial.WritePLCData(strTemp,(int)g_pFrm->m_pSysParam->dSpeedFeedUpVel);
// 
// 	if(fabs(g_pFrm->m_pSysParam->dSpeedFeedDownVel)>500)
// 		g_pFrm->m_pSysParam->dSpeedFeedDownVel = 500;
// 
// 	strTemp.Format("%d", PLC_SEAD_PORT_FEEDDOWN_VEL);
// 	theApp.m_Serial.WritePLCData(strTemp,(int)g_pFrm->m_pSysParam->dSpeedFeedDownVel);
// 	strTemp.Format("%d", PLC_SEAD_PORT_UNFEEDDOWN_VEL);
// 	theApp.m_Serial.WritePLCData(strTemp,(int)g_pFrm->m_pSysParam->dSpeedFeedDownVel);

}

void CFeedModelCtrlDlg::OnBnClickedOk()
{
	 CString strTemp;
	GetDlgItemText(IDC_FEED_FEED_UPVEL, strTemp);
	g_pFrm->m_pSysParam->dSpeedFeedUpVel = atof(strTemp);

	GetDlgItemText(IDC_FEED_FEED_DOWNVEL, strTemp);
	g_pFrm->m_pSysParam->dSpeedFeedDownVel = atof(strTemp);

	if(fabs(g_pFrm->m_pSysParam->dSpeedFeedUpVel)>500)
		g_pFrm->m_pSysParam->dSpeedFeedUpVel = 500;

// 	strTemp.Format("%d", PLC_SEAD_PORT_FEEDUP_VEL);
// 	theApp.m_Serial.WritePLCData(strTemp,(int)g_pFrm->m_pSysParam->dSpeedFeedUpVel);
// 	strTemp.Format("%d", PLC_SEAD_PORT_UNFEEDUP_VEL);
// 	theApp.m_Serial.WritePLCData(strTemp,(int)g_pFrm->m_pSysParam->dSpeedFeedUpVel);
// 
// 	if(fabs(g_pFrm->m_pSysParam->dSpeedFeedDownVel)>500)
// 		g_pFrm->m_pSysParam->dSpeedFeedDownVel = 500;
// 
// 	strTemp.Format("%d", PLC_SEAD_PORT_FEEDDOWN_VEL);
// 	theApp.m_Serial.WritePLCData(strTemp,(int)g_pFrm->m_pSysParam->dSpeedFeedDownVel);
// 	strTemp.Format("%d", PLC_SEAD_PORT_UNFEEDDOWN_VEL);
// 	theApp.m_Serial.WritePLCData(strTemp,(int)g_pFrm->m_pSysParam->dSpeedFeedDownVel);


	g_pFrm->SysParam(FALSE);
	OnOK();
}

BOOL CFeedModelCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    CString strTemp;

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedFeedUpVel);
	SetDlgItemText(IDC_FEED_FEED_UPVEL, strTemp);

	strTemp.Format("%.3f",g_pFrm->m_pSysParam->dSpeedFeedDownVel);
	SetDlgItemText(IDC_FEED_FEED_DOWNVEL, strTemp);

	return TRUE; 
}

// 上料接驳台上升
void CFeedModelCtrlDlg::OnBnClickedDlgFeedctrlFeedup()
{
	CString strtemp,strtp1;
	GetDlgItemText(IDC_DLG_FEEDCTRL_FEEDUP,strtemp);
	GetDlgItemText(IDC_DLG_FEEDCTRL_FEEDDOWN,strtp1);
	if(strtp1 == _T("停止"))
	{
		AfxMessageBox(_T("先停止下降动作"));
		return ;
	}
	if(strtemp == _T("上升"))
	{
		g_pFrm->m_Robot->CommMotionCtrl(PLC_SEND_PORT_FEEDUP,TRUE);
		SetDlgItemText(IDC_DLG_FEEDCTRL_FEEDUP,_T("停止"));
		CFunction::DelaySec(0.3); 
	}else
	{
		g_pFrm->m_Robot->CommMotionCtrl(PLC_SEND_PORT_FEEDUP,FALSE);
		SetDlgItemText(IDC_DLG_FEEDCTRL_FEEDUP,_T("上升"));
		CFunction::DelaySec(0.3); 
	}
}

void CFeedModelCtrlDlg::OnBnClickedDlgFeedctrlFeeddown()
{
	CString strtemp,strtp1;
	GetDlgItemText(IDC_DLG_FEEDCTRL_FEEDDOWN,strtemp);
	GetDlgItemText(IDC_DLG_FEEDCTRL_FEEDUP,strtp1);
	if(strtp1 == _T("停止"))
	{
		AfxMessageBox(_T("先停止上升动作"));
		return ;
	}
	if(strtemp == _T("下降"))
	{
		g_pFrm->m_Robot->CommMotionCtrl(PLC_SEND_PORT_FEEDDOWN,TRUE);
		SetDlgItemText(IDC_DLG_FEEDCTRL_FEEDDOWN,_T("停止"));
		CFunction::DelaySec(0.3); 
	}else
	{
		g_pFrm->m_Robot->CommMotionCtrl(PLC_SEND_PORT_FEEDDOWN,FALSE);
		SetDlgItemText(IDC_DLG_FEEDCTRL_FEEDDOWN,_T("下降"));
		CFunction::DelaySec(0.3); 
	}
}

void CFeedModelCtrlDlg::OnBnClickedDlgFeedctrlUnfeedup()
{
	CString strtemp,strtp1;
	GetDlgItemText(IDC_DLG_FEEDCTRL_UNFEEDUP,strtemp);
	GetDlgItemText(IDC_DLG_FEEDCTRL_UNFEEDDOWN,strtp1);
	if(strtp1 == _T("停止"))
	{
		AfxMessageBox(_T("先停止下降动作"));
		return ;
	}
	if(strtemp == _T("上升"))
	{
		g_pFrm->m_Robot->CommMotionCtrl(PLC_SEND_PORT_UNFEEDUP,TRUE);
		SetDlgItemText(IDC_DLG_FEEDCTRL_UNFEEDUP,_T("停止"));
		CFunction::DelaySec(0.3); 
	}else
	{
		g_pFrm->m_Robot->CommMotionCtrl(PLC_SEND_PORT_UNFEEDUP,FALSE);
		SetDlgItemText(IDC_DLG_FEEDCTRL_UNFEEDUP,_T("上升"));
		CFunction::DelaySec(0.3); 
	}
}

void CFeedModelCtrlDlg::OnBnClickedDlgFeedctrlUnfeeddown()
{
	CString strtemp,strtp1;
	GetDlgItemText(IDC_DLG_FEEDCTRL_UNFEEDDOWN,strtemp);
	GetDlgItemText(IDC_DLG_FEEDCTRL_UNFEEDUP,strtp1);
	if(strtp1 == _T("停止"))
	{
		AfxMessageBox(_T("先停止上升动作"));
		return ;
	}
	if(strtemp == _T("下降"))
	{
		g_pFrm->m_Robot->CommMotionCtrl(PLC_SEND_PORT_UNFEEDDOWN,TRUE);
		SetDlgItemText(IDC_DLG_FEEDCTRL_UNFEEDDOWN,_T("停止"));
		CFunction::DelaySec(0.3); 
	}else
	{
		g_pFrm->m_Robot->CommMotionCtrl(PLC_SEND_PORT_UNFEEDDOWN,FALSE);
		SetDlgItemText(IDC_DLG_FEEDCTRL_UNFEEDDOWN,_T("下降"));
		CFunction::DelaySec(0.3); 
	}
}

void CFeedModelCtrlDlg::OnBnClickedOk2()
{
    g_pFrm->m_Robot->ResetFeed(PLC_SEND_PORT_FEEDRESET);
	CFunction::DelaySec(0.3);                          // s

}

void CFeedModelCtrlDlg::OnBnClickedOk3()
{
	g_pFrm->m_Robot->ResetFeed(PLC_SEND_PORT_UNFEEDRESET);
	CFunction::DelaySec(0.3);                          // s
}
