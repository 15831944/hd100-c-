#pragma once
class CComLight
{
public:
	CComLight(void);
	~CComLight(void);

	// ���ù�Դ���ӵ�COM��
	SerialPort* m_pComPort;
	int SetComPort(SerialPort* pPort);

	int m_nLightType;	// ��Դ����  0,1

	int SetLightPower(int nVal);
	int GetLightPower(int &nVal);
};

