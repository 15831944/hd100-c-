// PageIn.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "PageIn.h"


// CPageIn dialog

IMPLEMENT_DYNAMIC(CPageIn, CPropertyPage)

CPageIn::CPageIn()
	: CPropertyPage(CPageIn::IDD)
{

}

CPageIn::~CPageIn()
{
	

}

void CPageIn::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_IO_IN_GRID, m_InputGrid);
	DDX_Control(pDX, IDC_DLG_IO_IN_GRID2, m_ExInputGrid);
}


BEGIN_MESSAGE_MAP(CPageIn, CPropertyPage)
	ON_NOTIFY(NM_CLICK, IDC_DLG_IO_IN_GRID, OnGridClick)
END_MESSAGE_MAP()


// CPageIn message handlers

BOOL CPageIn::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CreateGrid();
	CreateGridEx();

	ReadInputMap();
	ReadExInputMap();

	UpdateIOSts(0);
	UpdateExIOSts(0);

// 	CRect rc;
// 	GetClientRect(&rc);
// 	GetDlgItem(IDC_DLG_IO_IN_GRID)->MoveWindow(0,0,(rc.right-rc.left)/2-50,rc.bottom-rc.top-55);
// 	m_InputGrid.ExpandColumnsToFit();
   
//	SetTimer(0, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPageIn::OnVirtualMode()
{
	UpdateData();

	m_bVirtualMode = !m_bVirtualMode;
	m_InputGrid.SetVirtualMode(m_bVirtualMode);

	m_bHeaderSort = m_InputGrid.GetHeaderSort();
	m_bEditable = m_InputGrid.IsEditable();

	if (m_bVirtualMode)
	{
		m_InputGrid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);

		m_nFixCols = 1;
		m_nFixRows = 1;
		m_nCols = 100;
		m_nRows = 100000;

		TRY {
			m_InputGrid.SetRowCount(m_nRows);
			m_InputGrid.SetColumnCount(m_nCols);
			m_InputGrid.SetFixedRowCount(m_nFixRows);
			m_InputGrid.SetFixedColumnCount(m_nFixCols);
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

		m_InputGrid.SetEditable(m_bEditable);
		m_InputGrid.SetAutoSizeStyle();

		TRY {
			m_InputGrid.SetRowCount(m_nRows);
			m_InputGrid.SetColumnCount(m_nCols);
			m_InputGrid.SetFixedRowCount(m_nFixRows);
			m_InputGrid.SetFixedColumnCount(m_nFixCols);
		}
		CATCH (CMemoryException, e)
		{
			e->ReportError();
			return;
		}
		END_CATCH

			// fill rows/cols with text
			for (int row = 0; row < m_InputGrid.GetRowCount(); row++)
			{
				for (int col = 0; col < m_InputGrid.GetColumnCount(); col++)
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

					if (col < m_InputGrid.GetFixedColumnCount())
					{
						Item.iImage = rand()%m_ImageList.GetImageCount();
						Item.mask  |= (GVIF_IMAGE);
					}

					m_InputGrid.SetItem(&Item);
				}
			}
	}

	// Having weird Autosize problems with CE 2.11 - possibly just an emulation problem
#if !defined(_WIN32_WCE) || (_WIN32_WCE < 211)
	m_InputGrid.AutoSize();
#endif
	UpdateData(FALSE);

}

BOOL CALLBACK CPageIn::GridCallback(GV_DISPINFO *pDispInfo, LPARAM /*lParam*/) 
{
	pDispInfo->item.strText.Format(_T("Callback %d,%d"),pDispInfo->item.row, pDispInfo->item.col);
	return TRUE;
}

void CPageIn::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}

