#include "stdafx.h"
#include "mil.h"
#include ".\MilDyn\milpat.h"
#include ".\MilDyn\MilMod.h"
#pragma comment(lib,"mil.lib")
#pragma comment(lib,"milim.lib")
#pragma comment(lib,"milpat.lib")
//#pragma comment (lib,"milvga.lib")
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include "KSoft.h"

KS_ID m_KSApplication;
KS_ID m_KSSystem;
KS_ID m_KSDisplay;
KS_ID m_KSOverlayDisplay;
KSPat_Param g_KsPatParam;
//Alloc Buffer/ Copy Data to KS_Buffer
void KSOFT_API KS_CopyBuffer(KS_ID KSBuffer, BYTE *pData, const int width, const int height, int KSType)
{
	UNREFERENCED_PARAMETER(width);
	UNREFERENCED_PARAMETER(height);
	if(pData == NULL)  
	{
		TRACE(_T("\nKS_CopyBuffer pData=NULL\n"));
		return;
	}

	long  BufSizeX = 656, BufSizeY = 494;
	int _iSize = BufSizeX * BufSizeY;
	if (KSType==KS_RGB24)
	{
		BYTE *m_pGrayBuffer = new BYTE[_iSize];
		for(int i=0; i<_iSize; i++)
		{
			m_pGrayBuffer[i]=(BYTE)(0.299*pData[3*i+0]+0.587*pData[3*i+1]+0.114*pData[3*i+2]) ;//²ÊÉ«×ªºÚ°×
		}
		MbufPut2d(KSBuffer, 0, 0, BufSizeX, BufSizeY, m_pGrayBuffer);

		delete []m_pGrayBuffer;
		m_pGrayBuffer = NULL;
	}
	else if (KSType==KS_RGB8)
	{
		BYTE *tempData = new BYTE[_iSize];

		for (int i=0;i<BufSizeY;i++)
		{
			for (int j=0;j<BufSizeX;j++)
			{
				// tempData[BufSizeX*i+j]=pData[(BufSizeY-i-1)*BufSizeX+j];
                   tempData[BufSizeX*i+j]=pData[BufSizeX*i+j];
			}
		}

		MbufPut2d(KSBuffer, 0, 0, BufSizeX, BufSizeY, tempData);
		delete[]tempData;
		tempData = NULL;
	}
}
void KSOFT_API KS_CopyBufferByID(KS_ID SrcKSBufferID, KS_ID DesKSBufferID)
{
	if(SrcKSBufferID<=0) return;
	MpatCopy(SrcKSBufferID,DesKSBufferID,M_DEFAULT);
}


void CKSoftClass::GetBuff(BYTE* pData)
{
	MbufGet(m_KSBuffer, pData);
}

CKSoftClass::CKSoftClass()
{

}

CKSoftClass:: ~CKSoftClass()
{

}

