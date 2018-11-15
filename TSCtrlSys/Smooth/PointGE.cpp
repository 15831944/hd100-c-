//###########################################################################
//
// FILE:	PointGE.cpp
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 18 Jan 2011 |Joshua| Prilimilary
//      |             |      | 
//      |             |      |
//      |             |      | 
//###########################################################################
#include "stdafx.h"
#include "PointGE.h"
#define _USE_MATH_DEFINES
#include <math.h>

CPointGE::CPointGE()
{
}

CPointGE::~CPointGE()
{

}

Bool CPointGE::LineEquation(CMyPoint pt1,CMyPoint pt2,Float& A,Float& B,Float& C)
{//求直线方程 算出A B C
	A = 1.0;
	B = 1.0;
	C = 1.0;
	CMyPoint pt; 

	Float F = pt1.m_fX * pt2.m_fY - pt1.m_fY * pt2.m_fX;
	
	pt= pt2;
	pt-= pt1;
	if(fabs(F) < EPS)
	{

		if((fabs(pt.m_fX) < EPS)&&(fabs(pt.m_fY) < EPS))
		{
			return FALSE;
		}
		else if(fabs(pt.m_fX) < EPS)
		{
			A = 1.0;
			B = 0.0;
			C = 0.0 - pt1.m_fX;
		}
		else if(fabs(pt.m_fY) < EPS)
		{
			A = 0.0;
			B = 1.0;
			C = 0.0 - pt1.m_fY;	
		}
		else
		{
			A = 1.0;
			B = 0.0 - pt1.m_fX/pt1.m_fY;
			C = 0.0;
		}
	}
	else
	{
		A = 0.0 - pt.m_fY/F;
		B = pt.m_fX/F;
		C = 1.0;
	}
	return TRUE;
}

Bool CPointGE::LineEquation(CMyPoint pt,Float A,Float B,Float& C)
{	////求经过pt点的平行线 A B同 求C
	C = 0.0 - A * pt.m_fX - B * pt.m_fY; 
	return TRUE;
}

Bool CPointGE::LineExtend(CMyPoint& pt,Float fLength,CMyPoint pt1,CMyPoint pt2)
{//延长fLength，pt终点
	pt = pt2;
	pt -= pt1;
	Float fLen = pt.GetLength();

	if(fabs(fLen) < EPS)
	{
		return FALSE;
	}

	pt /= fLen;		//
	pt *= fLength;
	pt += pt2;

	return TRUE;
}

Bool CPointGE::LineExtend(CMyPoint& pt,Float fLength,Float fAngle,CMyPoint pt1,CMyPoint pt2)
{//pt1 pt2 直线旋转一个角度并延长到fLength pt终点 绕pt2旋转 fAngle 逆时针为正 顺时针为负  
	pt = pt2;
	pt -= pt1;
	Float fLen = pt.GetLength();

	if(fabs(fLen) < EPS)
	{
		return FALSE;
	}

	Float fA = atan2(pt.m_fY,pt.m_fX);
	fA += fAngle;
	pt.m_fX = fLength * cos(fA);
	pt.m_fY = fLength * sin(fA);

	pt += pt2;

	return TRUE;
}

Bool CPointGE::ArcExtend(Float fLength,Bool bCW,CMyPoint ptCen,CMyPoint ptEnd,CMyPoint& pt)
{//圆弧算收胶点或者结束中间点
	CMyPoint ptTemp = ptEnd;
	ptTemp -= ptCen;
	ptTemp.m_fZ = 0.0;
	Float fR = ptTemp.GetLength();
	Float fA = atan2(ptTemp.m_fY,ptTemp.m_fX);
	Float fDA = fLength/fR;
	if(TRUE == bCW)
	{
		fDA *= -1.0;
	}
	fA += fDA;
	pt.m_fX = fR * cos(fA);
	pt.m_fY = fR * sin(fA);
	pt += ptCen;

	return TRUE;
}


Bool CPointGE::LineLineCross(Float A1,Float B1,Float C1,Float A2,Float B2,Float C2,CMyPoint& ptCross)
{
	Float F = A1 * B2 - A2 * B1;
	if(fabs(F) < EPS)
	{
		return FALSE;
	}

	ptCross.m_fX = (B1 * C2 - B2 * C1)/F;
	ptCross.m_fY = 0.0 - (A1 * C2 - A2 * C1)/F;
	return TRUE;
}

