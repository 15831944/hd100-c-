// AListCtrl.cpp : implementation file
//

#include "stdafx.h"

#include "AListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
//tag_Aitemdata
tag_Aitemdata::tag_Aitemdata( int nSubitemCount )
{
	m_lParam = 0;
//	bParam = FALSE;
	
	m_textcor = RGB(0,0,0);
	m_bkcor = RGB(250,250,250);
	
	if( nSubitemCount <= 0)
	{
		m_SubitemArray.SetSize(0);
	}
	else
	{
		m_SubitemArray.SetSize( nSubitemCount );
		
		for( int i=0; i<nSubitemCount; i++)
		{
			tag_Asubitemdata *pnode = new tag_Asubitemdata;
			m_SubitemArray[i] = pnode;
		}
	}
}

tag_Aitemdata::~tag_Aitemdata()
{
	freeSubitemList();
}

void tag_Aitemdata::freeSubitemList()
{
	for( int i=0; i<m_SubitemArray.GetSize(); i++ )
	{
		tag_Asubitemdata *node = m_SubitemArray[i];
		
		if(node)
			delete node;
	}
	
	m_SubitemArray.RemoveAll();
}

BOOL tag_Aitemdata::insertSubitem( int nSubitem )
{
	if( nSubitem < 0 )
		return FALSE;		
	
	tag_Asubitemdata *psub = new tag_Asubitemdata;
	m_SubitemArray.InsertAt(nSubitem, psub);
	
	return TRUE;
}

BOOL tag_Aitemdata::deleteSubitem( int nSubitem )
{
	if( nSubitem < 0 || nSubitem >= m_SubitemArray.GetSize() )
		return FALSE;
	
	tag_Asubitemdata *psub = m_SubitemArray[nSubitem];
	if( psub )
		delete psub;
	m_SubitemArray[nSubitem] = NULL;
	m_SubitemArray.RemoveAt(nSubitem);
	
	return TRUE;
}

