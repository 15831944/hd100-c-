#pragma once
#include <vfw.h>
#include "CameraCtrlBase.h"

#include "AVT/FGCamera.h"
#include "AVT/convert.h"
#pragma comment(lib, "AVT/FGCamera.lib")

// AVT �������
class CAVTCameraCtrl:public CCameraCtrlBase,public CWnd
{
	DECLARE_DYNAMIC(CAVTCameraCtrl)
public:
	CAVTCameraCtrl(void);
	virtual ~CAVTCameraCtrl(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	bool OpenCarm(void);      // �����
	void Carmlive();          // �����ɼ�     
	void StopCarm(void);      // ֹͣ�ɼ�
	void CloseCarm(void);     // ֹͣ���ر����
	void SetCarmParam(int P_AVTShutter,int P_AVTBrightness,int P_AVTAVTGain);// �����������

	void InitBitmap(UINT32 ImageWidth, UINT32 ImageHeight, BOOL IsColorCamera);  // ����λͼͷ�ļ�
	void ShowBitmap(FGFRAME frame);// ��ʾλͼ

	// ���ջص�����
	static void WINAPI SnapCallback(void* Context,UINT32 wParam,void* lParam);

public:
	// ���������
	CFGCamera m_avtCamera;

	// �ɼ��߳�
	HANDLE m_hSnapThread;
	DWORD m_dwThreadId;

	//����AVI�߳�
	HANDLE m_hStoreAVIThread;
	DWORD  m_dwThreadIdAVI;

	int		m_AVTShutter;
	int     m_AVTGain;
	int     m_AVTPackageSize;
	int     m_AVTBrightness;
	int		m_AVTSnapMode;

	BOOL m_bIsColorCamera;     // �Ƿ�Ϊ��ɫ�����
	BOOL m_bLivingSnap;        // ��ǰ�Ƿ��ڽ��������ɼ�
	bool m_IsBayerImage;       // 

	CString strText;           // ��ʾ������
	int     CurrentImageNum;
	RECT    rcOutVideo;

	// �������ǰAOI�Ŀ���Լ���ʼλ��
	UINT32 m_nAOIWidth;
	UINT32 m_nAOIHeight;
	UINT32 m_nAOIX;
	UINT32 m_nAOIY;
	UINT32 m_TriggerValue;
	UINT32 m_nAOIStartX;
	UINT32 m_nAOIStartY;

	// ����ͼ����ֵ
	UINT32 m_nImageWidth;
	UINT32 m_nImageHeight;

	// ΪҪ��ʾ��ͼ������ڴ������Լ�λͼ��Ϣͷ�ռ�
	UINT8* m_pImageBuffer;
	UINT8* m_pBayerBuffer;
	BITMAPINFO* m_pBitmapInfo;  //λͼָ��

	// ��ʼ����AVI���жϱ�־	
	BOOL m_bStartAVI;
	int  BytesPerPixel;
	int  BytesPerPixel2;
	PAVIFILE	m_pFile;
	PAVISTREAM	m_ps;
	PAVISTREAM	m_psCompressed;
	int			m_nTimeFrame;
	HRESULT     hr;
};
