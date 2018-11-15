// PageMotor.cpp : 实现文件
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "PageMotor.h"
#include "afxdialogex.h"


LPCSTR g_strMtrParamName[MOTOR_GRID_COL_NUM] = 
{_T("马达名称"), _T("序号"), _T("手动速度"), _T("最大速度"),_T("加速度"), _T("回零速度"),_T("回零低速"),
_T("回零加速度"),_T("回零脱离距离"), _T("回零方向"),_T("负极限"), _T("正  极  限"), _T("分  辨  率"), 
/*_T("导程"),_T("每圈脉冲数"), _T("马达类型"), */_T("回零搜索距离"), _T("运动方式"), _T("卡类型")};
// CPageMotor 对话框

IMPLEMENT_DYNAMIC(CPageMotor, CPropertyPage)

CPageMotor::CPageMotor()
	: CPropertyPage(CPageMotor::IDD)
{

}

CPageMotor::~CPageMotor()
{
}

void CPageMotor::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_DLG_MTR_GRID, m_MotorParamGrid);
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageMotor, CPropertyPage)
	ON_NOTIFY(NM_CLICK, IDC_DLG_MTR_GRID, OnGridClick)
	ON_BN_CLICKED(ID_SAVE, &CPageMotor::OnBnClickedSave)
END_MESSAGE_MAP()


// CPageMotor 消息处理程序


BOOL CPageMotor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CreateGrid();

// 	CRect rc;
// 	GetClientRect(&rc);
//	GetDlgItem(IDC_DLG_MTR_GRID)->MoveWindow(0,0,rc.right-rc.left-5,rc.bottom-rc.top);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPageMotor::CreateGrid()
{
	//只支持8位
	//m_ImageList.Create(MAKEINTRESOURCE(IDB_BITMAP_IOSTS), 28, 1, RGB(255,255,255));

	//nInitial用来确定图像列表包含的图像数量；nGrow用来确定图像列表可控制的图像数量
//	m_ImageList.Create(30,30,ILC_COLOR24|ILC_MASK,2,1);
// 	CBitmap a;
// 	a.LoadBitmap(IDB_BITMAP_IOSTS);		
// 	m_ImageList.Add(&a,RGB(255,255,255));

//	m_MotorParamGrid.SetImageList(&m_ImageList);//设置大图标控制

	m_MotorParamGrid.EnableDragAndDrop(FALSE);
	m_MotorParamGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_MotorParamGrid.SetEditable(TRUE);
	//OnVirtualMode();    // Sets the grid mode, fills the grid
	m_MotorParamGrid.SetListMode(TRUE);
	m_MotorParamGrid.SetTrackFocusCell(FALSE);
	//m_MotorParamGrid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
	m_MotorParamGrid.SetFrameFocusCell(FALSE);	//去除鼠标点选时的边框线  
// 	m_MotorParamGrid.SetColumnCount(0);			//清除默认设置的状态，重新创建	
// 	m_MotorParamGrid.SetRowCount(0);			//设置初始行数量，初始化只有一行
//	m_MotorParamGrid.SetSingleRowSelection();
	m_MotorParamGrid.SetColumnCount(MOTOR_GRID_COL_NUM);		//初始列数：第一列为序号，第二列为状态，第三列为详细说明
	m_MotorParamGrid.SetRowCount(MAX_AXIS_NUM+LS_AXIS_NUM+1);		//设置初始行数量，初始化只有一行
	m_MotorParamGrid.SetFixedRowCount(1);	

// 	CFont m_font2;
// 	m_font2.CreateFont(22,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,
// 			CLIP_STROKE_PRECIS,DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("Arial"));
// 	m_MotorParamGrid.SetFont(&m_font2);

	m_MotorParamGrid.SetFixedBkColor(RGB(128,128,192));
	m_MotorParamGrid.SetGridLines(GVL_BOTH);
	//序号头宽度
	for (int i=0; i<MOTOR_GRID_COL_NUM; i++)
	{
		m_MotorParamGrid.SetGridItemText(0,i,g_strMtrParamName[i]);
		//size_t len = strlen(g_strMtrParamName[i]);
		m_MotorParamGrid.ExpandColumnsToFit();
		//m_MotorParamGrid.SetColumnWidth(i,30);	//调整列宽
	}
	m_MotorParamGrid.AutoSizeColumns();
	//m_MotorParamGrid.ExpandLastColumn();

	m_MotorParamGrid.SetColumnResize(TRUE);
	m_MotorParamGrid.SetRowResize(FALSE);

	//调整行高
	for(UINT i=0;i<(MAX_AXIS_NUM+LS_AXIS_NUM+1);i++)
	{
		m_MotorParamGrid.SetRowHeight(i,32);	
	}

	//调整风格
	for (UINT i=1;i<(MAX_AXIS_NUM+LS_AXIS_NUM+1);i++)
	{
		for (UINT j=0;j<MOTOR_GRID_COL_NUM;j++)
		{	
			m_MotorParamGrid.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
			if (0==j||1==j||9==j||14==j||15==j)
			{	// 马达ID、运动类型、卡类型、回零方向不能更改
				m_MotorParamGrid.GetCell(i, j)->SetBackClr(RGB(192,192,192));
				m_MotorParamGrid.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
			}
		}
	}	

// 	m_MotorParamGrid.SetGridItemText(0,0,_T("端口号"));
// 	m_MotorParamGrid.SetGridItemText(0,1,_T("状态"));
// 	m_MotorParamGrid.SetGridItemText(0,2,_T("说明"));

	CString str;
	for(UINT i=1;i<(MAX_AXIS_NUM+1);i++)
	{
		int j=0;
		m_MotorParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamGts[i-1].m_MotorName);	j++;
		str.Format(_T("%d"),g_pFrm->m_mtrParamGts[i-1].m_MotorID);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_VhandMove);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_Vmax);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_Acc);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_Vhome);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_VhomeLow);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_AccHome);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_HomeReturn);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%d"),g_pFrm->m_mtrParamGts[i-1].m_HomeDir);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_MaxTravelNegative);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_MaxTravelPositive);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_Resolution);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;

		str.Format(_T("%.3f"),g_pFrm->m_mtrParamGts[i-1].m_HomeStep);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		m_MotorParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamGts[i-1].m_MoveMode);j++;
		m_MotorParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamGts[i-1].m_CardType);
	}	
	for(UINT i=MAX_AXIS_NUM+1;i<(MAX_AXIS_NUM+LS_AXIS_NUM+1);i++)
	{
		int j=0;
		m_MotorParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamLs[i-MAX_AXIS_NUM-1].m_MotorName);	j++;
		str.Format(_T("%d"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MotorID);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_VhandMove);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Vmax);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Acc);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Vhome);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_VhomeLow);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_AccHome);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_HomeReturn);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%d"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_HomeDir);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MaxTravelNegative);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MaxTravelPositive);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Resolution);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;

		str.Format(_T("%.3f"),g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_HomeStep);
		m_MotorParamGrid.SetGridItemText(i,j,str);	j++;
		m_MotorParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MoveMode);j++;
		m_MotorParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_CardType);
	}
}

