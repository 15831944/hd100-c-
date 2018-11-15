#include "StdAfx.h"
#include "KFTimer.h"


CKFTimer::CKFTimer(void)
{
	TimerStart();
}


CKFTimer::~CKFTimer(void)
{
}

LONGLONG CKFTimer::GetCurrentCounter()
{
	LARGE_INTEGER lStartCount;
	QueryPerformanceCounter(&lStartCount);
	return lStartCount.QuadPart;
}

LONGLONG CKFTimer::GetCurrentFrequency()
{
	LARGE_INTEGER lFrequence;
	QueryPerformanceFrequency(&lFrequence);

	return lFrequence.QuadPart;
}

double CKFTimer::GetCurrentTimeEx()
{
	return double(GetCurrentCounter())/ GetCurrentFrequency();
}

void CKFTimer::TimerStart()
{
	m_StartCounter = GetCurrentCounter();
}

double CKFTimer::TimerGetMs()
{
	return double(GetCurrentCounter()-m_StartCounter)*1000 / (double)GetCurrentFrequency();
}

double CKFTimer::TimerGetSecond()
{
	return double(GetCurrentCounter()-m_StartCounter) / (double)GetCurrentFrequency();
}

double CKFTimer::TimerGetMinute()
{
	return double(GetCurrentCounter()-m_StartCounter) / (double)GetCurrentFrequency() / 60;
}