BOOL tag_Aitemdata::SetSubitemEdit( int nSubitem, BOOL bCanEdit )
{
	if( nSubitem >= 0 && nSubitem < m_SubitemArray.GetSize() )
	{
		tag_Asubitemdata *psub = m_SubitemArray[nSubitem];
		
		if( psub != NULL)
		{
			psub->m_bCanEdit = bCanEdit;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL tag_Aitemdata::GetSubitemEdit( int nSubitem )
{
	if( nSubitem >= 0 && nSubitem < m_SubitemArray.GetSize() )
	{
		tag_Asubitemdata *psub = m_SubitemArray[nSubitem];
		
		if( psub != NULL)
		{
			return psub->m_bCanEdit;
		}
	}
	return FALSE;
}

BOOL tag_Aitemdata::SetSubitemColor( int nSubitem, BOOL btxt, COLORREF txtcr, BOOL bBk, COLORREF bkcor )
{
	if( nSubitem >= 0 && nSubitem < m_SubitemArray.GetSize() )
	{
		tag_Asubitemdata *psub = m_SubitemArray[nSubitem];
		
		if( psub != NULL)
		{
			if( btxt )
				psub->m_textcor	= txtcr;
			if( bBk )
				psub->m_bkcor	= bkcor;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL tag_Aitemdata::GetSubitemColor( int nSubitem, COLORREF &txtcr, COLORREF &bkcor )
{
	if( nSubitem >= 0 && nSubitem < m_SubitemArray.GetSize() )
	{
		tag_Asubitemdata *psub = m_SubitemArray[nSubitem];
		
		if( psub != NULL)
		{
			txtcr = psub->m_textcor;
			bkcor = psub->m_bkcor;
			return TRUE;
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CAListCtrl

CAListCtrl::CAListCtrl()
{
	m_ToolTip.m_hWnd = NULL;
	
	m_CurRow = m_CurCol = -1;
	CurEditRow = CurEditCol = -1;
	m_LastRow		= -1;
	m_bAlwaysSelect = TRUE;
	m_bLBtDown		= FALSE;

	m_bkColorMode	= en_Alist_Mode_Odd_Even;//背景默认单双行模式
	m_odd_bkcor		= RGB(220,220,220);//奇数行背景颜色,灰
	m_even_bkcor	= RGB(255,255,255);//偶数行背景颜色,白

	m_TextColorMode	= en_Alist_Mode_Odd_Even;//文本默认单行控制模式
	m_odd_textcor	= RGB(0,0,0);	//奇数行文字颜色,黑
	m_even_textcor	= RGB(0,0,0);	//偶数行文字颜色,黑
}

CAListCtrl::CAListCtrl(CAListCtrl &srclist)
{
	UNREFERENCED_PARAMETER(srclist);
	m_ToolTip.m_hWnd = NULL;
	
	m_CurRow = m_CurCol = -1;
	CurEditRow = CurEditCol = -1;
	m_LastRow		= -1;
	m_bAlwaysSelect = TRUE;
	m_bLBtDown		= FALSE;
	
	m_bkColorMode	= en_Alist_Mode_Odd_Even;//背景默认单双行模式
	m_odd_bkcor		= RGB(220,220,220);//奇数行背景颜色,灰
	m_even_bkcor	= RGB(255,255,255);//偶数行背景颜色,白
	
	m_TextColorMode	= en_Alist_Mode_SingleRow;//文本默认单行控制模式
	m_odd_textcor	= RGB(0,0,0);	//奇数行文字颜色,黑
	m_even_textcor	= RGB(0,0,0);	//偶数行文字颜色,黑
}

CAListCtrl::~CAListCtrl()
{
// 	if(!m_ColEdit.IsEmpty())
// 		m_ColEdit.RemoveAll();

}


BEGIN_MESSAGE_MAP(CAListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CAListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	ON_EN_KILLFOCUS(IDC_AListCtrl_EDIT_20050228, OnKillFocusEdit1)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, OnItemChanged)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_LBUTTONUP()
	ON_WM_MEASUREITEM_REFLECT()//
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	//ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CAListCtrl::OnLvnItemchanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAListCtrl message handlers
int CAListCtrl::HitTestEx(CPoint& Point, int* pColumn)
{
	int ColumnNum = 0;
    int Row = HitTest(Point,NULL);
	
	if (pColumn)
		*pColumn = 0;

    //判断ListCtrl 是否为LVS_REPORT，该风格只有用到CListCtrl->EditLabel()时才有用
	//由于该程序中使用了自己创建的EDIT，可以不用判断LVS_REPORT。
	//if ((GetWindowLong(m_hWnd,GWL_STYLE)&LVS_TYPEMASK)!=LVS_REPORT)
	//	return Row;
	
	Row = GetTopIndex();
	int Bottom = Row + GetCountPerPage();
	if ( Bottom > GetItemCount() )
	{
		Bottom = GetItemCount();
	}

    //判断在哪一行 哪一列
	CHeaderCtrl* pHeader=(CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount;
	if(pHeader)	//判断有没有插入列
		nColumnCount=pHeader->GetItemCount();
	else
		nColumnCount = 0;

	for(;Row<=Bottom;++Row)
	{
		CRect rect;
		GetItemRect( Row, &rect, LVIR_BOUNDS );

		if (rect.PtInRect(Point))
		{
			for(ColumnNum=0;ColumnNum<nColumnCount;++ColumnNum)
			{
				int ColWidth=GetColumnWidth(ColumnNum);
				if (Point.x>=rect.left && Point.x<=(rect.left+ColWidth))
				{
					if (pColumn)
						*pColumn=ColumnNum;	//返回列
					return Row;				//返回行
				}
				rect.left +=ColWidth;
			}
		}
	}
	
	return -1;
}

//void CAListCtrl::EditSubItem(int Item, int Column)
//{
//	//CHeaderCtrl* pHeader=(CHeaderCtrl*)GetDlgItem(0);
//	CHeaderCtrl* pHeader=this->GetHeaderCtrl();
//	int nColumnCount=pHeader->GetItemCount();
//	if (Column>=nColumnCount || GetColumnWidth(Column)<5)   //判断宽度是否正确
//		return;
//	
//	int Offset=0;
//	for(int iColumn=0;iColumn<Column;++iColumn)             //得到CEditCell的right坐标
//	{
//		Offset +=GetColumnWidth(iColumn);
//	}
//
//	CRect rect;//对应行列CELL的大小，Edit1所要占用的位置
//	if(Column == 0)//判断是否是第0列，0列要考虑图标
//		GetItemRect(Item,rect,LVIR_LABEL);
//	else
//		GetItemRect(Item,rect,LVIR_BOUNDS);
//
//	CRect ClientRect;
//	GetClientRect(&ClientRect);
//	if (Offset+rect.left < 0 || Offset+rect.left>ClientRect.right ) //CEditCell的left坐标超出list范围
//	{
//		CSize size;
//		if ( Offset + rect.left > 0 )	
//		{
//			size.cx = Offset + rect.left - ClientRect.right;//计算滚动量,左边界在list外面
//															// 应该不会出现这种情况 -_-
//															//除非不是通过鼠标事件响应的
//		}
//		else
//		{
//			//size.cx = Offset - rect.left;
//			size.cx = Offset + rect.left;//计算滚动量，左边界<0的情况
//		}
//
//		size.cy=0;
//		Scroll(size);//滚动listctrl使被选中的item完全显示出来
//					//（对于处于listctrl右边露出一部分的cell不在此列）
//		rect.OffsetRect(-size);
//	}
//
////	CListCtrl::GetSubItemRect(Item, Column, LVIR_LABEL, rect);//这个似乎不准 -_-!
//
//	LV_COLUMN lvCol;
//	lvCol.mask=LVCF_FMT;
//	GetColumn(Column,&lvCol);
//
//	DWORD dwAlignStyle;	//edit 对齐风格
//	//判断item的显示位子,注：应该是判断列的对齐风格，保持edit风格与对应列一致
//	int aaa = lvCol.fmt & LVCFMT_JUSTIFYMASK;
//	if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
//		dwAlignStyle = ES_LEFT;
//    else if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
//		dwAlignStyle = ES_RIGHT;
//    else dwAlignStyle = ES_CENTER;
//
//    //调整CEditCell 的w位子 (两边缩小d)
//	long d = 1;
//	rect.left += (Offset+d);
//	if(Column != 0)
//		rect.right=rect.left+GetColumnWidth(Column)-2*d;//注：这里应该为2*d
//	
//	if(rect.right < rect.left) rect.right = rect.left;
//	if (rect.right>ClientRect.right)
//		rect.right=ClientRect.right;
//
//	EditCellShow(dwAlignStyle,rect,Item,Column);
//	
//	//ComboCellShow(dwStyle,rect,Item,Column);
//}

void CAListCtrl::EditSubItem(int Item, int Column)
{
	//CHeaderCtrl* pHeader=(CHeaderCtrl*)GetDlgItem(0);
	CHeaderCtrl* pHeader=this->GetHeaderCtrl();
	int nColumnCount=pHeader->GetItemCount();
	if (Column>=nColumnCount || GetColumnWidth(Column)<5)   //判断宽度是否正确
		return;

	CRect rect;//对应行列CELL的大小，Edit1所要占用的位置
	if(Column == 0)//判断是否是第0列，0列要考虑图标
		GetSubItemRect( Item, Column, LVIR_LABEL, rect );
	else
		GetSubItemRect( Item, Column, LVIR_BOUNDS, rect );

	//判断cell左右边界是否在list范围内，
	//优先考虑左边界，不考虑左右边界同时超出范围的情况，这种情况也无法处理 -_-!
	CRect ClientRect;
	GetClientRect(&ClientRect);
	if( rect.left < 0 && rect.right > ClientRect.right )
	{
		//左右边界同时出界,缩小edit1的大小
		rect.left = 0;
		rect.right = ClientRect.right;
	}
	else if (rect.left < 0 || rect.left > ClientRect.right ) //CEditCell的left坐标超出list范围
	{
		if(rect.Width() > ClientRect.Width())
		{
			//无法显示完整大小的edit，缩小edit
			rect.right = rect.left + ClientRect.Width();
		}

		CSize size;//滚动量
		if ( rect.left > 0 )
		{
			//左边界在list右侧外面，应该不会出现这种情况 -_-! ，除非不是通过鼠标事件响应的
			size.cx = rect.left - ClientRect.right;//计算滚动量
		}
		else
		{
			size.cx = rect.left;//计算滚动量，左边界<0的情况
		}
		
		size.cy=0;	//只左右滚动
		Scroll(size);//滚动listctrl使被选中的item完全显示出来
		rect.OffsetRect(-size);
	}
	else if( rect.right < 0 || rect.right > ClientRect.right )
	{
		CSize size;//滚动量

		if(rect.Width() > ClientRect.Width())
		{
			//无法显示完整大小的edit，缩小edit
			rect.right = rect.left + ClientRect.Width();
			//左边对齐
			size.cx = rect.left;
		}
		else
		{
			if ( rect.right < 0 )
			{
				//右边界在list左侧外面，应该不会出现这种情况 -_-! ，除非不是通过鼠标事件响应的
				size.cx = rect.right;
			}
			else
			{
				size.cx = rect.right - ClientRect.right;
			}
		}
		
		size.cy=0;	//只左右滚动
		Scroll(size);//滚动listctrl使被选中的item完全显示出来
		rect.OffsetRect(-size);
	}
	
	//	CListCtrl::GetSubItemRect(Item, Column, LVIR_LABEL, rect);//这个似乎不准 -_-!

	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT;
	GetColumn(Column,&lvCol);

	DWORD dwAlignStyle;	//edit 对齐风格
	//判断item的显示位子,注：应该是判断列的对齐风格，保持edit风格与对应列一致
//	int aaa = lvCol.fmt & LVCFMT_JUSTIFYMASK;
	if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwAlignStyle = ES_LEFT;
    else if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwAlignStyle = ES_RIGHT;
    else dwAlignStyle = ES_CENTER;
	
    //调整CEditCell 的w位子 (两边缩小d)
	long d = 1;
	rect.left += d;
	rect.right -= d;
	
	EditCellShow(dwAlignStyle,rect,Item,Column);	
	//ComboCellShow(dwStyle,rect,Item,Column);
}

void CAListCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CListCtrl::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
/*
	if(edit1.GetSafeHwnd())
	{
		if(edit1.IsWindowVisible())
		{
			if(GetDlgItem(IDC_AListCtrl_EDIT_20050228) != GetFocus())
				UpdateText();
		}
	}
*/
}

void CAListCtrl::UpdateText()
{	
	if(m_edit1.GetSafeHwnd())
	{
		if(m_edit1.IsWindowVisible())
		{
			m_edit1.ShowWindow(SW_HIDE);
			if(CurEditRow == -1 || CurEditCol == -1)
				return;

			CString buf;
			m_edit1.GetWindowText(buf);
			//this->SetItemText(CurRow,CurCol,buf);
			this->SetItem(CurEditRow,CurEditCol,LVIF_TEXT,buf,1,1, 1, 1);
			CString showstring;
			showstring.Format("%d,%d",CurEditRow,CurEditCol);
			m_edit1.SetWindowText("");
		}
	}
}

void CAListCtrl::EditCellShow(DWORD dwAlignStyle,CRect rect, int nItem, int nSubitem)
{	
	if( !GetEdit( nItem, nSubitem) )
	{
		return;	//不可编辑
	}

	if(!m_edit1.GetSafeHwnd())
	{
		m_edit1.Create( dwAlignStyle | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_CHILD |
			WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_MULTILINE ,
			CRect(0,0,0,0), this, IDC_AListCtrl_EDIT_20050228 );

		CFont *font = this->GetFont();
		m_edit1.SetFont(font);
	}
	else
	{
		//修改风格,ES_LEFT/ES_RIGHT/ES_CENTER
		m_edit1.ModifyStyle( ES_LEFT | ES_CENTER | ES_RIGHT /*0x11*/, dwAlignStyle );
	}

	m_edit1.MoveWindow(rect);

	CString strtxt = GetItemText(nItem,nSubitem);
	m_edit1.SetWindowText( strtxt );
	m_edit1.ShowWindow(SW_SHOW);
	m_edit1.SetFocus();
	m_edit1.SetSel(0,-1);

	//调整edit控件文字显示上下居中,需配合ES_MULTILINE风格,
	//eidt内不要换行,否则只能显示最后一行
	if ( strtxt == "" )
		strtxt = " ";	//如果cell中无内容, GetTextExtent得到的字符高度为0导致居中显示错误
	
	CRect rect1;
	m_edit1.GetClientRect(rect);
	rect1 = rect;
	CDC* pDC=m_edit1.GetDC();
//	int H = pDC->DrawText( GetItemText(Item, SubItem), rect1, DT_CALCRECT | DT_CENTER |DT_WORDBREAK );
	CSize sz=pDC->GetTextExtent(strtxt);//文本高度
	CRect txtrect(0,0,rect.Width(),sz.cy);	//文本显示区大小,仅调整高度
//	CRect txtrect(0,0,rect.Width(),H);	//文本显示区大小,仅调整高度
	
	rect.OffsetRect(rect.CenterPoint()-txtrect.CenterPoint());
	m_edit1.SetRect(&rect);
	//SendMessage(m_edit1.GetSafeHwnd(),EM_SETRECT,0,(LPARAM)&rect);

	CurEditRow = nItem;
	CurEditCol = nSubitem;
}

void CAListCtrl::OnKillFocusEdit1()
{
	// TODO: Add your control notification handler code here
	UpdateText();
}

BOOL CAListCtrl::IsEditing()
{
	if(m_edit1.GetSafeHwnd())
	{
		if(GetDlgItem(IDC_AListCtrl_EDIT_20050228) == GetFocus())
			return TRUE;
	}
	return FALSE;
}

int CAListCtrl::InsertColumn(int nCol,const LVCOLUMN* pColumn,BOOL bCanEdit)
{
	UNREFERENCED_PARAMETER(bCanEdit);
	int nrtn = CListCtrl::InsertColumn(nCol,pColumn);

	//设置编辑状态
	if(nrtn != -1)
	{
		nCol = nrtn;	//更新ncol实际值

		//更新每行的数据
		for( int nItem=0; nItem<GetItemCount(); nItem++)
		{
			tag_Aitemdata *pitemdate = (tag_Aitemdata*)(CListCtrl::GetItemData(nItem));
			
			if( pitemdate != NULL)
			{
				pitemdate->insertSubitem( nCol );
			}
		}
	}
	
	return nrtn;
}

int CAListCtrl::InsertColumn(int nCol,LPCTSTR lpszColumnHeading,int nFormat,int nWidth,int nSubItem,BOOL bCanEdit)
{
	LVCOLUMN lvColumn;
	
	lvColumn.mask = LVCF_FMT | LVCF_TEXT;
	if( nWidth != -1 )
	{
		lvColumn.mask = lvColumn.mask | LVCF_WIDTH;
	}
	if( nSubItem != -1 )
	{
		lvColumn.mask = lvColumn.mask | LVCF_SUBITEM;
	}
	
	lvColumn.fmt = nFormat;
	lvColumn.cx = nWidth;
	lvColumn.pszText = (char*)lpszColumnHeading;
	lvColumn.cchTextMax = strlen(lvColumn.pszText);
	lvColumn.iSubItem = nSubItem;
	
	return InsertColumn(nCol, &lvColumn, bCanEdit);	
}

BOOL CAListCtrl::DeleteColumn(int nCol)
{
	if( nCol < 0 || nCol >= GetColumnCount() )
		return FALSE;

	//更新每行的数据
	for( int nItem=0; nItem<GetItemCount(); nItem++)
	{
		tag_Aitemdata *pitemdate = (tag_Aitemdata*)(CListCtrl::GetItemData(nItem));
		
		if( pitemdate != NULL)
		{
			pitemdate->deleteSubitem( nCol );
		}
	}

	if( CListCtrl::DeleteColumn(nCol) )
	{
		if ( GetColumnCount() == 0 )
		{
			//当所有列都被删除时，删除所有项
			return DeleteAllItems();
		}

		return TRUE;
	}
	return FALSE;
}

BOOL CAListCtrl::DeleteAllColumn()
{
	while ( GetColumnCount() > 0 )
	{
		if(!DeleteColumn(0))
			return FALSE;
	}

	return TRUE;
}

BOOL CAListCtrl::DeleteItem(const int nItem)
{
	if ( nItem < 0 || nItem >= GetItemCount() )
		return FALSE;
	
	tag_Aitemdata *pitemdata = (tag_Aitemdata *)(CListCtrl::GetItemData( nItem ));
	if( pitemdata != NULL )
	{
		delete pitemdata;
		pitemdata = NULL;
	}

	CListCtrl::DeleteItem(nItem);
	
	if(nItem == m_CurRow)
	{
		m_CurRow = -1;
		m_CurCol = -1;
	}
	else if(nItem < m_CurRow)
		m_CurRow--;
	
	return TRUE;
}

BOOL CAListCtrl::DeleteAllItems()
{
	if(IsEditing())
		m_edit1.ShowWindow(SW_HIDE);//仅仅隐藏

	m_CurRow = -1;
	m_CurCol = -1;

	for( int nItem = 0; nItem < GetItemCount(); nItem ++ )
	{
		tag_Aitemdata *pitemdata = (tag_Aitemdata *)(CListCtrl::GetItemData( nItem ));
		if( pitemdata != NULL )
		{
			delete pitemdata;
			pitemdata = NULL;
		}
	}

	return CListCtrl::DeleteAllItems();
}

BOOL CAListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		if(IsEditing())
		{
			UpdateText();
			return TRUE;
		}
	}
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
	{
		if(IsEditing())
		{
			m_edit1.ShowWindow(SW_HIDE);//仅仅隐藏
			//UpdateText();
			return TRUE;
		}
	}
	if(pMsg->message == WM_MOUSEMOVE)
		if(!pMsg->wParam)
			m_bLBtDown = FALSE;

	return CListCtrl::PreTranslateMessage(pMsg);
}

void CAListCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(point);
	// TODO: Add your message handler code here
/*
	int ColNum;
	CRect mainrect;
	this->GetWindowRect(mainrect);		//获得控件的屏幕坐标
	CPoint CLPoint(point);				//point为屏幕坐标
	CLPoint.x -= mainrect.left;
	CLPoint.y -= mainrect.top;			//将坐标转化为控件内部坐标
	if(HitTestEx(CLPoint, &ColNum) != -1)	//判断右键是否点中某一项
    {
		CMenu MainTPMMenu;
		CMenu MiscFruitMenu;

		MainTPMMenu.CreatePopupMenu();
		CHeaderCtrl* pHeader=this->GetHeaderCtrl();
		int nColumnCount=pHeader->GetItemCount();
		HDITEM pHeaderItem;
		char buf[100];
		pHeaderItem.mask = HDI_TEXT;
		pHeaderItem.cchTextMax = 10;
		pHeaderItem.pszText = buf;
		if (ColNum>=nColumnCount || GetColumnWidth(ColNum)<5)   //判断宽度是否正确
			return;
		for(int i=0;i<nColumnCount;i++)
		{
			pHeader->GetItem(i,&pHeaderItem);					//获得列名
			MainTPMMenu.AppendMenu(MF_STRING | MF_ENABLED ,42+i,pHeaderItem.pszText);
		}
		MiscFruitMenu.CreatePopupMenu();
		MiscFruitMenu.AppendMenu(MF_STRING | MF_ENABLED, 40, "Mangos");
		MiscFruitMenu.AppendMenu(MF_STRING | MF_ENABLED, 41, "Tomatoes");
		MainTPMMenu.AppendMenu(MF_STRING | MF_POPUP | MF_ENABLED,
			(UINT)MiscFruitMenu.m_hMenu,"Other Misc. Fruit");
		MainTPMMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x,
			point.y, this, NULL);
	}
*/
}

void CAListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	UpdateText();
	int Index;
    int ColNum;
    if((Index = HitTestEx(point, &ColNum)) != -1)
    {
		BOOL bShowEdit = (m_CurRow == Index);
	
		m_bLBtDown = TRUE;
		m_CurCol = ColNum;
//		m_CurRow = Index;
		CListCtrl::OnLButtonDown(nFlags, point);//该响应应在显示edit控件之前
							//否则会导致edit丢失焦点

		//判断是否点击的文本区域
		CRect subrect;
		GetSubItemRect(Index, m_CurCol, LVIR_LABEL, subrect);
		if( subrect.PtInRect(point) && bShowEdit)
		{
			EditSubItem(Index, ColNum);
			//if (GetWindowLong (m_hWnd, GWL_STYLE) & LVS_EDITLABELS)
		}
    }
	else
	{
		CListCtrl::OnLButtonDown(nFlags, point);
		if(m_bAlwaysSelect)
			SetCurSel(m_CurRow);//用于
		else
		{
		}
	}
}

void CAListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int Index;
    int ColNum;
    if ((Index = HitTestEx(point, &ColNum)) != -1)
    {
		m_CurCol = ColNum;
//		m_CurRow = Index;
		CListCtrl::OnRButtonDown(nFlags, point);
		//if (GetWindowLong (m_hWnd, GWL_STYLE) & LVS_EDITLABELS)
//		EditSubItem(Index, ColNum);
    }
	else
	{
		CListCtrl::OnRButtonDown(nFlags, point);
		if(m_bAlwaysSelect)
			SetCurSel(m_CurRow);//用于
		else
		{
		}
	}
}

void CAListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int Index;
    int ColNum;
    if ((Index = HitTestEx (point, &ColNum)) != -1)
    {

		m_CurCol = ColNum;
		//		m_CurRow = Index;
		CListCtrl::OnLButtonDblClk(nFlags, point);//
		//if (GetWindowLong (m_hWnd, GWL_STYLE) & LVS_EDITLABELS)
		//	EditSubItem(Index, ColNum);
    }
	else
	{
		CListCtrl::OnLButtonDblClk(nFlags, point);
		if(m_bAlwaysSelect)
			SetCurSel(m_CurRow);
		else
		{
		}
	}
}

void CAListCtrl::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int Index;
    int ColNum;
    if ((Index = HitTestEx (point, &ColNum)) != -1)
    {
		m_CurCol = ColNum;
		m_CurRow = Index;
		CListCtrl::OnRButtonDblClk(nFlags, point);
		//if (GetWindowLong (m_hWnd, GWL_STYLE) & LVS_EDITLABELS)
		//	EditSubItem(Index, ColNum);
    }
	else
	{
		CListCtrl::OnRButtonDblClk(nFlags, point);
		if(m_bAlwaysSelect)
			SetCurSel(m_CurRow);//用于
		else
		{
		}
	}

}

BOOL CAListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
//	return TRUE;
	
	return CListCtrl::OnEraseBkgnd(pDC);
//	pDC->FillSolidRect(0,0,100,100,RGB(255,0,0));//这个只能改变背景，
				//这里的背景仅仅是指没有item的时候的list的背景，当插入item后背景即被遮住
				//要用颜色的话得改变item的背景，在onDrawItem/onCustomDrawList??里画
//	return FALSE;
}

void CAListCtrl::AdjustAllColumn()
{
	int ncolCount = GetColumnCount();
	int Col=ncolCount-1;
//	SetColumnWidth(Col,LVSCW_AUTOSIZE_USEHEADER);//

	for(Col=0;Col<ncolCount;Col++)
	{
		SetColumnWidth(Col,LVSCW_AUTOSIZE_USEHEADER);//
	}
}

void CAListCtrl::AdjustColWidth(int Row,int Col)
{
	SetColumnWidth(Col,LVSCW_AUTOSIZE_USEHEADER);//
	return;
	int nRowCount = GetItemCount();
	if(nRowCount==0)	return;
	int nColCount = GetColumnCount();
	CString text = "";
	if(Row < 0 || Row >= nRowCount)
	{//未选定行
		Row = 0;
		if(Col<0 || Col>= nColCount)
			return;//未列选定
		else
		{//选定列
			for(int i=0;i<nRowCount;i++)
			{//循环所有行
				CString buf = GetItemText(i,Col);
				if(text.GetLength() < buf.GetLength())
					text = buf;
			}
		}
	}
	else
	{//选定行
		if(Col<0 || Col>= nColCount)
			return;//未列选定
		else
			text = GetItemText(Row,Col);
	}
	
	AdjustColWidth(Row,Col,text);
}

//根据第Row行Col列调整列宽
void CAListCtrl::AdjustColWidth(int Row,int Col,CString text)
{
//	CWnd *parent = GetParent();
	CDC *pDc = GetDC();

	text = text;	//将text增加一个字符以增加计算出来的字符串宽度
	//动态调整列宽
	int Width = GetColumnWidth(Col);//先获得当前宽度
	
	CFont *pFont = GetFont();
	CFont *pOldFont=pDc->SelectObject(pFont);
	
	CRect textrect(0,0,10,10);
	pDc->DrawText(text,textrect,DT_LEFT | DT_CALCRECT);
	
	int w = textrect.Width();

	if(Col==0)
	{
		CRect rect;
		GetItemRect(Row,rect,LVIR_ICON);//对于第一列由于有图标还得加宽;
										//似乎得不到checkbox的大小
										//因此不建议使用checkbox风格
		w += rect.Width();
	}

	if(w>Width)
		SetColumnWidth(Col,w);//
	pDc->SelectObject(pOldFont);
	ReleaseDC(pDc);
}

//
void CAListCtrl::ResetColWidth(int col)
{
	int ncolCount = GetColumnCount();

//	CWnd *parent = GetParent();
	
	CDC *pDc = GetDC();
	CFont *pFont = GetFont();
	CFont *pOldFont = pDc->SelectObject(pFont); 

	CString text;
//	int maxw=0;
	
	for(int j=0;j<ncolCount;j++)
	{
		if(col != -1 && j != col)
			continue;
//		SetColumnWidth(j,-1);
//		int maxw = GetColumnWidth(j);//先获得当前宽度
		int maxw=0;
		GetColumnText(j,text);//获得第i列列头text
		
		CRect textrect(0,0,10,10);
		pDc->DrawText(text+"M",textrect,DT_LEFT | DT_CALCRECT);
		int w = textrect.Width();	//列头宽
		if(maxw < w)
			maxw = w;
		//循环计算每行宽
		for(int i=0;i<GetItemCount();i++)
		{
			text = GetItemText(i,j);

			pDc->DrawText(text+"M",textrect,DT_LEFT | DT_CALCRECT);
			w = textrect.Width();	//cell内文本宽
			if(j==0)
			{
				GetItemRect(i,textrect,LVIR_ICON);//对于第一列由于有图标还得加宽;
				//似乎得不到checkbox的大小
				//因此不建议使用checkbox风格
				w += textrect.Width();
			}
			if(maxw < w)
				maxw = w;
		}
		SetColumnWidth(j,maxw);
	}
	pDc->SelectObject(pOldFont);
	ReleaseDC(pDc);
}

void CAListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 20;
}

