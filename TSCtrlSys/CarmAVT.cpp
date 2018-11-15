// CarmAVT.cpp : implementation file
//

#include "stdafx.h"
#include "TSCtrlSys.h"
#include "CarmAVT.h"

#pragma warning(disable: 4100)

CCarmAVT *g_Carm = NULL;
// CCarmAVT

IMPLEMENT_DYNAMIC(CCarmAVT, CWnd)

CCarmAVT::CCarmAVT()
{
	FGINIT Arg;
	memset(&Arg, 0, sizeof(FGINIT));
	Arg.pCallback = SnapCallback;
	Arg.Context = m_hWnd;

	UINT32 Result = FCE_NOERROR;
	Result = FGInitModule(&Arg);
	m_AVTShutter = 4000;        // �ع�ʱ���ʱֵ
	m_AVTGain = 70;             // ����ֵ��ʱֵ
	m_AVTPackageSize = 3500;    // ������ʼֵ
	m_AVTBrightness = 128;      // ���ȳ�ʼֵ
	m_nAOIWidth  = 0;
	m_nAOIHeight = 0;
	m_nAOIX = 0;
	m_nAOIY = 0;
	m_nImageWidth  = 0;
	m_nImageHeight = 0;	
	m_bLivingSnap  = FALSE;
	m_AVTSnapMode  = 0;
	m_bIsColorCamera = FALSE;
	m_nTimeFrame     = 0;
	m_pBayerBuffer = NULL;
	m_pImageBuffer = NULL;
	m_pBitmapInfo  = NULL;
	hr = S_OK;
	m_pFile			= NULL;
	m_ps			= NULL;
	m_psCompressed	= NULL;
	m_bStartAVI		= FALSE;

	g_Carm=this;
}

CCarmAVT::~CCarmAVT()
{
	CloseCarm();
}


BEGIN_MESSAGE_MAP(CCarmAVT, CWnd)
END_MESSAGE_MAP()

