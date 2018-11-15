#include "stdafx.h"
#include "KSvdhEx.h"
#include "TSCtrlSys.h"


#pragma comment(lib,"hvdh/lib/Raw2Rgb.lib")
#pragma comment(lib,"hvdh/lib/HVDAILT.lib")
#pragma comment(lib,"hvdh/lib/HVUtil.lib")

const int IMAGE_WIDTH=1280;
const int IMAGE_HEIGHT=1024;

CKSVDHEx::CKSVDHEx()
{

	m_bOpen			= FALSE;
	m_bStart		= FALSE;
	m_pBmpInfo		= NULL;
	m_pRawBuffer	= NULL;
	m_pImageBuffer	= NULL;
	for(int i=0;i<256;i++)
	{
		m_pLutR[i] = (BYTE)i;
		m_pLutG[i] = (BYTE)i;
		m_pLutB[i] = (BYTE)i;
	}

}
CKSVDHEx::~CKSVDHEx()
{
}

BOOL CKSVDHEx::InitDH(int iIndexNo, long nUperFocus, long nDownFocus, 
			long nGain, int nStartX, int nStartY, int nWidth, int nHeight,
			HV_RESOLUTION Resolution, BOOL bRGB24)
 {
	 UNREFERENCED_PARAMETER(bRGB24);
	 UNREFERENCED_PARAMETER(nDownFocus);
	 UNREFERENCED_PARAMETER(nUperFocus);


	 HVGetDeviceTotal(&m_nTotal);
	 if(m_nTotal<=0)  return FALSE;
	 //	打开数字摄像机 1
	 HVSTATUS status = STATUS_OK;
	 status = BeginHVDevice(1, &m_hHV);
	 HV_VERIFY(status);
	 //	检验函数执行状态，如果失败，则返回错误状态消息框

	 HVSetResolution(m_hHV, Resolution);	

	 HVSetSnapSpeed(m_hHV, HIGH_SPEED);

	 //	采集模式，包括 CONTINUATION(连续)、TRIGGER(外触发)
	 HVSetSnapMode(m_hHV, CONTINUATION);

	 //  设置各个分量的增益
	 for (int i = 0; i < 4; i++){
		 HVAGCControl(m_hHV, (BYTE)(RED_CHANNEL + i), nGain);
	 }
	 //  设置ADC的级别
	 HVADCControl(m_hHV, ADC_BITS, ADC_LEVEL2);
	 /*
	 *	视频输出窗口，即视频输出范围，输出窗口取值范围必须在输入窗口范围以内，
	 *  视频窗口左上角X坐标和窗口宽度应为4的倍数，左上角Y坐标和窗口高度应为2的倍数
	 *	输出窗口的起始位置一般设置为(0, 0)即可。
	 */

	 if(nWidth<=0) m_OutSizeX=IMAGE_WIDTH; else m_OutSizeX=nWidth;
	  if(nHeight<=0) m_OutSizeY=IMAGE_HEIGHT; else m_OutSizeY=nHeight;
 	 HVSetOutputWindow(m_hHV, nStartX, nStartY, m_OutSizeX, m_OutSizeY);

	 //	m_pBmpInfo即指向m_chBmpBuf缓冲区，用户可以自己分配BTIMAPINFO缓冲区	
	 m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	 //	初始化BITMAPINFO 结构，此结构在保存bmp文件、显示采集图像时使用
	 m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	 //	图像宽度，一般为输出窗口宽度
	 m_pBmpInfo->bmiHeader.biWidth			= m_OutSizeX;
	 //	图像宽度，一般为输出窗口高度
	 m_pBmpInfo->bmiHeader.biHeight			= m_OutSizeY;
	 /*
	 *	以下设置一般相同，
	 *	对于低于8位的位图，还应设置相应的位图调色板
	 */
	 m_pBmpInfo->bmiHeader.biPlanes			= 1;
	 m_pBmpInfo->bmiHeader.biBitCount		= 24;
	 m_pBmpInfo->bmiHeader.biCompression	= BI_RGB;
	 m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	 m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	 m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	 m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	 m_pBmpInfo->bmiHeader.biClrImportant	= 0;
	 /*
	 *	分配原始图像缓冲区，一般用来存储采集图像原始数据
	 *  一般图像缓冲区大小由输出窗口大小和视频格式确定。
	 */
	 m_pRawBuffer = new BYTE[m_OutSizeX * m_OutSizeY];
	 ASSERT(m_pRawBuffer);

	 /*
	 分配Bayer转换后图像数据缓冲
	 */
	 m_pImageBuffer = new BYTE[m_OutSizeX * m_OutSizeY * 3];
	 ASSERT(m_pImageBuffer);

	 /*
	 *	初始化数字摄像机采集图像到内存的控制，
	 *	指定回调函数SnapThreadCallback和this指针
	 */
	 status = HVOpenSnap(m_hHV, SnapThreadCallback, this);					
	 HV_VERIFY(status);
	 if (HV_SUCCESS(status)) {
		 m_bOpen = TRUE;		//标志已经打开Snap环境
	 }
	 m_structKSVDH.iIndexNo=iIndexNo;
	 m_structKSVDH.hhv=m_hHV;
	 return m_bOpen;

 }
