// DlgWcLine.cpp : implementation file
//

#include "stdafx.h"
#include "DlgWcLine.h"
#include "TSCtrlSys.h"
#include "math.h"
// CDlgWcLine dialog

IMPLEMENT_DYNAMIC(CDlgWcLine, CDialog)

CDlgWcLine::CDlgWcLine(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWcLine::IDD, pParent)
	, m_WcLineNum(0)
	, m_dWCLineTDate(0)    // 线段总重
	, m_dLineSpeed(0)
{
  m_pCmdLine = NULL;
  m_dWCLPointData = 0 ;    // 单粒重量
}

CDlgWcLine::~CDlgWcLine()
{
}

void CDlgWcLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_COMB_POS, m_ComboSelPos);
	DDX_Text(pDX, IDC_WCL_LINEPOINTNUM, m_WcLineNum);
	DDX_Text(pDX, IDC_WCL_LINEEIGHT, m_dWCLineTDate);
	DDX_Text(pDX, IDC_WCL_LINESPEED, m_dLineSpeed);
}


BEGIN_MESSAGE_MAP(CDlgWcLine, CDialog)
	ON_BN_CLICKED(Btn_1, &CDlgWcLine::OnBnClicked1)
	ON_BN_CLICKED(Btn_3, &CDlgWcLine::OnBnClicked3)
	ON_CBN_SELCHANGE(IDC_DLG_COMB_POS, &CDlgWcLine::OnCbnSelchangeDlgCombPos)
	ON_BN_CLICKED(Btn_2, &CDlgWcLine::OnBnClicked2)
	ON_BN_CLICKED(IDOK, &CDlgWcLine::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgWcLine::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_WCL_LINESPEED, &CDlgWcLine::OnEnChangeWclLinespeed)
	ON_EN_CHANGE(IDC_WCL_LINEEIGHT, &CDlgWcLine::OnEnChangeWclLineeight)
END_MESSAGE_MAP()


// 移动到
void CDlgWcLine::OnBnClicked1()
{
	int n = m_ComboSelPos.GetCurSel();
	g_pFrm->m_Robot->GotoSafeZpos();
    double x=m_pCmdLine->pDblValue[n*4+0];
	double y=m_pCmdLine->pDblValue[n*4+1];
	double z=m_pCmdLine->pDblValue[n*4+2];
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

}

BOOL CDlgWcLine::OnInitDialog()
{
	CDialog::OnInitDialog();
   
	m_ComboSelPos.AddString("称重起点");

	m_ComboSelPos.AddString("称重终点");
	m_ComboSelPos.SetCurSel(0);

    RefreshAxisPos();  // 位置刷新

    CString strtemp;

	 // 单粒重量
	m_dWCLPointData  = g_pFrm->m_pSysParam->dBCMGPorDOT;   
	strtemp.Format(_T("%.5f"),m_dWCLPointData);
	GetDlgItem(IDC_WCL_WPERPOINT)->SetWindowText(strtemp);

	m_dWCLineTDate = 20;                                 // 线段总重量
    m_WcLineNum = (int)(m_dWCLineTDate/m_dWCLPointData); // 出点个数
    m_dLineSpeed = 40;                                   // 称重划线速度

	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Tag_01"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Tag_3"));
	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Tag_4"));

	GetDlgItem(Btn_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Btn_1"));
	GetDlgItem(Btn_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Btn_2"));
	GetDlgItem(Btn_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Btn_3"));
	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Btn_4"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Btn_5"));

	UpdateData(FALSE);
	return TRUE;  
}