Bool CPointGE::LineArcCross(Float A,Float B,Float C,Float a,Float b,Float r,CMyPoint& pt1,CMyPoint& pt2)
{//a b 圆心 r为半径 pt1 pt2 为两点坐标
	Float F = 0.0;
	if(fabs(B) < EPS)
	{
		if(fabs(A) < EPS)
		{
			return FALSE;
		}
		pt1.m_fX = 0.0 - C/A;
		pt2.m_fX = pt1.m_fX;

		F = sqrt(r * r - (pt1.m_fX - a) * (pt1.m_fX - a));
		pt1.m_fY = b + F; 
		pt2.m_fY = b - F; 
		
		return TRUE;
	}

	Float M = (A * A + B * B) / (B * B);
	Float N = 2 * (A * b * B + A * C - a * B * B)/(B * B);
	Float P =  (C * C + 2 * b * B * C)/(B * B) + a * a + b * b - r * r;

	F = N * N - 4 * M * P;
	if(F < 0)
	{
		return FALSE;
	}

	F = sqrt(F);
	pt1.m_fX = 0.5 * (0.0 - N + F)/M;
	pt2.m_fX = 0.5 * (0.0 - N - F)/M;

	pt1.m_fY = 0.0 - (A * pt1.m_fX + C)/B; 
	pt2.m_fY = 0.0 - (A * pt2.m_fX + C)/B; 
	return TRUE;
}

Bool CPointGE::Pt3Circle(CMyPoint pt1,CMyPoint pt2,CMyPoint pt3,Float& R,CMyPoint& ptCen,Bool& bCW)
{
	Float A[4];
	Float B[4];
	Float C[4];

	CMyPoint pt[3];

	// (1) 
	LineEquation(pt1,pt2,A[0],B[0],C[0]);
	LineEquation(pt2,pt3,A[1],B[1],C[1]);

	if(fabs(B[0]) < EPS)
	{
		if(fabs(B[1]) < EPS)
		{
			return FALSE;
		}
	}
	else
	{
		if(fabs(B[1]) > EPS)
		{
			if(fabs(A[0]/B[0] - A[1]/B[1]) < EPS)
			{
				return FALSE;
			}
		}
	}

	// (2) 
	pt[0] = pt1;
	pt[0] *= 0.5;
	pt[2] = pt2;
	pt[2] *= 0.5;
	pt[0] += pt[2];


	pt[1] = pt2;
	pt[1] *= 0.5;
	pt[2] = pt3;
	pt[2] *= 0.5;
	pt[1] += pt[2];

	// (3)
	A[2] = 0.0 - B[0];
	B[2] = A[0];
	LineEquation(pt[0],A[2],B[2],C[2]); 

	A[3] = 0.0 - B[1];
	B[3] = A[1];
	LineEquation(pt[1],A[3],B[3],C[3]); 

	LineLineCross(A[2],B[2],C[2],A[3],B[3],C[3],ptCen);

	pt[2] = pt1;
	pt[2] -= ptCen;
	pt[2].m_fZ = 0.0;
	R = pt[2].GetLength();

	//
	pt[0] = pt2;
	pt[0] -= pt1;
	pt[1] = pt3;
	pt[1] -= pt2;

	A[0] = atan2(pt[0].m_fY,pt[0].m_fX);
	A[1] = atan2(pt[1].m_fY,pt[1].m_fX);
	A[2] = A[1] - A[0];
	if(A[2] < 0.0 - M_PI)
	{
		A[2] += 2 * M_PI;
	}
	else if(A[2] > M_PI)
	{
		A[2] -= 2 * M_PI;
	}

	bCW = FALSE;
	if(A[2] < 0)
	{
		bCW = TRUE;
	}
	return TRUE;
}


