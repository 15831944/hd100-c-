/**************************************
// FILE:	PointGE.h
// 功能： 完成基于点的若干数学运算
***************************************/
#ifndef _POINT_GE_H_
#define _POINT_GE_H_

#include "MyPoint.h"

class CPointGE
{
public:
	CPointGE();
	virtual ~CPointGE();

	Bool LineExtend(CMyPoint& pt,Float fLength,CMyPoint pt1,CMyPoint pt2);
	Bool LineExtend(CMyPoint& pt,Float fLength,Float fAngle,CMyPoint pt1,CMyPoint pt2);
	Bool LineEquation(CMyPoint pt1,CMyPoint pt2,Float& A,Float& B,Float& C);
	Bool LineEquation(CMyPoint pt,Float A,Float B,Float& C);
	Bool LineLineCross(Float A1,Float B1,Float C1,Float A2,Float B2,Float C2,CMyPoint& ptCross);
	Bool LineArcCross(Float A,Float B,Float C,Float a,Float b,Float r,CMyPoint& pt1,CMyPoint& pt2);
	Bool Pt3Circle(CMyPoint pt1,CMyPoint pt2,CMyPoint pt3,Float& R,CMyPoint& ptCen,Bool& bCW);
	Int  Pt3Circle(CMyPoint& pt1,CMyPoint pt2,CMyPoint& pt3,Float R,Float E,CMyPoint& ptCen,Bool& bCW);
	Int  Arc2LineCircle(CMyPoint pt1,CMyPoint& pt2,CMyPoint pt3,CMyPoint& pt4,Float R,Float E,CMyPoint& ptCen,Bool& bCW);
	Int  Line2ArcCircle(CMyPoint& pt1,CMyPoint pt2,CMyPoint& pt3,CMyPoint pt4,Float R,Float E,CMyPoint& ptCen,Bool& bCW);
	Bool ArcExtend(Float fLength,Bool bCW,CMyPoint ptCen,CMyPoint ptEnd,CMyPoint& pt);
	Int  Line2ArcCircle(CMyPoint& pt1,CMyPoint pt2,CMyPoint& pt3,Float R,Float E,CMyPoint& ptCen,Bool& bCW);
	Int  Arc2LineCircle(CMyPoint& pt1,CMyPoint pt2,CMyPoint& pt3,Float R,Float E,CMyPoint& ptCen,Bool& bCW);
	Int  Arc2LineCircleEx(CMyPoint pt1,CMyPoint& pt2,CMyPoint pt3,CMyPoint& pt4,Float R,Float E,CMyPoint& ptCen,Bool& bCW);
    Int  Line2ArcCircle(CMyPoint& pt1,CMyPoint& pt2,Float R,Float E,CMyPoint& ptCen,Bool& bCW);

public:
	Short lwpolylineToArc(long xstart,long ystart,long xend,long yend,long ang,long * cx,long * cy,long * r);
	Float Get2ptArcAngle(double x0,double y0,double x1,double y1,double cx,double cy,double r,char cDir);
	Float Get3ptArcAngle(double x0,double y0,double x1,double y1,double x2,double y2,double cx,double cy,double r);
	Short CaclCirclePara(double x1,double y1,double x2,double y2,double x3,double y3,double * cx,double * cy,double * r);
};
#endif	//Not defined _POINT_GE_H_
