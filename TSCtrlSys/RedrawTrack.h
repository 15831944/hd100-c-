/************************************************************************/
/* 轨迹重绘的类															*/
/*	Ver 1.2          --120808											*/
/************************************************************************/
#pragma once
//#include "MathEx.h"
#include "paramdef.h"

enum T_Type
{
	T_POINT = 0,	
	T_LINE,	
	T_DCIRCLE,	//顺逆方向以实际轨迹数据为准，非绘图方向
	T_WCIRCLE,	
	T_MARK,		
	T_HOME,		
	T_WAIT		
};
// #define  T_POINT			0
// #define  T_LINE			1
// #define  T_DCIRCLE		2		//顺逆方向以实际轨迹数据为准，非绘图方向
// #define  T_WCIRCLE		3
// #define  T_MARK			4
// #define  T_HOME			5
// #define  T_WAIT			6

struct TRACKDATA 
{
	TRACKDATA()
	{
		iType = -1;
		iIndex = 0;
		bIsDraw = true;
		memset(dPos,0,sizeof(float)*8);
	}
	int iType;
	int iIndex;
	bool bIsDraw;
	double dPos[8];	//point:p line:p1,p2 arc:s,e,o,r
};

struct TRACKINFO 
{
	TRACKINFO()
	{
		line = 0;
		p.x = 0;
		p.y = 0;
		bIsDraw = true;
	}
	int line;
	bool bIsDraw;
	TPOINT p;
};

struct COLOR 
{
	int R;
	int G;
	int B;
	COLOR()
	{
		R = 128;
		G = 128;
		B = 128;
	}
	COLOR(int r,int g,int b)
	{
		R = r;
		G = g;
		B = b;
	}
};

struct ArcLineData 
{
	int line;
	double angle;
};

class CRedrawTrack
{
public:
	CRedrawTrack(void);
	~CRedrawTrack(void);
public:
	/* TrackData */
	bool		LoadTrack();																//加载轨迹命令
	bool		LoadTrack(int iStartIndex, int iEndIndex, double dScale=1,
					double dOffsetX=0, double dOffsetY=0);									
	bool		FreeTrackData();															//释放轨迹数据

	/* Thread Ctrl */
	bool		Start();																	//开始重绘计算
	bool		Pause();																	//暂停
	bool		Restart();																	//继续
	bool		Stop();																		//终止

	/* Setting */
	void		CollPos(double *px,double *py);												//绑定坐标变量
	void		CollValve(bool *bFlag);														//绑定阀开关  0:关阀 1:开阀
	void		SetValve(bool bFlag);														//设置阀状态
	void		SetOffset(int dx,int dy);													//设置轨迹显示偏移量

	/* Draw */
	void		Paint(CDC *pDC, CRect rect, BOOL bFront,
					double dOffsetX=0, double dOffsetY=0);									//实际绘图

	/* Get Status*/
	bool		GetValveSt();
	int			GetSize();

	/* Get Data */
	TRACKDATA   GetCurLineData(int n);

	/* Class&Structure Define */
	CGeometry	m_Geo;
	tgCmdParam	m_cmdParam;

protected:
	static UINT	TrackThread(LPVOID pParam);
	CPoint		GetPoint(double dX, double dY, double dScale, double dOffsetX, double dOffsetY, bool bTrack=FALSE);
	TPOINT		GetPoint(int pType, double dX, double dY, double dScale, double dOffsetX, double dOffsetY);
	bool		GetCircleInfo(TPOINT op,double r,TPOINT *sp,bool *dir);
	int			P3ToSEO(TPOINT *p,int iNum);
	TRACKINFO	GetTrackInfo(TRACKINFO info);
	void		DrawCircleEx(CDC *pDC,COLOR clr, CPoint point, int r, bool bFull = TRUE);
	void		DrawCross(CDC *pDC,COLOR clr, CPoint point, int r);
private:
	CArray		<TRACKDATA, TRACKDATA&> m_FrontData;
	double		m_dFLeft,m_dFRight,m_dFTop,m_dFBottom;
	double		m_dFScale;
	CArray		<TRACKDATA, TRACKDATA&> m_BackData;
	double		m_dBLeft,m_dBRight,m_dBTop,m_dBBottom;
	double		m_dBScale;

	double		cux,cuy;				
	TRACKINFO	*m_OldInfo;
	int			m_OldLine;
	int			m_iDrawFlag;			//当前轨迹绘制状态 0:Stop 1:Draw 2:Pause
	double		*m_dCurX,*m_dCurY;		//当前XY坐标
	bool		m_bValveOut;			//胶阀开关状态 0:关阀 1:开阀
	double		m_InterDr;
	TPOINT		m_dCurLinePos;			//Cur Line Pos
	CPoint		m_tTrackDrawOffset;
	ArcLineData	m_AData;
	CWinThread  *m_Thread;
	bool		m_bexit;
	bool		m_bCurLineSt;			//当前行是否开始画
	int			m_iCurLine;
	CEvent		g_exitdrawevent;
	bool		m_bDataUpdate;
};