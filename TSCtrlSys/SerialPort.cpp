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
	m_bConnected = false;			//ָʾ���ڻ�δ����
	m_sPort		 = "";				//�˿�����
	m_nComNo	 = -1;				//�˿����
	m_nBaud		 = 9600;			//������
	m_nDataBits  = 8;
	m_nParity	 = 0;
	m_nStopBits  = 0;
}

// SerialPort::SerialPort(int ComNo, DWORD nBaud, int nDataBits, int nParity, int nStopBits)
// {
// 	m_bConnected = false;     //ָʾ���ڻ�δ����
// 	//m_pThread  = NULL;      //�����߳̾��Ϊ��
// 	m_nBaud      = nBaud;     //������Ϊ9600
// 	m_nDataBits  = nDataBits; //����λ  
// 	m_bEcho      = FALSE;     //�����л���
// 	m_nFlowCtrl  = 0;         //����������
// 	m_bNewLine   = FALSE;     //�Ӵ��ڶ����س���������
// 	m_nParity    = nParity;   //����żУ��0: NOPARITY;2: EVENPARITY;1 ODDPARITY;
// 	//m_nParity  = 2;         //��У��0: NOPARITY;2: EVENPARITY;1 ODDPARITY;
// 	m_nStopBits  = nStopBits; //ֹͣλ
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

//��ʼ������
BOOL SerialPort::Init(int ComNo, DWORD nBaud, int nDataBits, int nParity, int nStopBits)
{
	m_bConnected = false;     //ָʾ���ڻ�δ����
	//m_pThread  = NULL;      //�����߳̾��Ϊ��
	m_nBaud      = nBaud;     //������Ϊ9600
	m_nDataBits  = nDataBits; //����λ  
	m_bEcho      = FALSE;     //�����л���
	m_nFlowCtrl  = 0;         //����������
	m_bNewLine   = FALSE;     //�Ӵ��ڶ����س���������
	m_nParity    = nParity;   //����żУ��0: NOPARITY;2: EVENPARITY;1 ODDPARITY;
	//m_nParity  = 2;         //��У��0: NOPARITY;2: EVENPARITY;1 ODDPARITY;
	m_nStopBits  = nStopBits; //ֹͣλ

	if (ComNo <= 0)
	{
		ComNo = 1;
	}

	m_sPort.Format("COM%d", ComNo);

	//�򿪴����豸
	if (!OpenConnection())
	{
		return FALSE;
	}

	return TRUE;
}

//�򿪴����豸
BOOL SerialPort::OpenConnection()
{
	COMMTIMEOUTS TimeOuts;

	if (m_bConnected)
	{
		return TRUE;
	}

	m_hCom = CreateFile(m_sPort, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL); // �ص���ʽ

	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(m_sPort + "��ʧ��");

		return FALSE;
	}

	memset(&m_osRead,0,sizeof(OVERLAPPED));
	memset(&m_osWrite,0,sizeof(OVERLAPPED));

	SetupComm(m_hCom, MAXBLOCK, MAXBLOCK);
	SetCommMask(m_hCom, EV_RXCHAR);

	//�Ѽ����ʱ��Ϊ��󣬰��ܳ�ʱ��Ϊ0������ReadFile�������ز���ɲ���
	TimeOuts.ReadIntervalTimeout=MAXDWORD; 
	TimeOuts.ReadTotalTimeoutMultiplier=0; 
	TimeOuts.ReadTotalTimeoutConstant=0; 
	/* ����д��ʱ��ָ��WriteComm��Ա�����е�
	GetOverlappedResult�����ĵȴ�ʱ��*/
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

//���ô����豸
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
	dcb.BaudRate=m_nBaud; //������
	dcb.ByteSize=m_nDataBits&0xff; //ÿ�ֽ�λ��
	dcb.fParity=TRUE;
	switch(m_nParity) //У������
	{
	case 0: dcb.Parity=NOPARITY;
		break;
	case 1: dcb.Parity=ODDPARITY;
		break;
	case 2: dcb.Parity=EVENPARITY;
		break;
	default:;
	}

	switch(m_nStopBits) //ֹͣλ
	{
	case 0: dcb.StopBits=ONESTOPBIT;
		break;
	case 1: dcb.StopBits=ONE5STOPBITS;
		break;
	case 2: dcb.StopBits=TWOSTOPBITS;
		break;
	default:;
	}


	//Ӳ������������
	dcb.fOutxCtsFlow=m_nFlowCtrl==1;
	dcb.fRtsControl=(m_nFlowCtrl==1?RTS_CONTROL_HANDSHAKE:RTS_CONTROL_ENABLE);
	//XON/XOFF����������
	dcb.fInX=dcb.fOutX=m_nFlowCtrl==2;
	dcb.XonChar=XON;
	dcb.XoffChar=XOFF;
	dcb.XonLim=50;
	dcb.XoffLim=50;

	return SetCommState(m_hCom, &dcb);
}

