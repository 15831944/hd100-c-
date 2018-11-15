// DlgNeedleZAdjust.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgNeedleZAdjust.h"
#include "math.h"


// CDlgNeedleZAdjust dialog

IMPLEMENT_DYNAMIC(CDlgNeedleZAdjust, CDialog)

CDlgNeedleZAdjust::CDlgNeedleZAdjust(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNeedleZAdjust::IDD, pParent)
{

}

CDlgNeedleZAdjust::~CDlgNeedleZAdjust()
{
}

void CDlgNeedleZAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_NEEDLE_ZH_LED, m_inZSensor);
}


BEGIN_MESSAGE_MAP(CDlgNeedleZAdjust, CDialog)
	ON_BN_CLICKED(IDC_DLG_NEEDLE_ZH_CALI, &CDlgNeedleZAdjust::OnBnClickedDlgNeedleZhCali)
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_DLG_NEEDLE_ZH_OFFSET, &CDlgNeedleZAdjust::OnBnClickedDlgNeedleZhOffset)
	ON_BN_CLICKED(IDC_DLG_NEEDLE_ZH_LED, &CDlgNeedleZAdjust::OnBnClickedDlgNeedleZhLed)
END_MESSAGE_MAP()


// CDlgNeedleZAdjust message handlers

BOOL CDlgNeedleZAdjust::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strTemp;

	if (g_pFrm->m_pSysParam->nToolType==0)
	{
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleZAdjustPos.x);
		SetDlgItemText(IDC_DLG_NEEDLE_ZH_EDIT_X, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleZAdjustPos.y);
		SetDlgItemText(IDC_DLG_NEEDLE_ZH_EDIT_Y, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tNeedleZAdjustPos.z);
		SetDlgItemText(IDC_DLG_NEEDLE_ZH_EDIT_Z, strTemp);

		strTemp.Format("%.3f", g_pFrm->m_pSysParam->dCaliNeedleStdHeight);
		SetDlgItemText(IDC_DLG_NEEDLE_ZH_EDIT_STD, strTemp);

		SetDlgItemText(IDC_DLG_NEEDLE_ZH_STATIC,_T("喷射阀高度校正"));

		strTemp.Format("%.3f",g_pFrm->m_pSysParam->dCaliJetDelta);
		SetDlgItemText(IDC_DLG_NEEDLE_ZH_EDIT_DELTA,strTemp);
	
	}
	else if(g_pFrm->m_pSysParam->nToolType==1)
	{
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tJetZAdjustPos.x);
		SetDlgItemText(IDC_DLG_NEEDLE_ZH_EDIT_X, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tJetZAdjustPos.y);
		SetDlgItemText(IDC_DLG_NEEDLE_ZH_EDIT_Y, strTemp);
		strTemp.Format("%.3f", g_pFrm->m_pSysParam->tJetZAdjustPos.z);
		SetDlgItemText(IDC_DLG_NEEDLE_ZH_EDIT_Z, strTemp);

		strTemp.Format("%.3f", g_pFrm->m_pSysParam->dCaliJetStdHeight);
		SetDlgItemText(IDC_DLG_NEEDLE_ZH_EDIT_STD, strTemp);

		SetDlgItemText(IDC_DLG_NEEDLE_ZH_STATIC,_T("针筒高度校正"));

		strTemp.Format("%.3f",g_pFrm->m_pSysParam->dCaliNeedleDelta);
		SetDlgItemText(IDC_DLG_NEEDLE_ZH_EDIT_DELTA,strTemp);
	}

	m_inZSensor.SetIcon(IDI_LED_ON,IDI_LED_OFF);
	m_inZSensor.OffsetColor(CButtonST::BTNST_COLOR_BK_IN,30);
	m_inZSensor.SetFlat(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNeedleZAdjust::OnBnClickedDlgNeedleZhCali()
{
	// TODO: Add your control notification handler code here
	short sTemp;
	double dValue=0;

	double x=0,y=0,z=0;

	g_pFrm->m_Robot->GotoSafeZpos();

	if (g_pFrm->m_pSysParam->nToolType==1)
	{
		x = g_pFrm->m_pSysParam->tNeedleZAdjustPos.x;
		y = g_pFrm->m_pSysParam->tNeedleZAdjustPos.y;
		z = g_pFrm->m_pSysParam->tNeedleZAdjustPos.z;

	}
	else if(g_pFrm->m_pSysParam->nToolType==0)
	{
		x = g_pFrm->m_pSysParam->tJetZAdjustPos.x;
		y = g_pFrm->m_pSysParam->tJetZAdjustPos.y;
		z = g_pFrm->m_pSysParam->tJetZAdjustPos.z;
	}

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

	sTemp = g_pFrm->m_Robot->m_pController->AxisNeedleCheckPort(Z_AXIS,z+5,2,100,g_pFrm->m_Robot->inSenseCheckZ,&dValue,FALSE);
	if(sTemp!=RTN_NONE) return;

	sTemp = g_pFrm->m_Robot->m_pController->AxisStop(0x0001<<Z_AXIS,TRUE,TRUE);
	if(sTemp!=RTN_NONE) return;

	if(dValue>0)
	{
		CString str;
		str.Format("%.3f",dValue);
		SetDlgItemText(IDC_DLG_NEEDLE_ZH_EDIT_STD,str);

		if (g_pFrm->m_pSysParam->nToolType==1)
		{
			g_pFrm->m_pSysParam->dCaliNeedleStdHeight = dValue;
		}
		else
		{
			g_pFrm->m_pSysParam->dCaliJetStdHeight = dValue;
		}
	}

	g_pFrm->m_Robot->GotoSafeZpos();
}

void CDlgNeedleZAdjust::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	g_pFrm->SysParam(FALSE);
	g_pView->Invalidate();

	CDialog::OnClose();
}

