#include "StdAfx.h"
#include "SerialPort.h"
#include "ComLaser.h"


CComLaser::CComLaser(void)
{
	m_pComPort = NULL;
}


CComLaser::~CComLaser(void)
{
	m_pComPort = NULL;
}

int CComLaser::SetComPort(SerialPort* pPort)
{	
	if (NULL == pPort)
	{
		return 0;
	}

	m_pComPort = pPort;
	return 1;
}

int CComLaser::ReadHeightData(double &dVal)
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

	szBuf[0] = 0x02;
	szBuf[1] = 'M';
	szBuf[2] = 'E';
	szBuf[3] = 'A';
	szBuf[4] = 'S';
	szBuf[5] = 'U';
	szBuf[6] = 'R';
	szBuf[7] = 'E';
	szBuf[8] = 0x03;
	strSend.Format("%s", szBuf);
	m_pComPort->PurgeReadbuf();
	nActWriteLength = m_pComPort->WriteComm((LPSTR)(LPCTSTR)strSend, strSend.GetLength());
	if (nActWriteLength <= 0)
	{
		strTemp.Format("���ýӿ�WriteComm()��ȡ�������ʱ����,���س���:[%d]ʧ��!", nActWriteLength);
		AfxMessageBox(strTemp);
		return 0;
	}
	Sleep(100);

	while ((GetTickCount()-lgStarttime) <= dwDelaytime)
	{
		nActRecLength = m_pComPort->ReadComm(szBuf, sizeof(szBuf)-1);
		if (nActRecLength >= 9)
		{
			strTemp.Format("%s", szBuf);
			if (szBuf[nActRecLength-1] == 0x03)
			{
				szBuf[0] = '0';
				szBuf[nActRecLength-1] = '\n';
				dVal = atof(szBuf);
				return 1;
			}
		}
		else
		{
			CFunction::DelaySec(50);
		}
	}

	// ��ʱû�ж�������
	return 0;
}