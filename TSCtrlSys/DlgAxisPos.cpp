// DlgAxisPosModified.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgAxisPos.h"

// CDlgAxisPos dialog
IMPLEMENT_DYNAMIC(CDlgAxisPos, CDialog)

CDlgAxisPos::CDlgAxisPos(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAxisPos::IDD, pParent)
{
	m_pCmdLine = NULL;
}

CDlgAxisPos::~CDlgAxisPos()
{

}

void CDlgAxisPos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CDlgAxisPos, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAxisPos::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_READ_AXISPOS, &CDlgAxisPos::OnBnClickedButtonReadAxispos)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDlgAxisPos::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDC_AXIS_POS_CHECK_VEL, &CDlgAxisPos::OnBnClickedCheckVel)
	ON_BN_CLICKED(IDC_AXIS_POS_CHECK_DELAY, &CDlgAxisPos::OnBnClickedCheckDelay)
	ON_CBN_SELCHANGE(IDC_AXIS_POS_COMBO_CMD_TYPE, &CDlgAxisPos::OnCbnSelchangeComboCmdType)
	ON_CBN_SELCHANGE(IDC_AXIS_POS_RADIO_HEAD, &CDlgAxisPos::OnCbnSelchangeRadioHead)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF, &CDlgAxisPos::OnBnClickedAxisPosCheckDispenseOnoff)
	ON_EN_CHANGE(IDC_AXIS_POS_EDIT_X, &CDlgAxisPos::OnEnChangeAxisPosEditX)
	ON_BN_CLICKED(IDCANCEL, &CDlgAxisPos::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgAxisPos message handlers
BOOL CDlgAxisPos::OnInitDialog()
{//非模态在Create的时候调用此函数
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Tag_3"));
	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Tag_4"));
	GetDlgItem(Tag_5)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Tag_5"));

	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_2"));
	GetDlgItem(IDC_BTN_READ_AXISPOS)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_3"));
	GetDlgItem(IDC_BTN_MOVE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_4"));
	GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_5"));
	GetDlgItem(IDC_AXIS_POS_CHECK_VEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_6"));
	GetDlgItem(IDC_AXIS_POS_CHECK_DELAY)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_6"));
	GetDlgItem(IDC_AXIS_POS_CHECK_ANGLE)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDAxis,"Btn_7"));


	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_AXIS_POS_COMBO_CMD_TYPE);
	pBox->AddString(szCmdline[CMD_DUMMY]);
	pBox->AddString(szCmdline[CMD_POINT]);
	pBox->AddString(szCmdline[CMD_LINE_START]);
	pBox->AddString(szCmdline[CMD_LINE_PASSING]);
	pBox->AddString(szCmdline[CMD_LINE_ARC]);
	pBox->AddString(szCmdline[CMD_LINE_END]);


	CComboBox *pBox1 = (CComboBox*)GetDlgItem(IDC_AXIS_POS_RADIO_HEAD);
	CString str;
	for (int i=0;i<TOOL_NUM;i++)
	{
		str.Format("#%d",i);
		pBox1->AddString(str);
	}
    
	RefreshAxisPos();

	CRect rc;
	GetWindowRect(rc);
//	MoveWindow(rc.left, rc.top, rc.Width(), rc.Height()-100);
   
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgAxisPos::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_AXIS_POS_COMBO_CMD_TYPE);
		CComboBox *pBox1 = (CComboBox*)GetDlgItem(IDC_AXIS_POS_RADIO_HEAD);

		GetDlgItemText(IDC_AXIS_POS_EDIT_X, strTemp);
		m_pCmdLine->pDblValue[0] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_POS_EDIT_Y, strTemp);
		m_pCmdLine->pDblValue[1] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_POS_EDIT_Z, strTemp);
		m_pCmdLine->pDblValue[2] = atof(strTemp);

		GetDlgItemText(IDC_AXIS_POS_EDIT_WEIGHT, strTemp);
		m_pCmdLine->pDblValue[3] = atof(strTemp);
		

		switch(m_pCmdLine->tType)
		{
		case CMD_DUMMY:
		case CMD_POINT:
		case CMD_LINE_START:
		case CMD_LINE_PASSING:
		case CMD_LINE_ARC:
		case CMD_LINE_END:
			m_pCmdLine->pIntValue[4] = ((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_VEL))->GetCheck();
			if(0 != m_pCmdLine->pIntValue[4])
			{
				GetDlgItemText(IDC_AXIS_POS_EDIT_VEL, strTemp);
				m_pCmdLine->pDblValue[4] = atof(strTemp);
			}
			else
				m_pCmdLine->pDblValue[4] = 0.0;

			m_pCmdLine->pIntValue[5] = ((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_DELAY))->GetCheck();
			if(0 != m_pCmdLine->pIntValue[5])
			{	
				GetDlgItemText(IDC_AXIS_POS_EDIT_WAIT_TIME, strTemp);
				m_pCmdLine->pDblValue[5] = atof(strTemp);		
			}
			else
				m_pCmdLine->pDblValue[5] = 0.0;
		
			if(m_pCmdLine->tType == CMD_LINE_PASSING)
			{
				CString str("");
				GetDlgItemText(IDC_AXIS_POS_EDIT_ANGLERADIUS,str);
				m_pCmdLine->pDblValue[6]=atof(str);
				m_pCmdLine->pIntValue[6]=((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_ANGLE))->GetCheck();
			}
			
			m_pCmdLine->pIntValue[0] = pBox1->GetCurSel();//阀ID

			if (m_pCmdLine->tType ==CMD_POINT || m_pCmdLine->tType ==CMD_LINE_START || m_pCmdLine->tType == CMD_LINE_END)
			{
				m_pCmdLine->pIntValue[1] = ((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF))->GetCheck();
			}
			break;

		default:break;
		}

		switch(m_pCmdLine->tType)
		{
		case CMD_DUMMY:
		case CMD_POINT:
		case CMD_LINE_START:
		case CMD_LINE_PASSING:
		case CMD_LINE_ARC:
		case CMD_LINE_END:
	        m_pCmdLine->tType = CMD_TYPE(pBox->GetCurSel() + CMD_DUMMY);

			if(m_pCmdLine->iIntNum>0)
				m_pCmdLine->pIntValue[1] = ((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF))->GetCheck();
			break;	
       
		default:break;
		}
	}

	g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);

	OnOK();
}

