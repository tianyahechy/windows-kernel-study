#include <Windows.h>
#include "StopWatch.h"
__int64 FileTimeToQuadWord(PFILETIME pft)
{
	return (Int64ShllMod32(pft->dwHighDateTime, 32) | pft->dwLowDateTime);
}

void performLonOperation()
{
	FILETIME fkKernelTimeStart;
	FILETIME ftKernelTimeEnd;
	FILETIME ftUserTimeStart;
	FILETIME ftUserTimeEnd;
	FILETIME ftDummy;

	__int64 qwKernelTimeElapsed, qwUserTimeElapsed, qwTotalTimeElapsed;

	//�õ���ʼʱ��
	GetThreadTimes(GetCurrentThread(), &ftDummy, &ftDummy, &fkKernelTimeStart, &ftUserTimeStart);

	//ִ��
	int a = 0;
	for (int i = 0; i < 100000000; i++)
	{
		a++;
	}
	//�õ�����ʱ��
	GetThreadTimes(GetCurrentThread(), &ftDummy, &ftDummy, &ftKernelTimeEnd, &ftUserTimeEnd);

	qwKernelTimeElapsed = FileTimeToQuadWord(&ftKernelTimeEnd) - FileTimeToQuadWord(&fkKernelTimeStart);
	qwUserTimeElapsed = FileTimeToQuadWord(&ftUserTimeEnd) - FileTimeToQuadWord(&ftUserTimeStart);

	qwTotalTimeElapsed = qwKernelTimeElapsed + qwUserTimeElapsed;
}



int main()
{
	//performLonOperation();
	CStopWatch theWatch;

	int a = 0;
	for (int i = 0; i < 100000000; i++)
	{
		a++;
	}
	theWatch.Now();
	return 0;
}