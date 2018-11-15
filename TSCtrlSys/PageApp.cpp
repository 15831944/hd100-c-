// PageApp.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "PageApp.h"
// CPageApp dialog

IMPLEMENT_DYNAMIC(CPageApp, CPropertyPage)

CPageApp::CPageApp()
: CPropertyPage(CPageApp::IDD)
, m_iWashCount(0)
, m_iWashTime(0)
, m_nPRSDelay(0)
, m_nPRSTimes(0)
, m_nSPRSTime(0)
, m_nSPRSDelay(0)
, m_dWeightMax(0)
, m_dWeightMin(0)
{

}

CPageApp::~CPageApp()
{
}

void CPageApp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PAGE_APP_WASH_COUNT, m_iWashCount);
	DDX_Text(pDX, IDC_PAGE_APP_WASH_TIMER, m_iWashTime);
	DDX_Text(pDX, IDC_EDIT_AUTORPSDELAY, m_nPRSDelay);
	DDX_Text(pDX, IDC_EDIT_AUTORPSTIMES, m_nPRSTimes);
	DDX_Text(pDX, IDC_EDIT_AUTOSRPSTIME, m_nSPRSTime);
	DDX_Text(pDX, IDC_EDIT_AUTOSRPSDELAY, m_nSPRSDelay);
	DDX_Text(pDX, IDC_PAGE_APP_WEGHT_MAXLMT, m_dWeightMax);
	DDX_Text(pDX, IDC_PAGE_APP_WEGHT_MINLMT, m_dWeightMin);
}


BEGIN_MESSAGE_MAP(CPageApp, CPropertyPage)

	ON_BN_CLICKED(IDC_PAGE_APP_RADISP1, &CPageApp::OnBnClickedPageAppRadisp1)
	ON_EN_CHANGE(IDC_PAGE_APP_WASH_COUNT, &CPageApp::OnEnChangePageAppWashCount)
	ON_BN_CLICKED(IDC_PAGE_APP_RADISP3, &CPageApp::OnBnClickedPageAppRadisp3)
	ON_EN_CHANGE(IDC_PAGE_APP_WASH_TIMER, &CPageApp::OnEnChangePageAppWashTimer)
	ON_BN_CLICKED(IDC_PAGE_APP_RADIST1, &CPageApp::OnBnClickedPageAppRadist1)
	ON_BN_CLICKED(IDC_PAGE_APP_RADISIDC_PAGE_APP_RADIST2, &CPageApp::OnBnClickedPageAppRadisidcPageAppRadist2)
	ON_BN_CLICKED(IDC_CHECK_GOSTANDBYPOS, &CPageApp::OnBnClickedCheckGostandbypos)
	ON_BN_CLICKED(IDC_PAGE_APP_RADIST2, &CPageApp::OnBnClickedPageAppRadist2)
	ON_BN_CLICKED(IDC_PAGE_APP_RADIST_WORKMODE1, &CPageApp::OnBnClickedPageAppRadistWorkmode1)
	ON_BN_CLICKED(IDC_PAGE_APP_RADIST_WORKMODE2, &CPageApp::OnBnClickedPageAppRadistWorkmode2)
	ON_EN_CHANGE(IDC_EDIT_FLYJETDELAY, &CPageApp::OnEnChangeEditFlyjetdelay)
	//ON_BN_CLICKED(IDC_CHECK_FILESAPTER, &CPageApp::OnBnClickedCheckFilesapter)
	ON_BN_CLICKED(IDC_CHECK_ENABLEWORKTEGETHER, &CPageApp::OnBnClickedCheckEnableworktegether)
	ON_EN_CHANGE(IDC_EDIT_MAKEUPNUM, &CPageApp::OnEnChangeEditMakeupnum)
	ON_BN_CLICKED(IDC_CHECK_ENABLESQERTEANLE, &CPageApp::OnBnClickedCheckEnablesqerteanle)
	ON_BN_CLICKED(IDC_PAGE_APP_RADISP2, &CPageApp::OnBnClickedPageAppRadisp2)
	ON_EN_CHANGE(IDC_EDIT_AUTORPSDELAY, &CPageApp::OnEnChangeEditAutorpsdelay)
	ON_EN_CHANGE(IDC_EDIT_AUTORPSTIMES, &CPageApp::OnEnChangeEditAutorpstimes)
	ON_EN_CHANGE(IDC_EDIT_AUTOSRPSTIME, &CPageApp::OnEnChangeEditAutosrpstime)
	ON_EN_CHANGE(IDC_EDIT_AUTOSRPSDELAY, &CPageApp::OnEnChangeEditAutosrpsdelay)
	ON_EN_CHANGE(IDC_PAGE_APP_WEGHT_MAXLMT, &CPageApp::OnEnChangePageAppWeghtMaxlmt)
	ON_EN_CHANGE(IDC_PAGE_APP_WEGHT_MINLMT, &CPageApp::OnEnChangePageAppWeghtMinlmt)
	ON_BN_CLICKED(IDC_CHECK_GOSTANDBYPOS2, &CPageApp::OnBnClickedCheckGostandbypos2)
