// PageBanleceSet.cpp : implementation file
//

#include "stdafx.h"
#include "PageBanleceSet.h"
#include "DlgJogXYZ.h"
#include "TSCtrlSys.h"
// CPageBanleceSet dialog

IMPLEMENT_DYNAMIC(CPageBanleceSet, CPropertyPage)

CPageBanleceSet::CPageBanleceSet()
	: CPropertyPage(CPageBanleceSet::IDD)
	, m_dBCPosX(0)
	, m_dBCPosY(0)
	, m_dBCPosZ(0)
	, m_dOpenDispTime(0)
	, m_dDispDis(0)
	, m_BCDispNum(0)
	, m_BCMProDot(0)
	, m_BCMCurData(0)
{

}

CPageBanleceSet::~CPageBanleceSet()
{
}

void CPageBanleceSet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_XPOS_BANLECE, m_dBCPosX);
	DDX_Text(pDX, IDC_EDIT_YPOS_BANLECE, m_dBCPosY);
	DDX_Text(pDX, IDC_EDIT_ZPOS_BANLECE, m_dBCPosZ);
	DDX_Text(pDX, IDC_EDIT_OPENDISPTIME, m_dOpenDispTime);
	DDX_Text(pDX, IDC_EDIT_DISPDISTIME, m_dDispDis);
	DDX_Text(pDX, IDC_EDIT_BCDISPNUM, m_BCDispNum);
	DDX_Text(pDX, IDC_EDIT_BCGGPB, m_BCMProDot);
	DDX_Text(pDX, IDC_EDIT_BCDATE, m_BCMCurData);
}


BEGIN_MESSAGE_MAP(CPageBanleceSet, CPropertyPage)
	ON_BN_CLICKED(IDC_BT_SETBANCELEPOS, &CPageBanleceSet::OnBnClickedBtSetbancelepos)
	ON_BN_CLICKED(IDC_BT_MOVETOBCPOS, &CPageBanleceSet::OnBnClickedBtMovetobcpos)
	ON_BN_CLICKED(IDC_BT_BANLENCE_ZERO, &CPageBanleceSet::OnBnClickedBtBanlenceZero)
	ON_BN_CLICKED(IDC_BUTTON_DOBC, &CPageBanleceSet::OnBnClickedButtonDobc)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLACBCPD, &CPageBanleceSet::OnBnClickedButtonClacbcpd)
	ON_EN_CHANGE(IDC_EDIT_DISPDISTIME, &CPageBanleceSet::OnEnChangeEditDispdistime)
	ON_EN_CHANGE(IDC_EDIT_XPOS_BANLECE, &CPageBanleceSet::OnEnChangeEditXposBanlece)
	ON_EN_CHANGE(IDC_EDIT_YPOS_BANLECE, &CPageBanleceSet::OnEnChangeEditYposBanlece)
	ON_EN_CHANGE(IDC_EDIT_ZPOS_BANLECE, &CPageBanleceSet::OnEnChangeEditZposBanlece)
	ON_EN_CHANGE(IDC_EDIT_OPENDISPTIME, &CPageBanleceSet::OnEnChangeEditOpendisptime)
	ON_EN_CHANGE(IDC_EDIT_BCDISPNUM, &CPageBanleceSet::OnEnChangeEditBcdispnum)
	ON_EN_CHANGE(IDC_EDIT_BCGGPB, &CPageBanleceSet::OnEnChangeEditBcggpb)
END_MESSAGE_MAP()

//标定称重位置
void CPageBanleceSet::OnBnClickedBtSetbancelepos()
{

	CDlgJogXYZ m_JogXYZ(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Tag_8"));

	if(IDOK!=m_JogXYZ.DoModal()) return;

	g_pFrm->m_pSysParam->tBCTestPos.x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
	g_pFrm->m_pSysParam->tBCTestPos.y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
	g_pFrm->m_pSysParam->tBCTestPos.z = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];

	m_dBCPosX = g_pFrm->m_pSysParam->tBCTestPos.x ;
	m_dBCPosY = g_pFrm->m_pSysParam->tBCTestPos.y ;
	m_dBCPosZ = g_pFrm->m_pSysParam->tBCTestPos.z ;

    UpdateData(FALSE);

	SetModified();
}

