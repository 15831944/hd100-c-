// PageOut.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "PageOut.h"


// CPageOut dialog

IMPLEMENT_DYNAMIC(CPageOut, CPropertyPage)

CPageOut::CPageOut()
	: CPropertyPage(CPageOut::IDD)
{
	m_curSelTable = -1;
}

CPageOut::~CPageOut()
{
	
}

void CPageOut::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_IO_OUT_GRID, m_OutputGrid);
	DDX_Control(pDX, IDC_DLG_IO_OUT_GRID2, m_OutputGridEx);
}


BEGIN_MESSAGE_MAP(CPageOut, CPropertyPage)
	ON_NOTIFY(NM_DBLCLK, IDC_DLG_IO_OUT_GRID, OnGridDbClick)
	ON_NOTIFY(NM_DBLCLK, IDC_DLG_IO_OUT_GRID2, OnGridDbClickEx)
	ON_BN_CLICKED(IDC_DLG_IO_OUT_OPEN, &CPageOut::OnBnClickedDlgIoOutOpen)
	ON_BN_CLICKED(IDC_DLG_IO_OUT_CLOSE, &CPageOut::OnBnClickedDlgIoOutClose)
	ON_BN_CLICKED(IDC_DLG_IO_OUT_JET, &CPageOut::OnBnClickedDlgIoOutJet)
END_MESSAGE_MAP()


// CPageOut message handlers

BOOL CPageOut::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CreateGrid();
	CreateGridEx();

	ReadOutputMap();
	ReadOutputMapEx();

	UpdateIOSts(0xffff);
	UpdateIOStsEx(0xffff);

// 	CRect rc;
// 	GetClientRect(&rc);
// 	GetDlgItem(IDC_DLG_IO_OUT_GRID)->MoveWindow(0,0,(rc.right-rc.left)/2-50,rc.bottom-rc.top-55);
// 	m_OutputGrid.ExpandColumnsToFit();

	GetDlgItem(IDC_DLG_IO_OUT_OPEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_DLG_IO_OUT_CLOSE)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPageOut::OnVirtualMode()
{
	UpdateData();

	m_bVirtualMode = !m_bVirtualMode;
	m_OutputGrid.SetVirtualMode(m_bVirtualMode);

	m_bHeaderSort = m_OutputGrid.GetHeaderSort();
	m_bEditable = m_OutputGrid.IsEditable();

	if (m_bVirtualMode)
	{
		m_OutputGrid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);

		m_nFixCols = 1;
		m_nFixRows = 1;
		m_nCols = 100;
		m_nRows = 100000;

		TRY {
			m_OutputGrid.SetRowCount(m_nRows);
			m_OutputGrid.SetColumnCount(m_nCols);
			m_OutputGrid.SetFixedRowCount(m_nFixRows);
			m_OutputGrid.SetFixedColumnCount(m_nFixCols);
		}
		CATCH (CMemoryException, e)
		{
			e->ReportError();
			return;
		}
		END_CATCH
	}
	else
	{
		m_nFixCols = 1;
		m_nFixRows = 1;
		m_nCols = 8;
		m_nRows = 20;

		m_OutputGrid.SetEditable(m_bEditable);
		m_OutputGrid.SetAutoSizeStyle();

		TRY {
			m_OutputGrid.SetRowCount(m_nRows);
			m_OutputGrid.SetColumnCount(m_nCols);
			m_OutputGrid.SetFixedRowCount(m_nFixRows);
			m_OutputGrid.SetFixedColumnCount(m_nFixCols);
		}
		CATCH (CMemoryException, e)
		{
			e->ReportError();
			return;
		}
		END_CATCH

			// fill rows/cols with text
			for (int row = 0; row < m_OutputGrid.GetRowCount(); row++)
			{
				for (int col = 0; col < m_OutputGrid.GetColumnCount(); col++)
				{ 
					CString str;

					GV_ITEM Item;

					Item.mask = GVIF_TEXT;
					Item.row = row;
					Item.col = col;

					if (row < m_nFixRows)
						str.Format(_T("Column %d"),col);
					else if (col < m_nFixCols) 
						str.Format(_T("Row %d"), row);
					else 
						str.Format(_T("%d"),row*col);
					Item.strText = str;

					if (rand() % 10 == 1)
					{
						COLORREF clr = RGB(rand()%128 + 128, rand()%128 + 128, rand()%128 + 128);
						Item.crBkClr = clr;             // or - m_Grid.SetItemBkColour(row, col, clr);
						Item.crFgClr = RGB(255,0,0);    // or - m_Grid.SetItemFgColour(row, col, RGB(255,0,0));				    
						Item.mask    |= (GVIF_BKCLR|GVIF_FGCLR);
					}

					if (col < m_OutputGrid.GetFixedColumnCount())
					{
						Item.iImage = rand()%m_ImageList.GetImageCount();
						Item.mask  |= (GVIF_IMAGE);
					}

					m_OutputGrid.SetItem(&Item);
				}
			}
	}

	// Having weird Autosize problems with CE 2.11 - possibly just an emulation problem
