#pragma once
#include "SerialPort.h"

// 武藏阀使用，int转换为16进制的二位数，再转换为ASCII，  ch 至少包含2个字符
int Int2Asc16Bit(int Val, char* ch);	

typedef struct DispenserParam
{
	/// 上升时间,10就等于1.0ms
	int Rising;

	/// 打开时间,10就等于1.0ms
	int OpenTime;

	/// 下降时间,15就等于0.15ms
	int Falling;

	/// 撞针升程,90等于90%
	int NeedleLift;

	/// 脉冲数
	int NumOfPul;

	/// 延迟,8就等于0.8ms
	int Delay;

	/// 温度,60等于60度
	int Temp;

	/// 备压，单位1kPa
	int Presure;

	/// 武藏阀吐出通道
	int Musashi_Channel;
	/// 武藏阀主模式		打点:0  划线:1
	int Musashi_Main;
	/// 武藏阀副模式		按设置好工作:0	手动:1
	int Musashi_Sub;
	/// 武藏阀打点打开时间
	int Musashi_DotOpen;
	/// 武藏阀上行
	int Musashi_Up;
	/// 武藏阀下行
	int Musashi_Down;

	DispenserParam()
	{
		Rising		= 0;
		OpenTime	= 0;
		Falling		= 0;
		NeedleLift	= 0;
		NumOfPul	= 0;
		Delay		= 0;
		Temp		= 0;
		Presure		= 0;
		//武藏阀吐出通道
		Musashi_Channel = 0;
		//武藏阀主模式		打点:0  划线:1
		Musashi_Main = 1;
		//武藏阀副模式		按设置好工作:0	手动:1
		Musashi_Sub = 0;
		//武藏阀打点打开时间
		Musashi_DotOpen = 10;
		//武藏阀上行
		Musashi_Up = 6;
		//武藏阀下行
		Musashi_Down = 6;
	}
} _DispenserParam;

enum DispenserType
{
	Disp_Vermes,	// Vermes喷射阀
	Disp_Kaifa,		// 开发PLC控制阀
	Disp_Musashi,	// 武藏喷射阀
	Disp_Yikai		// 意凯喷射阀
};

class CComDisp
{
public:
	CComDisp();	// index:胶阀索引   从0开始
	~CComDisp(void);

	// index:胶阀索引   从0开始
	int m_nIndex;

	// 胶阀参数
	DispenserParam m_dispParam;
	int HandleParamFile(BOOL bLoad);		// 从文件加载胶阀参数或保存参数到文件		bLoad : true= 加载   false=保存
	int HandleParamDispenser(BOOL bLoad);	// 从胶阀读取参数或设置参数到点胶阀			bLoad : true= 加载   false=保存
	double m_cycTimeMs;
	double m_freq;

	// 胶阀类型
	DispenserType m_eDispType;
	// 设置胶阀类型
	void SetDispenserType(DispenserType eType);

	// 配置胶阀连接的COM口
	SerialPort* m_pComPort;
	int SetComPort(SerialPort* pPort);

	// 配置胶阀		idx从0开始
	int AttachDispenser(int idx, DispenserType dispType, SerialPort* pPort);
	// 获取周期时间
	int GetCycleTimeAndFreq(double &cycTimeMs, double &freq);
};

