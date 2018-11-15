#pragma once

#include <vfw.h>
#include "KSvdhEx.h"
#include "KSoft.h"
//#include "CarmAVT.h"
#include "CameraHK.h"
#include "RedrawTrack.h"
// CImgStatic
#define  MaxDRAWDot   90000
struct tgImgPoint
{
	tgImgPoint(){x=y=0;}
	tgImgPoint(int ix, int iy){x=ix; y=iy;}
	int x, y;
};
typedef struct tgCFPoint
{
	tgCFPoint(){x=0; y=0;};
	tgCFPoint(double fx, double fy){x=fx; y=fy;}
	double x, y;
}CFPoint;

typedef CRect tgImgRect;

struct tgBrushEx
{
	double x,y;
	CMD_TYPE tType;  //0直线起点，1直线中间点，2圆弧，3 结束点
};

enum MK_MOUSE_EVENT
{
	MK_MOUSE_NONE,
	MK_MOUSE_ZOOM,
	MK_MOUSE_MOVE
};

class CImgStatic : public CStatic,public CKSVDHEx, public CKSoftClass  
{
	DECLARE_DYNAMIC(CImgStatic)
	
public:
	CCameraHK m_camHK;
//	CCarmAVT  m_CarmAVT;  // 相机对象
	CImgStatic();
	~CImgStatic();

	BYTE* m_pBufShow;	// 图像显示使用的Buff
	void CallBack(STRUCT_KSVDH KSVDH);
	void OnPaint(HDC hDC);
	BOOL ImgInit(HWND hWnd, long nUpFocus=15, long nGain=25);
	void ImgLive();
	void ImgStop();
	void ImgFree();
	void DrawCurrentPos();
	void CCDSoftTriggerOnce();
	//////////////////////////////////////////////////////////////////////////
	void ReverseY(BYTE* pBuf, BYTE* pDestBuf, const int width, const int height);
	void PaintShape(CDC *pDC, CRect rect);
	void PaintImage(CDC *pDC, CRect rect);
	void PaintTrack(CDC *pDC, CRect rect);
	void PaintTrack(CDC *pDC, CRect rect, double dCmdScale, int iStartIndex, int iEndIndex, double dOffsetX=0/*mm*/, double dOffsetY=0/*mm*/, BOOL bStepCmd=FALSE);

	void DrawCross(CDC *pDC, CPoint point, int rw, int rh);
	void DrawArc(CDC *pDC, CRect rect, CPoint tStartPoint, CPoint tEndPoint);
	void DrawCircle(CDC *pDC, CPoint point, int r, BOOL bFull=TRUE);
	void DrawEllipse(CDC *pDC, CPoint point, int r, BOOL bFull=TRUE);
	void DrawCircleEx(CDC *pDC, CPoint point, int r, BOOL bFull=TRUE);
	
	void GetCmdScale();
	double 	m_dCmdScale, m_dOldCmdScale;
	CPoint m_tTrackDrawOffset, m_tDownPoint,m_tOldTrackDrawOffset;
	MK_MOUSE_EVENT m_tMouseEvent;

	//////////////////////////////////////////////////////////////////////////
	CRect GetImgPaintRect(CRect rect);

	tgImgPoint GetImgDot(CPoint point);
	tgImgRect GetImgRect(CRect rect);
	CPoint GetPoint(tgImgPoint tPoint);
	CPoint GetPoint(double dX, double dY, double dScale, double dOffsetX, double dOffsetY, BOOL bTrack=FALSE);

	void LoadImageFromFile(CString strPath);
	void CopyImage();

	//////////////////////////////////////////////////////////////////////////
	BOOL PatAlloc(char *szFile=NULL);
	BOOL PatLoad(char *szFile);
	void PatSave(char *szFile);
	int PatFind(double dAcceptance,double &dx, double &dy);
	void PatSetParam(double dAcceptance);
	double PatGetParam();
	void PatShow(HWND hWnd);

	//void MoveCenter(CPoint point);
	//void MoveCenter(CFPoint point);
	void PatCenter(double &dx, double &dy);
	//Select Line
	CRedrawTrack m_Track;
	int         GetCurLine(CPoint pos,double dCmdScale);
	int         GetCmdLine();
	int         GetCmdLine(int iStartIndex, int iEndIndex, double dOffsetX=0, double dOffsetY=0);		//通过轨迹段计算命令行
	void		DrawCurSelect(CDC *pDC,int iln);

	int         m_CurSelectLine;
	//////////////////////////////////////////////////////////////////////////
	CRect m_tSelRect, m_tImgRect;
	BOOL m_bMouseDown;
	HDRAWDIB m_hDrawDib;
	double m_dScale;
	double m_lXlen;

	tgImgPoint m_ImgPoint, m_tPatPos;
	double m_dPatAcceptance;//实际查找到模板的分数

	CPoint m_tCurPoint;
    CPoint m_Draw_CurP;
	CKSoftPat m_Pat;
	int m_iImgWidth, m_iImgHeight;
	BOOL m_bImgInit;

	CFPoint m_tPatFindPos;


	//for Pat
	KS_ID m_PatBuffer;
	KS_ID m_PatDisplay;
	KS_ID m_BufRGB;
	//////////////////////////////////////////////////////////////////////////
	tgCmdParam	m_cmdParam;
	double cux;
	double cuy;

	//bool		m_frpos;
	//int			m_wOrH;//1:Width 2:Height
	//int			m_iStLn,m_iEndLn;
	//double		m_dZoomOffsetX,m_dZoomOffsetY;
	TRECT		m_TrackRect;

	//void SetDrawCmdLine(int ist,int iend);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
     CPoint m_DrawS[MaxDRAWDot];
     CPoint m_DrawE[MaxDRAWDot];
     
	bool ComPaxis(CPoint m_P1, CPoint m_P2, double m_Sclae, double m_Distance);

	int m_nDrawTarkCount;

	// 图像重绘
	LRESULT OnMessageRedrawCCDEvent(WPARAM wparam, LPARAM lparam);
};


