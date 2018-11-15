// DlgWcPoint.cpp : implementation file
//

#include "stdafx.h"
#include "DlgWcPoint.h"
#include "TSCtrlSys.h"

// CDlgWcPoint dialog

IMPLEMENT_DYNAMIC(CDlgWcPoint, CDialog)

CDlgWcPoint::CDlgWcPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWcPoint::IDD, pParent)
	, m_WLPNum(0)
	, m_dWCLPointTDate(0) // 总重量
{
  m_pCmdLine = NULL;
  m_WPLDrop  = NULL;   // 单点重量
}

CDlgWcPoint::~CDlgWcPoint()
{
}

void CDlgWcPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DLG_WL_POINTNUM, m_WLPNum);
	DDX_Text(pDX, IDC_WL_DISPMG, m_dWCLPointTDate);
}


BEGIN_MESSAGE_MAP(CDlgWcPoint, CDialog)
	ON_BN_CLICKED(Btn_1, &CDlgWcPoint::OnBnClicked1)
	ON_BN_CLICKED(Btn_2, &CDlgWcPoint::OnBnClicked2)
	ON_BN_CLICKED(Btn_3, &CDlgWcPoint::OnBnClicked3)
	ON_BN_CLICKED(IDOK, &CDlgWcPoint::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgWcPoint::OnBnClickedCancel)
END_MESSAGE_MAP()

// 移动到
void CDlgWcPoint::OnBnClicked1()
{
	double x,y,z;
	x = m_pCmdLine->pDblValue[0];
	y = m_pCmdLine->pDblValue[1];
    z = m_pCmdLine->pDblValue[2];
	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
}

BOOL CDlgWcPoint::OnInitDialog()
{
	CDialog::OnInitDialog();

    CString strTemp;
    
	RefreshAxisPos();                                    // 更新数据

    m_WPLDrop  = g_pFrm->m_pSysParam->dBCMGPorDOT ;      // 单点重量
	strTemp.Format(_T("%.4f"),m_WPLDrop);
	GetDlgItem(IDC_WL_DISPDROPDATE)->SetWindowText(strTemp);

    SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Tag_2"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Tag_3"));
   
	GetDlgItem(Btn_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Btn_1"));
	GetDlgItem(Btn_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Btn_2"));
	GetDlgItem(Btn_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Btn_3"));
	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Btn_4"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCPL,"Btn_5"));
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgWcPoint::RefreshAxisPos()
{
	CString strTemp;
	if(m_pCmdLine != NULL)
	{
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_WLPOSX, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_WLPOSY, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_WLPOSZ, strTemp);
         
		m_dWCLPointTDate = m_pCmdLine->pDblValue[4];                      // 称重重量
		m_WLPNum = (int)(m_dWCLPointTDate / g_pFrm->m_pSysParam->dBCMGPorDOT);   // 打点次数

		UpdateData(FALSE);
	}
}
// 读取轴位置
void CDlgWcPoint::OnBnClicked2()
{
	CString strTemp;

	if(m_pCmdLine != NULL)
	{
		g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, 0);
		
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_WLPOSX, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_WLPOSY, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_WLPOSZ, strTemp);
	}
}
// 应用
void CDlgWcPoint::OnBnClicked3()
{
	 UpdateData(TRUE);
   
	 m_pCmdLine->pIntValue[1] = 1;                 // 开阀
	 m_pCmdLine->pDblValue[4] = m_dWCLPointTDate ; // 称重总数

     m_WLPNum = (int)(m_dWCLPointTDate / g_pFrm->m_pSysParam->dBCMGPorDOT);
     m_pCmdLine->pIntValue[4]=  m_WLPNum;   // 打点次数

     RefreshAxisPos();
	 g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);
}

void CDlgWcPoint::OnBnClickedOk()
{
	OnBnClicked3(); 
	OnOK();
}

void CDlgWcPoint::OnBnClickedCancel()
{
	OnCancel();
}