// 运动到称重位
void CPageBanleceSet::OnBnClickedBtMovetobcpos()
{
	UpdateData(TRUE);
	g_pFrm->m_pSysParam->tBCTestPos.x = m_dBCPosX;
	g_pFrm->m_pSysParam->tBCTestPos.y = m_dBCPosY;
	g_pFrm->m_pSysParam->tBCTestPos.z = m_dBCPosZ;

	double x=0,y=0,z=0;

	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		x = g_pFrm->m_pSysParam->tBCTestPos.x;
		y = g_pFrm->m_pSysParam->tBCTestPos.y;
		z = g_pFrm->m_pSysParam->tBCTestPos.z;

		g_pFrm->m_Robot->GotoSafeZpos();
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
	}
}
// 天平清零
void CPageBanleceSet::OnBnClickedBtBanlenceZero()
{
//	  theApp.m_Serial.BalanceZero();

	  CString str = _T("");
	  SetDlgItemText(IDC_STATIC_BCSTS,str);// 状态
}

// 单粒称重功能
void CPageBanleceSet::OnBnClickedButtonDobc()
{
	UpdateData(TRUE);
	// 移动到称重位
	g_pFrm->m_pSysParam->tBCTestPos.x = m_dBCPosX;
	g_pFrm->m_pSysParam->tBCTestPos.y = m_dBCPosY;
	g_pFrm->m_pSysParam->tBCTestPos.z = m_dBCPosZ;
 
	double x=0,y=0,z=0;
	CString strtempx,tempy;
	if(g_pFrm->m_Robot->m_pController->IsInitOk())
	{
		x = g_pFrm->m_pSysParam->tBCTestPos.x;
		y = g_pFrm->m_pSysParam->tBCTestPos.y;
		z = g_pFrm->m_pSysParam->tBCTestPos.z;

#ifdef  DEBUG_SD950
		strtempx.Format(_T("%.3f"),x);
		tempy.Format(_T("%.3f"),y);
		tempy= _T("X:")+strtempx+_T("Y:")+tempy;
        g_pFrm->AddMsg(tempy);
#endif
		x = x +(g_pFrm->m_pSysParam->tOffset[0].x);
		y = y +(g_pFrm->m_pSysParam->tOffset[0].y);

#ifdef  DEBUG_SD950
		strtempx.Format(_T("%.3f"),x);
		tempy.Format(_T("%.3f"),y);
		tempy= _T("X:")+strtempx+_T("Y:")+tempy;
		g_pFrm->AddMsg(tempy);
#endif
		g_pFrm->m_Robot->GotoSafeZpos();
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
		g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
	}
	else
	{
		return ;
	}
	
	int Tem= (int) m_BCDispNum;  // 次数
	if (Tem>0)
	{  
		if (g_pFrm->m_pSysParam->nToolType == 0)
		{
			// 阀体动作
			g_pFrm->m_Robot->JetOutputEx(Tem,(long)m_dOpenDispTime,(long)m_dDispDis,TRUE);
		}
	}
	else
	{
		AfxMessageBox(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,_T("Msg_1")));
	}
   
}

BOOL CPageBanleceSet::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

   bNeedBC = TRUE;
   // 天平参数
   m_dBCPosX = g_pFrm->m_pSysParam->tBCTestPos.x;
   m_dBCPosY = g_pFrm->m_pSysParam->tBCTestPos.y;
   m_dBCPosZ = g_pFrm->m_pSysParam->tBCTestPos.z;

   m_dOpenDispTime = g_pFrm->m_pSysParam->dBCDispTime;
   m_dDispDis = g_pFrm->m_pSysParam->dBCDispDis;
   m_BCDispNum = g_pFrm->m_pSysParam->dBCDispNum;
   m_BCMProDot = g_pFrm->m_pSysParam->dBCMGPorDOT;
   m_BCMCurData = 0.0;  //  天平数据
    
   UpdateData(FALSE);
   GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Tag_1"));
   GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Tag_2"));
   GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Tag_3"));
   GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Tag_4"));
   GetDlgItem(Tag_5)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Tag_5"));
   GetDlgItem(Tag_6)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Tag_6"));
   GetDlgItem(Tag_7)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Tag_7"));
   GetDlgItem(Tag_8)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Tag_9"));

   GetDlgItem(IDC_BT_SETBANCELEPOS)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Btn_1"));
   GetDlgItem(IDC_BT_MOVETOBCPOS)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Btn_2"));
   GetDlgItem(IDC_BT_BANLENCE_ZERO)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Btn_3"));
   GetDlgItem(IDC_BUTTON_DOBC)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Btn_4"));
   GetDlgItem(IDC_BUTTON_CLACBCPD)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,"Btn_5"));
   

   SetTimer(5,100,NULL);

	return TRUE;  
}

