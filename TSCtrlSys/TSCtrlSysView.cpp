
// TSCtrlSysView.cpp : CTSCtrlSysView 类的实现
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "TSCtrlSysDoc.h"
#include "TSCtrlSysView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTSCtrlSysView

IMPLEMENT_DYNCREATE(CTSCtrlSysView, CView)

BEGIN_MESSAGE_MAP(CTSCtrlSysView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTSCtrlSysView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()

	ON_MESSAGE(WM_REDRAWIMAGEFROMCCD, &CTSCtrlSysView::OnMessageRedrawCCDEvent)
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CTSCtrlSysView 构造/析构

CTSCtrlSysView::CTSCtrlSysView()
{
	// TODO: 在此处添加构造代码
	m_Rect = CRect(0,0,20,20);
	m_tSelRect = CRect(IMG_WIDTH/2-100, IMG_HEIGHT/2-100, IMG_WIDTH/2+100, IMG_HEIGHT/2+100);
	m_dScale = 1;
	m_pImageBuffer = NULL;

	m_bMilInit = false;
	m_bImgInit = FALSE;

	m_pCamera = NULL;
	m_nCursorIndex = 0;
}

CTSCtrlSysView::~CTSCtrlSysView()
{
}

BOOL CTSCtrlSysView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTSCtrlSysView 绘制

void CTSCtrlSysView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
//	CTSCtrlSysDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}
// CTSCtrlSysView 打印
void CTSCtrlSysView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CTSCtrlSysView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTSCtrlSysView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTSCtrlSysView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CTSCtrlSysView::OnRButtonUp(UINT nFlags, CPoint point)
{
// 	UNREFERENCED_PARAMETER(nFlags);
// 	ClientToScreen(&point);
// 	OnContextMenu(this, point);

	if (K_RUN_STS_RUN == g_pFrm->m_Robot->m_tSysStatus)		return;	// 自动运行中
	if (m_dScale < 0.1)										return;	// 缩放比例太大

	if(g_pFrm->m_Robot->m_pController != NULL)
	{
		g_pFrm->m_Robot->m_pController->AxisStop(0xFF);
	}

	CView::OnRButtonUp(nFlags, point);
}

void CTSCtrlSysView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	UNREFERENCED_PARAMETER(pWnd);
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CTSCtrlSysView 诊断

#ifdef _DEBUG
void CTSCtrlSysView::AssertValid() const
{
	CView::AssertValid();
}

void CTSCtrlSysView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTSCtrlSysDoc* CTSCtrlSysView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTSCtrlSysDoc)));
	return (CTSCtrlSysDoc*)m_pDocument;
}
#endif //_DEBUG


// CTSCtrlSysView 消息处理程序

void CTSCtrlSysView::OnInitialUpdate()
{
	static BOOL b = TRUE;
	if(!b)return;
	b=FALSE;	

	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	g_pView = this;

	if (!m_bMilInit)
	{
		InitMilSys();
	}

	if (0 == g_pFrm->m_pRobotParam->m_nCameraType)
	{
		m_pCamera = &m_camDS;
	}
	else if (1 == g_pFrm->m_pRobotParam->m_nCameraType)
	{
		m_pCamera = &m_camHK;
	}
	else
	{
		CString str;
		str.Format("未指定相机型号:[%d]!!!", g_pFrm->m_pRobotParam->m_nCameraType);
		AfxMessageBox(str);
	}

	m_bImgInit = m_pCamera->OpenCamera("相机1", GetSafeHwnd(), m_pImageBuffer);

// 	GetClientRect(&m_Rect);
// 	m_ImgStatic.Create("IMAGE", WS_CHILD|WS_VISIBLE|SS_NOTIFY, /*m_Rect*/CRect(0,0,20,20), this);
// 	if(!m_ImgStatic.ImgInit(m_hWnd))
// 	{
// 		NULL;
// 		//AfxMessageBox("图像初始化错误.");
// 		//g_pFrm->PostMessage(WM_CLOSE, 0,0);
// 	}
// 	else
// 	{
// //		m_ImgStatic.ImgLive();
// 	}
	if (NULL == g_pFrm->m_wndEditBar.GetSafeHwnd())
	{
		g_pFrm->m_wndEditBar.Create(IDD_DLG_EDIT, g_pFrm);
		g_pFrm->m_wndEditBar.m_pMsgList = &g_pFrm->m_wndOutput.m_wndOutputBuild;
		g_pFrm->m_wndEditBar.EnableWindow(TRUE);
	}

	SetTimer(2001, 5000, NULL);
	///////////////////////////
}

