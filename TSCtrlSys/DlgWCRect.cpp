// DlgWCRect.cpp : implementation file
//

#include "stdafx.h"
#include "DlgWCRect.h"
#include "TSCtrlSys.h"
#include "math.h"
// CDlgWCRect dialog

IMPLEMENT_DYNAMIC(CDlgWCRect, CDialog)

CDlgWCRect::CDlgWCRect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWCRect::IDD, pParent)
	, m_dWcPointNum(0)
	, m_dWCRectTatolW(0)
	, m_dWcRectVel(0)
{
	m_pCmdLine = NULL;
	m_dWCLPointData = 0 ;    // ��������
}

CDlgWCRect::~CDlgWCRect()
{
}

void CDlgWCRect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_COMB_WCRPOS, m_ComboxCtrl);
	DDX_Text(pDX, IDC_WCR_POINTNUM, m_dWcPointNum);
	DDX_Text(pDX, IDC_WCL_LINEEIGHT, m_dWCRectTatolW);
	DDX_Text(pDX, IDC_WCL_LINESPEED, m_dWcRectVel);
}


BEGIN_MESSAGE_MAP(CDlgWCRect, CDialog)
	ON_BN_CLICKED(Btn_3, &CDlgWCRect::OnBnClicked3)
	ON_CBN_SELCHANGE(IDC_DLG_COMB_WCRPOS, &CDlgWCRect::OnCbnSelchangeDlgCombWcrpos)
	ON_BN_CLICKED(Btn_1, &CDlgWCRect::OnBnClicked1)
	ON_BN_CLICKED(Btn_2, &CDlgWCRect::OnBnClicked2)
	ON_BN_CLICKED(IDOK, &CDlgWCRect::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgWCRect::OnBnClickedCancel)
END_MESSAGE_MAP()


//Ӧ��
void CDlgWCRect::OnBnClicked3()
{
	CString strTemp;
	int n = m_ComboxCtrl.GetCurSel();
	UpdateData(TRUE);
	if(m_pCmdLine != NULL)
	{
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+0]);
		SetDlgItemText(IDC_WCL_POSX, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+1]);
		SetDlgItemText(IDC_WCL_POSY, strTemp);

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+2]);
		SetDlgItemText(IDC_WCL_POSZ, strTemp);

	     // ����������
		m_pCmdLine->pDblValue[20] = m_dWCRectTatolW;              

		// ��������
		m_dWcPointNum = (int)(m_dWCRectTatolW / g_pFrm->m_pSysParam->dBCMGPorDOT); 
		m_pCmdLine->pIntValue[3] =  m_dWcPointNum;   

		//  // ���ػ����ٶ�
		m_pCmdLine->pDblValue[21] =  m_dWcRectVel; 

		g_pFrm->m_wndEditBar.UpdateCmdListBox(m_iSelIndex);

	}
	CheckPara();   // �������Ϸ���

	UpdateData(FALSE);
}

void CDlgWCRect::OnCbnSelchangeDlgCombWcrpos()
{
	UpdateData(TRUE);
	if(m_pCmdLine != NULL)
	{
		CString strTemp;
		int n = m_ComboxCtrl.GetCurSel();

		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+0]);
		SetDlgItemText(IDC_WCR_POSX, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+1]);
		SetDlgItemText(IDC_WCR_POSY, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+2]);
		SetDlgItemText(IDC_WCR_POSZ, strTemp);

	}
}
// �ƶ���
void CDlgWCRect::OnBnClicked1()
{
	int n = m_ComboxCtrl.GetCurSel();
	g_pFrm->m_Robot->GotoSafeZpos();
	double x=m_pCmdLine->pDblValue[n*4+0];
	double y=m_pCmdLine->pDblValue[n*4+1];
	double z=m_pCmdLine->pDblValue[n*4+2];
	g_pFrm->m_Precision.LineMoveXY(CRD1,0,x,y,g_pFrm->m_mtrParamGts[X_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[X_AXIS].m_Acc);
	g_pFrm->m_Robot->m_pController->AxisMove(Z_AXIS,z, g_pFrm->m_mtrParamGts[Z_AXIS].m_Vmax,g_pFrm->m_mtrParamGts[Z_AXIS].m_Acc,TRUE);

}

BOOL CDlgWCRect::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ComboxCtrl.AddString(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Btn_6"));
    m_ComboxCtrl.AddString(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Btn_7"));
	m_ComboxCtrl.AddString(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Btn_8"));
	m_ComboxCtrl.AddString(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Btn_9"));
	m_ComboxCtrl.AddString(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Btn_10"));
	m_ComboxCtrl.SetCurSel(0);

	RefreshAxisPos();  // λ��ˢ��

	CString strtemp;

	// ��������
	m_dWCLPointData  = g_pFrm->m_pSysParam->dBCMGPorDOT;   
	strtemp.Format(_T("%.5f"),m_dWCLPointData);
	GetDlgItem(IDC_WCL_WPERPOINT)->SetWindowText(strtemp);

	m_dWCRectTatolW = 20;                                 // ����������
	m_dWcPointNum = (int)(m_dWCRectTatolW/m_dWCLPointData); // �������
	m_dWcRectVel = 40;                                   // ���ػ����ٶ�

	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Tag_3"));
	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Tag_4"));

	GetDlgItem(Btn_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Btn_1"));
	GetDlgItem(Btn_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Btn_2"));
	GetDlgItem(Btn_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Btn_3"));
	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Btn_4"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgWCRECT,"Btn_5"));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgWCRect::RefreshAxisPos()
{
	CString strTemp;
	int n = m_ComboxCtrl.GetCurSel();

	if(m_pCmdLine != NULL)
	{
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+0]);
		SetDlgItemText(IDC_WCR_POSX, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+1]);
		SetDlgItemText(IDC_WCR_POSY, strTemp);
		strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+2]);
		SetDlgItemText(IDC_WCR_POSZ, strTemp);

		m_dWCRectTatolW = m_pCmdLine->pDblValue[20];              // ����������
		m_dWcPointNum = (int)(m_dWCRectTatolW/g_pFrm->m_pSysParam->dBCMGPorDOT); // �������
		m_dWcRectVel = m_pCmdLine->pDblValue[21];                // ���ػ����ٶ�

		UpdateData(FALSE);
	}
}
// ����λ��
void CDlgWCRect::OnBnClicked2()
{
	CString strTemp;
	int n = m_ComboxCtrl.GetCurSel();
	
		if(m_pCmdLine != NULL)
		{
			g_pFrm->m_wndEditBar.LoadListArrayPos(*m_pCmdLine, n*4);

			strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+0]);
			SetDlgItemText(IDC_WCR_POSX, strTemp);

			strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+1]);
			SetDlgItemText(IDC_WCR_POSY, strTemp);

			strTemp.Format("%.3f", m_pCmdLine->pDblValue[n*4+2]);
			SetDlgItemText(IDC_WCR_POSZ, strTemp);
		}

}

