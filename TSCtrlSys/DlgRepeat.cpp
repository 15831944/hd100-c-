// DlgStep.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgRepeat.h"
#include "BoardCalibDlg.h"
#include "MainFrm.h"
#include <math.h>

// CDlgStep dialog

IMPLEMENT_DYNAMIC(CDlgRepeat, CDialog)

CDlgRepeat::CDlgRepeat(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRepeat::IDD, pParent)
{
	m_pCmdLine   = NULL;
	m_iSelIndex  = 1;
	m_bExpand    = FALSE;

	m_dLTx = 0.0;
	m_dLTy = 0.0;
	m_dRBx = 0.0;
	m_dRBy = 0.0;
	m_dRTx = 0.0;
	m_dRTy = 0.0;
	m_nXnum = 0;
	m_dXSpace = 0.0;
	m_nYnum = 0;
	m_dYSpace = 0.0;
	m_nIndex = 0;
	m_nXStart = 0;
	m_nYStart = 0;
	m_nIndexEnd = 0;
	m_nDispUnitNum = 0;
}

CDlgRepeat::~CDlgRepeat()
{
    
}

void CDlgRepeat::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_BTN_SET_1, m_btn[0]);
	DDX_Control(pDX, IDC_BTN_MOV_1, m_btn[1]);
	DDX_Control(pDX, IDC_BTN_SET_2, m_btn[2]);
	DDX_Control(pDX, IDC_BTN_MOV_2, m_btn[3]);
	DDX_Control(pDX, IDC_BTN_SET_3, m_btn[4]);
	DDX_Control(pDX, IDC_BTN_MOV_3, m_btn[5]);

	DDX_Text(pDX, IDC_EDIT_LTX, m_dLTx);
	DDX_Text(pDX, IDC_EDIT_LTY, m_dLTy);
	DDX_Text(pDX, IDC_EDIT_RBX, m_dRBx);
	DDX_Text(pDX, IDC_EDIT_RBY, m_dRBy);
	DDX_Text(pDX, IDC_EDIT_RTX, m_dRTx);
	DDX_Text(pDX, IDC_EDIT_RTY, m_dRTy);
	DDX_Text(pDX, IDC_REPEAT_EDIT_XNUM, m_nXnum);
	DDX_Text(pDX, IDC_REPEAT_EDIT_XSPACE, m_dXSpace);
	DDX_Text(pDX, IDC_REPEAT_EDIT_YNUM, m_nYnum);
	DDX_Text(pDX, IDC_REPEAT_EDIT_YSPACE, m_dYSpace);
	DDX_Text(pDX, IDC_REPEAT_EDIT_INDEX, m_nIndex);
	DDX_Text(pDX, IDC_EDIT_XS, m_nXStart);
	DDX_Text(pDX, IDC_EDIT_YS, m_nYStart);

	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_REPEAT_EDIT_INDEX2, m_nIndexEnd);
	DDX_Text(pDX, IDC_EDIT_UNIT_NUM, m_nDispUnitNum);
}


BEGIN_MESSAGE_MAP(CDlgRepeat, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgRepeat::OnBnClickedOk)
	ON_BN_CLICKED(IDC_REPEAT_BTN_EXPAND, &CDlgRepeat::OnBnClickedButtonExpand)
// 	ON_EN_CHANGE(IDC_REPEAT_EDIT_XSPACE, &CDlgRepeat::OnEnChangeRepeatEditXspace)
// 	ON_EN_CHANGE(IDC_REPEAT_EDIT_XNUM, &CDlgRepeat::OnEnChangeRepeatEditXnum)
// 	ON_EN_CHANGE(IDC_REPEAT_EDIT_INDEX, &CDlgRepeat::OnEnChangeRepeatEditIndex)
// 	ON_EN_CHANGE(IDC_REPEAT_EDIT_REPARC, &CDlgRepeat::OnEnChangeRepeatEditReparc)
//	ON_BN_CLICKED(IDC_BUTTON_ARCMARK, &CDlgRepeat::OnBnClickedButtonArcmark)
	ON_BN_CLICKED(IDC_BTN_SET_1, &CDlgRepeat::OnBnClickedBtnSet1)
	ON_BN_CLICKED(IDC_BTN_MOV_1, &CDlgRepeat::OnBnClickedBtnMov1)
	ON_BN_CLICKED(IDC_BTN_SET_2, &CDlgRepeat::OnBnClickedBtnSet2)
	ON_BN_CLICKED(IDC_BTN_MOV_2, &CDlgRepeat::OnBnClickedBtnMov2)
	ON_BN_CLICKED(IDC_BTN_SET_3, &CDlgRepeat::OnBnClickedBtnSet3)
	ON_BN_CLICKED(IDC_BTN_MOV_3, &CDlgRepeat::OnBnClickedBtnMov3)
END_MESSAGE_MAP()


