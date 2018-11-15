// DlgParamLine.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgParamLine.h"
#include "afxdialogex.h"
#include "TSCtrlSys.h"

// CDlgParamLine 对话框
LPCSTR g_strLineParamName[LINE_GRID_COL_NUM] = 
{_T("参数名称"), _T("序号"), _T("停留时间"), _T("Z轴下降速度"),_T("Z轴下降加速度"),
_T("点胶高度"),_T("点胶线段速度"),_T("提前关胶距离"),_T("回吸时间"),_T("点胶后停留时间"),
_T("避开上升高度"),_T("避开抬高高度"), _T("回走距离"),
_T("避开上升速度"),_T("避开上升加速度")};

IMPLEMENT_DYNAMIC(CDlgParamLine, CDialogEx)

CDlgParamLine::CDlgParamLine(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgParamLine::IDD, pParent)
{

}

CDlgParamLine::~CDlgParamLine()
{
}

void CDlgParamLine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_PARAM_LINE_GRID, m_LineParamGrid);
}


BEGIN_MESSAGE_MAP(CDlgParamLine, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_DLG_PARAM_LINE_GRID, OnGridClick)
	ON_BN_CLICKED(IDC_BTN_PARAM_LINE_SAVE, &CDlgParamLine::OnBnClickedBtnParamLineSave)
END_MESSAGE_MAP()


// CDlgParamLine 消息处理程序


BOOL CDlgParamLine::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CreateGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgParamLine::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CDlgParamLine::CreateGrid()
{
	m_LineParamGrid.EnableDragAndDrop(FALSE);
	m_LineParamGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_LineParamGrid.SetEditable(TRUE);
	//OnVirtualMode();    // Sets the grid mode, fills the grid
	m_LineParamGrid.SetListMode(TRUE);
	m_LineParamGrid.SetTrackFocusCell(FALSE);
	//m_MotorParamGrid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
	m_LineParamGrid.SetFrameFocusCell(FALSE);	//去除鼠标点选时的边框线  
	// 	m_MotorParamGrid.SetColumnCount(0);			//清除默认设置的状态，重新创建	
	// 	m_MotorParamGrid.SetRowCount(0);			//设置初始行数量，初始化只有一行
	//	m_MotorParamGrid.SetSingleRowSelection();
	m_LineParamGrid.SetColumnCount(LINE_GRID_COL_NUM);			// 初始列数：第一列为序号，第二列为状态，第三列为详细说明
	m_LineParamGrid.SetRowCount(LINE_GRID_HEIGHT+1);		//设置初始行数量，初始化只有一行
	m_LineParamGrid.SetFixedRowCount(1);	

	m_LineParamGrid.SetFixedBkColor(RGB(128,128,192));
	m_LineParamGrid.SetGridLines(GVL_BOTH);
	//序号头宽度
	for (int i=0; i<LINE_GRID_COL_NUM; i++)
	{
		m_LineParamGrid.SetGridItemText(0,i,g_strLineParamName[i]);
		//size_t len = strlen(g_strMtrParamName[i]);
		m_LineParamGrid.ExpandColumnsToFit();
		//m_MotorParamGrid.SetColumnWidth(i,30);	//调整列宽
	}
	m_LineParamGrid.AutoSizeColumns();
	//m_MotorParamGrid.ExpandLastColumn();

	m_LineParamGrid.SetColumnResize(TRUE);
	m_LineParamGrid.SetRowResize(FALSE);

	//调整行高
	for(UINT i=0;i<(LINE_GRID_HEIGHT+1);i++)
	{
		m_LineParamGrid.SetRowHeight(i,32);	
	}

	//调整风格
	for (UINT i=1;i<(LINE_GRID_HEIGHT+1);i++)
	{
		for (UINT j=0;j<LINE_GRID_COL_NUM;j++)
		{	
			m_LineParamGrid.SetItemState(i,j,/*GVIS_READONLY|*/GVIS_DROPHILITED);
			if (0==j||1==j/*||9==j||14==j||15==j*/)
			{	// 参数ID、参数类型不能更改
				m_LineParamGrid.GetCell(i, j)->SetBackClr(RGB(192,192,192));
				m_LineParamGrid.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
			}
			if (2==j||3==j||4==j)
			{
				m_LineParamGrid.GetCell(i, j)->SetBackClr(RGB(191,188,248));
			}
			
			if (5==j||6==j||7==j||8==j||9==j)
			{
				m_LineParamGrid.GetCell(i, j)->SetBackClr(RGB(143,138,242));
			}
		}
	}	

	// 	m_MotorParamGrid.SetGridItemText(0,0,_T("端口号"));
	// 	m_MotorParamGrid.SetGridItemText(0,1,_T("状态"));
	// 	m_MotorParamGrid.SetGridItemText(0,2,_T("说明"));

	CString str;
	for(UINT i=1;i<(MAX_LINE_PARAM_NUM+1);i++)
	{
		int j=0;
		m_LineParamGrid.SetGridItemText(i,j,g_pFrm->m_LineParm[i-1].strName);	j++;
		str.Format(_T("%d"),g_pFrm->m_LineParm[i-1].nTypeID);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfPremoveDelay);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfDownSpeed);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfDownAccel);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfDispenseGap);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfSpeed);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfShutOffDistance);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfSuckback);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfDwellSecs);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfRetractDistance);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfBacktrackGap);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfBacktrackLength);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfRetractSpeed);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
		str.Format(_T("%.3f"),g_pFrm->m_LineParm[i-1].dfRetractAccel);
		m_LineParamGrid.SetGridItemText(i,j,str);	j++;
	}
}

void CDlgParamLine::OnBnClickedBtnParamLineSave()
{
	CString str;
	// 线参数保存
	for(UINT i=1;i<(MAX_LINE_PARAM_NUM+1);i++)
	{
		int j=0;
		g_pFrm->m_LineParm[i-1].strName			= m_LineParamGrid.GetItemText(i, j);			j++;
		g_pFrm->m_LineParm[i-1].nTypeID			= m_LineParamGrid.GetItemDataInt(i, j);		j++;
		
		g_pFrm->m_LineParm[i-1].dfPremoveDelay				= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfDownSpeed				= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfDownAccel				= m_LineParamGrid.GetItemDataDouble(i, j);		j++;

		g_pFrm->m_LineParm[i-1].dfDispenseGap				= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfSpeed			= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfShutOffDistance			= m_LineParamGrid.GetItemDataDouble(i, j);		j++;

		g_pFrm->m_LineParm[i-1].dfSuckback			= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfDwellSecs			= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfRetractDistance  = m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfBacktrackGap  = m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfBacktrackLength			= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfRetractSpeed			= m_LineParamGrid.GetItemDataDouble(i, j);		j++;
		g_pFrm->m_LineParm[i-1].dfRetractAccel			= m_LineParamGrid.GetItemDataDouble(i, j);			j++;
		
	}	

	g_pFrm->DotParam(FALSE);
	AfxMessageBox("保存完成!");
}
