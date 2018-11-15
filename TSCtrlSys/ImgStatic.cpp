// ImgStatic.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "ImgStatic.h"
#pragma comment(lib, "vfw32.lib")
#define _USE_MATH_DEFINES
#include <math.h>
#include "KFTimer.h"

#pragma warning(disable: 4100 4244)
//#include "MemDC.h"
// CImgStatic

IMPLEMENT_DYNAMIC(CImgStatic, CStatic)

#define DrawColor RGB(255,255,255);

CImgStatic::CImgStatic()
{
	m_tSelRect = CRect(0,0,1,1);
	m_bMouseDown = FALSE;
	m_hDrawDib = DrawDibOpen();
	m_iImgWidth = IMG_WIDTH;
	m_iImgHeight = IMG_HEIGHT;
	m_bImgInit = FALSE;
	m_PatBuffer = NULL;
	m_PatDisplay = NULL;
	m_BufRGB = NULL;
	m_dCmdScale = 1.0;
	m_dPatAcceptance = 0;
	m_tTrackDrawOffset.x=0;
	m_tTrackDrawOffset.y =0;
	m_lXlen = 200.0;
	m_tMouseEvent = MK_MOUSE_NONE;
	m_nDrawTarkCount=100;
	m_Draw_CurP.x=0;
	m_Draw_CurP.y=0;

	m_pImageBuffer = NULL;
	m_pBufShow = NULL;
	m_Track.SetOffset(m_tTrackDrawOffset.x,m_tTrackDrawOffset.y);
}

CImgStatic::~CImgStatic()
{
	if(m_hDrawDib != NULL)   
	{
		DrawDibClose(m_hDrawDib);
		m_hDrawDib = NULL;
	}
	if (m_pBmpInfo!=NULL)
	{
		delete m_pBmpInfo;
	}
	FreeDisplay(m_PatDisplay, m_PatBuffer);
	ImgFree();
	TRACE(_T("\n~CImgStatic()"));
}


BEGIN_MESSAGE_MAP(CImgStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()

	ON_MESSAGE(WM_REDRAWIMAGEFROMCCD, &CImgStatic::OnMessageRedrawCCDEvent)
END_MESSAGE_MAP()
//////////////////////////////////////////////////
void CImgStatic::CallBack(STRUCT_KSVDH KSVDH)
{
	UNREFERENCED_PARAMETER(KSVDH);
	if(GetSafeHwnd()) Invalidate();
}

void CImgStatic::OnPaint(HDC hdc)
{
	UNREFERENCED_PARAMETER(hdc);
}
// 
BOOL CImgStatic::ImgInit(HWND hWnd, long nUpFocus, long nGain)
{
	m_OutSizeX=IMG_WIDTH; m_OutSizeY=IMG_HEIGHT;
	if (NULL != m_pBufShow)
	{
		delete []m_pBufShow;
		m_pBufShow = NULL;
	}
	m_pBufShow = new BYTE[IMG_WIDTH*IMG_HEIGHT];

//	Init(GetSafeHwnd(), m_OutSizeX, m_OutSizeY, 1, false);
	m_bImgInit = TRUE;
	m_pBmpInfo = (BITMAPINFO*)(new char[sizeof(BITMAPINFOHEADER) + 256*4]);

	if(g_pFrm->m_pSysParam->nCCDType == DefCCDType_Color)
	   m_pBmpInfo->bmiHeader.biBitCount=(UINT16)(24);
	else
       m_pBmpInfo->bmiHeader.biBitCount=(UINT16)(8);

	m_pBmpInfo->bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth  = m_OutSizeX;
	m_pBmpInfo->bmiHeader.biHeight = m_OutSizeY;
	m_pBmpInfo->bmiHeader.biPlanes = 1;

	m_pBmpInfo->bmiHeader.biCompression  = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage    = m_OutSizeX * m_OutSizeY * 3;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter= 0;
	m_pBmpInfo->bmiHeader.biClrUsed      = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;	

	for(int i=0;i<256;i++)
	{
		m_pBmpInfo->bmiColors[i].rgbRed=(UINT8)i;
		m_pBmpInfo->bmiColors[i].rgbGreen=(UINT8)i;
		m_pBmpInfo->bmiColors[i].rgbBlue=(UINT8)i;
		m_pBmpInfo->bmiColors[i].rgbReserved=0;
	}

	m_bImgInit = m_camHK.OpenCamera("���1", GetSafeHwnd(), m_pImageBuffer);
//	m_bImgInit=m_CarmAVT.OpenCarm();
	
	return (m_bImgInit);
}

void CImgStatic::ImgFree()
{
	ImgStop();
	m_camHK.CloseCamera();
//	CKSVDHEx::FreeDH();
    m_Pat.FreePattern();
	CKSoftClass::Free();
	this->DestroyWindow();
}

void CImgStatic::ImgLive()
{
	m_camHK.SetTriggerMode(false);
//	m_CarmAVT.Carmlive();
}

void CImgStatic::ImgStop()
{
	m_camHK.SetTriggerMode(true);
//	m_CarmAVT.StopCarm();
}

void CImgStatic::CCDSoftTriggerOnce()
{
	m_camHK.SoftTrigger();
}

void CImgStatic::DrawCurrentPos()
{
	if(GetSafeHwnd()) Invalidate();
}

void CImgStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(GetSafeHwnd() == NULL)	return;

// 	CDC cdc;
// 	CBitmap bitmap;
// 	CRect rect;
// 	GetClientRect(&rect);
// 
// 	cdc.CreateCompatibleDC(&dc);//��������Ļ��ʾ���ݵ��ڴ���ʾ�豸
// 	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С��
// 	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
// 	CBitmap *pOldBit = cdc.SelectObject(&bitmap);
// 	//////////////////////////////////////////////////////////////////////////
// 	CBrush brush;
// 	brush.CreateSolidBrush(RGB(163, 176, 188));   // ����ɫ 
// 	CBrush *pOldBrush = cdc.SelectObject(&brush);
// 	cdc.SetBkMode(TRANSPARENT);
// 	cdc.Rectangle(rect);
// 	//����ͼ����ʾ�����С	
// 	m_tImgRect = GetImgPaintRect(rect);
// 	CFont Font;
// 	Font.CreatePointFont(100, "Arial");
// 	CFont *pOldFont = cdc.SelectObject(&Font);
// 
// 	// ��ȡ�������
// 	if(g_pFrm->m_pSysParam->nCCDType == DefCCDType_Color)
// 	{
// 	   cdc.SetTextColor(RGB(230,255,255));  // �������ɫ
// 	}
// 	else
// 	{
//        cdc.SetTextColor(RGB(0,0,200));      // �������ɫ
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	PaintImage(&cdc, m_tImgRect);//ͼ��
//    
// 	PaintShape(&cdc, m_tImgRect);//�̶ȺͿ��
// 	
// 	//PaintTrack(&cdc,m_tImgRect);//��·��
// 	//////////////////////////////////////////////////////////////////////////
// 	dc.StretchBlt(0,0,rect.Width(), rect.Height(), &cdc, 0, 0,rect.Width(), rect.Height(), SRCCOPY);
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	cdc.SelectObject(pOldFont);
// 	cdc.SelectObject(pOldBrush);
// 	cdc.SelectObject(pOldBit);
// 	bitmap.DeleteObject();
// 	cdc.DeleteDC();
}
CRect CImgStatic::GetImgPaintRect(CRect rect)
{
	double dxScale = double(rect.Width())/m_iImgWidth;
	double dyScale = double(rect.Height())/m_iImgHeight;
	m_dScale = dxScale>dyScale ? dyScale : dxScale;

	CRect rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = static_cast<long>(m_dScale*m_iImgWidth);
	rc.bottom = static_cast<long>(m_dScale*m_iImgHeight);
	
	return rc;
}
tgImgPoint CImgStatic::GetImgDot(CPoint point)
{
	tgImgPoint dot;

	dot.x =static_cast<int>(point.x/m_dScale);
	dot.y = static_cast<int>(point.y/m_dScale);

	return dot;
}

CPoint CImgStatic::GetPoint(tgImgPoint tPoint)
{
	CPoint dot;
	dot.x =static_cast<int>(tPoint.x*m_dScale);
	dot.y = static_cast<int>(tPoint.y*m_dScale);
	return dot;
}

CPoint CImgStatic::GetPoint(double dX, double dY, double dScale, double dOffsetX, double dOffsetY, BOOL bTrack)
{
	CPoint dot;
	dot.x = static_cast<long>((dX+dOffsetX)*dScale);
	dot.y = static_cast<long>((dY+dOffsetY)*dScale);
	
	if(bTrack)
	{
		dot.x += m_tTrackDrawOffset.x;
		dot.y += m_tTrackDrawOffset.y;
	}
	//dot.x = static_cast<long>(m_lXlen - dot.x);
	//dot.x = static_cast<long>(dot.x);
	return dot;
}

tgImgRect CImgStatic::GetImgRect(CRect rect)
{
	tgImgRect rc;
	tgImgPoint t;
	int iTemp;
	t = GetImgDot(rect.TopLeft());
	rc.left = t.x; rc.top = t.y;
	t = GetImgDot(rect.BottomRight());
	rc.right = t.x; rc.bottom = t.y;

	if(rc.left > rc.right) 
	{
		iTemp = rc.left;
		rc.left = rc.right;
		rc.right = iTemp;
	}

	if(rc.top > rc.bottom) 
	{
		iTemp = rc.top;
		rc.top = rc.bottom;
		rc.bottom = iTemp;
	}
	if( rc.right > m_iImgWidth-1)	rc.right = m_iImgWidth-1;
	if( rc.bottom > m_iImgHeight-1)	rc.bottom = m_iImgHeight-1;

	TRACE(_T("\nL=%d,T=%d,B=%d,R=%d\n"), rc.left, rc.top, rc.bottom, rc.right);
	return rc;
}

void CImgStatic::LoadImageFromFile(CString strPath)
{
// 	if (NULL == m_pImageBuffer)
// 	{
// 		m_pImageBuffer = new BYTE[IMG_WIDTH*IMG_HEIGHT+2048];
		LoadImage(strPath);

		m_pBmpInfo->bmiHeader.biWidth = IMG_WIDTH;
		m_pBmpInfo->bmiHeader.biHeight = IMG_HEIGHT;

//		GetBuff(m_pImageBuffer);
//		Invalidate();
//	}
	//MbufRestore(strPath.GetBuffer(strPath.GetLength()), )
}

