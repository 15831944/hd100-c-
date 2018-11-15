// DlgWCL.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgWCL.h"


// CDlgWCL dialog

IMPLEMENT_DYNAMIC(CDlgWCL, CDialog)

CDlgWCL::CDlgWCL(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWCL::IDD, pParent)
{
   m_pCmdLine = NULL;
}

CDlgWCL::~CDlgWCL()
{
}

void CDlgWCL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WCL_COMBO, m_Combox);
}


BEGIN_MESSAGE_MAP(CDlgWCL, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgWCL::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgWCL::OnBnClickedBtnApply)
	ON_CBN_SELCHANGE(IDC_WCL_COMBO, &CDlgWCL::OnCbnSelchangeWclCombo)
	ON_BN_CLICKED(IDC_BTN_READ_AXISPOS, &CDlgWCL::OnBnClickedBtnReadAxispos)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CDlgWCL::OnBnClickedBtnMove)
	ON_BN_CLICKED(IDCANCEL, &CDlgWCL::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_WORKMODE2, &CDlgWCL::OnBnClickedWorkmode2)
	ON_BN_CLICKED(IDC_WORKMODE1, &CDlgWCL::OnBnClickedWorkmode1)
	ON_EN_CHANGE(IDC_WCL_EDIT_W, &CDlgWCL::OnEnChangeWclEditW)
END_MESSAGE_MAP()


// CDlgWCL message handlers

BOOL CDlgWCL::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Combox.AddString("直线起点");
	m_Combox.AddString("直线终点");
    m_Combox.SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgWCL::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnBnClickedBtnApply();
	OnOK();
}
void CDlgWCL::RefreshAxisPos(void)
{

	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		int n = m_Combox.GetCurSel();
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+0]);
		SetDlgItemText(IDC_WCL_EDIT_X, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+1]);
		SetDlgItemText(IDC_WCL_EDIT_Y, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+2]);
		SetDlgItemText(IDC_WCL_EDIT_Z, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+3]);
		SetDlgItemText(IDC_WCL_EDIT_U, strTemp);
        
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[8]);
		SetDlgItemText( IDC_WCL_C1, strTemp);
        
		//strTemp.Format("%ld", g_pFrm->m_Robot->m_pSysParam->S_DispOPenT);
         strTemp.Format("%d", m_pCmdLine->pIntValue[1]);
		SetDlgItemText(IDC_WCL_T1, strTemp);
		
		strTemp.Format("%d", m_pCmdLine->pIntValue[2]);
		SetDlgItemText(IDC_WCL_T2, strTemp);
		strTemp.Format("%d", m_pCmdLine->pIntValue[3]);//次数
		SetDlgItemText(IDC_WCL_EDIT_W, strTemp);
          
	     CButton *pbox2=(CButton*)GetDlgItem(IDC_CDISP);

         
            pbox2->SetCheck(m_pCmdLine->pIntValue[4]);
        
      
          CButton *pbox=(CButton*)GetDlgItem(IDC_WORKMODE1);
		  CButton *pbox1=(CButton*)GetDlgItem(IDC_WORKMODE2);

		if( m_pCmdLine->pIntValue[0]==0)
		{
			pbox->SetCheck(1);
			pbox1->SetCheck(0);
			OnBnClickedWorkmode1();
		}
		else
		{
			pbox->SetCheck(0);
			pbox1->SetCheck(1);
           OnBnClickedWorkmode2();
		}
       

	}
}

