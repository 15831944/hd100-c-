#pragma once
class CComLaser
{
public:
	CComLaser(void);
	~CComLaser(void);

	// ���ü����������ӵ�COM��
	SerialPort* m_pComPort;
	int SetComPort(SerialPort* pPort);

	int ReadHeightData(double &dVal);
};