BOOL CAListCtrl::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	//该函数可以得到鼠标点击事件及通过键盘的方向键及PageUp、PageDown选择的item
	//而且当使用checkbox风格时，可以响应鼠标点击checkbox的事件,该事件在普通的鼠标事件中不响应
	//部分风格下(如LVS_EX_CHECKBOXES)响应insertitem，似乎不响应SetItemText、insertcolumn等

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iRow = pNMListView->iItem;
//	int iCol = pNMListView->iSubItem;//该值似乎一直为0？？？？？

#ifdef _DEBUG
	CString strrow;
	strrow.Format("行: %d , new:%04X, old:%04X, changed:%04X\n",
		iRow,pNMListView->uNewState,pNMListView->uOldState,pNMListView->uChanged);	//uChanged对应LVITEM中mask
	CString str1 = (pNMListView->uNewState & LVIS_SELECTED)?"new select\n":"new none select\n";
	CString str2 = (pNMListView->uOldState & LVIS_SELECTED)?"old select\n":"old none select\n";
	TRACE(strrow + str1 + str2);
#endif

	if( pNMListView->uChanged == LVIF_STATE )//状态(选择/焦点等)发生改变
	{
		if( (pNMListView->uNewState & LVIS_SELECTED) && !(pNMListView->uOldState & LVIS_SELECTED) )
		{
			//iRow行从未选中到选中
			m_LastRow = m_CurRow;
			m_CurRow = iRow;
			SendMessage(LVN_ITEMCHANGED, (WPARAM) pNMHDR, (LPARAM)pResult);
		}
		else if( (pNMListView->uOldState & LVIS_SELECTED) && !(pNMListView->uNewState & LVIS_SELECTED) )
		{
			//iRow行从选中到未选中
		}
		else if( (pNMListView->uNewState & 0x2000) && (pNMListView->uOldState & 0x1000) )
		{
			//LVS_EX_CHECKBOXES风格时iRow行checkbox从未选中到选中
		}
		else if( (pNMListView->uNewState & 0x1000) && (pNMListView->uOldState & 0x2000) )
		{
			//LVS_EX_CHECKBOXES风格时iRow行checkbox被取消选中
		}
	}	
	else if( pNMListView->uChanged == LVIF_IMAGE )//图标发生改变
	{
	}
	else if( pNMListView->uChanged == LVIF_INDENT )//缩进发生改变
	{
	}

	return TRUE;
}