void CImgStatic::CopyImage()
{
	if(m_pImageBuffer != NULL)
	{
		if(g_pFrm->m_pSysParam->nCCDType == DefCCDType_Color)
			KS_CopyBuffer(m_KSBuffer, m_pImageBuffer, IMG_WIDTH, IMG_HEIGHT, KS_RGB24);
		else
			KS_CopyBuffer(m_KSBuffer, m_pImageBuffer, IMG_WIDTH, IMG_HEIGHT, KS_RGB8);
	}
}

void CImgStatic::PatCenter(double &dx, double &dy)
{
	// ����Mark��ͼ��λ����ͼ�����ľ���  ���� ��Ӧ ��е�������
	dx = (dx - m_iImgWidth*0.5) * g_pFrm->m_pSysParam->dPixelScale;
	dy = (dy - m_iImgHeight*0.5) * g_pFrm->m_pSysParam->dPixelScale;

	//g_pFrm->m_Robot->m_pController->GetCurPosMM(); 
	g_pFrm->m_Precision.GetCurPosMM();

	double x = 0, y = 0;

	//ͼ���������е������ķ������+��-
	// ��������ĵ�ǰλ��
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
BOOL CImgStatic::PatAlloc(char *szFile)
{
	CopyImage();
	tgImgRect rc = GetImgRect(m_tSelRect);

	if(!m_Pat.AllocPattern(m_KSBuffer, szFile, rc.left, rc.top, rc.Width(), rc.Height()))
	{
		AfxMessageBox("SetModelError.");
		return FALSE;
	}
	KSPat_Param param;
	m_Pat.GetParam(param);
	param.angle=0;
	param.angle_delta=10;
	m_Pat.SetParam(param);
	return TRUE;
}

void CImgStatic::PatShow(HWND hWnd)
{
	KSPat_Param Param;
	m_Pat.GetParam(Param);
	if(Param.sizex < 1 || Param.sizey <1)
		return;

	FreeDisplay(m_PatDisplay, m_PatBuffer);
	AllocDisplay(hWnd, &m_PatDisplay, &m_PatBuffer, static_cast<int>(Param.sizex), static_cast<int>(Param.sizey),1, TRUE);
	KS_CopyBufferByID(m_Pat.GetPatBufferID(), m_PatBuffer);
}

BOOL CImgStatic::PatLoad(char *szFile)
{
	return m_Pat.LoadPattern(szFile);
}
void CImgStatic::PatSave(char *szFile)
{
	m_Pat.SavePattern(szFile);
}
int CImgStatic::PatFind(double dAcceptance,double &dx, double &dy)
{
	CopyImage();

	KSPat_Param param;
	m_Pat.GetParam(param);
	param.angle_enable = FALSE;
	param.angle=0;
	param.angle_delta=180;
	param.acceptance=dAcceptance;
	param.certainty=dAcceptance;
    m_Pat.SetParam(param);

	double pScore;
	long nCount = m_Pat.FindPattern(m_KSBuffer, &pScore, &dx, &dy);
	if(nCount<1)return -1;
	
	//m_Pat.FindPattern(m_KSBuffer, &pScore, &pX, &pY);
	m_tPatPos.x =static_cast<int>(dx);
	m_tPatPos.y = static_cast<int>(m_iImgHeight-dy);
	m_dPatAcceptance = pScore;
	if( m_dPatAcceptance < dAcceptance)
		nCount=0;
	Invalidate();
	return nCount;
}

void CImgStatic::PatSetParam(double dAcceptance)
{
	KSPat_Param param;
	m_Pat.GetParam(param);
	param.acceptance = dAcceptance;
	param.angle=0;
	param.angle_delta=5;
	m_Pat.SetParam(param);
}

double CImgStatic::PatGetParam()
{
	KSPat_Param param;
	m_Pat.GetParam(param);
	return param.acceptance;
}

/////////////////// �̶ȺͿ����ʾ ///////////
void CImgStatic::PaintShape(CDC *pDC, CRect rect)
{
	if(g_pFrm == NULL)
		return;

	CString strTemp;
    CPoint point;
	double dmm;
	CPoint t;
    
    // ��ȡ�������
	int CCDType = g_pFrm->m_pSysParam->nCCDType ;
	if(CCDType == DefCCDType_Color)
	{
		// ��ɫ��� 
		CPen pen(PS_DOT, 1, RGB(255,255,255));                            // ����ʮ�̶ֿ���ɫ 
		CPen *pOldPen = pDC->SelectObject(&pen);

		if((g_pFrm->m_dlgMark.GetSafeHwnd() && g_pFrm->m_dlgMark.IsWindowVisible())
			||(g_pFrm->m_dlgPixel.GetSafeHwnd() && g_pFrm->m_dlgPixel.IsWindowVisible())
			||(g_pFrm->m_dlgBoardCheckEx.GetSafeHwnd() && g_pFrm->m_dlgBoardCheckEx.IsWindowVisible()))
		{
			pDC->Draw3dRect(m_tSelRect, RGB(255,240,255), RGB(255,255,255));  // ��ѡ��ɫ
		}

		pDC->Draw3dRect(rect, RGB(0,240,0), RGB(0,240,0));                     // �����ɫ 
		DrawCross(pDC, rect.CenterPoint(), rect.Width()/2, rect.Height()/2);

		strTemp.Format("%d,%d", m_ImgPoint.x, m_ImgPoint.y);

		pDC->TextOut(static_cast<int>(m_tCurPoint.x), static_cast<int>(m_tCurPoint.y+10), strTemp);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID, 2, RGB(0,255,250));                         //  
		pOldPen = pDC->SelectObject(&pen);

		point = GetPoint(m_tPatPos);
		DrawCross(pDC, point, 15,15);

		strTemp.Format("%.2f(%d,%d)", m_dPatAcceptance, point.x, point.y);
		pDC->SetTextColor(RGB(255,0,0));                               // ?
		pDC->TextOut(point.x+10, point.y-20, strTemp);
		pDC->SetTextColor(RGB(255,0,0));                                  
		//////////////////////////////////////////////////////////////////////////
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID, 1, RGB(255,255,255));                    // ʮ�ִ�̶Ⱥ�Բ����ɫ
		pOldPen = pDC->SelectObject(&pen);

		//��0.5mm�̶�
		dmm = 0.5/g_pFrm->m_pSysParam->dPixelScale * m_dScale;	
		
		int i=0;
		int x = rect.CenterPoint().x;
		int y = rect.CenterPoint().y;
		t.x = x; t.y = y;
		DrawEllipse(pDC, t, g_pFrm->m_pSysParam->dDrawCircleR1, FALSE);//�����Ļ�Բ
		DrawEllipse(pDC, t, g_pFrm->m_pSysParam->dDrawCircleR2, FALSE);//�����Ļ�Բ
		while(1)
		{
			t.x = static_cast<int>(x - dmm*i);
			t.y = static_cast<int>(y - dmm*i);

			if(t.x <=0 && t.y<=0)break;
			pDC->MoveTo(t.x, y-8);
			pDC->LineTo(t.x, y+8);
			pDC->MoveTo(x-8, t.y);
			pDC->LineTo(x+8, t.y);

			t.x = static_cast<int>(x + dmm*i);
			t.y = static_cast<int>(y + dmm*i);

			pDC->MoveTo(t.x, y-8);
			pDC->LineTo(t.x, y+8);
			pDC->MoveTo(x-8, t.y);
			pDC->LineTo(x+8, t.y);

			i++;
		}
	    pDC->SelectObject(pOldPen);
	}else
	{
		// �ڰ����
		CPen pen(PS_DOT, 1, RGB(255,0,0));                            // ����ʮ�̶ֿ���ɫ 
		CPen *pOldPen = pDC->SelectObject(&pen);

		if((g_pFrm->m_dlgMark.GetSafeHwnd() && g_pFrm->m_dlgMark.IsWindowVisible())
			||(g_pFrm->m_dlgPixel.GetSafeHwnd() && g_pFrm->m_dlgPixel.IsWindowVisible())
			||(g_pFrm->m_dlgBoardCheckEx.GetSafeHwnd() && g_pFrm->m_dlgBoardCheckEx.IsWindowVisible()))
		{
			pDC->Draw3dRect(m_tSelRect, RGB(200,0,0), RGB(255,0,0));  // ��ѡ��ɫ
		}

		pDC->Draw3dRect(rect, RGB(0,240,0), RGB(0,240,0));                     // �����ɫ 
		DrawCross(pDC, rect.CenterPoint(), rect.Width()/2, rect.Height()/2);

		strTemp.Format("%d,%d", m_ImgPoint.x, m_ImgPoint.y);

		pDC->TextOut(static_cast<int>(m_tCurPoint.x), static_cast<int>(m_tCurPoint.y+10), strTemp);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID, 2, RGB(0,255,250));                         //  
		pOldPen = pDC->SelectObject(&pen);

		point = GetPoint(m_tPatPos);
		DrawCross(pDC, point, 15,15);

		strTemp.Format("%.2f(%d,%d)", m_dPatAcceptance, point.x, point.y);
		pDC->SetTextColor(RGB(255,0,0));                               // ?
		pDC->TextOut(point.x+10, point.y-20, strTemp);
		pDC->SetTextColor(RGB(255,0,0));                                  
		//////////////////////////////////////////////////////////////////////////
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID, 1, RGB(255,0,0));                    // ʮ�ִ�̶Ⱥ�Բ����ɫ
		pOldPen = pDC->SelectObject(&pen);

		//��0.5mm�̶�
		dmm = 0.5/g_pFrm->m_pSysParam->dPixelScale * m_dScale;	

		int i=0;
		int x = rect.CenterPoint().x;
		int y = rect.CenterPoint().y;
		t.x = x; t.y = y;
		DrawEllipse(pDC, t, g_pFrm->m_pSysParam->dDrawCircleR1, FALSE);//�����Ļ�Բ
		DrawEllipse(pDC, t, g_pFrm->m_pSysParam->dDrawCircleR2, FALSE);//�����Ļ�Բ
		while(1)
		{
			t.x = static_cast<int>(x - dmm*i);
			t.y = static_cast<int>(y - dmm*i);

			if(t.x <=0 && t.y<=0)break;
			pDC->MoveTo(t.x, y-8);
			pDC->LineTo(t.x, y+8);
			pDC->MoveTo(x-8, t.y);
			pDC->LineTo(x+8, t.y);

			t.x = static_cast<int>(x + dmm*i);
			t.y = static_cast<int>(y + dmm*i);

			pDC->MoveTo(t.x, y-8);
			pDC->LineTo(t.x, y+8);
			pDC->MoveTo(x-8, t.y);
			pDC->LineTo(x+8, t.y);

			i++;
		}
		pDC->SelectObject(pOldPen);
	}
}

