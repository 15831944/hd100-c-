// PageJet.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "PageJet.h"
#include "math.h"


// CPageJet dialog

IMPLEMENT_DYNAMIC(CPageJet, CPropertyPage)

CPageJet::CPageJet()
	: CPropertyPage(CPageJet::IDD)
	, m_PageJetPulse(0)
	, m_PageJetOffTime(0)
	, m_PageJetOnTime(0)
	, m_PageJetPress1(0)
	, m_PageJetPress2(0)
	, m_PageJetTp(0)
{

}

CPageJet::~CPageJet()
{
}

void CPageJet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PAGE_JET_EDIT_PULSE, m_PageJetPulse);
	DDX_Text(pDX, IDC_PAGE_JET_EDIT_OFFTIME, m_PageJetOffTime);
	DDX_Text(pDX, IDC_PAGE_JET_EDIT_ONTIME, m_PageJetOnTime);
	DDX_Text(pDX, IDC_PAGE_JET_EDIT_PRESS1, m_PageJetPress1);
	DDX_Text(pDX, IDC_PAGE_JET_EDIT_PRESS2, m_PageJetPress2);
	DDX_Text(pDX, IDC_PAGE_JET_EDIT_TP, m_PageJetTp);
}


BEGIN_MESSAGE_MAP(CPageJet, CPropertyPage)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_PAGE_JET_EDIT_PULSE, &CPageJet::OnEnChangePageJetEditPulse)
	ON_EN_CHANGE(IDC_PAGE_JET_EDIT_ONTIME, &CPageJet::OnEnChangePageJetEditOntime)
	ON_EN_CHANGE(IDC_PAGE_JET_EDIT_OFFTIME, &CPageJet::OnEnChangePageJetEditOfftime)
	ON_EN_CHANGE(IDC_PAGE_JET_EDIT_PRESS1, &CPageJet::OnEnChangePageJetEditPress1)
	ON_EN_CHANGE(IDC_PAGE_JET_EDIT_PRESS2, &CPageJet::OnEnChangePageJetEditPress2)
	ON_EN_CHANGE(IDC_PAGE_JET_EDIT_TP, &CPageJet::OnEnChangePageJetEditTp)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_CLOSEPRESS, &CPageJet::OnBnClickedButtonClosepress)
	ON_BN_CLICKED(IDC_BUTTON_OPENPRESS, &CPageJet::OnBnClickedButtonOpenpress)
	ON_BN_CLICKED(IDC_BUTTON_OPENHREAT, &CPageJet::OnBnClickedButtonOpenhreat)
END_MESSAGE_MAP()


// CPageJet message handlers

BOOL CPageJet::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_PageJetPulse = g_pFrm->m_pSysParam->nJetPulse;
	m_PageJetOffTime = g_pFrm->m_pSysParam->nCloseTime;
	m_PageJetOnTime = g_pFrm->m_pSysParam->nOpenTime;
	m_PageJetPress1 = g_pFrm->m_pSysParam->dPressJet;
	m_PageJetPress2 = g_pFrm->m_pSysParam->dPressDisp;
	m_PageJetTp = g_pFrm->m_pSysParam->dJetSetTemperature;
     
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageJet,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageJet,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageJet,"Tag_3"));
	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageJet,"Tag_4"));
	GetDlgItem(Tag_5)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageJet,"Tag_5"));
	GetDlgItem(Tag_6)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageJet,"Tag_6"));
	
	GetDlgItem(IDC_BUTTON_CLOSEPRESS)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageJet,"Btn_1"));
	GetDlgItem(IDC_BUTTON_OPENPRESS)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageJet,"Btn_2"));

	UpdateData(FALSE);

	// 获取加热状态
//	int nPlcRData = -1;
	Sleep(50);
	CString str;
	str.Format(_T("%d"),PLC_READ_PORT_STS_HEART);
// 	nPlcRData = theApp.m_Serial.ReadPlcRData(str);
// 	if(nPlcRData==0||nPlcRData==1)
// 	{
// 		if(nPlcRData==1)
// 		{   // 有加热
// 			GetDlgItem(IDC_BUTTON_OPENHREAT)->SetWindowText("关闭");
// 		}
// 		else if(nPlcRData==0)
// 		{
// 			GetDlgItem(IDC_BUTTON_OPENHREAT)->SetWindowText("打开");
// 		}
// 	}

	return TRUE;
}

void CPageJet::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//读取温度
	CString strTemp;
	strTemp.Format("%ld", PLC_READ_PORT_TEMPERATRUE);
