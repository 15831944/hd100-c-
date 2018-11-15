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
	// ����������ñ���
	static int m_nCamNum;		// �ɹ���ʼ���������
	static bool m_bCamSysInit;	// ���ϵͳ��ʼ��
    static MV_CC_DEVICE_INFO_LIST m_stDeviceList;	// �豸�б�ṹ�����
	// end ����������ñ���
	//////////////////////////////////////////////////////////////////////////
            
	void* m_pHandle;	// ������
	MV_CC_DEVICE_INFO m_stDeviceInfo;	// ��ǰ�豸��Ϣ�ṹ�����
	BYTE* m_pImageData;	// �ɼ�����ͼ������
	int m_nWidth;
	int m_nHeight;
	int m_nImageSize;
	HWND m_hWndShowImage;

	bool m_bCameraInitFlag;	// ��ǰ�����ʼ���ɹ���־
	int m_nCamIndex;	// ��ǰ������	��0��ʼ
	int m_nRet;			// ���������������ֵ

	// �����
	BOOL OpenCamera(CString camName, HWND hWndDisplay, LPBYTE& lpDesIMGAddress, int nWidth=IMG_WIDTH, int nHeight=IMG_HEIGHT, int nDepth=8);	
	BOOL CloseCamera();			// �ر����
	BOOL GrabOneFrame();		// MV_CC_GetOneFrameTimeout ��ȡһ֡ͼ��	
	BOOL WaitGrabOneFrame();	// callback������ȡһ֡ͼ��,ͼ�����ݱ�����buf��

	BOOL SetTriggerMode(bool bTriggerMode);		// ����ΪTriggerģʽ  bTriggerMode  true:����ΪTriggerģʽ   false:�����ɼ�ģʽ
	BOOL SoftTrigger();		// ��������

	BOOL SetGain(double gain);	// ��������ͷ����
	BOOL SetExp(long expUs);	// �����ع�ʱ�� ��λ:us
	bool m_bTranseferEnd;

	MV_FRAME_OUT_INFO_EX stImageInfo;
    MV_FRAME_OUT_INFO stMVFrameInfo;

public:
	static BOOL InitCamera();			// ��ʼ�����ϵͳ,������ȫ����ʼ��
};

#endif // !defined(AFX_HKCAMERA_H__1059D248_131A_49A7_AE19_41335E233EFB__INCLUDED_)