END_MESSAGE_MAP()

// CPageApp message handlers

BOOL CPageApp::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
   
	CString  str;  
// 	// ����ģʽ
// 	if(g_pFrm->m_pSysParam->nWorkMode == 0)
// 	{
// 		// ��ͨģʽ
// 		((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1))->SetCheck(1);
// 		((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2))->SetCheck(0);
// 		GetDlgItem(IDC_EDIT_FLYJETDELAY)->EnableWindow(FALSE);
// 
// 	}else
// 	{
// 		// �ɽ�ģʽ
// 		((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1))->SetCheck(0);
// 		((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2))->SetCheck(1);
// 		GetDlgItem(IDC_EDIT_FLYJETDELAY)->EnableWindow(TRUE);
// 	}
	str.Format("%.2f",g_pFrm->m_pSysParam->dJetOpenDelay);// ����ϵ��
	GetDlgItem(IDC_EDIT_FLYJETDELAY)->SetWindowText(str);

	// �ֶμӹ�ʹ��
	if(g_pFrm->m_pSysParam->nFileSeparaEnable == 1)
		((CButton*)GetDlgItem(IDC_CHECK_ENABLESQERTEANLE))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_CHECK_ENABLESQERTEANLE))->SetCheck(FALSE);

	// ƴ�ռӹ�ʹ��
	if(g_pFrm->m_pSysParam->nFileScrapeEnable == 1)
	{
		((CButton*)GetDlgItem(IDC_CHECK_ENABLEWORKTEGETHER))->SetCheck(TRUE);
		GetDlgItem(IDC_EDIT_MAKEUPNUM)->EnableWindow(TRUE);

	}else
	{
		((CButton*)GetDlgItem(IDC_CHECK_ENABLEWORKTEGETHER))->SetCheck(FALSE);
		GetDlgItem(IDC_EDIT_MAKEUPNUM)->EnableWindow(FALSE);
	}

	// ƴ�ո���
	str.Format("%d",g_pFrm->m_pSysParam->nFileScrapeNum);
	GetDlgItem(IDC_EDIT_MAKEUPNUM)->SetWindowText(str);

	// ��������
	if(g_pFrm->m_pSysParam->nToolType==0)
	{  
		// ���䷧
		((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP1))->SetCheck(1); 
		((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP2))->SetCheck(0); 
		((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP3))->SetCheck(0);
		GetDlgItem(IDC_PAGE_APP_RADIST1)->EnableWindow(TRUE);
		GetDlgItem(IDC_PAGE_APP_RADIST2)->EnableWindow(TRUE);
		GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1)->EnableWindow(TRUE);
        GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2)->EnableWindow(TRUE);
		GetDlgItem(Tag_5)->EnableWindow(TRUE);
	}
	else if(g_pFrm->m_pSysParam->nToolType==1)
	{
		// ��ͷģʽ
		((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP1))->SetCheck(0);
        ((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP2))->SetCheck(1);
		((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP3))->SetCheck(0);
		GetDlgItem(IDC_PAGE_APP_RADIST1)->EnableWindow(FALSE);
		GetDlgItem(IDC_PAGE_APP_RADIST2)->EnableWindow(FALSE);
		GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2)->EnableWindow(FALSE);
		GetDlgItem(Tag_5)->EnableWindow(FALSE);

	}
	else if(g_pFrm->m_pSysParam->nToolType==2)
	{
		// ͼ��ģʽ
		((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP1))->SetCheck(0);
		((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP2))->SetCheck(0);
		((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP3))->SetCheck(1);
		GetDlgItem(IDC_PAGE_APP_RADIST1)->EnableWindow(FALSE);
		GetDlgItem(IDC_PAGE_APP_RADIST2)->EnableWindow(FALSE);
		GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2)->EnableWindow(FALSE);
		GetDlgItem(Tag_5)->EnableWindow(FALSE);

	}

	((CButton*)GetDlgItem(IDC_CHECK_GOSTANDBYPOS))->SetCheck(g_pFrm->m_pSysParam->nGoStandByPos);
	((CButton*)GetDlgItem(IDC_CHECK_GOSTANDBYPOS2))->SetCheck(g_pFrm->m_pSysParam->bEableWeightAlm);

	m_iWashCount = g_pFrm->m_pSysParam->uiWashPinCycle;
	m_iWashTime= g_pFrm->m_pSysParam->iWashNum;

    m_nPRSDelay = g_pFrm->m_pSysParam->dAutoPRSdelay;
	m_nPRSTimes = g_pFrm->m_pSysParam->dPRSTimes;
	m_nSPRSDelay = g_pFrm->m_pSysParam->dSPRSDelay;
	m_nSPRSTime = g_pFrm->m_pSysParam->dSPRSTime;
     m_dWeightMin = g_pFrm->m_pSysParam->dWeightLmtMin;
	 m_dWeightMax= g_pFrm->m_pSysParam->dWeightLmtMax;
	  
	GetDlgItem(Tag_1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageAPP,"Tag_1"));
	GetDlgItem(Tag_2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageAPP,"Tag_2"));
	GetDlgItem(Tag_3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageAPP,"Tag_3"));
	GetDlgItem(Tag_4)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageAPP,"Tag_4"));
	GetDlgItem(Tag_5)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageAPP,"Tag_5"));
	GetDlgItem(IDC_PAGE_APP_RADISP1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageAPP,"Btn_1"));
	GetDlgItem(IDC_PAGE_APP_RADISP3)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageAPP,"Btn_3"));
	GetDlgItem(IDC_PAGE_APP_RADIST1)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageAPP,"Btn_4"));
	GetDlgItem(IDC_PAGE_APP_RADIST2)->SetWindowText(g_pFrm->LanguageMgr->ReadStringFromIni(DefPageAPP,"Btn_5"));
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageApp::OnApply()
{
	UpdateData(TRUE);
	CString str;
	g_pFrm->m_pSysParam->uiWashPinCycle = m_iWashCount;
	g_pFrm->m_pSysParam->iWashNum       = m_iWashTime;
	g_pFrm->m_pSysParam->dAutoPRSdelay  = m_nPRSDelay;
	g_pFrm->m_pSysParam->dPRSTimes      = m_nPRSTimes;
	g_pFrm->m_pSysParam->dSPRSDelay     = m_nSPRSDelay;
	g_pFrm->m_pSysParam->dSPRSTime      = m_nSPRSTime;
	g_pFrm->m_pSysParam->dWeightLmtMin  = m_dWeightMin;
    g_pFrm->m_pSysParam->dWeightLmtMax  = m_dWeightMax;
	g_pFrm->m_pSysParam->nGoStandByPos  = ((CButton*)GetDlgItem(IDC_CHECK_GOSTANDBYPOS))->GetCheck();
    g_pFrm->m_pSysParam->bEableWeightAlm = ((CButton*)GetDlgItem(IDC_CHECK_GOSTANDBYPOS2))->GetCheck();
	
	GetDlgItem(IDC_EDIT_FLYJETDELAY)->GetWindowText(str);
	g_pFrm->m_pSysParam->dJetOpenDelay = atof(str);

	// ƴ�ռӹ�ʹ��
	g_pFrm->m_pSysParam->nFileScrapeEnable = ((CButton*)GetDlgItem(IDC_CHECK_ENABLEWORKTEGETHER))->GetCheck();
	// ƴ�ո���

	GetDlgItem(IDC_EDIT_MAKEUPNUM)->GetWindowText(str);
	g_pFrm->m_pSysParam->nFileScrapeNum = atoi(str);

	g_pFrm->SysParam(FALSE);
	return CPropertyPage::OnApply();
}