// 	g_pFrm->m_pSysParam->dJetCurTemprature = theApp.m_Serial.ReadPlcData(strTemp)/10.0;
// 
// 	if(fabs(g_pFrm->m_pSysParam->dJetCurTemprature)>100)
// 	{
// 		g_pFrm->m_pSysParam->dJetCurTemprature = 0;
// 		SetDlgItemText(IDC_PAGE_JET_STATIC_TP, "");
// 	}
// 	else
// 	{
// 		strTemp.Format("%.3f", g_pFrm->m_pSysParam->dJetCurTemprature);
// 		SetDlgItemText(IDC_PAGE_JET_STATIC_TP, strTemp);
// 	}

	CPropertyPage::OnTimer(nIDEvent);
}

void CPageJet::OnEnChangePageJetEditMode()
{
	SetModified();
}

void CPageJet::OnEnChangePageJetEditPulse()
{
	SetModified();
}

void CPageJet::OnEnChangePageJetEditOntime()
{
	SetModified();
}

void CPageJet::OnEnChangePageJetEditOfftime()
{
	SetModified();
}

void CPageJet::OnEnChangePageJetEditPress1()
{
	SetModified();
}

void CPageJet::OnEnChangePageJetEditPress2()
{
	SetModified();
}

void CPageJet::OnEnChangePageJetEditTp()
{
	SetModified();
}

BOOL CPageJet::OnApply()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	g_pFrm->m_pSysParam->nJetPulse=m_PageJetPulse;
	g_pFrm->m_pSysParam->nCloseTime=m_PageJetOffTime;
	g_pFrm->m_pSysParam->nOpenTime=m_PageJetOnTime;

	if (m_PageJetPress1>0.6) 
		m_PageJetPress1 = 0.6;
	if (m_PageJetPress1<0) 
		m_PageJetPress1 = 0;
	if (m_PageJetPress2>0.6) 
 		m_PageJetPress2 = 0.6;
 	if (m_PageJetPress2<0) 
 		m_PageJetPress2 = 0;

 	g_pFrm->m_pSysParam->dPressJet=m_PageJetPress1;
 	g_pFrm->m_pSysParam->dPressDisp=m_PageJetPress2;

	g_pFrm->m_pSysParam->dJetSetTemperature=m_PageJetTp;
	CString strTemp;
	strTemp.Format("%d", PLC_SEND_PORT_TEMPERATRUE);
//	theApp.m_Serial.WritePLCData(strTemp,(int)(g_pFrm->m_pSysParam->dJetSetTemperature*10));

	g_pFrm->SysParam(FALSE);

	return CPropertyPage::OnApply();
}

void CPageJet::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if(bShow)
	{
		SetTimer(5,1000,NULL);
	}
	else
		KillTimer(5);
}
// 关闭胶筒压力

void CPageJet::OnBnClickedButtonClosepress()
{
  
   if(IDYES == AfxMessageBox(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageJet,"Msg_1"),MB_YESNO))
   {
	  g_pFrm->m_Robot->TubePressCtrl(0);	 
   }
}
// 打开 胶筒压力输出
void CPageJet::OnBnClickedButtonOpenpress()
{
	UpdateData(TRUE);
	if (m_PageJetPress1>0.6) 
		m_PageJetPress1 = 0.6;
	if (m_PageJetPress1<0) 
	 	m_PageJetPress1 = 0;
	g_pFrm->m_pSysParam->dPressJet=m_PageJetPress1;
    g_pFrm->m_Robot->TubePressCtrl(g_pFrm->m_pSysParam->dPressJet);

	if (m_PageJetPress2>0.6) 
		m_PageJetPress2 = 0.6;
	if (m_PageJetPress2<0) 
		m_PageJetPress2 = 0;
}

// 打开加热
void CPageJet::OnBnClickedButtonOpenhreat()
{
	short iRet;
	CString str;
     GetDlgItem(IDC_BUTTON_OPENHREAT)->GetWindowText(str);
	if( _T("关闭") ==str )
	{
        iRet = g_pFrm->m_Robot->ExOutput(_T("阀体加热"),TRUE);
         GetDlgItem(IDC_BUTTON_OPENHREAT)->SetWindowText(_T("打开"));
	} 
	if(_T("打开") == str)
	{
		iRet = g_pFrm->m_Robot->ExOutput(_T("阀体加热"),FALSE);
		GetDlgItem(IDC_BUTTON_OPENHREAT)->SetWindowText(_T("关闭"));
	} 
}
