// SerialPort.h: interface for the SerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORT_H__EA5D3C5F_A302_4BBA_BF82_5C3455CA5100__INCLUDED_)
#define AFX_SERIALPORT_H__EA5D3C5F_A302_4BBA_BF82_5C3455CA5100__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//add by magezhou begin
#define MAXBLOCK	8192
#define XON			0x11
#define XOFF		0x13
//add by magezhou end--

class SerialPort  
{
public:
	SerialPort();
// 	SerialPort(int ComNo     = 1,		  //���ں�
// 			   DWORD nBaud   = 9600,	  //������
// 			   int nDataBits = 8,		  //����λ
// 			   int nParity   = NOPARITY,  //��У��0: NOPARITY;1: EVENPARITY;2 ODDPARITY;
// 			   int nStopBits = ONESTOPBIT //ֹͣλ
// 			   );
	virtual ~SerialPort();

//////////////////////////////////////////////////////////////////////////
public:
	volatile HANDLE m_hCom;          //���пھ��
	
	//CWinThread* m_pThread;         //�������߳�
	volatile BOOL m_bConnected;      //��־�ʹ��ڵ�����״̬ 
	//volatile HWND m_hTermWnd;      //����WM_COMMNOTIFY��Ϣ����ͼ���� 
	//volatile HANDLE m_hPostMsgEvent;
	//����WM_COMMNOTIFY��Ϣ���¼������¼��������ź�ʱ�ſ��Է�����Ϣ
	
	OVERLAPPED m_osRead,m_osWrite;   //�����ص���/д
	
	CString m_szPortName;			//�ⲿ����Ķ˿�����
	int		m_nComNo;				//�˿����
	DWORD	m_nBaud;				//������
	int		m_nDataBits;			//����λ
	BOOL	m_bEcho;				//�Ƿ����
	int		m_nFlowCtrl;			//�������� 
	BOOL	m_bNewLine;				//�Ƿ�����һ��
	int		m_nParity;				//��żУ��
	int		m_nStopBits;			//ֹͣλ
	CString m_sPort;				//�˿�����

//////////////////////////////////////////////////////////////////////////
public:
	//��ʼ������
	BOOL Init(int ComNo, DWORD nBaud, int nDataBits, int nParity, int nStopBits);

	BOOL IsConnected();				 //�����Ƿ�򿪴���
    BOOL ConfigConnection();		 //���ô����豸
	BOOL OpenConnection();			 //�򿪴����豸
	void CloseConnection();			 //�رմ����豸
	
	//���ô���
	void SetPortNo(int& newPortNo);
	void SetBaud(long newBaud);
	void SetPort(int nDataBits, int nParity, int nStopBits);
	
	//��д����
	DWORD ReadCommByByte(BYTE *buf, DWORD dwLength);
	DWORD ReadComm(char *buf, DWORD dwLength);        //�������豸
	//�Ӵ����ж�ȡ�����������ַ��������������������������ʱ����FALSE
	BOOL ReadComByNum(char *buf, DWORD dwLength, DWORD *Length, DWORD TimeOut=0);
	//�Ӵ����ж�ȡ���������ַ���һ������,����������������ַ�,����ʱ����FALSE,�ﵽ��󳤶�Ҳ����
	BOOL ReadComByEndChar(char *buf, DWORD MaxLen, char endchar, DWORD *Length, DWORD TimeOut=100);
	
	//д�����豸
	DWORD WriteComm(char *buf, DWORD dwLength);
	
	BOOL PurgeWritebuf();
	BOOL PurgeReadbuf();
};

#endif // !defined(AFX_SERIALPORT_H__EA5D3C5F_A302_4BBA_BF82_5C3455CA5100__INCLUDED_)
