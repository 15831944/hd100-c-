// DlgRepeatZheight.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgRepeatZheight.h"


// DlgRepeatZheight dialog

IMPLEMENT_DYNAMIC(CDlgRepeatZheight, CDialog)

CDlgRepeatZheight::CDlgRepeatZheight(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRepeatZheight::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgRepeatZheight::~CDlgRepeatZheight()
{
}

void CDlgRepeatZheight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRepeatZheight, CDialog)
	ON_BN_CLICKED(IDC_REPEAT_ZHEIGHT_APPLY, &CDlgRepeatZheight::OnBnClickedRepeatZheightApply)
	ON_BN_CLICKED(IDC_REPEAT_ZHEIGHT_SAVE, &CDlgRepeatZheight::OnBnClickedRepeatZheightSave)
	ON_BN_CLICKED(IDC_REPEAT_ZHEIGHT_MOVE, &CDlgRepeatZheight::OnBnClickedRepeatZheightMove)
	ON_BN_CLICKED(IDC_REPEAT_ZHEIGHT_READ_AXISPOS, &CDlgRepeatZheight::OnBnClickedRepeatZheightReadAxispos)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CDlgRepeatZheight::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgRepeatZheight message handlers
BOOL CDlgRepeatZheight::OnInitDialog()
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

void CDlgRepeatZheight::OnBnClickedRepeatZheightApply()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		GetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_X, strTemp);
		m_pCmdLine->pDblValue[0] =atof(strTemp);
		GetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_Y, strTemp);
		m_pCmdLine->pDblValue[1] =atof(strTemp);
		GetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_Z, strTemp);
		m_pCmdLine->pDblValue[2] =atof(strTemp);

		GetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_B1, strTemp);
		m_pCmdLine->pDblValue[4] =atof(strTemp);
		GetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_T2, strTemp);
		m_pCmdLine->pDblValue[5] =atof(strTemp);

		///////////
		GetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_XSPACE, strTemp);
		m_pCmdLine->pDblValue[6] = atof(strTemp);//X间距

		GetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_YSPACE, strTemp);
		m_pCmdLine->pDblValue[7] = atof(strTemp);//Y间距

		//m_pCmdLine->pIntValue[0] = GetDlgItemInt(IDC_REPEAT_ZHEIGHT_EDIT_INDEX);
		m_pCmdLine->pIntValue[1] = CWnd::GetCheckedRadioButton(IDC_REPEAT_ZHEIGHT_RADIO1, IDC_REPEAT_ZHEIGHT_RADIO2)-IDC_REPEAT_ZHEIGHT_RADIO1;
		m_pCmdLine->pIntValue[2] = GetDlgItemInt(IDC_REPEAT_ZHEIGHT_EDIT_XNUM);
		m_pCmdLine->pIntValue[3] = GetDlgItemInt(IDC_REPEAT_ZHEIGHT_EDIT_YNUM);

		g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);
	}
}

void CDlgRepeatZheight::OnBnClickedRepeatZheightSave()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		GetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_M1,strTemp);	
		SetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_B1, strTemp);

		GetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_B1, strTemp);
		m_pCmdLine->pDblValue[4] =atof(strTemp);
	}
}

void CDlgRepeatZheight::OnBnClickedRepeatZheightMove()
{
	// TODO: Add your control notification handler code here
	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,m_pCmdLine->pDblValue[0],m_pCmdLine->pDblValue[1],g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,m_pCmdLine->pDblValue[2], g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

}

void CDlgRepeatZheight::OnBnClickedRepeatZheightReadAxispos()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, 0);
		RefreshAxisPos();
	}
}
void CDlgRepeatZheight::RefreshAxisPos(void)
{
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_X, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_Y, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_Z, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[4]);//基位高度
		SetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_B1, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[5]);//延时
		SetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_T2, strTemp);

		////////

		SetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_TYPE, szCmdline[m_pCmdLine->tType]);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[6]);
		SetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_XSPACE, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[7]);
		SetDlgItemText(IDC_REPEAT_ZHEIGHT_EDIT_YSPACE, strTemp);

		//SetDlgItemInt(IDC_REPEAT_EDIT_INDEX, m_pCmdLine->pIntValue[0]);
		CWnd::CheckRadioButton(IDC_REPEAT_ZHEIGHT_RADIO1, IDC_REPEAT_ZHEIGHT_RADIO2, IDC_REPEAT_ZHEIGHT_RADIO1 +m_pCmdLine->pIntValue[1]);

		SetDlgItemInt(IDC_REPEAT_ZHEIGHT_EDIT_XNUM, m_pCmdLine->pIntValue[2]);
		SetDlgItemInt(IDC_REPEAT_ZHEIGHT_EDIT_YNUM, m_pCmdLine->pIntValue[3]);
	}
}

void CDlgRepeatZheight::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	OnBnClickedRepeatZheightApply();
	CDialog::OnOK();
}

void CDlgRepeatZheight::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if(m_pCmdLine != NULL)
	{
		RefreshAxisPos();
	}
}

void CDlgRepeatZheight::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
