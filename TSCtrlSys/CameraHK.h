// HKCamera.h: interface for the CHKCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HKCAMERA_H__1059D248_131A_49A7_AE19_41335E233EFB__INCLUDED_)
#define AFX_HKCAMERA_H__1059D248_131A_49A7_AE19_41335E233EFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Camera.h"

class CCameraHK : public CCamera 
{
public:
	CCameraHK();
	virtual ~CCameraHK();

	//////////////////////////////////////////////////////////////////////////
	// 所有相机公用变量
	static int m_nCamNum;		// 成功初始化相机总数
	static bool m_bCamSysInit;	// 相机系统初始化
    static MV_CC_DEVICE_INFO_LIST m_stDeviceList;	// 设备列表结构体变量
	// end 所有相机公用变量
	//////////////////////////////////////////////////////////////////////////
            
	void* m_pHandle;	// 相机句柄
	MV_CC_DEVICE_INFO m_stDeviceInfo;	// 当前设备信息结构体变量
	BYTE* m_pImageData;	// 采集到的图像数据
	int m_nWidth;
	int m_nHeight;
	int m_nImageSize;
	HWND m_hWndShowImage;

	bool m_bCameraInitFlag;	// 当前相机初始化成功标志
	int m_nCamIndex;	// 当前相机序号	从0开始
	int m_nRet;			// 海康相机函数返回值

	// 打开相机
	BOOL OpenCamera(CString camName, HWND hWndDisplay, LPBYTE& lpDesIMGAddress, int nWidth=IMG_WIDTH, int nHeight=IMG_HEIGHT, int nDepth=8);	
	BOOL CloseCamera();			// 关闭相机
	BOOL GrabOneFrame();		// MV_CC_GetOneFrameTimeout 获取一帧图像	
	BOOL WaitGrabOneFrame();	// callback方法获取一帧图像,图像数据保存在buf中

	BOOL SetTriggerMode(bool bTriggerMode);		// 设置为Trigger模式  bTriggerMode  true:设置为Trigger模式   false:连续采集模式
	BOOL SoftTrigger();		// 发送软触发

	BOOL SetGain(double gain);	// 设置摄像头增益
	BOOL SetExp(long expUs);	// 设置曝光时间 单位:us
	bool m_bTranseferEnd;

	MV_FRAME_OUT_INFO_EX stImageInfo;
    MV_FRAME_OUT_INFO stMVFrameInfo;

public:
	static BOOL InitCamera();			// 初始化相机系统,多个相机全部初始化
};

#endif // !defined(AFX_HKCAMERA_H__1059D248_131A_49A7_AE19_41335E233EFB__INCLUDED_)
