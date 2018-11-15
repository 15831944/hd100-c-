#if !defined(AFX_NEWLIST_H__F4C02AFA_F4FA_42D8_8765_28C8D2FD7FB4__INCLUDED_)
#define AFX_NEWLIST_H__F4C02AFA_F4FA_42D8_8765_28C8D2FD7FB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AListCtrl.h : header file
//
#include <afxtempl.h>

#define IDC_AListCtrl_EDIT_20050228		50228
#define IDC_AListCtrl_COMBO_20050228	50229

//排序时的类型定义
#define Sort_S		0	//字符串
#define Sort_N		1	//整数
#define Sort_F		2	//浮点数

enum AListCtrlItemMode	//item显示模式(颜色等)
{
	en_Alist_Mode_Odd_Even = 0,	//奇偶行模式(背景默认模式)
	en_Alist_Mode_SingleRow,	//单行格式，每行颜色独立控制模式(文本默认模式)
	en_Alist_Mode_Cell_Separate,//单元格模式,每个格子单独控制颜色
	en_Alist_Mode_Count
};

//////////////////////////////////////////////////////////////////////////
// class tag_Aitemdata
// listctrl 每一个单元格的数据结构(en_Alist_Mode_Cell_Separate)
class tag_Asubitemdata
{
public:
	BOOL		m_bCanEdit;
	COLORREF	m_textcor;
	COLORREF	m_bkcor;
	CFont		*m_pTxtFont;
	HFONT		m_hFont;

public:
	tag_Asubitemdata()
	{
		m_bCanEdit	= FALSE;
		m_textcor	= RGB(0,0,0);
		m_bkcor		= RGB(250,250,250);
//		m_pTxtFont	= NULL;
//		m_hFont		= NULL;
	}
	
	//屏蔽复制操作
	tag_Asubitemdata(const tag_Asubitemdata &src);
	tag_Asubitemdata &operator = (const tag_Asubitemdata &Src);
	
//	tag_Asubitemdata(const tag_Asubitemdata &src)
//	{
//		*this = src;
//	}
//	tag_Asubitemdata &operator = (const tag_Asubitemdata &Src)
//	{
//		m_textcor	= Src.m_textcor;
//		m_bkcor		= Src.m_bkcor;
////	m_pTxtFont	= Src.m_pTxtFont;
//
//		return *this;
//	}
};

//////////////////////////////////////////////////////////////////////////
// class tag_Aitemdata
// listctrl每行的数据结构
class tag_Aitemdata
{
public:
	LPARAM	m_lParam;	// The user-32 bits data lParam member for origin itemdata

	COLORREF	m_textcor;	//单行模式en_Alist_Mode_SingleRow时文本颜色
	COLORREF	m_bkcor;	//单行模式en_Alist_Mode_SingleRow时背景颜色
private:
	CArray < tag_Asubitemdata*, tag_Asubitemdata* > m_SubitemArray;	//该行中每个cell的数据列表(en_Alist_Mode_Cell_Separate)
	
public:
	tag_Aitemdata( int nSubitemCount = 0 );
	~tag_Aitemdata();

	// ban copying.
	tag_Aitemdata( const tag_Aitemdata& );
	tag_Aitemdata& operator=( const tag_Aitemdata& );

	//屏蔽复制操作
//	tag_Aitemdata(const tag_Aitemdata &src)
//	{
//		*this = src;
//	}
//
//	tag_Aitemdata &operator = (const tag_Aitemdata &Src)
//	{
//		lParam		= Src.lParam;
//		bParam		= Src.bParam;
//		
//		textcor		= Src.textcor;
//		bkcor		= Src.bkcor;
//		
//		m_SubitemArray = Src.m_SubitemArray;
//		
//		return *this;
//	}
	
	void freeSubitemList();

	BOOL insertSubitem( int nSubitem );
	BOOL deleteSubitem( int nSubitem );

	BOOL SetSubitemColor( int nSubitem, BOOL btxt, COLORREF txtcr, BOOL bBk, COLORREF bkcor );
	BOOL GetSubitemColor( int nSubitem, COLORREF &txtcr, COLORREF &bkcor );

	BOOL SetSubitemEdit( int nSubitem, BOOL bCanEdit );
	BOOL GetSubitemEdit( int nSubitem );
};


/////////////////////////////////////////////////////////////////////////////
// CAListCtrl window
class CAListCtrl : public CListCtrl
{
// Construction
public:
	CAListCtrl();
	CAListCtrl(CAListCtrl &srclist);

// Attributes
public:
	BOOL m_bAlwaysSelect;		//一直选中状态
	BOOL m_bLBtDown;			//左键状态
	int m_LastRow;				//上一次选中的行号

private:
	CEdit m_edit1;				//编辑框

	int m_CurRow,m_CurCol;		//当前选中的行列号，包括左右键单击双击,键盘上下箭头、pageup、pagedown
	int CurEditRow,CurEditCol;	//正在编辑的cell行列号

	int m_width[100];

	CToolTipCtrl m_ToolTip;
	
//	CList<tag_Aitemdata,tag_Aitemdata> m_itemlist;	//单行数据
//	CList<BOOL,BOOL> m_ColEdit;

	int m_bkColorMode;	//背景色显示模式
	int m_TextColorMode;	//文本色显示模式

	COLORREF m_odd_bkcor;//奇数行背景颜色
	COLORREF m_even_bkcor;//偶数行背景颜色