void CTSCtrlSysView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect clientRect;
	GetClientRect(clientRect);
	double dScalex = (double)clientRect.Width()/(double)IMG_WIDTH;
	double dScaley = (double)clientRect.Height()/(double)IMG_HEIGHT;
	m_dScale = min(dScalex, dScaley);

	m_tImgRect.left = 0;
	m_tImgRect.top = 0;
	m_tImgRect.right = static_cast<long>(m_dScale*IMG_WIDTH);
	m_tImgRect.bottom = static_cast<long>(m_dScale*IMG_HEIGHT);

	if (m_bMilInit)
	{
		MdispZoom(MilDisplay, m_dScale, m_dScale);
	}

	// TODO: Add your message handler code here
// 	if(m_ImgStatic.GetSafeHwnd())
// 	{
// 		m_ImgStatic.MoveWindow(0,0,cx, cy);
// 	}
}

void CTSCtrlSysView::OnDestroy()
{
	FreeMilSys();
	CView::OnDestroy();
	// TODO: Add your message handler code here
}

void CTSCtrlSysView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (K_RUN_STS_RUN == g_pFrm->m_Robot->m_tSysStatus)		return;	// 自动运行中
	if (m_dScale < 0.1)										return;	// 缩放比例太大

	double ptX = point.x / m_dScale;
	double ptY = point.y / m_dScale;
	int iDirX=0, iDirY=0;
	ptX = ptX - IMG_WIDTH/2;
	iDirX = ptX>0 ? 1:0;
	ptX /= (IMG_WIDTH/2);
	ptX = abs(ptX);

	ptY = ptY - IMG_HEIGHT/2;
	iDirY = ptY>0 ? 1:0;
	ptY /= (IMG_HEIGHT/2);
	ptY = abs(ptY);

	if (ptX > ptY)
	{
		if (iDirX)
		{
			if (ptX < 0.33)			m_nCursorIndex = 0;
			else if (ptX < 0.66)	m_nCursorIndex = 1;
			else					m_nCursorIndex = 2;
		}
		else
		{
			if (ptX < 0.33)			m_nCursorIndex = 3;
			else if (ptX < 0.66)	m_nCursorIndex = 4;
			else					m_nCursorIndex = 5;
		}
	}
	else
	{
		if (iDirY)
		{
			if (ptY < 0.33)			m_nCursorIndex = 6;
			else if (ptY < 0.66)	m_nCursorIndex = 7;
			else					m_nCursorIndex = 8;
		}
		else
		{
			if (ptY < 0.33)			m_nCursorIndex = 9;
			else if (ptY < 0.66)	m_nCursorIndex = 10;
			else					m_nCursorIndex = 11;
		}
	}

	CView::OnMouseMove(nFlags, point);
}


