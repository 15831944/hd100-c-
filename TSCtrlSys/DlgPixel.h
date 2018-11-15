#pragma once

#include "paramdef.h"

// CDlgPixel dialog

class CDlgPixel : public CDialog
{
	DECLARE_DYNAMIC(CDlgPixel)

public:
	CDlgPixel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPixel();

// Dialog Data
	enum { IDD = IDD_DLG_PIXEL };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedCancel();

public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedDlgPixelBtnLearn();
	afx_msg void OnBnClickedDlgPixelBtnMove();
	afx_msg void OnBnClickedDlgPixelBtnPatTest();
	afx_msg void OnBnClickedDlgPixelBtnPatSave();
	afx_msg void OnBnClickedDlgPixelBtnCali();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedDlgPixelBtnPatEdit();
	bool m_bIsEditMod;
	bool m_bLBtnDown;
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};