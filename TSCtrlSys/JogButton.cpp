// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "JogButton.h"


// CColorButton

IMPLEMENT_DYNAMIC(CJogButton, CButton)

double CJogButton::m_dJogSpdRatio = 1;
double CJogButton::s_fStepDist = 0;
BOOL CJogButton::s_bEnableStep = FALSE;
BOOL CJogButton::m_bEnableTrack = FALSE;

CJogButton::CJogButton()
{
	m_bOver = FALSE;
	m_bTracking = FALSE;
	m_bSelected = FALSE;
	m_bFocus = FALSE;
	m_bDisable = FALSE;
	m_iIndex = 0;

	m_Font.CreatePointFont(75, "Arial Black", NULL);
}

CJogButton::~CJogButton()
{
}


BEGIN_MESSAGE_MAP(CJogButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CColorButton message handlers

BOOL CJogButton::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	return CButton::PreCreateWindow(cs);
}

void CJogButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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

	/*
	if (m_bSelected&&m_bFocus)
		pDC->Draw3dRect(rect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHIGHLIGHT));
	else
		pDC->Draw3dRect(rect, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_BTNSHADOW));
	*/

	CPen Pen(PS_SOLID, 2, RGB(0,255,50));
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

void CJogButton::OnMouseMove(UINT nFlags, CPoint point) 
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


LRESULT CJogButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	m_bOver = FALSE;
	m_bTracking = FALSE;
	InvalidateRect(NULL, FALSE);
	return 0;
}

LRESULT CJogButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	m_bOver = TRUE;
	InvalidateRect(NULL);
	return 0;
}

void CJogButton::SetBitmapEx(UINT ID)
{
	m_Bitmap.DeleteObject();
	m_Bitmap.LoadBitmap(ID);
}
void CJogButton::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	SetButtonStyle(GetButtonStyle() |BS_OWNERDRAW |BS_BITMAP);

	CButton::PreSubclassWindow();
}

void CJogButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CButton::OnLButtonUp(nFlags, point);
	m_bSelected = FALSE;

	if(g_pFrm->m_Robot->m_pController != NULL)
	{
		ReleaseCapture();
		if (!s_bEnableStep)
		{
			g_pFrm->m_Robot->m_pController->AxisStop(0xFF);
		}
	}
}

void CJogButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CButton::OnLButtonDown(nFlags, point);
	if(g_pFrm->m_Robot->m_pController != NULL)
	{
		SetCapture();
		switch(GetDlgCtrlID())
		{
		case IDC_SET_JOGXYZ_XN:
		case IDC_JOG_BUTTON_MV10:
			if (m_bEnableTrack)
				g_pFrm->m_Robot->Jog(T1_AXIS, m_dJogSpdRatio,-1, s_bEnableStep, s_fStepDist);
			else
				g_pFrm->m_Robot->Jog(X_AXIS, m_dJogSpdRatio,-1, s_bEnableStep, s_fStepDist);
			break;
		case IDC_SET_JOGXYZ_XP:
		case IDC_JOG_BUTTON_MV11:
			if (m_bEnableTrack)
				g_pFrm->m_Robot->Jog(T1_AXIS, m_dJogSpdRatio,1, s_bEnableStep, s_fStepDist);
			else
				g_pFrm->m_Robot->Jog(X_AXIS, m_dJogSpdRatio,1, s_bEnableStep, s_fStepDist);
			break;
		case IDC_SET_JOGXYZ_YN:
		case IDC_JOG_BUTTON_MV12:
			if (m_bEnableTrack)
				g_pFrm->m_Robot->Jog(W1_AXIS, m_dJogSpdRatio,-1, s_bEnableStep, s_fStepDist);
			else
				g_pFrm->m_Robot->Jog(Y_AXIS, m_dJogSpdRatio,-1, s_bEnableStep, s_fStepDist);
			break;
		case IDC_SET_JOGXYZ_YP:
		case IDC_JOG_BUTTON_MV13:
			if (m_bEnableTrack)
				g_pFrm->m_Robot->Jog(W1_AXIS, m_dJogSpdRatio,1, s_bEnableStep, s_fStepDist);
			else
				g_pFrm->m_Robot->Jog(Y_AXIS, m_dJogSpdRatio,1, s_bEnableStep, s_fStepDist);
			break;
		case IDC_SET_JOGXYZ_ZN:
		case IDC_JOG_BUTTON_MV14:
			if (m_bEnableTrack)
				g_pFrm->m_Robot->Jog(R_AXIS, m_dJogSpdRatio, -1, s_bEnableStep, s_fStepDist);
			else
				g_pFrm->m_Robot->Jog(Z_AXIS, m_dJogSpdRatio, -1, s_bEnableStep, s_fStepDist);
			break;
		case IDC_SET_JOGXYZ_ZP:
		case IDC_JOG_BUTTON_MV15:
			if (m_bEnableTrack)
				g_pFrm->m_Robot->Jog(R_AXIS, m_dJogSpdRatio, 1, s_bEnableStep, s_fStepDist);
			else
				g_pFrm->m_Robot->Jog(Z_AXIS, m_dJogSpdRatio, 1, s_bEnableStep, s_fStepDist);
			break;
// 		case IDC_JOG_BUTTON_MV16:
// 			g_pFrm->m_Robot->Jog(R_AXIS, m_dJogSpdRatio, -1, s_bEnableStep, s_fStepDist);
// 			break;
// 		case IDC_JOG_BUTTON_MV17:
// 			g_pFrm->m_Robot->Jog(R_AXIS, m_dJogSpdRatio, 1, s_bEnableStep, s_fStepDist);
// 			break;
		case IDC_SET_JOGXYZ_VN:
			g_pFrm->m_Robot->Jog(T1_AXIS, m_dJogSpdRatio, -1, s_bEnableStep, s_fStepDist);
			break;
		case IDC_SET_JOGXYZ_VP:
			g_pFrm->m_Robot->Jog(T1_AXIS, m_dJogSpdRatio, 1, s_bEnableStep, s_fStepDist);
			break;
		case IDC_SET_JOGXYZ_WN:
			g_pFrm->m_Robot->Jog(W1_AXIS, m_dJogSpdRatio, -1, s_bEnableStep, s_fStepDist);
			break;
		case IDC_SET_JOGXYZ_WP:
			g_pFrm->m_Robot->Jog(W1_AXIS, m_dJogSpdRatio, 1, s_bEnableStep, s_fStepDist);
			break;
		case IDC_SET_JOGXYZ_MN:
			g_pFrm->m_Robot->Jog(LOAD_AXIS, m_dJogSpdRatio, -1, s_bEnableStep, s_fStepDist);
			break;
		case IDC_SET_JOGXYZ_MP:
			g_pFrm->m_Robot->Jog(LOAD_AXIS, m_dJogSpdRatio, 1, s_bEnableStep, s_fStepDist);
			break;
		case IDC_SET_JOGXYZ_NN:
			g_pFrm->m_Robot->Jog(UNLOAD_AXIS, m_dJogSpdRatio, -1, s_bEnableStep, s_fStepDist);
			break;
		case IDC_SET_JOGXYZ_NP:
			g_pFrm->m_Robot->Jog(UNLOAD_AXIS, m_dJogSpdRatio, 1, s_bEnableStep, s_fStepDist);
			break;
			
		default:
			NULL;
		}
	}
}
