#include "StdAfx.h"
#include "Camera.h"


CCamera::CCamera(void)
{
}


CCamera::~CCamera(void)
{
}

BOOL CCamera::OpenCamera(CString camName, HWND hWndDisplay, LPBYTE& lpDesIMGAddress, int nWidth, int nHeight, int nDepth)	
{
	UNREFERENCED_PARAMETER(camName);
	UNREFERENCED_PARAMETER(hWndDisplay);
	UNREFERENCED_PARAMETER(lpDesIMGAddress);
	UNREFERENCED_PARAMETER(nWidth);
	UNREFERENCED_PARAMETER(nHeight);
	UNREFERENCED_PARAMETER(nDepth);
	return TRUE;
}

// ����ΪTriggerģʽ  bTriggerMode  true:����ΪTriggerģʽ   false:�����ɼ�ģʽ
BOOL CCamera::SetTriggerMode(bool bTriggerMode)	
{
	UNREFERENCED_PARAMETER(bTriggerMode);
	return TRUE;
}

// ��������
BOOL CCamera::SoftTrigger()		
{
	return TRUE;
}

// �ȴ�callback������ȡһ֡ͼ��,ͼ�����ݱ�����buf��
BOOL CCamera::WaitGrabOneFrame()	
{
	return TRUE;
}