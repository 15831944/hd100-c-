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
// 	SerialPort(int ComNo     = 1,		  //串口号
// 			   DWORD nBaud   = 9600,	  //波特率
// 			   int nDataBits = 8,		  //数据位
// 			   int nParity   = NOPARITY,  //奇校验0: NOPARITY;1: EVENPARITY;2 ODDPARITY;
// 			   int nStopBits = ONESTOPBIT //停止位
// 			   );
	virtual ~SerialPort();

//////////////////////////////////////////////////////////////////////////
public:
	volatile HANDLE m_hCom;          //串行口句柄
	
	//CWinThread* m_pThread;         //代表辅助线程
	volatile BOOL m_bConnected;      //标志和串口的连接状态 
	//volatile HWND m_hTermWnd;      //接收WM_COMMNOTIFY消息的视图窗口 
	//volatile HANDLE m_hPostMsgEvent;
	//用于WM_COMMNOTIFY消息的事件对象，事件处于有信号时才可以发送消息
	
	OVERLAPPED m_osRead,m_osWrite;   //用于重叠读/写
	
	CString m_szPortName;			//外部定义的端口名称
	int		m_nComNo;				//端口序号
	DWORD	m_nBaud;				//波特率
	int		m_nDataBits;			//数据位
	BOOL	m_bEcho;				//是否回显
	int		m_nFlowCtrl;			//流量控制 
	BOOL	m_bNewLine;				//是否另起一行
	int		m_nParity;				//奇偶校验
	int		m_nStopBits;			//停止位
	CString m_sPort;				//端口名称

//////////////////////////////////////////////////////////////////////////
public:
	//初始化串口
	BOOL Init(int ComNo, DWORD nBaud, int nDataBits, int nParity, int nStopBits);

	BOOL IsConnected();				 //返回是否打开串口
    BOOL ConfigConnection();		 //设置串口设备
	BOOL OpenConnection();			 //打开串口设备
	void CloseConnection();			 //关闭串口设备
	
	//设置串口
	void SetPortNo(int& newPortNo);
	void SetBaud(long newBaud);
	void SetPort(int nDataBits, int nParity, int nStopBits);
	
	//读写串口
	DWORD ReadCommByByte(BYTE *buf, DWORD dwLength);
	DWORD ReadComm(char *buf, DWORD dwLength);        //读串口设备
	//从串口中读取给定数量的字符，必须读到给定的数量，否则超时返回FALSE
	BOOL ReadComByNum(char *buf, DWORD dwLength, DWORD *Length, DWORD TimeOut=0);
	//从串口中读取给定结束字符的一段内容,必须读到给定结束字符,否则超时返回FALSE,达到最大长度也返回
	BOOL ReadComByEndChar(char *buf, DWORD MaxLen, char endchar, DWORD *Length, DWORD TimeOut=100);
	
	//写串口设备
	DWORD WriteComm(char *buf, DWORD dwLength);
	
	BOOL PurgeWritebuf();
	BOOL PurgeReadbuf();
};

#endif // !defined(AFX_SERIALPORT_H__EA5D3C5F_A302_4BBA_BF82_5C3455CA5100__INCLUDED_)