BOOL CAListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//没起作用-_-, 难道要在CListView中才有效??
	//或者是要自己Create的才行????
//	cs.style |= LVS_REPORT | LVS_OWNERDRAWFIXED ;//自己画
	return CListCtrl::PreCreateWindow(cs);
}

void CAListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	LPDRAWITEMSTRUCT lpDIS = lpDrawItemStruct;
//	CDC* pDC = CDC::FromHandle(lpDIS->hDC);//???????DC

    UINT state = lpDIS->itemState; //得到状态
    CRect focusRect, btnRect;//两个矩形，表示得当输入焦点时的虚线矩形和按钮矩形
    focusRect.CopyRect(&lpDIS->rcItem);
    btnRect.CopyRect(&lpDIS->rcItem);
    //
    //调整虚线矩形
    //
//	focusRect.left += 4;
//	focusRect.right -= 4;
//	focusRect.top += 4;
//	focusRect.bottom -= 4;
    //
    // 得当Button上文字
    //
	const int bufSize = 512;
	TCHAR buffer[bufSize];
	GetWindowText(buffer, bufSize);
	
	// 使用m_fg, m_bg颜色利用 Draw3dRect(...)绘制按钮边框
	// FillRect(...)填充按钮内部
	// DrawText(...)绘制文字
    //根据当前状态调整显示
    //
    if (state & ODS_FOCUS) {//得到输入焦点，通过focusRect画虚线
			if (state & ODS_SELECTED){
				// 被按下，绘制下陷边框
//				CBrush br(RGB(21,134,56));
//				pDC->FillRect(focusRect,&br);
			}
    }
    else if (state & ODS_DISABLED) {
		//失效，通过m_disabled_fg, m_disabled_bg 重绘按钮内部
    }
	
}

