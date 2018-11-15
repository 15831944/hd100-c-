// DlgTool.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgTool.h"


// CDlgTool dialog

IMPLEMENT_DYNAMIC(CDlgTool, CDialog)

CDlgTool::CDlgTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTool::IDD, pParent)
{
   m_dJethightDis =  4;
}

CDlgTool::~CDlgTool()
{
}

void CDlgTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTool, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgTool::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_DLG_TOOL_RADIO_HEAD, &CDlgTool::OnCbnSelchangeDlgToolRadioHead)
	ON_EN_CHANGE(IDC_DLG_TOOL_Z_HEIGHT2, &CDlgTool::OnEnChangeDlgToolZHeight2)
	ON_BN_CLICKED(ID_APP, &CDlgTool::OnBnClickedApp)
	ON_BN_CLICKED(IDCANCEL, &CDlgTool::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgTool message handlers

BOOL CDlgTool::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSelectJet,"Tag_0"));
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSelectJet,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSelectJet,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSelectJet,"Tag_3"));
	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSelectJet,"Tag_4"));
	GetDlgItem(Tag_5)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSelectJet,"Tag_5"));

	GetDlgItem(IDOK)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSelectJet,"Btn_1"));
	GetDlgItem(IDCANCEL)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefDlgCMDSelectJet,"Btn_2"));
	
	// TODO:  Add extra initialization here
	CString str;
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_DLG_TOOL_RADIO_HEAD);
	for (int i=0;i<TOOL_NUM;i++)
	{
		str.Empty();
		str.Format("#%d",i);
		pBox->AddString(str);
	}
    
	pBox->SetCurSel(m_pCmdLine->pIntValue[0]);
	OnCbnSelchangeDlgToolRadioHead();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTool::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_DLG_TOOL_RADIO_HEAD);
	m_pCmdLine->pIntValue[0] = pBox->GetCurSel();

	CString str;
	GetDlgItemText(IDC_DLG_TOOL_OPEN_TIME,str);
	m_pCmdLine->pIntValue[1] = atoi(str);   // 出胶周期时间     

	GetDlgItemText(IDC_DLG_TOOL_CLOSE_TIME,str);
	m_pCmdLine->pIntValue[2] = atoi(str);   // 出胶周期间隔

	GetDlgItemText(IDC_DLG_TOOL_PULSE_NUM,str);
	m_pCmdLine->pDblValue[0] = atof(str);   // 周期出胶次数

	GetDlgItemText(IDC_DLG_TOOL_Z_HEIGHT2,str); // 相对高度
	m_pCmdLine->pDblValue[2] = atof(str); 

	// Z轴工作高度 = Z轴基准高度- 相对高度
	m_pCmdLine->pDblValue[1] = 
		g_pFrm->m_pRobotParam->m_zContactPos - m_pCmdLine->pDblValue[2];

	str.Format("%.3f",m_pCmdLine->pDblValue[1]);  // Z轴绝对高度
	SetDlgItemText(IDC_DLG_TOOL_Z_HEIGHT,str);

	g_pFrm->m_Robot->m_DispCYtime = m_pCmdLine->pIntValue[1]; // 
	OnOK();
}

void CDlgTool::OnCbnSelchangeDlgToolRadioHead()
{
	// TODO: Add your control notification handler code here
	CString str;
	CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_DLG_TOOL_RADIO_HEAD);
	int n = pBox->GetCurSel();
	if(n==0)
	{
		GetDlgItem(IDC_DLG_TOOL_OPEN_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_DLG_TOOL_CLOSE_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_DLG_TOOL_PULSE_NUM)->EnableWindow(TRUE);

		str.Format("%d",m_pCmdLine->pIntValue[1]);    // 开胶时间
		SetDlgItemText(IDC_DLG_TOOL_OPEN_TIME,str);
		str.Format("%d",m_pCmdLine->pIntValue[2]);    // 出胶间隔
		SetDlgItemText(IDC_DLG_TOOL_CLOSE_TIME,str);
		str.Format("%ld",(long)(m_pCmdLine->pDblValue[0])); // 单循环出胶次数
		SetDlgItemText(IDC_DLG_TOOL_PULSE_NUM,str);
	}
	else if (n==1)
	{
		GetDlgItem(IDC_DLG_TOOL_OPEN_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_DLG_TOOL_CLOSE_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_DLG_TOOL_PULSE_NUM)->EnableWindow(FALSE);
	}
    
	// Z轴工作高度 = Z轴基准高度- 相对高度
    m_pCmdLine->pDblValue[1] = 
		g_pFrm->m_pRobotParam->m_zContactPos - m_pCmdLine->pDblValue[2];

	str.Format("%.3f",m_pCmdLine->pDblValue[1]);  // Z轴高度
	SetDlgItemText(IDC_DLG_TOOL_Z_HEIGHT,str);

    str.Format("%.3f",m_pCmdLine->pDblValue[2] ); // Z轴相对高度
	SetDlgItemText(IDC_DLG_TOOL_Z_HEIGHT2,str);
    
	str.Format("%.3f",g_pFrm->m_pRobotParam->m_zContactPos);  // Z轴基准面高度
	SetDlgItemText(IDC_DLG_TOOL_Z_HEIGHT3,str);
}

// 相对高度
void CDlgTool::OnEnChangeDlgToolZHeight2()
{
	
}

// 应用
void CDlgTool::OnBnClickedApp()
{
	CString str;
	GetDlgItemText(IDC_DLG_TOOL_Z_HEIGHT2,str); // 相对高度
	m_pCmdLine->pDblValue[2] = atof(str); 

	// Z轴工作高度 = Z轴基准高度- 相对高度
	m_pCmdLine->pDblValue[1] = 
		g_pFrm->m_pRobotParam->m_zContactPos - m_pCmdLine->pDblValue[2];

	str.Format("%.3f",m_pCmdLine->pDblValue[1]);  // Z轴绝对高度
	SetDlgItemText(IDC_DLG_TOOL_Z_HEIGHT,str);
}

void CDlgTool::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