	COLORREF m_odd_textcor;//奇数行文字颜色
	COLORREF m_even_textcor;//偶数行文字颜色

// Operations
public:
	void InitToolTip();
	void SetTooltipText(int nText, BOOL bActivate=TRUE);
	void SetTooltipText(LPCTSTR lpszText, BOOL bActivate=TRUE);

	//由于增加一个用于标志每列是否可编辑的组合框
	//所以要重新写(隐藏基类相同成员函数)插入/删除列的操作，默认不能编辑
	int InsertColumn(int nCol,const LVCOLUMN* pColumn,BOOL bCanEdit = FALSE);
	int InsertColumn(int nCol,LPCTSTR lpszColumnHeading,int nFormat = LVCFMT_LEFT,
							int nWidth = -1,int nSubItem = -1,BOOL bCanEdit = FALSE);
	BOOL DeleteColumn(int nCol);
	BOOL DeleteAllColumn();
	
	//通过行号和列名获得对应列的文本
	CString GetItemTextByHeadStr(int nItem, CString HeadStr, BOOL bCaseCare=FALSE);
	int GetColumIndexByHeadStr(CString HeadStr);

	//通过行号和列名设置文本
	BOOL SetItemTextByHeadStr(int nItem, CString HeadStr, LPCTSTR lpszText, BOOL bCaseCare=FALSE);

	int GetCurRow();
	int GetCurCol();

	//////////////////////////////////////////////////////////////////////////
	//重定义(隐藏)CListCtrl的部分函数
	BOOL GetItem(LVITEM* pItem) const;
	BOOL SetItem(const LVITEM* pItem);
	BOOL SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
					int nImage, UINT nState, UINT nStateMask, LPARAM lParam);

	BOOL SetItemData(int nItem, DWORD dwData);
	DWORD GetItemData(int nItem) const;

	int InsertItem(const LVITEM* pItem);
	int InsertItem(int nItem, LPCTSTR lpszItem);
	int InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
	int InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, 
					UINT nStateMask, int nImage, LPARAM lParam);

	BOOL DeleteItem( int nItem );
	BOOL DeleteAllItems();//该函数仅仅删除所有行而不删除列

	inline CString GetItemText(int nItem, int nSubItem) const;
	inline int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const;

	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
	//////////////////////////////////////////////////////////////////////////

	//缩进，iIndent为图标宽度的倍数
	BOOL SetIndent( int nItem, int iIndent );
	int  GetIndent( int nItem );

	//颜色模式
	void SetTextCorMode(int mode);
	void SetBkCorMode(int mode);

	//单行模式
	BOOL GetItemTextColor(int nItem, COLORREF &cr);
	BOOL SetItemTextColor(int nItem, COLORREF cr);
	BOOL SetItemTextBkColor(int nItem, COLORREF cr);

	//奇偶行模式
	void SetOddTextColor(COLORREF newCor);
	void SetEvenTextColor(COLORREF newCor);
	void SetOddBkColor(COLORREF newCor);
	void SetEvenBKColor(COLORREF newCor);

	//单元格模式en_Alist_Mode_Cell_Separate
	BOOL SetSubItemTextColor(int nItem, int nSubitem, COLORREF cr);
	BOOL SetSubItemTextBkColor(int nItem, int nSubitem, COLORREF cr);

//	//交换两行数据
//	void ChangeRow(int FirstIndex,int SecondIndex);
//	//排序
//	void SortListby1Colum(int ColumIndex,int CellType,BOOL bASC = TRUE);	//默认升序排列
//	void SortListby2Colum(int ColumIndex1,int CellType1,int ColumIndex2,int CellType2,
//		BOOL bASC1 = TRUE,BOOL bASC2 = TRUE);	//默认升序排列

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAListCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SelectNone();
	void SetCurSel(int nItem);
	void SetCurSel(int nItem, int nSubItem);
	int HitTestEx(CPoint& Point, int* pColumn);
	void EditSubItem(int Item, int Column);
	void EditCellShow(DWORD dwAlignStyle, CRect rect, int nItem, int nSubitem);
	void UpdateText();
	BOOL IsEditing();

	//设置列是否可编辑
	void SetEdit(CString nSubitem, BOOL bCanEdit);			//整列控制
	BOOL SetEdit(int nSubitem, BOOL bCanEdit);				//整列控制
	BOOL SetEdit(int nItem, int nSubitem, BOOL bCanEdit);	//单个单元格控制
	BOOL GetEdit(int nItem, int nSubitem);


	virtual ~CAListCtrl();
	
	BOOL PreTranslateMessage(MSG* pMsg);
	// Generated message map functions	

	void ResetColWidth(int col=-1);	//根据列头重置列宽

	void AdjustAllColumn();
	void AdjustColWidth(int Row, int Col);
	void AdjustColWidth(int Row, int Col,CString text);

	BOOL GetColumnText(int nSubItem, CString &text);//获得列头text
	BOOL SetColumnText(int nCol, LPSTR newHeadTxt);
	int GetColumnCount();//获得列数

//	CAListCtrl &operator = (CAListCtrl &srclist);
	
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);//必需重载的函数
//	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//{{AFX_MSG(CAListCtrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKillFocusEdit1();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWLIST_H__F4C02AFA_F4FA_42D8_8765_28C8D2FD7FB4__INCLUDED_)
