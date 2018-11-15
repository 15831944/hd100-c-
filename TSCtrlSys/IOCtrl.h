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
	short Output(BOOL OutputSts);    // 板载IO输出
	short ExtOutput(BOOL  OutputSts);// 拓展IO用
	short WaitForSensorSignaled(LONG dwWaitSeconds,/*HANDLE hevtReturn[2],*/BOOL bWaitSignalSts=TRUE,BOOL bNeedMonitor=FALSE);
	short GetInputSts(WORD & SensorSts);	// 板载Input输入
	short GetExInputSts(WORD & SensorSts);	// 拓展Input输入
	short InitIOChannel(const WORD & chnBit, const WORD & nIsInputChn,const CString & strIOName);
	short SetIOType(WORD nIsInputChn);
	short SetIoModuleID(WORD nID);
	void  EnableOperation(bool enable);
	void  EnableCheck(bool enable,BOOL checksts=TRUE);
	void  EnableTigger(bool enable);
	CIOCtrl();
	virtual ~CIOCtrl();	

	CString m_strIOName;		//IO定义
	BOOL m_IsOpen;				//输入是否有信号 T=有信号，只针对INPUT点
	BOOL m_bNeedRTCheck;		//输出点是否要全程实时监测
	short m_bNeedRTCheckSts;    //IO点需要监测的状态
	WORD m_IOBit;				//IO位号
	WORD m_nIOType;				//IO口类型，输入/输出
	bool m_bEnable;             //使能
	bool m_IsSignalTriggerFlag; //信号触发过标志
	bool m_bIsExInp;			// 是否扩展IO输入口
};

#endif // !defined(AFX_IOCTRL_H__2F70F9A3_B460_48EA_98AF_07394D58ED29__INCLUDED_)
