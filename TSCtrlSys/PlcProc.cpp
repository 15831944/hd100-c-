#include "StdAfx.h"
#include "PlcProc.h"
#include "TSCtrlSys.h"
#include "Function.h"
CEvent g_Comexit(0,1);
CEvent g_commevent(1);
CPlcProc::CPlcProc(void)
{
}

CPlcProc::~CPlcProc(void)
{
}

BOOL CPlcProc::Init(CWnd* pPortOwner,	// the owner (CWnd) of the port (receives message)
					UINT  portnr,		// portnumber (1..4)
					UINT  baud,			// baudrate
					char  parity,		// parity 
					UINT  databits,		// databits 
					UINT  stopbits,		// stopbits 
					DWORD dwCommEvents,	// EV_RXCHAR, EV_CTS etc
					UINT  writebuffersize)	// size to the writebuffer
{
	if(0 == m_Port.m_iComState)
	{
		if(m_Port.InitPort(pPortOwner,portnr,baud,parity,databits,stopbits,dwCommEvents,writebuffersize))
		{
			m_Port.m_iComState = 1;
			if(m_Port.StartMonitoring()) 
				m_Port.m_iComState = 2;
			return TRUE;
		}
		else
		{
			m_Port.m_iComState = -1;
			return FALSE;
		}
	}
	return TRUE;
}
long CPlcProc::ReadPlcData(CString PLCDataID)
{
	CString Sendstr1,Sendstr2;
	CString Sendstr;
	int TempVal;
	CString AccpStr,AccpTempStr1,AccpTempStr2;
	CString  DataDT1;
	Sendstr1="%01#RDD";
	TempVal=atoi(PLCDataID.GetBuffer(0))+1;
	DataDT1.Format("%d",TempVal);
	Sendstr2=AddZero(PLCDataID,5)+AddZero(DataDT1,5);//数据DT串
	Sendstr=Sendstr1+Sendstr2+"**"+char(13);
	if (NULL != m_Port.m_hComm)
	{
		m_Port.WriteToPort(Sendstr);
		Sleep(100);
		if (m_Port.m_chnum > 0)
		{
			AccpStr.Format("%s",m_Port.m_chbuf);
			AccpStr = AccpStr.Left(m_Port.m_chnum);
			m_Port.m_chnum = 0;
			return ChangStrTOlong(AccpStr);
		}
		else 
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

long CPlcProc::ChangStrTOlong(CString ChangStr)
{
	CString DataStr,DataStr1,DataStr2;
	CString temStr1,temStr2;
	long    AccpVal;
	if (ChangStr.GetLength()<14)//断定是否接收正常
	{
		return -1;
	}
	DataStr1=ChangStr.Mid(6,4);
	DataStr2=ChangStr.Mid(10,4);
	temStr1=DataStr1.Mid(0,2);
	temStr2=DataStr1.Mid(2,2);
	DataStr1=temStr2+temStr1;//两位对换
	temStr1=DataStr2.Mid(0,2);
	temStr2=DataStr2.Mid(2,2);
	DataStr2=temStr2+temStr1;
	DataStr=DataStr2+DataStr1;
	sscanf(DataStr,"%X",&AccpVal);//十六进制转换
	return AccpVal;
}

CString CPlcProc::AddZero(CString str, int StrCout)
{
	int Strleng;
	CString tempstr;
	if(str.GetLength()<StrCout)
	{
		Strleng=StrCout-str.GetLength();
		for (int i=0;i<Strleng;i++)
		{
			str=_T("0")+str;
		}
	}
	return str;
}

BOOL CPlcProc::WritePlcYOut(CString OutID, bool on_off)
{

	CString  stron_off,strSendPLC;
	OutID=AddZero(OutID,4);
	if (on_off)
	{
		stron_off=_T("1**");
	}
	else
		stron_off=_T("0**");
	strSendPLC=_T("%01#WCSY")+OutID+stron_off+char(13);
	m_Port.WriteToPort(strSendPLC);
	return FALSE;
}

BOOL CPlcProc::WritePlcROut(CString OutID, BOOL on_off)
{
	CString  stron_off,strSendPLC;
	OutID=AddZero(OutID,4);
	if (on_off)
	{
		stron_off=_T("1**");
	}
	else
	{
		stron_off=_T("0**");
	}
	strSendPLC=_T("%01#WCSR")+OutID+stron_off+char(13);
	m_Port.WriteToPort(strSendPLC);
	return FALSE;
}

void CPlcProc::DotPLCR(CString PlcRID)
{
	WritePlcROut(PlcRID,TRUE);
	Sleep(200);
	WritePlcROut(PlcRID,FALSE);
}

//void CPlcProc::SendPLCR(int BtID, bool on_off)
//{
	//switch(BtID)
	//{
	//case 1:
	//	ChangeOutCh(true);
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outStickJogAdd,on_off);
	//	break;
	//case 2:
	//	ChangeOutCh(true);
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outStickJogSub,on_off);	
	//	break; 
	//case 3:
	//	ChangeOutCh(true);
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outRectifyJogAddX,on_off);
	//	break; 
	//case 4:
	//	ChangeOutCh(true);
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outRectifyJogSubX,on_off);	
	//	break; 
	//case 5:
	//	ChangeOutCh(true);
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outRectifyJogAddY,on_off);
	//	break; 
	//case 6:
	//	ChangeOutCh(true);
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outRectifyJogSubY,on_off);	
	//	break; 
	//case 7:
	//	ChangeOutCh(false);
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outStickJogAdd,on_off);		//out_5
	//	break; 
	//case 8:
	//	ChangeOutCh(false);
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outStickJogSub,on_off);		//out_6
	//	break; 
	//case 9:
	//	ChangeOutCh(false);
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outRectifyJogAddX,on_off);	//out_7
	//	break; 
	//case 10:
	//	ChangeOutCh(false);
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outRectifyJogSubX,on_off);	//out_8
	//	break; 
	//case 11:
	//	ChangeOutCh(false);
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outRectifyJogAddY,on_off);	//out_9
	//	break; 
	//case 12:
	//	ChangeOutCh(false);
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outRectifyJogSubY,on_off);	//out_10
	//	break;
	//default:
	//	break;
	//}
