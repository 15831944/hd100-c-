#pragma once
class CComWeight
{
public:
	CComWeight(void);
	~CComWeight(void);

	// 配置电子秤连接的COM口
	SerialPort* m_pComPort;
	int SetComPort(SerialPort* pPort);

	// 电子秤置零
	int SetZero();

	// 电子秤读数
	int GetWeight(double &dVal);

	int SetImdRpt();
};

