
#include "hvdh/include/HVDAILT.H"
#include "hvdh/include/Raw2Rgb.h"

#ifndef CKSVDH_H
#define CKSVDH_H

struct STRUCT_KSVDH
{
	HHV hhv;
	int iIndexNo;//0.1.2 ...
};

//////////////////////////////////////////////////////////////////////////
class CKSVDHEx
{
public:
	CKSVDHEx();
	~CKSVDHEx();

public:
	BOOL InitDH(int iIndexNo=0/*0.1.2...*/, long nUperFocus = 30, long nDownFocus =1000, 
		long nGain=25, int nStartX=0, int nStartY=0, int nWidth=-1, int nHeight=-1,
		HV_RESOLUTION Resolution = RES_MODE0, BOOL bRGB24 = FALSE);
	void FreeDH();

	////
	void Live();
	void Stop();
	///
	virtual void  CallBack(STRUCT_KSVDH KSVDH)=0;
private:
	static int CALLBACK SnapThreadCallback(HV_SNAP_INFO *pInfo); 
	STRUCT_KSVDH m_structKSVDH;

public:	
	int		m_iIndexNo;/*0.1.2...*/
	int		m_OutSizeX, m_OutSizeY;
	BYTE	*m_pRawBuffer;
	BYTE	*m_pImageBuffer;
	HHV		m_hHV;
	HWND	m_hWnd;
	
	BITMAPINFO *m_pBmpInfo;
private:
	char m_chBmpBuf[2048]; 
	BOOL m_bStart;
	BOOL m_bOpen;
	HV_RESOLUTION m_Resolution;
	BOOL m_bRGB24;

	BYTE m_pLutR[256];
	BYTE m_pLutG[256];
	BYTE m_pLutB[256];
    int	m_nTotal;

};

#endif