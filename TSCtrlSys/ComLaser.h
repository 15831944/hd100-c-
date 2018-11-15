#pragma once
class CComLaser
{
public:
	CComLaser(void);
	~CComLaser(void);

	// 配置激光测高仪连接的COM口
	SerialPort* m_pComPort;
	int SetComPort(SerialPort* pPort);

	int ReadHeightData(double &dVal);
};

