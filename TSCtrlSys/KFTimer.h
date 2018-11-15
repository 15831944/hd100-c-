#pragma once
class CKFTimer
{
public:
	CKFTimer(void);
	~CKFTimer(void);	

	static LONGLONG GetCurrentCounter();
	static LONGLONG GetCurrentFrequency();
	static double GetCurrentTimeEx();//S

	//////////////////////////////////////////////////////////////////////////
	// ��ʱ������
	void TimerStart();
	double TimerGetMs();
	double TimerGetSecond();
	double TimerGetMinute();
	LONGLONG m_StartCounter;
};

