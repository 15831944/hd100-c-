// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "ColorButton.h"


// CColorButton

IMPLEMENT_DYNAMIC(CColorButton, CButton)

CColorButton::CColorButton()
{
	m_bOver = FALSE;
	m_bTracking = FALSE;
	m_bSelected = FALSE;
	m_bFocus = FALSE;
	m_bDisable = FALSE;
	m_iIndex = 0;

	m_Font.CreatePointFont(75, "Arial Black", NULL);

	//m_pController = NULL;
}

CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)

END_MESSAGE_MAP()



// CColorButton message handlers

BOOL CColorButton::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	return CButton::PreCreateWindow(cs);
}

void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//从lpDrawItemStruct获取控件的相关信息
	CRect rect=lpDrawItemStruct->rcItem;
	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	int nSaveDC=pDC->SaveDC();

	UINT state = lpDrawItemStruct->itemState;
	CString strText;
	GetWindowText(strText);
	int iOldMode = pDC->SetBkMode(TRANSPARENT);
	CFont *pOldFont = pDC->SelectObject(&m_Font);
	pDC->SetTextColor(RGB(15, 88, 110));

	//////////////////////////////////////////////////////////////////////////	
	//获取按钮的状态
	if (state & ODS_FOCUS)
	{
		m_bFocus = TRUE;
		m_bSelected = TRUE;
	}
	else
	{
		m_bFocus = FALSE;
		m_bSelected = FALSE;
	}

	if (state & ODS_SELECTED || state & ODS_DEFAULT)
	{
		m_bFocus = TRUE;
	}
	
	//////////////////////////////////////////////////////////////////////////
	CSize Size = pDC->GetTextExtent(strText);
	CDC cdc;
	cdc.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = NULL;
	pDC->SetTextColor(RGB(200,0,250));
	pOldBitmap = cdc.SelectObject(&m_Bitmap);
	pDC->StretchBlt(0,0,rect.Width(), rect.Height(), &cdc, 48*m_iIndex, 0, 48, 48, SRCCOPY);
	pDC->TextOut(2,22, strText);
	CPen Pen(PS_SOLID, 2, RGB(255,0,50));
	CPen *pOldPen = pDC->SelectObject(&Pen);

	cdc.SelectObject(pOldBitmap);
	cdc.DeleteDC();

	//Draw OutSide
	CGdiObject *pOldBrush = pDC->SelectStockObject(NULL_BRUSH);
	if(m_bOver)	pDC->RoundRect(rect, CPoint(5,5));

	//////////////////////////////////////////////////////////////////////////
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(iOldMode);
	pDC->RestoreDC(nSaveDC);

}
void CColorButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}


LRESULT CColorButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	m_bOver = FALSE;
	m_bTracking = FALSE;
	InvalidateRect(NULL, FALSE);
	return 0;
}

LRESULT CColorButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	m_bOver = TRUE;
	InvalidateRect(NULL);
	return 0;
}

void CColorButton::SetBitmapEx(UINT ID)
{
	m_Bitmap.DeleteObject();
	m_Bitmap.LoadBitmap(ID);
}
void CColorButton::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	SetButtonStyle(GetButtonStyle() |BS_OWNERDRAW |BS_BITMAP);

	CButton::PreSubclassWindow();
}