BOOL CPageBanleceSet::OnApply()
{
	UpdateData(TRUE);

	// 天平参数
	g_pFrm->m_pSysParam->tBCTestPos.x = m_dBCPosX;
	g_pFrm->m_pSysParam->tBCTestPos.y = m_dBCPosY;
	g_pFrm->m_pSysParam->tBCTestPos.z = m_dBCPosZ;

	g_pFrm->m_pSysParam->dBCDispTime = m_dOpenDispTime;
    g_pFrm->m_pSysParam->dBCDispDis  = m_dDispDis;
	g_pFrm->m_pSysParam->dBCDispNum  = m_BCDispNum;
	g_pFrm->m_pSysParam->dBCMGPorDOT = m_BCMProDot;

	return CPropertyPage::OnApply();
}
// 计算 单点重量 
void CPageBanleceSet::OnBnClickedButtonClacbcpd()
{
	UpdateData(TRUE);
	long    Tem= (long)m_BCDispNum ;    // 单周期点胶次数
	double	m_temWeight= m_BCMCurData;  // 称重值 mg

	if (Tem>0)
	{
		g_pFrm->m_pSysParam->dBCMGPorDOT= m_temWeight / Tem;
	}
	else
	{
        AfxMessageBox(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,_T("Msg_1")));
	}
    
	m_BCMProDot = g_pFrm->m_pSysParam->dBCMGPorDOT;
	UpdateData(FALSE);

	SetDlgItemText(IDC_STATIC_BCSTS,_T(""));// 状态
}

void CPageBanleceSet::OnTimer(UINT_PTR nIDEvent)
{
//	double dBCData;
	CString str ;
//	int nRet;

	if(bNeedBC)
	{
// 		nRet=theApp.m_Serial.ReadBalance(dBCData);
// 		if ( nRet == -1 )
// 		{
// 			str = g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,_T("Msg_2"));
// 			SetDlgItemText(IDC_STATIC_BCSTS,str);  // 状态
//              m_BCMCurData=0;
// 			str.Format("%.5f",m_BCMCurData);
// 			SetDlgItemText(IDC_EDIT_BCDATE,str);  // 称重数据
// 		}
// 		else if ( nRet == -2 )
// 		{
// 			str = g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,_T("Msg_3"));
// 			g_pFrm->m_dlgJetAdjust.SetDlgItemText(IDC_STATIC_BCSTS,str); // 状态
//                m_BCMCurData=0;
// 			str.Format("%.5f",0);
// 			SetDlgItemText(IDC_EDIT_BCDATE,str);  // 称重数据
// 		}
// 		else if(nRet == 1)
// 		{
// 			if(dBCData<0.000001) 
// 				dBCData = 0.00000;
// 
//             dBCData *=1000 ;  // g -> mg
//             
// 			 m_BCMCurData = dBCData;
// 			str.Format("%.3f",m_BCMCurData);
// 			SetDlgItemText(IDC_EDIT_BCDATE,str);  // 称重数据
// 
// 			str = g_pFrm->LanguageMgr->ReadStringFromIni(DefPageBalance,_T("Msg_4"));
// 			SetDlgItemText(IDC_STATIC_BCSTS,str);// 状态
// 		}
	}else
	{
		str = _T("");
	     SetDlgItemText(IDC_STATIC_BCSTS,str);// 状态
	}
	CPropertyPage::OnTimer(nIDEvent);
}

void CPageBanleceSet::OnEnChangeEditDispdistime()
{
	SetModified();
}

void CPageBanleceSet::OnEnChangeEditXposBanlece()
{
	SetModified();
}

void CPageBanleceSet::OnEnChangeEditYposBanlece()
{
	SetModified();
}

void CPageBanleceSet::OnEnChangeEditZposBanlece()
{
	SetModified();
}

void CPageBanleceSet::OnEnChangeEditOpendisptime()
{
	SetModified();
}

void CPageBanleceSet::OnEnChangeEditBcdispnum()
{
	SetModified();
}

void CPageBanleceSet::OnEnChangeEditBcggpb()
{
	SetModified();
}