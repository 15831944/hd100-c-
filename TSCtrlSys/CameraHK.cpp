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
// �����̬��Ա����
int CCameraHK::m_nCamNum = 0;			// �ɹ���ʼ���������
bool CCameraHK::m_bCamSysInit = false;	// ���ϵͳ��ʼ��
MV_CC_DEVICE_INFO_LIST CCameraHK::m_stDeviceList;	// �豸�б�ṹ�����
// end �����̬��Ա����
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
	m_nCamIndex = -1;	// ��ǰ������
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
BOOL CCameraHK::InitCamera()			// ��ʼ�����ϵͳ
{
	if (true == m_bCamSysInit)	return TRUE;	// �Ѿ��ɹ���ʼ��

	memset(&m_stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));	// ����豸�б�  
	BOOL bRet = MV_CC_EnumDevices(MV_GIGE_DEVICE, &m_stDeviceList);// ����ö���豸�ӿڣ� ö�ٵ�Ŀ���豸����ΪGIGE�� ö�ٵ����豸��ŵ��豸�б�stDeviceList��
	if (MV_OK == bRet)										// ���ö���豸�ӿں���������ȷ
	{
		m_nCamNum = m_stDeviceList.nDeviceNum;
		m_bCamSysInit = true;
		return TRUE;
	}
	else
	{
		AfxMessageBox(_T("���ö��ʧ��!"));
		m_nCamNum = 0;
		m_bCamSysInit = false;
		return FALSE;
	}
}


BOOL CCameraHK::SetTriggerMode(bool bTriggerMode)		// ����ΪTriggerģʽ  bTriggerMode  true:����ΪTriggerģʽ   false:�����ɼ�ģʽ
{
	if (bTriggerMode)
	{
		//ֹͣ�ɼ�ͼ�� 
// 		m_nRet = MV_CC_StopGrabbing(m_pHandle);
// 		if (MV_OK != m_nRet)
// 		{
// 			printf("error: StopGrabbing fail [%x]\n", m_nRet);
// 			return FALSE;
// 		}

		m_nRet = MV_CC_SetTriggerMode(m_pHandle, MV_TRIGGER_MODE_ON);        // ��������ģʽ		
		if (MV_OK != m_nRet)
		{
			printf("��������ģʽʧ��!\n");
			return FALSE;
		}
		
		m_nRet = MV_CC_SetTriggerSource(m_pHandle, MV_TRIGGER_SOURCE_SOFTWARE);  // ���ô���ԴΪ����		
		if (MV_OK != m_nRet)
		{
			printf("��������ģʽʧ��!\n");
			return FALSE;
		}
		
// 		while (MV_OK == MV_CC_GetOneFrameTimeout(m_pHandle, m_pImageData, m_nImageSize, &stImageInfo, 1000))	// ץȡһ֡ͼ��Buffer 
// 		{
// 			// �������ͷ�е�ͼ�񻺴�
// 		}

		//��ʼ�ɼ�ͼ�� 
// 		m_nRet = MV_CC_StartGrabbing(m_pHandle);
// 		if (MV_OK != m_nRet)
// 		{
// 			printf("error: StopGrabbing fail [%x]\n", m_nRet);
// 			return FALSE;
// 		}
	}
	else
	{
		//ֹͣ�ɼ�ͼ�� 
// 		m_nRet = MV_CC_StopGrabbing(m_pHandle);
// 		if (MV_OK != m_nRet)
// 		{
// 			printf("error: StopGrabbing fail [%x]\n", m_nRet);
// 			return FALSE;
// 		}

		m_nRet = MV_CC_SetTriggerMode(m_pHandle, MV_TRIGGER_MODE_OFF);        // �ر�����ģʽ		
		if (MV_OK != m_nRet)
		{
			printf("����Ϊ����ģʽʧ��!\n");
			return FALSE;
		}

		//��ʼ�ɼ�ͼ�� 
// 		m_nRet = MV_CC_StartGrabbing(m_pHandle);
// 		if (MV_OK != m_nRet)
// 		{
// 			printf("error: StopGrabbing fail [%x]\n", m_nRet);
// 			return FALSE;
// 		}
	}

	return TRUE;
}
BOOL CCameraHK::SoftTrigger()		// ��������
{	
	if (!m_bCameraInitFlag)
	{
		TRACE(_T("û�гɹ���ʼ��,����ʧ��!\n"));
		return FALSE;
	}

	m_bTranseferEnd = false;
	m_nRet = MV_CC_TriggerSoftwareExecute(m_pHandle);                        // ִ��һ������	
	if (MV_OK != m_nRet)
	{
		TRACE("ִ��һ������ʧ��!\n");
		return FALSE;                    
	}

	return TRUE;
}