void CDlgWCRect::OnBnClickedOk()
{
	OnBnClicked3();
	OnOK();
}

void CDlgWCRect::OnBnClickedCancel()
{
	
	OnCancel();
}

void CDlgWCRect::CheckPara()
{
	// ����λ��
	double dxA,dyA,dzA,duA;
	double dxB,dyB,dzB,duB;
	double dxC,dyC,dzC,duC;
	double dxD,dyD,dzD,duD;
	double dxE,dyE,dzE,duE;
	
	dxA = m_pCmdLine->pDblValue[0];
	dyA = m_pCmdLine->pDblValue[1];
	dzA = m_pCmdLine->pDblValue[2];
	duA = m_pCmdLine->pDblValue[3];

	dxB = m_pCmdLine->pDblValue[4];
	dyB = m_pCmdLine->pDblValue[5];
	dzB = m_pCmdLine->pDblValue[6];
	duB = m_pCmdLine->pDblValue[7];

	dxC = m_pCmdLine->pDblValue[8];
	dyC = m_pCmdLine->pDblValue[9];
	dzC = m_pCmdLine->pDblValue[10];
	duC = m_pCmdLine->pDblValue[11];

	dxD = m_pCmdLine->pDblValue[12];
	dyD = m_pCmdLine->pDblValue[13];
	dzD = m_pCmdLine->pDblValue[14];
	duD = m_pCmdLine->pDblValue[15];

	dxE = m_pCmdLine->pDblValue[16];
	dyE = m_pCmdLine->pDblValue[17];
	dzE = m_pCmdLine->pDblValue[18];
	duE = m_pCmdLine->pDblValue[19];

	//�����ٶ� 
	double dWorkVel  = m_pCmdLine->pDblValue[21];

   
	//
	double disx,disy;
	double dis1,dis2,dis3,dis4,dis0;
    double t ;

	disx = dxB-dxA ;
	disy = dyB-dyA ;
	dis1=sqrt((disx)*(disx)+(disy)*(disy)); // �߶γ�mm
     
	disx = dxC-dxB ;
	disy = dyC-dyB ;
	dis2=sqrt((disx)*(disx)+(disy)*(disy)); // �߶γ�mm

	disx = dxD-dxC ;
	disy = dyD-dyC ;
	dis3=sqrt((disx)*(disx)+(disy)*(disy)); // �߶γ�mm

	disx = dxE-dxD ;
	disy = dyE-dyD ;
	dis4=sqrt((disx)*(disx)+(disy)*(disy)); // �߶γ�mm
   
	dis0= dis1+dis2+dis3+dis4;   // �����ܱ߳�

	t= dis0 / dWorkVel;           // ��Ҫ����ʱ�� s
	t= 1000*t;                    // ms

	int tdis = (int)( t / (int)m_pCmdLine->pIntValue[3]);  // ����ÿ�����ʱ���� ms
	tdis -= g_pFrm->m_Robot->m_DispCYtime;                 // ѭ�����ڼ��

	if(tdis<=0)
	{
		AfxMessageBox(_T("�ٶ������ʱ�������ƥ�䣬����������"));
	}
}