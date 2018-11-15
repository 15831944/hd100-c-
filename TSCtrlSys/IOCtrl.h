// IOCtrl.h: interface for the CIOCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCTRL_H__2F70F9A3_B460_48EA_98AF_07394D58ED29__INCLUDED_)
#define AFX_IOCTRL_H__2F70F9A3_B460_48EA_98AF_07394D58ED29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIOCtrl  
{
public:
	short WaitForSensorPulse(bool & bHavePulse,WORD PulseType, long lWaitTime,long lExamSpan=50);
	short GetSensorPulse(bool & bHavePulse,WORD PulseType, long lExamSpan=500);
	short Output(BOOL OutputSts);    // ����IO���
	short ExtOutput(BOOL  OutputSts);// ��չIO��
	short WaitForSensorSignaled(LONG dwWaitSeconds,/*HANDLE hevtReturn[2],*/BOOL bWaitSignalSts=TRUE,BOOL bNeedMonitor=FALSE);
	short GetInputSts(WORD & SensorSts);	// ����Input����
	short GetExInputSts(WORD & SensorSts);	// ��չInput����
	short InitIOChannel(const WORD & chnBit, const WORD & nIsInputChn,const CString & strIOName);
	short SetIOType(WORD nIsInputChn);
	short SetIoModuleID(WORD nID);
	void  EnableOperation(bool enable);
	void  EnableCheck(bool enable,BOOL checksts=TRUE);
	void  EnableTigger(bool enable);
	CIOCtrl();
	virtual ~CIOCtrl();	

	CString m_strIOName;		//IO����
	BOOL m_IsOpen;				//�����Ƿ����ź� T=���źţ�ֻ���INPUT��
	BOOL m_bNeedRTCheck;		//������Ƿ�Ҫȫ��ʵʱ���
	short m_bNeedRTCheckSts;    //IO����Ҫ����״̬
	WORD m_IOBit;				//IOλ��
	WORD m_nIOType;				//IO�����ͣ�����/���
	bool m_bEnable;             //ʹ��
	bool m_IsSignalTriggerFlag; //�źŴ�������־
	bool m_bIsExInp;			// �Ƿ���չIO�����
};

#endif // !defined(AFX_IOCTRL_H__2F70F9A3_B460_48EA_98AF_07394D58ED29__INCLUDED_)
