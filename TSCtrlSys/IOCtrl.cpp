// IOCtrl.cpp: implementation of the CIOCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IOCtrl.h"
#include "TSCtrlSys.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIOCtrl::CIOCtrl()
{
	m_nIOType=0xffff;
	m_IOBit=0xffff;
	m_IsOpen=FALSE;
	m_bNeedRTCheck=FALSE;
	m_bNeedRTCheckSts=RTN_OFF;
	m_bEnable=TRUE;
	m_strIOName=_T("");
	m_IsSignalTriggerFlag=FALSE;
}

CIOCtrl::~CIOCtrl()
{

}

/****************************************************************************/
/*  ������:SetIOType			      										*/
/*  ��  �ܣ�����IO����													    */
/*  ��  ���� 																*/
/*         1. WORD nIsInputChn	 ����or���								    */
/*  ����ֵ�� ��������ֵ														*/
/*  ˵  ����1.															  	*/
/*  �汾:  V1.0													            */
/*  �޶�����:  5:1:2009   15:26                                             */
/*  ��д��: wusheng                                                         */
/****************************************************************************/
short CIOCtrl::SetIOType(WORD nIsInputChn)
{
	m_nIOType=nIsInputChn;
	return RTN_NONE;
}

/****************************************************************************/
/*  ������:InitIOChannel		      										*/
/*  ��  �ܣ���ʼ��IO��	 												    */
/*  ��  ���� 																*/
/*         1. const WORD & nMotherCard IOĸ����								*/
/*         2. const WORD & chnName ͨ����									*/
/*		   3. const WORD & chnBit  IOλ										*/
/*         4. const WORD & nIsInputChn IO���ͣ�����/�����					*/
/*         5. const CString & strIOName ��IO���������						*/
/*  ����ֵ�� ��������ֵ														*/
/*  ˵  ����1.															  	*/
/*  �汾:  V1.0													            */
/*  �޶�����:  5:1:2009   15:26                                             */
/*  ��д��: wsg																*/
/****************************************************************************/
short CIOCtrl::InitIOChannel( const WORD & chnBit, const WORD & nIsInputChn,const CString & strIOName)
{	
	m_IOBit=chnBit;     
	m_nIOType=nIsInputChn;      
	m_strIOName=strIOName;
	return RTN_NONE;
}

/****************************************************************************/
/*  ������:GetSensorSts			      										*/
/*  ��  ��:��ȡ����㵱ǰ״̬											    */
/*  ��  ���� 																*/
/*         1.WORD & SensorSts ���ص�����㵱ǰ״̬							*/
/*         2. 																*/
/*		   3.																*/
/*         4. 																*/
/*  ����ֵ�� ��������ֵ														*/
/*  ˵  ����1.															  	*/
/*  �汾:  V1.0													            */
/*  �޶�����:  5:1:2009   15:26                                             */
/*  ��д��: wsg                                                         */
/****************************************************************************/
short CIOCtrl::GetInputSts(WORD & SensorSts)
{  
	short sts;
	sts = g_pFrm->m_Robot->m_pController->InpBit(m_IOBit);
	
	if ((sts == RTN_ON)||(sts == RTN_OFF))
		SensorSts = sts;
	else 
		return sts;

	return RTN_NONE;
}

