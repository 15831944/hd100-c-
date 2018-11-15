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

//����ʱ�����Ͷ���
#define Sort_S		0	//�ַ���
#define Sort_N		1	//����
#define Sort_F		2	//������

enum AListCtrlItemMode	//item��ʾģʽ(��ɫ��)
{
	en_Alist_Mode_Odd_Even = 0,	//��ż��ģʽ(����Ĭ��ģʽ)
	en_Alist_Mode_SingleRow,	//���и�ʽ��ÿ����ɫ��������ģʽ(�ı�Ĭ��ģʽ)
	en_Alist_Mode_Cell_Separate,//��Ԫ��ģʽ,ÿ�����ӵ���������ɫ
	en_Alist_Mode_Count
};

//////////////////////////////////////////////////////////////////////////
// class tag_Aitemdata
// listctrl ÿһ����Ԫ������ݽṹ(en_Alist_Mode_Cell_Separate)
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
	
	//���θ��Ʋ���
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
// listctrlÿ�е����ݽṹ
class tag_Aitemdata
{
public:
	LPARAM	m_lParam;	// The user-32 bits data lParam member for origin itemdata

	COLORREF	m_textcor;	//����ģʽen_Alist_Mode_SingleRowʱ�ı���ɫ
	COLORREF	m_bkcor;	//����ģʽen_Alist_Mode_SingleRowʱ������ɫ
private:
	CArray < tag_Asubitemdata*, tag_Asubitemdata* > m_SubitemArray;	//������ÿ��cell�������б�(en_Alist_Mode_Cell_Separate)
	
public:
	tag_Aitemdata( int nSubitemCount = 0 );
	~tag_Aitemdata();

	// ban copying.
	tag_Aitemdata( const tag_Aitemdata& );
	tag_Aitemdata& operator=( const tag_Aitemdata& );

	//���θ��Ʋ���
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
	BOOL m_bAlwaysSelect;		//һֱѡ��״̬
	BOOL m_bLBtDown;			//���״̬
	int m_LastRow;				//��һ��ѡ�е��к�

private:
	CEdit m_edit1;				//�༭��

	int m_CurRow,m_CurCol;		//��ǰѡ�е����кţ��������Ҽ�����˫��,�������¼�ͷ��pageup��pagedown
	int CurEditRow,CurEditCol;	//���ڱ༭��cell���к�

	int m_width[100];

	CToolTipCtrl m_ToolTip;
	
//	CList<tag_Aitemdata,tag_Aitemdata> m_itemlist;	//��������
//	CList<BOOL,BOOL> m_ColEdit;

	int m_bkColorMode;	//����ɫ��ʾģʽ
	int m_TextColorMode;	//�ı�ɫ��ʾģʽ

	COLORREF m_odd_bkcor;//�����б�����ɫ
	COLORREF m_even_bkcor;//ż���б�����ɫ

	COLORREF m_odd_textcor;//������������ɫ
	COLORREF m_even_textcor;//ż����������ɫ

// Operations
public:
	void InitToolTip();
	void SetTooltipText(int nText, BOOL bActivate=TRUE);
	void SetTooltipText(LPCTSTR lpszText, BOOL bActivate=TRUE);

	//��������һ�����ڱ�־ÿ���Ƿ�ɱ༭����Ͽ�
	//����Ҫ����д(���ػ�����ͬ��Ա����)����/ɾ���еĲ�����Ĭ�ϲ��ܱ༭
	int InsertColumn(int nCol,const LVCOLUMN* pColumn,BOOL bCanEdit = FALSE);
	int InsertColumn(int nCol,LPCTSTR lpszColumnHeading,int nFormat = LVCFMT_LEFT,
							int nWidth = -1,int nSubItem = -1,BOOL bCanEdit = FALSE);
	BOOL DeleteColumn(int nCol);
	BOOL DeleteAllColumn();
	
	//ͨ���кź�������ö�Ӧ�е��ı�
	CString GetItemTextByHeadStr(int nItem, CString HeadStr, BOOL bCaseCare=FALSE);
	int GetColumIndexByHeadStr(CString HeadStr);

	//ͨ���кź����������ı�
	BOOL SetItemTextByHeadStr(int nItem, CString HeadStr, LPCTSTR lpszText, BOOL bCaseCare=FALSE);

	int GetCurRow();
	int GetCurCol();

	//////////////////////////////////////////////////////////////////////////
	//�ض���(����)CListCtrl�Ĳ��ֺ���
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
	BOOL DeleteAllItems();//�ú�������ɾ�������ж���ɾ����

	inline CString GetItemText(int nItem, int nSubItem) const;
	inline int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const;

	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
	//////////////////////////////////////////////////////////////////////////

	//������iIndentΪͼ���ȵı���
	BOOL SetIndent( int nItem, int iIndent );
	int  GetIndent( int nItem );

	//��ɫģʽ
	void SetTextCorMode(int mode);
	void SetBkCorMode(int mode);

	//����ģʽ
	BOOL GetItemTextColor(int nItem, COLORREF &cr);
	BOOL SetItemTextColor(int nItem, COLORREF cr);
	BOOL SetItemTextBkColor(int nItem, COLORREF cr);

	//��ż��ģʽ
	void SetOddTextColor(COLORREF newCor);
	void SetEvenTextColor(COLORREF newCor);
	void SetOddBkColor(COLORREF newCor);
	void SetEvenBKColor(COLORREF newCor);

	//��Ԫ��ģʽen_Alist_Mode_Cell_Separate
	BOOL SetSubItemTextColor(int nItem, int nSubitem, COLORREF cr);
	BOOL SetSubItemTextBkColor(int nItem, int nSubitem, COLORREF cr);

//	//������������
//	void ChangeRow(int FirstIndex,int SecondIndex);
//	//����
//	void SortListby1Colum(int ColumIndex,int CellType,BOOL bASC = TRUE);	//Ĭ����������
//	void SortListby2Colum(int ColumIndex1,int CellType1,int ColumIndex2,int CellType2,
//		BOOL bASC1 = TRUE,BOOL bASC2 = TRUE);	//Ĭ����������

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

	//�������Ƿ�ɱ༭
	void SetEdit(CString nSubitem, BOOL bCanEdit);			//���п���
	BOOL SetEdit(int nSubitem, BOOL bCanEdit);				//���п���
	BOOL SetEdit(int nItem, int nSubitem, BOOL bCanEdit);	//������Ԫ�����
	BOOL GetEdit(int nItem, int nSubitem);


	virtual ~CAListCtrl();
	
	BOOL PreTranslateMessage(MSG* pMsg);
	// Generated message map functions	

	void ResetColWidth(int col=-1);	//������ͷ�����п�

	void AdjustAllColumn();
	void AdjustColWidth(int Row, int Col);
	void AdjustColWidth(int Row, int Col,CString text);

	BOOL GetColumnText(int nSubItem, CString &text);//�����ͷtext
	BOOL SetColumnText(int nCol, LPSTR newHeadTxt);
	int GetColumnCount();//�������

//	CAListCtrl &operator = (CAListCtrl &srclist);
	
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);//�������صĺ���
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
