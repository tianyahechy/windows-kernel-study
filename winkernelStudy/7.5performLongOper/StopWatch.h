#pragma once
#include <Windows.h>

class CStopWatch
{
public:
	CStopWatch();
	~CStopWatch();
	
	void start();

	__int64 Now() const;

private:
	LARGE_INTEGER m_liPerFreq;
	LARGE_INTEGER m_liPerfStart;
};