short CIOCtrl::GetExInputSts(WORD & SensorSts)
{
	long exInputSts = g_pFrm->m_Robot->m_exInputSts;
	if (exInputSts & 1<<m_IOBit)
		SensorSts = RTN_ON;
	else
		SensorSts = RTN_OFF;

	return RTN_NONE;
}
/****************************************************************************/
/*  ������:WaitForSensorSignaled      										*/
/*  ��  �ܣ��ȴ�һ��ʱ�䣬��ȡ�����ź�									    */
/*  ��  ���� 																*/
/*         1.LONG dwWaitSeconds �ȴ�ʱ�䣬��λ:�룬��������Ϊ����������	*/
/*         2.HANDLE hEvtReturn[2] �����ֹͣ����ͣ�¼����					*/
/*		   3.BOOL bWaitSignalSts Ҫ�������ȴ����ź�״̬					*/
/*         4. 																*/
/*  ����ֵ�� ��������ֵ														*/
/*  ˵  ����1.															  	*/
/*  �汾:  V1.0													            */
/*  �޶�����:  5:1:2009   15:26                                             */
/*  ��д��: wsg                                                        */
/****************************************************************************/
short CIOCtrl::WaitForSensorSignaled(LONG dwWaitSeconds,BOOL bWaitSignalSts,BOOL bNeedMonitor)
{	
	short dwRet;
	WORD sensorSts;
	WORD wait_sts;

	if (bWaitSignalSts)
		wait_sts=RTN_OFF;
	else
		wait_sts=RTN_ON;

	dwRet=GetInputSts(sensorSts);
    if (dwRet!=RTN_NONE)
		return dwRet;	
	
    CTimeSpan timeSpan;
	CTime StartTime,CurTime;
	
    StartTime=CTime::GetCurrentTime();

	if (dwWaitSeconds!=-1)
	{//�����ȴ��ź�
		while(sensorSts!=wait_sts)
		{
			Sleep(1);
			dwRet=GetInputSts(sensorSts);
			if (dwRet!=RTN_NONE)
			{
				m_bNeedRTCheck=FALSE;
				return dwRet;	
			}

			CurTime=CTime::GetCurrentTime();
			timeSpan=CurTime-StartTime;

			if (timeSpan.GetTotalSeconds()>=dwWaitSeconds) 
			{
				//int timeno=(int)timeSpan.GetTotalSeconds();
				m_bNeedRTCheck=FALSE;
				return RTN_TIMEOUT;
			}
			
			if (g_pFrm->m_Robot->m_tSysStatus == K_RUN_STS_STOP)
				break;
		}
		//�õ��ź�֮��ſ�ʼ��֪
		if (bNeedMonitor)
		{
			m_bNeedRTCheckSts=wait_sts;
			m_bNeedRTCheck=TRUE;
		}
		else
			m_bNeedRTCheck=FALSE;
	}
	else
	{////�ȴ���ģ���ź�
		dwRet=GetInputSts(sensorSts);
		while((!m_IsSignalTriggerFlag)||(sensorSts!=bWaitSignalSts))
		{
			Sleep(2);
			dwRet=GetInputSts(sensorSts);
			if (dwRet!=RTN_NONE)
				return dwRet;
		}
		m_IsSignalTriggerFlag=FALSE;
	}

	return RTN_NONE;
}

/****************************************************************************/
/*  ������:Output				      										*/
/*  ��  �ܣ���������Ϊָ��״̬										    */
/*  ��  ���� 																*/
/*         1.WORD  OutputSts  ָ�����״̬									*/
/*         2. 																*/
/*		   3.																*/
/*         4. 																*/
/*  ����ֵ�� ��������ֵ														*/
/*  ˵  ����1.															  	*/
/*  �汾:  V1.0													            */
/*  �޶�����:  5:1:2009   15:26                                             */
/*  ��д��: wusheng                                                         */
/****************************************************************************/
short CIOCtrl::Output(BOOL  OutputSts)
{ 
	if (m_nIOType==TYPE_INPUT) 
		return RTN_PARAMETER_ERR;
	if (!m_bEnable)
		return RTN_NONE;
	//	return IO_OPERATE_DISABLE;

    short m_iRet=g_pFrm->m_Robot->m_pController->SetOutport(m_IOBit,OutputSts);	
	if (RTN_NONE !=m_iRet) return m_iRet;

	return RTN_NONE;
}

short CIOCtrl::ExtOutput(BOOL  OutputSts)
{
	UNREFERENCED_PARAMETER(OutputSts);
	if (m_nIOType==TYPE_INPUT) 
		return RTN_PARAMETER_ERR;
	if (!m_bEnable)
		return RTN_NONE;

	short m_iRet=g_pFrm->m_Robot->m_pController->SetExOutport(0, m_IOBit,OutputSts);	
	if (RTN_NONE !=m_iRet) return m_iRet;

// 	//	return IO_OPERATE_DISABLE;
// 
// 	CString str;
// 	int nBitNum = m_IOBit + PLC_SEND_PORT_SDSET;
// 	str.Format("%d",nBitNum);
// 	
// 	if ((theApp.m_Serial.m_Port[0].m_iComState == 1)||(theApp.m_Serial.m_Port[0].m_iComState == 2))
// 	{
// 		theApp.m_Serial.WritePlcROut(str,TRUE);
// 		Sleep(100);
// 		theApp.m_Serial.WritePlcROut(str,FALSE);
// 		m_IsOpen = OutputSts;
// 	}
// 	else
// 	{
// 		return RTN_PARAMETER_ERR;
// 	}

	return RTN_NONE;
}