void CKSVDHEx::FreeDH()
{
	if(m_nTotal<=0)  return;
	HVSTATUS status = STATUS_OK;
	/*
	*	终止数字摄像机采集图像到内存，同时释放所有采集环境，
	*	再次启动数字摄像机采集，必须重新初始化	
	*/
	if (m_bOpen) {
		HVCloseSnap(m_hHV);
	}
	HV_VERIFY(status);

	if (HV_SUCCESS(status)) {
		m_bOpen		= FALSE;
		m_bStart	= FALSE;
	}

	//	关闭数字摄像机，释放数字摄像机内部资源
	status = EndHVDevice(m_hHV);
	HV_VERIFY(status);

	//	回收图像缓冲区
	delete []m_pRawBuffer;
	m_pRawBuffer = NULL;
	delete []m_pImageBuffer;
	m_pImageBuffer = NULL;

}
void CKSVDHEx::Live()
{
	if(m_nTotal<=0)  return;
	if(m_bStart) return;
	/*
	*	启动数字摄像机采集图像到内存
	*/
	HVSTATUS status = STATUS_OK;
	BYTE *ppBuf[1];
	ppBuf[0] = m_pRawBuffer;
	status = HVStartSnap(m_hHV, ppBuf,1);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) {
		m_bStart = TRUE;
	}
}
void CKSVDHEx::Stop()
{
	 if(m_nTotal<=0)  return ;
	if(!m_bStart) return;
	HVSTATUS status =STATUS_OK;

	//	停止采集图像到内存，可以再次调用HVStartSnapEx启动数字摄像机采集
	status = HVStopSnap(m_hHV);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) {
		m_bStart = FALSE;
	}
}
int CALLBACK CKSVDHEx::SnapThreadCallback(HV_SNAP_INFO *pInfo)
{
	CKSVDHEx *This = (CKSVDHEx *)(pInfo->pParam);

	HV_ARG_GET_LAST_STATUS ArgFeature;
	ArgFeature.type = HV_LAST_STATUS_TRANSFER;

	HV_RES_GET_LAST_STATUS ResFeature;

	HVAPI_CONTROL_PARAMETER  p;
	p.pInBuf		 = &ArgFeature;
	p.dwInBufSize	 = sizeof(ArgFeature);	
	p.pOutBuf		 = &ResFeature;
	p.dwOutBufSize	 = sizeof(ResFeature);
	p.pBytesRet		 = NULL;		
	p.code			 = ORD_GET_LAST_STATUS;
	int	dwSize = sizeof(p);

	HVSTATUS status = HVCommand(This->m_hHV, CMD_HVAPI_CONTROL, &p, &dwSize);
	if(status != STATUS_OK)
	{
		TRACE("HVCommand return status is %d", status);
	}

	if (HV_SUCCESS(status)) 
	{
		ConvertBayer2Rgb(This->m_pImageBuffer,This->m_pRawBuffer,This->m_OutSizeX,This->m_OutSizeY,BAYER2RGB_NEIGHBOUR,This->m_pLutR,This->m_pLutG,This->m_pLutB,true,BAYER_GR);
		This->CallBack(This->m_structKSVDH );
	}
	return 1;
}