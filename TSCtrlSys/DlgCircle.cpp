// DlgCircle.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgCircle.h"


// CDlgCircle dialog

IMPLEMENT_DYNAMIC(CDlgCircle, CDialog)

CDlgCircle::CDlgCircle(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCircle::IDD, pParent)
{
	m_pCmdLine = NULL;
	disbool=false;
}
CDlgCircle::~CDlgCircle()
{

}
void CDlgCircle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CDlgCircle, CDialog)
	ON_BN_CLICKED(IDC_CIRCLE_RADIO1, &CDlgCircle::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_CIRCLE_RADIO2, &CDlgCircle::OnBnClickedRadio2)
	ON_CBN_SELENDOK(IDC_CIRCLE_COMBO_ITEM, &CDlgCircle::OnCbnSelendokComboItem)
	ON_BN_CLICKED(IDC_BTN_READ_AXISPOS, &CDlgCircle::OnBnClickedButtonReadAxispos)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgCircle::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDlgCircle::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDOK, &CDlgCircle::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CIRCLE_CHECK_DISPENSE_ONOFF, &CDlgCircle::OnBnClickedCircleCheckDispenseOnoff)
	ON_CBN_SELCHANGE(IDC_CIRCLE_RADIO_HEAD, &CDlgCircle::OnCbnSelchangeCircleRadioHead)
END_MESSAGE_MAP()


// CDlgCircle message handlers
BOOL CDlgCircle::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Tag_3"));
	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Tag_4"));
	
	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Btn_2"));
	GetDlgItem(IDC_BTN_APPLY)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Btn_3"));
	GetDlgItem(IDC_BTN_READ_AXISPOS)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Btn_4"));
	GetDlgItem(IDC_BTN_MOVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Btn_5"));
	GetDlgItem(IDC_CIRCLE_RADIO1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Btn_6"));
	GetDlgItem(IDC_CIRCLE_RADIO2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Btn_7"));
	GetDlgItem(IDC_CIRCLE_CHECK_DISPENSE_ONOFF)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Btn_8"));

	if(m_pCmdLine != NULL)
	{
		CWnd::CheckRadioButton(IDC_CIRCLE_RADIO1, IDC_CIRCLE_RADIO2, IDC_CIRCLE_RADIO1 + m_pCmdLine->pIntValue[4]);
		if( m_pCmdLine->pIntValue[4]==0)
			OnBnClickedRadio1();
		else
			OnBnClickedRadio2();
	}

	/////
	CString str;
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_CIRCLE_RADIO_HEAD);
	for (int i=0;i<TOOL_NUM;i++)
	{
		str.Empty();
		str.Format("#%d",i);
		pBox->AddString(str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgCircle::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	OnBnClickedButtonApply();
	CDialog::OnOK();
}
void CDlgCircle::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	CString  str;

	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_CIRCLE_COMBO_ITEM);
	pBox->ResetContent();
	str.Format("%s",g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Msg_1"));
	pBox->AddString(str);
	pBox->SetCurSel(0);
	m_pCmdLine->pIntValue[4]=0;
	OnCbnSelendokComboItem();
	GetDlgItem(IDC_CIRCLE_EDIT_DIAMETER)->EnableWindow(TRUE);

	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[16]);//直径m_pCmdLine->pDblValue[4] --->m_pCmdLine->pDblValue[12]
		SetDlgItemText(IDC_CIRCLE_EDIT_DIAMETER, strTemp);

		if( m_pCmdLine->pDblValue[17]>0.000001)
		{
			strTemp.Format("%.3f", m_pCmdLine->pDblValue[17]);//超越角度m_pCmdLine->pDblValue[12] --->m_pCmdLine->pDblValue[13]
		}
		else
		{
			strTemp.Format("0.000");
		}
		SetDlgItemText(IDC_CIRCLE_EDIT_OVERANGLE, strTemp);
	}
}
void CDlgCircle::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_CIRCLE_COMBO_ITEM);
	CString strTemp;
	pBox->ResetContent();
	strTemp.Format("%s",g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Msg_2"));
	pBox->AddString(strTemp);
	strTemp.Format("%s",g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Msg_3"));
	pBox->AddString(strTemp);
	strTemp.Format("%s",g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDCIRC,"Msg_4"));
	pBox->AddString(strTemp);
	pBox->SetCurSel(0);
	OnCbnSelendokComboItem();
	m_pCmdLine->pIntValue[4]=1;//三点方式

	if(m_pCmdLine != NULL)
	{
		if( m_pCmdLine->pDblValue[17]>0.000001)
		{
			strTemp.Format("%.3f", m_pCmdLine->pDblValue[17]);//超越角度m_pCmdLine->pDblValue[12] --->m_pCmdLine->pDblValue[13]
		}
		else
		{
			strTemp.Format("0.000");
		}
		SetDlgItemText(IDC_CIRCLE_EDIT_OVERANGLE, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[3]);
		SetDlgItemText(IDC_CIRCLE_EDIT_WEIGHT, strTemp);
	}

	GetDlgItem(IDC_CIRCLE_EDIT_DIAMETER)->EnableWindow(FALSE);
}

