// HKCamera.cpp: implementation of the CHKCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TSCtrlSys.h"

#include "CameraHK.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int IMG_WIDTH	= 1280;
const int IMG_HEIGHT = 1024;
#define MAX_FRAME_SIZE      (IMG_WIDTH * IMG_HEIGHT * 16)

//////////////////////////////////////////////////////////////////////////
// 相机静态成员变量
int CCameraHK::m_nCamNum = 0;			// 成功初始化相机总数
bool CCameraHK::m_bCamSysInit = false;	// 相机系统初始化
MV_CC_DEVICE_INFO_LIST CCameraHK::m_stDeviceList;	// 设备列表结构体变量
// end 相机静态成员变量
//////////////////////////////////////////////////////////////////////////

CCriticalSection g_criticalImageBuf;
void __stdcall ImageCallBack(unsigned char * pData, MV_FRAME_OUT_INFO* pFrameInfo, void* pUser)
{
    if (pFrameInfo)
    {
		Sleep(10);
		CCameraHK* pCamera = (CCameraHK*)pUser;
//		g_criticalImageBuf.Lock();
		memcpy(pCamera->m_pImageData, pData, pCamera->m_nImageSize);
		pCamera->m_bTranseferEnd = true;
//		g_criticalImageBuf.Unlock();
		Sleep(50);
		
		// Redraw the image.
		::PostMessage( pCamera->m_hWndShowImage, WM_REDRAWIMAGEFROMCCD , NULL, NULL );
    }
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCameraHK::CCameraHK()
{
	m_nCamIndex = -1;	// 当前相机序号
	m_bCameraInitFlag = false;
	m_bTranseferEnd = false;
	m_pImageData = NULL;
	memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

	InitCamera();
}

CCameraHK::~CCameraHK()
{
	CloseCamera();
}

//////////////////////////////////////////////////////////////////////////
BOOL CCameraHK::InitCamera()			// 初始化相机系统
{
	if (true == m_bCamSysInit)	return TRUE;	// 已经成功初始化

	memset(&m_stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));	// 清空设备列表  
	BOOL bRet = MV_CC_EnumDevices(MV_GIGE_DEVICE, &m_stDeviceList);// 调用枚举设备接口， 枚举的目标设备类型为GIGE， 枚举到的设备存放到设备列表stDeviceList中
	if (MV_OK == bRet)										// 如果枚举设备接口函数返回正确
	{
		m_nCamNum = m_stDeviceList.nDeviceNum;
		m_bCamSysInit = true;
		return TRUE;
	}
	else
	{
		AfxMessageBox(_T("相机枚举失败!"));
		m_nCamNum = 0;
		m_bCamSysInit = false;
		return FALSE;
	}
}


BOOL CCameraHK::SetTriggerMode(bool bTriggerMode)		// 设置为Trigger模式  bTriggerMode  true:设置为Trigger模式   false:连续采集模式
{
	if (bTriggerMode)
	{
		//停止采集图像 
// 		m_nRet = MV_CC_StopGrabbing(m_pHandle);
// 		if (MV_OK != m_nRet)
// 		{
// 			printf("error: StopGrabbing fail [%x]\n", m_nRet);
// 			return FALSE;
// 		}

		m_nRet = MV_CC_SetTriggerMode(m_pHandle, MV_TRIGGER_MODE_ON);        // 开启软触发模式		
		if (MV_OK != m_nRet)
		{
			printf("开启触发模式失败!\n");
			return FALSE;
		}
		
		m_nRet = MV_CC_SetTriggerSource(m_pHandle, MV_TRIGGER_SOURCE_SOFTWARE);  // 设置触发源为软触发		
		if (MV_OK != m_nRet)
		{
			printf("设置软触发模式失败!\n");
			return FALSE;
		}
		
// 		while (MV_OK == MV_CC_GetOneFrameTimeout(m_pHandle, m_pImageData, m_nImageSize, &stImageInfo, 1000))	// 抓取一帧图像Buffer 
// 		{
// 			// 清空摄像头中的图像缓存
// 		}

		//开始采集图像 
// 		m_nRet = MV_CC_StartGrabbing(m_pHandle);
// 		if (MV_OK != m_nRet)
// 		{
// 			printf("error: StopGrabbing fail [%x]\n", m_nRet);
// 			return FALSE;
// 		}
	}
	else
	{
		//停止采集图像 
// 		m_nRet = MV_CC_StopGrabbing(m_pHandle);
// 		if (MV_OK != m_nRet)
// 		{
// 			printf("error: StopGrabbing fail [%x]\n", m_nRet);
// 			return FALSE;
// 		}

		m_nRet = MV_CC_SetTriggerMode(m_pHandle, MV_TRIGGER_MODE_OFF);        // 关闭软触发模式		
		if (MV_OK != m_nRet)
		{
			printf("设置为连续模式失败!\n");
			return FALSE;
		}

		//开始采集图像 
// 		m_nRet = MV_CC_StartGrabbing(m_pHandle);
// 		if (MV_OK != m_nRet)
// 		{
// 			printf("error: StopGrabbing fail [%x]\n", m_nRet);
// 			return FALSE;
// 		}
	}

	return TRUE;
}
BOOL CCameraHK::SoftTrigger()		// 发送软触发
{	
	if (!m_bCameraInitFlag)
	{
		TRACE(_T("没有成功初始化,软触发失败!\n"));
		return FALSE;
	}

	m_bTranseferEnd = false;
	m_nRet = MV_CC_TriggerSoftwareExecute(m_pHandle);                        // 执行一次软触发	
	if (MV_OK != m_nRet)
	{
		TRACE("执行一次软触发失败!\n");
		return FALSE;                    
	}

	return TRUE;
}