void CTSCtrlSysView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (K_RUN_STS_RUN == g_pFrm->m_Robot->m_tSysStatus)		return;	// 自动运行中
	if (m_dScale < 0.1)										return;	// 缩放比例太大

	if((g_pFrm->m_dlgMark.GetSafeHwnd() && g_pFrm->m_dlgMark.IsWindowVisible())
		||(g_pFrm->m_dlgPixel.GetSafeHwnd() && g_pFrm->m_dlgPixel.IsWindowVisible())
		||(g_pFrm->m_dlgBoardCheckEx.GetSafeHwnd() && g_pFrm->m_dlgBoardCheckEx.IsWindowVisible()))
	{
		CRect rc;
		GetWindowRect(rc);
		m_tSelRect.top = int(point.y / m_dScale);
		m_tSelRect.left = int(point.x / m_dScale);

		if (m_tSelRect.top >= IMG_HEIGHT || m_tSelRect.left >= IMG_WIDTH)
		{
			return;
		}

		if (m_tSelRect.top > (IMG_HEIGHT/2))
		{
			m_tSelRect.bottom = m_tSelRect.top;
			m_tSelRect.top = IMG_HEIGHT-m_tSelRect.top;
		}
		else
		{
			m_tSelRect.bottom = IMG_HEIGHT-m_tSelRect.top;
		}


		if (m_tSelRect.left > (IMG_WIDTH/2))
		{
			m_tSelRect.right = m_tSelRect.left;
			m_tSelRect.left = IMG_WIDTH-m_tSelRect.left;
		}
		else
		{
			m_tSelRect.right = IMG_WIDTH-m_tSelRect.left;
		}

		PaintShape(CRect(0, 0, IMG_WIDTH, IMG_HEIGHT));
	//	pDC->Draw3dRect(m_tSelRect, RGB(200,0,0), RGB(255,0,0));  // 框选颜色
	}
	else
	{
		g_pFrm->m_Robot->GotoSafeZpos();

		int l_iDirX=1, l_iDirY=1;
		double l_dMovDistX = point.x / m_dScale;
		double l_dMovDistY = point.y / m_dScale;
		l_dMovDistX = l_dMovDistX - IMG_WIDTH/2;
		l_iDirX = l_dMovDistX>0 ? 1: -1;
		l_dMovDistX = abs(l_dMovDistX*g_pFrm->m_pSysParam->dPixelScale);

		l_dMovDistY = l_dMovDistY - IMG_HEIGHT/2;
		l_iDirY = l_dMovDistY>0 ? 1: -1;
		l_dMovDistY = abs(l_dMovDistY*g_pFrm->m_pSysParam->dPixelScale);

		g_pFrm->m_Robot->Jog(X_AXIS, l_dMovDistX, l_iDirX, TRUE, l_dMovDistX);
		g_pFrm->m_Robot->Jog(Y_AXIS, l_dMovDistY, l_iDirY, TRUE, l_dMovDistY);
	}

	POINT pt;
	pt.x = int(point.x / m_dScale);
	pt.y = int(point.y / m_dScale);

	CView::OnLButtonDown(nFlags, point);
}


void CTSCtrlSysView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (K_RUN_STS_RUN == g_pFrm->m_Robot->m_tSysStatus)		return;	// 自动运行中
	if (m_dScale < 0.1)										return;	// 缩放比例太大
	
	CView::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CTSCtrlSysView::InitMilSys()
{
	/* Allocate a MIL application. */
	MappAlloc(M_DEFAULT, &MilApplication);

	/* Allocate a MIL system. */
	MsysAlloc("M_DEFAULT", M_DEFAULT, M_DEFAULT, &MilSystem);

	/* Allocate a MIL display. */
	MdispAlloc(MilSystem, M_DEFAULT, "M_DEFAULT", M_WINDOWED, &MilDisplay);

	MbufAlloc2d(MilSystem, IMG_WIDTH, IMG_HEIGHT,M_DEF_IMAGE_TYPE,M_IMAGE+M_DISP+M_PROC,&MilImage);
	MbufClear(MilImage, 192);

	MdispSelectWindow(MilDisplay, MilImage, GetSafeHwnd());
	/* Enable the display of overlay annotations. */
	MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
	/* Inquire the overlay buffer associated with the display. */
	MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
	/* Clear the overlay to transparent. */
	MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

	m_bMilInit = true;
}

void CTSCtrlSysView::FreeMilSys()
{
	for (int i=0; i<Mod_Num; i++)
	{
		g_ProgData.m_mod[i].FreeMem();
	}
	if (M_NULL != MilImage)
	{
		MbufFree(MilImage);
	}
	if (M_NULL != MilDisplay)
	{
//		MdispSelect(MilDisplay, M_NULL);
		MdispFree(MilDisplay);
	}

	if (M_NULL != MilSystem)
		MsysFree(MilSystem);

	if (M_NULL != MilApplication)
		MappFree(MilApplication);
}

