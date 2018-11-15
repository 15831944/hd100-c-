
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
	TRECT		GetCircleRect(TPOINT sp,TPOINT ep,TPOINT op);													//����Բ������
	TRECT		GetArcRect(TPOINT sp,TPOINT ep,TPOINT op,bool bDr);												//����Բ��������
	TRECT		GetLineRect(TPOINT sp,TPOINT ep);																//�����߶ε�����
	TRECT		GetMaxLineRect(int quadrant,TRECT rect,double r);												//������������߶�����
	double		ReversalAngle(double angle);
	TPOINT		Intersection(TLINE u,TLINE v);
	TPOINT		Intersection(TPOINT u1,TPOINT u2,TPOINT v1,TPOINT v2);
	int			Getquforpoint(TPOINT p);																		//
	double		Xmult(TPOINT p1,TPOINT p2,TPOINT p0);
	double		Distance(TPOINT p1,TPOINT p2);
	int			GetStepQuadrant(TPOINT sp,TPOINT ep,TPOINT op);
	double		Distance(double sx,double sy,double ex,double ey);												//��������
	double		ArcLength(double sx,double sy,double ex,double ey,double ox,double oy,bool bDr);				//bDr ���� 0.˳ʱ�� 1.��ʱ��
	double		Disptoline(TPOINT p,TPOINT l1,TPOINT l2);														//�㵽ֱ�ߵľ���
	bool		IsLittleArc(TPOINT sp,TPOINT ep,TPOINT op,bool dDr);											//�Ƿ�Ϊ�ӻ�
	int			P3ToSEO(TPOINT *p,int iNum);																	//����ת��ʼ�㡢��ֹ�㡢Բ�ĵ�
	int			IsInsLC(TPOINT c,double r, TPOINT l1,TPOINT l2);												//�߶���Բ�Ƿ��ཻ
	int			IsLineInsCircle(TPOINT c,double r, TPOINT l1,TPOINT l2);										//ֱ����Բ�Ƿ��ཻ
	int			InsCC(TPOINT c1,double r1,TPOINT c2,double r2);													//��Բ�Ƿ��ཻ
	void		InsLC(TPOINT c,double r,TPOINT l1,TPOINT l2,TPOINT& p1,TPOINT& p2);								//���߶���Բ�Ľ���
	bool		Getagforpoint(TPOINT p,double *angle);															//�����p����������X�ļн�
	void		Intersection_circle_circle(TPOINT c1,double r1,TPOINT c2,double r2,TPOINT& p1,TPOINT& p2);		//����Բ����
	int			GetDrlinedot(TPOINT sp,TPOINT ep,TPOINT tp);													//�����tp���߶�λ��,0.�߶���  1.���  2.�Ҳ�
	void		RotatePoint(double *x,double *y,double angle);													//��0��ΪԲ����תһ����
	TPOINT		SymmetryPoint(TPOINT tp,TPOINT sp,TPOINT ep);													//�����ֱ�ߵ�tp�ĶԳƵ�
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