void CPageMotor::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CPageMotor::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	// 固高卡参数保存
	for(UINT i=1;i<(MAX_AXIS_NUM+1);i++)
	{
		int j=0;
		g_pFrm->m_mtrParamGts[i-1].m_MotorName			= m_MotorParamGrid.GetItemText(i, j);			j++;
		g_pFrm->m_mtrParamGts[i-1].m_MotorID			= m_MotorParamGrid.GetItemDataInt(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_VhandMove				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_Vmax				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_Acc				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_Vhome				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_VhomeLow			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_AccHome			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_HomeReturn			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_HomeDir			= m_MotorParamGrid.GetItemDataInt(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_MaxTravelNegative  = m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_MaxTravelPositive  = m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_Resolution			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_HomeStep			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamGts[i-1].m_MoveMode			= m_MotorParamGrid.GetItemText(i, j);			j++;
		g_pFrm->m_mtrParamGts[i-1].m_CardType			= m_MotorParamGrid.GetItemText(i, j);			
	}	
	// 雷赛卡参数保存
	for(UINT i=MAX_AXIS_NUM+1;i<(MAX_AXIS_NUM+LS_AXIS_NUM+1);i++)
	{
		int j=0;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MotorName			= m_MotorParamGrid.GetItemText(i, j);			j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MotorID			= m_MotorParamGrid.GetItemDataInt(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_VhandMove				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Vmax				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Acc				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Vhome				= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_VhomeLow			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_AccHome			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_HomeReturn			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_HomeDir			= m_MotorParamGrid.GetItemDataInt(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MaxTravelNegative  = m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MaxTravelPositive  = m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_Resolution			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_HomeStep			= m_MotorParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_MoveMode			= m_MotorParamGrid.GetItemText(i, j);			j++;
		g_pFrm->m_mtrParamLs[i-1-MAX_AXIS_NUM].m_CardType			= m_MotorParamGrid.GetItemText(i, j);			
	}	
	g_pFrm->MotorParam(FALSE);
	AfxMessageBox("保存完成!");
}
