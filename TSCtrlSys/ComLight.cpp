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

int CComLight::SetLightPower(int nValCh1, int nValCh2)
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
		if (0<=nValCh1 && nValCh1<=255 && 0<=nValCh2 && nValCh2<=255)
		{
			strSend.Format("SA%04d#SB%04d#", nValCh1, nValCh2);
			strSend += "\r\n";
		}
		else
		{
			AfxMessageBox(_T("��Դ����ֵ������Χ!"));
			return 0;
		}
	}
	else
	{
		if (0<nValCh1 && nValCh1<100)
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


int CComLight::GetLightPower(int &nValCh1, int &nValCh2)
{
	nValCh1 = 0; nValCh2 = 0;
	if (NULL == m_pComPort)
	{
		return 0;
	}

	CString strGet = "";
	CString strSend = "";
	CString strTemp = "";
	char szBuf[128] = {0};


	//������ʱ�������ѭ��
	DWORD dwDelaytime = 2000;

	int nActWriteLength = 0;	// ʵ�ʷ��ͳɹ����ݳ���
	int nActRecLength = 0;		// ʵ�ʽ��ܳɹ����ݳ���

	//////////////////////////////////////////////////////////////////////////
	//							ͨ��1										//
	//////////////////////////////////////////////////////////////////////////
	m_pComPort->PurgeReadbuf();
	m_pComPort->PurgeWritebuf();
	if (m_nLightType == 1)
	{
		strSend = "SA#\r\n";
	}
	else
	{
		strSend = "@RD1\r\n";
	}
	DWORD lgStarttime = GetTickCount();
	nActWriteLength = m_pComPort->WriteComm((LPSTR)(LPCTSTR)strSend, strSend.GetLength());
	if (nActWriteLength <= 0)
	{
		strTemp.Format("���ýӿ�WriteComm()��ȡ��Դ1����ʱ,���س���:[%d]ʧ��!", nActWriteLength);
		AfxMessageBox(strTemp);
		return 0;
	}
	//CFunction::DelayEx(50);

	while ((GetTickCount()-lgStarttime) <= dwDelaytime)
	{
		nActRecLength = m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
		if (nActRecLength <= 0)
		{
			Sleep(100);
			continue;
		}

		if (1 == m_nLightType)
		{
			nValCh1 = atoi(&szBuf[1]);
			break;
		}
		else
		{
			nValCh1 = atoi(&szBuf[1]);
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//							ͨ��2										//
	//////////////////////////////////////////////////////////////////////////
	m_pComPort->PurgeReadbuf();
	m_pComPort->PurgeWritebuf();

	if (m_nLightType == 1)
	{
		strSend = "SB#\r\n";
	}
	else
	{
		strSend = "@RD2\r\n";
	}
	lgStarttime = GetTickCount();
	nActWriteLength = m_pComPort->WriteComm((LPSTR)(LPCTSTR)strSend, strSend.GetLength());
	if (nActWriteLength <= 0)
	{
		strTemp.Format("���ýӿ�WriteComm()��ȡ��Դ2����ʱ,���س���:[%d]ʧ��!", nActWriteLength);
		AfxMessageBox(strTemp);
		return 0;
	}
	//CFunction::DelayEx(50);

	while ((GetTickCount()-lgStarttime) <= dwDelaytime)
	{
		nActRecLength = m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
		if (nActRecLength <= 0)
		{
			Sleep(100);
			continue;
		}

		if (1 == m_nLightType)
		{
			nValCh2 = atoi(&szBuf[1]);
			break;
		}
		else
		{
			nValCh2 = atoi(&szBuf[1]);
			break;
		}
	}

	return 1;
}