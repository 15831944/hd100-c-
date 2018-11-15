// DlgGotoCall.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgGoSub.h"

#include "MainFrm.h"
// CDlgGoSub dialog

IMPLEMENT_DYNAMIC(CDlgGoSub, CDialog)

CDlgGoSub::CDlgGoSub(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGoSub::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgGoSub::~CDlgGoSub()
{

}

void CDlgGoSub::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgGoSub, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgGoSub::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDlgGoSub::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDC_BTN_READ_AXISPOS, &CDlgGoSub::OnBnClickedButtonReadAxispos)
	ON_BN_CLICKED(IDC_GOSUB_RADIO1, &CDlgGoSub::OnBnClickedGosubRadio1)
	ON_BN_CLICKED(IDC_GOSUB_RADIO2, &CDlgGoSub::OnBnClickedGosubRadio2)
END_MESSAGE_MAP()


// CDlgGoSub message handlers

BOOL CDlgGoSub::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCallSub,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCallSub,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCallSub,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCallSub,"Tag_3"));

	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCallSub,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCallSub,"Btn_2"));
	GetDlgItem(IDC_BTN_READ_AXISPOS)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCallSub,"Btn_3"));
	GetDlgItem(IDC_BTN_MOVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCallSub,"Btn_4"));
	GetDlgItem(IDC_GOSUB_RADIO1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCallSub,"Btn_5"));
	GetDlgItem(IDC_GOSUB_RADIO2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCallSub,"Btn_6"));
  
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("行号范围: 1~%d", g_pFrm->m_wndEditBar.m_rawList.GetCount());
		SetDlgItemText(IDC_GOSUB_EDIT_TYPE, szCmdline[m_pCmdLine->tType]);
		CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_GOSUB_COMBO_LABEL);

		////////////////////////
		int index=0;
		for(int i=0;i<g_pFrm->m_wndEditBar.m_rawList.GetCount();i++)
		{
			if(g_pFrm->m_wndEditBar.m_rawList.GetAt(g_pFrm->m_wndEditBar.m_rawList.FindIndex(i)).tType == CMD_LABEL)
			{
				pBox->InsertString(index,g_pFrm->m_wndEditBar.m_rawList.GetAt(g_pFrm->m_wndEditBar.m_rawList.FindIndex(i)).pszNote);
				index++;
			}
		}
		CString str;
		for(int i=0;i<pBox->GetCount();i++)
		{
			pBox->GetLBText(i,str); 
				pBox->SetCurSel(i);
		}
		/////////////////////
		strTemp.Format("%d", m_pCmdLine->pIntValue[1]);
		SetDlgItemText(IDC_GOSUB_EDIT_START_CMD, strTemp);

		////////////////////////////////////
		CWnd::CheckRadioButton(IDC_GOSUB_RADIO1, IDC_GOSUB_RADIO2, m_pCmdLine->pIntValue[0]==0 ? IDC_GOSUB_RADIO1:IDC_GOSUB_RADIO2);
		if(m_pCmdLine->pIntValue[0]==0)//地址
		{
			GetDlgItem(Tag_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_GOSUB_COMBO_LABEL)->ShowWindow(SW_HIDE);
			GetDlgItem(Tag_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_GOSUB_EDIT_START_CMD)->ShowWindow(SW_SHOW);
		}
		else//标号
		{
			GetDlgItem(Tag_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_GOSUB_EDIT_START_CMD)->ShowWindow(SW_HIDE);
			GetDlgItem(Tag_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_GOSUB_COMBO_LABEL)->ShowWindow(SW_SHOW);
		}
		///////////////////
		// X微调
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[4]);
		SetDlgItemText(IDC_GOSUB_XDIRDISOFFSET, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[5]);
		SetDlgItemText(IDC_GOSUB_YDIRDISOFFSET, strTemp);
		RefreshAxisPos();

		/*
		for(int i=0; i<AXISNUM; i++)
		{
			strTemp.Format("%.3f", m_pCmdLine->pDblValue[i]);
			SetDlgItemText(IDC_GOSUB_EDIT1+i, strTemp);
		}*/
		///////////////////
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGoSub::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		m_pCmdLine->pIntValue[0] = CWnd::GetCheckedRadioButton(IDC_GOSUB_RADIO1,IDC_GOSUB_RADIO2)-IDC_GOSUB_RADIO1;
		if(m_pCmdLine->pIntValue[0]==1)//选择标号
		{
			int iSel=((CComboBox*)GetDlgItem(IDC_GOSUB_COMBO_LABEL))->GetCurSel();
			if(iSel == -1)
			{
				OnOK();
				return ;
			}
			((CComboBox*)GetDlgItem(IDC_GOSUB_COMBO_LABEL))->GetLBText(iSel,m_pCmdLine->pszNote);


			m_pCmdLine->pIntValue[1] = 0;//默认
		}
		else
		{
			GetDlgItemText(IDC_GOSUB_EDIT_START_CMD,strTemp);
			m_pCmdLine->pIntValue[1] =atoi(strTemp);
			strcpy_s(m_pCmdLine->pszNote, "");
		}

		for(int j=0;j<3;j++)
		{
			GetDlgItemText(IDC_GOSUB_EDIT1+j, strTemp);
			m_pCmdLine->pDblValue[j] = atof(strTemp);
		}
		GetDlgItemText(IDC_GOSUB_XDIRDISOFFSET, strTemp);
		m_pCmdLine->pDblValue[4] = atof(strTemp);

		GetDlgItemText(IDC_GOSUB_YDIRDISOFFSET, strTemp);
		m_pCmdLine->pDblValue[5] = atof(strTemp);
	}
	

	OnOK();
}
void CDlgGoSub::OnBnClickedButtonMove()
{
	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,m_pCmdLine->pDblValue[0],m_pCmdLine->pDblValue[1],g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,m_pCmdLine->pDblValue[2], g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

}