BOOL CCameraHK::OpenCamera(CString camName, HWND hWndDisplay, LPBYTE& lpDesIMGAddress, int nWidth, int nHeight, int nDepth)	// 打开相机
{	
	UNREFERENCED_PARAMETER(nDepth);
	if (m_bCameraInitFlag)	return TRUE;
	if (m_stDeviceList.nDeviceNum < 1)
	{
		AfxMessageBox("未找到相机！");
		return FALSE;
	}

	CString strTemp;

	m_hWndShowImage = hWndDisplay;
	m_nWidth = nWidth;	
	m_nHeight = nHeight;

	unsigned int i=0;
	for (i=0; i<m_stDeviceList.nDeviceNum; i++)
	{
		strTemp.Format("%s", m_stDeviceList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chUserDefinedName);
		if (strTemp == camName)
		{
			m_nCamIndex = i;
			memset(&m_stDeviceInfo, 0, sizeof(MV_CC_DEVICE_INFO));    // 将当前设备信息结构体变量清空初始化		
			memcpy(&m_stDeviceInfo, m_stDeviceList.pDeviceInfo[m_nCamIndex], sizeof(MV_CC_DEVICE_INFO)); // 将设备列表的第i个设备存储下来
			break;
		}
	}

	if (-1 == m_nCamIndex)
	{
		CString tmpStr;
		tmpStr = "相机: " + camName + "未初始化成功";
		//theApp.SetStartDlgText(tmpStr);
		AfxMessageBox(tmpStr);
		return FALSE;
	}

	m_nRet = MV_CC_CreateHandle(&m_pHandle, &m_stDeviceInfo);		// 此时handle与stDeviceInfo绑定，以后的handle将会代表此stDeviceInfo
	if (MV_OK == m_nRet)											// 如果创建句柄成功
	{  	
		//////////////////////////////////////////////////////////////////////////
		m_nRet = MV_CC_RegisterImageCallBack(m_pHandle, ImageCallBack, this);
		if (MV_OK != m_nRet)
		{
			AfxMessageBox("注册数据回调失败!\n");
			return FALSE;
		}		

		m_nRet = MV_CC_OpenDevice(m_pHandle);						// 打开设备，只要传入一个句柄即可, 因为句柄已经和设备信息绑定在一起了
		if (MV_OK == m_nRet)										// 打开设备成功
		{	
			if (!SetTriggerMode(false))
			{
				AfxMessageBox("设置Trigger模式失败!");
				return FALSE;
			}

			m_nRet = MV_CC_StartGrabbing(m_pHandle);            // 开启抓图，只要传入一个句柄即可, 因为句柄已经和设备信息绑定在一起了
			if (MV_OK != m_nRet)
			{
				strTemp.Format(_T("相机:%d开启抓图失败"), m_nCamIndex);
				AfxMessageBox(strTemp);
				return FALSE;
			}
		}
		else
		{
			strTemp.Format(_T("相机:%d打开失败"), m_nCamIndex);
			AfxMessageBox(strTemp);
			return FALSE;
		}
	}
	else
	{
		strTemp.Format(_T("相机:%d创建句柄失败!"), m_nCamIndex);
		AfxMessageBox(strTemp);
		return FALSE;
	}

	// 6.获取图像大小
	MVCC_INTVALUE stIntvalue = {0};
	m_nRet = MV_CC_GetIntValue(m_pHandle, "PayloadSize", &stIntvalue);
	if (m_nRet != MV_OK)
	{
		strTemp.Format(_T("Get PayloadSize failed! nRet [%x]\n"), m_nRet);
		return FALSE;
	}

	m_nImageSize = stIntvalue.nCurValue + 2048;  // 一帧数据大小 + 预留2048byte用于SDK内部处理

	if (NULL != m_pImageData)
	{
		delete []m_pImageData;
		m_pImageData = NULL;
	}
	m_pImageData = new BYTE[m_nImageSize];
	lpDesIMGAddress = m_pImageData;

	m_bCameraInitFlag = true;	// 相机初始化成功
	return TRUE;
}