void CTSCtrlSysView::ImgLive()
{
	m_pCamera->SetTriggerMode(false);
}

void CTSCtrlSysView::ImgStop()
{
	m_pCamera->SetTriggerMode(true);
}

void CTSCtrlSysView::PatShow(HWND hWnd, const int modIdx)
{
	g_ProgData.m_mod[modIdx].ShowMod(hWnd);
}

BOOL CTSCtrlSysView::PatAlloc(const int modIdx)
{
	g_ProgData.m_mod[modIdx].CreateMod(MilImage, m_tSelRect.left, m_tSelRect.top, m_tSelRect.Width(), m_tSelRect.Height());
	return TRUE;
}

int CTSCtrlSysView::PatFind(const int modIdx, double dAcceptance,double &dx, double &dy)
{
	MbufPut(MilImage, m_pImageBuffer);

	PaintShape(CRect(0,0,IMG_WIDTH, IMG_HEIGHT));
	return g_ProgData.m_mod[modIdx].FindMod(dAcceptance, dx, dy);
}

void CTSCtrlSysView::PatSetParam(const int modIdx, double dAcceptance)
{
	g_ProgData.m_mod[modIdx].m_acceptanceScore = (int)dAcceptance;
}

double CTSCtrlSysView::PatGetParam(const int modIdx)
{
	return (double)g_ProgData.m_mod[modIdx].m_acceptanceScore;
}

BOOL CTSCtrlSysView::PatLoad(const int modIdx)
{
	g_ProgData.m_mod[modIdx].LoadMod();
	return TRUE;
}

void CTSCtrlSysView::PatSave(const int modIdx)
{
	g_ProgData.m_mod[modIdx].SaveMod();
}
void CTSCtrlSysView::PatCenter(double &dx, double &dy)
{
	// 根据Mark点图像位置与图像中心距离  计算 对应 机械坐标距离
	dx = (dx - IMG_WIDTH*0.5) * g_pFrm->m_pSysParam->dPixelScale;
	dy = (dy - IMG_HEIGHT*0.5) * g_pFrm->m_pSysParam->dPixelScale;

	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();

	double x = 0, y = 0;

	//图像坐标与机械臂坐标的方向决定+或-
	// 坐标计算后的当前位置
	if(g_pFrm->m_pSysParam->iXDir==0)
		x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS] + dx;
	else
		x = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[X_AXIS] - dx;

	if(g_pFrm->m_pSysParam->iYDir==0)
		y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS] + dy;
	else
		y = g_pFrm->m_Robot->m_pController->g_ExMtnPara->dfCurpos[Y_AXIS] - dy;

	dx = x;
	dy = y;
}

void CTSCtrlSysView::LoadImage(CString strPath)
{
	if (MilImage > 0)
	{
		MbufFree(MilImage);
		MilImage = 0;
	}
	MbufRestore(strPath.GetBuffer(strPath.GetLength()), MilSystem, &MilImage);
	MdispSelectWindow(MilDisplay, MilImage, m_hWnd);

	PaintShape(/*pDC, */CRect(0, 0, IMG_WIDTH, IMG_HEIGHT));
}

CPoint CTSCtrlSysView::GetPoint(tgImgPoint tPoint)
{
	CPoint dot;
	dot.x =static_cast<int>(tPoint.x*m_dScale);
	dot.y = static_cast<int>(tPoint.y*m_dScale);
	return dot;
}

void CTSCtrlSysView::DrawCross(CDC *pDC, CPoint point, int rw, int rh)
{
	pDC->MoveTo(point.x, point.y-rh);
	pDC->LineTo(point.x, point.y+rh);
	pDC->MoveTo(point.x-rw, point.y);
	pDC->LineTo(point.x+rw, point.y);
}