// CDlgStep message handlers

BOOL CDlgRepeat::OnInitDialog()
{
	CDialog::OnInitDialog();

// 	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Tag_0"));
// 	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Tag_1"));
// 	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Tag_2"));
// 	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Tag_3"));
// 	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Tag_4"));
// 	GetDlgItem(Tag_5)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Tag_5"));
// 	GetDlgItem(Tag_6)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Tag_6"));
// 	GetDlgItem(Tag_7)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Tag_7"));
//     
// 	GetDlgItem(IDC_REPEAT_RADIO1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Btn_1"));
// 	GetDlgItem(IDC_REPEAT_RADIO2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Btn_2"));
// 	GetDlgItem(IDC_REPEAT_BTN_EXPAND)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Btn_3"));
// 	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Btn_4"));
// 	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDRepeat,"Btn_5"));
     
//	double temparc;
	
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		
		SetDlgItemText(IDC_REPEAT_EDIT_TYPE, szCmdline[m_pCmdLine->tType]);

		strTemp.Format("行号范围:0 ~ %d", m_iSelIndex-1);
		SetDlgItemText(IDC_REPEAT_STATIC_NOTE, strTemp);

		UpdateCtrl(TRUE);
	}

	m_btn[0].SetIcon(IDI_ICON_SINGLE_ADJUST);
	m_btn[0].SetTooltipText("更新矩阵左上角坐标");
	m_btn[0].OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btn[1].SetIcon(IDI_ICON_MOVETO);
	m_btn[1].SetTooltipText("移动到矩阵左上角");
	m_btn[1].OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btn[2].SetIcon(IDI_ICON_SINGLE_ADJUST);
	m_btn[2].SetTooltipText("更新矩阵左上角坐标");
	m_btn[2].OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btn[3].SetIcon(IDI_ICON_MOVETO);
	m_btn[3].SetTooltipText("移动到矩阵右上角");
	m_btn[3].OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btn[4].SetIcon(IDI_ICON_SINGLE_ADJUST);
	m_btn[4].SetTooltipText("更新矩阵左上角坐标");
	m_btn[4].OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btn[5].SetIcon(IDI_ICON_MOVETO);
	m_btn[5].SetTooltipText("移动到矩阵右下角");
	m_btn[5].OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);

	return TRUE;  
}

void CDlgRepeat::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		UpdateCtrl(FALSE);
	}
	OnOK();
}

void CDlgRepeat::OnBnClickedButtonExpand()
{
	// TODO: Add your control notification handler code here
	OnBnClickedOk();
	m_bExpand = TRUE;
}

// void CDlgRepeat::OnBnClickedButtonArcmark()
// {
//       int iDlgResult = IDOK;
//       CString str;
// 	  double temparc;
// 	  CBoardCalibDlg  m_dlgBoardCheck;
// 	  m_dlgBoardCheck.m_pCmdLine = m_pCmdLine;
// 	  iDlgResult = m_dlgBoardCheck.DoModal();
// 	  if(iDlgResult == IDOK)
// 	  {
//           // 夹角数据
// 		  m_pCmdLine->pDblValue[2] = m_dlgBoardCheck.m_pCmdLine->pDblValue[2];
// 
// 		  temparc = 180*m_pCmdLine->pDblValue[2]/M_PI; // 弧度转角度显示
// 		  str.Format(_T("%.3f"),temparc);
// 		  SetDlgItemText(IDC_REPEAT_EDIT_REPARC,str);
// 	  }
// }

// 夹角数据的手动修改
// void CDlgRepeat::OnEnChangeRepeatEditReparc()
// {
// 	
// }



void CDlgRepeat::OnBnClickedBtnSet1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK == g_pFrm->m_Robot->AfxMessageBox(_T("是否更新当前位置"), MB_OKCANCEL))
	{
		g_pFrm->m_Precision.GetCurPosMM();
		if(m_pCmdLine != NULL)
		{
			m_pCmdLine->pDblValue[0] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
			m_pCmdLine->pDblValue[1] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];

			if (m_pCmdLine->pDblValue[4]<EPS && m_pCmdLine->pDblValue[5]<EPS) 
			{
				m_pCmdLine->pDblValue[4] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
				m_pCmdLine->pDblValue[5] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
			}
			if (m_pCmdLine->pDblValue[8]<EPS && m_pCmdLine->pDblValue[9]<EPS) 
			{
				m_pCmdLine->pDblValue[8] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
				m_pCmdLine->pDblValue[9] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
			}
		}

		UpdateCtrl(TRUE);
	}
}


void CDlgRepeat::OnBnClickedBtnMov1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (K_RUN_STS_STOP != g_pFrm->m_Robot->m_tSysStatus)	return;
	if (!g_pFrm->m_Robot->m_bHomeSuccess)					return;
	if (NULL == m_pCmdLine)									return;

	double dXpos = m_pCmdLine->pDblValue[0];
	double dYpos = m_pCmdLine->pDblValue[1];

	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, dXpos, dYpos, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
}