void CDlgNeedleZAdjust::UpdateZSensorSts(long sts)
{
	// TODO: Add your message handler code here and/or call default
	if((sts&IOstatusBitsMask[g_pFrm->m_Robot->inSenseCheckZ])==IOstatusBitsMask[g_pFrm->m_Robot->inSenseCheckZ])
		m_inZSensor.SetIcon(IDI_LED_ON);
	else
		m_inZSensor.SetIcon(IDI_LED_OFF);

}

void CDlgNeedleZAdjust::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
	if (nState==1)
	{
		if(g_pFrm->m_pSysParam->nToolType==1)
		{
			SetDlgItemText(IDC_DLG_NEEDLE_ZH_STATIC,_T("针筒高度校正"));

			if(theApp.m_SysUser.m_CurUD.level>GM_PERSONNEL)
				GetDlgItem(IDC_DLG_NEEDLE_ZH_CALI)->ShowWindow(SW_SHOW);
			else
				GetDlgItem(IDC_DLG_NEEDLE_ZH_CALI)->ShowWindow(SW_HIDE);

		}
		else if(g_pFrm->m_pSysParam->nToolType==0)
			SetDlgItemText(IDC_DLG_NEEDLE_ZH_STATIC,_T("喷射阀高度校正"));
	}
}

void CDlgNeedleZAdjust::OnBnClickedDlgNeedleZhOffset()
{
	// TODO: Add your control notification handler code here
	short sTemp;
	double dValue=0;
	double dStrValue = 0;

	double x=0,y=0,z=0;

	g_pFrm->m_Robot->GotoSafeZpos();

	if (g_pFrm->m_pSysParam->nToolType==1)
	{
		x = g_pFrm->m_pSysParam->tNeedleZAdjustPos.x;
		y = g_pFrm->m_pSysParam->tNeedleZAdjustPos.y;
		z = g_pFrm->m_pSysParam->tNeedleZAdjustPos.z;

	}
	else if(g_pFrm->m_pSysParam->nToolType==0)
	{
		x = g_pFrm->m_pSysParam->tJetZAdjustPos.x;
		y = g_pFrm->m_pSysParam->tJetZAdjustPos.y;
		z = g_pFrm->m_pSysParam->tJetZAdjustPos.z;
	}

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

	sTemp = g_pFrm->m_Robot->m_pController->AxisNeedleCheckPort(Z_AXIS,z+5,2,100,g_pFrm->m_Robot->inSenseCheckZ,&dValue,FALSE);
	if(sTemp!=RTN_NONE) return;

	sTemp = g_pFrm->m_Robot->m_pController->AxisStop(0x0001<<Z_AXIS,TRUE,TRUE);
	if(sTemp!=RTN_NONE) return;

	if (g_pFrm->m_pSysParam->nToolType==1)
	{
		g_pFrm->m_pSysParam->dCaliNeedleDelta = dValue - g_pFrm->m_pSysParam->dCaliNeedleStdHeight;

		if(fabs(g_pFrm->m_pSysParam->dCaliNeedleDelta)>10)
			g_pFrm->m_pSysParam->dCaliNeedleDelta = 0;
		dStrValue = g_pFrm->m_pSysParam->dCaliNeedleDelta;

	}
	else
	{
		g_pFrm->m_pSysParam->dCaliJetDelta = dValue - g_pFrm->m_pSysParam->dCaliJetStdHeight ;
		if(fabs(g_pFrm->m_pSysParam->dCaliJetDelta)>10)
			g_pFrm->m_pSysParam->dCaliJetDelta = 0;

		dStrValue = g_pFrm->m_pSysParam->dCaliJetDelta;
	}

	CString str;
	str.Format("%.3f",dStrValue);
	SetDlgItemText(IDC_DLG_NEEDLE_ZH_EDIT_DELTA,str);


	g_pFrm->m_Robot->GotoSafeZpos();
}

void CDlgNeedleZAdjust::OnBnClickedDlgNeedleZhLed()
{
	// TODO: Add your control notification handler code here
}