//void CAListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
//{
//	//要控制每个cell的颜色在这个函数里操作
///*
//    if (!lpDrawItemStruct) return;
//	BOOL IsCreated=TRUE;
//	CDC*		pDC			= CDC::FromHandle(lpDrawItemStruct->hDC);
////	CMyLParam*	pMyLParam	= GetMyLParam(lpDrawItemStruct->itemID);
//	CRect		rcItem;
//	
////  if (!pMyLParam) return;
//    int x=0;
//    typedef enum {R_NOTHINGS=0, R_FOCUS, R_SELECT, R_NORMAL, R_SEL_NOFOCUS} redrawmode;
//    redrawmode redraw=R_NORMAL;
//	
//	// Get item image and state info
//	LV_ITEM lvi;
//	lvi.mask = LVIF_IMAGE | LVIF_STATE;
//	lvi.iItem = lpDrawItemStruct->itemID;
//	lvi.iSubItem = 0;
//	lvi.stateMask = 0xFFFF;		// get all state flags
//	GetItem(&lvi);
//	
//	BOOL bHighlight = ((lvi.state & LVIS_DROPHILITED)||
//		((lvi.state & LVIS_SELECTED) && 
//		((GetFocus() == this )||( GetStyle() & LVS_SHOWSELALWAYS ))));
//	
//    BOOL bLostFocus=(GetFocus()!=this);
//
//	if ( (lpDrawItemStruct->itemAction&ODA_FOCUS)==ODA_FOCUS)
//    {   //the control gains or loses input focus
//        if (bLostFocus) redraw=R_SEL_NOFOCUS;
//        else
//			if ((lvi.state&LVIS_FOCUSED)==LVIS_FOCUSED) redraw=R_FOCUS;
//			else 
//			{
//				if (bHighlight) redraw=R_SELECT;
//				else redraw=R_NORMAL;
//			}
//    }
//    else if ((lpDrawItemStruct->itemAction&ODA_SELECT)==ODA_SELECT)
//	{   //the selection status has changed
//		if (bHighlight)
//		{
//			if (bLostFocus) redraw=R_SEL_NOFOCUS;
//			else redraw=R_SELECT;
//		}
//		else redraw=R_NORMAL;
//	}
//	else
//	{   //redraw the item
//		if (bLostFocus)
//		{
//			if (bHighlight) redraw=R_SEL_NOFOCUS;
//			else
//				redraw=R_NORMAL;
//		}
//		else 
//		{
//			if ((lvi.state&LVIS_FOCUSED)==LVIS_FOCUSED) redraw=R_FOCUS;
//			else 
//			{
//				if (bHighlight) redraw=R_SELECT;
//				else redraw=R_NORMAL;
//			}
//		}
//	}
//*/
//	
//}

void CAListCtrl::SelectNone()
{
	int oldrow = m_CurRow;
	m_CurRow = -1;
	SetItemState( oldrow, 0, LVIS_SELECTED|LVIS_FOCUSED );
	//SetItemState( oldrow, 0, -1 );//取消所有状态
}

void CAListCtrl::SetCurSel(int nItem,int nSubItem)
{
	if ( nItem < 0 || nItem >= GetItemCount() )
		return;

	CHeaderCtrl* pHeader=this->GetHeaderCtrl();
	if(nSubItem >= pHeader->GetItemCount())
		return;

	LVITEM lvItem;
	memset(&lvItem, 0, sizeof(LVITEM));

//	//////////////////////////////////////////////////////////////////////////
//	//去除原选择的行, 该部分代码无效..... 
//	if( m_CurRow != -1 )
//	{
//		m_LastRow = m_CurRow;
//
//		lvItem.mask = LVIF_STATE;
//		lvItem.iItem = m_CurRow;
//		lvItem.iSubItem  = 0;
//		GetItem(&lvItem);
//	
//		lvItem.stateMask = lvItem.stateMask & (~(LVIS_FOCUSED | LVIS_SELECTED));
//		lvItem.state = 0xFF;
//		
//		SetItem(&lvItem);
//	}
//	//////////////////////////////////////////////////////////////////////////
	
	m_CurRow = nItem;
	m_CurCol = nSubItem;
	
	memset(&lvItem, 0, sizeof(LVITEM));
	lvItem.mask = LVIF_STATE;
	lvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	lvItem.state = 0xFF;
	lvItem.iItem = m_CurRow;
	lvItem.iSubItem  = m_CurCol;//当不为0时，如果鼠标未点中任何item
								//即没有因为鼠标等操作使整行高亮的情况下
								//显示为一个Cell而不是整行高亮，
								//且在选择其它项时该Cell仍然保持高亮
								//......这里还是没有完全弄明白，有待研究.....
	SetItem(&lvItem);
	EnsureVisible(m_CurRow,TRUE);
}

void CAListCtrl::SetCurSel(int nItem)
{
	if(GetItemCount()==0)
	{
		m_CurRow = -1;
		return;
	}

	if(nItem >= GetItemCount())
		return;

	m_CurRow = nItem;

	LVITEM lvItem;
	memset(&lvItem, 0, sizeof(LVITEM));
	lvItem.mask = LVIF_STATE;
	lvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	lvItem.state = 0xFF;
	lvItem.iItem = m_CurRow;
	lvItem.iSubItem  = 0;//0表示选择整行，显示为整行高亮
	
	SetItem(&lvItem);
	EnsureVisible(m_CurRow,TRUE);
	//SendMessage(LVN_ITEMCHANGED,(WPARAM)0,(LPARAM)0);
}

BOOL CAListCtrl::SetColumnText(int nCol,LPSTR newHeadTxt)
{
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();
	
	if(nCol>=nColumnCount || nCol<0)
		return FALSE;

	HDITEM pHeaderItem;
	pHeaderItem.mask = HDI_TEXT;
	pHeaderItem.cchTextMax = 254;
	pHeaderItem.pszText = newHeadTxt;
	
	return pHeader->SetItem(nCol,&pHeaderItem);
}

BOOL CAListCtrl::GetColumnText(int nSubItem,CString &text)
{
	text = "";
	
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();
	HDITEM pHeaderItem;
	char buf1[255];
	pHeaderItem.mask = HDI_TEXT;
	pHeaderItem.cchTextMax = 254;
	pHeaderItem.pszText = buf1;

	if(nSubItem>=nColumnCount || nSubItem<0)
		return FALSE;
		
	pHeader->GetItem(nSubItem,&pHeaderItem);
	
	text = pHeaderItem.pszText;			//获得列名

	return TRUE;
}

int CAListCtrl::GetColumnCount()
{
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = 0;
	if(pHeader)
		nColumnCount = pHeader->GetItemCount();
	return nColumnCount;
}

BOOL CAListCtrl::SetItemTextByHeadStr(int nItem,CString HeadStr,LPCTSTR lpszText, BOOL bCaseCare)
{
	CString text = "";
	
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();	//列数
	HDITEM pHeaderItem;
	char buf1[255];
	pHeaderItem.mask = HDI_TEXT;
	pHeaderItem.cchTextMax = 254;
	pHeaderItem.pszText = buf1;
	
	for(int i=0;i<nColumnCount;i++)
	{
		pHeader->GetItem(i,&pHeaderItem);
		if( bCaseCare )
		{
			if( pHeaderItem.pszText == HeadStr )
			{
				return SetItemText(nItem,i,lpszText);
			}
		}
		else if(0 == HeadStr.CompareNoCase(pHeaderItem.pszText ))
		{
			return SetItemText(nItem,i,lpszText);
		}
	}
	return FALSE;
}

BOOL CAListCtrl::SetItemText(int nItem,int nSubItem,LPCTSTR lpszText)
{
	return CListCtrl::SetItemText(nItem,nSubItem,lpszText);
}

CString CAListCtrl::GetItemText(int nItem, int nSubItem) const
{
	return CListCtrl::GetItemText(nItem, nSubItem);
}

int CAListCtrl::GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const
{
	return CListCtrl::GetItemText(nItem, nSubItem, lpszText, nLen);
}

int CAListCtrl::GetColumIndexByHeadStr(CString HeadStr)
{
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();	//列数
	HDITEM pHeaderItem;
	char buf1[255];
	pHeaderItem.mask = HDI_TEXT;
	pHeaderItem.cchTextMax = 254;
	pHeaderItem.pszText = buf1;
	
	for(int i=0;i<nColumnCount;i++)
	{
		pHeader->GetItem(i,&pHeaderItem);
		if(pHeaderItem.pszText == HeadStr)
		{
			return i;			//获得列号
		}
	}
	return -1;
}


CString CAListCtrl::GetItemTextByHeadStr(int nItem, CString HeadStr, BOOL bCaseCare)
{
	CString text = "";
	
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();	//列数
	HDITEM pHeaderItem;
	char buf1[255];
	pHeaderItem.mask = HDI_TEXT;
	pHeaderItem.cchTextMax = 254;
	pHeaderItem.pszText = buf1;
	
	for(int i=0;i<nColumnCount;i++)
	{
		pHeader->GetItem(i,&pHeaderItem);
		if( bCaseCare )
		{
			if( pHeaderItem.pszText == HeadStr )
			{
				return GetItemText(nItem, i);			//获得内容
			}
		}
		else if( 0 == HeadStr.CompareNoCase( pHeaderItem.pszText ) )
		{
			return GetItemText(nItem, i);			//获得列名
		}
	}	
	return text;
}

void CAListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = CDRF_DODEFAULT;
	NMLVCUSTOMDRAW * lplvdr=(NMLVCUSTOMDRAW*)pNMHDR;
	int nRow=lplvdr->nmcd.dwItemSpec;	//行
	tag_Aitemdata *pitemdata = (tag_Aitemdata *)(lplvdr->nmcd.lItemlParam); //(tag_Aitemdata *)(CListCtrl::GetItemData( nRow ));

	switch(lplvdr->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT | CDRF_NOTIFYPOSTERASE;
		break;
	case CDDS_ITEMPREPAINT:
		//lplvdr->clrText=(COLORREF)lplvdr->nmcd.lItemlParam;	//利用SetItemDate()
		//lplvdr->clrTextBk=RGB(255,255,255)-(COLORREF)lplvdr->nmcd.lItemlParam;
		{
			if(m_TextColorMode == en_Alist_Mode_SingleRow)
			{
				//单行模式
				lplvdr->clrText = pitemdata->m_textcor;//文本颜色
			}
			else if(m_TextColorMode == en_Alist_Mode_Odd_Even)
			{
				//奇偶行模式
				if(nRow%2==0)
					lplvdr->clrText = m_even_textcor;//偶数行
				else
					lplvdr->clrText = m_odd_textcor;//奇数行
			}

			//背景色
			if(m_bkColorMode == en_Alist_Mode_SingleRow)
			{
				//单行模式
				lplvdr->clrText = pitemdata->m_bkcor;//背景颜色
			}
			else if(m_bkColorMode == en_Alist_Mode_Odd_Even)
			{
				//奇偶行模式
				if(nRow%2==0)
					lplvdr->clrTextBk = m_even_bkcor;//偶数行
				else
					lplvdr->clrTextBk = m_odd_bkcor;//奇数行
			}

//			CDC* pDC = CDC::FromHandle(lplvdr->nmcd.hdc);
//			CRect focusRect;
//			
//			GetItemRect(nRow, focusRect, LVIR_BOUNDS);
//			
//			if (lplvdr->nmcd.uItemState & ODS_FOCUS)
//			{//得到输入焦点，通过focusRect画虚线
//				if (lplvdr->nmcd.uItemState & ODS_SELECTED)
//				{
//					// 被按下，绘制下陷边框
//					CBrush br(RGB(21,134,56));
//					//pDC->FillRect(focusRect,&br);
//					CPen CurPen(PS_SOLID,2,RGB(0,0,0));
//					CPen *pOldPen = NULL;
//					pOldPen = pDC->SelectObject(&CurPen);
//					pDC->MoveTo(focusRect.left+1,focusRect.top+1);
//					pDC->LineTo(focusRect.right-1,focusRect.top+1);
//					pDC->LineTo(focusRect.right-1,focusRect.bottom-2);
//					pDC->LineTo(focusRect.left+1,focusRect.bottom-2);
//					pDC->LineTo(focusRect.left+1,focusRect.top+1);
//					pDC->SelectObject(pOldPen);
//					CurPen.DeleteObject();
//				}
//			}
//			else
//			{
//				CPen CurPen(PS_SOLID,2,RGB(20,50,255));
//				CPen *pOldPen = NULL;
//				pOldPen = pDC->SelectObject(&CurPen);
//				pDC->MoveTo(focusRect.left+1,focusRect.top+1);
//				pDC->LineTo(focusRect.right-1,focusRect.top+1);
//				pDC->LineTo(focusRect.right-1,focusRect.bottom-2);
//				pDC->LineTo(focusRect.left+1,focusRect.bottom-2);
//				pDC->LineTo(focusRect.left+1,focusRect.top+1);
//				pDC->SelectObject(pOldPen);
//				CurPen.DeleteObject();
//			}
		}

		if( m_bkColorMode == en_Alist_Mode_Cell_Separate || m_TextColorMode == en_Alist_Mode_Cell_Separate)
		{
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
		}
		else
		{
			*pResult = CDRF_DODEFAULT;
			//*pResult = CDRF_SKIPDEFAULT;	//该项需要自绘所有item
		}

		*pResult = CDRF_NOTIFYSUBITEMDRAW;

		break;
		
	case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
		//控制单元格显示
		if( m_bkColorMode == en_Alist_Mode_Cell_Separate || m_TextColorMode == en_Alist_Mode_Cell_Separate)
		{			
			tag_Aitemdata *pitemdata = (tag_Aitemdata*)(CListCtrl::GetItemData( nRow ));
			if( pitemdata )
			{
				COLORREF txtcor, bkcor;//颜色
				pitemdata->GetSubitemColor( lplvdr->iSubItem, txtcor, bkcor );
				if( m_TextColorMode == en_Alist_Mode_Cell_Separate )
				{
					lplvdr->clrText = txtcor;
				}
				if( m_bkColorMode == en_Alist_Mode_Cell_Separate )
				{
					lplvdr->clrTextBk = bkcor;
				}
			}
		}
//		if(lplvdr->iSubItem == 1)//列
//		{
//
//			if( lplvdr->nmcd.dwItemSpec == 0)//行
//			{
//				lplvdr->clrText = RGB(0,0,255);
//				lplvdr->clrTextBk = RGB(255,0,0); //红色
//			}
//			if( lplvdr->nmcd.dwItemSpec == 1)
//			{
//				lplvdr->clrTextBk = RGB(255,255,0); 
//			}
//			if( lplvdr->nmcd.dwItemSpec == 2)
//			{
//				lplvdr->clrTextBk = RGB(155,155,255); 
//			}
//			if( lplvdr->nmcd.dwItemSpec == 3)
//			{
//				lplvdr->clrTextBk = RGB(255,255,255); 
//			}
//		}

		*pResult = CDRF_DODEFAULT;
		
		break;
	case CDDS_POSTERASE:
//		{
//			CDC dc;  
//			dc.Attach(lplvdr->nmcd.hdc);  
//			CRect rectClient;  
//			GetClientRect(&rectClient);  
//			CBrush selectBrush;  
//			selectBrush.CreateSolidBrush(RGB(203, 223, 239));  
//			
//			dc.FillRect(&rectClient,&selectBrush);
//		}
		break;
	case CDDS_POSTPAINT :
//		//绘制item以外的部分
//		{
//			int nTop = lplvdr->nmcd.rc.top;  
//			    int nCount = GetItemCount();  
//			    if (nCount > 0)  
//				    {  
//				        CRect rcItem;  
//				        GetItemRect(nCount - 1, &rcItem, LVIR_LABEL);  
//				        nTop = rcItem.bottom;  
//				    }  
//			    CRect rectClient;  
//			    GetClientRect(&rectClient);  
//			    if (nTop < lplvdr->nmcd.rc.bottom) // 有剩余   
//				    {  
//				        CRect rcRemain = lplvdr->nmcd.rc;  
//				        rcRemain.top = nTop;  
//				        rcRemain.right = rectClient.right;  
////				        int nRemainItem = rcRemain.Height() / LIST_ITEM_HEIGHT;  
//						int nRemainItem = 1;
////				        if (rcRemain.Height() % LIST_ITEM_HEIGHT)  
////					        {  
////					            nRemainItem++;  
////					        }  
//				        int pos = GetScrollPos(SB_HORZ);  
//				        CDC dc;  
//				        dc.Attach(lplvdr->nmcd.hdc);  
//				        for (int i = 0; i < nRemainItem; ++i)  
//					        {  
//					            CRect rcItem;  
////					            rcItem.top = rcRemain.top + i * LIST_ITEM_HEIGHT;  
//								rcItem.top = 150;
//					            rcItem.left = rcRemain.left;  
//					            rcItem.right = rcRemain.right;  
////					            rcItem.bottom = rcItem.top + LIST_ITEM_HEIGHT;  
//								rcItem.bottom = 300;
//					            int nColumnCount = GetHeaderCtrl()->GetItemCount();  
//					            CRect  rcSubItem;  
//					            for (int j = 0; j < nColumnCount; ++j)  
//						            {  
//						                GetHeaderCtrl()->GetItemRect(j, &rcSubItem);  
//						                rcSubItem.top = rcItem.top;  
//						                rcSubItem.bottom = rcItem.bottom;  
//						                rcSubItem.OffsetRect(-pos, 0);  
//						                if(rcSubItem.right < rcRemain.left || rcSubItem.left > rcRemain.right)  
//							                    continue;
//
//										CBrush selectBrush;  
//										selectBrush.CreateSolidBrush(RGB(203, 223, 239));  
//										
//										dc.FillRect(&rcRemain,&selectBrush);
////						                draw_row_bg(&dc, rcSubItem, false, false, i + nCount);            
//						            }  
//					        }  
//				        dc.Detach();  
//				    }  
//		//	*pResult = CDRF_SKIPDEFAULT;
//		}
		break;
	}
}

int CAListCtrl::InsertItem(const LVITEM* pItem)
{
	LVITEM* pNewItem = (LVITEM*)pItem;
	
	tag_Aitemdata *pitemdata = new tag_Aitemdata( GetColumnCount() );	//每行的数据结构
	
	//原始itemdata赋值给pitemdata->m_lParam
	pitemdata->m_lParam = pItem->lParam;
	
	pNewItem->lParam = (LPARAM)pitemdata;	//将自定义的tag_Aitemdata结构地址作为listctrl的itemdata
	pNewItem->mask = pNewItem->mask | LVIF_PARAM;	
	
	int newRow = CListCtrl::InsertItem(pNewItem);
	if(newRow == -1)
	{
		delete pitemdata;	//插入失败释放资源
	}
	else
	{
		if(newRow <= m_CurRow)
			m_CurRow++;
	}
	
	return newRow;
}

int CAListCtrl::InsertItem(int nItem,LPCTSTR lpszItem)
{
	return InsertItem(LVIF_TEXT, nItem, lpszItem, 0, 0, 0, 0);
}

int CAListCtrl::InsertItem(int nItem,LPCTSTR lpszItem,int nImage)
{
	return InsertItem(LVIF_TEXT|LVIF_IMAGE, nItem, lpszItem, 0, 0, nImage, 0);
}