void CKSoftClass::Free()
{	
	 if(m_KSBuffer>0) 
		MbufFree(m_KSBuffer);
     MappFreeDefault(m_KSApplication,m_KSSystem,m_KSDisplay,M_NULL,M_NULL);
}
bool CKSoftClass:: Init(HWND hWnd, int Width, int Height, int Band, bool bReSizeWindow)
{
//	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(Band);
	UNREFERENCED_PARAMETER(bReSizeWindow);
	m_Width = Width;									
	m_Height = Height;	

	MappAllocDefault(M_SETUP,&m_KSApplication,&m_KSSystem,&m_KSDisplay,M_NULL,M_NULL);
	MbufAlloc2d(m_KSSystem,Width,Height,M_DEF_IMAGE_TYPE,M_IMAGE+M_DISP+M_PROC,&m_KSBuffer);

	MdispSelectWindow(m_KSDisplay, m_KSBuffer, hWnd);
	/* Enable the display of overlay annotations. */
	MdispControl(m_KSDisplay, M_OVERLAY, M_ENABLE);
	/* Inquire the overlay buffer associated with the display. */
	MdispInquire(m_KSDisplay, M_OVERLAY_ID, &m_KSOverlayDisplay);
	/* Clear the overlay to transparent. */
	MdispControl(m_KSDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
	return TRUE;
}

void CKSoftClass::DisplayZoom(double dScalex, double dScaley)
{
	MdispZoom(m_KSDisplay, dScalex, dScaley);
}
//
bool CKSoftClass::AllocDisplay(HWND hWnd, KS_ID *pDisplayID, KS_ID *pDisBuffer, int Width, int Height, int Band, bool bResizeWindow)
{
	UNREFERENCED_PARAMETER(Band);
	UNREFERENCED_PARAMETER(bResizeWindow);
	MdispAlloc(m_KSSystem,M_DEFAULT,M_DEF_DISPLAY_FORMAT,M_DEFAULT,pDisplayID);
	MbufAlloc2d(m_KSSystem,Width,Height,8 + M_UNSIGNED,M_IMAGE+M_DISP+M_PROC,pDisBuffer);
	if ( bResizeWindow )
	{
		CRect rt;
		GetClientRect(hWnd, rt);
		double iScale_x=1.0*Width/rt.Width();
		double iScale_y=1.0*Height/rt.Height();

		iScale_x = iScale_x>1 ? -floor(iScale_x):floor(1/iScale_x);
		iScale_y = iScale_y>1 ? -floor(iScale_y):floor(1/iScale_y);
		TRACE("\nAllocDisplay: w=%d, h=%d, X=%.2f, y=%.2f, w1=%d, h1=%d", Width, Height, iScale_x, iScale_y, rt.Width(), rt.Height());
		iScale_x = fabs(iScale_x) > 16 ? 16:iScale_x;
		iScale_y = fabs(iScale_y) > 16 ? 16:iScale_y;
		TRACE("\nAllocDisplay: w=%d, h=%d, X=%.2f, y=%.2f, w1=%d, h1=%d", Width, Height, iScale_x, iScale_y, rt.Width(), rt.Height());

		MdispZoom(*pDisplayID, long(iScale_x), long(iScale_y));
	}

	MdispSelectWindow(*pDisplayID,*pDisBuffer,hWnd);
	return TRUE;
}

void CKSoftClass::FreeDisplay(KS_ID DisplayID, KS_ID KSDisBuffer)
{
	if(DisplayID>0)
		MdispFree(DisplayID);

	if(KSDisBuffer>0)
		MbufFree(KSDisBuffer);
}

void CKSoftClass::LoadImage(CString strPath)
{
	if (m_KSBuffer > 0)
	{
		MbufFree(m_KSBuffer);
		m_KSBuffer = 0;
	}
	MbufRestore(strPath.GetBuffer(strPath.GetLength()), m_KSSystem, &m_KSBuffer);
}

void CKSoftClass::SaveImage(CString strPath)
{
	if (m_KSBuffer > 0)
	{
		MbufExport(strPath.GetBuffer(strPath.GetLength()), M_BMP, m_KSBuffer);
		strPath.ReleaseBuffer();
	}
}

CKSoftPat::CKSoftPat()
{
	g_KsPatParam.acceptance=70;
	g_KsPatParam.accuracy=M_HIGH;
	g_KsPatParam.certainty=80;
	g_KsPatParam.angle=0;
	g_KsPatParam.angle_delta=180;
	g_KsPatParam.angle_enable = FALSE;
	g_KsPatParam.number=1L;
	g_KsPatParam.offsetx=0;
	g_KsPatParam.offsety=0;
	g_KsPatParam.sizex=0;
	g_KsPatParam.sizey=0;
	g_KsPatParam.speed = M_MEDIUM;
}
CKSoftPat:: ~CKSoftPat()
{

}

bool CKSoftPat::AllocPattern(KS_ID ImageBufferID,char *pSavePath, int left, int top, int width, int height)
{
	if(ImageBufferID<=0 || width<=1 || height<=1)
		return FALSE;

	if(m_KSPattern>0)
		MpatFree(m_KSPattern);

	MpatAllocModel(m_KSSystem,ImageBufferID,
		left,top,
		width,height,
		M_NORMALIZED,
		&m_KSPattern);  

	g_KsPatParam.offsetx=left;
	g_KsPatParam.offsety = top;
	g_KsPatParam.sizex=width;
	g_KsPatParam.sizey=height;

	if(pSavePath != NULL)
		MpatSave(pSavePath,m_KSPattern);

	TRACE(_T("\nAllocPattern:L=%d,T=%d,B=%d,R=%d\n"), left, top, width, height);
	return TRUE;
}

bool CKSoftPat::LoadPattern(char *pLoadPath)
{
	ASSERT(pLoadPath != NULL);
	CFileFind   find;
	if(find.FindFile( pLoadPath ) == 0) return FALSE;
	TRACE(_T("\nLoadPattern**********m_KSPatternr=%ld"), m_KSPattern);
	FreePattern();//modify by zdx
	if(MpatRestore(m_KSSystem,pLoadPath,&m_KSPattern)==M_NULL) return FALSE;
	if(m_KSPattern<=0) return FALSE;

	MpatInquire(m_KSPattern,M_POSITION_ACCURACY,&g_KsPatParam.accuracy);
	MpatInquire(m_KSPattern,M_NUMBER_OF_OCCURRENCES+M_TYPE_LONG,&g_KsPatParam.number);
	MpatInquire(m_KSPattern,M_SPEED_FACTOR,&g_KsPatParam.speed);
	MpatInquire(m_KSPattern,M_ACCEPTANCE_THRESHOLD	,&g_KsPatParam.acceptance);
	MpatInquire(m_KSPattern,M_CERTAINTY_THRESHOLD	,&g_KsPatParam.certainty);
	MpatInquire(m_KSPattern,M_SEARCH_ANGLE_MODE,&g_KsPatParam.angle_enable);
	MpatInquire(m_KSPattern,M_SEARCH_ANGLE	,&g_KsPatParam.angle);
	MpatInquire(m_KSPattern,M_SEARCH_ANGLE_DELTA_NEG,&g_KsPatParam.angle_delta);
	MpatInquire(m_KSPattern,M_ALLOC_SIZE_X	,&g_KsPatParam.sizex);
	MpatInquire(m_KSPattern,M_ALLOC_SIZE_Y	,&g_KsPatParam.sizey);
	return TRUE;
}

void CKSoftPat::SavePattern(char *pSavepath)
{
	ASSERT(pSavepath != NULL);
	MpatSave(pSavepath,m_KSPattern);
}

void CKSoftPat::FreePattern()
{
	if(m_KSPattern>0)
		MpatFree(m_KSPattern);
}

long CKSoftPat::FindPattern(KS_ID SrcImageBufId, double *dScore, double *dX, double *dY)
{
	if(SrcImageBufId<=0 || m_KSPattern<=0 || m_KSSystem <=0 )  return 0;
	KS_ID Result;
	long number =0;
	MpatAllocResult(m_KSSystem, 1L, &Result);
	TRACE(_T("\nResult = %ld, g_KsPatParam.number=%ld"), Result, g_KsPatParam.number);
	MpatSetNumber(m_KSPattern,(long)g_KsPatParam.number);
	MpatSetAccuracy(m_KSPattern,(long)g_KsPatParam.accuracy);
	MpatSetSpeed(m_KSPattern,(long)g_KsPatParam.speed);
	MpatSetAcceptance(m_KSPattern,(long)g_KsPatParam.acceptance);
	MpatSetCertainty(m_KSPattern,(long)g_KsPatParam.certainty);

	if(g_KsPatParam.angle_enable)
	{
		MpatSetAngle(m_KSPattern,M_SEARCH_ANGLE_MODE	,M_ENABLE);
		MpatSetAngle(m_KSPattern,M_SEARCH_ANGLE,g_KsPatParam.angle);
		MpatSetAngle(m_KSPattern,M_SEARCH_ANGLE_DELTA_NEG,g_KsPatParam.angle_delta);
		MpatSetAngle(m_KSPattern,M_SEARCH_ANGLE_DELTA_POS,g_KsPatParam.angle_delta);
	}
	//	MpatSetPosition(m_KSPattern,g_KsPatParam.offsetx,g_KsPatParam.offsety,g_KsPatParam.sizex,g_KsPatParam.sizey)

	MpatFindModel(SrcImageBufId, m_KSPattern, Result);
	if( MpatGetNumber(Result,&number) >= 1 )
	{
		if(dX != NULL)
			MpatGetResult(Result,M_POSITION_X,dX);
		if(dY != NULL)
			MpatGetResult(Result,M_POSITION_Y,dY);
		if(dScore != NULL)
			MpatGetResult(Result,M_SCORE,dScore);

		//double da=0.0;
		//MpatGetResult(Result,M_ANGLE	,&da);
		//CString str;
		//str.Format(_T("da=%.3f"),da);
		//AfxMessageBox(str);
	}

	MpatFree(Result);
	return number;
}
KS_ID CKSoftPat::GetPatBufferID(int *pWidth , int *pHeight)
{
	UNREFERENCED_PARAMETER(pWidth);
	UNREFERENCED_PARAMETER(pHeight);
	return m_KSPattern;
}

void CKSoftPat::SetParam(KSPat_Param param)
{
	memcpy(&g_KsPatParam,&param,sizeof(param));
}
void CKSoftPat::GetParam(KSPat_Param &param)
{
	memcpy(&param,&g_KsPatParam,sizeof(g_KsPatParam));
}