//�رմ����豸	
void SerialPort::CloseConnection()
{
	if (!m_bConnected)
	{
		return;
	}

	m_bConnected=FALSE;
	//����CommProc�߳���WaitSingleObject�����ĵȴ�
	//SetEvent(m_hPostMsgEvent); 

	//����CommProc�߳���WaitCommEvent�ĵȴ�
	SetCommMask(m_hCom, 0); 

	//�ȴ������߳���ֹ
	//WaitForSingleObject(m_pThread->m_hThread, INFINITE);
	//m_pThread=NULL;
	CloseHandle(m_hCom);
}

//��д����
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

//��д����
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
			GetOverlappedResult(m_hCom, &m_osWrite, &length, FALSE); //�ȴ�
		}
		else
		{
			length = 0;
		}
	}

	return length;
}

//д�����豸
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
	fState = WriteFile(m_hCom, buf, length, &length, &m_osWrite); //���óɹ�,���ط�0; ���ò��ɹ�,����Ϊ0
	/*
	BOOL WriteFile(
	HANDLE hFile,                   // �ļ����
	LPCVOID lpBuffer,               // ���ݻ�����ָ��
	DWORD nNumberOfBytesToWrite,    // ��Ҫд���ֽ���
	LPDWORD lpNumberOfBytesWritten, // ���ڱ���ʵ��д���ֽ����Ĵ洢�����ָ��
	LPOVERLAPPED lpOverlapped       // OVERLAPPED�ṹ��ָ��
	)
	*/

	//WriteFile ����ʧ��
	if (!fState)
	{
		if (GetLastError()==ERROR_IO_PENDING)
		{
			//�ȴ�
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

//���ô���
void SerialPort::SetBaud(long newBaud)
{
	m_nBaud = newBaud;
	ConfigConnection();
}

//���ô���
void SerialPort::SetPort(int nDataBits, int nParity, int nStopBits)
{
	m_nDataBits = nDataBits;
	m_nParity = nParity;
	m_nStopBits = nStopBits;
	ConfigConnection();
}

//�����Ƿ�򿪴���
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
	//PURGE_TXABORT �����ж�δ��ɵ�д����
	//PURGE_TXCLEAR �������Output buffer
}

BOOL SerialPort::PurgeReadbuf()
{
	if (!m_bConnected)
	{
		return FALSE;
	}

	return PurgeComm(m_hCom, PURGE_RXABORT|PURGE_RXCLEAR);
	//PURGE_RXABORT �����ж�δ��ɵĶ�����
	//PURGE_RXCLEAR �������input buffer
}

void SerialPort::SetPortNo(int& newPortNo)
{
	if (newPortNo<1)
	{
		AfxMessageBox("���ںŲ���С��1");
		return;
	}

	CString buf;
	buf.Format("COM%d",newPortNo); //�˿�
	if(buf == m_sPort)
		return;	

	if(m_bConnected)
	{
		CloseConnection();	//�ȹرվɶ˿�
		m_sPort = buf;		//���¶˿�
		OpenConnection();	//�ٴ��¶˿�
	}
	else
	{
		m_sPort = buf; //���¶˿�
	}
}

//�Ӵ����ж�ȡ�����������ַ��������������������������ʱ����false
BOOL SerialPort::ReadComByNum(char *buf, DWORD dwLength, DWORD *Length, DWORD TimeOut)
{
	DWORD st = GetTickCount();

	DWORD LengthReturn = 0;	//�Ӵ�����һ��ʵ�ʶ������ֽ���
	*Length = 0;			//�Ӵ�����ʵ�ʶ������ֽ���

	*Length = *Length + LengthReturn;

	do
	{
		LengthReturn = ReadComm(buf+(*Length),dwLength - *Length); //������
		*Length = *Length + LengthReturn; //����

		if(TimeOut == 0) break;
		if(dwLength == *Length)
			break;
	}while(GetTickCount() - st < TimeOut);

	return (dwLength == *Length?TRUE:FALSE);
}

BOOL SerialPort::ReadComByEndChar(char *buf, DWORD MaxLen, char endchar, DWORD *Length, DWORD TimeOut)
{
	DWORD st = GetTickCount();

	DWORD LengthReturn = 0;	//�Ӵ�����һ��ʵ�ʶ������ֽ���
	*Length = 0;			//�Ӵ�����ʵ�ʶ������ֽ���

	*Length = *Length + LengthReturn;

	do
	{
		if(MaxLen == *Length)
			break;

		LengthReturn = ReadComm(buf+(*Length),1);//������,ÿ�ζ�һ���ֽ�

		if(LengthReturn>0)
		{
			*Length = *Length + LengthReturn;	//����

			if(buf[*Length-1] == endchar)
				return TRUE;

			if(TimeOut == 0)
				continue;	//��timeoutΪ0,��ѻ��������ݶ���

			st = GetTickCount();	//���ڶ�������������,������ʼʱ��
		}

	}while(GetTickCount() - st < TimeOut);

	return FALSE;
}