Int CPointGE::Pt3Circle(CMyPoint& pt1,CMyPoint pt2,CMyPoint& pt3,Float R,Float E,CMyPoint& ptCen,Bool& bCW)
{
	Float A[4] = {0.0};
	Float B[4] = {0.0};
	Float C[6] = {0.0};
	

	CMyPoint pt[3];

	// (1) 

	pt[0] = pt2;
	pt[0] -= pt1;
	pt[1] = pt3;
	pt[1] -= pt2;

	A[0] = atan2(pt[0].m_fY,pt[0].m_fX);
	A[1] = atan2(pt[1].m_fY,pt[1].m_fX);
	A[2] = A[1] - A[0];
	if(A[2] < 0.0 - M_PI)
	{
		A[2] += 2 * M_PI;
	}
	else if(A[2] > M_PI)
	{
		A[2] -= 2 * M_PI;
	}
	if(fabs(A[2]) > (2.0 * M_PI / 3.0))
	{
		return 0;	// No smooth
	}
	else if(fabs(A[2]) < (M_PI / 6.0))
	{	
		return 1;	// Smooth directly	
	}

	A[3] = 0.5 * M_PI;
	bCW = FALSE;
	if(A[2] < 0)
	{
		A[3] *= -1.0;
		bCW = TRUE;
	}

	LineEquation(pt1,pt2,A[0],B[0],C[0]);
	LineEquation(pt2,pt3,A[1],B[1],C[1]);
	C[4] = C[0];
	C[5] = C[1];
	LineExtend(pt[0],R,A[3],pt1,pt2);
	LineExtend(pt[1],R,A[3],pt2,pt3);
	LineEquation(pt[0],A[0],B[0],C[0]);
	LineEquation(pt[1],A[1],B[1],C[1]);
	LineLineCross(A[0],B[0],C[0],A[1],B[1],C[1],ptCen);
	
	ptCen.m_fZ = pt2.m_fZ;
	pt[2] = ptCen;
	pt[2] -= pt2;
	if(pt[2].GetLength() - R > E)
	{
		return 0;	// Exceed tolerance, No smooth
	}

	// (3)
	A[2] = B[0];
	B[2] = 0.0 - A[0];
	A[3] = B[1];
	B[3] = 0.0 - A[1];
	LineEquation(ptCen,A[2],B[2],C[2]); 
	LineEquation(ptCen,A[3],B[3],C[3]); 

	LineLineCross(A[2],B[2],C[2],A[0],B[0],C[4],pt1);
	LineLineCross(A[3],B[3],C[3],A[1],B[1],C[5],pt3);
	return 2;	// Smooth indirectly
}

Int CPointGE::Arc2LineCircle(CMyPoint pt1,CMyPoint& pt2,CMyPoint pt3,CMyPoint& pt4,Float R,Float E,CMyPoint& ptCen,Bool& bCW)
{//圆弧转直线 
	Int nValue = 0;
	CMyPoint pt[4];
	pt[0] = pt4;
	pt[1] = pt3;
	pt[2] = pt2;
	pt[3] = pt1;
	nValue = Line2ArcCircle(pt[0],pt[1],pt[2],pt[3],R,E,ptCen,bCW);

	bCW = !bCW;
	switch(nValue)
	{
		case 0:
			break;
		case 3:
			break;
		case 4:
			pt4 = pt[0];
			pt2 = pt[2];
			break;
	}

	return nValue;	// Smooth indirectly
}

Int CPointGE::Arc2LineCircle(CMyPoint& pt1,CMyPoint pt2,CMyPoint& pt3,Float R,Float E,CMyPoint& ptCen,Bool& bCW)
{
	Int nValue = 0;
	CMyPoint pt[3];
	pt[0] = pt3;
	pt[1] = pt2;
	pt[2] = pt1;
	bCW = !bCW;
	nValue = Line2ArcCircle(pt[0],pt[1],pt[2],R,E,ptCen,bCW);

	bCW = !bCW;
	switch(nValue)
	{
		case 0:
			break;
		case 3:
			break;
		case 4:
			pt3 = pt[0];
			pt1 = pt[2];
			break;
	}

	return nValue;	// Smooth indirectly	
}

