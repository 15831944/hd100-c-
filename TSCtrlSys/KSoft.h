//********************************************
//
//		Name: KSoft.dll
//		CopyRight: KSoft China, Ltd. (C) 2011
//		Version: 1.0
//		Date: 2011-09-28
//		Author:wang qijun  TenSun China, Ltd.
//
//*******************************************
#include "KSColorDef.h"

#ifndef _KSOFT_H
#define _KSOFT_H

#define KSOFT_API _stdcall

typedef long KS_ID;

#define KS_GRAY256	0
#define KS_RGB24	1
#define KS_RGB8	2
/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

//////////////////
//Init
//Alloc Buffer/ Copy Data to KS_Buffer
void KSOFT_API KS_CopyBuffer(KS_ID KSBuffer, BYTE *pData, const int width, const int height, int KSType=KS_GRAY256);
void KSOFT_API KS_CopyBufferByID(KS_ID SrcKSBufferID, KS_ID DesKSBufferID);

///////
class CKSoftClass  
{
public:
	CKSoftClass();
	~CKSoftClass();

public:
	void Free();
	bool Init(HWND hWnd, int Width, int Height, int Band, bool bReSizeWindow=false);
	//
	bool AllocDisplay(HWND hWnd, KS_ID *pDisplayID, KS_ID *pDisBuffer, int Width, int Height, int Band, bool bResizeWindow=false);
	void FreeDisplay(KS_ID DisplayID, KS_ID KSDisBuffer);
	void DisplayZoom(double dScalex, double dScaley);

	void LoadImage(CString strPath);
	void GetBuff(BYTE* pData);
	void SaveImage(CString strPath);
public:
	int m_Width;
	int m_Height;

public:
	KS_ID m_KSBuffer;   //Դ
};

//**********************************************
//Pattern Function
struct KSPat_Param
{
	double accuracy; //M_LOW, M_MEDIUM, M_HIGH
	long number;//1~20
	double speed; //M_HIGH, M_MEDIUM, M_LOW, M_VERY_LOW
	double acceptance; // 0~100
	double certainty; //0~100

	//angle
	bool angle_enable;
	double angle;//0~360
	double angle_delta;//0.1~180
	//
	double offsetx, offsety;
	double sizex, sizey;
};


class CKSoftPat
{
public:
	CKSoftPat();
	~CKSoftPat();
public:
	bool AllocPattern(KS_ID ImageBufferID,char *pSavePath, int left, int top, int width, int height);
	bool LoadPattern(char *pLoadPath);
	void SavePattern(char *pSavepath);
	void FreePattern();

	long FindPattern(KS_ID SrcImageBufId, double *dScore=NULL, double *dX=NULL, double *dY=NULL);
	KS_ID GetPatBufferID(int *pWidth =NULL, int *pHeight = NULL);

	void SetParam(KSPat_Param param);
	void GetParam(KSPat_Param &param);

public:	
	KS_ID m_KSPattern;
	KS_ID m_KSBuffer;
};
//**********************************************

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#endif