void CTSCtrlSysView::DrawEllipse(/*CDC *pDC, */CPoint point, int r, BOOL bFull)
{
// 	if(!bFull)
// 	{
// 		CGdiObject *pOldGdi = pDC->SelectStockObject(NULL_BRUSH);
// 		pDC->Ellipse(int(point.x-r*m_dScale), int(point.y-r*m_dScale), int(point.x+r*m_dScale), int(point.y+r*m_dScale));
// 		pDC->SelectObject(pOldGdi);
// 	}
// 	else
// 	{
// 		pDC->Ellipse(point.x-r, point.y-r, point.x+r, point.y+r);
// 	}
	if (bFull)
	{
		MgraArcFill(M_DEFAULT, MilOverlayImage, point.x, point.y, r, r, 0, 360);
	}
	else
	{
		MgraArc(M_DEFAULT, MilOverlayImage, point.x, point.y, r, r, 0, 360);
	}

}

void CTSCtrlSysView::ClearOverlay()
{
	/* Clear the overlay image. */
	MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
}

/////////////////// 刻度和框架显示 ///////////
void CTSCtrlSysView::PaintShape(/*CDC *pDC, */CRect rect)
{
	if(g_pFrm == NULL)
		return;

	if (!m_bMilInit)
		return;

	/* Clear the overlay image. */
	MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

	CString strTemp;
	CPoint point;
	double dmm;
	CPoint t;

	// 获取相机类型
	int CCDType = g_pFrm->m_pSysParam->nCCDType ;
	if(CCDType == DefCCDType_Color)
 	{
// 		// 彩色相机 
// 		CPen pen(PS_DOT, 1, RGB(255,255,255));                            // 背景十字刻度颜色 
// 		CPen *pOldPen = pDC->SelectObject(&pen);
// 
// 		if((g_pFrm->m_dlgMark.GetSafeHwnd() && g_pFrm->m_dlgMark.IsWindowVisible())
// 			||(g_pFrm->m_dlgPixel.GetSafeHwnd() && g_pFrm->m_dlgPixel.IsWindowVisible())
// 			||(g_pFrm->m_dlgBoardCheckEx.GetSafeHwnd() && g_pFrm->m_dlgBoardCheckEx.IsWindowVisible()))
// 		{
// 			pDC->Draw3dRect(m_tSelRect, RGB(255,240,255), RGB(255,255,255));  // 框选颜色
// 		}
// 
// 		pDC->Draw3dRect(rect, RGB(0,240,0), RGB(0,240,0));                     // 外框颜色 
// 		DrawCross(pDC, rect.CenterPoint(), rect.Width()/2, rect.Height()/2);
// 
// 		strTemp.Format("%d,%d", m_ImgPoint.x, m_ImgPoint.y);
// 
// 		pDC->TextOut(static_cast<int>(m_tCurPoint.x), static_cast<int>(m_tCurPoint.y+10), strTemp);
// 		pDC->SelectObject(pOldPen);
// 		pen.DeleteObject();
// 		pen.CreatePen(PS_SOLID, 2, RGB(0,255,250));                         //  
// 		pOldPen = pDC->SelectObject(&pen);
// 
// 		point = GetPoint(m_tPatPos);
// 		DrawCross(pDC, point, 15,15);
// 
// 		strTemp.Format("%.2f(%d,%d)", m_dPatAcceptance, point.x, point.y);
// 		pDC->SetTextColor(RGB(255,0,0));                               // ?
// 		pDC->TextOut(point.x+10, point.y-20, strTemp);
// 		pDC->SetTextColor(RGB(255,0,0));                                  
// 		//////////////////////////////////////////////////////////////////////////
// 		pDC->SelectObject(pOldPen);
// 		pen.DeleteObject();
// 		pen.CreatePen(PS_SOLID, 1, RGB(255,255,255));                    // 十字大刻度和圆形颜色
// 		pOldPen = pDC->SelectObject(&pen);
// 
// 		//画0.5mm刻度
// 		dmm = 0.5/g_pFrm->m_pSysParam->dPixelScale * m_dScale;	
// 
// 		int i=0;
// 		int x = rect.CenterPoint().x;
// 		int y = rect.CenterPoint().y;
// 		t.x = x; t.y = y;
// 		DrawEllipse(pDC, t, g_pFrm->m_pSysParam->dDrawCircleR1, FALSE);//在中心画圆
// 		DrawEllipse(pDC, t, g_pFrm->m_pSysParam->dDrawCircleR2, FALSE);//在中心画圆
// 		while(1)
// 		{
// 			t.x = static_cast<int>(x - dmm*i);
// 			t.y = static_cast<int>(y - dmm*i);
// 
// 			if(t.x <=0 && t.y<=0)break;
// 			pDC->MoveTo(t.x, y-8);
// 			pDC->LineTo(t.x, y+8);
// 			pDC->MoveTo(x-8, t.y);
// 			pDC->LineTo(x+8, t.y);
// 
// 			t.x = static_cast<int>(x + dmm*i);
// 			t.y = static_cast<int>(y + dmm*i);
// 
// 			pDC->MoveTo(t.x, y-8);
// 			pDC->LineTo(t.x, y+8);
// 			pDC->MoveTo(x-8, t.y);
// 			pDC->LineTo(x+8, t.y);
// 
// 			i++;
// 		}
// 		pDC->SelectObject(pOldPen);
	}
	else
	{
		// 黑白相机
// 		CPen pen(PS_DOT, 1, RGB(255,0,0));                            // 背景十字刻度颜色 
// 		CPen *pOldPen = pDC->SelectObject(&pen);
		MgraColor(M_DEFAULT, M_RGB888(255, 0, 0));

		if((g_pFrm->m_dlgMark.GetSafeHwnd() && g_pFrm->m_dlgMark.IsWindowVisible())
			||(g_pFrm->m_dlgPixel.GetSafeHwnd() && g_pFrm->m_dlgPixel.IsWindowVisible())
			||(g_pFrm->m_dlgBoardCheckEx.GetSafeHwnd() && g_pFrm->m_dlgBoardCheckEx.IsWindowVisible()))
		{
			MgraRect(M_DEFAULT, MilOverlayImage, m_tSelRect.left, m_tSelRect.top, 
				m_tSelRect.right, m_tSelRect.bottom);
		}
		
		MgraRect(M_DEFAULT, MilOverlayImage, rect.left, rect.top, rect.right, rect.bottom);
		MgraLine(M_DEFAULT, MilOverlayImage, 0, rect.CenterPoint().y, rect.right, rect.CenterPoint().y);
		MgraLine(M_DEFAULT, MilOverlayImage, rect.CenterPoint().x, 0, rect.CenterPoint().x, rect.bottom);

		MgraColor(M_DEFAULT, M_RGB888(255,0,0));

		//画0.5mm刻度
		dmm = 0.5/g_pFrm->m_pSysParam->dPixelScale/* * m_dScale*/;	

		int i=0;
		int x = rect.CenterPoint().x;
		int y = rect.CenterPoint().y;
		t.x = x; t.y = y; 
		int l_DrawRadius = Round(g_pFrm->m_pSysParam->dDrawCircleR1/g_pFrm->m_pSysParam->dPixelScale/2.0);
		DrawEllipse(/*pDC, */t, l_DrawRadius, FALSE);//在中心画圆
		MgraRect(M_DEFAULT, MilOverlayImage, x-l_DrawRadius, y-l_DrawRadius, x+l_DrawRadius, y+l_DrawRadius);

		while(1)
		{
			t.x = static_cast<int>(x - dmm*i);
			t.y = static_cast<int>(y - dmm*i);

			if(t.x <=0 && t.y<=0)	break;

			MgraLine(M_DEFAULT, MilOverlayImage, t.x, y-8, t.x, y+8);
			MgraLine(M_DEFAULT, MilOverlayImage, x-8, t.y, x+8, t.y);

			t.x = static_cast<int>(x + dmm*i);
			t.y = static_cast<int>(y + dmm*i);

			MgraLine(M_DEFAULT, MilOverlayImage, t.x, y-8, t.x, y+8);
			MgraLine(M_DEFAULT, MilOverlayImage, x-8, t.y, x+8, t.y);

			i++;
		}
	}
}