Int CPointGE::Line2ArcCircle(CMyPoint& pt1,CMyPoint pt2,CMyPoint& pt3,CMyPoint pt4,Float R,Float E,CMyPoint& ptCen,Bool& bCW)
{
	Float A[3] = {0.0,0.0,0.0};
	Float B[2] = {0.0,0.0};
	Float C[3] = {0.0,0.0,0.0};
	CMyPoint pt[4];

	Bool bTemp = TRUE;
	CMyPoint ptTemp;
	Float fR[2] = {0.0,0.0};
	Float fAngle = 0.0;

	E = 0;
	// (1) 
	pt[1] = pt2;
	pt[2] = pt3;
	pt[3] = pt4;
	Pt3Circle(pt[1],pt[2],pt[3],fR[0],ptTemp,bTemp);
	
	// (2)
	pt[0] = pt[1];
	pt[0] -= ptTemp;
	A[0] = atan2(pt[0].m_fY,pt[0].m_fX);
	if(TRUE != bTemp)
	{
		A[0] += 0.5 * M_PI;
	}
	else
	{
		A[0] -= 0.5 * M_PI;
	}

	pt[1] = pt2;
	pt[1] -= pt1;
	A[1] = atan2(pt[1].m_fY,pt[1].m_fX);
	A[2] = A[0] - A[1];
	if(A[2] < 0.0 - M_PI)
	{
		A[2] += 2 * M_PI;
	}
	else if(A[2] > M_PI)
	{
		A[2] -= 2 * M_PI;
	}

	if(fabs(A[2]) > (2.0 * M_PI / 3.0))
	{
		return 0;	// No smooth
	}
	else if(fabs(A[2]) < (M_PI / 6.0))
	{
		bCW = bTemp;
		ptCen = ptTemp;
			
		return 3;	// Smooth directly	
	}
	
	if(A[2] > 0.0)
	{
		bCW = FALSE;
		fAngle = 0.5 * M_PI;
	}
	else
	{	
		bCW = TRUE;
		fAngle = 0.0 - 0.5 * M_PI;		
	}

	// (3) Smooth indirectly
	LineExtend(pt[0],0.0 - R,pt1,pt2);
	pt[0] -= ptTemp;
	pt[0].m_fZ = 0.0;	
	if(pt[0].GetLength() > fR[0])
	{
		fR[1] = fR[0] + R; 
	}
	else
	{ 
		fR[1] = fR[0] - R; 
	}

	// (4)
	LineEquation(pt1,pt2,A[0],B[0],C[0]);
	C[2] = C[0];
	LineExtend(pt[0],R,fAngle,pt1,pt2);	
	LineEquation(pt[0],A[0],B[0],C[0]);
	LineArcCross(A[0],B[0],C[0],ptTemp.m_fX,ptTemp.m_fY,fR[1],pt[2],pt[3]);
	pt[0] = pt[2];
	pt[0] -= pt2;
	pt[0].m_fZ = 0.0;
	pt[1] = pt[3];
	pt[1] -= pt2;
	pt[1].m_fZ = 0.0;
	if(pt[0].GetLength() > pt[1].GetLength())
	{
		ptCen = pt[3];
	}
	else
	{
	    ptCen = pt[2];	
	}

	// (5)
	A[1] = 0.0 - B[0];
	B[1] = A[0];
	LineEquation(ptCen,A[1],B[1],C[1]);
	LineLineCross(A[0],B[0],C[2],A[1],B[1],C[1],pt1);

	// (6) 
	LineEquation(ptTemp,ptCen,A[0],B[0],C[0]);
	LineArcCross(A[0],B[0],C[0],ptTemp.m_fX,ptTemp.m_fY,fR[0],pt[2],pt[3]);
	pt[0] = pt[2];
	pt[0] -= pt2;
	pt[0].m_fZ = 0.0;
	pt[1] = pt[3];
	pt[1] -= pt2;
	pt[1].m_fZ = 0.0;
	if(pt[0].GetLength() > pt[1].GetLength())
	{
		pt3 = pt[3];
	}
	else
	{
	    pt3 = pt[2];	
	}

	// (7)


	return 4;	// Smooth indirectly
}
Int CPointGE::Line2ArcCircle(CMyPoint& pt1,CMyPoint pt2,CMyPoint& pt3,Float R,Float E,CMyPoint& ptCen,Bool& bCW)
{
	Float A[3] = {0.0};
	Float B[2] = {0.0};
	Float C[3] = {0.0};
	CMyPoint pt[4];

	CMyPoint ptTemp = ptCen;
	Float fR[2] = {0.0,0.0};
	Float fAngle = 0.0;

	E = 0;

	// (1) 
	pt[1] = pt2;
	pt[2] = pt3;

	// (2)
	pt[0] = pt[1];
	pt[0] -= ptCen;
	pt[0].m_fZ = 0.0;
	A[0] = atan2(pt[0].m_fY,pt[0].m_fX);
	fR[0] = pt[0].GetLength();
	if(TRUE != bCW)
	{
		A[0] += 0.5 * M_PI;
	}
	else
	{
		A[0] -= 0.5 * M_PI;
	}

	pt[1] = pt2;
	pt[1] -= pt1;
	A[1] = atan2(pt[1].m_fY,pt[1].m_fX);
	A[2] = A[0] - A[1];
	if(A[2] < 0.0 - M_PI)
	{
		A[2] += 2 * M_PI;
	}
	else if(A[2] > M_PI)
	{
		A[2] -= 2 * M_PI;
	}

	if(fabs(A[2]) > (2.0 * M_PI / 3.0))
	{
		return 0;	// No smooth
	}
	else if(fabs(A[2]) < (M_PI / 6.0))
	{			
		return 3;	// Smooth directly	
	}
	
	if(A[2] > 0.0)
	{
		bCW = FALSE;
		fAngle = 0.5 * M_PI;
	}
	else
	{
		bCW = TRUE;	
		fAngle = 0.0 - 0.5 * M_PI;		
	}


	// (3) Smooth indirectly
	LineExtend(pt[0],0.0 - R,pt1,pt2);
	pt[0] -= ptTemp;
	pt[0].m_fZ = 0.0;	
	if(pt[0].GetLength() > fR[0])
	{
		fR[1] = fR[0] + R; 
	}
	else
	{ 
		fR[1] = fR[0] - R; 
	}

	// (4)
	LineEquation(pt1,pt2,A[0],B[0],C[0]);
	C[2] = C[0];
	LineExtend(pt[0],R,fAngle,pt1,pt2);	
	LineEquation(pt[0],A[0],B[0],C[0]);
	LineArcCross(A[0],B[0],C[0],ptTemp.m_fX,ptTemp.m_fY,fR[1],pt[2],pt[3]);
	pt[0] = pt[2];
	pt[0] -= pt2;
	pt[0].m_fZ = 0.0;
	pt[1] = pt[3];
	pt[1] -= pt2;
	pt[1].m_fZ = 0.0;
	if(pt[0].GetLength() > pt[1].GetLength())
	{
		ptCen = pt[3];
	}
	else
	{
	    ptCen = pt[2];	
	}

	// (5)
	A[1] = 0.0 - B[0];
	B[1] = A[0];
	LineEquation(ptCen,A[1],B[1],C[1]);
	LineLineCross(A[0],B[0],C[2],A[1],B[1],C[1],pt1);

	// (6) 
	LineEquation(ptTemp,ptCen,A[0],B[0],C[0]);
	LineArcCross(A[0],B[0],C[0],ptTemp.m_fX,ptTemp.m_fY,fR[0],pt[2],pt[3]);
	pt[0] = pt[2];
	pt[0] -= pt2;
	pt[0].m_fZ = 0.0;
	pt[1] = pt[3];
	pt[1] -= pt2;
	pt[1].m_fZ = 0.0;
	if(pt[0].GetLength() > pt[1].GetLength())
	{
		pt3 = pt[3];
	}
	else
	{
	    pt3 = pt[2];	
	}

	// (7)
	return 4;	// Smooth indirectly
}


