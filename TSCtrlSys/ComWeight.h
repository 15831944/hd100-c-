#pragma once
class CComWeight
{
public:
	CComWeight(void);
	~CComWeight(void);

	// ���õ��ӳ����ӵ�COM��
	SerialPort* m_pComPort;
	int SetComPort(SerialPort* pPort);

	// ���ӳ�����
	int SetZero();

	// ���ӳӶ���
	int GetWeight(double &dVal);

	int SetImdRpt();
};

