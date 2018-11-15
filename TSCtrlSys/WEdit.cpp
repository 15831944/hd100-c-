// WEdit.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "WEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWEdit* pActiveEdit = NULL;
/////////////////////////////////////////////////////////////////////////////
// CWEdit

CWEdit::CWEdit() : CEdit()
{
	mSetLmt = 0;

	mReadOnly = 0;
	mNumOnly  = 1;
}

CWEdit::~CWEdit()
{
    pActiveEdit = NULL;
}

int CWEdit::Create(CWnd* pwnd, int x, int y, int w, int h)
{
	int EditStyle = ES_CENTER | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;

	CEdit::DestroyWindow();	
	CEdit::Create(EditStyle, CRect(x, y, x + w, y + h), pwnd, 1000);
	
 	m_font1.CreateFont(22,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,
		CLIP_STROKE_PRECIS,DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("UniDreamLED"));
// 	m_font2.CreateFont(22,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,
// 		CLIP_STROKE_PRECIS,DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("Arial"));

	SetWindowText("0");
	SetFont(&m_font1);
	return 1;
}

CString CWEdit::GetString()
{	
	CString str;
	GetWindowText(str);

	return str;
}

int CWEdit::SetString(CString str)
{
	SetWindowText(str);
	return 1;
}

int CWEdit::GetData()
{
	CString str;
	GetWindowText(str);

	int data = atoi(str);
	return data;
}

int CWEdit::SetData(int data)
{
	if (mSetLmt == 1)
	{
		data = data < mMax ? data : mMax;
		data = data > mMin ? data : mMin;
	}

	CString str;
	str.Format("%d", data);	
	
	SetWindowText(str);
	return data;
}

int CWEdit::LmtData(int min, int max)
{	
	int Min = max > min ? min : max;
	int Max = max > min ? max : min;
	
	mMin = Min;
	mMax = Max;
	mSetLmt = 1;

	return 1;
}

void CWEdit::Show()
{
	UpdateData();
	Invalidate();
	ShowWindow(SW_SHOW);
}

void CWEdit::Hide()
{
	ShowWindow(SW_HIDE);
}

BOOL CWEdit::SetReadOnly(BOOL flag)
{
	mReadOnly = flag;
	CEdit::SetReadOnly(flag);

	return flag;
}

BOOL CWEdit::SetNumOnly(BOOL flag)
{
	mNumOnly = flag;
	return flag;
}

void CWEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(nRepCnt);

	if (mNumOnly)
	{
		if( (nChar >= '0' && nChar <= '9') || nChar == VK_BACK || nChar == '-' || nChar == '.')
		{
			CEdit::OnChar(nChar, 0, 0);
		}
	}
	else 
	{
		CEdit::OnChar(nChar, 0, 0);
	}
}

LRESULT CWEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CHAR)
	{
		OnChar(wParam, 0, 0);
		return 1;
	}

	return CEdit::WindowProc(message, wParam, lParam);

	CPoint Pos;

	switch (message)
	{
	case WM_CHAR:
		OnChar(wParam, 0, 0);
		break;

// 	case WM_LBUTTONDOWN:
// 		Pos.x = LOWORD(lParam);  
// 		Pos.y = HIWORD(lParam); 
// 		
// 		OnLButtonDown(wParam, Pos);
// 		break;

	default:
		CEdit::WindowProc(message, wParam, lParam);
		break;
	}	

	return message;
}

int CWEdit::SetActive()
{
	if (pActiveEdit != NULL) // 前次active
	{
		pActiveEdit->SetDeactive(); //取消激活
	}
	
	mText  = RGB(255, 0, 0 );
	mBkgnd = RGB(0, 192, 192);
	
	m_brBkgnd.DeleteObject();
	m_brBkgnd.CreateSolidBrush( mBkgnd );
	
	pActiveEdit = this;	
	
	return 1;
}

int CWEdit::SetDeactive() //黑字，白底
{	
	mText  = RGB(0, 0, 0 );
	mBkgnd = RGB(255, 255, 255);
	
	m_brBkgnd.DeleteObject();
	m_brBkgnd.CreateSolidBrush( mBkgnd );
	
	Invalidate();
	
	return 1;
}

int CWEdit::GetActive()
{	
	if (pActiveEdit == this) { return 1; }
	else                     { return 0; }
}

BEGIN_MESSAGE_MAP(CWEdit, CEdit)
//{{AFX_MSG_MAP(testedit)
ON_WM_LBUTTONDOWN()
ON_WM_CTLCOLOR_REFLECT()
ON_WM_KILLFOCUS()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CWEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    
	if (mReadOnly)
	{ 
		return;
	}
	
	SetActive();
	Invalidate();
	CEdit::OnLButtonDown(nFlags, point);
}


HBRUSH CWEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(nCtlColor);

	// TODO: Change any attributes of the DC here
	if (mReadOnly)
	{
		mText  = RGB(255, 255, 255);
		mBkgnd = RGB(64, 64, 64);
		
		m_brBkgnd.DeleteObject();
		m_brBkgnd.CreateSolidBrush(mBkgnd);		
	}
	
	pDC->SetTextColor(mText);
	pDC->SetBkColor(mBkgnd);
	return m_brBkgnd;
}