void CImgStatic::ReverseY(BYTE* pBuf, BYTE* pDestBuf, const int width, const int height)
{
	if (NULL == pBuf || NULL==pDestBuf)
	{
		return;
	}

	for (int i=0; i<height; i++)
	{
		memcpy(pDestBuf+i*width, pBuf+(height-1-i)*width, sizeof(BYTE)*width);
	}
}

void CImgStatic::PaintImage(CDC *pDC, CRect rect)
{
	if(m_pImageBuffer == NULL)
		return;

//	ReverseY(m_pImageBuffer, m_pBufShow, IMG_WIDTH, IMG_HEIGHT);
// 	BOOL bRtn = DrawDibDraw(m_hDrawDib, 
// 		pDC->GetSafeHdc(), 
//  		rect.left, rect.top,
//  		rect.Width(), rect.Height(),
// 		&m_pBmpInfo->bmiHeader,
// 		//m_pBufShow,
// 		m_pImageBuffer, 
// 		0,
// 		0,
// 		m_pBmpInfo->bmiHeader.biWidth,
// 		m_pBmpInfo->bmiHeader.biHeight, 
// 		SRCCOPY);
// 
// 	if (!bRtn)
// 	{
// 		TRACE("DrawDibDraw Error!");
// 	}
}
void CImgStatic::DrawArc(CDC *pDC, CRect rect, CPoint tStartPoint, CPoint tEndPoint)
{
	pDC->Arc(rect, tStartPoint, tEndPoint);
}

void CImgStatic::DrawEllipse(CDC *pDC, CPoint point, int r, BOOL bFull)
{
	if(!bFull)
	{
		CGdiObject *pOldGdi = pDC->SelectStockObject(NULL_BRUSH);
		pDC->Ellipse(point.x-r*m_dScale, point.y-r*m_dScale, point.x+r*m_dScale, point.y+r*m_dScale);
		pDC->SelectObject(pOldGdi);
	}
	else
	{
		pDC->Ellipse(point.x-r, point.y-r, point.x+r, point.y+r);
	}
}


void CImgStatic::DrawCircle(CDC *pDC, CPoint point, int r, BOOL bFull)
{
	if(!bFull)
	{
		CGdiObject *pOldGdi = pDC->SelectStockObject(NULL_BRUSH);
		pDC->Ellipse(point.x-r, point.y-r, point.x+r, point.y+r);
		pDC->SelectObject(pOldGdi);
	}
	else
	{
		pDC->Ellipse(point.x-r, point.y-r, point.x+r, point.y+r);
	}
}
void CImgStatic::DrawCircleEx(CDC *pDC, CPoint point, int r, BOOL bFull)
{
	CBrush brush(RGB(240,0,0));
	CBrush *pOldBrush = pDC->SelectObject(&brush);
	if(!bFull)
	{
		CGdiObject *pOldGdi = pDC->SelectStockObject(NULL_BRUSH);
		pDC->Ellipse(point.x-r, point.y-r, point.x+r, point.y+r);
		pDC->SelectObject(pOldGdi);
	}
	else
	{
		pDC->Ellipse(point.x-r, point.y-r, point.x+r, point.y+r);
	}
	pDC->SelectObject(pOldBrush);
}

void CImgStatic::DrawCross(CDC *pDC, CPoint point, int rw, int rh)
{
	pDC->MoveTo(point.x, point.y-rh);
	pDC->LineTo(point.x, point.y+rh);
	pDC->MoveTo(point.x-rw, point.y);
	pDC->LineTo(point.x+rw, point.y);
}
//
//void CImgStatic::SetDrawCmdLine(int ist,int iend)
//{
//	m_iStLn = ist;
//	m_iEndLn = iend;
//}
//
//void CImgStatic::GetTrackRect()
//{
//	m_TrackRect = GetTrackRect(m_iStLn,m_iEndLn,0,0);//Top �� Botton ����
//}
void CImgStatic::GetCmdScale()
{
	double dx = g_pFrm->m_mtrParamGts[X_AXIS].m_MaxTravelPositive; //x���г�
	if(dx < g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive)//y���г�
		dx = g_pFrm->m_mtrParamGts[Y_AXIS].m_MaxTravelPositive;

	m_dCmdScale = m_tImgRect.Height()/dx;//x,y���г̽ϴ����
   // m_dCmdScale = m_tImgRect.Height()/50.0;
	m_tTrackDrawOffset.x=0;
	m_tTrackDrawOffset.y=0;

	/*double size[3];
	size[0] = m_tImgRect.Height()/m_TrackRect.Height();
	size[1] = m_tImgRect.Width()/m_TrackRect.Width();
	if(m_TrackRect.Height()-0<EPS) size[0] = 1;
	if(m_TrackRect.Width()-0<EPS) size[1] = 1;
	if(size[0]<size[1])
	{
		m_dCmdScale = size[0];
		m_wOrH = 2;
	}
	else
	{
		m_dCmdScale = size[1];
		m_wOrH = 1;
	}
	if(m_dCmdScale <= 0) m_dCmdScale = 1;
	m_dOldCmdScale = m_dCmdScale;
	m_tTrackDrawOffset.x=-(int)(m_TrackRect.left*m_dCmdScale);
	m_tTrackDrawOffset.y=-(int)(m_TrackRect.bottom*m_dCmdScale);
	m_dZoomOffsetX=0;
	m_dZoomOffsetY=0;
	m_Track.SetOffset(m_tTrackDrawOffset.x,m_tTrackDrawOffset.y);*/
}

