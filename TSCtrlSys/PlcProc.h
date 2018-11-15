#pragma once
#include "SerialPort.h"

class CPlcProc/* :
	public CSerialPort*/
{
public:
	CPlcProc(void);
	~CPlcProc(void);
	CSerialPort m_Port;
public:
	BOOL		Init(CWnd* pPortOwner, UINT portnr = 1, UINT baud = 19200, char parity = 'N', UINT databits = 8, UINT stopbits = 1, DWORD dwCommEvents = EV_RXCHAR, UINT writebuffersize = 1024);
	long		ReadPlcData(CString PLCDataID);				//读取双字节数据DT1-DT2
	long		ChangStrTOlong(CString ChangStr);
	CString		AddZero(CString str, int StrCout);			//加零
	BOOL		WritePlcYOut(CString OutID, bool on_off);
	BOOL		WritePlcROut(CString OutID, BOOL on_off);
	void		DotPLCR(CString PlcRID);
	//void		SendPLCR(int BtID, bool on_off);
	//void		ChangeOutCh(bool m_WID);
	BOOL		WritePLCData(CString DTID, int DateVal);
	BOOL		CommSendPlc(CString StrPlc);

	int			m_WordID;
};

struct Tag_PlcError  
{ 
	int  ErrorID;
	CString ErrorText;
};

struct Tag_PlcSataut  
{
	int  SatautID;
	CString SatautText;
};

class CPlcCommInfo /*: public CPlcProc*/
{
public:
	CPlcCommInfo();
	~CPlcCommInfo();
public:
	BOOL ReadParamFile();
	BOOL StartComm();
	BOOL StopComm();
private:
	CWinThread  *m_Thread;

public:
	Tag_PlcError	*PlcError;
	Tag_PlcSataut	*PlcSataut;
	int				ErrorCount;
	int				SatautCount;
	CString			*pReserve;
	int				ReserveCount;
	int				ErrorVal;
	CString			m_Str[10];
	int				PlcRunID;
	int				PlcErrorID;
};