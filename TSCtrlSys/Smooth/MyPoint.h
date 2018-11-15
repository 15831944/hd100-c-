/********************************************************************
//
// FILE:	point.h
//
*******************************************************************/

#ifndef _MYPOINT_H_
#define _MYPOINT_H_


typedef  BOOL	Bool;
typedef  double Float;
typedef  int	Int;
typedef  short	Short;
typedef  void	Void;

class CMyPoint
{
public:
	CMyPoint();
	CMyPoint(Float fX,Float fY);
	CMyPoint(Float fX,Float fY,Float fZ);
	virtual ~CMyPoint();

	Void operator=(const CMyPoint pt);
	Void operator-=(const CMyPoint pt);
	Void operator+=(const CMyPoint pt);
	Void operator/=(const Float fFactor);
	Void operator*=(const Float fFactor);
	Float GetLength();
public:
	Float	m_fX;
	Float	m_fY;
	Float	m_fZ;
	Float   m_fU;
};
#endif	//Not defined _MYPOINT_H_