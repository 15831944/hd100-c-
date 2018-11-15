// BoardCalibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BoardCalibDlg.h"
#include "TSCtrlSys.h"
#include <math.h>
// CBoardCalibDlg dialog

IMPLEMENT_DYNAMIC(CBoardCalibDlg, CDialog)

CBoardCalibDlg::CBoardCalibDlg(CWnd* pParent /*=NULL*/)
: CDialog(CBoardCalibDlg::IDD, pParent)
{
   m_pCmdLine = NULL;
}

CBoardCalibDlg::~CBoardCalibDlg()
{
}

void CBoardCalibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBoardCalibDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_BMARK, &CBoardCalibDlg::OnCbnSelchangeComboBmark)
	ON_BN_CLICKED(IDC_BUTTON_BLEARN, &CBoardCalibDlg::OnBnClickedButtonBlearn)
	ON_BN_CLICKED(IDC_BTN_BMOVE, &CBoardCalibDlg::OnBnClickedBtnBmove)
	ON_BN_CLICKED(IDC_BTN_MOVE_CALCA, &CBoardCalibDlg::OnBnClickedBtnMoveCalca)
	ON_BN_CLICKED(IDCANCEL, &CBoardCalibDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CBoardCalibDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_MARK_EDIT_BA, &CBoardCalibDlg::OnEnChangeMarkEditBa)
	ON_BN_CLICKED(IDOK, &CBoardCalibDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBoardCalibDlgEx message handlers

BOOL CBoardCalibDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_BMARK);

	pBox->AddString("MARK A");
	pBox->AddString("MARK B");

	pBox->SetCurSel(0);
	CString temp;

	
	//GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgBoardCalib,"Tag_1"));
	//GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgBoardCalib,"Tag_2"));
	//GetDlgItem(IDC_BUTTON_BLEARN)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgBoardCalib,"Btn_1"));
	//GetDlgItem(IDC_BUTTON_PAT_BTEST)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgBoardCalib,"Btn_2"));
	//GetDlgItem(IDC_BUTTON_PAT_BSAVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgBoardCalib,"Btn_3"));
	GetDlgItem(IDC_BTN_BMOVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgBoardCalib,"Btn_4"));
	GetDlgItem(IDC_BTN_MOVE_CALCA)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgBoardCalib,"Btn_5"));
