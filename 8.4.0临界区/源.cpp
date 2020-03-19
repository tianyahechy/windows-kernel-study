
#include <Windows.h>
const int maxTimes = 1000;
int g_nIndex = 0;
DWORD g_dwTimes[maxTimes];
CRITICAL_SECTION g_cs;

DWORD WINAPI firstThread(PVOID pvParam)
{
	while (g_nIndex < maxTimes)
	{
		EnterCriticalSection(&g_cs);
		g_dwTimes[g_nIndex] = GetTickCount();
		g_nIndex++;
		LeaveCriticalSection(&g_cs);
	}
	return 0;
}

DWORD WINAPI secondThread(PVOID pvParam)
{

	while (g_nIndex < maxTimes)
	{
		EnterCriticalSection(&g_cs);
		g_nIndex++;
		g_dwTimes[g_nIndex--] = GetTickCount();
		LeaveCriticalSection(&g_cs);
	}
	return 0;
}