LRESULT CTSCtrlSysView::OnMessageRedrawCCDEvent(WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(wparam);
	UNREFERENCED_PARAMETER(lparam);

	if (NULL == m_pImageBuffer)
	{
		return 0;
	}

	if (M_NULL == MilImage)
	{
		return 0;
	}

	MbufPut(MilImage, m_pImageBuffer);
	Invalidate();

	return 0;
}



void CTSCtrlSysView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (K_RUN_STS_RUN == g_pFrm->m_Robot->m_tSysStatus)		return;	// 自动运行中
	if (m_dScale < 0.1)										return;	// 缩放比例太大

	g_pFrm->m_Robot->GotoSafeZpos(TRUE);

	int l_iDirX=1, l_iDirY=1;
	double l_dMovSpdX = point.x / m_dScale;
	double l_dMovSpdY = point.y / m_dScale;
	l_dMovSpdX = l_dMovSpdX - IMG_WIDTH/2;
	l_iDirX = l_dMovSpdX>0 ? 1: -1;
	l_dMovSpdX /= (IMG_WIDTH/2);
	l_dMovSpdX = abs(l_dMovSpdX);

	l_dMovSpdY = l_dMovSpdY - IMG_HEIGHT/2;
	l_iDirY = l_dMovSpdY>0 ? 1: -1;
	l_dMovSpdY /= (IMG_HEIGHT/2);
	l_dMovSpdY = abs(l_dMovSpdY);


	// 判断右键在Z轴提升时是否已经松开
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		if (abs(l_dMovSpdX) > abs(l_dMovSpdY))
		{
			if (l_dMovSpdX < 0.33)		l_dMovSpdX = 0.001;
			else if (l_dMovSpdX < 0.66) l_dMovSpdX = 0.1;
			else l_dMovSpdX = min(1, l_dMovSpdX);
			g_pFrm->m_Robot->Jog(X_AXIS, l_dMovSpdX, l_iDirX, FALSE, 1);
		}
		else
		{
			if (l_dMovSpdY < 0.33)		l_dMovSpdY = 0.001;
			else if (l_dMovSpdY < 0.66) l_dMovSpdY = 0.1;
			else l_dMovSpdY = min(1, l_dMovSpdY);
			g_pFrm->m_Robot->Jog(Y_AXIS, l_dMovSpdY, l_iDirY, FALSE, 1);
		}
	}
	
	CView::OnRButtonDown(nFlags, point);
}


