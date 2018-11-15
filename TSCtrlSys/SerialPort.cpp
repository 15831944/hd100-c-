// SerialPort.cpp: implementation of the SerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "APD.h"
#include "SerialPort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SerialPort::SerialPort()
{
	m_bConnected = false;			//指示串口还未被打开
	m_sPort		 = "";				//端口名称
	m_nComNo	 = -1;				//端口序号
	m_nBaud		 = 9600;			//波特率
	m_nDataBits  = 8;
	m_nParity	 = 0;
	m_nStopBits  = 0;
}

// SerialPort::SerialPort(int ComNo, DWORD nBaud, int nDataBits, int nParity, int nStopBits)
// {
// 	m_bConnected = false;     //指示串口还未被打开
// 	//m_pThread  = NULL;      //辅助线程句柄为空
// 	m_nBaud      = nBaud;     //波特率为9600
// 	m_nDataBits  = nDataBits; //数据位  
// 	m_bEcho      = FALSE;     //不进行回显
// 	m_nFlowCtrl  = 0;         //无流量控制
// 	m_bNewLine   = FALSE;     //从串口读到回车符不换行
// 	m_nParity    = nParity;   //无奇偶校验0: NOPARITY;2: EVENPARITY;1 ODDPARITY;
// 	//m_nParity  = 2;         //奇校验0: NOPARITY;2: EVENPARITY;1 ODDPARITY;
// 	m_nStopBits  = nStopBits; //停止位
// 	
// 	if (ComNo <= 0)
// 	{
// 		ComNo = 1;
// 	}
// 	
// 	m_sPort.Format("COM%d", ComNo);
// }

SerialPort::~SerialPort()
{
	if (m_bConnected)
	{
		CloseConnection();
	}
}

//初始化串口
BOOL SerialPort::Init(int ComNo, DWORD nBaud, int nDataBits, int nParity, int nStopBits)
{
	m_bConnected = false;     //指示串口还未被打开
	//m_pThread  = NULL;      //辅助线程句柄为空
	m_nBaud      = nBaud;     //波特率为9600
	m_nDataBits  = nDataBits; //数据位  
	m_bEcho      = FALSE;     //不进行回显
	m_nFlowCtrl  = 0;         //无流量控制
	m_bNewLine   = FALSE;     //从串口读到回车符不换行
	m_nParity    = nParity;   //无奇偶校验0: NOPARITY;2: EVENPARITY;1 ODDPARITY;
	//m_nParity  = 2;         //奇校验0: NOPARITY;2: EVENPARITY;1 ODDPARITY;
	m_nStopBits  = nStopBits; //停止位

	if (ComNo <= 0)
	{
		ComNo = 1;
	}

	m_sPort.Format("COM%d", ComNo);

	//打开串口设备
	if (!OpenConnection())
	{
		return FALSE;
	}

	return TRUE;
}

//打开串口设备
BOOL SerialPort::OpenConnection()
{
	COMMTIMEOUTS TimeOuts;

	if (m_bConnected)
	{
		return TRUE;
	}

	m_hCom = CreateFile(m_sPort, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL); // 重叠方式

	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(m_sPort + "打开失败");

		return FALSE;
	}

	memset(&m_osRead,0,sizeof(OVERLAPPED));
	memset(&m_osWrite,0,sizeof(OVERLAPPED));

	SetupComm(m_hCom, MAXBLOCK, MAXBLOCK);
	SetCommMask(m_hCom, EV_RXCHAR);

	//把间隔超时设为最大，把总超时设为0将导致ReadFile立即返回并完成操作
	TimeOuts.ReadIntervalTimeout=MAXDWORD; 
	TimeOuts.ReadTotalTimeoutMultiplier=0; 
	TimeOuts.ReadTotalTimeoutConstant=0; 
	/* 设置写超时以指定WriteComm成员函数中的
	GetOverlappedResult函数的等待时间*/
	//TimeOuts.WriteTotalTimeoutMultiplier=50; 
	//TimeOuts.WriteTotalTimeoutConstant=2000;
	TimeOuts.WriteTotalTimeoutMultiplier=0; 
	TimeOuts.WriteTotalTimeoutConstant=1000;

	SetCommTimeouts(m_hCom, &TimeOuts);

	m_bConnected = TRUE;

	if (!ConfigConnection())
	{
		return FALSE;
	}

	return TRUE;
}