int CAListCtrl::InsertItem(UINT nMask,int nItem,LPCTSTR lpszItem,UINT nState,UINT nStateMask,int nImage,LPARAM lParam)
{
	//构造LVITEM, 然后调用CAListCtrl::InsertItem(const LVITEM* pItem)
	LVITEM lvitem;
	lvitem.mask = nMask;
	lvitem.iItem = nItem;
	lvitem.iSubItem = 0;
	lvitem.pszText = (char*)lpszItem;
	lvitem.cchTextMax = 0;
	
	if(nMask & LVIF_TEXT)
	{
		lvitem.cchTextMax = strlen(lvitem.pszText);
	}
	
	lvitem.state = nState;
	lvitem.stateMask = nStateMask;
	lvitem.iImage = nImage;
	lvitem.lParam = lParam;
	
	return ( InsertItem(&lvitem) );
}

BOOL CAListCtrl::GetItemTextColor(int nItem,COLORREF &cr)
{
	tag_Aitemdata *pitemdate = (tag_Aitemdata*)(CListCtrl::GetItemData(nItem));
	
	cr = pitemdate->m_textcor;

	return TRUE;
}

BOOL CAListCtrl::SetItemTextColor(int nItem,COLORREF cr)
{
	//单行模式
	tag_Aitemdata *pitemdate = (tag_Aitemdata*)(CListCtrl::GetItemData(nItem));
	pitemdate->m_textcor = cr;
	return TRUE;
}

BOOL CAListCtrl::SetItemTextBkColor(int nItem,COLORREF cr)
{
	//单行模式
	tag_Aitemdata *pitemdate = (tag_Aitemdata*)(CListCtrl::GetItemData(nItem));
	pitemdate->m_bkcor = cr;
	return TRUE;
}

BOOL CAListCtrl::SetSubItemTextColor(int nItem, int nSubitem, COLORREF cr)
{
	//单元格模式
	if( nItem >= GetItemCount() || nSubitem >= GetColumnCount() || nItem < 0 || nSubitem < 0 )
		return FALSE;

	tag_Aitemdata *pitemdate = (tag_Aitemdata*)(CListCtrl::GetItemData(nItem));
	
	if( pitemdate != NULL)
	{
		return pitemdate->SetSubitemColor( nSubitem, TRUE, cr, FALSE, 0);
	}
	
	return FALSE;
}

BOOL CAListCtrl::SetSubItemTextBkColor(int nItem, int nSubitem, COLORREF cr)
{
	//单元格模式
	if( nItem >= GetItemCount() || nSubitem >= GetColumnCount() || nItem < 0 || nSubitem < 0 )
		return FALSE;
	
	tag_Aitemdata *pitemdate = (tag_Aitemdata*)(CListCtrl::GetItemData(nItem));
	
	if( pitemdate != NULL)
	{
		return pitemdate->SetSubitemColor( nSubitem, FALSE, 0, TRUE, cr);
	}
	
	return FALSE;
}

//设置列是否可编辑，
//ColNum:列号
//bCanEditFlagL:FALSE----不可编辑;TRUE----可编辑
BOOL CAListCtrl::SetEdit(int nSubitem, BOOL bCanEdit)
{
	CHeaderCtrl* pHeader=this->GetHeaderCtrl();
	int nColumnCount=pHeader->GetItemCount();
	if ( nSubitem > nColumnCount )
	{
		return FALSE;
	}

	for ( int i=0; i<GetItemCount(); i++ )
	{
		SetEdit( i, nSubitem, bCanEdit );
	}
	return TRUE;
}
//设置列是否可编辑，
//ColName:列名
//bCanEditFlagL:FALSE----不可编辑;TRUE----可编辑
void CAListCtrl::SetEdit(CString nSubitem,BOOL bCanEdit)
{
	CHeaderCtrl* pHeader=this->GetHeaderCtrl();
	int nColumnCount=pHeader->GetItemCount();
	HDITEM pHeaderItem;
	char buf[100];
	pHeaderItem.mask = HDI_TEXT;
	pHeaderItem.cchTextMax = 99;
	pHeaderItem.pszText = buf;
	
	BOOL findflag = FALSE;
	for(int i=0;i<nColumnCount;i++)
	{	//查找对应的列
		pHeader->GetItem(i,&pHeaderItem);
		//if(ColName == pHeaderItem.pszText)
		if(_strcmpi(nSubitem,pHeaderItem.pszText)==0)
		{
			findflag = TRUE;
			SetEdit(i,bCanEdit);
		}
	}
	if(!findflag)
	{
		CString buf;
		buf.Format("CAListCtrl.SetEdit(CString ColName,BOOL bCanEditFlag) 参数1:%s错,\n请检查程序与数据库中字段命名是否一致",nSubitem);
		//		AfxMessageBox(buf);
	}
}

BOOL CAListCtrl::GetEdit(int nItem, int nSubitem)
{
	tag_Aitemdata *pitemdata = (tag_Aitemdata*)(CListCtrl::GetItemData( nItem ));
	if( pitemdata )
	{
		return pitemdata->GetSubitemEdit( nSubitem );
	}	
	return FALSE;
}

BOOL CAListCtrl::SetEdit(int nItem, int nSubitem, BOOL bCanEdit)
{
	//单元格模式
	if( nItem >= GetItemCount() || nSubitem >= GetColumnCount() || nItem < 0 || nSubitem < 0 )
		return FALSE;
	
	tag_Aitemdata *pitemdate = (tag_Aitemdata*)(CListCtrl::GetItemData(nItem));
	
	if( pitemdate != NULL)
	{
		return pitemdate->SetSubitemEdit( nSubitem, bCanEdit );
	}
	
	return FALSE;
}

void CAListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bLBtDown = FALSE;
	CListCtrl::OnLButtonUp(nFlags, point);
}

void CAListCtrl::SetTextCorMode(int mode)
{
	if( mode < 0 || mode >= en_Alist_Mode_Count )
		return;
	m_TextColorMode = mode;
}

void CAListCtrl::SetBkCorMode(int mode)
{
	if( mode < 0 || mode >= en_Alist_Mode_Count )
		return;
	m_bkColorMode = mode;
}

void CAListCtrl::SetOddTextColor(COLORREF newCor)
{
	m_odd_textcor = newCor;
}

void CAListCtrl::SetEvenTextColor(COLORREF newCor)
{
	m_even_textcor = newCor;
}

void CAListCtrl::SetOddBkColor(COLORREF newCor)
{
	m_odd_bkcor = newCor;
}

void CAListCtrl::SetEvenBKColor(COLORREF newCor)
{
	m_even_bkcor = newCor;
}

int CAListCtrl::GetCurRow()
{
	return m_CurRow;
}

int CAListCtrl::GetCurCol()
{
	return m_CurCol;
}

void CAListCtrl::InitToolTip()
{
	if(m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create(this, TTS_NOPREFIX | TTS_ALWAYSTIP);
		m_ToolTip.Activate(FALSE);
		m_ToolTip.SetDelayTime(300);
		// Enable multiline
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 100);
		//m_ToolTip.SendMessage(TTM_SETTITLE, TTI_INFO, (LPARAM)_T("Title"));
	} // if
}

void CAListCtrl::SetTooltipText(int nText, BOOL bActivate)
{
	CString sText;
	
	// Load string resource
	sText.LoadString(nText);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE)
		SetTooltipText((LPCTSTR)sText, bActivate);
} // End of SetTooltipText

void CAListCtrl::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (lpszText == NULL) return;
	
	// Initialize ToolTip
	InitToolTip();
	
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, lpszText, rectBtn, 1);
	} // if
	
	// Set text for tooltip
	m_ToolTip.UpdateTipText(lpszText, this, 1);
	m_ToolTip.Activate(bActivate);
}// End of SetTooltipText

//CAListCtrl &CAListCtrl::operator = (CAListCtrl &srclist)
//{
//	DeleteAllItems();
//	DeleteAllColumn();
//
//	//颜色显示
//	m_bkColorMode = srclist.m_bkColorMode;	//背景色显示模式
//	m_TextColorMode = srclist.m_TextColorMode;	//文本色显示模式
//	
//	m_odd_bkcor = srclist.m_odd_bkcor;//奇数行背景颜色
//	m_even_bkcor = srclist.m_even_bkcor;//偶数行背景颜色
//	
//	m_odd_textcor = srclist.m_odd_textcor;//奇数行文字颜色
//	m_even_textcor = srclist.m_even_textcor;//偶数行文字颜色
//	
//	//复制列头
//	for(int i=0;i<srclist.GetColumnCount();i++)
//	{
//		CString columtxt;
//		if(!srclist.GetColumnText(i,columtxt))
//			continue;
//		if(-1 == InsertColumn(i,columtxt))
//			continue;
//
//		SetEdit(i,srclist.GetEdit(i));
//	}
//
//	//复制行
//	for(i=0;i<srclist.GetItemCount();i++)
//	{
//		InsertItem(i,"");
//		for(int j=0;j<srclist.GetColumnCount();j++)
//		{
//			CString celltxt = srclist.GetItemText(i,j);
//			SetItemText(i,j,celltxt);
//		}
//	}
//
//	//复制每行的自定义数据
//	m_itemlist.RemoveAll();
//	POSITION pos = srclist.m_itemlist.GetHeadPosition();
//	while(pos != NULL)
//		m_itemlist.AddTail(srclist.m_itemlist.GetNext(pos));
//
//	//复制tooltip
//	if(srclist.m_ToolTip.m_hWnd)
//	{
//		InitToolTip();
//
//		CString tiptxt;
//		srclist.m_ToolTip.GetText(tiptxt,&srclist);
//		SetTooltipText((LPCTSTR)tiptxt);
//	}
//	return *this;
//}

