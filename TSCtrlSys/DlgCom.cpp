// DlgComConfig.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgCom.h"


// CDlgComConfig dialog

IMPLEMENT_DYNAMIC(CDlgComConfig, CDialog)

CDlgComConfig::CDlgComConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgComConfig::IDD, pParent)
{

}

CDlgComConfig::~CDlgComConfig()
{
}

void CDlgComConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgComConfig, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgComConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgComConfig message handlers

void CDlgComConfig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CDlgComConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
