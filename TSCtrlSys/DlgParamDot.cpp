// DlgParamDot.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgParamDot.h"
#include "afxdialogex.h"
#include "TSCtrlSys.h"

// CDlgParamDot 对话框

LPCSTR g_strDotParamName[DOT_GRID_COL_NUM] = 
{_T("参数名称"), _T("序号"), _T("停留时间"), _T("Z轴下降速度"),_T("Z轴下降加速度"),
_T("开阀时间"),_T("点胶高度"),_T("打点个数"),
_T("点胶后停留时间"), _T("避开上升高度"),_T("上升速度"), _T("上升加速度"),
_T("回吸时间")};

IMPLEMENT_DYNAMIC(CDlgParamDot, CDialogEx)

CDlgParamDot::CDlgParamDot(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgParamDot::IDD, pParent)
{

}

CDlgParamDot::~CDlgParamDot()
{
}

void CDlgParamDot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_PARAM_DOT_GRID, m_DotParamGrid);
}


BEGIN_MESSAGE_MAP(CDlgParamDot, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgParamDot::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_DLG_PARAM_DOT_GRID, OnGridClick)
	ON_BN_CLICKED(IDC_BTN_PARAM_DOT_SAVE, &CDlgParamDot::OnBnClickedBtnParamDotSave)
END_MESSAGE_MAP()


// CDlgParamDot 消息处理程序


BOOL CDlgParamDot::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CreateGrid();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgParamDot::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码



	CDialogEx::OnOK();
}

void CDlgParamDot::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CDlgParamDot::CreateGrid()
{
	//只支持8位
	//m_ImageList.Create(MAKEINTRESOURCE(IDB_BITMAP_IOSTS), 28, 1, RGB(255,255,255));

	//nInitial用来确定图像列表包含的图像数量；nGrow用来确定图像列表可控制的图像数量
	//	m_ImageList.Create(30,30,ILC_COLOR24|ILC_MASK,2,1);
	// 	CBitmap a;
	// 	a.LoadBitmap(IDB_BITMAP_IOSTS);		
	// 	m_ImageList.Add(&a,RGB(255,255,255));

	//	m_MotorParamGrid.SetImageList(&m_ImageList);//设置大图标控制



	m_DotParamGrid.EnableDragAndDrop(FALSE);
	m_DotParamGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_DotParamGrid.SetEditable(TRUE);
	//OnVirtualMode();    // Sets the grid mode, fills the grid
	m_DotParamGrid.SetListMode(TRUE);
	m_DotParamGrid.SetTrackFocusCell(FALSE);
	//m_MotorParamGrid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
	m_DotParamGrid.SetFrameFocusCell(FALSE);	//去除鼠标点选时的边框线  
	// 	m_MotorParamGrid.SetColumnCount(0);			//清除默认设置的状态，重新创建	
	// 	m_MotorParamGrid.SetRowCount(0);			//设置初始行数量，初始化只有一行
	//	m_MotorParamGrid.SetSingleRowSelection();
	m_DotParamGrid.SetColumnCount(DOT_GRID_COL_NUM);			// 初始列数：第一列为序号，第二列为状态，第三列为详细说明
	m_DotParamGrid.SetRowCount(DOT_GRID_HEIGHT+1);				// 设置初始行数量，初始化只有一行
	m_DotParamGrid.SetFixedRowCount(1);	

	m_DotParamGrid.SetFixedBkColor(RGB(128,128,192));
	m_DotParamGrid.SetGridLines(GVL_BOTH);
	//序号头宽度
	for (int i=0; i<DOT_GRID_COL_NUM; i++)
	{
		m_DotParamGrid.SetGridItemText(0,i,g_strDotParamName[i]);
		//size_t len = strlen(g_strMtrParamName[i]);
		m_DotParamGrid.ExpandColumnsToFit();
		//m_MotorParamGrid.SetColumnWidth(i,30);	//调整列宽
	}
	m_DotParamGrid.AutoSizeColumns();
	//m_MotorParamGrid.ExpandLastColumn();

	m_DotParamGrid.SetColumnResize(TRUE);
	m_DotParamGrid.SetRowResize(FALSE);

	//调整行高
	for(UINT i=0;i<(DOT_GRID_HEIGHT+1);i++)
	{
		m_DotParamGrid.SetRowHeight(i,32);	
	}

	//调整风格
	for (UINT i=1;i<(DOT_GRID_HEIGHT+1);i++)
	{
		for (UINT j=0;j<DOT_GRID_COL_NUM;j++)
		{	
			m_DotParamGrid.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
			if (0==j||1==j/*||9==j||14==j||15==j*/)
			{	// 参数ID、参数类型、卡类型、回零方向不能更改
				m_DotParamGrid.GetCell(i, j)->SetBackClr(RGB(192,192,192));
				m_DotParamGrid.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
			}
			if (2==j||3==j||4==j)
			{
				m_DotParamGrid.GetCell(i, j)->SetBackClr(RGB(230,230,230));
			}
			if (5==j||6==j||7==j)
			{
				m_DotParamGrid.GetCell(i, j)->SetBackClr(RGB(143,138,242));
			}
		}
	}	

	// 	m_MotorParamGrid.SetGridItemText(0,0,_T("端口号"));
	// 	m_MotorParamGrid.SetGridItemText(0,1,_T("状态"));
	// 	m_MotorParamGrid.SetGridItemText(0,2,_T("说明"));

	CString str;
	for(UINT i=1;i<(MAX_DOT_PARAM_NUM+1);i++)
	{
		int j=0;
		m_DotParamGrid.SetGridItemText(i,j,g_pFrm->m_DotParam[i-1].strName);	j++;
		str.Format(_T("%d"),g_pFrm->m_DotParam[i-1].nTypeID);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfSettlingTime);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfDownSpeed);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfDownAccel);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfOnTime);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfDispenseGap);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfNumShots);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfDwellSecs);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfRetractDistance);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfRetractSpeed);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfRetractAccel);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_DotParam[i-1].dfSuckback);
		m_DotParamGrid.SetGridItemText(i,j,str);	j++;

