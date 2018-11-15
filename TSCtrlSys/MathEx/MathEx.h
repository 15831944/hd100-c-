
#ifndef _MATHEX_H_
#define _MATHEX_H_
#ifdef MATHEX_EXPORTS
#define MATHEX_API __declspec(dllexport)
#else
#define MATHEX_API __declspec(dllimport)
#endif

#define  EPS	0.000001
#define  PI		3.14159265358979323846

struct MATHEX_API TPOINT
{
public:
	TPOINT()
	{
		x = 0;
		y = 0;
		z = 0;
		a = 0;
	}

	double x;
	double y;
	double z;
	double a;
};

struct MATHEX_API TRECT
{
	double top;
	double bottom;
	double left;
	double right;
	TRECT()
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	TRECT(double l,double t,double r,double b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
	double Width()
	{
		return right>left?(right-left):(left-right);
	}
	double Height()
	{
		return top>bottom?(top-bottom):(bottom-top);
	}
};

struct MATHEX_API TLINE
{
	TPOINT a;
	TPOINT b;
};

class MATHEX_API CGeometry
{
public:
	CGeometry();
	~CGeometry();

public:
	TRECT		GetCircleRect(TPOINT sp,TPOINT ep,TPOINT op);													//计算圆的区域
	TRECT		GetArcRect(TPOINT sp,TPOINT ep,TPOINT op,bool bDr);												//计算圆弧的区域
	TRECT		GetLineRect(TPOINT sp,TPOINT ep);																//计算线段的区域
	TRECT		GetMaxLineRect(int quadrant,TRECT rect,double r);												//计算增长后的线段区域
	double		ReversalAngle(double angle);
	TPOINT		Intersection(TLINE u,TLINE v);
	TPOINT		Intersection(TPOINT u1,TPOINT u2,TPOINT v1,TPOINT v2);
	int			Getquforpoint(TPOINT p);																		//
	double		Xmult(TPOINT p1,TPOINT p2,TPOINT p0);
	double		Distance(TPOINT p1,TPOINT p2);
	int			GetStepQuadrant(TPOINT sp,TPOINT ep,TPOINT op);
	double		Distance(double sx,double sy,double ex,double ey);												//两点间距离
	double		ArcLength(double sx,double sy,double ex,double ey,double ox,double oy,bool bDr);				//bDr 方向 0.顺时针 1.逆时针
	double		Disptoline(TPOINT p,TPOINT l1,TPOINT l2);														//点到直线的距离
	bool		IsLittleArc(TPOINT sp,TPOINT ep,TPOINT op,bool dDr);											//是否为劣弧
	int			P3ToSEO(TPOINT *p,int iNum);																	//三点转起始点、终止点、圆心点
	int			IsInsLC(TPOINT c,double r, TPOINT l1,TPOINT l2);												//线段与圆是否相交
	int			IsLineInsCircle(TPOINT c,double r, TPOINT l1,TPOINT l2);										//直线与圆是否相交
	int			InsCC(TPOINT c1,double r1,TPOINT c2,double r2);													//两圆是否相交
	void		InsLC(TPOINT c,double r,TPOINT l1,TPOINT l2,TPOINT& p1,TPOINT& p2);								//求线段与圆的交点
	bool		Getagforpoint(TPOINT p,double *angle);															//算出点p的向量与正X的夹角
	void		Intersection_circle_circle(TPOINT c1,double r1,TPOINT c2,double r2,TPOINT& p1,TPOINT& p2);		//求两圆交点
	int			GetDrlinedot(TPOINT sp,TPOINT ep,TPOINT tp);													//计算点tp在线段位置,0.线段上  1.左侧  2.右侧
	void		RotatePoint(double *x,double *y,double angle);													//以0点为圆心旋转一个点
	TPOINT		SymmetryPoint(TPOINT tp,TPOINT sp,TPOINT ep);													//求关于直线点tp的对称点
	short		CaclCirclePara(double x1,double y1,double x2,double y2,double x3,double y3,double * cx,double * cy,double * r);
};

class MATHEX_API CMathEx
{
public:
	CMathEx(void);
	~CMathEx();
public:
	double		Sqrt(double dval);
};

#endif