#if !defined(_WIN32_WCE) || (_WIN32_WCE < 211)
	m_OutputGrid.AutoSize();
#endif
	UpdateData(FALSE);

}

BOOL CALLBACK CPageOut::GridCallback(GV_DISPINFO *pDispInfo, LPARAM /*lParam*/) 
{
	pDispInfo->item.strText.Format(_T("Callback %d,%d"),pDispInfo->item.row, pDispInfo->item.col);


	return TRUE;
}

void CPageOut::OnGridDbClickEx(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	m_curSelTable = 1;
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	TRACE(_T("Clicked on 扩展口 row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	CString str = m_OutputGridEx.GetItemText(pItem->iRow,2);
	int ibit = g_pFrm->m_Robot->ExOutputBitNameToNo(str);

	if((ibit<(OUT_POINT+EX_OUT_POINT))&&(ibit>=OUT_POINT))
	{
		if(g_pFrm->m_Robot->m_OutPutCtrl[ibit].m_IsOpen==FALSE)
		{
			short iRet = g_pFrm->m_Robot->ExOutput(str,TRUE);
			if (iRet!=RTN_NONE)	return;

			GetDlgItem(IDC_DLG_IO_OUT_OPEN)->EnableWindow(TRUE);
			GetDlgItem(IDC_DLG_IO_OUT_CLOSE)->EnableWindow(FALSE);
		}
		else
		{
			short iRet = g_pFrm->m_Robot->ExOutput(str,FALSE);
			if (iRet!=RTN_NONE)	return;

			GetDlgItem(IDC_DLG_IO_OUT_OPEN)->EnableWindow(FALSE);
			GetDlgItem(IDC_DLG_IO_OUT_CLOSE)->EnableWindow(TRUE);
		}
	}	
}

void CPageOut::OnGridDbClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	m_curSelTable = 0;
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	TRACE(_T("DbClicked on 输入口 row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	CString str = m_OutputGrid.GetItemText(pItem->iRow,2);
	int ibit = g_pFrm->m_Robot->ExOutputBitNameToNo(str);

	if((ibit<OUT_POINT)&&(ibit>=0))
	{
		if(((g_pFrm->m_Robot->m_outputSts&(0x0001<<ibit))==(0x0001<<ibit)))
		{
			short iRet = g_pFrm->m_Robot->ExOutput(str,TRUE);
			if (iRet!=RTN_NONE)	return;

			GetDlgItem(IDC_DLG_IO_OUT_OPEN)->EnableWindow(FALSE);
			GetDlgItem(IDC_DLG_IO_OUT_CLOSE)->EnableWindow(TRUE);
		}
		else
		{
			short iRet = g_pFrm->m_Robot->ExOutput(str,FALSE);
			if (iRet!=RTN_NONE)	return;

			GetDlgItem(IDC_DLG_IO_OUT_OPEN)->EnableWindow(TRUE);
			GetDlgItem(IDC_DLG_IO_OUT_CLOSE)->EnableWindow(FALSE);
		}
	}	
}

void CPageOut::CreateGridEx()
{
	m_OutputGridEx.SetImageList(&m_ImageList);//设置大图标控制

	m_OutputGridEx.EnableDragAndDrop(FALSE);
	m_OutputGridEx.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_OutputGridEx.SetEditable(FALSE);
	//	OnVirtualMode();    // Sets the grid mode, fills the grid
	m_OutputGridEx.SetListMode(TRUE);
	m_OutputGridEx.SetTrackFocusCell(FALSE);
	//	m_OutputGridEx.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
	m_OutputGridEx.SetFrameFocusCell(FALSE);	//去除鼠标点选时的边框线  
	m_OutputGridEx.SetSingleRowSelection();
	m_OutputGridEx.SetColumnCount(3);	//初始列数：第一列为序号，第二列为状态，第三列为详细说明
	m_OutputGridEx.SetRowCount(OUT_POINT/*+EX_OUT_POINT*/+1);		//设置初始行数量，初始化只有一行
	m_OutputGridEx.SetFixedRowCount(1);	
	m_OutputGridEx.SetFixedBkColor(RGB(128,128,192));
	m_OutputGridEx.SetGridLines(GVL_BOTH);

	m_OutputGridEx.SetGridItemText(0,0,_T("端  口  号 "));
	m_OutputGridEx.SetGridItemText(0,1,_T("状  态"));
	m_OutputGridEx.SetGridItemText(0,2,_T("说                       明"));
	//序号头宽度	
	m_OutputGridEx.ExpandColumnsToFit();
	m_OutputGridEx.AutoSizeColumns();

	m_OutputGridEx.SetColumnResize(TRUE);
	m_OutputGridEx.SetRowResize(FALSE);

	//调整行高
	for(UINT i=0;i<(IN_POINT/*+EX_IN_POINT*/+1);i++)
		m_OutputGridEx.SetRowHeight(i,INPUT_GRID_HEIGHT);	

	//调整风格
	  for (UINT i=0;i<(IN_POINT/*+EX_IN_POINT*/+1);i++){
		for (UINT j=0;j<INPUT_GRID_COL_NUM;j++)
  		m_OutputGridEx.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
	}	

	CString str;
	for(UINT i=1;i<(OUT_POINT/*+EX_OUT_POINT*/+1);i++)
	{
		str.Format(_T("扩展# %2d"),i-1);
		m_OutputGridEx.SetGridItemText(i,0,str);
	}
}


void CPageOut::CreateGrid()
{
	//只支持8位
	//m_ImageList.Create(MAKEINTRESOURCE(IDB_BITMAP_IOSTS), 28, 1, RGB(255,255,255));

	//nInitial用来确定图像列表包含的图像数量；nGrow用来确定图像列表可控制的图像数量
	m_ImageList.Create(30,30,ILC_COLOR24|ILC_MASK,2,1);
	CBitmap a;
	a.LoadBitmap(IDB_BITMAP_IOSTS);		
	m_ImageList.Add(&a,RGB(255,255,255));
	m_OutputGrid.SetImageList(&m_ImageList);//设置大图标控制

	m_OutputGrid.EnableDragAndDrop(FALSE);
	m_OutputGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_OutputGrid.SetEditable(FALSE);
//	OnVirtualMode();    // Sets the grid mode, fills the grid
	m_OutputGrid.SetListMode(TRUE);
	m_OutputGrid.SetTrackFocusCell(FALSE);
//	m_OutputGrid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
	m_OutputGrid.SetFrameFocusCell(FALSE);	//去除鼠标点选时的边框线  
	m_OutputGrid.SetSingleRowSelection();
	m_OutputGrid.SetColumnCount(3);	//初始列数：第一列为序号，第二列为状态，第三列为详细说明
	m_OutputGrid.SetRowCount(OUT_POINT/*+EX_OUT_POINT*/+1);		//设置初始行数量，初始化只有一行
	m_OutputGrid.SetFixedRowCount(1);	
	m_OutputGrid.SetFixedBkColor(RGB(128,128,192));
	m_OutputGrid.SetGridLines(GVL_BOTH);
	
	m_OutputGrid.SetGridItemText(0,0,_T("端  口  号 "));
	m_OutputGrid.SetGridItemText(0,1,_T("状  态"));
	m_OutputGrid.SetGridItemText(0,2,_T("说                       明"));
	//序号头宽度	
	m_OutputGrid.ExpandColumnsToFit();
	m_OutputGrid.AutoSizeColumns();

	m_OutputGrid.SetColumnResize(TRUE);
	m_OutputGrid.SetRowResize(FALSE);

	//调整行高
	for(UINT i=0;i<(IN_POINT/*+EX_IN_POINT*/+1);i++)
		m_OutputGrid.SetRowHeight(i,INPUT_GRID_HEIGHT);	

	//调整风格
	for (UINT i=0;i<(IN_POINT/*+EX_IN_POINT*/+1);i++){
		for (UINT j=0;j<INPUT_GRID_COL_NUM;j++)
			m_OutputGrid.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
	}	

	CString str;
	for(UINT i=1;i<(OUT_POINT/*+EX_OUT_POINT*/+1);i++)
	{
		str.Format(_T("输出# %2d"),i-1);
		m_OutputGrid.SetGridItemText(i,0,str);
	}
}


void CPageOut::UpdateIOStsEx(long sts)
{
	GV_ITEM Item;

	for (int i=0;i<OUT_POINT;i++)
	{
		Item.col = 1;
		Item.row = i+1;
		m_OutputGridEx.GetItem(&Item);

		if ((sts&IOstatusBitsMask[i])==IOstatusBitsMask[i])
			Item.iImage = 1;//输出低电平触发
		else
			Item.iImage = 0;	

		Item.mask  |= (GVIF_IMAGE);
		m_OutputGridEx.SetItem(&Item);		
	}
}

// 输出口刷新 板载直接获取 其他的根据 输出列表状态设置
void CPageOut::UpdateIOSts(long sts)
{
	GV_ITEM Item;

	for (int i=0;i<OUT_POINT;i++)
	{
		Item.col = 1;
		Item.row = i+1;
		m_OutputGrid.GetItem(&Item);

		if ((sts&IOstatusBitsMask[i])==IOstatusBitsMask[i])
			Item.iImage = 1;//输出低电平触发
		else
			Item.iImage = 0;	

		Item.mask  |= (GVIF_IMAGE);
		m_OutputGrid.SetItem(&Item);		
	}
}


void CPageOut::OnBnClickedDlgIoOutOpen()
{
	int row_click = 0;
	CString str = "";
	if (-1 == m_curSelTable)
	{
		return ;
	}
	else if (0 == m_curSelTable)
	{
	    row_click = m_OutputGrid.GetFocusCell().row;
		str = m_OutputGrid.GetItemText(row_click, 2);
	}
	else if (1 == m_curSelTable)
	{
		row_click = m_OutputGridEx.GetFocusCell().row;
		str = m_OutputGridEx.GetItemText(row_click,2);
	}

	short iRet = g_pFrm->m_Robot->ExOutput(str,TRUE);
	if (iRet!=RTN_NONE)	return;

	//更新列表状态
	GV_ITEM Item;
	Item.col = 1;
	Item.row = row_click;
	if (0 == m_curSelTable)
	{
		m_OutputGrid.GetItem(&Item); 
	}
	else if (1 == m_curSelTable)
	{
		m_OutputGridEx.GetItem(&Item);
	}

	Item.iImage = 0;
	Item.mask  |= (GVIF_IMAGE);

	if (0 == m_curSelTable)
	{
		m_OutputGrid.SetItem(&Item);
	}
	else if (1 == m_curSelTable)
	{
		m_OutputGridEx.SetItem(&Item);
	}

	GetDlgItem(IDC_DLG_IO_OUT_CLOSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_DLG_IO_OUT_OPEN)->EnableWindow(FALSE);
}

void CPageOut::OnBnClickedDlgIoOutClose()
{
	// TODO: Add your control notification handler code here
	int row_click = 0;
	CString str = "";
	if (-1 == m_curSelTable)
	{
		return ;
	}
	else if (0 == m_curSelTable)
	{
		row_click = m_OutputGrid.GetFocusCell().row;
		str = m_OutputGrid.GetItemText(row_click, 2);
	}
	else if (1 == m_curSelTable)
	{
		row_click = m_OutputGridEx.GetFocusCell().row;
		str = m_OutputGridEx.GetItemText(row_click,2);
	}

	short iRet = g_pFrm->m_Robot->ExOutput(str,FALSE);
	if (iRet!=RTN_NONE)	return;
	
	//更新列表状态
	GV_ITEM Item;
	Item.col = 1;
	Item.row = row_click;
	m_OutputGrid.GetItem(&Item); 

	Item.iImage=1;
	Item.mask  |= (GVIF_IMAGE);
	m_OutputGrid.SetItem(&Item);

	GetDlgItem(IDC_DLG_IO_OUT_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_DLG_IO_OUT_OPEN)->EnableWindow(TRUE);
}


void CPageOut::ReadOutputMapEx()
{
	CString strid,strinfo;	
	for (int j = OUT_POINT; j <(OUT_POINT+EX_OUT_POINT); j++)
	{ 
		if (g_pFrm->m_Robot->m_OutPutCtrl[j].m_nIOType==TYPE_OUTPUT)
		{
			strinfo=g_pFrm->m_Robot->m_OutPutCtrl[j].m_strIOName;
			m_OutputGridEx.SetGridItemText(j+1-OUT_POINT,2,strinfo);//说明
		}		
	} 
}

void CPageOut::ReadOutputMap()
{
	CString strid,strinfo;	
	for (int j = 0; j < (OUT_POINT); j++)
	{ 
		if (g_pFrm->m_Robot->m_OutPutCtrl[j].m_nIOType==TYPE_OUTPUT)
		{
			strinfo=g_pFrm->m_Robot->m_OutPutCtrl[j].m_strIOName;
			m_OutputGrid.SetGridItemText(j+1,2,strinfo);//说明
		}		
	} 
} 


// 手动出胶
void CPageOut::OnBnClickedDlgIoOutJet()
{
	if (g_pFrm->m_pSysParam->nJetPulse==0)//跟随模式
	{
		static bool bMyJetOn=false;
		if(bMyJetOn==false)
		{  
            if(g_pFrm->m_pSysParam->nToolType == 0)
			   g_pFrm->m_Robot->JetOutput(1,TRUE);
			bMyJetOn = true;
		}
		else
		{
		    if(g_pFrm->m_pSysParam->nToolType == 0)
			    g_pFrm->m_Robot->JetOutput(1,FALSE);

			bMyJetOn = false;
		}
	}
	else
	{
		if(g_pFrm->m_pSysParam->nToolType == 0)
		    g_pFrm->m_Robot->JetOutput(1,TRUE);
	}
}