void CDlgWcLine::RefreshAxisPos()
{
	CString strTemp;
	int n = m_ComboSelPos.GetCurSel();
	if(m_pCmdLine != NULL)
	{
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+0]);
		SetDlgItemText(IDC_WCL_POSX, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+1]);
		SetDlgItemText(IDC_WCL_POSY, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+2]);
		SetDlgItemText(IDC_WCL_POSZ, strTemp);
   
		m_dWCLineTDate = m_pCmdLine->pDblValue[8];              // 线段总重量
		m_WcLineNum = (int)(m_dWCLineTDate/g_pFrm->m_pSysParam->dBCMGPorDOT); // 出点个数
		m_dLineSpeed = m_pCmdLine->pDblValue[9];                // 称重划线速度

		UpdateData(FALSE);
	}
}
// 应用
void CDlgWcLine::OnBnClicked3()
{
	CString strTemp;
	int n = m_ComboSelPos.GetCurSel();
	UpdateData(TRUE);
	if(m_pCmdLine != NULL)
	{
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+0]);
		SetDlgItemText(IDC_WCL_POSX, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+1]);
		SetDlgItemText(IDC_WCL_POSY, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+2]);
		SetDlgItemText(IDC_WCL_POSZ, strTemp);

        // 线段总重量
        m_pCmdLine->pDblValue[8] = m_dWCLineTDate;              

		// 出胶次数
		m_WcLineNum = (int)(m_dWCLineTDate / g_pFrm->m_pSysParam->dBCMGPorDOT); 
		m_pCmdLine->pIntValue[3] =  m_WcLineNum;   

        // 划线速度
		m_pCmdLine->pDblValue[9] =  m_dLineSpeed; 

		g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);
		
	}
      CheckPara();   // 检测

	UpdateData(FALSE);
}
void CDlgWcLine::CheckPara()
{
	// 划线位置
	double dxA,dyA,dzA,duA;
	double dxB,dyB,dzB,duB;

	dxA = m_pCmdLine->pDblValue[0];
	dyA = m_pCmdLine->pDblValue[1];
	dzA = m_pCmdLine->pDblValue[2];
	duA = m_pCmdLine->pDblValue[3];
	
	dxB = m_pCmdLine->pDblValue[4];
	dyB = m_pCmdLine->pDblValue[5];
	dzB = m_pCmdLine->pDblValue[6];
	duB = m_pCmdLine->pDblValue[7];
	
	//出胶速度 
	double dWorkVel  = m_pCmdLine->pDblValue[9];//m_cmdParam.tLine.dVel;

	// double DispWegiht  = m_pCmdLine->pDblValue[8];    // 出胶重量
	//
	double disx = dxB-dxA ;
	double disy = dyB-dyA ;
	double dis=sqrt((disx)*(disx)+(disy)*(disy));// 线段长   mm
	double t = dis / dWorkVel;             // 需要的时间    s
	t= 1000*t;                            // ms

	int tdis = (int)( t / (int)m_pCmdLine->pIntValue[3]);  // 计算每个点的时间间隔 ms
	tdis -= g_pFrm->m_Robot->m_DispCYtime;                 // 循环周期间隔
  
	if(tdis<=0)
	{
		AfxMessageBox(_T("速度与出胶时间参数不匹配，请重新设置"));
	}
}
void CDlgWcLine::OnCbnSelchangeDlgCombPos()
{
	UpdateData(TRUE);
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		int n = m_ComboSelPos.GetCurSel();
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+0]);
		SetDlgItemText(IDC_WCL_POSX, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+1]);
		SetDlgItemText(IDC_WCL_POSY, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+2]);
		SetDlgItemText(IDC_WCL_POSZ, strTemp);
		
	}
}
// 读轴位置
void CDlgWcLine::OnBnClicked2()
{
	CString strTemp;
	int n = m_ComboSelPos.GetCurSel();
	if(m_pCmdLine != NULL)
	{
		g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, n*4);
        
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+0]);
		SetDlgItemText(IDC_WCL_POSX, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+1]);
		SetDlgItemText(IDC_WCL_POSY, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+2]);
		SetDlgItemText(IDC_WCL_POSZ, strTemp);
	}

}

void CDlgWcLine::OnBnClickedOk()
{
	OnBnClicked3();
	OnOK();
}

void CDlgWcLine::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDlgWcLine::OnEnChangeWclLinespeed()
{
	UpdateData(TRUE);
}
void CDlgWcLine::OnEnChangeWclLineeight()
{
	UpdateData(TRUE);
}