//	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgBoardCalib,"Btn_6"));

	OnCbnSelchangeComboBmark();
    RefreshDate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CBoardCalibDlg::RefreshDate()
{
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		double dStdAngle = 180*m_pCmdLine->pDblValue[2]/M_PI; // 弧度转角度显示
		strTemp.Format(_T("%f"),dStdAngle);
		SetDlgItemText(IDC_MARK_EDIT_BA,strTemp);
	}
}
// 切换显示模板
void CBoardCalibDlg::OnCbnSelchangeComboBmark()
{
	CComboBox *p = (CComboBox*)GetDlgItem(IDC_COMBO_BMARK);
	CString strTemp;
	int n = p->GetCurSel();
	if(n==0)
	{
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tBoardCalib_A.x);
		SetDlgItemText(IDC_MARK_EDIT_BX, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tBoardCalib_A.y);
		SetDlgItemText(IDC_MARK_EDIT_BY, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tBoardCalib_A.z);
		SetDlgItemText(IDC_MARK_EDIT_BZ, strTemp);
		strTemp.Format("%d", g_pFrm->m_pSysParam->m_BoardCaliAccA);
		SetDlgItemText(IDC_MARK_EDIT_BACCEPTANCE, strTemp);
	}
	if(n==1)
	{
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tBoardCalib_B.x);
		SetDlgItemText(IDC_MARK_EDIT_BX, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tBoardCalib_B.y);
		SetDlgItemText(IDC_MARK_EDIT_BY, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tBoardCalib_B.z);
		SetDlgItemText(IDC_MARK_EDIT_BZ, strTemp);
		strTemp.Format("%d", g_pFrm->m_pSysParam->m_BoardCaliAccB);
		SetDlgItemText(IDC_MARK_EDIT_BACCEPTANCE, strTemp);
	}
}
// 学习模板
void CBoardCalibDlg::OnBnClickedButtonBlearn()
{
	CString strFile, strTemp;
	CComboBox *p = (CComboBox*)GetDlgItem(IDC_COMBO_BMARK);

	//更新位置
	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
     g_pFrm->m_Precision.GetCurPosMM();

	int n = p->GetCurSel();
	if(n==0)
	{
		g_pFrm->m_pSysParam->tBoardCalib_A.x = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
		g_pFrm->m_pSysParam->tBoardCalib_A.y = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
		g_pFrm->m_pSysParam->tBoardCalib_A.z = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tBoardCalib_A.x);
		SetDlgItemText(IDC_MARK_EDIT_BX, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tBoardCalib_A.y);
		SetDlgItemText(IDC_MARK_EDIT_BY, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tBoardCalib_A.z);
		SetDlgItemText(IDC_MARK_EDIT_BZ, strTemp);

	}else
	{
		g_pFrm->m_pSysParam->tBoardCalib_B.x = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[X_AXIS];
		g_pFrm->m_pSysParam->tBoardCalib_B.y = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Y_AXIS];
		g_pFrm->m_pSysParam->tBoardCalib_B.z = g_pFrm->m_Robot-> m_pController->g_ExMtnPara->dfCurpos[Z_AXIS];
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tBoardCalib_B.x);
		SetDlgItemText(IDC_MARK_EDIT_BX, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tBoardCalib_B.y);
		SetDlgItemText(IDC_MARK_EDIT_BY, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tBoardCalib_B.z);
		SetDlgItemText(IDC_MARK_EDIT_BZ, strTemp);
	}

	CRect rect;
	GetWindowRect(&rect);
	rect.left = rect.left+1;
	MoveWindow(rect,TRUE);

	rect.left = rect.left-1;
	MoveWindow(rect,TRUE);
}
// 移动到模板位置
void CBoardCalibDlg::OnBnClickedBtnBmove()
{
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_COMBO_BMARK);
	CString strTemp;
	double x,y,z;
	int n = pBox->GetCurSel();
	if(0==n)
	{
		x = g_pFrm->m_pSysParam->tBoardCalib_A.x;
		y = g_pFrm->m_pSysParam->tBoardCalib_A.y;
	}
	else
	{
		x = g_pFrm->m_pSysParam->tBoardCalib_B.x;
		y = g_pFrm->m_pSysParam->tBoardCalib_B.y;
	}

	z = g_pFrm->m_pSysParam->tPixelAdjustPos.z; // 高度同标定模板高度

	g_pFrm->m_Robot->GotoSafeZpos();
	//g_pFrm->m_Robot->m_pController->LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParam[X_AXIS].m_Vmin,g_pFrm->m_mtrParam[X_AXIS].m_Acc);
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
}
// 计算夹角并保存
void CBoardCalibDlg::OnBnClickedBtnMoveCalca()
{
	SetDlgItemText(IDC_MARK_EDIT_BA,_T(""));

	double  dStdAngle=0;  // 弧度
	double  posA_X=g_pFrm->m_pSysParam->tBoardCalib_A.x;
	double  posA_Y=g_pFrm->m_pSysParam->tBoardCalib_A.y;

	double  posB_X=g_pFrm->m_pSysParam->tBoardCalib_B.x;
	double  posB_Y=g_pFrm->m_pSysParam->tBoardCalib_B.y;

	dStdAngle  = atan2(posB_Y-posA_Y,posB_X-posA_X);

	if(fabs(dStdAngle) > M_PI_2)  // 大于 
	{
		if(dStdAngle<0)
			dStdAngle += M_PI;
		else
			dStdAngle -= M_PI;
	}		

	g_pFrm->m_pSysParam->m_BoardArc = dStdAngle;  // 直线角度

	dStdAngle = 180*dStdAngle/M_PI; // 弧度转角度显示

	CString temp;
	temp.Format(_T("%f"),dStdAngle);
	SetDlgItemText(IDC_MARK_EDIT_BA,temp);
}
// 确定
void CBoardCalibDlg::OnBnClickedCancel()
{
	OnCancel();
}
// 测试
void CBoardCalibDlg::OnBnClickedButton1()
{
	CString str;
	GetDlgItemText(IDC_MARK_EDIT_BA,str);
	double drc = atof(str);  // 输入角度

	drc = M_PI*drc / 180;    // 角度转弧度
	g_pFrm->m_pSysParam->m_BoardArc = drc;  // 直线角度

}

void CBoardCalibDlg::OnEnChangeMarkEditBa()
{
	
}

void CBoardCalibDlg::OnBnClickedOk()
{
	if(m_pCmdLine != NULL)
	{
		m_pCmdLine->pDblValue[2] = g_pFrm->m_pSysParam->m_BoardArc;   // 
	}

	g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);

	OnOK();
}