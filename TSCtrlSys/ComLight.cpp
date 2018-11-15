#include "StdAfx.h"
#include "SerialPort.h"
#include "ComLight.h"


CComLight::CComLight(void)
{
	m_pComPort = NULL;
	m_nLightType = 1;
}


CComLight::~CComLight(void)
{
	m_pComPort = NULL;
}


int CComLight::SetComPort(SerialPort* pPort)
{
	if (NULL == pPort)
	{
		return 0;
	}

	m_pComPort = pPort;
	return 1;
}

int CComLight::SetLightPower(int nVal)
{
	if (NULL == m_pComPort)
	{
		return 0;
	}
	
	CString strGet = "";
	CString strSend = "";
	CString strTemp = "";
	
	int nActWriteLength = 0;	// ʵ�ʷ��ͳɹ����ݳ���

	if (1 == m_nLightType)
	{
		if (0<nVal && nVal<255)
		{
			strSend.Format("SA%04d#", nVal);
			strSend += "\r\n";
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (0<nVal && nVal<100)
		{
			strSend.Format("@WR1%04d");
			strSend += "\r\n";
		}
		else
		{
			return 0;
		}
	}

	nActWriteLength = m_pComPort->WriteComm((LPSTR)(LPCTSTR)strSend, strSend.GetLength());
	if (nActWriteLength <= 0)
	{
		strTemp.Format("���ýӿ�WriteComm()���ù�Դ����ʱ,���س���:[%d]ʧ��!", nActWriteLength);
		AfxMessageBox(strTemp);
		return 0;
	}

	return 1;
}


int CComLight::GetLightPower(int &nVal)
{
	if (NULL == m_pComPort)
	{
		return 0;
	}

	CString strGet = "";
	CString strSend = "";
	CString strTemp = "";
	char szBuf[128] = {0};

	DWORD lgStarttime = GetTickCount();

	//������ʱ�������ѭ��
	DWORD dwDelaytime = 2000;

	int nActWriteLength = 0;	// ʵ�ʷ��ͳɹ����ݳ���
	int nActRecLength = 0;		// ʵ�ʽ��ܳɹ����ݳ���

	if (m_nLightType == 1)
	{
		strSend = "SA#\r\n";
	}
	else
	{
		strSend = "@RD1\r\n";
	}
	nActWriteLength = m_pComPort->WriteComm((LPSTR)(LPCTSTR)strSend, strSend.GetLength());
	if (nActWriteLength <= 0)
	{
		strTemp.Format("���ýӿ�WriteComm()��ȡ��Դ����ʱ,���س���:[%d]ʧ��!", nActWriteLength);
		AfxMessageBox(strTemp);
		return 0;
	}
	//CFunction::DelayEx(50);

	while ((GetTickCount()-lgStarttime) <= dwDelaytime)
	{
		nActRecLength = m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
		if (nActRecLength < 0)
		{
			Sleep(100);
			continue;
		}

		if (1 == m_nLightType)
		{
			nVal = atoi(&szBuf[1]);
		}
		else
		{
			nVal = atoi(&szBuf[1]);
		}
	}

	return 1;
}