void CDlgCircle::OnCbnSelendokComboItem()
{
	// TODO: Add your control notification handler code here

	if(m_pCmdLine != NULL)
	{
		CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_CIRCLE_COMBO_ITEM);
		CString strTemp;
		int n= pBox->GetCurSel();

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4 +3]);
		SetDlgItemText(IDC_CIRCLE_EDIT_WEIGHT, strTemp);

		m_pCmdLine->pIntValue[4] = CWnd::GetCheckedRadioButton(IDC_CIRCLE_RADIO1, IDC_CIRCLE_RADIO2)-IDC_CIRCLE_RADIO1;
	
		if (m_pCmdLine->pIntValue[4]==1)
		{
			n+=1;
			strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4 +0]);
			SetDlgItemText(IDC_CIRCLE_EDIT_X, strTemp);

			strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4 +1]);
			SetDlgItemText(IDC_CIRCLE_EDIT_Y, strTemp);

			strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4 +2]);
			SetDlgItemText(IDC_CIRCLE_EDIT_Z, strTemp);
		}
		else
		{
			strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4 +0]);
			SetDlgItemText(IDC_CIRCLE_EDIT_X, strTemp);

			strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4 +1]);
			SetDlgItemText(IDC_CIRCLE_EDIT_Y, strTemp);

			strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4 +2]);
			SetDlgItemText(IDC_CIRCLE_EDIT_Z, strTemp);
		}

	}
}
void CDlgCircle::OnBnClickedButtonReadAxispos()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_CIRCLE_COMBO_ITEM);
		int n = pBox->GetCurSel();

		m_pCmdLine->pIntValue[4] = CWnd::GetCheckedRadioButton(IDC_CIRCLE_RADIO1, IDC_CIRCLE_RADIO2)-IDC_CIRCLE_RADIO1;

		if (m_pCmdLine->pIntValue[4]==1)
		{
			g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, (n+1)*4);
		}
		else
		{
			g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, n*4);
		}
		
		OnCbnSelendokComboItem();
	}
}
void CDlgCircle::OnBnClickedButtonApply()
{//需要在这里计算出圆心和半径
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_CIRCLE_COMBO_ITEM);
		int n = pBox->GetCurSel();

		GetDlgItemText(IDC_CIRCLE_EDIT_WEIGHT, strTemp);
		m_pCmdLine->pDblValue[n*4+3] =atof(strTemp);

		m_pCmdLine->pIntValue[4] = CWnd::GetCheckedRadioButton(IDC_CIRCLE_RADIO1, IDC_CIRCLE_RADIO2)-IDC_CIRCLE_RADIO1;

		if (m_pCmdLine->pIntValue[4]==1)
		{
			n+=1;
			GetDlgItemText(IDC_CIRCLE_EDIT_X, strTemp);
			m_pCmdLine->pDblValue[n*4+0] =atof(strTemp);
			GetDlgItemText(IDC_CIRCLE_EDIT_Y, strTemp);
			m_pCmdLine->pDblValue[n*4+1] =atof(strTemp);
			GetDlgItemText(IDC_CIRCLE_EDIT_Z, strTemp);
			m_pCmdLine->pDblValue[n*4+2] =atof(strTemp);

			CaliCenter();
			strTemp.Format("%.3f",m_pCmdLine->pDblValue[16]);
			SetDlgItemText(IDC_CIRCLE_EDIT_DIAMETER, strTemp);
		}
		else
		{
			GetDlgItemText(IDC_CIRCLE_EDIT_X, strTemp);
			m_pCmdLine->pDblValue[n*4+0] =atof(strTemp);
			GetDlgItemText(IDC_CIRCLE_EDIT_Y, strTemp);
			m_pCmdLine->pDblValue[n*4+1] =atof(strTemp);
			GetDlgItemText(IDC_CIRCLE_EDIT_Z, strTemp);
			m_pCmdLine->pDblValue[n*4+2] =atof(strTemp);

			GetDlgItemText(IDC_CIRCLE_EDIT_DIAMETER, strTemp);
			m_pCmdLine->pDblValue[16] =atof(strTemp);
		}

		GetDlgItemText(IDC_CIRCLE_EDIT_OVERANGLE, strTemp);
		m_pCmdLine->pDblValue[17] =atof(strTemp);

		CComboBox *pBox1 = (CComboBox*)GetDlgItem(IDC_CIRCLE_RADIO_HEAD);
		m_pCmdLine->pIntValue[0] = pBox1->GetCurSel();
		m_pCmdLine->pIntValue[1] = ((CButton*)GetDlgItem(IDC_CIRCLE_CHECK_DISPENSE_ONOFF))->GetCheck();

		g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);
	}
}
void CDlgCircle::OnBnClickedButtonMove()
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_CIRCLE_COMBO_ITEM);
	int n = pBox->GetCurSel()*4;

	g_pFrm->m_Robot->GotoSafeZpos();
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,m_pCmdLine->pDblValue[n+0],m_pCmdLine->pDblValue[n+1],g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,m_pCmdLine->pDblValue[n+2], g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

}
void CDlgCircle::RefreshAxisPos(void)
{
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
        // 设置园类型 
		CWnd::CheckRadioButton(IDC_CIRCLE_RADIO1, IDC_CIRCLE_RADIO2, IDC_CIRCLE_RADIO1 + m_pCmdLine->pIntValue[4]);
		if( m_pCmdLine->pIntValue[4]==0)  // IntValue 0-圆心+半径 1-3点
			OnBnClickedRadio1();
		else
			OnBnClickedRadio2();

		CComboBox *pBox1 = (CComboBox*)GetDlgItem(IDC_CIRCLE_RADIO_HEAD);
		pBox1->SetCurSel(m_pCmdLine->pIntValue[0]);
		((CButton*)GetDlgItem(IDC_CIRCLE_CHECK_DISPENSE_ONOFF))->SetCheck(m_pCmdLine->pIntValue[1]);
	}
}

void CDlgCircle::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnBnClickedButtonApply();
	OnOK();
}

void CDlgCircle::CaliCenter()
{//仅限三点方式计算
	//计算圆心和直径
	double dXc, dYc, dRc;

	double posArrayX[3];
	double posArrayY[3];
	for(int i=0; i<3; i++)
	{
		posArrayX[i] = m_pCmdLine->pDblValue[(i+1)*4 + 0];
		posArrayY[i] = m_pCmdLine->pDblValue[(i+1)*4 + 1];
	}

	g_pFrm->m_wndEditBar.GetCircle(posArrayX,posArrayY,&dXc, &dYc, &dRc);
	m_pCmdLine->pDblValue[0] = dXc;
	m_pCmdLine->pDblValue[1] = dYc;
	m_pCmdLine->pDblValue[16] = dRc;

}
void CDlgCircle::OnBnClickedCircleCheckDispenseOnoff()
{
	// TODO: Add your control notification handler code here
}

void CDlgCircle::OnCbnSelchangeCircleRadioHead()
{
	// TODO: Add your control notification handler code here
}
