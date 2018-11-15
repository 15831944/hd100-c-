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

// 设置为Trigger模式  bTriggerMode  true:设置为Trigger模式   false:连续采集模式
BOOL CCamera::SetTriggerMode(bool bTriggerMode)	
{
	UNREFERENCED_PARAMETER(bTriggerMode);
	return TRUE;
}

// 发送软触发
BOOL CCamera::SoftTrigger()		
{
	return TRUE;
}

// 等待callback方法获取一帧图像,图像数据保存在buf中
BOOL CCamera::WaitGrabOneFrame()	
{
	return TRUE;
}