void CPageApp::OnBnClickedPageAppRadisp1()
{
	// ���䷧����ģʽ
	g_pFrm->m_pSysParam->nToolType = 0;
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP1))->SetCheck(1);
    ((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP2))->SetCheck(0);
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP3))->SetCheck(0);
   
	// ���䷧����ѡ������
	GetDlgItem(IDC_PAGE_APP_RADIST1)->EnableWindow(TRUE);
	GetDlgItem(IDC_PAGE_APP_RADIST2)->EnableWindow(TRUE);

	GetDlgItem(Tag_5)->EnableWindow(TRUE);

// 	//����ģʽ
 	GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1)->EnableWindow(TRUE);
 	GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2)->EnableWindow(TRUE);
 
// 	GetDlgItem(Tag_7)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT_FLYJETDELAY)->EnableWindow(TRUE);
// 	((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1))->SetCheck(0);
// 	((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2))->SetCheck(1);

	SetModified();
}


void CPageApp::OnEnChangePageAppWashCount()
{
	SetModified();
}

void CPageApp::OnBnClickedPageAppRadisp3()
{
	// TODO: Add your control notification handler code here

	g_pFrm->m_pSysParam->nToolType = 2;
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP1))->SetCheck(0);
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP2))->SetCheck(0);
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP3))->SetCheck(1);

	GetDlgItem(IDC_PAGE_APP_RADIST1)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAGE_APP_RADIST2)->EnableWindow(FALSE);

	GetDlgItem(Tag_5)->EnableWindow(FALSE);

	// ͼ��ģʽ���ù���ģʽѡ��
 	GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1)->EnableWindow(FALSE);
 	GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2)->EnableWindow(FALSE);
 
 	GetDlgItem(Tag_7)->EnableWindow(FALSE);
 	GetDlgItem(IDC_EDIT_FLYJETDELAY)->EnableWindow(FALSE);

	SetModified();
}

