#pragma once
#include "SapClassBasic.h"
//#include "SapClassGui.h"
#include "Camera.h"

//void XferCallback(SapXferCallbackInfo *pInfo);
class CCameraDS : public CCamera 
{
public:
	CCameraDS(void);
	virtual ~CCameraDS(void);

	// 打开相机
	BOOL OpenCamera(CString camName, HWND hWndDisplay, LPBYTE& lpDesIMGAddress);
	static void XferCallback(SapXferCallbackInfo *pInfo);
	BOOL CreateObjects();
	BOOL DestroyObjects();
	BOOL Init0();
	BOOL Init();

	// 设置为Trigger模式  bTriggerMode  true:设置为Trigger模式   false:连续采集模式
	BOOL SetTriggerMode(bool bTriggerMode);		
	// 发送软触发
	BOOL SoftTrigger();		
	// 等待callback方法获取一帧图像,图像数据保存在buf中
	BOOL WaitGrabOneFrame();

	bool m_bCameraInitFlag;	// 当前相机初始化成功标志
	HWND m_hWndShowImage;
	BYTE* m_pBuffer;

//	CImageExWnd		m_ImageWnd;
	SapAcqDevice	*m_AcqDevice;
	SapBuffer		*m_Buffers;
	SapTransfer		*m_Xfer;
	SapView			*m_View;
	CString			m_strFilePath;
	CString			m_serverName; 
	int				m_resourceIndex;
};