void CImgStatic::PaintTrack(CDC *pDC, CRect rect)
{
	if(g_pFrm == NULL)return;
    CPoint p1;
	CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen *pOldPen=pDC->SelectObject(&redPen);
	if(g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_RUN)
	{
		p1 = GetPoint(g_pFrm->m_Robot->g_CurDrawPos[0], g_pFrm->m_Robot->g_CurDrawPos[1], m_dCmdScale, 0, 0, TRUE);
		DrawCross(pDC, p1, 8, 8);
	}
	pDC->SelectObject(pOldPen);

	CPen pen(PS_SOLID, 2, RGB(100, 100, 250));
	pOldPen = pDC->SelectObject(&pen);
	CBrush brush;
	brush.CreateSolidBrush(RGB(150, 255, 50));
	CBrush *pOldBrush = pDC->SelectObject(&brush);

	//////////////////////////////////////////////////////////////////////////
	WaitForSingleObject(g_runListEvent, 0);
	int iCount=g_pFrm->m_wndEditBar.m_rawList.GetCount();
	g_runListEvent.SetEvent();
	m_cmdParam.reset();

	//for (int i=0;i<TOOL_NUM;i++)
	//	PaintTrack(pDC, rect, m_dCmdScale, 0, iCount,g_pFrm->m_pSysParam->tOffset[i].x,g_pFrm->m_pSysParam->tOffset[i].y);

	PaintTrack(pDC, rect, m_dCmdScale, 0, iCount,0,0);

	//if (g_pFrm->m_Robot->m_tSysStatus != K_RUN_STS_RUN )
	//	DrawCurSelect(pDC,m_CurSelectLine);

	//////////////////////////////////////////////////////////////////////////
	//Change DC
	//CPen pen2(PS_SOLID, 2, RGB(0,255, 100));
	//pDC->SelectObject(&pen2);
	//m_Track.Paint(pDC,rect,m_dCmdScale);


/////////////////////////////////////////////////////////////////////////////////
	/*
	if(g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_RUN )
	{
		CPen pen2(PS_SOLID, 3, RGB(255, 20, 0));
		pOldPen = pDC->SelectObject(&pen2);

		if ( !g_pFrm->m_Robot->m_drawTrack )  		
			g_pFrm->m_Robot->m_bStartOpenDisp=false;	

		if ( g_pFrm->m_Robot->m_drawTrack && !g_pFrm->m_Robot->m_bStartOpenDisp )
		{	
			g_pFrm->m_Robot->m_bStartOpenDisp=true;			
			p1 = GetPoint(g_pFrm->m_Robot->g_EndDrawPos[0], g_pFrm->m_Robot->g_EndDrawPos[1], m_dCmdScale, 0, 0, TRUE);
			
			m_Draw_CurP.x=p1.x;
			m_Draw_CurP.y=p1.y;
		}

		if( g_pFrm->m_Robot->m_drawTrack )
		{
			if (g_pFrm->m_Robot->m_drawP<MaxDRAWDot)
			{				
				m_DrawS[g_pFrm->m_Robot->m_drawP].x=m_Draw_CurP.x;
				m_DrawS[g_pFrm->m_Robot->m_drawP].y=m_Draw_CurP.y;

				p1 = GetPoint(g_pFrm->m_Robot->g_EndDrawPos[0], g_pFrm->m_Robot->g_EndDrawPos[1], m_dCmdScale, 0, 0, TRUE);
				m_DrawE[g_pFrm->m_Robot->m_drawP].x=p1.x;
				m_DrawE[g_pFrm->m_Robot->m_drawP].y=p1.y;
				m_Draw_CurP.x=p1.x; m_Draw_CurP.y=p1.y;

				g_pFrm->m_Robot->m_drawP++;
			}
			else
			{
				g_pFrm->m_Robot->m_drawP=0;
			}
		}
		if (g_pFrm->m_Robot->m_drawP<MaxDRAWDot)
		{
			for (int i=0;i<g_pFrm->m_Robot->m_drawP;i++)
			{		
				pDC->MoveTo(m_DrawS[i].x, m_DrawS[i].y);
				pDC->LineTo(m_DrawE[i].x, m_DrawE[i].y);
			}
		}
	}
	else
	{
		g_pFrm->m_Robot->m_drawP=0;
		ZeroMemory(m_DrawS, sizeof(m_DrawS));
	}
	*/

	//////////////////////////////////////////////////////////////////////////
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CImgStatic::PaintTrack(CDC *pDC, CRect rect, double dCmdScale, int iStartIndex, int iEndIndex, double dOffsetX, double dOffsetY, BOOL bStepCmd)
{
	//////////////////////////////////////////////////////////////////////////
	int i=0, j=0, m=0, n=0;
	
	tgArc tArc;
	CPoint p1, p2,p3;
	int iTemp=0;
	double dXc, dYc, dRc;
	CString strTemp;
	double x=0, y=0;
	m_lXlen = rect.Width();
	int nx=0, ny=0;
    int y_REP;int x_REP;
	tgCmdLine pPaintCmd;

	//////////////////////////////////////////////////////////////////////////

	CString strText;
	int nRunIndex = 1;
	for(i=iStartIndex; i<iEndIndex; i++)
	{
		WaitForSingleObject(g_runListEvent, 0);
		POSITION posIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(i);
		pPaintCmd = g_pFrm->m_wndEditBar.m_rawList.GetAt(posIndex);

		g_runListEvent.SetEvent();

		switch(pPaintCmd.tType)
		{
		case CMD_NONE:
			break;
		case CMD_MARK:
			if(bStepCmd)continue;
			p1 = GetPoint(pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
			DrawCross(pDC, p1, 10, 10);
			DrawCircle(pDC, p1, 5);
			pDC->SetTextColor(RGB(200,0,0));
			pDC->TextOut(p1.x, p1.y, "M1");
			if(g_pFrm->m_wndEditBar.IsSelState(i))DrawCircleEx(pDC, p1, 4);

			p1 = GetPoint(pPaintCmd.pDblValue[4], pPaintCmd.pDblValue[5], dCmdScale, dOffsetX, dOffsetY, TRUE);
			DrawCross(pDC, p1, 10, 10);
			DrawCircle(pDC, p1, 5);
			pDC->TextOut(p1.x, p1.y, "M2");
			break;
		case CMD_POINT:
			p1 = GetPoint(pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
			DrawCircle(pDC, p1, 2);

			strText.Format(_T("%d"), nRunIndex++);
			pDC->SetTextColor(RGB(0,0,255));
			pDC->TextOut(p1.x, p1.y, strText);
			break;//����㽺,tgPos[1]
		case CMD_LINE_START:
			p1 = GetPoint(pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
			pDC->MoveTo(p1);
			DrawCircle(pDC, p1, 2);
			pDC->SetTextColor(RGB(200,0,0));
			pDC->TextOut(p1.x-10, p1.y, "S");
		
			strText.Format(_T("%d"), nRunIndex++);
			pDC->SetTextColor(RGB(0,0,255));
			pDC->TextOut(p1.x-10, p1.y-18, strText);
			break;//�߶ε㽺���tgPos[1]

		case CMD_LINE_PASSING:
			if(pPaintCmd.pIntValue[6] && pPaintCmd.pDblValue[6]>1)
			{
				tgCmdLine tPreDotEx,tNextDotEx;
				int m0=-1, m2=-1;
				m0 = g_pFrm->m_wndEditBar.GetPreTrackPoint(i, TRUE);
				m2 = g_pFrm->m_wndEditBar.GetNextTrackPoint(i, TRUE);
				if(m2<0) 
					goto lab;

				POSITION PreposIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(m0);
				tPreDotEx = g_pFrm->m_wndEditBar.m_rawList.GetAt(PreposIndex);

				POSITION NextposIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(m2);
				tNextDotEx = g_pFrm->m_wndEditBar.m_rawList.GetAt(NextposIndex);

				double m_CirX=0,m_CirY=0;
				BOOL dir;
				BOOL IsRetOK=g_pFrm->m_wndEditBar.TrackArcHandle(tPreDotEx,pPaintCmd,tNextDotEx,pPaintCmd.pDblValue[6],m_CirX,m_CirY,dir);
				if (IsRetOK==TRUE)
				{
					p1 = GetPoint(tPreDotEx.pDblValue[0], tPreDotEx.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
					p2 = GetPoint(m_CirX, m_CirY, dCmdScale, dOffsetX, dOffsetY, TRUE);//Բ��
					p3 = GetPoint(tNextDotEx.pDblValue[0], tNextDotEx.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);//Բ��
					iTemp = static_cast<int>(pPaintCmd.pDblValue[6]* dCmdScale);
					pDC->LineTo(p1);
					pDC->MoveTo(p1);

					if(dir==TRUE)//��ʱ��
						DrawArc(pDC, CRect(p2.x - iTemp, p2.y-iTemp, p2.x + iTemp, p2.y +iTemp), CPoint(p1.x, p1.y), CPoint(p3.x, p3.y));
					else
						DrawArc(pDC, CRect(p2.x - iTemp, p2.y-iTemp, p2.x + iTemp, p2.y +iTemp), CPoint(p3.x, p3.y), CPoint(p1.x, p1.y));

					//DrawCross(pDC, p2, 5,5);//Բ��
					p1 = GetPoint(pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
					DrawCircle(pDC, p1, 2);
					pDC->MoveTo(p3);
					break;
				}

			lab:p1 = GetPoint(pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
				pDC->LineTo(p1);
				DrawCircle(pDC, p1, 2);
			}
			else
			{
				p1 = GetPoint(pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
				pDC->LineTo(p1);
				DrawCircle(pDC, p1, 2);
			}			
			
			break;// �߶ε㽺�м��tgPos[1]
		case CMD_LINE_ARC:
			if(!g_pFrm->m_wndEditBar.GetCmdArcInfo(pPaintCmd, i, &tArc))
				return;
			p1 = GetPoint(tArc.dx[0], tArc.dy[0], dCmdScale, dOffsetX, dOffsetY, TRUE);
			p2 = GetPoint(tArc.x, tArc.y, dCmdScale, dOffsetX, dOffsetY, TRUE);//Բ��
			p3 = GetPoint(tArc.dx[2], tArc.dy[2], dCmdScale, dOffsetX, dOffsetY, TRUE);//Բ��
			iTemp = static_cast<int>(tArc.r * dCmdScale);
		//	if(tArc.angle>0)
			if(tArc.angle<0)
				DrawArc(pDC, CRect(p2.x - iTemp, p2.y-iTemp, p2.x + iTemp, p2.y +iTemp), CPoint(p1.x, p1.y), CPoint(p3.x, p3.y));
			else
				DrawArc(pDC, CRect(p2.x - iTemp, p2.y-iTemp, p2.x + iTemp, p2.y +iTemp), CPoint(p3.x, p3.y), CPoint(p1.x, p1.y));
			//DrawCross(pDC, p2, 5,5);//Բ��
			p1 = GetPoint(pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
			DrawCircle(pDC, p1, 2);
			pDC->MoveTo(p3);

			strText.Format(_T("%d"), nRunIndex++);
			pDC->SetTextColor(RGB(0,0,255));
			pDC->TextOut(p1.x, p1.y, strText);
			break;//Բ���㽺 tgPos[1]
		case CMD_CIRCLE:
			if(pPaintCmd.pIntValue[4]==0)//Բ�ģ�ֱ��
			{
				p1 = GetPoint(pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
				iTemp = static_cast<int>(pPaintCmd.pDblValue[4]*dCmdScale*0.5/*�뾶*/);
				if(iTemp<1)iTemp = 1;
				DrawCircle(pDC, p1, 2);
				DrawCircle(pDC, p1, iTemp, FALSE);
			}
			else//����
			{
				CDlgPathEdit::GetCircle(&pPaintCmd, &dXc, &dYc, &dRc);
				p1 = GetPoint(dXc, dYc, dCmdScale, dOffsetX, dOffsetY, TRUE);
				iTemp = static_cast<int>(dRc *dCmdScale);
				if(iTemp<1)iTemp=1;
				DrawCircle(pDC, p1, iTemp, FALSE);
				for(j=0; j<3; j++)
				{
					p1 = GetPoint(pPaintCmd.pDblValue[j*4+0], pPaintCmd.pDblValue[j*4+1], dCmdScale, dOffsetX, dOffsetY, TRUE);
					DrawCircle(pDC, p1, 2);
				}
			}

			strText.Format(_T("%d"), nRunIndex++);
			pDC->SetTextColor(RGB(0,0,255));
			pDC->TextOut(p1.x, p1.y, strText);
			break;//Բ�ε㽺 1:break;Բ��+ֱ��, tgPos[1], double diameter, 2:break;3�㻭Բ,tgPos[3]
		case CMD_LINE_END:
			if(g_pFrm->m_wndEditBar.GetEndPos(&pPaintCmd, i, m_cmdParam.tLine.dTailCloseLen, &x, &y))
			{
				p1 = GetPoint(x, y, dCmdScale, dOffsetX, dOffsetY, TRUE);
				DrawCircle(pDC, p1, 3,FALSE);
			}
			p1 = GetPoint(pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
			pDC->LineTo(p1);
			DrawCircle(pDC, p1, 2);
			pDC->SetTextColor(RGB(200,0,0));
			pDC->TextOut(p1.x+5, p1.y, "E");

			strText.Format(_T("%d"), nRunIndex++);
			pDC->SetTextColor(RGB(0,0,255));
			pDC->TextOut(p1.x+5, p1.y-18, strText);
			break;//�߶ε㽺������,tgPos[1]
		case CMD_DISPENSER:
		/*	p1 = GetPoint(pCmd->pDblValue[0], pCmd->pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
			pDC->LineTo(p1);
			DrawCircle(pDC, p1, 2);*/
			break;//�㽺����/�� BOOL On/Off
		case CMD_GOTO_ADDRESS:
			break;//��תָ�� int iIndex
		case CMD_REPEATX://X�������� int iIndex:break; int stepType(S/N):break; int iXNum:break; int iYNum:break;  double xSpace:break; double ySpace

			 y_REP=pPaintCmd.pIntValue[3];
			if (y_REP>15) y_REP=15;
			 x_REP=pPaintCmd.pIntValue[2];
			if (x_REP>15) x_REP=15;
			for(m=0; m<y_REP; m++)//y
			{
				for(n=0; n<x_REP; n++)//x
				{
					if(m==0 && n==0)continue;
					if(pPaintCmd.pIntValue[1] ==0)//S
					{
						if((m%2) == 0) nx = n;
						else nx = pPaintCmd.pIntValue[2] -1 - n;
					}
					else
					{
						nx = n;
					}
					ny = m;

					PaintTrack(pDC, rect, dCmdScale,  pPaintCmd.pIntValue[0], i, pPaintCmd.pDblValue[0]*nx+dOffsetX, pPaintCmd.pDblValue[1]*ny+dOffsetY, TRUE);
				}
			}
			break;
		case CMD_REPEATY://Y�������� int iIndex:break; int stepType(S/N):break; int iXNum:break; int iYNum:break;  double xSpace:break; double ySpace
			 y_REP=pPaintCmd.pIntValue[3];
			 if(y_REP>15) y_REP=15;
			 x_REP=pPaintCmd.pIntValue[2];
			if (x_REP>15) x_REP=15;
			for(m=0; m<x_REP; m++)//x
			{
				for(n=0; n<y_REP; n++)//y
				{
					if(m==0 && n==0)continue;
					nx = m;
					if(pPaintCmd.pIntValue[1] ==0)//S
					{
						if((m%2) == 0) ny = n;
						else ny = pPaintCmd.pIntValue[3] -1 - n;
					}
					else
					{
						ny = n;
					}
					PaintTrack(pDC, rect, dCmdScale, pPaintCmd.pIntValue[0], i, pPaintCmd.pDblValue[0]*nx+dOffsetX, pPaintCmd.pDblValue[1]*ny+dOffsetY, TRUE);
				}
			}
			break;
		case CMD_BRUSH_AREA:			
			if(pPaintCmd.pIntValue[5] == 0)//����
			{
				vector <tgBrushEx> brushRunList;
				tgBrushEx brushLine;				
				tgBrush tArea;
				BOOL bSet = FALSE;

				tArea.nDir = pPaintCmd.pIntValue[4]==0?0:1;//0--X����Ϳ��  1--Y����Ϳ��

				if(g_pFrm->m_wndEditBar.GetCmdBrushInfo(pPaintCmd,&tArea)==FALSE) break;

				brushLine.x=pPaintCmd.pDblValue[0];
				brushLine.y=pPaintCmd.pDblValue[1];
				brushLine.tType=CMD_LINE_START;
				brushRunList.push_back(brushLine);
	
				for (int i=0;i<(2*tArea.nNum+1);i++)
				{//����Բ�����ǹ���
					////step1
					brushLine.x = pPaintCmd.pDblValue[0] + (tArea.nDir==0?0:1)*((i/2)%2==0?1:0)*tArea.dx + (tArea.nDir==0?1:0)*((i/2)*tArea.dx +((i%2)==0?0:1)*tArea.dx);
					brushLine.y = pPaintCmd.pDblValue[1] + (tArea.nDir==0?1:0)*((i/2)%2==0?1:0)*tArea.dy + (tArea.nDir==0?0:1)*((i/2)*tArea.dy +((i%2)==0?0:1)*tArea.dy);

					bSet = (i%2)==0?TRUE:FALSE;//ż����λ�û��ж��Ƿ�ֹͣѹ��ָ��
					if(bSet==TRUE)
					{
						if (tArea.nDir==0)
						{//tArea.nDir==0 X����Ϳ��						
							if(fabs(pPaintCmd.pDblValue[4] - brushLine.x)<fabs(tArea.dx))
							{
								brushLine.tType=CMD_LINE_END;
								brushRunList.push_back(brushLine);
								break;
							}
						}
						else
						{//tArea.nDir==1 Y����Ϳ��
							if(fabs(pPaintCmd.pDblValue[5] - brushLine.y)<fabs(tArea.dy))
							{
								brushLine.tType=CMD_LINE_END;
								brushRunList.push_back(brushLine);
								break;
							}
						}
					}
					brushLine.tType=CMD_LINE_PASSING;
					brushRunList.push_back(brushLine);
				}
				////////////////////////////////////////////////////////////////////////
				for(UINT i=0;i<brushRunList.size();i++)
				{
					if(brushRunList[i].tType==CMD_LINE_START)
					{
						p1 = GetPoint(brushRunList[i].x,brushRunList[i].y, dCmdScale, dOffsetX, dOffsetY, TRUE);
						pDC->MoveTo(p1);
						pDC->TextOut(p1.x, p1.y, "S");
					}
					else if(brushRunList[i].tType==CMD_LINE_PASSING)
					{	
						tgBrushEx brush[3];
						brush[0]=brushRunList[i-1];
						brush[1]=brushRunList[i];
						brush[2]=brushRunList[i+1];

						tgCmdLine tPreDotEx ,tCurDoxEx,tNextDotEx;
						tPreDotEx.pDblValue[0] = brush[0].x;
						tPreDotEx.pDblValue[1] = brush[0].y;
						tCurDoxEx.pDblValue[0] = brush[1].x;
						tCurDoxEx.pDblValue[1] = brush[1].y;
						tNextDotEx.pDblValue[0] = brush[2].x;
						tNextDotEx.pDblValue[1] = brush[2].y;

						if(pPaintCmd.pIntValue[6]==2)//����Բ��
						{
							double m_CirX=0,m_CirY=0;
							BOOL dir=FALSE;
							BOOL  bRet=g_pFrm->m_wndEditBar.TrackArcHandle(tPreDotEx,tCurDoxEx,tNextDotEx,pPaintCmd.pDblValue[9],m_CirX,m_CirY,dir);
							if (bRet==TRUE)
							{
								p1 = GetPoint(tPreDotEx.pDblValue[0], tPreDotEx.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
								p2 = GetPoint(m_CirX, m_CirY, dCmdScale, dOffsetX, dOffsetY, TRUE);//Բ��
								p3 = GetPoint(tNextDotEx.pDblValue[0], tNextDotEx.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);//Բ��
								iTemp = static_cast<int>(pPaintCmd.pDblValue[9]* dCmdScale);
								pDC->LineTo(p1);
								pDC->MoveTo(p1);
								if(dir==TRUE)//��ʱ��
									DrawArc(pDC, CRect(p2.x - iTemp, p2.y-iTemp, p2.x + iTemp, p2.y +iTemp), CPoint(p1.x, p1.y), CPoint(p3.x, p3.y));
								else
									DrawArc(pDC, CRect(p2.x - iTemp, p2.y-iTemp, p2.x + iTemp, p2.y +iTemp), CPoint(p3.x, p3.y), CPoint(p1.x, p1.y));
								pDC->MoveTo(p3);
							}
							else
							{
								p1 = GetPoint(tCurDoxEx.pDblValue[0],tCurDoxEx.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
								pDC->LineTo(p1);
								pDC->MoveTo(p1);
							}
						}						
						else
						{
							p1 = GetPoint(tCurDoxEx.pDblValue[0],tCurDoxEx.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
							pDC->LineTo(p1);
							pDC->MoveTo(p1);
						}
					}
					else if(brushRunList[i].tType==CMD_LINE_END)
					{
						p1 = GetPoint(brushRunList[i].x,brushRunList[i].y, dCmdScale, dOffsetX, dOffsetY, TRUE);
						pDC->LineTo(p1);
						pDC->TextOut(p1.x, p1.y, "E");
					}
				}
				//���궨�Ľ�����
				p1 = GetPoint(pPaintCmd.pDblValue[4], pPaintCmd.pDblValue[5], dCmdScale, dOffsetX, dOffsetY, TRUE);
				DrawCircle(pDC, p1, 2);

			}
			else//Բ��
			{
				tgBrush tArea;
				double arcx=0.0,arcy=0.0;

				if(g_pFrm->m_wndEditBar.GetCmdBrushInfo(pPaintCmd,&tArea)==FALSE) break;

				cux=tArea.x;
				cuy=tArea.y;
				double dRadius=tArea.dRadius;
				for(j = 0; j < tArea.nNum; j ++)
				{	
					if(dRadius<tArea.dStep) break;
					if(0 == j % 2)
					{
						if(tArea.dAngle>90 && tArea.dAngle <=180) 
						{
							x=tArea.dfCx[0]-dRadius*cos(tArea.dAngle*M_PI/180);
							y=tArea.dfCy[0]-dRadius*sin(tArea.dAngle*M_PI/180);
							arcx=(x+cux)/2.0;
							arcy=(y+cuy)/2.0;
						}
						else if(tArea.dAngle<90 && tArea.dAngle>0)
						{
							x=tArea.dfCx[0]+dRadius*cos((180+tArea.dAngle)*M_PI/180);
							y=tArea.dfCy[0]+dRadius*sin((180+tArea.dAngle)*M_PI/180);
							arcx=(x+cux)/2.0;
							arcy=(y+cuy)/2.0;
						}

						else if(tArea.dAngle<-90 && tArea.dAngle>=-180)
						{
							x=tArea.dfCx[0]+dRadius*cos((180+tArea.dAngle)*M_PI/180);
							y=tArea.dfCy[0]+dRadius*sin((180+tArea.dAngle)*M_PI/180);
							arcx=(x+cux)/2.0;
							arcy=(y+cuy)/2.0;
						}

						else if(tArea.dAngle>-90 && tArea.dAngle<0)
						{
							x=tArea.dfCx[0]+dRadius*cos((180+tArea.dAngle)*M_PI/180);
							y=tArea.dfCy[0]+dRadius*sin((180+tArea.dAngle)*M_PI/180);
							arcx=(x+cux)/2.0;
							arcy=(y+cuy)/2.0;
						}
					}
					else
					{
						if(tArea.dAngle>90 && tArea.dAngle <=180) 
						{
							x=tArea.dfCx[0]-dRadius*cos((180-tArea.dAngle)*M_PI/180);
							y=tArea.dfCy[0]+dRadius*sin((180-tArea.dAngle)*M_PI/180);
							arcx=(x+cux)/2.0;
							arcy=(y+cuy)/2.0;
						}
						else if(tArea.dAngle<90 && tArea.dAngle>0)
						{
							x=tArea.dfCx[0]+dRadius*cos(tArea.dAngle*M_PI/180);
							y=tArea.dfCy[0]+dRadius*sin(tArea.dAngle*M_PI/180);
							arcx=(x+cux)/2.0;
							arcy=(y+cuy)/2.0;
						}

						else if(tArea.dAngle<-90 && tArea.dAngle>=-180)
						{
							x=tArea.dfCx[0]+dRadius*cos((360+tArea.dAngle)*M_PI/180);
							y=tArea.dfCy[0]+dRadius*sin((360+tArea.dAngle)*M_PI/180);
							arcx=(x+cux)/2.0;
							arcy=(y+cuy)/2.0;
						}
						else if(tArea.dAngle>-90 && tArea.dAngle<0)
						{
							x=tArea.dfCx[0]+dRadius*cos((360+tArea.dAngle)*M_PI/180);
							y=tArea.dfCy[0]+dRadius*sin((360+tArea.dAngle)*M_PI/180);
							arcx=(x+cux)/2.0;
							arcy=(y+cuy)/2.0;
						}
						//break;
					}

					p1 = GetPoint(cux, cuy, dCmdScale, dOffsetX, dOffsetY, TRUE);
					p2 = GetPoint(arcx, arcy, dCmdScale, dOffsetX, dOffsetY, TRUE);
					p3 = GetPoint(x, y, dCmdScale, dOffsetX, dOffsetY, TRUE);
					iTemp =(int)(sqrt((cux-x)*(cux-x)+(cuy-y)*(cuy-y)) * dCmdScale/2);
					DrawArc(pDC, CRect(p2.x - iTemp, p2.y-iTemp,p2.x+ iTemp, p2.y +iTemp), p1,p3);
					dRadius-=tArea.dStep;
					if(j < tArea.nNum)
					{
						cux=x;
						cuy=y;
					}
				}
				p1 = GetPoint(tArea.dfCx[0], tArea.dfCy[0], dCmdScale, dOffsetX, dOffsetY, TRUE);
				p3 = GetPoint(x, y, dCmdScale, dOffsetX, dOffsetY, TRUE);
				pDC->MoveTo(p3);
				pDC->LineTo(p1);
			}
			
			break;//Ϳ��
		case CMD_CALL_SUB:
			break;//���ӳ���
		case CMD_LABEL:
			break;//��ǩ
		case CMD_NULL_LINE:
			break;//����ָ��
		case CMD_DELAY:
			//p1 = GetPoint(pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
			//DrawCross(pDC, p1, 5,5);
			//pDC->TextOut(p1.x, p1.y, "W");
			break;//��ͷ���������ض�λ�õȴ�������������������ִ�м�¼�㡣
		case CMD_PAUSE:
			//p1 = GetPoint(pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
			//DrawCross(pDC, p1, 5,5);
			//pDC->TextOut(p1.x, p1.y, "S");
			break;//��ͷ���������ض�λ�ò���ֹͣ�ƶ���ֱ��ʹ���߰��»�̨����ϵ�START��ť���������ִ�м�¼��
		case CMD_HOME:
			p1 = GetPoint(g_pFrm->m_pSysParam->tDefHomePos.x,g_pFrm->m_pSysParam->tDefHomePos.y,dCmdScale,dOffsetX,dOffsetY,TRUE);
			DrawCross(pDC, p1, 5,5);
			pDC->TextOut(p1.x, p1.y, "H");
			strText.Format(_T("%d"), nRunIndex++);
			pDC->SetTextColor(RGB(0,0,255));
			pDC->TextOut(p1.x, p1.y-15, strText);
			break;//X:break; Y:break; Z�Ὣ�ع����ԭ��(�ȴ���)���������ִ�м�¼�㡣
		case CMD_LOOP:
			break;//ͨ����ѭ��ִ�е�ַ��Ϊ1�����̵�һ�������ɺ󣬿����ô˹������趨��������
		case CMD_DUMMY:
			p1 = GetPoint(pPaintCmd.pDblValue[0], pPaintCmd.pDblValue[1], dCmdScale, dOffsetX, dOffsetY, TRUE);
			DrawCross(pDC, p1, 5,5);
			pDC->TextOut(p1.x, p1.y, "N");
			strText.Format(_T("%d"), nRunIndex++);
			pDC->SetTextColor(RGB(0,0,255));
			pDC->TextOut(p1.x, p1.y-15, strText);
			break;//�趨�㽺��ͷʵ�����ڵ�XYZ ����Ϊ�յ㣬����¼��Ŀǰ�ļ�¼��λ�á�ͨ��Ӧ���ڱܿ��������赲��
		case CMD_NOTE:
			break;
		case CMD_END_PROGRAM:
			return;
			break;//�����������
			//////////////////////////////////////////////////////////////////////////
		case PARAM_LINE_SPEED:
			m_cmdParam.tLine.dVel = pPaintCmd.pDblValue[0];
			break;//�߶ε㽺�ٶ�
		case PARAM_LINE_SETUP:
			m_cmdParam.tLine.dPreTime =  pPaintCmd.pDblValue[0];			// ����ʱ��
			m_cmdParam.tLine.dNodeTime =  pPaintCmd.pDblValue[1];		// ��;ʱ��
			m_cmdParam.tLine.dTailTime =  pPaintCmd.pDblValue[3];		// ͣ��ʱ��
			m_cmdParam.tLine.dTailCloseLen =  pPaintCmd.pDblValue[2];	// ����������
			if( pPaintCmd.iDblNum>4)
				m_cmdParam.tLine.dPreMoveTime = 0;//tDot1->pDblValue[4];
			else
				m_cmdParam.tLine.dPreMoveTime = 0;

			break;//�߶ε㽺��������
		case PARAM_LINE_END:
			m_cmdParam.tLine.dEndLen = pPaintCmd.pDblValue[0];
			m_cmdParam.tLine.dEndZupHigh = pPaintCmd.pDblValue[1];
			m_cmdParam.tLine.dEndVel = pPaintCmd.pDblValue[2];
			m_cmdParam.tLine.iEndType = pPaintCmd.pIntValue[0];
			break;//�߶ε㽺��������
		case PARAM_POINT_SETUP:
			m_cmdParam.tPoint.dPreTime = pPaintCmd.pDblValue[0];
			m_cmdParam.tPoint.dDispTime = pPaintCmd.pDblValue[1];
			m_cmdParam.tPoint.dTailTime = pPaintCmd.pDblValue[2];
			break;//����㽺��������
		case PARAM_JET_SETUP:
			m_cmdParam.nHighTime = pPaintCmd.pIntValue[0];
			m_cmdParam.nLowTime = pPaintCmd.pIntValue[1];
			m_cmdParam.lPulseNum = (long)pPaintCmd.pDblValue[0];
			break;
		case PARAM_POINT_END:
			m_cmdParam.tPoint.dZupHVel = pPaintCmd.pDblValue[0];
			m_cmdParam.tPoint.dZupLVel = pPaintCmd.pDblValue[1];
			m_cmdParam.tPoint.dZupHigh = pPaintCmd.pDblValue[2];
			break;//����㽺��������
		case PARAM_Z_CLEARANCE:
			m_cmdParam.dZcleaType = pPaintCmd.pIntValue[0];
			m_cmdParam.dZclearance = pPaintCmd.pDblValue[0];
			break;//Z����߸߶����� 
		case PARAM_Z_WORKPOSSET:
			//m_cmdParam.dZcleaType = pPaintCmd.pIntValue[0];
			//m_cmdParam.dZclearance = pPaintCmd.pDblValue[0];
			break;//Z����߸߶����� 
		case PARAM_Z_SET_HEIGHT:
			m_cmdParam.dZSetType = pPaintCmd.pIntValue[0];
			m_cmdParam.dZSetHeight = pPaintCmd.pDblValue[0];
			break;//Z����߸߶����� 
		case PARAM_XY_VEL:	//XY������ƶ��ٶ�
		case PARAM_Z_VEL:	//Z������ƶ��ٶ�
			m_cmdParam.dVelXY = pPaintCmd.pDblValue[0];
			m_cmdParam.dVelZ = pPaintCmd.pDblValue[1];
			m_cmdParam.dAccXY = pPaintCmd.pDblValue[2];
			m_cmdParam.dAccZ = pPaintCmd.pDblValue[3];
			break;
		default:
			NULL;
		}
		if(g_pFrm->m_wndEditBar.IsSelState(i))
		{
			DrawCircleEx(pDC, p1, 4);
		}
	}
}

void CImgStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPoint oldPt;
	oldPt = point;
	//point.x = static_cast<int>(m_lXlen - point.x);
	point.x = static_cast<int>(point.x);

	if(g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_RUN)
		return;
	if(m_bMouseDown)
	{		
		if((g_pFrm->m_dlgMark.GetSafeHwnd() && g_pFrm->m_dlgMark.IsWindowVisible())
			||(g_pFrm->m_dlgPixel.GetSafeHwnd() && g_pFrm->m_dlgPixel.IsWindowVisible())
			||(g_pFrm->m_dlgBoardCheckEx.GetSafeHwnd() && g_pFrm->m_dlgBoardCheckEx.IsWindowVisible()))
		{
			CPoint t = GetPoint(tgImgPoint(m_iImgWidth/2, m_iImgHeight/2));
			TRACE(_T("\nCPoint t.x = %d, t.y = %d "), t.x, t.y);
			int nx = abs(point.x - t.x);
			int ny = abs(point.y - t.y);
			m_tSelRect.left = t.x-nx;
			m_tSelRect.top = t.y-ny;
			m_tSelRect.right = t.x+nx;
			m_tSelRect.bottom = t.y+ny;
		}
		else
		{
			CRect rect;
			GetClientRect(&rect);
			CPoint t;
			switch(m_tMouseEvent)
			{
			case MK_MOUSE_ZOOM:
				t = point - m_tDownPoint;
				m_dCmdScale = m_dOldCmdScale * (1-(double)t.y/rect.Height());
				break;
			case MK_MOUSE_MOVE:
				m_tTrackDrawOffset = (point - m_tDownPoint)+m_tOldTrackDrawOffset;
				m_Track.SetOffset(m_tTrackDrawOffset.x,m_tTrackDrawOffset.y);
				break;
			default:
				m_tSelRect.right = point.x;
				m_tSelRect.bottom = point.y;	
				NULL;
			}		
		}
	}

	m_tCurPoint = point;
	m_ImgPoint = GetImgDot(point);

	point = oldPt;	
	//Invalidate();

	CStatic::OnMouseMove(nFlags, point);
}

void CImgStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
	m_dOldCmdScale = m_dCmdScale;

	m_CurSelectLine = GetCurLine(point,m_dCmdScale);
	if(m_CurSelectLine >= 0)
	{
		g_pFrm->m_wndEditBar.SetCmdCurSel(GetCmdLine());
	}

	CPoint oldPt;
	oldPt = point;
	m_bMouseDown = TRUE;
	//point.x = static_cast<int>(m_lXlen - point.x);
	point.x = static_cast<int>(point.x);
	m_tSelRect.left = m_tSelRect.right = point.x;
	m_tSelRect.top = m_tSelRect.bottom = point.y;
	m_tMouseEvent = MK_MOUSE_MOVE;
	m_tDownPoint = point;
	m_tOldTrackDrawOffset = m_tTrackDrawOffset;

	Invalidate();
	SetCapture();
	point = oldPt;
	CStatic::OnLButtonDown(nFlags, point);
}

void CImgStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseDown = FALSE;
	m_tMouseEvent = MK_MOUSE_NONE;
	ReleaseCapture();
	CStatic::OnLButtonUp(nFlags, point);
}

void CImgStatic::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseDown = FALSE;
	m_tMouseEvent = MK_MOUSE_NONE;
	ReleaseCapture();
	CStatic::OnRButtonUp(nFlags, point);
}

void CImgStatic::OnMButtonDown(UINT nFlags, CPoint point)
{
	
	//SetFocus();
	//m_dOldCmdScale = m_dCmdScale;
	//return;
	//// TODO: Add your message handler code here and/or call default
	//tgImgPoint t = GetImgDot(point);
	////t.y = m_iImgHeight - t.y;
	//MoveCenter(CPoint(t.x, t.y));

	CStatic::OnMButtonDown(nFlags, point);
}
 BOOL CImgStatic::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	if(g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_RUN)
		return TRUE;
	
	CRect rect;
	GetClientRect(&rect);//Add by zdx
	m_dCmdScale = m_dOldCmdScale * (1+((float)zDelta*0.5/rect.Height()));
	m_tTrackDrawOffset.x += (int)((m_tCurPoint.x-m_tTrackDrawOffset.x)*(1-m_dCmdScale/m_dOldCmdScale));
	m_tTrackDrawOffset.y += (int)((m_tCurPoint.y+10-m_tTrackDrawOffset.y)*(1-m_dCmdScale/m_dOldCmdScale));
	m_dOldCmdScale = m_dCmdScale;

	m_Track.SetOffset(m_tTrackDrawOffset.x,m_tTrackDrawOffset.y);
	Invalidate();
	return CStatic::OnMouseWheel(nFlags,zDelta,pt);
}

BOOL CImgStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	::SetCursor(::LoadCursor(NULL, IDC_CROSS));
	return TRUE;
	return __super::OnSetCursor(pWnd, nHitTest, message);
}

void CImgStatic::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//����Onpaint֮ǰִ�� ��Ҫ��ȡ�»�ͼ����
	CRect rect;
	GetClientRect(&rect);

	m_tImgRect = GetImgPaintRect(rect);
// 	double dScalex = (double)rect.Width()/(double)IMG_WIDTH;
// 	double dScaley = (double)rect.Height()/(double)IMG_HEIGHT;
// 	double dScale = min(dScalex, dScaley);
// 	DisplayZoom(dScale, dScale);
	
	GetCmdScale();
}

void CImgStatic::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_RUN)
		return ;
	m_bMouseDown = TRUE;
	m_tMouseEvent = MK_MOUSE_ZOOM;
	m_tDownPoint = point;
	m_dOldCmdScale = m_dCmdScale;
	SetCapture();

	__super::OnRButtonDown(nFlags, point);
}

void CImgStatic::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_RUN)
		return ;
	GetCmdScale();
	Invalidate();
	__super::OnLButtonDblClk(nFlags, point);
}

bool CImgStatic::ComPaxis(CPoint m_P1, CPoint m_P2, double m_Sclae, double m_Distance)
{
	
	double m_temp=(m_P1.x-m_P2.x)*(m_P1.x-m_P2.x)+(m_P1.y-m_P2.y)*(m_P1.y-m_P2.y);
	if (m_temp==0)
	{
		return true;
	}
	if (m_temp>=m_Distance*m_Distance/(m_Sclae*m_Sclae))
	{
		return true;
	}
	else
	    return false;
}

int CImgStatic::GetCurLine(CPoint pos,double dCmdScale)
{
	double kpos[2];
	double m_InterDr = 2;//����
	CPoint p1,p2,p3;

	//ȡ�õ�ǰ�켣����
	kpos[0] = (pos.x-m_tTrackDrawOffset.x);
	kpos[1] = (pos.y-m_tTrackDrawOffset.y);
	kpos[0] = kpos[0]/dCmdScale;
	kpos[1] = kpos[1]/dCmdScale;
	m_InterDr = m_InterDr/dCmdScale;

	TRACKINFO tInfo;
	TRACKDATA tk;
	TPOINT	dp[4];
	int iLn = m_Track.GetSize();
	for(int i=0;i<iLn;i++)
	{
		tk = m_Track.GetCurLineData(i);
		switch(tk.iType)
		{
		case T_MARK:
			break;
		case T_WAIT:
		case T_HOME:
		case T_POINT:
			{
				if(fabs(kpos[0] - tk.dPos[0])<m_InterDr &&
					fabs(kpos[1] - tk.dPos[1])<m_InterDr)
				{
					return i;
				}
			}
			break;
		case T_LINE:
			{
				dp[0].x = kpos[0];
				dp[0].y = kpos[1];
				dp[1].x = tk.dPos[0];
				dp[1].y = tk.dPos[1];
				dp[2].x = tk.dPos[2];
				dp[2].y = tk.dPos[3];
				if(m_Track.m_Geo.IsInsLC(dp[0],m_InterDr,dp[1],dp[2]))
					return i;
			}
			break;
		case T_DCIRCLE:
			{
				double angle[6];
				dp[1].x = kpos[0];
				dp[1].y = kpos[1];
				dp[2].x = tk.dPos[4];
				dp[2].y = tk.dPos[5];
				dp[0].x = dp[1].x - dp[2].x;
				dp[0].y = dp[1].y - dp[2].y;
				int co = m_Track.m_Geo.InsCC(dp[1],m_InterDr,dp[2],tk.dPos[6]);
				if(co)
				{
					m_Track.m_Geo.Getagforpoint(dp[0],&angle[1]);//��ǰ��
					dp[0].x = tk.dPos[2] - dp[2].x;
					dp[0].y = tk.dPos[3] - dp[2].y;
					m_Track.m_Geo.Getagforpoint(dp[0],&angle[2]);//DC:Ŀ���
					dp[0].x = tk.dPos[0] - dp[2].x;
					dp[0].y = tk.dPos[1] - dp[2].y;
					m_Track.m_Geo.Getagforpoint(dp[0],&angle[3]);//DC:��ʼ��
					angle[4] = angle[2] - angle[3];
					angle[5] = angle[1] - angle[3];
					while(angle[4] <= 0) angle[4] += 360;
					while(angle[4] > 360) angle[4] -= 360;
					while(angle[5] <= 0) angle[5] += 360;
					while(angle[5] > 360) angle[5] -= 360;
					if(angle[4] < angle[5])
						return i;
				}
			}
			break;
		case T_WCIRCLE:
			{
				//SEOR
				double angle[6];
				dp[1].x = kpos[0];
				dp[1].y = kpos[1];
				dp[2].x = tk.dPos[4];
				dp[2].y = tk.dPos[5];
				dp[0].x = dp[1].x - dp[2].x;
				dp[0].y = dp[1].y - dp[2].y;
				int co = m_Track.m_Geo.InsCC(dp[1],m_InterDr,dp[2],tk.dPos[6]);
				if(co)
				{
					m_Track.m_Geo.Getagforpoint(dp[0],&angle[1]);//��ǰ��
					dp[0].x = tk.dPos[2] - dp[2].x;
					dp[0].y = tk.dPos[3] - dp[2].y;
					m_Track.m_Geo.Getagforpoint(dp[0],&angle[2]);//DC:Ŀ���
					dp[0].x = tk.dPos[0] - dp[2].x;
					dp[0].y = tk.dPos[1] - dp[2].y;
					m_Track.m_Geo.Getagforpoint(dp[0],&angle[3]);//DC:��ʼ��
					angle[4] = angle[2] - angle[3];
					angle[5] = angle[1] - angle[3];
					while(angle[4] <= 0) angle[4] += 360;
					while(angle[4] > 360) angle[4] -= 360;
					while(angle[5] <= 0) angle[5] += 360;
					while(angle[5] > 360) angle[5] -= 360;
					if(angle[4] > angle[5])
						return i;
				}
			}
			break;
		default:
			break;
		}
	}
	return -1;
}

int CImgStatic::GetCmdLine(int iStartIndex, int iEndIndex, double dOffsetX/*=0*/, double dOffsetY/*=0*/)
{
	int LineAdd = 0;
	int i=0, /*j=0,*/ m=0, n=0;
	tgArc tArc;
	TPOINT p1, p2,p3;
	int nx=0, ny=0;
	tgCmdLine pPaintCmd;
	//////////////////////////////////////////////////////////////////////////
	for(i=iStartIndex; i<iEndIndex; i++)
	{
		POSITION posIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(i);
		pPaintCmd = g_pFrm->m_wndEditBar.m_rawList.GetAt(posIndex);

		switch(pPaintCmd.tType)
		{
		case CMD_NONE:
			break;
		case CMD_MARK:
			break;
		case CMD_POINT:
			LineAdd++;
			break;//����㽺,tgPos[1]
		case CMD_LINE_START:
			break;//�߶ε㽺���tgPos[1]
		case CMD_LINE_PASSING:
			if(pPaintCmd.pIntValue[6] && pPaintCmd.pDblValue[6]>1)
			{
				tgCmdLine tPreDotEx,tNextDotEx;
				int m0=-1, m2=-1;
				m0 = g_pFrm->m_wndEditBar.GetPreTrackPoint(i, TRUE);
				m2 = g_pFrm->m_wndEditBar.GetNextTrackPoint(i, TRUE);
				if(m2<0) 
					goto lab;

				POSITION PreposIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(m0);
				tPreDotEx = g_pFrm->m_wndEditBar.m_rawList.GetAt(PreposIndex);

				POSITION NextposIndex = g_pFrm->m_wndEditBar.m_rawList.FindIndex(m2);
				tNextDotEx = g_pFrm->m_wndEditBar.m_rawList.GetAt(NextposIndex);

				double m_CirX=0,m_CirY=0;
				BOOL dir;
				BOOL IsRetOK=g_pFrm->m_wndEditBar.TrackArcHandle(tPreDotEx,pPaintCmd,tNextDotEx,pPaintCmd.pDblValue[6],m_CirX,m_CirY,dir);
				if (IsRetOK==TRUE)
				{
					LineAdd += 2;
					break;
				}
lab:
				LineAdd++;
			}
			else
			{
				LineAdd++;
			}
			break;// �߶ε㽺�м��tgPos[1]
		case CMD_LINE_ARC:
			if(!g_pFrm->m_wndEditBar.GetCmdArcInfo(pPaintCmd, i, &tArc))
				return FALSE;
			LineAdd++;
			break;//Բ���㽺 tgPos[1]
		case CMD_CIRCLE:
			LineAdd++;
			break;//Բ�ε㽺 1:break;Բ��+ֱ��, tgPos[1], double diameter, 2:break;3�㻭Բ,tgPos[3]
		case CMD_LINE_END:
			LineAdd++;
			break;//�߶ε㽺������,tgPos[1]
		case CMD_DISPENSER:
			break;//�㽺����/�� BOOL On/Off
		case CMD_GOTO_ADDRESS:
			break;//��תָ�� int iIndex
		case CMD_REPEATX://X�������� int iIndex:break; int stepType(S/N):break; int iXNum:break; int iYNum:break;  double xSpace:break; double ySpace
			for(m=0; m<pPaintCmd.pIntValue[3]; m++)//y
			{
				for(n=0; n<pPaintCmd.pIntValue[2]; n++)//x
				{
					if(m==0 && n==0)continue;
					if(pPaintCmd.pIntValue[1] ==0)//S
					{
						if((m%2) == 0) nx = n;
						else nx = pPaintCmd.pIntValue[2] -1 - n;
					}
					else
					{
						nx = n;
					}
					ny = m;

					GetCmdLine(pPaintCmd.pIntValue[0], i,pPaintCmd.pDblValue[0]*nx+dOffsetX, pPaintCmd.pDblValue[1]*ny+dOffsetY);
				}
			}
			break;
		case CMD_REPEATY://Y�������� int iIndex:break; int stepType(S/N):break; int iXNum:break; int iYNum:break;  double xSpace:break; double ySpace
			for(m=0; m<pPaintCmd.pIntValue[2]; m++)//x
			{
				for(n=0; n<pPaintCmd.pIntValue[3]; n++)//y
				{
					if(m==0 && n==0)continue;
					nx = m;
					if(pPaintCmd.pIntValue[1] ==0)//S
					{
						if((m%2) == 0) ny = n;
						else ny = pPaintCmd.pIntValue[3] -1 - n;
					}
					else
					{
						ny = n;
					}
					GetCmdLine( pPaintCmd.pIntValue[0], i,pPaintCmd.pDblValue[0]*nx+dOffsetX, pPaintCmd.pDblValue[1]*ny+dOffsetY);
				}
			}
			break;
		case CMD_BRUSH_AREA:			
			if(pPaintCmd.pIntValue[5] == 0)//����
			{
				vector <tgBrushEx> brushRunList;
				tgBrushEx brushLine;				
				tgBrush tArea;
				bool bSet = FALSE;

				tArea.nDir = pPaintCmd.pIntValue[4]==0?0:1;//0--X����Ϳ��  1--Y����Ϳ��

				if(g_pFrm->m_wndEditBar.GetCmdBrushInfo(pPaintCmd,&tArea)==FALSE) break;

				brushLine.x=pPaintCmd.pDblValue[0];
				brushLine.y=pPaintCmd.pDblValue[1];
				brushLine.tType=CMD_LINE_START;
				brushRunList.push_back(brushLine);

				for (int i=0;i<(2*tArea.nNum+1);i++)
				{//����Բ�����ǹ���
					////step1
					brushLine.x = pPaintCmd.pDblValue[0] + (tArea.nDir==0?0:1)*((i/2)%2==0?1:0)*tArea.dx + (tArea.nDir==0?1:0)*((i/2)*tArea.dx +((i%2)==0?0:1)*tArea.dx);
					brushLine.y = pPaintCmd.pDblValue[1] + (tArea.nDir==0?1:0)*((i/2)%2==0?1:0)*tArea.dy + (tArea.nDir==0?0:1)*((i/2)*tArea.dy +((i%2)==0?0:1)*tArea.dy);

					bSet = (i%2)==0?TRUE:FALSE;//ż����λ�û��ж��Ƿ�ֹͣѹ��ָ��
					if(bSet==TRUE)
					{
						if (tArea.nDir==0)
						{//tArea.nDir==0 X����Ϳ��						
							if(fabs(pPaintCmd.pDblValue[4] - brushLine.x)<fabs(tArea.dx))
							{
								brushLine.tType=CMD_LINE_END;
								brushRunList.push_back(brushLine);
								break;
							}
						}
						else
						{//tArea.nDir==1 Y����Ϳ��
							if(fabs(pPaintCmd.pDblValue[5] - brushLine.y)<fabs(tArea.dy))
							{
								brushLine.tType=CMD_LINE_END;
								brushRunList.push_back(brushLine);
								break;
							}
						}
					}
					brushLine.tType=CMD_LINE_PASSING;
					brushRunList.push_back(brushLine);
				}
				////////////////////////////////////////////////////////////////////////
				for(UINT i=0;i<brushRunList.size();i++)
				{
					if(brushRunList[i].tType==CMD_LINE_START)
					{

					}
					else if(brushRunList[i].tType==CMD_LINE_PASSING)
					{	
						tgBrushEx brush[3];
						brush[0]=brushRunList[i-1];
						brush[1]=brushRunList[i];
						brush[2]=brushRunList[i+1];

						tgCmdLine tPreDotEx ,tCurDoxEx,tNextDotEx;
						tPreDotEx.pDblValue[0] = brush[0].x;
						tPreDotEx.pDblValue[1] = brush[0].y;
						tCurDoxEx.pDblValue[0] = brush[1].x;
						tCurDoxEx.pDblValue[1] = brush[1].y;
						tNextDotEx.pDblValue[0] = brush[2].x;
						tNextDotEx.pDblValue[1] = brush[2].y;

						if(pPaintCmd.pIntValue[6]==2)//����Բ��
						{
							double m_CirX=0,m_CirY=0;
							BOOL dir=FALSE;
							BOOL  bRet=g_pFrm->m_wndEditBar.TrackArcHandle(tPreDotEx,tCurDoxEx,tNextDotEx,pPaintCmd.pDblValue[9],m_CirX,m_CirY,dir);
							if (bRet==TRUE)
							{
								LineAdd += 2;
							}
							else
							{
								LineAdd++;
							}
						}						
						else
						{
							LineAdd++;
						}
					}
					else if(brushRunList[i].tType==CMD_LINE_END)
					{
						LineAdd++;
					}
				}
			}
			else//Բ��
			{
				tgBrush tArea;

				if(g_pFrm->m_wndEditBar.GetCmdBrushInfo(pPaintCmd,&tArea)==FALSE) break;

				cux=tArea.x;
				cuy=tArea.y;
				double dRadius=tArea.dRadius;

				//////////////////////////////////////////////////////////////////////////
				double tx = 0,ty = 0;
				while(tArea.dAngle <0)  tArea.dAngle+=360;
				CString tmpstr;
				while(dRadius>tArea.dStep)
				{
					tArea.dAngle += 9;//�ֳ�40�ȷ�
					if(tArea.dAngle>=360) tArea.dAngle -= 360;

					if(dRadius>(tArea.dStep/40))
						dRadius = dRadius - (tArea.dStep/40);

					tArea.dfCx[1] = dRadius * cos(tArea.dAngle*M_PI/180) + tArea.dfCx[0];//��һԲ����
					tArea.dfCy[1] = dRadius * sin(tArea.dAngle*M_PI/180) + tArea.dfCy[0];//��һԲ����
					TPOINT sp,ep,tp1,tp2;
					TPOINT tmp1,tmp2;
					CPoint dop,dsp,dep;
					sp.x = cux;
					sp.y = cuy;
					ep.x = tArea.dfCx[1];
					ep.y = tArea.dfCy[1];
					if(1 == m_Track.m_Geo.InsCC(sp,dRadius,ep,dRadius))
					{
						m_Track.m_Geo.Intersection_circle_circle(sp,dRadius,ep,dRadius,tp1,tp2);
						dsp = GetPoint(sp.x, sp.y, m_dCmdScale, dOffsetX, dOffsetY, TRUE);
						dep = GetPoint(ep.x, ep.y, m_dCmdScale, dOffsetX, dOffsetY, TRUE);
						if(m_Track.m_Geo.GetDrlinedot(sp,ep,tp1) == 2)
						{
							dop = GetPoint(tp1.x, tp1.y, m_dCmdScale, dOffsetX, dOffsetY, TRUE);	
							LineAdd++;
						}
						else if(m_Track.m_Geo.GetDrlinedot(sp,ep,tp2) == 2)
						{
							dop = GetPoint(tp2.x, tp2.y, m_dCmdScale, dOffsetX, dOffsetY, TRUE);
							LineAdd++;
						}
						else
						{
							ASSERT(0);
						}
					}
					tx = tArea.dfCx[1];
					ty = tArea.dfCy[1];
					//if(j < tArea.nNum)
					{
						cux=tArea.dfCx[1];//ep.x;
						cuy=tArea.dfCy[1];//ep.y;
					}
				}
				LineAdd++;
			}
			break;//Ϳ��
		case CMD_END_PROGRAM:
			return FALSE;
			break;//�����������
		default:
			NULL;
		}
		if(LineAdd > m_CurSelectLine)
			return i;
	}
	return TRUE;
}

int CImgStatic::GetCmdLine()
{
	int iCount = g_pFrm->m_wndEditBar.m_rawList.GetCount();
	return GetCmdLine(0,iCount);
}

void CImgStatic::DrawCurSelect(CDC *pDC,int iln)
{
	int iTemp;
	double	dPt[7];
	CPoint p1,p2,p3;
	TRACKDATA tk = m_Track.GetCurLineData(iln);

	CPen pen2(PS_SOLID, 2, RGB(255, 0, 0));
	CPen *oldPen = pDC->SelectObject(&pen2);
	for(int i=0;i<7;i++)
	{
		dPt[i] = tk.dPos[i];
	}
	switch(tk.iType)
	{
	case T_POINT:
		{
			p1 = GetPoint(dPt[0], dPt[1], m_dCmdScale, 0, 0, TRUE);
			DrawCircleEx(pDC,p1,3);
		}
		break;
	case T_LINE:
		{
			p1 = GetPoint(dPt[0], dPt[1], m_dCmdScale, 0, 0, TRUE);
			p2 = GetPoint(dPt[2], dPt[3], m_dCmdScale, 0, 0, TRUE);
			pDC->MoveTo(p1);
			pDC->LineTo(p2);
		}
		break;
	case T_DCIRCLE:
		{
			p1 = GetPoint(dPt[0], dPt[1], m_dCmdScale, 0, 0, TRUE);
			p2 = GetPoint(dPt[2], dPt[3], m_dCmdScale, 0, 0, TRUE);
			p3 = GetPoint(dPt[4], dPt[5], m_dCmdScale, 0, 0, TRUE);
			iTemp = static_cast<int>(dPt[6]* m_dCmdScale);
			pDC->MoveTo(p1);
			pDC->Arc(CRect(p3.x - iTemp,p3.y - iTemp,p3.x + iTemp,p3.y + iTemp),p1,p2);
		}
		break;
	case T_WCIRCLE:
		{
			p1 = GetPoint(dPt[0], dPt[1], m_dCmdScale, 0, 0, TRUE);
			p2 = GetPoint(dPt[2], dPt[3], m_dCmdScale, 0, 0, TRUE);
			p3 = GetPoint(dPt[4], dPt[5], m_dCmdScale, 0, 0, TRUE);
			iTemp = static_cast<int>(dPt[6]* m_dCmdScale);
			pDC->MoveTo(p2);
			pDC->Arc(CRect(p3.x - iTemp,p3.y - iTemp,p3.x + iTemp,p3.y + iTemp),p2,p1);
		}
		break;
	default:
		break;
	}
	pDC->SelectObject(oldPen);
	pen2.DeleteObject();
}


LRESULT CImgStatic::OnMessageRedrawCCDEvent(WPARAM wparam, LPARAM lparam)
{
	Invalidate();
	//OnPaint();
	return 0;
}