void CPageApp::OnEnChangePageAppWashTimer()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
	SetModified();
}
//�������䷧
void CPageApp::OnBnClickedPageAppRadist1()
{
	g_pFrm->m_pSysParam->nToolType = 0;
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST1))->SetCheck(1);
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST2))->SetCheck(0);

	((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP1))->SetCheck(1);
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP3))->SetCheck(0);

//    // �ؼ�ԭ����Ҫ��������
// 	if(g_pFrm->m_pSysParam->nWorkMode == 0)
//  	{
//  		((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1))->SetCheck(1);
//  		((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2))->SetCheck(0);
//  	}else
//  	{
//  		((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1))->SetCheck(0);
//  		((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2))->SetCheck(1);
//  	}

	SetModified();
}
// ѹ�����䷧
void CPageApp::OnBnClickedPageAppRadisidcPageAppRadist2()
{
	g_pFrm->m_pSysParam->nToolType = 0;
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST1))->SetCheck(0);
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST2))->SetCheck(1);

	((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP1))->SetCheck(1);
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP3))->SetCheck(0);


// 	if(g_pFrm->m_pSysParam->nWorkMode == 0)
//  	{
//  		((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1))->SetCheck(1);
//  		((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2))->SetCheck(0);
//  	}else
// 	{
// 		((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1))->SetCheck(0);
//  		((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2))->SetCheck(1);
//  	}

	SetModified();
}

void CPageApp::OnBnClickedCheckGostandbypos()
{
	SetModified();
}

void CPageApp::OnBnClickedPageAppRadist2()
{
	SetModified();
}
// ����ģʽѡ�� ��ͨģʽ 0
void CPageApp::OnBnClickedPageAppRadistWorkmode1()
{
// 	g_pFrm->m_pSysParam->nWorkMode = 0;
// 	((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1))->SetCheck(1);
// 	((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2))->SetCheck(0);
// 	GetDlgItem(IDC_EDIT_FLYJETDELAY)->EnableWindow(FALSE);

	SetModified();

}
// ����ģʽѡ�� �������� 1
void CPageApp::OnBnClickedPageAppRadistWorkmode2()
{
// 	g_pFrm->m_pSysParam->nWorkMode = 1;
// 	((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1))->SetCheck(0);
// 	((CButton*) GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2))->SetCheck(1);
// 	GetDlgItem(IDC_EDIT_FLYJETDELAY)->EnableWindow(TRUE);

	SetModified();
}

