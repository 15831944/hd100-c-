#pragma once
#include "SerialPort.h"

// ��ط�ʹ�ã�intת��Ϊ16���ƵĶ�λ������ת��ΪASCII��  ch ���ٰ���2���ַ�
int Int2Asc16Bit(int Val, char* ch);	

typedef struct DispenserParam
{
	/// ����ʱ��,10�͵���1.0ms
	int Rising;

	/// ��ʱ��,10�͵���1.0ms
	int OpenTime;

	/// �½�ʱ��,15�͵���0.15ms
	int Falling;

	/// ײ������,90����90%
	int NeedleLift;

	/// ������
	int NumOfPul;

	/// �ӳ�,8�͵���0.8ms
	int Delay;

	/// �¶�,60����60��
	int Temp;

	/// ��ѹ����λ1kPa
	int Presure;

	/// ��ط��³�ͨ��
	int Musashi_Channel;
	/// ��ط���ģʽ		���:0  ����:1
	int Musashi_Main;
	/// ��ط���ģʽ		�����úù���:0	�ֶ�:1
	int Musashi_Sub;
	/// ��ط�����ʱ��
	int Musashi_DotOpen;
	/// ��ط�����
	int Musashi_Up;
	/// ��ط�����
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
		//��ط��³�ͨ��
		Musashi_Channel = 0;
		//��ط���ģʽ		���:0  ����:1
		Musashi_Main = 1;
		//��ط���ģʽ		�����úù���:0	�ֶ�:1
		Musashi_Sub = 0;
		//��ط�����ʱ��
		Musashi_DotOpen = 10;
		//��ط�����
		Musashi_Up = 6;
		//��ط�����
		Musashi_Down = 6;
	}
} _DispenserParam;

enum DispenserType
{
	Disp_Vermes,	// Vermes���䷧
	Disp_Kaifa,		// ����PLC���Ʒ�
	Disp_Musashi,	// ������䷧
	Disp_Yikai		// �⿭���䷧
};

class CComDisp
{
public:
	CComDisp();	// index:��������   ��0��ʼ
	~CComDisp(void);

	// index:��������   ��0��ʼ
	int m_nIndex;

	// ��������
	DispenserParam m_dispParam;
	int HandleParamFile(BOOL bLoad);		// ���ļ����ؽ��������򱣴�������ļ�		bLoad : true= ����   false=����
	int HandleParamDispenser(BOOL bLoad);	// �ӽ�����ȡ���������ò������㽺��			bLoad : true= ����   false=����
	double m_cycTimeMs;
	double m_freq;

	// ��������
	DispenserType m_eDispType;
	// ���ý�������
	void SetDispenserType(DispenserType eType);

	// ���ý������ӵ�COM��
	SerialPort* m_pComPort;
	int SetComPort(SerialPort* pPort);

	// ���ý���		idx��0��ʼ
	int AttachDispenser(int idx, DispenserType dispType, SerialPort* pPort);
	// ��ȡ����ʱ��
	int GetCycleTimeAndFreq(double &cycTimeMs, double &freq);
};