BOOL CCameraHK::CloseCamera()			// 关闭相机
{

	//⑥停止抓图
	//解释：	依然只输入一个handle即可成功停止抓图，便没有数据流动了
	int nRtn = MV_CC_StopGrabbing(m_pHandle);		

	//⑦关闭设备
	//解释：	依然只输入一个handle即可成功关闭设备
	nRtn = MV_CC_CloseDevice(m_pHandle);		

	//⑧销毁句柄
	//解释：	依然只输入一个handle即可销毁句柄
	nRtn = MV_CC_DestroyHandle(m_pHandle);

	Sleep(100);

	if (NULL != m_pImageData)
	{
		delete []m_pImageData;
		m_pImageData = NULL;
	}
	return TRUE;
}

BOOL CCameraHK::GrabOneFrame()		// MV_CC_GetOneFrameTimeout 获取一帧图像
{
	int nRet = 0;
	while (MV_OK == MV_CC_GetOneFrameTimeout(m_pHandle, m_pImageData, m_nImageSize, &stImageInfo, 1000))
	{
		::PostMessage( m_hWndShowImage, WM_REDRAWIMAGEFROMCCD , NULL, NULL );

		unsigned char* pBufForSaveImage = NULL;
		pBufForSaveImage = new unsigned char[m_nImageSize];
		//设置对应的相机参数
		MV_SAVE_IMAGE_PARAM_EX stParam;
		memset(&stParam, 0, sizeof(MV_SAVE_IMAGE_PARAM_EX));
		stParam.enImageType = MV_Image_Bmp; //需要保存的图像类型
		stParam.enPixelType = stImageInfo.enPixelType;  //相机对应的像素格式
		stParam.nBufferSize = m_nImageSize;  //存储节点的大小
		stParam.nWidth      = stImageInfo.nWidth;         //相机对应的宽
		stParam.nHeight     = stImageInfo.nHeight;          //相机对应的高
		stParam.nDataLen    = stImageInfo.nFrameLen;
		stParam.pData       = m_pImageData;
		stParam.pImageBuffer = pBufForSaveImage;
		stParam.nJpgQuality = 80;

		nRet = MV_CC_SaveImageEx(&stParam);
		if(MV_OK != nRet)
		{
			printf("failed in MV_CC_SaveImage,nRet[%x]\n", nRet);
		}
		char pImageName[32] = {0};
		sprintf_s(pImageName, 32, "%03d.bmp", stImageInfo.nFrameNum);
		FILE* fp = fopen(pImageName, "wb");
		fwrite(pBufForSaveImage, 1, stParam.nImageLen, fp);
		fclose(fp);
		delete []pBufForSaveImage;
	}
	return TRUE;
}

BOOL CCameraHK::WaitGrabOneFrame()	// 获取一帧图像,图像数据保存在buf中
{      
	if ( !m_bCameraInitFlag)	{
		AfxMessageBox(_T("相机初始化未成功!"));
		return FALSE;
	}
	if (NULL == m_pImageData)	{
		AfxMessageBox(_T("获取图像时图像buf为空!"));
		return FALSE;
	}

	DWORD st = GetTickCount();
	while (GetTickCount()-st < 1000)
	{
		if (m_bTranseferEnd)	
		{
			return TRUE;
		}
		
		Sleep(5);
	}
	return FALSE;	// 超时返回失败

// 	m_nRet = MV_CC_GetOneFrame(m_pHandle, m_pImageData, MAX_FRAME_SIZE, &stMVFrameInfo);  // 抓取一帧图像Buffer            
// 	if (m_nRet != MV_OK)
// 		return FALSE;
// 	else
// 		return TRUE;
}

BOOL CCameraHK::SetGain(double gain)	// 设置摄像头增益
{
	MV_CC_SetGain(m_pHandle, (float)gain);
	return TRUE;
}

BOOL CCameraHK::SetExp(long expUs)	// 设置曝光时间 单位:us
{
	MV_CC_SetExposureTime(m_pHandle, (float)expUs);
	return TRUE;
}