void CPageIn::CreateGridEx(void)
{
	//nInitial用来确定图像列表包含的图像数量；nGrow用来确定图像列表可控制的图像数量
// 	m_ImageList.Create(30,30,ILC_COLOR24|ILC_MASK,2,1);
// 	CBitmap a;
// 	a.LoadBitmap(IDB_BITMAP_IOSTS);
// 	m_ImageList.Add(&a,RGB(255,255,255));
	m_ExInputGrid.SetImageList(&m_ImageList);//设置大图标控制

	m_ExInputGrid.EnableDragAndDrop(FALSE);
	m_ExInputGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_ExInputGrid.SetEditable(FALSE);
	//	OnVirtualMode();    // Sets the grid mode, fills the grid
	m_ExInputGrid.SetListMode(TRUE);
	m_ExInputGrid.SetTrackFocusCell(FALSE);
	//	m_ExInputGrid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
	m_ExInputGrid.SetFrameFocusCell(FALSE);	//去除鼠标点选时的边框线  
	m_ExInputGrid.SetSingleRowSelection();
	m_ExInputGrid.SetColumnCount(3);	//初始列数：第一列为序号，第二列为状态，第三列为详细说明
	m_ExInputGrid.SetRowCount(IN_POINT/*+EX_IN_POINT*/+1);		//设置初始行数量，初始化只有一行
	m_ExInputGrid.SetFixedRowCount(1);	
	m_ExInputGrid.SetFixedBkColor(RGB(128,128,192));
	m_ExInputGrid.SetGridLines(GVL_BOTH);

	m_ExInputGrid.SetGridItemText(0,0,_T("端  口  号 "));
	m_ExInputGrid.SetGridItemText(0,1,_T("状  态"));
	m_ExInputGrid.SetGridItemText(0,2,_T("说                       明"));
	//序号头宽度	
	m_ExInputGrid.ExpandColumnsToFit();
	m_ExInputGrid.AutoSizeColumns();

	m_ExInputGrid.SetColumnResize(TRUE);
	m_ExInputGrid.SetRowResize(FALSE);

	//调整行高
	for(UINT i=0;i<(IN_POINT/*+EX_IN_POINT*/+1);i++)
		m_ExInputGrid.SetRowHeight(i,INPUT_GRID_HEIGHT);	

	//调整风格
	for (UINT i=0;i<(IN_POINT/*+EX_IN_POINT*/+1);i++){
		for (UINT j=0;j<INPUT_GRID_COL_NUM;j++)
			m_ExInputGrid.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
	}	

	CString str;
	for(UINT i=1;i<(IN_POINT/*+EX_IN_POINT*/+1);i++)
	{
		str.Format(_T("扩展# %2d"),i-1);
		m_ExInputGrid.SetGridItemText(i,0,str);
	}
}

void CPageIn::CreateGrid()
{
	//nInitial用来确定图像列表包含的图像数量；nGrow用来确定图像列表可控制的图像数量
	m_ImageList.Create(30,30,ILC_COLOR24|ILC_MASK,2,1);
	CBitmap a;
	a.LoadBitmap(IDB_BITMAP_IOSTS);
	m_ImageList.Add(&a,RGB(255,255,255));
	m_InputGrid.SetImageList(&m_ImageList);//设置大图标控制

	m_InputGrid.EnableDragAndDrop(FALSE);
	m_InputGrid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_InputGrid.SetEditable(FALSE);
//	OnVirtualMode();    // Sets the grid mode, fills the grid
	m_InputGrid.SetListMode(TRUE);
	m_InputGrid.SetTrackFocusCell(FALSE);
//	m_InputGrid.SetCallbackFunc(m_bCallback? GridCallback : NULL, 0);
	m_InputGrid.SetFrameFocusCell(FALSE);	//去除鼠标点选时的边框线  
	m_InputGrid.SetSingleRowSelection();
	m_InputGrid.SetColumnCount(3);	//初始列数：第一列为序号，第二列为状态，第三列为详细说明
	m_InputGrid.SetRowCount(IN_POINT/*+EX_IN_POINT*/+1);		//设置初始行数量，初始化只有一行
	m_InputGrid.SetFixedRowCount(1);	
	m_InputGrid.SetFixedBkColor(RGB(128,128,192));
	m_InputGrid.SetGridLines(GVL_BOTH);
	
	m_InputGrid.SetGridItemText(0,0,_T("端  口  号 "));
	m_InputGrid.SetGridItemText(0,1,_T("状  态"));
	m_InputGrid.SetGridItemText(0,2,_T("说                       明"));
	//序号头宽度	
	m_InputGrid.ExpandColumnsToFit();
	m_InputGrid.AutoSizeColumns();

	m_InputGrid.SetColumnResize(TRUE);
	m_InputGrid.SetRowResize(FALSE);

	//调整行高
	for(UINT i=0;i<(IN_POINT/*+EX_IN_POINT*/+1);i++)
		m_InputGrid.SetRowHeight(i,INPUT_GRID_HEIGHT);	

	//调整风格
	for (UINT i=0;i<(IN_POINT/*+EX_IN_POINT*/+1);i++){
		for (UINT j=0;j<INPUT_GRID_COL_NUM;j++)
			m_InputGrid.SetItemState(i,j,GVIS_READONLY|GVIS_DROPHILITED);
	}	

	CString str;
	for(UINT i=1;i<(IN_POINT/*+EX_IN_POINT*/+1);i++)
	{
		str.Format(_T("输入# %2d"),i-1);
		m_InputGrid.SetGridItemText(i,0,str);
	}
}