/****************************************************************************/
/*  ������:EnableOperation				      								*/
/*  ��  �ܣ�IO��ʹ��             										    */
/*  ��  ���� 																*/
/*         1.bool enable     �Ƿ�ʹ��        								*/
/*         2. 																*/
/*		   3.																*/
/*         4. 																*/
/*  ����ֵ�� ��������ֵ														*/
/*  ˵  ����1.															  	*/
/*  �汾:  V1.0													            */
/*  �޶�����:  5:1:2009   15:26                                             */
/*  ��д��: wusheng                                                         */
/****************************************************************************/
void  CIOCtrl::EnableOperation(bool enable)
{
	m_bEnable=enable;
}
void CIOCtrl::EnableTigger(bool enable)
{
	if(enable)
		m_IsSignalTriggerFlag=true;
	else
		m_IsSignalTriggerFlag=false;	
}
void CIOCtrl::EnableCheck(bool enable,BOOL checksts)
{
	if(enable)
	{
		m_bNeedRTCheck=TRUE;

		if (checksts)
		{
			m_bNeedRTCheckSts = RTN_ON;
		}
		else
		{
			m_bNeedRTCheckSts = RTN_OFF;
		}
	}
	else
		m_bNeedRTCheck=FALSE;	
}

/****************************************************************************/
/*  ������:GetSensorPulse				      								*/
/*  ��  �ܣ���ȡIo����             										    */
/*  ��  ���� 																*/
/*         1.bool bHavePulse     �Ƿ��ȡ�������ź�    				  		*/
/*         2.Word PulseType   ������/�½��� 		    					*/
/*		   3.long lExamSpan	  ��ȡ�����źŵ���ʱ				    		*/
/*         4. 																*/
/*  ����ֵ�� ��������ֵ														*/
/*  ˵  ����1.															  	*/
/*  �汾:  V1.0													            */
/*  �޶�����:  5:1:2009   15:26                                             */
/*  ��д��: wusheng                                                         */
/****************************************************************************/
short CIOCtrl::GetSensorPulse(bool & bHavePulse,WORD PulseType, long lExamSpan)
{
	WORD sensorSts1,sensorSts2;

	if((PulseType!=EDGE_ASCENDING)&&(EDGE_DESCENDING))
		return RTN_PARAMETER_ERR;

	bHavePulse=false;
	if (PulseType==EDGE_ASCENDING) //������
	{
		GetInputSts(sensorSts1);
		if (sensorSts1==RTN_OFF)
		{
			Sleep(lExamSpan);
			GetInputSts(sensorSts2);
			if (sensorSts2==RTN_ON)
			{
				//Sleep(100);
				GetInputSts(sensorSts2);
				if (sensorSts2==RTN_ON)
					bHavePulse=true;
				
			}
		}
	}
	else  //�½���
	{
		GetInputSts(sensorSts1);
		if (sensorSts1==RTN_ON)
		{
			Sleep(lExamSpan);
			GetInputSts(sensorSts2);
			if (sensorSts2==RTN_OFF)
			{
				Sleep(lExamSpan);
				GetInputSts(sensorSts2);
				if (sensorSts2==RTN_OFF)
					bHavePulse=true;
			}
		}
	}
	return RTN_NONE;
}
/****************************************************************************/
/*  ������:WaitForSensorPulse				      							*/
/*  ��  �ܣ��ȴ�Io����          										    */
/*  ��  ���� 																*/
/*         1.bool bHavePulse     �Ƿ��ȡ�������ź�    				  		*/
/*         2.Word PulseType   ������/�½��� 		    					*/
/*		   3.long lExamSpan	  ��ȡ�����źŵ���ʱ				    		*/
/*         4. long lWaitTime   �ȴ�ʱ��		-1 ���õȴ�						*/
/*  ����ֵ�� ��������ֵ														*/
/*  ˵  ����1.															  	*/
/*  �汾:  V1.0													            */
/*  �޶�����:  5:1:2009   15:26                                             */
/*  ��д��: wusheng                                                        */
/****************************************************************************/
short CIOCtrl::WaitForSensorPulse(bool &bHavePulse, WORD PulseType, long lWaitTime, long lExamSpan)
{
	GetSensorPulse(bHavePulse,PulseType,lExamSpan);

	CTimeSpan timeSpan;
	CTime StartTime,CurTime;

	short dwRet;
	
    StartTime=CTime::GetCurrentTime();
	while(!bHavePulse)
	{
		if (lWaitTime!=-1) 
		{
			CurTime=CTime::GetCurrentTime();
			timeSpan=CurTime-StartTime;
			if (timeSpan.GetTotalSeconds()>=lWaitTime) //����ʱ��
			{				
				return RTN_TIMEOUT;
			}	
			dwRet=GetSensorPulse(bHavePulse,PulseType,lExamSpan);
			if (dwRet!=RTN_NONE)
				return dwRet;			
		}
		else//���õȴ�
		{
			GetSensorPulse(bHavePulse,PulseType,lExamSpan);
		}
	}	
	return RTN_NONE;
}
