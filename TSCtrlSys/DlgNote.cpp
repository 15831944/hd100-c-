// DlgNote.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgNote.h"


// CDlgNote dialog

IMPLEMENT_DYNAMIC(CDlgNote, CDialog)

CDlgNote::CDlgNote(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNote::IDD, pParent)
{

}
CDlgNote::CDlgNote(CString strText,CWnd* pParent /*=NULL*/)
: CDialog(CDlgNote::IDD, pParent)
{
	m_strText = strText;


}

CDlgNote::~CDlgNote()
{
}

void CDlgNote::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNote, CDialog)
	ON_BN_CLICKED(IDC_BTN_READ_AXISPOS, &CDlgNote::OnBnClickedBtnReadAxispos)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDlgNote::OnBnClickedBtnMove)
END_MESSAGE_MAP()


// CDlgNote message handlers

void CDlgNote::OnOK()
{
	// TODO: Add your specialized	code here and/or call the base class
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		GetDlgItemText(IDC_EDIT_NOTE, strTemp);
		if(strTemp.GetLength()>64)
		{
			strTemp = strTemp.Mid(0, 64);
		}
		strcpy_s(m_pCmdLine->pszNote, (LPSTR)(LPCSTR)strTemp);

		GetDlgItemText(IDC_AXIS_HPOS_EDIT_X, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_HPOS_EDIT_Y, strTemp);
		m_pCmdLine->pDblValue[1] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_HPOS_EDIT_Z, strTemp);
		m_pCmdLine->pDblValue[2] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_HPOS_EDIT_PREDOT, strTemp);
		m_pCmdLine->pDblValue[4] = atof(strTemp);
	}

	g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);
	CDialog::OnOK();
}

BOOL CDlgNote::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if(m_pCmdLine != NULL)
	{
		SetDlgItemText(IDC_EDIT_NOTE, m_pCmdLine->pszNote);
	}
	SetWindowText(m_strText);

	RefreshAxisPos();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNote::OnBnClickedBtnReadAxispos()
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

void CDlgNote::RefreshAxisPos(void)
{
	if(m_pCmdLine != NULL && CMD_LABEL == m_pCmdLine->tType)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_X, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_Y, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_AXIS_HPOS_EDIT_Z, strTemp);
	}
	else
	{
		GetDlgItem(IDC_AXIS_HPOS_EDIT_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_AXIS_HPOS_EDIT_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_AXIS_HPOS_EDIT_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_READ_AXISPOS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MOVE)->EnableWindow(FALSE);
	}
}

void CDlgNote::OnBnClickedBtnMove()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,m_pCmdLine->pDblValue[0],m_pCmdLine->pDblValue[1],
		g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
}
