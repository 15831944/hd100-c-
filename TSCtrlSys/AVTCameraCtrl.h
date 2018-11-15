#pragma once
#include <vfw.h>
#include "CameraCtrlBase.h"

#include "AVT/FGCamera.h"
#include "AVT/convert.h"
#pragma comment(lib, "AVT/FGCamera.lib")

// AVT 相机控制
class CAVTCameraCtrl:public CCameraCtrlBase,public CWnd
{
	DECLARE_DYNAMIC(CAVTCameraCtrl)
public:
	CAVTCameraCtrl(void);
	virtual ~CAVTCameraCtrl(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	bool OpenCarm(void);      // 打开相机
	void Carmlive();          // 连续采集     
	void StopCarm(void);      // 停止采集
	void CloseCarm(void);     // 停止并关闭相机
	void SetCarmParam(int P_AVTShutter,int P_AVTBrightness,int P_AVTAVTGain);// 设置相机参数

	void InitBitmap(UINT32 ImageWidth, UINT32 ImageHeight, BOOL IsColorCamera);  // 创建位图头文件
	void ShowBitmap(FGFRAME frame);// 显示位图

	// 拍照回调函数
	static void WINAPI SnapCallback(void* Context,UINT32 wParam,void* lParam);

public:
	// 摄像机对象
	CFGCamera m_avtCamera;

	// 采集线程
	HANDLE m_hSnapThread;
	DWORD m_dwThreadId;

	//保存AVI线程
	HANDLE m_hStoreAVIThread;
	DWORD  m_dwThreadIdAVI;

	int		m_AVTShutter;
	int     m_AVTGain;
	int     m_AVTPackageSize;
	int     m_AVTBrightness;
	int		m_AVTSnapMode;

	BOOL m_bIsColorCamera;     // 是否为彩色摄像机
	BOOL m_bLivingSnap;        // 当前是否在进行连续采集
	bool m_IsBayerImage;       // 

	CString strText;           // 显示的文字
	int     CurrentImageNum;
	RECT    rcOutVideo;

	// 摄像机当前AOI的宽高以及起始位置
	UINT32 m_nAOIWidth;
	UINT32 m_nAOIHeight;
	UINT32 m_nAOIX;
	UINT32 m_nAOIY;
	UINT32 m_TriggerValue;
	UINT32 m_nAOIStartX;
	UINT32 m_nAOIStartY;

	// 保存图像宽高值
	UINT32 m_nImageWidth;
	UINT32 m_nImageHeight;

	// 为要显示的图像分配内存区域以及位图信息头空间
	UINT8* m_pImageBuffer;
	UINT8* m_pBayerBuffer;
	BITMAPINFO* m_pBitmapInfo;  //位图指针

	// 开始保存AVI的判断标志	
	BOOL m_bStartAVI;
	int  BytesPerPixel;
	int  BytesPerPixel2;
	PAVIFILE	m_pFile;
	PAVISTREAM	m_ps;
	PAVISTREAM	m_psCompressed;
	int			m_nTimeFrame;
	HRESULT     hr;
};