void CDlgWCL::OnBnClickedBtnApply()
{
	// TODO: Add your control notification handler code here

	if(m_pCmdLine != NULL)
	{
		
		int n= m_Combox.GetCurSel();

		m_pCmdLine->pIntValue[0] = CWnd::GetCheckedRadioButton(IDC_WORKMODE1, IDC_WORKMODE2)-IDC_WORKMODE1;
        
		CString strTemp;
		GetDlgItemText(IDC_WCL_EDIT_X, strTemp);
		m_pCmdLine->pDblValue[n*4+0] =atof(strTemp);
		GetDlgItemText(IDC_WCL_EDIT_Y, strTemp);
		m_pCmdLine->pDblValue[n*4+1] =atof(strTemp);
		GetDlgItemText(IDC_WCL_EDIT_Z, strTemp);
		m_pCmdLine->pDblValue[n*4+2] =atof(strTemp);
		GetDlgItemText(IDC_WCL_EDIT_U, strTemp);
		m_pCmdLine->pDblValue[n*4+3] =atof(strTemp);
         
		GetDlgItemText(IDC_WCL_EDIT_U, strTemp);
		m_pCmdLine->pDblValue[n*4+3] =atof(strTemp);
      
		
		GetDlgItemText(IDC_WCL_C1, strTemp);
        m_pCmdLine->pDblValue[8] =atof(strTemp);
		
		GetDlgItemText(IDC_WCL_T1, strTemp);
        m_pCmdLine->pIntValue[1] =atoi(strTemp);
		
		GetDlgItemText(IDC_WCL_T2, strTemp);
		m_pCmdLine->pIntValue[2] =atoi(strTemp);
	
		GetDlgItemText(IDC_WCL_EDIT_W, strTemp);
		m_pCmdLine->pIntValue[3] =atoi(strTemp);
		CButton *pbox2=(CButton*)GetDlgItem(IDC_CDISP);


		m_pCmdLine->pIntValue[4]=pbox2->GetCheck();

		g_pFrm->m_wndEditBar.UpateCmdListBox(m_iSelIndex);
	}




















}

void CDlgWCL::OnCbnSelchangeWclCombo()
{
	// TODO: Add your control notification handler code here
     
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		int n = m_Combox.GetCurSel();
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+0]);
		SetDlgItemText(IDC_WCL_EDIT_X, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+1]);
		SetDlgItemText(IDC_WCL_EDIT_Y, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+2]);
		SetDlgItemText(IDC_WCL_EDIT_Z, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+3]);
		SetDlgItemText(IDC_WCL_EDIT_U, strTemp);
	}

}

void CDlgWCL::OnBnClickedBtnReadAxispos()
{
	// TODO: Add your control notification handler code here

	if(m_pCmdLine != NULL)
	{
	
		int n = m_Combox.GetCurSel();

		g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, n*4);
		RefreshAxisPos();
	}









}

void CDlgWCL::OnBnClickedBtnMove()
{
	// TODO: Add your control notification handler code here
	
	int n = m_Combox.GetCurSel()*4;

	double dValue[4];
	double dVel[4];
	double dAcc[4];

	for (int i=0;i<4;i++)
	{
		dValue[i] = m_pCmdLine->pDblValue[n+i];
		dAcc[i] = g_pFrm->m_Robot->m_pSysParam->tAxis[i].acc;//mm/s*s
	}

	dVel[0] = g_pFrm->m_Robot->m_pSysParam->dVelEditXY[1];
	dVel[1] = g_pFrm->m_Robot->m_pSysParam->dVelEditXY[1];
	dVel[2] = g_pFrm->m_Robot->m_pSysParam->dVelEditZ[1];
	dVel[3] = g_pFrm->m_Robot->m_pSysParam->dVelEditA[1];//mm/s

	g_pFrm->m_Robot->m_pController->MoveToPoint(dValue,dVel,dAcc,TRUE);



}

void CDlgWCL::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDlgWCL::OnBnClickedWorkmode2()
{
	// TODO: Add your control notification handler code here
   for (int i=1;i<3;i++)
   {
       GetDlgItem(IDC_WCL_EDIT_W+i)->EnableWindow(true);


   }

   GetDlgItem(IDC_WCL_EDIT_W)->EnableWindow(false);







}

void CDlgWCL::OnBnClickedWorkmode1()
{
	// TODO: Add your control notification handler code here
	for (int i=1;i<4;i++)
	{
		GetDlgItem(IDC_WCL_EDIT_W+i)->EnableWindow(false);


	}

	GetDlgItem(IDC_WCL_EDIT_W)->EnableWindow(true);


}

void CDlgWCL::OnEnChangeWclEditW()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString  str;
	GetDlgItemText(IDC_WCL_EDIT_W,str);
	double tempW=atoi(str)*g_pFrm->m_Robot->m_pSysParam->WeightRate*1000;
	str.Format("%0.4f",tempW);
	SetDlgItemText(IDC_WCL_C1,str);
}