//		m_DotParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamGts[i-1].m_MoveMode);j++;
//		m_DotParamGrid.SetGridItemText(i,j,g_pFrm->m_mtrParamGts[i-1].m_CardType);
	}
}

void CDlgParamDot::OnBnClickedBtnParamDotSave()
{
	CString str;
	// 点参数保存
	for(UINT i=1;i<(MAX_DOT_PARAM_NUM+1);i++)
	{
		int j=0;
		g_pFrm->m_DotParam[i-1].strName			= m_DotParamGrid.GetItemText(i, j);			j++;
		g_pFrm->m_DotParam[i-1].nTypeID			= m_DotParamGrid.GetItemDataInt(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfSettlingTime	= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfDownSpeed				= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfDownAccel				= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		
		g_pFrm->m_DotParam[i-1].dfOnTime				= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfDispenseGap			= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfNumShots			= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		
		g_pFrm->m_DotParam[i-1].dfDwellSecs			= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfRetractDistance			= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfRetractSpeed  = m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfRetractAccel  = m_DotParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_DotParam[i-1].dfSuckback			= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
//		g_pFrm->m_DotParam[i-1].m_HomeStep			= m_DotParamGrid.GetItemDataDouble(i, j);		j++;
//		g_pFrm->m_DotParam[i-1].m_MoveMode			= m_DotParamGrid.GetItemText(i, j);			j++;
//		g_pFrm->m_DotParam[i-1].m_CardType			= m_DotParamGrid.GetItemText(i, j);			
	}	

	g_pFrm->DotParam(FALSE);
	GetDlgItem(IDC_STATIC_PARAM_DOT)->SetWindowText(_T("保存完成"));
}