BOOL CCameraHK::OpenCamera(CString camName, HWND hWndDisplay, LPBYTE& lpDesIMGAddress, int nWidth, int nHeight, int nDepth)	// �����
{	
	UNREFERENCED_PARAMETER(nDepth);
	if (m_bCameraInitFlag)	return TRUE;
	if (m_stDeviceList.nDeviceNum < 1)
	{
		AfxMessageBox("δ�ҵ������");
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
			memset(&m_stDeviceInfo, 0, sizeof(MV_CC_DEVICE_INFO));    // ����ǰ�豸��Ϣ�ṹ�������ճ�ʼ��		
			memcpy(&m_stDeviceInfo, m_stDeviceList.pDeviceInfo[m_nCamIndex], sizeof(MV_CC_DEVICE_INFO)); // ���豸�б�ĵ�i���豸�洢����
			break;
		}
	}

	if (-1 == m_nCamIndex)
	{
		CString tmpStr;
		tmpStr = "���: " + camName + "δ��ʼ���ɹ�";
		//theApp.SetStartDlgText(tmpStr);
		AfxMessageBox(tmpStr);
		return FALSE;
	}

	m_nRet = MV_CC_CreateHandle(&m_pHandle, &m_stDeviceInfo);		// ��ʱhandle��stDeviceInfo�󶨣��Ժ��handle��������stDeviceInfo
	if (MV_OK == m_nRet)											// �����������ɹ�
	{  	
		//////////////////////////////////////////////////////////////////////////
		m_nRet = MV_CC_RegisterImageCallBack(m_pHandle, ImageCallBack, this);
		if (MV_OK != m_nRet)
		{
			AfxMessageBox("ע�����ݻص�ʧ��!\n");
			return FALSE;
		}		

		m_nRet = MV_CC_OpenDevice(m_pHandle);						// ���豸��ֻҪ����һ���������, ��Ϊ����Ѿ����豸��Ϣ����һ����
		if (MV_OK == m_nRet)										// ���豸�ɹ�
		{	
			if (!SetTriggerMode(false))
			{
				AfxMessageBox("����Triggerģʽʧ��!");
				return FALSE;
			}

			m_nRet = MV_CC_StartGrabbing(m_pHandle);            // ����ץͼ��ֻҪ����һ���������, ��Ϊ����Ѿ����豸��Ϣ����һ����
			if (MV_OK != m_nRet)
			{
				strTemp.Format(_T("���:%d����ץͼʧ��"), m_nCamIndex);
				AfxMessageBox(strTemp);
				return FALSE;
			}
		}
		else
		{
			strTemp.Format(_T("���:%d��ʧ��"), m_nCamIndex);
			AfxMessageBox(strTemp);
			return FALSE;
		}
	}
	else
	{
		strTemp.Format(_T("���:%d�������ʧ��!"), m_nCamIndex);
		AfxMessageBox(strTemp);
		return FALSE;
	}

	// 6.��ȡͼ���С
	MVCC_INTVALUE stIntvalue = {0};
	m_nRet = MV_CC_GetIntValue(m_pHandle, "PayloadSize", &stIntvalue);
	if (m_nRet != MV_OK)
	{
		strTemp.Format(_T("Get PayloadSize failed! nRet [%x]\n"), m_nRet);
		return FALSE;
	}

	m_nImageSize = stIntvalue.nCurValue + 2048;  // һ֡���ݴ�С + Ԥ��2048byte����SDK�ڲ�����

	if (NULL != m_pImageData)
	{
		delete []m_pImageData;
		m_pImageData = NULL;
	}
	m_pImageData = new BYTE[m_nImageSize];
	lpDesIMGAddress = m_pImageData;

	m_bCameraInitFlag = true;	// �����ʼ���ɹ�
	return TRUE;
}

