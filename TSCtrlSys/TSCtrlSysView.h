
// TSCtrlSysView.h : CTSCtrlSysView ��Ľӿ�
//
#pragma once

#include "ImgStatic.h"
#include "Camera.h"
#include "CameraHK.h"
#include "CameraDS.h"

class CTSCtrlSysView : public CView
{
protected: // �������л�����
	CTSCtrlSysView();
	DECLARE_DYNCREATE(CTSCtrlSysView)

// ����
public:
	CTSCtrlSysDoc* GetDocument() const;
//	CImgStatic m_ImgStatic;

// ����
public:
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);


// ʵ��
public:
	virtual ~CTSCtrlSysView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int m_nCursorIndex;
// ���ɵ���Ϣӳ�亯��
	CRect m_Rect;
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	//////////////////////////////////////////////////////////////////////////
	/////////////////// �̶ȺͿ����ʾ ///////////	
	CRect m_tSelRect, m_tImgRect;
	double m_dScale;
	void DrawCross(CDC *pDC, CPoint point, int rw, int rh);
	void ClearOverlay();
	void PaintShape(/*CDC *pDC, */CRect rect);
	void DrawEllipse(/*CDC *pDC, */CPoint point, int r, BOOL bFull);
	CPoint GetPoint(tgImgPoint tPoint);

	CCamera*	m_pCamera;
	CCameraHK	m_camHK;
	CCameraDS	m_camDS;

	//////////////////////////////////////////////////////////////////////////
	MIL_ID MilApplication,      /* Application identifier. */
		MilSystem,              /* System identifier.      */
		MilDisplay,             /* Display identifier.     */
		MilOverlayImage,		/* OverlayImage identifier.*/
		MilDigitizer,           /* Digitizer identifier.   */
		MilImageShow,			/* Image be shown identifier. */
		MilImage;               /* Image identifier.       */

	bool m_bMilInit;	// Mil��ʼ�����
	BOOL m_bImgInit;	// �����ʼ�����
	BYTE* m_pImageBuffer;	// ������ͷ��ȡ��ͼ������ָ��
	LRESULT OnMessageRedrawCCDEvent(WPARAM wparam, LPARAM lparam);
	void InitMilSys();
	void FreeMilSys();

	void ImgLive();
	void ImgStop();
	void LoadImage(CString strPath);

	//////////////////////////////////////////////////////////////////////////
	BOOL PatAlloc(const int modIdx=0);
	BOOL PatLoad(const int modIdx=0);
	void PatSave(const int modIdx=0);
	int PatFind(const int modIdx, double dAcceptance,double &dx, double &dy);
	void PatSetParam(const int modIdx, double dAcceptance);
	double PatGetParam(const int modIdx);
	void PatShow(HWND hWnd, const int modIdx=0);
	void PatCenter(double &dx, double &dy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // TSCtrlSysView.cpp �еĵ��԰汾
inline CTSCtrlSysDoc* CTSCtrlSysView::GetDocument() const
   { return reinterpret_cast<CTSCtrlSysDoc*>(m_pDocument); }
#endif

