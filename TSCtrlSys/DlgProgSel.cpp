// DlgProgSel.cpp : 实现文件
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "DlgProgSel.h"
#include "afxdialogex.h"

const static UINT GridColumnNum = 4;
// CDlgProgSel 对话框

IMPLEMENT_DYNAMIC(CDlgProgSel, CDialogEx)

CDlgProgSel::CDlgProgSel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgProgSel::IDD, pParent)
{

}

CDlgProgSel::~CDlgProgSel()
{
}

void CDlgProgSel::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_PROG_LIST, m_progListGrid);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgProgSel, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_PROG_LIST, OnGridClick)
	ON_BN_CLICKED(IDC_BTN_SEL, &CDlgProgSel::OnBnClickedBtnSel)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgProgSel::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDlgProgSel::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDOK, &CDlgProgSel::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgProgSel 消息处理程序
BOOL CDlgProgSel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CreateGrid();

	SetDlgItemText(IDC_EDIT_CUR_PROG, g_AllProgInfo[0].m_curProgName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgProgSel::CreateGrid()
{
	m_progListGrid.EnableDragAndDrop(FALSE);
	m_progListGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_progListGrid.SetEditable(TRUE);
	m_progListGrid.SetListMode(TRUE);
	m_progListGrid.SetTrackFocusCell(FALSE);
	m_progListGrid.SetFrameFocusCell(FALSE);	//去除鼠标点选时的边框线  
	m_progListGrid.SetColumnCount(GridColumnNum);		//初始列数：第一列为序号，第二列为状态，第三列为详细说明
	m_progListGrid.SetRowCount(g_AllProgInfo[0].m_nProgNum+1);		//设置初始行数量，初始化只有一行
	m_progListGrid.SetFixedRowCount(1);	
	m_progListGrid.SetFixedBkColor(RGB(128,128,192));
	m_progListGrid.SetGridLines(GVL_BOTH);

	//序号头宽度
	m_progListGrid.SetGridItemText(0, 0, "   产   品   名   称   ");
	m_progListGrid.SetGridItemText(0, 1, "   创      建      时      间   ");
	m_progListGrid.SetGridItemText(0, 2, "   产      品      描      述   ");
	m_progListGrid.SetGridItemText(0, 3, "   上   次   调   用   时   间   ");
	m_progListGrid.ExpandColumnsToFit();
	m_progListGrid.AutoSizeColumns();
	//m_progListGrid.SetColumnWidth(i,30);	//调整列宽

	m_progListGrid.ExpandLastColumn();

	m_progListGrid.SetColumnResize(TRUE);
	m_progListGrid.SetRowResize(FALSE);

	//调整行高
	for(int i=0;i<(g_AllProgInfo[0].m_nProgNum+1);i++)
	{
		m_progListGrid.SetRowHeight(i,32);	
	}

	//调整风格
	for (int i=1;i<(g_AllProgInfo[0].m_nProgNum+1);i++)
	{
		for (UINT j=0;j<GridColumnNum;j++)
		{	
			m_progListGrid.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
		}
	}	

	CString str;
	for(int i=1;i<(g_AllProgInfo[0].m_nProgNum+1);i++)
	{
		POSITION ps;
		ps = g_AllProgInfo[0].m_listAllProgInfo.FindIndex(i-1);
		tgStructProgInfo tmpInfo = g_AllProgInfo[0].m_listAllProgInfo.GetAt(ps);

		str = tmpInfo.m_strProgName;
		m_progListGrid.SetGridItemText(i, 0, str);
		str = tmpInfo.m_strCreateTime;
		m_progListGrid.SetGridItemText(i, 1, str);
		str = tmpInfo.m_strDiscription;
		m_progListGrid.SetGridItemText(i, 2, str);
		str = tmpInfo.m_strLastSelect;
		m_progListGrid.SetGridItemText(i, 3, str);
	}	
}

void CDlgProgSel::UpdataGrid()
{	
	m_progListGrid.SetRowCount(g_AllProgInfo[0].m_nProgNum+1);		//设置初始行数量，初始化只有一行

	//调整风格
	for (int i=1;i<(g_AllProgInfo[0].m_nProgNum+1);i++)
	{
		for (UINT j=0;j<GridColumnNum;j++)
		{	
			m_progListGrid.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
		}
	}	

	CString str;
	for(int i=1;i<(g_AllProgInfo[0].m_nProgNum+1);i++)
	{
		POSITION ps;
		ps = g_AllProgInfo[0].m_listAllProgInfo.FindIndex(i-1);
		m_progListGrid.SetGridItemText(i, 0, g_AllProgInfo[0].m_listAllProgInfo.GetAt(ps).m_strProgName);
		m_progListGrid.SetGridItemText(i, 1, g_AllProgInfo[0].m_listAllProgInfo.GetAt(ps).m_strCreateTime);
		m_progListGrid.SetGridItemText(i, 2, g_AllProgInfo[0].m_listAllProgInfo.GetAt(ps).m_strDiscription);
		m_progListGrid.SetGridItemText(i, 3, g_AllProgInfo[0].m_listAllProgInfo.GetAt(ps).m_strLastSelect);
	}	

	SetDlgItemText(IDC_EDIT_CUR_PROG, g_AllProgInfo[0].m_curProgName);
}

void CDlgProgSel::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	TRACE(_T("Clicked on 输入口 row %d, col %d\n"), pItem->iRow, pItem->iColumn);

// 	CString str = m_progListGrid.GetItemText(pItem->iRow,0);
// 	g_AllProgInfo[0].Select(str);
}

