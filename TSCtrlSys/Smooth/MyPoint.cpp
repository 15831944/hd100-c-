//###########################################################################
//
// FILE:	Point.cpp
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
#include "MyPoint.h"
#define _USE_MATH_DEFINES
#include <math.h>


CMyPoint::CMyPoint()
{
	m_fX = 0.0;
	m_fY = 0.0;
	m_fZ = 0.0;
	m_fU = 0.0;
}

CMyPoint::CMyPoint(Float fX,Float fY)
{
	m_fX = fX;
	m_fY = fY;
	m_fZ = 0.0;
	m_fU = 0.0;
}

CMyPoint::CMyPoint(Float fX,Float fY,Float fZ)
{
	m_fX = fX;
	m_fY = fY;
	m_fZ = fZ;
	m_fU = 0.0;
}

CMyPoint::~CMyPoint()
{

}

Void CMyPoint::operator=(const CMyPoint pt)
{
	m_fX = pt.m_fX;
	m_fY = pt.m_fY;
	m_fZ = pt.m_fZ;
	m_fU = pt.m_fU;	
}

Void CMyPoint::operator-=(const CMyPoint pt)
{
	m_fX -= pt.m_fX;
	m_fY -= pt.m_fY;
	m_fZ -= pt.m_fZ;
	m_fU -= pt.m_fU;	
}

Void CMyPoint::operator+=(const CMyPoint pt)
{
	m_fX += pt.m_fX;
	m_fY += pt.m_fY;
	m_fZ += pt.m_fZ;
	m_fU += pt.m_fU;	
}

Void CMyPoint::operator*=(const Float fFactor)
{
	m_fX *= fFactor;
	m_fY *= fFactor;
	m_fZ *= fFactor;
	m_fU *= fFactor;	
}

Void CMyPoint::operator/=(const Float fFactor)
{
	m_fX /= fFactor;
	m_fY /= fFactor;
	m_fZ /= fFactor;
	m_fU /= fFactor;	
}

Float CMyPoint::GetLength()
{
	Float fLength = 0.0;
	fLength =  sqrt(m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ );
	return fLength;
}
