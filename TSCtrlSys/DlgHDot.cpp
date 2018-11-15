// DlgHDot.cpp : 实现文件
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgHDot.h"
#include "afxdialogex.h"


// DlgHDot 对话框

IMPLEMENT_DYNAMIC(CDlgHDot, CDialogEx)

CDlgHDot::CDlgHDot(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgHDot::IDD, pParent)
{

}

CDlgHDot::~CDlgHDot()
{
}

void CDlgHDot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHDot, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgHDot::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_READ_AXISPOS, &CDlgHDot::OnBnClickedBtnReadAxispos)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDlgHDot::OnBnClickedBtnMove)
END_MESSAGE_MAP()


// DlgHDot 消息处理程序


void CDlgHDot::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	if(m_pCmdLine != NULL)
	{
		GetDlgItemText(IDC_AXIS_HPOS_EDIT_X, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_HPOS_EDIT_Y, strTemp);
		m_pCmdLine->pDblValue[1] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_HPOS_EDIT_Z, strTemp);
		m_pCmdLine->pDblValue[2] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_HPOS_EDIT_Z2, strTemp);
		m_pCmdLine->pDblValue[3] = atof(strTemp);
	}

	g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);

	CDialogEx::OnOK();
}


void CDlgHDot::OnBnClickedBtnReadAxispos()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pCmdLine != NULL)
	{
		if (IDYES == AfxMessageBox("是否要更新当前点位!", MB_YESNO))
		{
			g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, 0);
			RefreshAxisPos();
		}
	}
}


void CDlgHDot::OnBnClickedBtnMove()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->GotoSafeZpos();

	if(m_pCmdLine != NULL)
	{
		g_pFrm->m_Precision.LineMoveXY(CRD1,0,m_pCmdLine->pDblValue[0],m_pCmdLine->pDblValue[1],
			g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	}
}


BOOL CDlgHDot::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	RefreshAxisPos();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgHDot::RefreshAxisPos(void)
{
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_X, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_Y, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_Z, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[3]);
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_Z2, strTemp);
	}
}