////交换两行数据
//void CAListCtrl::ChangeRow(int FirstIndex,int SecondIndex)
//{
//	if(FirstIndex == SecondIndex)
//		return;
//	//复制单元中数据
//	CString celltxt;
//	for(int j=0;j<GetColumnCount();j++)
//	{
//		celltxt = GetItemText(FirstIndex,j);	//保存fisrt中内容
//		SetItemText(FirstIndex,j,GetItemText(SecondIndex,j));//将sec赋给first
//		SetItemText(SecondIndex,j,celltxt);	//将celltxt赋给Sec
//	}
//
//	//复制自定义数据
//	if(FirstIndex > SecondIndex)
//	{
//		int a = FirstIndex;
//		FirstIndex = SecondIndex;
//		SecondIndex = a;
//	}
//
//	tag_Aitemdata firstnode;
//
//	POSITION Firstpos = m_itemlist.FindIndex(FirstIndex);
//	POSITION Secondpos = m_itemlist.FindIndex(SecondIndex);
//
//	firstnode = m_itemlist.GetAt(Firstpos);
//	m_itemlist.SetAt(Firstpos,m_itemlist.GetAt(Secondpos));
//	m_itemlist.SetAt(Secondpos,firstnode);
//}
//
////默认升序排列
//void CAListCtrl::SortListby1Colum(int ColumIndex,int CellType,BOOL bASC)
//{
//	if(ColumIndex<0 || ColumIndex>GetColumnCount()-1)
//		return;
//
//	CString buf1,buf2;
//	for(int i=0;i<GetItemCount();i++)
//	{
//		buf1 = GetItemText(i,ColumIndex);
//		int tempindex = i;
//		for(int j=i+1;j<GetItemCount();j++)
//		{	
//			buf2 = GetItemText(j,ColumIndex);
//			BOOL b = FALSE;
//			if(CellType == Sort_S)	b = buf1>buf2;
//			else if(CellType == Sort_N)	b = atoi(buf1)>atoi(buf2);
//			else if(CellType == Sort_F)	b = atof(buf1)>atof(buf2);
//			else	return;
//			if(bASC)
//			{
//				//升序
//				if(b)
//				{
//					buf1 = buf2;
//					tempindex = j;
//				}
//			}
//			else
//			{
//				//降序
//				if(!b)
//				{
//					buf1 = buf2;
//					tempindex = j;
//				}
//			}
//		}
//		if(i != tempindex)
//			ChangeRow(i,tempindex);
//	}
//}
//
////默认升序排列
//void CAListCtrl::SortListby2Colum(int ColumIndex1,int CellType1,int ColumIndex2,int CellType2,
//		BOOL bASC1,BOOL bASC2)
//{
//	if(ColumIndex1<0 || ColumIndex1>GetColumnCount()-1)
//		return;
//	if(ColumIndex2<0 || ColumIndex2>GetColumnCount()-1)
//		return;
//	
//	//先排主列
//	SortListby1Colum(ColumIndex1,CellType1,bASC1);
//
//	//再排副列
//	CString buf1,buf2;
//	for(int i=0;i<GetItemCount();i++)
//	{
//		CString MainTxt = GetItemText(i,ColumIndex1);	//第i行主列的内容
//		buf1 = GetItemText(i,ColumIndex2);	//第i行副列的内容
//		int tempindex = i;
//		for(int j=i+1;j<GetItemCount();j++)
//		{
//			if(GetItemText(j,ColumIndex1) != MainTxt)	//判断主列名是否相同，相同的才排序
//				break;
//			buf2 = GetItemText(j,ColumIndex2);	//第j行副列的内容
//			BOOL b = FALSE;
//			if(CellType2 == Sort_S)	b = buf1>buf2;
//			else if(CellType2 == Sort_N)	b = atoi(buf1)>atoi(buf2);
//			else if(CellType2 == Sort_F)	b = atof(buf1)>atof(buf2);
//			else	return;
//			if(bASC2)
//			{
//				//升序
//				if(b)
//				{
//					buf1 = buf2;
//					tempindex = j;
//				}
//			}
//			else
//			{
//				//降序
//				if(!b)
//				{
//					buf1 = buf2;
//					tempindex = j;
//				}
//			}
//		}
//		if(i != tempindex)
//			ChangeRow(i,tempindex);
//	}
//}

BOOL CAListCtrl::SetIndent( int nItem, int iIndent )
{
	LVITEM lvItem;
	memset(&lvItem, 0, sizeof(LVITEM));
	lvItem.mask = LVIF_INDENT;	//LVIF_INDENT只在iSubItem=0时有效
	lvItem.iItem = nItem;
	lvItem.iSubItem  = 0;
	
	if( GetItem(&lvItem) )
	{
		//缩进iIndent个图标的位置
		lvItem.iIndent = iIndent;
		return SetItem(&lvItem);
	}
	else
	{
		return FALSE;
	}
}

int CAListCtrl::GetIndent( int nItem )
{
	LVITEM lvItem;
	memset(&lvItem, 0, sizeof(LVITEM));
	lvItem.mask = LVIF_INDENT ;
	lvItem.iItem = nItem;
	lvItem.iSubItem  = 0;
	
	GetItem(&lvItem);
	
	return lvItem.iIndent;
}

BOOL CAListCtrl::SetItemData(int nItem, DWORD dwData)
{
	//构造tag_Aitemdata, 然后调用CListCtrl::SetItemData(int nItem, DWORD dwData)
	tag_Aitemdata *pitemdate = (tag_Aitemdata*)( CListCtrl::GetItemData( nItem ) );

	if( !pitemdate )
	{
		pitemdate = new tag_Aitemdata;
		pitemdate->m_lParam = dwData;
		return CListCtrl::SetItemData( nItem, DWORD(pitemdate) );
	}
	else
	{
		pitemdate->m_lParam = dwData;
		return TRUE;
	}
}

DWORD CAListCtrl::GetItemData(int nItem) const
{
	tag_Aitemdata *pitemdate = (tag_Aitemdata*)(CListCtrl::GetItemData(nItem));
	if( pitemdate )
		return pitemdate->m_lParam;
	else
		return 0;
}

BOOL CAListCtrl::GetItem(LVITEM* pItem) const
{
	BOOL bResult = CListCtrl::GetItem(pItem);

	if (bResult && pItem->mask & LVIF_PARAM)
	{
		// 替换 lParam , 现在的lParam是一个指向tag_Aitemdata对象的指针
		// 实际的lParam 存储在 tag_Aitemdata 中的 m_lParam 变量中
		tag_Aitemdata * pitemdate = (tag_Aitemdata*)(pItem->lParam);
		pItem->lParam = pitemdate->m_lParam;
	}

	return bResult;
}

BOOL CAListCtrl::SetItem( const LVITEM* pItem )
{
	if( pItem->iItem < 0 || pItem->iItem >= GetItemCount() )
		return FALSE;
	
	LVITEM pNewItem = (LVITEM) *pItem;
	
	tag_Aitemdata *pitemdate = (tag_Aitemdata*)( CListCtrl::GetItemData( pItem->iItem ) );

	if( !pitemdate )
	{
		pitemdate = new tag_Aitemdata;
	}

	if ( pItem->mask & LVIF_PARAM )
	{
		// Management of lParam
		pitemdate->m_lParam = pNewItem.lParam;
	}
	
	pNewItem.lParam = LPARAM( pitemdate );
	
	return (CListCtrl::SetItem( &pNewItem ));
}

BOOL CAListCtrl::SetItem( int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
		int nImage, UINT nState, UINT nStateMask, LPARAM lParam )
{
	// Create an LVITEM and insert it
	LVITEM lvitem;
	lvitem.mask = nMask;
	lvitem.iItem = nItem;
	lvitem.iSubItem = nSubItem;
	lvitem.pszText = (char*)lpszItem;
	lvitem.cchTextMax = 0;
	
	if(nMask & LVIF_TEXT)
	{
		lvitem.cchTextMax = strlen(lvitem.pszText);
	}
	
	lvitem.state = nState;
	lvitem.stateMask = nStateMask;
	lvitem.iImage = nImage;
	lvitem.lParam = lParam;
	lvitem.iIndent = 0;
	
	// Insert it
	return ( SetItem( &lvitem ) );
}

void CAListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, LVS_REPORT, SWP_FRAMECHANGED);

	CListCtrl::PreSubclassWindow();
}

void CAListCtrl::OnDestroy() 
{
	TRACE( "CAListCtrl::OnDestroy()\n" );

	DeleteAllItems();	// 释放空间

	CListCtrl::OnDestroy();
	
	// TODO: Add your message handler code here		
}


// void CAListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
// {
// 	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
// 	// TODO: 在此添加控件通知处理程序代码
// 	*pResult = 0;
// }
