// DlgZHeight.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgZHeight.h"
#include <math.h>


// CDlgZHeight dialog

IMPLEMENT_DYNAMIC(CDlgZHeight, CDialog)

CDlgZHeight::CDlgZHeight(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgZHeight::IDD, pParent)
{
   m_pCmdLine = NULL;
}

CDlgZHeight::~CDlgZHeight()
{
}

void CDlgZHeight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgZHeight, CDialog)	
	ON_BN_CLICKED(IDOK, &CDlgZHeight::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ZHEIGHT_APPLY, &CDlgZHeight::OnBnClickedZheightApply)
	ON_BN_CLICKED(IDC_ZHEIGHT_READ_AXISPOS, &CDlgZHeight::OnBnClickedZheightReadAxispos)
	ON_BN_CLICKED(IDC_ZHEIGHT_MOVE, &CDlgZHeight::OnBnClickedZheightMove)
	ON_BN_CLICKED(IDC_ZHEIGHT_SAVE, &CDlgZHeight::OnBnClickedZheightSave)
	ON_BN_CLICKED(IDCANCEL, &CDlgZHeight::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgZHeight message handlers

BOOL CDlgZHeight::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if(m_pCmdLine != NULL)
	{
		RefreshAxisPos();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgZHeight::RefreshAxisPos(void)
{
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_ZHEIGHT_EDIT_X, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_ZHEIGHT_EDIT_Y, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_ZHEIGHT_EDIT_Z, strTemp);
	
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[4]);//基位高度
		SetDlgItemText(IDC_ZHEIGHT_EDIT_B1, strTemp);
		SetDlgItemText(IDC_ZHEIGHT_EDIT_M1, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[5]);//延时
		SetDlgItemText(IDC_ZHEIGHT_EDIT_T2, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[6]);//点胶高度
		SetDlgItemText(IDC_EDIT_DISP_HEIGHT, strTemp);
//		OnBnClickedZheightSave();
	}
}

void CDlgZHeight::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnBnClickedZheightApply();
	OnOK();
}

void CDlgZHeight::OnBnClickedZheightApply()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		GetDlgItemText(IDC_ZHEIGHT_EDIT_X, strTemp);
		m_pCmdLine->pDblValue[0] =atof(strTemp);
		GetDlgItemText(IDC_ZHEIGHT_EDIT_Y, strTemp);
		m_pCmdLine->pDblValue[1] =atof(strTemp);
		GetDlgItemText(IDC_ZHEIGHT_EDIT_Z, strTemp);
		m_pCmdLine->pDblValue[2] =atof(strTemp);

		GetDlgItemText(IDC_ZHEIGHT_EDIT_M1, strTemp);
		SetDlgItemText(IDC_ZHEIGHT_EDIT_B1, strTemp);
		m_pCmdLine->pDblValue[4] =atof(strTemp);
		GetDlgItemText(IDC_ZHEIGHT_EDIT_T2, strTemp);
		m_pCmdLine->pDblValue[5] =atof(strTemp);

		GetDlgItemText(IDC_EDIT_DISP_HEIGHT, strTemp);
		m_pCmdLine->pDblValue[6] =atof(strTemp);

		g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);
	}
}

void CDlgZHeight::OnBnClickedZheightReadAxispos()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		if (IDYES == AfxMessageBox("是否要更新当前点位!", MB_YESNO))
		{
			g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, 0);
			RefreshAxisPos();
		}
	}
}

void CDlgZHeight::OnBnClickedZheightMove()
{
	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,m_pCmdLine->pDblValue[0],m_pCmdLine->pDblValue[1],g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
//	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,m_pCmdLine->pDblValue[2], g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);
}

void CDlgZHeight::OnBnClickedZheightSave()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{	
		double dx = m_pCmdLine->pDblValue[0];
		double dy = m_pCmdLine->pDblValue[1];

		if (0 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		{
			double doffsetCameraLaserX = g_pFrm->m_pRobotParam->m_posLaserCross.x - g_pFrm->m_pRobotParam->m_posCamereCross.x;
			double doffsetCameraLaserY = g_pFrm->m_pRobotParam->m_posLaserCross.y - g_pFrm->m_pRobotParam->m_posCamereCross.y;
			dx += doffsetCameraLaserX;
			dy += doffsetCameraLaserY;

			g_pFrm->m_Robot->GotoSafeZpos();
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,dx,dy,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			CFunction::DelaySec(0.1);

			double dValue = 0;
			if (!g_pFrm->m_comLaser.ReadHeightData(dValue))
			{
				AfxMessageBox("读取激光测高数值出错!");
				return;
			}
			CString strTemp;
			strTemp.Format("%.3f", dValue);
			SetDlgItemText(IDC_ZHEIGHT_EDIT_M1, strTemp);

			if (fabs(dValue)>34 || fabs(dValue)<26)
			{
				strTemp.Format("激光测高读数:[%.3f],此数值不可用，应在26~34之间，请重新选择测高点位", dValue);
				AfxMessageBox(strTemp);
				return;
			}
		}
		else if (1 == g_pFrm->m_pRobotParam->m_structHeightRead.m_nHtReadMode)
		{
			double doffsetCameraProbeX = g_pFrm->m_pRobotParam->m_posProbeCross.x - g_pFrm->m_pRobotParam->m_posCamereCross.x;
			double doffsetCameraProbeY = g_pFrm->m_pRobotParam->m_posProbeCross.y - g_pFrm->m_pRobotParam->m_posCamereCross.y;
			dx += doffsetCameraProbeX;
			dy += doffsetCameraProbeY;

			g_pFrm->m_Robot->GotoSafeZpos();
			g_pFrm->m_Precision.LineMoveXY(CRD1,0,dx,dy,g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
			g_pFrm->m_Robot->ExOutput(_T("探针气缸电磁阀"), TRUE);
			CFunction::DelaySec(1);

			double dPos = g_pFrm->m_pRobotParam->m_zContactPos-5;	// 探针测高极限位置
			double dValue = 0;
			short rtn = g_pFrm->m_Robot->m_pController->AxisProbeCheck(Z_AXIS, dPos, 5, 10, &dValue);
			if (RTN_NONE != rtn)
			{
				AfxMessageBox("读取探针测高数值出错!");
				return;
			}
			CString strTemp;
			strTemp.Format("%.3f", dValue);
			SetDlgItemText(IDC_ZHEIGHT_EDIT_M1, strTemp);
			g_pFrm->m_Robot->ExOutput(_T("探针气缸电磁阀"), FALSE);

			rtn = g_pFrm->m_Robot->GotoSafeZpos();
// 			if (fabs(dValue)>34 || fabs(dValue)<26)
// 			{
// 				strTemp.Format("激光测高读数:[%.3f],此数值不可用，应在26~34之间，请重新选择测高点位", dValue);
// 				AfxMessageBox(strTemp);
// 				return;
// 			}
		}
		//m_pCmdLine->pDblValue[4] = dValue;
	}
}

void CDlgZHeight::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}