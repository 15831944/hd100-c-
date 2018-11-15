// DlgHDot.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgHDot.h"
#include "afxdialogex.h"


// DlgHDot �Ի���

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


// DlgHDot ��Ϣ�������


void CDlgHDot::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pCmdLine != NULL)
	{
		if (IDYES == AfxMessageBox("�Ƿ�Ҫ���µ�ǰ��λ!", MB_YESNO))
		{
			g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, 0);
			RefreshAxisPos();
		}
	}
}


void CDlgHDot::OnBnClickedBtnMove()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	RefreshAxisPos();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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