void CDlgRepeat::OnBnClickedBtnSet2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK == g_pFrm->m_Robot->AfxMessageBox(_T("是否更新当前位置"), MB_OKCANCEL))
	{
		g_pFrm->m_Precision.GetCurPosMM();
		if(m_pCmdLine != NULL)
		{
			m_pCmdLine->pDblValue[4] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
			m_pCmdLine->pDblValue[5] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
		}

		UpdateCtrl(TRUE);
	}
}


void CDlgRepeat::OnBnClickedBtnMov2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (K_RUN_STS_STOP != g_pFrm->m_Robot->m_tSysStatus)	return;
	if (!g_pFrm->m_Robot->m_bHomeSuccess)					return;
	if (NULL == m_pCmdLine)									return;

	double dXpos = m_pCmdLine->pDblValue[4];
	double dYpos = m_pCmdLine->pDblValue[5];

	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, dXpos, dYpos, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
}


void CDlgRepeat::OnBnClickedBtnSet3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDOK == g_pFrm->m_Robot->AfxMessageBox(_T("是否更新当前位置"), MB_OKCANCEL))
	{
		g_pFrm->m_Precision.GetCurPosMM();
		if(m_pCmdLine != NULL)
		{
			m_pCmdLine->pDblValue[8] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
			m_pCmdLine->pDblValue[9] = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
		}

		UpdateCtrl(TRUE);
	}
}


void CDlgRepeat::OnBnClickedBtnMov3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (K_RUN_STS_STOP != g_pFrm->m_Robot->m_tSysStatus)	return;
	if (!g_pFrm->m_Robot->m_bHomeSuccess)					return;
	if (NULL == m_pCmdLine)									return;

	double dXpos = m_pCmdLine->pDblValue[8];
	double dYpos = m_pCmdLine->pDblValue[9];

	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1, 0, dXpos, dYpos, g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove, g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
}

void CDlgRepeat::UpdateCtrl(BOOL bIfShow)	// 更新控件数值		bIfShow= TRUE:将数值显示到控件    FALSE:从控件读取数值到内存
{
	if (NULL == m_pCmdLine)		return;

	if (bIfShow)
	{
		m_dLTx = m_pCmdLine->pDblValue[0];
		m_dLTy = m_pCmdLine->pDblValue[1];
		m_dRTx = m_pCmdLine->pDblValue[4];
		m_dRTy = m_pCmdLine->pDblValue[5];
		m_dRBx = m_pCmdLine->pDblValue[8];
		m_dRBy = m_pCmdLine->pDblValue[9];
		m_nIndex = m_pCmdLine->pIntValue[0];
		CWnd::CheckRadioButton(IDC_REPEAT_RADIO1, IDC_REPEAT_RADIO2, IDC_REPEAT_RADIO1 +m_pCmdLine->pIntValue[1]);
		m_nXStart = m_pCmdLine->pIntValue[4];
		m_nYStart = m_pCmdLine->pIntValue[5];
		m_nXnum = m_pCmdLine->pIntValue[2];
		m_nYnum = m_pCmdLine->pIntValue[3];
		m_nIndexEnd = m_pCmdLine->pIntValue[6];
		m_nDispUnitNum = m_pCmdLine->pIntValue[7];

		UpdateData(FALSE);
	}
	else
	{
		UpdateData(TRUE);

		m_pCmdLine->pDblValue[0] = m_dLTx  ;
		m_pCmdLine->pDblValue[1] = m_dLTy  ;
		m_pCmdLine->pDblValue[4] = m_dRTx  ;
		m_pCmdLine->pDblValue[5] = m_dRTy  ;
		m_pCmdLine->pDblValue[8] = m_dRBx  ;
		m_pCmdLine->pDblValue[9] = m_dRBy  ;

		m_pCmdLine->pIntValue[0] = m_nIndex;
		m_pCmdLine->pIntValue[1] = CWnd::GetCheckedRadioButton(IDC_REPEAT_RADIO1, IDC_REPEAT_RADIO2)-IDC_REPEAT_RADIO1;
		m_pCmdLine->pIntValue[2] = m_nXnum ;
		m_pCmdLine->pIntValue[3] = m_nYnum ;
		m_pCmdLine->pIntValue[4] = m_nXStart;
		m_pCmdLine->pIntValue[5] = m_nYStart;	
		m_pCmdLine->pIntValue[6] = m_nIndexEnd;
		m_pCmdLine->pIntValue[7] = m_nDispUnitNum;
// 
// 		m_pCmdLine->iDotNum = 3;
// 		m_pCmdLine->iIntNum = 8;
// 		m_pCmdLine->iDblNum = 12;
	}
}