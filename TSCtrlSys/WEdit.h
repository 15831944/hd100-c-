#if !defined(AFX_WEDIT_H__7F38788F_B806_4C6F_AF18_D6382CBCAFDB__INCLUDED_)
#define AFX_WEDIT_H__7F38788F_B806_4C6F_AF18_D6382CBCAFDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWEdit window

class CWEdit : public CEdit
{
public:
	CWEdit();
	~CWEdit();
	
	int Create(CWnd* pwnd, int x, int y, int w=60, int h=15);
	
	CString GetString();
	int     SetString(CString str);
	
	int GetData();
	int SetData(int data);
	int LmtData(int min, int max);
	
	void Show();
	void Hide();
	
	BOOL SetReadOnly(BOOL flag);
	BOOL SetNumOnly (BOOL flag);
	
	int GetActive();
	int SetActive();
	int SetDeactive();
	
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	/*void OnLButtonDown(UINT nFlags, CPoint point);*/
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	//{{AFX_MSG(myedit)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
		
private:
	COLORREF mText;
	COLORREF mBkgnd;
	
	CFont m_font1, 
		m_font2;
	CBrush m_brBkgnd;
	
	int mReadOnly;
	int mNumOnly;
	
	int mMax;
	int mMin;
	int mSetLmt;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEDIT_H__7F38788F_B806_4C6F_AF18_D6382CBCAFDB__INCLUDED_)
