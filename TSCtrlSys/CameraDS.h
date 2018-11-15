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

	// �����
	BOOL OpenCamera(CString camName, HWND hWndDisplay, LPBYTE& lpDesIMGAddress);
	static void XferCallback(SapXferCallbackInfo *pInfo);
	BOOL CreateObjects();
	BOOL DestroyObjects();
	BOOL Init0();
	BOOL Init();

	// ����ΪTriggerģʽ  bTriggerMode  true:����ΪTriggerģʽ   false:�����ɼ�ģʽ
	BOOL SetTriggerMode(bool bTriggerMode);		
	// ��������
	BOOL SoftTrigger();		
	// �ȴ�callback������ȡһ֡ͼ��,ͼ�����ݱ�����buf��
	BOOL WaitGrabOneFrame();

	bool m_bCameraInitFlag;	// ��ǰ�����ʼ���ɹ���־
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

