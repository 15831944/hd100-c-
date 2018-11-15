#pragma once
// #include "CameraDS.h"
// #include "CameraHK.h"

class CCamera
{
public:
	CCamera(void);
	~CCamera(void);

	/*	功能:	打开相机
	 *	参数：	camName			相机名称,一般在外部配置好.本程序中在外部配置好"相机1"
	 			hWndDisplay		显示相机图像的窗口句柄
				lpDesIMGAddress 图像地址,BYTE类型指针,相机初始化成功后将此指针指向相机程序分配的内存
	 *	返回值:	TRUE	FALSE	相机是否初始化成功							
	 */
	virtual BOOL OpenCamera(CString camName, HWND hWndDisplay, LPBYTE& lpDesIMGAddress, int nWidth=IMG_WIDTH, int nHeight=IMG_HEIGHT, int nDepth=8);	
	
	// 设置为Trigger模式  bTriggerMode  true:设置为Trigger模式   false:连续采集模式
	virtual BOOL SetTriggerMode(bool bTriggerMode);		

	// 发送软触发
	virtual BOOL SoftTrigger();		

	// 等待callback方法获取一帧图像,图像数据保存在buf中
	virtual BOOL WaitGrabOneFrame();	
};