BOOL CCameraHK::CloseCamera()			// �ر����
{

	//��ֹͣץͼ
	//���ͣ�	��Ȼֻ����һ��handle���ɳɹ�ֹͣץͼ����û������������
	int nRtn = MV_CC_StopGrabbing(m_pHandle);		

	//�߹ر��豸
	//���ͣ�	��Ȼֻ����һ��handle���ɳɹ��ر��豸
	nRtn = MV_CC_CloseDevice(m_pHandle);		

	//�����پ��
	//���ͣ�	��Ȼֻ����һ��handle�������پ��
	nRtn = MV_CC_DestroyHandle(m_pHandle);

	Sleep(100);

	if (NULL != m_pImageData)
	{
		delete []m_pImageData;
		m_pImageData = NULL;
	}
	return TRUE;
}

BOOL CCameraHK::GrabOneFrame()		// MV_CC_GetOneFrameTimeout ��ȡһ֡ͼ��
{
	int nRet = 0;
	while (MV_OK == MV_CC_GetOneFrameTimeout(m_pHandle, m_pImageData, m_nImageSize, &stImageInfo, 1000))
	{
		::PostMessage( m_hWndShowImage, WM_REDRAWIMAGEFROMCCD , NULL, NULL );

		unsigned char* pBufForSaveImage = NULL;
		pBufForSaveImage = new unsigned char[m_nImageSize];
		//���ö�Ӧ���������
		MV_SAVE_IMAGE_PARAM_EX stParam;
		memset(&stParam, 0, sizeof(MV_SAVE_IMAGE_PARAM_EX));
		stParam.enImageType = MV_Image_Bmp; //��Ҫ�����ͼ������
		stParam.enPixelType = stImageInfo.enPixelType;  //�����Ӧ�����ظ�ʽ
		stParam.nBufferSize = m_nImageSize;  //�洢�ڵ�Ĵ�С
		stParam.nWidth      = stImageInfo.nWidth;         //�����Ӧ�Ŀ�
		stParam.nHeight     = stImageInfo.nHeight;          //�����Ӧ�ĸ�
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

BOOL CCameraHK::WaitGrabOneFrame()	// ��ȡһ֡ͼ��,ͼ�����ݱ�����buf��
{      
	if ( !m_bCameraInitFlag)	{
		AfxMessageBox(_T("�����ʼ��δ�ɹ�!"));
		return FALSE;
	}
	if (NULL == m_pImageData)	{
		AfxMessageBox(_T("��ȡͼ��ʱͼ��bufΪ��!"));
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
	return FALSE;	// ��ʱ����ʧ��

// 	m_nRet = MV_CC_GetOneFrame(m_pHandle, m_pImageData, MAX_FRAME_SIZE, &stMVFrameInfo);  // ץȡһ֡ͼ��Buffer            
// 	if (m_nRet != MV_OK)
// 		return FALSE;
// 	else
// 		return TRUE;
}

BOOL CCameraHK::SetGain(double gain)	// ��������ͷ����
{
	MV_CC_SetGain(m_pHandle, (float)gain);
	return TRUE;
}

BOOL CCameraHK::SetExp(long expUs)	// �����ع�ʱ�� ��λ:us
{
	MV_CC_SetExposureTime(m_pHandle, (float)expUs);
	return TRUE;
}