/************************************************************************/
/* �켣�ػ����															*/
/*	Ver 1.2          --120808											*/
/************************************************************************/
#pragma once
//#include "MathEx.h"
#include "paramdef.h"

enum T_Type
{
	T_POINT = 0,	
	T_LINE,	
	T_DCIRCLE,	//˳�淽����ʵ�ʹ켣����Ϊ׼���ǻ�ͼ����
	T_WCIRCLE,	
	T_MARK,		
	T_HOME,		
	T_WAIT		
};
// #define  T_POINT			0
// #define  T_LINE			1
// #define  T_DCIRCLE		2		//˳�淽����ʵ�ʹ켣����Ϊ׼���ǻ�ͼ����
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
	bool		LoadTrack();																//���ع켣����
	bool		LoadTrack(int iStartIndex, int iEndIndex, double dScale=1,
					double dOffsetX=0, double dOffsetY=0);									
	bool		FreeTrackData();															//�ͷŹ켣����

	/* Thread Ctrl */
	bool		Start();																	//��ʼ�ػ����
	bool		Pause();																	//��ͣ
	bool		Restart();																	//����
	bool		Stop();																		//��ֹ

	/* Setting */
	void		CollPos(double *px,double *py);												//���������
	void		CollValve(bool *bFlag);														//�󶨷�����  0:�ط� 1:����
	void		SetValve(bool bFlag);														//���÷�״̬
	void		SetOffset(int dx,int dy);													//���ù켣��ʾƫ����

	/* Draw */
	void		Paint(CDC *pDC, CRect rect, BOOL bFront,
					double dOffsetX=0, double dOffsetY=0);									//ʵ�ʻ�ͼ

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
	int			m_iDrawFlag;			//��ǰ�켣����״̬ 0:Stop 1:Draw 2:Pause
	double		*m_dCurX,*m_dCurY;		//��ǰXY����
	bool		m_bValveOut;			//��������״̬ 0:�ط� 1:����
	double		m_InterDr;
	TPOINT		m_dCurLinePos;			//Cur Line Pos
	CPoint		m_tTrackDrawOffset;
	ArcLineData	m_AData;
	CWinThread  *m_Thread;
	bool		m_bexit;
	bool		m_bCurLineSt;			//��ǰ���Ƿ�ʼ��
	int			m_iCurLine;
	CEvent		g_exitdrawevent;
	bool		m_bDataUpdate;
};