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
/*  函数名:SetIOType			      										*/
/*  功  能：设置IO类型													    */
/*  参  数： 																*/
/*         1. WORD nIsInputChn	 输入or输出								    */
/*  返回值： 操作返回值														*/
/*  说  明：1.															  	*/
/*  版本:  V1.0													            */
/*  修订日期:  5:1:2009   15:26                                             */
/*  编写者: wusheng                                                         */
/****************************************************************************/
short CIOCtrl::SetIOType(WORD nIsInputChn)
{
	m_nIOType=nIsInputChn;
	return RTN_NONE;
}

/****************************************************************************/
/*  函数名:InitIOChannel		      										*/
/*  功  能：初始化IO点	 												    */
/*  参  数： 																*/
/*         1. const WORD & nMotherCard IO母卡号								*/
/*         2. const WORD & chnName 通道名									*/
/*		   3. const WORD & chnBit  IO位										*/
/*         4. const WORD & nIsInputChn IO类型（输入/输出）					*/
/*         5. const CString & strIOName 该IO点的中文名						*/
/*  返回值： 操作返回值														*/
/*  说  明：1.															  	*/
/*  版本:  V1.0													            */
/*  修订日期:  5:1:2009   15:26                                             */
/*  编写者: wsg																*/
/****************************************************************************/
short CIOCtrl::InitIOChannel( const WORD & chnBit, const WORD & nIsInputChn,const CString & strIOName)
{	
	m_IOBit=chnBit;     
	m_nIOType=nIsInputChn;      
	m_strIOName=strIOName;
	return RTN_NONE;
}

/****************************************************************************/
/*  函数名:GetSensorSts			      										*/
/*  功  能:获取输入点当前状态											    */
/*  参  数： 																*/
/*         1.WORD & SensorSts 返回的输入点当前状态							*/
/*         2. 																*/
/*		   3.																*/
/*         4. 																*/
/*  返回值： 操作返回值														*/
/*  说  明：1.															  	*/
/*  版本:  V1.0													            */
/*  修订日期:  5:1:2009   15:26                                             */
/*  编写者: wsg                                                         */
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
/*  函数名:WaitForSensorSignaled      										*/
/*  功  能：等待一段时间，获取输入信号									    */
/*  参  数： 																*/
/*         1.LONG dwWaitSeconds 等待时间，单位:秒，超过则认为该输入点出错	*/
/*         2.HANDLE hEvtReturn[2] 传入的停止和暂停事件句柄					*/
/*		   3.BOOL bWaitSignalSts 要求输入点等待的信号状态					*/
/*         4. 																*/
/*  返回值： 操作返回值														*/
/*  说  明：1.															  	*/
/*  版本:  V1.0													            */
/*  修订日期:  5:1:2009   15:26                                             */
/*  编写者: wsg                                                        */
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
	{//其他等待信号
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
		//得到信号之后才开始检知
		if (bNeedMonitor)
		{
			m_bNeedRTCheckSts=wait_sts;
			m_bNeedRTCheck=TRUE;
		}
		else
			m_bNeedRTCheck=FALSE;
	}
	else
	{////等待开模完信号
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
/*  函数名:Output				      										*/
/*  功  能：输出点输出为指定状态										    */
/*  参  数： 																*/
/*         1.WORD  OutputSts  指定输出状态									*/
/*         2. 																*/
/*		   3.																*/
/*         4. 																*/
/*  返回值： 操作返回值														*/
/*  说  明：1.															  	*/
/*  版本:  V1.0													            */
/*  修订日期:  5:1:2009   15:26                                             */
/*  编写者: wusheng                                                         */
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
/*  函数名:EnableOperation				      								*/
/*  功  能：IO口使能             										    */
/*  参  数： 																*/
/*         1.bool enable     是否使能        								*/
/*         2. 																*/
/*		   3.																*/
/*         4. 																*/
/*  返回值： 操作返回值														*/
/*  说  明：1.															  	*/
/*  版本:  V1.0													            */
/*  修订日期:  5:1:2009   15:26                                             */
/*  编写者: wusheng                                                         */
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
/*  函数名:GetSensorPulse				      								*/
/*  功  能：获取Io脉冲             										    */
/*  参  数： 																*/
/*         1.bool bHavePulse     是否获取到脉冲信号    				  		*/
/*         2.Word PulseType   上升沿/下降沿 		    					*/
/*		   3.long lExamSpan	  获取脉冲信号的延时				    		*/
/*         4. 																*/
/*  返回值： 操作返回值														*/
/*  说  明：1.															  	*/
/*  版本:  V1.0													            */
/*  修订日期:  5:1:2009   15:26                                             */
/*  编写者: wusheng                                                         */
/****************************************************************************/
short CIOCtrl::GetSensorPulse(bool & bHavePulse,WORD PulseType, long lExamSpan)
{
	WORD sensorSts1,sensorSts2;

	if((PulseType!=EDGE_ASCENDING)&&(EDGE_DESCENDING))
		return RTN_PARAMETER_ERR;

	bHavePulse=false;
	if (PulseType==EDGE_ASCENDING) //上升沿
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
	else  //下降沿
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
/*  函数名:WaitForSensorPulse				      							*/
/*  功  能：等待Io脉冲          										    */
/*  参  数： 																*/
/*         1.bool bHavePulse     是否获取到脉冲信号    				  		*/
/*         2.Word PulseType   上升沿/下降沿 		    					*/
/*		   3.long lExamSpan	  获取脉冲信号的延时				    		*/
/*         4. long lWaitTime   等待时间		-1 永久等待						*/
/*  返回值： 操作返回值														*/
/*  说  明：1.															  	*/
/*  版本:  V1.0													            */
/*  修订日期:  5:1:2009   15:26                                             */
/*  编写者: wusheng                                                        */
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
			if (timeSpan.GetTotalSeconds()>=lWaitTime) //超出时间
			{				
				return RTN_TIMEOUT;
			}	
			dwRet=GetSensorPulse(bHavePulse,PulseType,lExamSpan);
			if (dwRet!=RTN_NONE)
				return dwRet;			
		}
		else//永久等待
		{
			GetSensorPulse(bHavePulse,PulseType,lExamSpan);
		}
	}	
	return RTN_NONE;
}