void CPageApp::OnEnChangeEditFlyjetdelay()
{
	SetModified();
}
// ƴ��ӹ�ʹ��
void CPageApp::OnBnClickedCheckEnableworktegether()
{
	UpdateData(TRUE);
	// ƴ��״̬
	BOOL Hsts = ((CButton*)GetDlgItem(IDC_CHECK_ENABLEWORKTEGETHER))->GetCheck();
	if((1 == Hsts))  
	{ 
		g_pFrm->m_pSysParam->nFileScrapeEnable = 1;
		GetDlgItem(IDC_EDIT_MAKEUPNUM)->EnableWindow(TRUE);

		// �ֶμӹ�״̬
		if(g_pFrm->m_pSysParam->nFileSeparaEnable == 1)
		{
			((CButton*)GetDlgItem(IDC_CHECK_ENABLESQERTEANLE))->SetCheck(FALSE);
			g_pFrm->m_pSysParam->nFileSeparaEnable = 0;
			g_pFrm->m_Robot->ExOutput("�ְ�ʹ��",FALSE);
		}
		
	}else
	{
		g_pFrm->m_pSysParam->nFileScrapeEnable = 0;
		GetDlgItem(IDC_EDIT_MAKEUPNUM)->EnableWindow(FALSE);
	}

	SetModified();
}

void CPageApp::OnEnChangeEditMakeupnum()
{
	SetModified();
}

// �ְ�ӹ�ʹ��
void CPageApp::OnBnClickedCheckEnablesqerteanle()
{
	UpdateData(TRUE);

	// �ֶμӹ�״̬
	BOOL Hsts = ((CButton*)GetDlgItem(IDC_CHECK_ENABLESQERTEANLE))->GetCheck();
	if((1 == Hsts))  
	{   
		// �ֶμӹ�ʹ��
		g_pFrm->m_pSysParam->nFileSeparaEnable = 1;
		g_pFrm->m_Robot->ExOutput("�ְ�ʹ��",TRUE);
        
		// ƴ��ȡ��
		if(1 == g_pFrm->m_pSysParam->nFileScrapeEnable)  
		{ 
			((CButton*)GetDlgItem(IDC_CHECK_ENABLEWORKTEGETHER))->SetCheck(FALSE);
			g_pFrm->m_pSysParam->nFileScrapeEnable = 0;
		}

	}else
	{
		g_pFrm->m_pSysParam->nFileSeparaEnable = 0;
		g_pFrm->m_Robot->ExOutput("�ְ�ʹ��",FALSE);
	}

	SetModified();
}
// ��Ͳģʽ
void CPageApp::OnBnClickedPageAppRadisp2()
{
	g_pFrm->m_pSysParam->nToolType = 1;
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP1))->SetCheck(0);
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP2))->SetCheck(1);
	((CButton*) GetDlgItem(IDC_PAGE_APP_RADISP3))->SetCheck(0);

	GetDlgItem(IDC_PAGE_APP_RADIST1)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAGE_APP_RADIST2)->EnableWindow(FALSE);

	GetDlgItem(Tag_5)->EnableWindow(FALSE);

	// ͼ��ģʽ���ù���ģʽѡ��
	GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE1)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAGE_APP_RADIST_WORKMODE2)->EnableWindow(FALSE);

	GetDlgItem(Tag_7)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FLYJETDELAY)->EnableWindow(FALSE);

	SetModified();
}

void CPageApp::OnEnChangeEditAutorpsdelay()
{
    SetModified();
}

void CPageApp::OnEnChangeEditAutorpstimes()
{
	SetModified();
}

void CPageApp::OnEnChangeEditAutosrpstime()
{
    SetModified();
}

void CPageApp::OnEnChangeEditAutosrpsdelay()
{
	SetModified();
}

void CPageApp::OnEnChangePageAppWeghtMaxlmt()
{
		SetModified();
}

void CPageApp::OnEnChangePageAppWeghtMinlmt()
{
		SetModified();
}

void CPageApp::OnBnClickedCheckGostandbypos2()
{
	SetModified();
}