void CDlgGoSub::OnBnClickedButtonReadAxispos()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, 0);
		RefreshAxisPos();
	}
}

void CDlgGoSub::RefreshAxisPos(void)
{
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_GOSUB_EDIT1, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_GOSUB_EDIT2, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_GOSUB_EDIT3, strTemp);

	}
}

void CDlgGoSub::OnBnClickedGosubRadio1()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine!=NULL)
	{
		m_pCmdLine->pIntValue[0]=0;
		GetDlgItem(Tag_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GOSUB_COMBO_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(Tag_2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_GOSUB_EDIT_START_CMD)->ShowWindow(SW_SHOW);

		CString str;
		str.Format("行号：0~%d",g_pFrm->m_wndEditBar.m_wndList.GetItemCount());//行号
		SetDlgItemText(Tag_2,str);

		if(m_pCmdLine->pIntValue[1]>=g_pFrm->m_wndEditBar.m_wndList.GetItemCount())
		{
			m_pCmdLine->pIntValue[1]=0;
		}

		str.Format("%d",m_pCmdLine->pIntValue[1]);
		SetDlgItemText(IDC_GOSUB_EDIT_START_CMD,str);
	}
}

void CDlgGoSub::OnBnClickedGosubRadio2()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		CString str;
		CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_GOSUB_COMBO_LABEL);
		pBox->SetCurSel(0);
		int index=0;
		for(int i=0;i<g_pFrm->m_wndEditBar.m_wndList.GetItemCount();i++)
		{
			if(g_pFrm->m_wndEditBar.m_rawList.GetAt(g_pFrm->m_wndEditBar.m_rawList.FindIndex(i)).tType == CMD_LABEL)
			{
				index++;
			}
		}
		for(int i=0;i<pBox->GetCount();i++)
		{
			pBox->GetLBText(i,str);
			if(str == m_pCmdLine->pszNote)
				pBox->SetCurSel(i);
		}
		GetDlgItem(Tag_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GOSUB_EDIT_START_CMD)->ShowWindow(SW_HIDE);
		GetDlgItem(Tag_3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_GOSUB_COMBO_LABEL)->ShowWindow(SW_SHOW);
	}
}