//设置串口设备
BOOL SerialPort::ConfigConnection()
{
	if (!m_bConnected)
	{
		return FALSE;
	}

	DCB dcb;

	if (!GetCommState(m_hCom, &dcb))
	{
		return FALSE;
	}

	dcb.fBinary=TRUE;
	dcb.BaudRate=m_nBaud; //波特率
	dcb.ByteSize=m_nDataBits&0xff; //每字节位数
	dcb.fParity=TRUE;
	switch(m_nParity) //校验设置
	{
	case 0: dcb.Parity=NOPARITY;
		break;
	case 1: dcb.Parity=ODDPARITY;
		break;
	case 2: dcb.Parity=EVENPARITY;
		break;
	default:;
	}

	switch(m_nStopBits) //停止位
	{
	case 0: dcb.StopBits=ONESTOPBIT;
		break;
	case 1: dcb.StopBits=ONE5STOPBITS;
		break;
	case 2: dcb.StopBits=TWOSTOPBITS;
		break;
	default:;
	}


	//硬件流控制设置
	dcb.fOutxCtsFlow=m_nFlowCtrl==1;
	dcb.fRtsControl=(m_nFlowCtrl==1?RTS_CONTROL_HANDSHAKE:RTS_CONTROL_ENABLE);
	//XON/XOFF流控制设置
	dcb.fInX=dcb.fOutX=m_nFlowCtrl==2;
	dcb.XonChar=XON;
	dcb.XoffChar=XOFF;
	dcb.XonLim=50;
	dcb.XoffLim=50;

	return SetCommState(m_hCom, &dcb);
}

//关闭串口设备	
void SerialPort::CloseConnection()
{
	if (!m_bConnected)
	{
		return;
	}

	m_bConnected=FALSE;
	//结束CommProc线程中WaitSingleObject函数的等待
	//SetEvent(m_hPostMsgEvent); 

	//结束CommProc线程中WaitCommEvent的等待
	SetCommMask(m_hCom, 0); 

	//等待辅助线程终止
	//WaitForSingleObject(m_pThread->m_hThread, INFINITE);
	//m_pThread=NULL;
	CloseHandle(m_hCom);
}

//读写串口
DWORD SerialPort::ReadCommByByte(BYTE *buf, DWORD dwLength)
{
	DWORD length=0;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	ClearCommError(m_hCom, &dwErrorFlags, &ComStat);

	length=min(dwLength, ComStat.cbInQue);
	if(!length) return length;

	ReadFile(m_hCom, buf, length, &length, &m_osRead);

	return length;
}

//读写串口
DWORD SerialPort::ReadComm(char *buf, DWORD dwLength)
{
	DWORD length=0;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	ClearCommError(m_hCom, &dwErrorFlags, &ComStat);

	length = min(dwLength, ComStat.cbInQue);
	if (0 == length)
	{
		return length;
	}

	memset(&m_osRead,0,sizeof(OVERLAPPED));

	if (!ReadFile(m_hCom, buf, length, &length, &m_osRead))
	{
		if(GetLastError()==ERROR_IO_PENDING)
		{
			GetOverlappedResult(m_hCom, &m_osWrite, &length, FALSE); //等待
		}
		else
		{
			length = 0;
		}
	}

	return length;
}

//写串口设备
DWORD SerialPort::WriteComm(char *buf, DWORD dwLength)
{
	BOOL fState;
	DWORD length = dwLength;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	//add 2005.3
	//		PurgeComm(m_hCom, PURGE_TXCLEAR|PURGE_TXABORT);
	ClearCommError(m_hCom,&dwErrorFlags,&ComStat);

	memset(&m_osWrite,0,sizeof(OVERLAPPED));
	fState = WriteFile(m_hCom, buf, length, &length, &m_osWrite); //调用成功,返回非0; 调用不成功,返回为0
	/*
	BOOL WriteFile(
	HANDLE hFile,                   // 文件句柄
	LPCVOID lpBuffer,               // 数据缓存区指针
	DWORD nNumberOfBytesToWrite,    // 你要写的字节数
	LPDWORD lpNumberOfBytesWritten, // 用于保存实际写入字节数的存储区域的指针
	LPOVERLAPPED lpOverlapped       // OVERLAPPED结构体指针
	)
	*/

	//WriteFile 返回失败
	if (!fState)
	{
		if (GetLastError()==ERROR_IO_PENDING)
		{
			//等待
			if(!GetOverlappedResult(m_hCom,&m_osWrite,&length,TRUE))
			{
				DWORD errcode = GetLastError();
				CTime curtime=CTime::GetCurrentTime();
				CString filename = "c:\\err_com.log";
				FILE *pf = fopen(filename,"a+");
				if (pf)
				{
					//					DWORD d = GetTickCount();
					fprintf(pf,"%s : write err - %d \n",curtime.Format("%H:%M:%S"),errcode);	
					fclose(pf);
				}
			}
		}
		else
		{
			length=0;
			DWORD errcode = GetLastError();
			CTime curtime=CTime::GetCurrentTime();
			CString filename = "c:\\err_com.log";
			FILE *pf = fopen(filename,"a+");
			if (pf)
			{
				//				DWORD d = GetTickCount();
				fprintf(pf,"%s : write err - %d \n",curtime.Format("%H:%M:%S"),errcode);	
				fclose(pf);
			}
		}
	}
	return length;
}