Int CPointGE::Arc2LineCircleEx(CMyPoint pt1,CMyPoint& pt2,CMyPoint pt3,CMyPoint& pt4,Float R,Float E,CMyPoint& ptCen,Bool& bCW)
{
	Int nValue = 0;
	CMyPoint pt[4];
	pt[0] = pt4;
	pt[1] = pt3;
	pt[2] = pt2;
	pt[3] = pt1;
	Bool bTemp = FALSE;
	Float fR = 0.0;
	Pt3Circle(pt[1],pt[2],pt[3],fR,pt[2],bTemp);
	nValue = Line2ArcCircle(pt[0],pt[1],R,E,pt[2],bTemp);

	bCW = !bTemp;
	switch(nValue)
	{
		case 0:
			break;
		case 3:
			break;
		case 4:
			pt4 = pt[0];
			pt2 = pt[1];
			ptCen = pt[2];
			break;
	}

	return nValue;	// Smooth indirectly
}

Int CPointGE::Line2ArcCircle(CMyPoint& pt1,CMyPoint& pt2,Float R,Float E,CMyPoint& ptCen,Bool& bCW)
{
	Float A[3] = {0.0,0.0,0.0};
	Float B[2] = {0.0,0.0};
	Float C[3] = {0.0,0.0,0.0};
	CMyPoint pt[4];
	CMyPoint ptTemp = ptCen;
	Float fR[2] = {0.0,0.0};
	Float fAngle = 0.0;

	E = 0;

	// (1) 切线角度 A[0]
	pt[0] = pt2;
	pt[0] -= ptCen;
	pt[0].m_fZ = 0.0;
	A[0] = atan2(pt[0].m_fY,pt[0].m_fX);
	fR[0] = pt[0].GetLength();
	if(TRUE != bCW)
	{
		A[0] += 0.5 * M_PI;
	}
	else
	{
		A[0] -= 0.5 * M_PI;
	}

	// (2) 直线角度 A[1]
	pt[1] = pt2;
	pt[1] -= pt1;
	A[1] = atan2(pt[1].m_fY,pt[1].m_fX);

	// (3) 角度变化 A[2]
	A[2] = A[0] - A[1];
	if(A[2] < 0.0 - M_PI)
	{
		A[2] += 2 * M_PI;
	}
	else if(A[2] > M_PI)
	{
		A[2] -= 2 * M_PI;
	}

	if(fabs(A[2]) > (2.0 * M_PI / 3.0))
	{
		return 0;	// No smooth
	}
	else if(fabs(A[2]) < (M_PI / 6.0))
	{			
		return 3;	// Smooth directly	
	}
	
	// (4) 间接平滑
	if(A[2] > 0.0)
	{
		bCW = FALSE;
		fAngle = 0.5 * M_PI;
	}
	else
	{	
		bCW = TRUE;
		fAngle = 0.0 - 0.5 * M_PI;		
	}

	// (5) 确定内接还是外接
	LineExtend(pt[0],0.0 - R,pt1,pt2);
	pt[0] -= ptCen;
	pt[0].m_fZ = 0.0;	
	if(pt[0].GetLength() > fR[0])
	{
		fR[1] = fR[0] + R; 
	}
	else
	{ 
		fR[1] = fR[0] - R; 
	}

	// (6) 直线方程偏移与圆弧偏移交点
	LineEquation(pt1,pt2,A[0],B[0],C[0]);
	C[2] = C[0];
	LineExtend(pt[0],R,fAngle,pt1,pt2);	
	LineEquation(pt[0],A[0],B[0],C[0]);
	LineArcCross(A[0],B[0],C[0],ptCen.m_fX,ptCen.m_fY,fR[1],pt[2],pt[3]);

	// (7) 选一点，得圆心
	pt[0] = pt[2];
	pt[0] -= pt2;
	pt[0].m_fZ = 0.0;
	pt[1] = pt[3];
	pt[1] -= pt2;
	pt[1].m_fZ = 0.0;
	if(pt[0].GetLength() > pt[1].GetLength())
	{
		ptCen = pt[3];
	}
	else
	{
	    ptCen = pt[2];	
	}

	// (8) 求圆心到直线垂足
	A[1] = 0.0 - B[0];
	B[1] = A[0];
	LineEquation(ptCen,A[1],B[1],C[1]);
	LineLineCross(A[0],B[0],C[2],A[1],B[1],C[1],pt1);

	// (9) 求圆心到圆弧垂足
	pt[0] = ptCen;
	pt[0] -= ptTemp;
	A[0] = atan2(pt[0].m_fY,pt[0].m_fX);
	pt[0].m_fX = fR[0] * cos(A[0]);
	pt[0].m_fY = fR[0] * sin(A[0]);
	pt2 = ptTemp;
	pt2 += pt[0];


	return 4;	// Smooth indirectly
}
Short CPointGE::lwpolylineToArc(long xstart,long ystart,long xend,long yend,long ang,long * cx,long * cy,long * r)
{
	double centerAngle;//包角
	double DirectionAngle = 0;//弦中点到圆心的直线向量的方向角（0-2PI之间）
	double d;//圆心到弦的距离
	double x1,x2,y1,y2;//圆弧起始点和终止点
	double u;
	double R;//圆弧半径

	//double k;//起始点和终止点连线的中垂线斜率
	double xc,yc;//圆心坐标
	double xa,ya; //起始点和终止点连线的中点横纵坐标

	//centerAngle=4*atan(abs(u));//U为凸度值
	if(ang==360) return 0;

	double ang2 = ang;
	centerAngle = ang2*M_PI/180.0; 

	//printf("centerAngle%lf\n",centerAngle);

	u = -sin(centerAngle/4.0)/cos(centerAngle/4.0);	
	//u = -tanl(centerAngle/4.0);//用tan和tanl会出现死机	

	if(centerAngle<0) centerAngle = 0-centerAngle;

	//printf("1 ");

	x1=xstart;
	x2=xend;
	y1=ystart;
	y2=yend;

	//if(((xstart-xend)==0)&&((ystart-yend)==0))
	//	return 1;

	double L=0; //弦长
	double Ltemp =(xend-xstart)*(xend-xstart)+(yend-ystart)*(yend-ystart);
	//L=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)); 
	//L=sqrt(pow(x2-x1,2)+pow(y2-y1,2));

	L = sqrt(Ltemp);

	//printf("L %lf\n",L);

	if((ang==0)||(((xstart-xend)==0)&&((ystart-yend)==0)))
	{
		*cx = 0;
		*cy = 0;
		*r = 0;		
		return 1;	
	}
	else
		R=0.5*L/sin(0.5*centerAngle);

	//已知圆上两点和半径，求圆心坐标
	//double h;//圆心到弦的距离
	//h=sqrt(R*R-L*L/4);

	xa=0.5*(x1+x2);
	ya=0.5*(y1+y2);

	//弦的方向角（0-2PI之）

	double angle;//起点到终点的弦向量与x正方向之间的倾斜角
	angle=acos((x2-x1)/sqrt(pow(x2-x1,2)+pow(y2-y1,2)));

	double amass; //弦向量与X轴正向单位向量的叉积
	amass = y1-y2;//由（由(x2-x1)*0-1*(y2-y1)）得到

	if (amass<=0)
	{  
		angle=-angle;
		angle=2*M_PI+angle;
		//printf("x  ");
	} 

	if (((u>0) && (centerAngle<M_PI))||((u<0) && (centerAngle>M_PI)))
		DirectionAngle=angle+M_PI_2;

	if(((u<0) && (centerAngle<M_PI))||((u>0) && (centerAngle>M_PI)))
		DirectionAngle=angle-M_PI_2;

	if (DirectionAngle>(2.0*M_PI))
		DirectionAngle= DirectionAngle-2*M_PI;

	d=sqrt(R*R-L*L/4.0);

	if ((DirectionAngle==0)||(DirectionAngle==2*M_PI))///==0
	{
		xc=xa+d;
		yc=ya;
	}
	else if(DirectionAngle==M_PI_2)
	{
		//printf("4  ");
		xc=xa;
		yc=ya-d;
	}
	else if(DirectionAngle==M_PI)
	{
		//printf("5  ");
		xc=xa-d;
		yc=ya;
	}
	else if(DirectionAngle==(M_PI+M_PI_2))
	{
		//printf("6  ");
		xc=xa;
		yc=ya+d;
	}
	else
	{
		//printf("7  ");
		double nslope,k1=0;//nslope 为弦的斜率，K为弦中垂线的斜率
		double nAngle;//中垂线的倾斜角；
		double X,Y; //圆心相对于弦中心点的坐标偏移量
		nslope = (y2 - y1) / (x2-x1);//当y1==y2 时出错
		if(nslope!=0)
		{
			k1 = -1 / nslope;
		}
		//else
		//{
		//	if(u>0) k1 = M_PI +M_PI_2;
		//	else k1 =  M_PI_2;
		//}

		nAngle = atan(k1) ;
		X = cos(nAngle) * d;
		Y = sin(nAngle) * d;

		if ((DirectionAngle > M_PI_2 )&& (DirectionAngle < M_PI ))
		{
			X = -X;
			Y = -Y;
		}
		if ((DirectionAngle > M_PI )&& (DirectionAngle < (M_PI + M_PI_2) ))
		{
			X = -X;
			Y = -Y;
		}

		xc=xa+X;
		yc=ya+Y;


	}

	*cx = (long)xc;
	*cy = (long)yc;
	*r = (long)R;
	*r = *r + 1;

	return 0;

}