void CPageIn::UpdateExIOSts(long sts)
{
	GV_ITEM Item;

	for (int i=0;i<IN_POINT;i++)
	{
		Item.col = 1;
		Item.row = i+1;
		m_ExInputGrid.GetItem(&Item);

		if ((sts&IOstatusBitsMask[i])==IOstatusBitsMask[i])
			Item.iImage =1;
		else
			Item.iImage =0;	

		Item.mask  |= (GVIF_IMAGE);
		m_ExInputGrid.SetItem(&Item);		
	}
}

// 输入状态设置
void CPageIn::UpdateIOSts(long sts)
{
	GV_ITEM Item;

	for (int i=0;i<IN_POINT;i++)
	{
		Item.col = 1;
		Item.row = i+1;
		m_InputGrid.GetItem(&Item);

		if ((sts&IOstatusBitsMask[i])==IOstatusBitsMask[i])
			Item.iImage =1;
		else
			Item.iImage =0;	

		Item.mask  |= (GVIF_IMAGE);
		m_InputGrid.SetItem(&Item);		
	}
}
void CPageIn::GetPLCRSts()
{		
//     GV_ITEM Item;
// 	BOOL sts =FALSE;
// 	CString strtemp;
// 	int nBitNum =0;
// 	int nPlcRData = -1;
// 	int bBitBase = PLC_READ_PORT_BEIT_AIRSAFE;
// 	for (int n=0;n<EX_IN_POINT;n++)
// 	{
// 		nBitNum = bBitBase+n; 
// 		strtemp.Format("%d",nBitNum);
// 		nPlcRData = -1;
// 		if ((theApp.m_Serial.m_Port[0].m_iComState == 1)||(theApp.m_Serial.m_Port[0].m_iComState == 2))
// 		{
// 			Sleep(10);
// 			nPlcRData = theApp.m_Serial.ReadPlcRData(strtemp);
// 			if(nPlcRData==0||nPlcRData==1)
// 			{
// 				if(nPlcRData==1)
// 				{  
// 					sts= TRUE;
// 				}
// 				else if(nPlcRData==0)
// 				{
// 					sts = FALSE;
// 				}
// 			}
// 		}
// 		//////////////////////////////////////////////////////////////////////////
// 		Item.col = 1;
// 		Item.row = n+1+IN_POINT;
// 		m_InputGrid.GetItem(&Item);
// 		if (sts == TRUE) 
// 			Item.iImage =1;
// 		else
// 			Item.iImage =0;	
// 
// 		Item.mask  |= (GVIF_IMAGE);
// 		m_InputGrid.SetItem(&Item);	
// 	}	
// 	//  喷射阀加热状态单独处理
// 	nBitNum = PLC_READ_PORT_STS_HEART; 
// 	strtemp.Format("%d",nBitNum);
// 	nPlcRData = -1;
// 	if ((theApp.m_Serial.m_Port[0].m_iComState == 1)||(theApp.m_Serial.m_Port[0].m_iComState == 2))
// 	{
// 		Sleep(10);
// 		nPlcRData = theApp.m_Serial.ReadPlcRData(strtemp);
// 		if(nPlcRData==0||nPlcRData==1)
// 		{
// 			if(nPlcRData==1)
// 			{  
// 				sts= TRUE;
// 			}
// 			else if(nPlcRData==0)
// 			{
// 				sts = FALSE;
// 			}
// 		}
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	Item.col = 1;
// 	Item.row = 16+1+8;
// 	m_InputGrid.GetItem(&Item);
// 	if (sts == TRUE) 
// 		Item.iImage =1;
// 	else
// 		Item.iImage =0;	
// 
// 	Item.mask  |= (GVIF_IMAGE);
// 	m_InputGrid.SetItem(&Item);	
}

void CPageIn::ReadInputMap()
{
	int j = 0;
	CString strid,strinfo;	
	for (j = 0; j < IN_POINT/*+EX_IN_POINT*/; j++)
	{ 
		if (g_pFrm->m_Robot->m_InPutCtrl[j].m_nIOType==TYPE_INPUT)
		{
			strinfo=g_pFrm->m_Robot->m_InPutCtrl[j].m_strIOName;
			m_InputGrid.SetGridItemText(j+1,2,strinfo);//说明
		}		
	}  
} 


void CPageIn::ReadExInputMap()
{
	CString strid,strinfo;	
	for (int j = IN_POINT; j < IN_POINT+EX_IN_POINT; j++)
	{ 
		if (g_pFrm->m_Robot->m_InPutCtrl[j].m_nIOType==TYPE_INPUT)
		{
			strinfo=g_pFrm->m_Robot->m_InPutCtrl[j].m_strIOName;
			m_ExInputGrid.SetGridItemText(j+1-IN_POINT,2,strinfo);//说明
		}		
	}
}