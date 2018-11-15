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
	 //	����������� 1
	 HVSTATUS status = STATUS_OK;
	 status = BeginHVDevice(1, &m_hHV);
	 HV_VERIFY(status);
	 //	���麯��ִ��״̬�����ʧ�ܣ��򷵻ش���״̬��Ϣ��

	 HVSetResolution(m_hHV, Resolution);	

	 HVSetSnapSpeed(m_hHV, HIGH_SPEED);

	 //	�ɼ�ģʽ������ CONTINUATION(����)��TRIGGER(�ⴥ��)
	 HVSetSnapMode(m_hHV, CONTINUATION);

	 //  ���ø�������������
	 for (int i = 0; i < 4; i++){
		 HVAGCControl(m_hHV, (BYTE)(RED_CHANNEL + i), nGain);
	 }
	 //  ����ADC�ļ���
	 HVADCControl(m_hHV, ADC_BITS, ADC_LEVEL2);
	 /*
	 *	��Ƶ������ڣ�����Ƶ�����Χ���������ȡֵ��Χ���������봰�ڷ�Χ���ڣ�
	 *  ��Ƶ�������Ͻ�X����ʹ��ڿ��ӦΪ4�ı��������Ͻ�Y����ʹ��ڸ߶�ӦΪ2�ı���
	 *	������ڵ���ʼλ��һ������Ϊ(0, 0)���ɡ�
	 */

	 if(nWidth<=0) m_OutSizeX=IMAGE_WIDTH; else m_OutSizeX=nWidth;
	  if(nHeight<=0) m_OutSizeY=IMAGE_HEIGHT; else m_OutSizeY=nHeight;
 	 HVSetOutputWindow(m_hHV, nStartX, nStartY, m_OutSizeX, m_OutSizeY);

	 //	m_pBmpInfo��ָ��m_chBmpBuf���������û������Լ�����BTIMAPINFO������	
	 m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	 //	��ʼ��BITMAPINFO �ṹ���˽ṹ�ڱ���bmp�ļ�����ʾ�ɼ�ͼ��ʱʹ��
	 m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	 //	ͼ���ȣ�һ��Ϊ������ڿ��
	 m_pBmpInfo->bmiHeader.biWidth			= m_OutSizeX;
	 //	ͼ���ȣ�һ��Ϊ������ڸ߶�
	 m_pBmpInfo->bmiHeader.biHeight			= m_OutSizeY;
	 /*
	 *	��������һ����ͬ��
	 *	���ڵ���8λ��λͼ����Ӧ������Ӧ��λͼ��ɫ��
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
	 *	����ԭʼͼ�񻺳�����һ�������洢�ɼ�ͼ��ԭʼ����
	 *  һ��ͼ�񻺳�����С��������ڴ�С����Ƶ��ʽȷ����
	 */
	 m_pRawBuffer = new BYTE[m_OutSizeX * m_OutSizeY];
	 ASSERT(m_pRawBuffer);

	 /*
	 ����Bayerת����ͼ�����ݻ���
	 */
	 m_pImageBuffer = new BYTE[m_OutSizeX * m_OutSizeY * 3];
	 ASSERT(m_pImageBuffer);

	 /*
	 *	��ʼ������������ɼ�ͼ���ڴ�Ŀ��ƣ�
	 *	ָ���ص�����SnapThreadCallback��thisָ��
	 */
	 status = HVOpenSnap(m_hHV, SnapThreadCallback, this);					
	 HV_VERIFY(status);
	 if (HV_SUCCESS(status)) {
		 m_bOpen = TRUE;		//��־�Ѿ���Snap����
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
	*	��ֹ����������ɼ�ͼ���ڴ棬ͬʱ�ͷ����вɼ�������
	*	�ٴ���������������ɼ����������³�ʼ��	
	*/
	if (m_bOpen) {
		HVCloseSnap(m_hHV);
	}
	HV_VERIFY(status);

	if (HV_SUCCESS(status)) {
		m_bOpen		= FALSE;
		m_bStart	= FALSE;
	}

	//	�ر�������������ͷ�����������ڲ���Դ
	status = EndHVDevice(m_hHV);
	HV_VERIFY(status);

	//	����ͼ�񻺳���
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
	*	��������������ɼ�ͼ���ڴ�
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

	//	ֹͣ�ɼ�ͼ���ڴ棬�����ٴε���HVStartSnapEx��������������ɼ�
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