void CTSCtrlSysView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnLButtonDblClk(nFlags, point);
}

static int g_nCursor[] = {
	IDC_CURSOR_RS, IDC_CURSOR_RM, IDC_CURSOR_RF,
	IDC_CURSOR_LS, IDC_CURSOR_LM, IDC_CURSOR_LF,
	IDC_CURSOR_DS, IDC_CURSOR_DM, IDC_CURSOR_DF,
	IDC_CURSOR_US, IDC_CURSOR_UM, IDC_CURSOR_UF,};

BOOL CTSCtrlSysView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if((g_pFrm->m_dlgMark.GetSafeHwnd() && g_pFrm->m_dlgMark.IsWindowVisible())
		||(g_pFrm->m_dlgPixel.GetSafeHwnd() && g_pFrm->m_dlgPixel.IsWindowVisible())
		||(g_pFrm->m_dlgBoardCheckEx.GetSafeHwnd() && g_pFrm->m_dlgBoardCheckEx.IsWindowVisible()))
	{
		return CView::OnSetCursor(pWnd, nHitTest, message);
	}
	else
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(g_nCursor[m_nCursorIndex])));
		return TRUE;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CTSCtrlSysView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (2001 == nIDEvent)
	{
		PaintShape(CRect(0,0,IMG_WIDTH, IMG_HEIGHT));
	}

	CView::OnTimer(nIDEvent);
}
