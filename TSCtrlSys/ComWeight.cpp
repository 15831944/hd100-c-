#include "StdAfx.h"
#include "SerialPort.h"
#include "ComWeight.h"
#include "StringParser.h"

CComWeight::CComWeight(void)
{
	m_pComPort = NULL;
}


CComWeight::~CComWeight(void)
{
	m_pComPort = NULL;
}


int CComWeight::SetComPort(SerialPort* pPort)
{
	if (NULL == pPort)
	{
		return 0;
	}

	m_pComPort = pPort;
	return 1;
}

// 电子秤置零
int CComWeight::SetZero()
{
	if (NULL == m_pComPort)
	{
		return 0;
	}

	CString strGet = "";
	CString strSend = "";
	CString strTemp = "";

	int nActWriteLength = 0;	// 实际发送成功数据长度

	strSend = "ZI\r\n";

	nActWriteLength = m_pComPort->WriteComm((LPSTR)(LPCTSTR)strSend, strSend.GetLength());
	if (nActWriteLength <= 0)
	{
		strTemp.Format("调用接口WriteComm()电子秤置零时,返回长度:[%d]失败!", nActWriteLength);
		AfxMessageBox(strTemp);
		return 0;
	}

	return 1;
}

// 电子秤读数
int CComWeight::GetWeight(double &dVal)
{
	if (NULL == m_pComPort)
	{
		return 0;
	}
	m_pComPort->PurgeReadbuf();
	CFunction::DelaySec(0.1);
	CString strGet = "";
	CString strSend = "";
	CString strTemp = "";
	char szBuf[128] = {0};
//	int len = sizeof(szBuf);

	DWORD lgStarttime = GetTickCount();

	//超过此时间就跳出循环
	DWORD dwDelaytime = 2000;

	int nActRecLength = 0;		// 实际接受成功数据长度
	while ((GetTickCount()-lgStarttime) <= dwDelaytime)
	{
		nActRecLength = m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
		if (nActRecLength < 1)
		{
			Sleep(100);
			continue;
		}

		std::string tmpStr(szBuf);
		std::vector<std::string> strVec;
		StringParser::GetParamFromString(tmpStr, strVec, 'g');
		std::string strTmp[10];
		int strNum = strVec.size();
		if (strNum < 3)	// 没有读到有效的重量值
		{
			return 0;
		}

		for (int i=1; i<min(strNum-1, 10); i++)
		{
			strTmp[i] = strVec.at(i);
			size_t stIndex = strTmp[i].find_last_of('D');
			if (-1 == stIndex)
			{
				stIndex = strTmp[i].find_last_of('S');
			}
			size_t endIndex = strTmp[i].find_last_of('m');
			if (endIndex <= stIndex)
			{
				continue;
			}
			strTemp.Format("%s", strTmp[i].substr(stIndex+1, endIndex-stIndex-1).c_str());
			dVal = atof(strTemp);
		}

		break;
	}

	return 1;
}


int CComWeight::SetImdRpt()
{
	if (NULL == m_pComPort)
	{
		return 0;
	}

	CString strGet = "";
	CString strSend = "";
	CString strTemp = "";

	int nActWriteLength = 0;	// 实际发送成功数据长度

	strSend = "SIR\r\n";

	nActWriteLength = m_pComPort->WriteComm((LPSTR)(LPCTSTR)strSend, strSend.GetLength());
	if (nActWriteLength <= 0)
	{
		strTemp.Format("调用接口WriteComm()电子秤置零时,返回长度:[%d]失败!", nActWriteLength);
		AfxMessageBox(strTemp);
		return 0;
	}

	m_pComPort->PurgeReadbuf();
	return 1;
}