//设置串口
void SerialPort::SetBaud(long newBaud)
{
	m_nBaud = newBaud;
	ConfigConnection();
}

//设置串口
void SerialPort::SetPort(int nDataBits, int nParity, int nStopBits)
{
	m_nDataBits = nDataBits;
	m_nParity = nParity;
	m_nStopBits = nStopBits;
	ConfigConnection();
}

//返回是否打开串口
BOOL SerialPort::IsConnected()	
{
	return m_bConnected;
}

BOOL SerialPort::PurgeWritebuf()
{
	if (!m_bConnected)
	{
		return FALSE;
	}

	return PurgeComm(m_hCom, PURGE_TXABORT|PURGE_TXCLEAR);
	//PURGE_TXABORT 用于中断未完成的写操作
	//PURGE_TXCLEAR 用于清空Output buffer
}

BOOL SerialPort::PurgeReadbuf()
{
	if (!m_bConnected)
	{
		return FALSE;
	}

	return PurgeComm(m_hCom, PURGE_RXABORT|PURGE_RXCLEAR);
	//PURGE_RXABORT 用于中断未完成的读操作
	//PURGE_RXCLEAR 用于清空input buffer
}

void SerialPort::SetPortNo(int& newPortNo)
{
	if (newPortNo<1)
	{
		AfxMessageBox("串口号不能小于1");
		return;
	}

	CString buf;
	buf.Format("COM%d",newPortNo); //端口
	if(buf == m_sPort)
		return;	

	if(m_bConnected)
	{
		CloseConnection();	//先关闭旧端口
		m_sPort = buf;		//更新端口
		OpenConnection();	//再打开新端口
	}
	else
	{
		m_sPort = buf; //更新端口
	}
}

//从串口中读取给定数量的字符，必须读到给定的数量，否则超时返回false
BOOL SerialPort::ReadComByNum(char *buf, DWORD dwLength, DWORD *Length, DWORD TimeOut)
{
	DWORD st = GetTickCount();

	DWORD LengthReturn = 0;	//从串口中一次实际读到的字节数
	*Length = 0;			//从串口中实际读到的字节数

	*Length = *Length + LengthReturn;

	do
	{
		LengthReturn = ReadComm(buf+(*Length),dwLength - *Length); //增量读
		*Length = *Length + LengthReturn; //增量

		if(TimeOut == 0) break;
		if(dwLength == *Length)
			break;
	}while(GetTickCount() - st < TimeOut);

	return (dwLength == *Length?TRUE:FALSE);
}

BOOL SerialPort::ReadComByEndChar(char *buf, DWORD MaxLen, char endchar, DWORD *Length, DWORD TimeOut)
{
	DWORD st = GetTickCount();

	DWORD LengthReturn = 0;	//从串口中一次实际读到的字节数
	*Length = 0;			//从串口中实际读到的字节数

	*Length = *Length + LengthReturn;

	do
	{
		if(MaxLen == *Length)
			break;

		LengthReturn = ReadComm(buf+(*Length),1);//增量读,每次读一个字节

		if(LengthReturn>0)
		{
			*Length = *Length + LengthReturn;	//增量

			if(buf[*Length-1] == endchar)
				return TRUE;

			if(TimeOut == 0)
				continue;	//若timeout为0,则把缓冲中内容读完

			st = GetTickCount();	//串口读缓冲中有内容,更新起始时间
		}

	}while(GetTickCount() - st < TimeOut);

	return FALSE;
}