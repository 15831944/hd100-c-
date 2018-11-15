// DlgBalanceOp.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgBalanceOp.h"


// CDlgBalanceOp dialog

IMPLEMENT_DYNAMIC(CDlgBalanceOp, CPropertyPage)

CDlgBalanceOp::CDlgBalanceOp()
	: CPropertyPage(CDlgBalanceOp::IDD)
{
	
}

CDlgBalanceOp::~CDlgBalanceOp()
{
}

void CDlgBalanceOp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_DATATOTOL, m_listBC);
}


BEGIN_MESSAGE_MAP(CDlgBalanceOp, CPropertyPage)
	ON_BN_CLICKED(IDC_BT_BC_GETDATA2, &CDlgBalanceOp::OnBnClickedBtBcGetdata2)
	ON_BN_CLICKED(IDC_BT_BANLENCE_ZERO2, &CDlgBalanceOp::OnBnClickedBtBanlenceZero2)
	ON_BN_CLICKED(IDC_BT_BANLENCE_ZERO, &CDlgBalanceOp::OnBnClickedBtBanlenceZero)
	ON_BN_CLICKED(IDC_BT_BC_GETDATA3, &CDlgBalanceOp::OnBnClickedBtBcGetdata3)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgBalanceOp::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgBalanceOp::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgBalanceOp::OnBnClickedButton1)
END_MESSAGE_MAP()



BOOL CDlgBalanceOp::OnInitDialog()
{
	CDialog::OnInitDialog();

    m_listBC.SetExtendedStyle(m_listBC.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listBC.InsertColumn(0, "序号", LVCFMT_CENTER, 50);
	m_listBC.InsertColumn(1, "空板重量(g)", LVCFMT_RIGHT, 132);
	m_listBC.InsertColumn(2, "满板重量(g)", LVCFMT_RIGHT, 132);
    m_listBC.InsertColumn(3, "油墨重量(g)", LVCFMT_RIGHT, 132);
	UpdateBClist(g_pFrm->m_Robot->m_WCBatetID);

	return TRUE;  
}
void CDlgBalanceOp::UpdateBClist(int num)
{  
	int nRow;
	CString strID,strdate;
	if(num>MAX_WCDATE)
		num = MAX_WCDATE;

	for (int i=0;i<num;i++)
	{
		strID.Format(_T("%3d#"),i);
        nRow= m_listBC.InsertItem(0, strID);//插入行
    
        strdate.Format(_T("%.3f"),g_pFrm->m_BatetWCDate[i].dWCDate_0);
	    m_listBC.SetItemText(nRow, 1, strdate);//数据

		 strdate.Format(_T("%.3f"),g_pFrm->m_BatetWCDate[i].dWCDate_1);
	    m_listBC.SetItemText(nRow, 2, strdate);//数据

		strdate.Format(_T("%.3f"),g_pFrm->m_BatetWCDate[i].dWCDate_1-g_pFrm->m_BatetWCDate[i].dWCDate_0);
		m_listBC.SetItemText(nRow, 3, strdate);//出胶数据
	}
	
}
// 电子称2 去皮操作
void CDlgBalanceOp::OnBnClickedBtBanlenceZero2()
{
//	 theApp.m_Serial.BSMBalanceTareEx();

	  CString str = _T("--0---");
	  SetDlgItemText(IDC_STATIC_BCDATA2,str);// 状态
}

void CDlgBalanceOp::OnBnClickedBtBanlenceZero()
{
//	theApp.m_Serial.BSMBalanceTare();

	CString str = _T("--0---");
	SetDlgItemText(IDC_STATIC_BCDATA,str);// 状态
}

// 获取下料电子称数据
void CDlgBalanceOp::OnBnClickedBtBcGetdata3()
{
// 	CString str;
// 	double  date;
// 	BOOL bIsS;

// 	bIsS = theApp.m_Serial.ReadBSMBalanceEx(date);
// 	if(bIsS == TRUE)
// 	{
// 		str.Format(_T("%.3f"),date);
// 	}
// 	else
// 		str="-Error-";

//	SetDlgItemText(IDC_STATIC_BCDATA2,str);// 状态
}

// 获取上料电子称数据
void CDlgBalanceOp::OnBnClickedBtBcGetdata2()
{
// 	CString str;
// 	double  date;
// 	BOOL bIsS;

// 	bIsS = theApp.m_Serial.ReadBSMBalance(date);
//     if(bIsS == TRUE)
// 	{
//        str.Format(_T("%.3f"),date);
// 	}
// 	else
//       str="-Error-";

//	SetDlgItemText(IDC_STATIC_BCDATA,str);// 状态
}

void CDlgBalanceOp::OnBnClickedButton6()
{
	g_pFrm->m_Robot->m_WCBatetID  = 0;
	g_pFrm->m_Robot->m_WCBatetID2  = 0;
}

void CDlgBalanceOp::OnBnClickedButton4()
{   
	CString strFile, strTemp, strKey;
	int num = 0;
	for(int i=0;i<MAX_WCDATE;i++)
	{
		if(g_pFrm->m_BatetWCDate[i].ID != -1)
			num++;
	}

	CFileDialog fd(FALSE, ".ts", NULL, 4|2, "*.ts|*.ts||");
	if(fd.DoModal() == IDOK)
	{
		strFile = fd.GetPathName();
		CFunction::HandleInt(FALSE, "MAX_NUM", "NUM", num, strFile); // 个数
	}

	for( int j=0;j<num;j++)
	{
		strKey.Format(_T("%3d#"),j);
		CFunction::HandleInt(FALSE,strKey,"BC_ID",g_pFrm->m_BatetWCDate[j].ID,strFile);
		CFunction::HandleDouble(FALSE,strKey,"BC_0",g_pFrm->m_BatetWCDate[j].dWCDate_0,strFile);
        CFunction::HandleDouble(FALSE,strKey,"BC_1",g_pFrm->m_BatetWCDate[j].dWCDate_1,strFile);
		g_pFrm->m_BatetWCDate[j].reset();
	}

	m_listBC.DeleteAllItems();

   
}
// 导入
void CDlgBalanceOp::OnBnClickedButton1()
{ 
    CString strFile,strKey;
	int num=1;
	CFileDialog fd(TRUE, ".ts", NULL, 4|2, "*.ts|*.ts||");
	if(fd.DoModal() == IDOK)
	{
	    strFile = fd.GetPathName();
        CFunction::HandleInt(TRUE, "MAX_NUM", "NUM", num, strFile); // 个数
	}
     m_listBC.DeleteAllItems();
	for( int j=0;j<num;j++)
	{
		strKey.Format(_T("%3d#"),j);
		CFunction::HandleInt(TRUE,strKey,"BC_ID",g_pFrm->m_BatetWCDate[j].ID,strFile);
		CFunction::HandleDouble(TRUE,strKey,"BC_0",g_pFrm->m_BatetWCDate[j].dWCDate_0,strFile);
		CFunction::HandleDouble(TRUE,strKey,"BC_1",g_pFrm->m_BatetWCDate[j].dWCDate_1,strFile);
	}
	UpdateBClist(num);

}
