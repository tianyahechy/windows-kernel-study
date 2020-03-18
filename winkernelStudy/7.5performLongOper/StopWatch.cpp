#include "StopWatch.h"

CStopWatch::CStopWatch()
{
	QueryPerformanceFrequency(&m_liPerFreq);
	start();
}

CStopWatch::~CStopWatch()
{
}

void CStopWatch::start()
{
	QueryPerformanceCounter(&m_liPerfStart);
}

__int64 CStopWatch::Now() const
{
	LARGE_INTEGER liPerfNow;
	QueryPerformanceCounter(&liPerfNow);

	__int64 theInterval = (liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000 / m_liPerFreq.QuadPart;
	return theInterval;
}