// 三点算圆
Short CPointGE::CaclCirclePara(double x1,double y1,double x2,double y2,double x3,double y3,double * cx,double * cy,double * r)
{
	double a,b,c,d,e,f,ccx,ccy,ccr;
	double xx1,xx2,xx3,yy1,yy2,yy3;
	double k1=0,k2=0;

	xx1 = x1;
	xx2 = x2;
	xx3 = x3;
	yy1 = y1;
	yy2 = y2;
	yy3 = y3;

	if((yy1==yy2)&&(yy2==yy3))//三点成线不成圆
		return 1;

	if((xx1==xx2)&&(xx2==xx3))//三点成线不成圆
		return 1;

	if((yy1!=yy2)&&(yy2!=yy3))
	{
		k1=(xx2-xx1)/(yy2-yy1);
		k2=(xx3-xx2)/(yy3-yy2);
		if(k1==k2)
			return 1;
	}

	a = 2*(xx2-xx1);
	b = 2*(yy2-yy1);
	c = xx2*xx2+yy2*yy2-xx1*xx1-yy1*yy1;
	d = 2*(xx3-xx2);
	e = 2*(yy3-yy2);
	f = xx3*xx3+yy3*yy3-xx2*xx2-yy2*yy2;
	ccx = (b*f-e*c)/(b*d-e*a);
	ccy = (d*c-a*f)/(b*d-e*a);
	ccr = sqrt((ccx-xx1)*(ccx-xx1)+(ccy-yy1)*(ccy-yy1));

	*cx = ccx;
	*cy = ccy;
	*r = ccr;
	return 0;
}
Float CPointGE::Get2ptArcAngle(double x0,double y0,double x1,double y1,double cx,double cy,double r,char cDir)
{//从(x0,y0) 到 (x1,y1) 角度 分正负 顺时针为正，逆时针为负 跟坐标系有关
	double angle0,angle1;
	double cosLen0,cosLen1;
	double retvalue = 0;

	cosLen0 = x0 - cx;
	cosLen1 = x1 - cx;

	if((cosLen0 == 0)&&(y0>cy))
		angle0 = 90;
	else if((cosLen0 == 0)&&(y0<cy))
		angle0 = 270;
	else
	{
		angle0 = (acos (cosLen0/fabs(r)))*180/M_PI;//0 ~180
		if(y0<cy)
			angle0 = 360 - angle0;
		//if((cosLen0>0)&&(y0<cy))
		//	angle0 = 360 - angle0;
		//else if((cosLen0<0)&&(y0>=cy))
		//	angle0 = angle0 + 180;
		//else if((cosLen0<0)&&(y0<cy))
		//	angle0 = 180 - angle0;
	}

	if((cosLen1 == 0)&&(y1>cy))
		angle1 = 90;
	else if((cosLen1 == 0)&&(y1<cy))
		angle1 = 270;
	else
	{
		angle1 = (acos (cosLen1/fabs(r)))*180/M_PI;//0 ~180
		if(y1<cy)
			angle1 = 360 - angle1;
		//if((cosLen1>0)&&(y1<cy))
		//	angle1 = 360 - angle1;
		//else if((cosLen1<0)&&(y1>=cy))
		//	angle1 = angle1 + 180;
		//else if((cosLen1<0)&&(y1<cy))
		//	angle1 = 180 - angle1;
	}
	//angle0,angle1---- 0~360

	if(angle1>angle0)
	{
		if(cDir==1)//FOR_CIRCLE retvalue为负数
			retvalue = angle1-angle0-360;
		else if(cDir==2)//INV_CIRCLE retvalue为正数
			retvalue = angle1-angle0;
	}
	if(angle1<angle0)
	{
		if(cDir==1)//FOR_CIRCLE
			retvalue = angle1 - angle0;//retvalue为负数
		else if(cDir==2)//INV_CIRCLE
			retvalue = (angle1-angle0)+360;//retvalue为正数
	}


	//TRACE("first out%.3f angle1%.3f angle0%.3f \n",retvalue,angle1,angle0);
		
	return retvalue;
}
// 计算角度
Float CPointGE::Get3ptArcAngle(double x0,double y0,double x1,double y1,double x2,double y2,double cx,double cy,double r)
{
	double angle0,angle1,angle2;
	double cosLen0,cosLen1,cosLen2;
	double retvalue=0;

	cosLen0 = x0 - cx;
	cosLen1 = x1 - cx;
	cosLen2 = x2 - cx;
	if((cosLen0 == 0)&&(y0>cy))
		angle0 = 90;
	else if((cosLen0 == 0)&&(y0<cy))
		angle0 = 270;
	else
	{
		angle0 = (acos (cosLen0/r))*180/M_PI;
		if((angle0>0)&&(y0<cy))
			angle0 = 360 - angle0;
		else if((angle0<0)&&(y0>=cy))
			angle0 = angle0 + 180;
		else if((angle0<0)&&(y0<cy))
			angle0 = 180 - angle0;
	}

	if((cosLen1 == 0)&&(y1>cy))
		angle1 = 90;
	else if((cosLen1 == 0)&&(y1<cy))
		angle1 = 270;
	else
	{
		angle1 = (acos (cosLen1/r))*180/M_PI;
		if((angle1>0)&&(y1<cy))
			angle1 = 360 - angle1;
		else if((angle1<0)&&(y1>=cy))
			angle1 = angle1 + 180;
		else if((angle1<0)&&(y1<cy))
			angle1 = 180 - angle1;
	}

	if((cosLen2 == 0)&&(y2>cy))
		angle2 = 90;
	else if((cosLen2 == 0)&&(y2<cy))
		angle2 = 270;
	else
	{
		angle2 = (acos (cosLen2/r))*180/M_PI;
		if((angle2>0)&&(y2<cy))
			angle2 = 360 - angle2;
		else if((angle2<0)&&(y2>=cy))
			angle2 = angle2 + 180;
		else if((angle2<0)&&(y2<cy))
			angle2 = 180 - angle2;
	}

	if(angle2>angle0)
	{
		if((angle1>angle0)&&(angle2>angle1))
			retvalue = angle2-angle0;
		else retvalue = angle2-angle0-360;
	}
	if(angle2<angle0)
	{
		if((angle2<angle1)&&(angle1<angle0))
			retvalue = angle2-angle0;
		else retvalue = angle2-angle0+360;
	}

	return retvalue;
}