void CDlgAxisPos::OnBnClickedButtonReadAxispos()
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

void CDlgAxisPos::OnBnClickedButtonMove()
{
	// TODO: Add your control notification handler code here
	g_pFrm->m_Robot->GotoSafeZpos();

	g_pFrm->m_Precision.LineMoveXY(CRD1,0,m_pCmdLine->pDblValue[0],m_pCmdLine->pDblValue[1],g_pFrm->m_mtrParamGts[X_AXIS].m_VhandMove,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
//	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,m_pCmdLine->pDblValue[2], g_pFrm->m_mtrParam[Z_AXIS].m_Vmax,g_pFrm->m_mtrParam[Z_AXIS].m_Acc,TRUE);

}
void CDlgAxisPos::RefreshAxisPos(void)
{
	if(m_pCmdLine != NULL)
	{
		CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_AXIS_POS_COMBO_CMD_TYPE);
		pBox->SetCurSel(m_pCmdLine->tType - CMD_DUMMY);
		CComboBox *pBox1 = (CComboBox*)GetDlgItem(IDC_AXIS_POS_RADIO_HEAD);
		pBox1->SetCurSel(m_pCmdLine->pIntValue[0]);//阀ID

		CString strTemp;

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[0]);
		SetDlgItemText(IDC_AXIS_POS_EDIT_X, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[1]);
		SetDlgItemText(IDC_AXIS_POS_EDIT_Y, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[2]);
		SetDlgItemText(IDC_AXIS_POS_EDIT_Z, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[3]);
		SetDlgItemText(IDC_AXIS_POS_EDIT_WEIGHT, strTemp);


		//配置选项实际值
		switch(m_pCmdLine->tType)
		{
		case CMD_DUMMY:
			break;
		case CMD_LINE_ARC:
			EnableCheckVel(FALSE);
			EnableCheckDelay(FALSE);
			EnablePassAngle(FALSE);

			((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF))->SetCheck(m_pCmdLine->pIntValue[1]);

			break;
		case CMD_POINT:
		case CMD_LINE_START:
	        
			EnableCheckVel(FALSE);
			EnablePassAngle(FALSE);
			EnableCheckDelay(TRUE);

			((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_DELAY))->SetCheck(m_pCmdLine->pIntValue[5]);
			if(0 != m_pCmdLine->pIntValue[5])//临时延时命令有效
			{	
				strTemp.Format("%.3f", m_pCmdLine->pDblValue[5]);
				SetDlgItemText(IDC_AXIS_POS_EDIT_WAIT_TIME, strTemp);
				GetDlgItem(IDC_AXIS_POS_EDIT_WAIT_TIME)->EnableWindow(TRUE);
			}
			else
			{
				strTemp.Empty();
				SetDlgItemText(IDC_AXIS_POS_EDIT_WAIT_TIME, strTemp);
				GetDlgItem(IDC_AXIS_POS_EDIT_WAIT_TIME)->EnableWindow(FALSE);
			}
			
			//阀开关
			((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF))->SetCheck(m_pCmdLine->pIntValue[1]);


			break;
		case CMD_LINE_PASSING:
		case CMD_LINE_END:

			EnableCheckVel(TRUE);
			((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_VEL))->SetCheck(m_pCmdLine->pIntValue[4]);
			if(0 != m_pCmdLine->pIntValue[4])
			{
				strTemp.Format("%.3f", m_pCmdLine->pDblValue[4]);
				SetDlgItemText(IDC_AXIS_POS_EDIT_VEL, strTemp);
				GetDlgItem(IDC_AXIS_POS_EDIT_VEL)->EnableWindow(TRUE);
			}
			else
			{
				strTemp.Empty();
				SetDlgItemText(IDC_AXIS_POS_EDIT_VEL, strTemp);
				GetDlgItem(IDC_AXIS_POS_EDIT_VEL)->EnableWindow(FALSE);
			}

			EnableCheckDelay(TRUE);
			((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_DELAY))->SetCheck(m_pCmdLine->pIntValue[5]);
			if(0 != m_pCmdLine->pIntValue[5])
			{	
				strTemp.Format("%.3f", m_pCmdLine->pDblValue[5]);
				SetDlgItemText(IDC_AXIS_POS_EDIT_WAIT_TIME, strTemp);
				GetDlgItem(IDC_AXIS_POS_EDIT_WAIT_TIME)->EnableWindow(TRUE);
			}
			else
			{
				strTemp.Empty();
				SetDlgItemText(IDC_AXIS_POS_EDIT_WAIT_TIME, strTemp);
				GetDlgItem(IDC_AXIS_POS_EDIT_WAIT_TIME)->EnableWindow(FALSE);
			}
		
			if(m_pCmdLine->tType == CMD_LINE_PASSING)
			{
				((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF))->SetCheck(m_pCmdLine->pIntValue[1]);
				
				EnablePassAngle(TRUE);
				((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_ANGLE))->SetCheck(m_pCmdLine->pIntValue[6]);
				if(m_pCmdLine->pIntValue[6]!=0)
				{
					CString str("");
					str.Format(_T("%.3f"),m_pCmdLine->pDblValue[6]);
					SetDlgItemText(IDC_AXIS_POS_EDIT_ANGLERADIUS,str);
					GetDlgItem(IDC_AXIS_POS_EDIT_ANGLERADIUS)->EnableWindow(TRUE);
				}
				else
				{
					GetDlgItem(IDC_AXIS_POS_EDIT_ANGLERADIUS)->EnableWindow(FALSE);
				}
			}
			else
			{
				EnablePassAngle(FALSE);
			}

			break;

		default:break;

		}	
		//////
		OnCbnSelchangeComboCmdType();
		OnCbnSelchangeRadioHead();
		//////	
	}
}
void CDlgAxisPos::OnBnClickedCheckVel()
{
	// TODO: Add your control notification handler code here	
	GetDlgItem(IDC_AXIS_POS_EDIT_VEL)->EnableWindow(0 == ((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_VEL))->GetCheck()?FALSE:TRUE);
}
void CDlgAxisPos::OnBnClickedCheckDelay()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_AXIS_POS_EDIT_WAIT_TIME)->EnableWindow(0 == ((CButton*)GetDlgItem(IDC_AXIS_POS_CHECK_DELAY))->GetCheck()?FALSE:TRUE);
}
void CDlgAxisPos::OnCbnSelchangeComboCmdType()
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_AXIS_POS_COMBO_CMD_TYPE);
	CComboBox *pBox1 = (CComboBox*)GetDlgItem(IDC_AXIS_POS_RADIO_HEAD);

	switch(pBox->GetCurSel())
	{
	case 0://CMD_DUNNY
		EnableCheckVel(FALSE);
		EnableCheckDelay(FALSE);
		EnablePassAngle(FALSE);
		GetDlgItem(IDC_AXIS_POS_RADIO_HEAD)->EnableWindow(FALSE);

		GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF)->ShowWindow(SW_HIDE);
     	break;
	case 4://CMD_LINE_ARC
		GetDlgItem(IDC_AXIS_POS_RADIO_HEAD)->EnableWindow(FALSE);
		EnableCheckVel(FALSE);
		EnableCheckDelay(FALSE);
		EnablePassAngle(FALSE);
		break;
	case 1://CMD_POINT
	case 2://CMD_LINE_START
		GetDlgItem(IDC_AXIS_POS_RADIO_HEAD)->EnableWindow(TRUE);
		if((g_pFrm->m_Robot->dcp[pBox1->GetCurSel()].bBitEnable))
		{
			GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF)->EnableWindow(TRUE);			
		}
		else
		{
			GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF)->ShowWindow(SW_HIDE);
		}


		EnableCheckVel(FALSE);
		EnableCheckDelay(TRUE);
		EnablePassAngle(FALSE);
		break;
	case 3://CMD_LINE_PASSING
	case 5://CMD_LINE_END
		GetDlgItem(IDC_AXIS_POS_RADIO_HEAD)->EnableWindow(FALSE);
		if((g_pFrm->m_Robot->dcp[pBox1->GetCurSel()].bBitEnable))
		{
			GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF)->ShowWindow(SW_SHOW);
			if(pBox->GetCurSel()==3)//passing
				GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF)->EnableWindow(TRUE);//wsg	
			else
				GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF)->EnableWindow(FALSE);//wsg	
		}
		else
		{
			GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF)->ShowWindow(SW_HIDE);
		}


		if (pBox->GetCurSel() ==3)
			EnablePassAngle(TRUE);
		else
			EnablePassAngle(FALSE);

		EnableCheckVel(TRUE);
		EnableCheckDelay(TRUE);
		break;

       case 6://CMD_POT
		  
		   GetDlgItem(IDC_AXIS_POS_RADIO_HEAD)->EnableWindow(FALSE);
		   GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF)->ShowWindow(SW_SHOW);
           break;

	}
}
void CDlgAxisPos::EnablePassAngle(BOOL bEnable)
{
	GetDlgItem(IDC_AXIS_POS_CHECK_ANGLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_AXIS_POS_EDIT_ANGLERADIUS)->EnableWindow(bEnable);
	GetDlgItem(Tag_5)->EnableWindow(bEnable);
}
void CDlgAxisPos::EnableCheckVel(BOOL bEnable)
{
	GetDlgItem(IDC_AXIS_POS_CHECK_VEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_AXIS_POS_EDIT_VEL)->EnableWindow(bEnable);
	GetDlgItem(Tag_3)->EnableWindow(bEnable);
}
void CDlgAxisPos::EnableCheckDelay(BOOL bEnable)
{
	GetDlgItem(IDC_AXIS_POS_CHECK_DELAY)->EnableWindow(bEnable);
	GetDlgItem(IDC_AXIS_POS_EDIT_WAIT_TIME)->EnableWindow(bEnable);
	GetDlgItem(Tag_4)->EnableWindow(bEnable);
}
void CDlgAxisPos::OnCbnSelchangeRadioHead()
{

	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_AXIS_POS_RADIO_HEAD);
	m_pCmdLine->pIntValue[0] = pBox->GetCurSel();
	if(g_pFrm->m_Robot->dcp[m_pCmdLine->pIntValue[0]].bBitEnable)
		GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_AXIS_POS_CHECK_DISPENSE_ONOFF)->ShowWindow(SW_HIDE);

	OnCbnSelchangeComboCmdType();
}

void CDlgAxisPos::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

}

void CDlgAxisPos::OnBnClickedAxisPosCheckDispenseOnoff()
{
	
}

void CDlgAxisPos::OnEnChangeAxisPosEditX()
{

}

void CDlgAxisPos::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