//}

//void CPlcProc::ChangeOutCh(bool m_WID)
//{
	//if (m_WID)
	//{
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outPLCID,true);
	//	m_WordID=1;
	//}
	//else 
	//{
	//	g_pFrm->m_Robot->m_pController->Outp(g_pFrm->m_Robot->m_IOCtrl.outPLCID,false);
	//	m_WordID=2;
	//}
//}

BOOL CPlcProc::WritePLCData(CString DTID, int DateVal)
{
	CString DTID1,strdata,strSendPLC;
	CString  SendDa1,SendDa2,SendDa3,SendDa4;
	CString  SendData;
	int TempVal;
	TempVal=atoi(DTID.GetBuffer(0))+1;
	DTID1.Format("%d",TempVal);
	DTID=AddZero(DTID,5);
	DTID1=AddZero(DTID1,5);
	////////////////////////////
	strdata.Format(_T("%X"),DateVal);//16进制
	strdata= AddZero(strdata,8);
	SendDa1=strdata.Mid(6,2);
	SendDa2=strdata.Mid(4,2);
	SendDa3=strdata.Mid(2,2);
	SendDa4=strdata.Mid(0,2);
	SendData=SendDa1+SendDa2+SendDa3+SendDa4;
	strSendPLC=_T("%01#WDD")+DTID+DTID1+SendData+_T("**")+char(13);
	//strSendPLC=_T("%01#WDD01000010011A270011**")+char(13);
	//%01#WDD01000010011A270000**
	CommSendPlc(strSendPLC);
	return false;
}

BOOL CPlcProc::CommSendPlc(CString StrPlc)
{
	if (NULL != m_Port.m_hComm)
	{
		m_Port.WriteToPort(StrPlc);
		return true;
	}
	else
		return false;
}


//////////////////////////////////////////////////////////////////////////
//CPlcCommInfo Class

CPlcCommInfo::CPlcCommInfo()
{
	PlcError = NULL;
	PlcSataut = NULL;
	pReserve = NULL;
	m_Thread = NULL;
};

CPlcCommInfo::~CPlcCommInfo()
{
	if(PlcError != NULL)
		delete []PlcError;
	if(PlcSataut != NULL)
		delete []PlcSataut;
	if(pReserve != NULL)
		delete []pReserve;
}

BOOL CPlcCommInfo::ReadParamFile()
{
	//CString  filename;
	//CString  errorTempID,SatauTempID,ReswrveTempID;

	//filename = CFunction::GetDirectory()+"system\\Plctext.ini";
	//ErrorCount = ::GetPrivateProfileInt("ErrorInfo","ErrorCount",NULL,filename);
	//if (ErrorCount>0)
	//{
	//	PlcError = new Tag_PlcError[ErrorCount];
	//	for(int i=0;i<ErrorCount;i++)
	//	{
	//		errorTempID.Format("ErrorText%d",i);
	//		::GetPrivateProfileString("ErrorText",errorTempID,NULL,PlcError[i].ErrorText.GetBuffer(128),128,filename);
	//		PlcError[i].ErrorText.ReleaseBuffer();
	//	}
	//}
	//else
	//	return FALSE;

	//SatautCount = ::GetPrivateProfileInt("SatautInfo","SatautCount",0,filename);

	//PlcSataut = new Tag_PlcSataut[SatautCount];
	//if (SatautCount>0)
	//{
	//	for(int i=0;i<SatautCount;i++)
	//	{
	//		SatauTempID.Format("SatauText%d",i);
	//		::GetPrivateProfileString("SatautText",SatauTempID,NULL,PlcSataut[i].SatautText.GetBuffer(128),128,filename);
	//		PlcSataut[i].SatautText.ReleaseBuffer();
	//	}
	//}
	//else
	//	return FALSE;
	//ReserveCount=::GetPrivateProfileInt("ReserveInfo","ReserCount",0,filename);
	//if (ReserveCount>0)
	//{
	//	pReserve=new CString[ReserveCount];
	//	for (int i=0;i<ReserveCount;i++)
	//	{
	//		ReswrveTempID.Format("ReserText%d",i);
	//		GetPrivateProfileString("ReserText",ReswrveTempID,NULL,pReserve[i].GetBuffer(128),128,filename);
	//		pReserve[i].ReleaseBuffer();
	//	}
	//}
	//else
	//	return FALSE;
	return TRUE;
}

BOOL CPlcCommInfo::StartComm()
{
	return TRUE;
}



BOOL CPlcCommInfo::StopComm()
{
	g_Comexit.SetEvent();
	::WaitForSingleObject(g_commevent,INFINITE);
	return TRUE;
}