// ����� 
bool CCarmAVT::OpenCarm(void)
{
	UINT32 Result = FCE_NOERROR;
	FGNODEINFO NodeInfo[2];
	UINT32 NodeCnt;

	// ��ȡ������б�
	Result = FGGetNodeList(NodeInfo,2,&NodeCnt);
	if(Result != FCE_NOERROR )
	{
		MessageBox("û���ҵ������", _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// ���������
	Result=m_avtCamera.Connect(&NodeInfo[0].Guid, (void*)1);
	if(Result != FCE_NOERROR || Result != FCE_NOERROR)
	{
		MessageBox("���������ʧ��", _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	//�����������ͺ��ǲ���AVT��ɫ�����
	char* deviceName = new char[100];
	m_avtCamera.GetDeviceName(deviceName, 100);
	CString strDevName = deviceName;
	if(strDevName.Find("AVT") == 0)
	{
		if(strDevName.Find("C") == strDevName.GetLength() - 1)
		{
			m_bIsColorCamera = true;
			BytesPerPixel = 3;
		}
		else
		{
			m_bIsColorCamera = false;
			BytesPerPixel = 1;
		}
	}
	delete[] deviceName;

	//������������ͼ���ʽ������
	if(m_bIsColorCamera)
		Result = m_avtCamera.SetParameter(FGP_IMAGEFORMAT, MAKEIMAGEFORMAT(RES_SCALABLE, CM_RAW8, 0)); 
	else
		Result = m_avtCamera.SetParameter(FGP_IMAGEFORMAT, MAKEIMAGEFORMAT(RES_SCALABLE, CM_Y8, 0)); 
	if(Result != FCE_NOERROR )
	{
		MessageBox("��֧������ͼ���ʽ", _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	//��ȡ��ǰ�����1�ֱ�����Ϣ
	m_avtCamera.GetParameter(FGP_XSIZE, &m_nAOIWidth);  // 
	m_avtCamera.GetParameter(FGP_YSIZE, &m_nAOIHeight);
	m_avtCamera.GetParameter(FGP_XPOSITION, &m_nAOIX);
	m_avtCamera.GetParameter(FGP_YPOSITION, &m_nAOIY);

	//Ϊͼ�������ʴ�С�Ļ����������������ʵ�λͼͷ�ļ�
	InitBitmap(m_nAOIWidth, m_nAOIHeight, m_bIsColorCamera);

	//Ϊ�ɼ�����DMAͨ��
	m_avtCamera.SetParameter(FGP_USEIRMFORCHN, 0);
	m_avtCamera.SetParameter(FGP_ISOCHANNEL, 0);

	//���ð���Ϊ���
	FGPINFO pInfo;
	m_avtCamera.GetParameterInfo(FGP_PACKETSIZE, &pInfo);
	m_avtCamera.SetParameter(FGP_PACKETSIZE, pInfo.MaxValue);
	m_avtCamera.SetParameter(FGP_DMAMODE, DMA_REPLACE);

	//����ɼ��ڴ沢��ʼ�ɼ�
	Result = m_avtCamera.OpenCapture();
	if(Result != FCE_NOERROR )
	{
		MessageBox("OpenCapture Error", _T("Error"), MB_OK | MB_ICONEXCLAMATION);			
		m_avtCamera.Disconnect();

		return false;
	}
	Result = m_avtCamera.StartDevice();
	if(Result != FCE_NOERROR )
	{
		MessageBox("StartDevice Error", _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		m_avtCamera.CloseCapture();
		m_avtCamera.Disconnect();

		return false;
	}
	m_bLivingSnap = true;

	return true;
}

void CCarmAVT::Carmlive(void)
{
	m_avtCamera.StartDevice();
}

void CCarmAVT::ShowBitmap(FGFRAME frame)
{
	// ���û�з���ͼ�񻺳���
	if(!m_pImageBuffer)
		return;

	if(m_bIsColorCamera)    
	{
		// ��ͼ�����Bayerת��, Bayerת��֮�����ݴ����m_pBayerImage��
		ConvertRawY8(m_nImageWidth, m_nImageHeight, frame.pData, m_pBayerBuffer, 0);

		// Bayerת��δ���ͼ�����ݵ����·�ת����תͼ�񣬷���m_pImageBuffer��
		for ( unsigned int i = 0; i<m_nImageHeight; i++)
		{
			for ( unsigned int j = 0; j<m_nImageWidth*3; j+=3)
			{
				*( m_pImageBuffer + i * m_nImageWidth * 3 + j) = *( m_pBayerBuffer + ( m_nImageHeight - i -1 ) * m_nImageWidth * 3 + j) ;
				*( m_pImageBuffer + i * m_nImageWidth * 3 + j + 1) = *( m_pBayerBuffer + ( m_nImageHeight - i -1 ) * m_nImageWidth * 3 + j + 1) ;
				*( m_pImageBuffer + i * m_nImageWidth * 3 + j + 2) = *( m_pBayerBuffer + ( m_nImageHeight - i -1 ) * m_nImageWidth * 3 + j + 2) ;
			}
		}
	}
	else
	{
		// ֱ�ӽ���ͼ�����ݿ���
		memcpy(m_pBayerBuffer, frame.pData, frame.Length);

		for ( unsigned int i = 0; i<m_nImageHeight; i++)
		{
			for ( unsigned int j = 0; j<m_nImageWidth; j++)
			{
				*( m_pImageBuffer + i * m_nImageWidth + j) = *( m_pBayerBuffer + ( m_nImageHeight - i - 1 ) * m_nImageWidth + j ) ;
			}
		}
	}
}

void CCarmAVT::InitBitmap(UINT32 ImageWidth, UINT32 ImageHeight, BOOL IsColorCamera)
{
	m_nImageWidth = ImageWidth;
	m_nImageHeight = ImageHeight;
	if(m_pImageBuffer)
	{
		delete[] m_pImageBuffer;
		m_pImageBuffer = NULL;
	}

	if(m_pBayerBuffer)
	{
		delete[] m_pBayerBuffer;
		m_pBayerBuffer = NULL;
	}

	if(m_pBitmapInfo)
	{
		delete[] m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}

	if (BytesPerPixel == 1 ) 
	{	

		m_pBayerBuffer = new UINT8[m_nImageWidth * m_nImageHeight * BytesPerPixel + 1582];    
		m_pImageBuffer = new UINT8[m_nImageWidth * m_nImageHeight * BytesPerPixel + 1582];
	}
	else
	{

		m_pBayerBuffer = new UINT8[m_nImageWidth * m_nImageHeight * BytesPerPixel];    
		m_pImageBuffer = new UINT8[m_nImageWidth * m_nImageHeight * BytesPerPixel];
	}

	if (IsColorCamera)
	{
		m_pBitmapInfo = (BITMAPINFO*)(new char[sizeof(BITMAPINFOHEADER)]);
		m_pBitmapInfo->bmiHeader.biBitCount=(UINT16)(24);				
	}
	else
	{
		m_pBitmapInfo = (BITMAPINFO*)(new char[sizeof(BITMAPINFOHEADER) + 256*4]);
		m_pBitmapInfo->bmiHeader.biBitCount=(UINT16)(8);
	}

	m_pBitmapInfo->bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biWidth  = m_nImageWidth;
	m_pBitmapInfo->bmiHeader.biHeight = m_nImageHeight;
	m_pBitmapInfo->bmiHeader.biPlanes = 1;

	m_pBitmapInfo->bmiHeader.biCompression  = BI_RGB;
	m_pBitmapInfo->bmiHeader.biSizeImage    = m_nImageWidth * m_nImageHeight * BytesPerPixel;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter= 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter= 0;
	m_pBitmapInfo->bmiHeader.biClrUsed      = 0;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;	

	if(!IsColorCamera)
	{
		for(int i=0;i<256;i++)
		{
			m_pBitmapInfo->bmiColors[i].rgbRed=(UINT8)i;
			m_pBitmapInfo->bmiColors[i].rgbGreen=(UINT8)i;
			m_pBitmapInfo->bmiColors[i].rgbBlue=(UINT8)i;
			m_pBitmapInfo->bmiColors[i].rgbReserved=0;
		}
	}
}

void WINAPI CCarmAVT::SnapCallback(void* Context,UINT32 wParam,void* lParam)
{
	FGFRAME frame;
	UINT32 Result = 0;

	switch(wParam) {
		case WPARAM_ERROR:
			break;
		case WPARAM_FRAMESREADY:
			do
			{
				Result=g_Carm->m_avtCamera.GetFrame(&frame, 0);

				if(Result!=FCE_NOERROR)	      // Any error?
				{ 	
					break;	                  // Stop loop on error
				}

				// Only if <pData> is valid (ignore pure frame start events)
				if(frame.pData)
				{
					g_Carm->ShowBitmap(frame);
					g_Carm->m_avtCamera.PutFrame(&frame);
					g_pView->m_pImageBuffer=g_Carm->m_pImageBuffer;
					g_pView->Invalidate();
				}
			}while(!(frame.Flags&FGF_LAST));
			break;

		default:
			break;
	}
}

void CCarmAVT::CloseCarm(void)
{
	m_bLivingSnap = FALSE;
	m_avtCamera.StopDevice();	
	m_avtCamera.CloseCapture();
	m_avtCamera.Disconnect();

	FGExitModule();
	if(m_pImageBuffer)
	{
		delete[] m_pImageBuffer;
		m_pImageBuffer = NULL;
	}

	if(m_pBayerBuffer)
	{
		delete[] m_pBayerBuffer;
		m_pBayerBuffer = NULL;
	}

	if(m_pBitmapInfo)
	{
		delete[] m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
}

void CCarmAVT::SetCarmParam(int P_AVTShutter,int P_AVTBrightness,int P_AVTAVTGain)
{
	m_avtCamera.SetParameter(FGP_SHUTTER, P_AVTShutter);
	m_avtCamera.SetParameter(FGP_BRIGHTNESS, P_AVTBrightness);
	m_avtCamera.SetParameter(FGP_GAIN, P_AVTAVTGain);
}

void CCarmAVT::StopCarm(void)
{
	m_avtCamera.StopDevice();
}

