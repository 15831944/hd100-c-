#pragma once
// #include "CameraDS.h"
// #include "CameraHK.h"

class CCamera
{
public:
	CCamera(void);
	~CCamera(void);

	/*	����:	�����
	 *	������	camName			�������,һ�����ⲿ���ú�.�����������ⲿ���ú�"���1"
	 			hWndDisplay		��ʾ���ͼ��Ĵ��ھ��
				lpDesIMGAddress ͼ���ַ,BYTE����ָ��,�����ʼ���ɹ��󽫴�ָ��ָ��������������ڴ�
	 *	����ֵ:	TRUE	FALSE	����Ƿ��ʼ���ɹ�							
	 */
	virtual BOOL OpenCamera(CString camName, HWND hWndDisplay, LPBYTE& lpDesIMGAddress, int nWidth=IMG_WIDTH, int nHeight=IMG_HEIGHT, int nDepth=8);	
	
	// ����ΪTriggerģʽ  bTriggerMode  true:����ΪTriggerģʽ   false:�����ɼ�ģʽ
	virtual BOOL SetTriggerMode(bool bTriggerMode);		

	// ��������
	virtual BOOL SoftTrigger();		

	// �ȴ�callback������ȡһ֡ͼ��,ͼ�����ݱ�����buf��
	virtual BOOL WaitGrabOneFrame();	
};