void CDlgProgSel::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strName = "";
	GetDlgItemText(IDC_EDIT_ADD_PROG, strName);
	if ("" == strName)
	{
		g_pFrm->m_Robot->AfxMessageBox("产品名称不能为空!!!");
		return;
	}

	CString strDisc = "";
	GetDlgItemText(IDC_EDIT_ADD_PROG_DISCRIPTION, strDisc);
	g_AllProgInfo[0].Add(strName, strDisc);
	UpdataGrid();
}

void CDlgProgSel::OnBnClickedBtnSel()
{
	// TODO: 在此添加控件通知处理程序代码
	int rowClick = m_progListGrid.GetFocusCell().row;
	if (rowClick < 1)						return ;

	CString strProgName = m_progListGrid.GetItemText(rowClick, 0);
	if ("" == strProgName)
	{
		return;
	}

	g_AllProgInfo[0].Select(strProgName);
	UpdataGrid();
}

void CDlgProgSel::OnBnClickedBtnDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int rowClick = m_progListGrid.GetFocusCell().row;
	if (rowClick < 1)
	{
		g_pFrm->m_Robot->AfxMessageBox(_T("没有选中要删除的程序!"));
		return ;
	}

	CString strProgName = m_progListGrid.GetItemText(rowClick, 0);
	if ("" == strProgName)
	{
		return;
	}

	if (IDOK != g_pFrm->m_Robot->AfxMessageBox(_T("是否要删除程序？删除后将无法恢复!"), MB_OKCANCEL))
	{
		return;
	}

	g_AllProgInfo[0].Delete(strProgName);
	UpdataGrid();
}


void CDlgProgSel::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;

	g_AllProgInfo[0].m_listAllProgInfo.RemoveAll();
	for(int i=1;i<(g_AllProgInfo[0].m_nProgNum+1);i++)
	{
		tgStructProgInfo tmpInfo;
		strTemp = m_progListGrid.GetItemText(i, 0);
		tmpInfo.m_strProgName = strTemp;
		strTemp = m_progListGrid.GetItemText(i, 1);
		tmpInfo.m_strCreateTime = strTemp;
		strTemp = m_progListGrid.GetItemText(i, 2);
		tmpInfo.m_strDiscription = strTemp;

		g_AllProgInfo[0].m_listAllProgInfo.AddTail(tmpInfo);
	}

	g_AllProgInfo[0].HandleAllProg(FALSE);
	g_ProgData.LoadProgData();

	CDialogEx::OnOK();
}
