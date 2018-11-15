// PagePort.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "PagePort.h"
#pragma warning(disable: 4100)

// CPagePort dialog

IMPLEMENT_DYNAMIC(CPagePort, CPropertyPage)

CPagePort::CPagePort()
	: CPropertyPage(CPagePort::IDD)
	, m_WorkMode(0)
	, m_Count(0)
	, m_OpenTime(0)
	, m_CloseTime(0)
	, m_pressJet(0)
	, m_pressDisp(0)
{

}

CPagePort::~CPagePort()
{
}

void CPagePort::DoDataExchange(CDataExchange* pDX)
{

	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_WorkMode);
	DDX_Text(pDX, IDC_EDIT2, m_Count);
	DDX_Text(pDX, IDC_EDIT3, m_OpenTime);
	DDX_Text(pDX, IDC_EDIT4, m_CloseTime);
	DDX_Text(pDX, IDC_EDIT5, m_pressJet);
	DDX_Text(pDX, IDC_EDIT6, m_pressDisp);
}


BEGIN_MESSAGE_MAP(CPagePort, CPropertyPage)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_MAP_EDIT_OUTPUT, IDC_MAP_EDIT_INPUT, &CPagePort::OnEnChangeEditOutput)
	ON_BN_CLICKED(IDC_BtRead, &CPagePort::OnBnClickedBtread)
	ON_BN_CLICKED(IDC_BtWrite, &CPagePort::OnBnClickedBtwrite)

	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT8, &CPagePort::OnEnChangeEditOutput)

END_MESSAGE_MAP()


// CPagePort message handlers

BOOL CPagePort::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	OnBnClickedBtread();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPagePort::OnApply()
{
	UpdateData(true);
	//g_pFrm->m_Robot->SetJetContrlParam(m_WorkMode,m_OpenTime,m_CloseTime,m_Count,m_pressJet,m_pressDisp);

	return CPropertyPage::OnApply();
}

void CPagePort::OnEnChangeEditOutput(UINT ID)
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UNREFERENCED_PARAMETER(ID);
	SetModified();
}

void CPagePort::OnBnClickedBtread()
{
	// TODO: Add your control notification handler code here
   //if(! g_pFrm->m_Robot->ReadJetContrlParam(&m_WorkMode,&m_OpenTime,&m_CloseTime,&m_Count,&m_pressJet,&m_pressDisp))
   //    MessageBox("喷射阀控制器通迅错误！");

	UpdateData(false);

}

void CPagePort::OnBnClickedBtwrite()
{
	// TODO: Add your control notification handler code here

	UpdateData(true);
    //g_pFrm->m_Robot->SetJetContrlParam(m_WorkMode,m_OpenTime,m_CloseTime,m_Count,m_pressJet,m_pressDisp);
		 //MessageBox("参数设定失败！");
	 //else
		 //MessageBox("参数设定成功！");
	Sleep(500);
	//g_pFrm->m_Robot->SetJetContrlParam(m_WorkMode,m_OpenTime,m_CloseTime,m_Count,m_pressJet,m_pressDisp);

    SetModified();

}

void CPagePort::OnEnChangeEdit1(UINT ID)
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	SetModified();
}
