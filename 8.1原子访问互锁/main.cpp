#include <Windows.h>
#include <iostream>

long g_x = 0;
BOOL g_fReourceInUse = FALSE;

DWORD WINAPI ThreadFunc1(PVOID pvParam)
{
	InterlockedExchangeAdd(&g_x, 1);
	return 0;
}

void func2()
{
	while (InterlockedExchange(&g_fReourceInUse, TRUE) == TRUE)
	{
		Sleep(0);
	}
	InterlockedExchange(&g_fReourceInUse, FALSE);
}

LONG InterLockedCompareExchange(PLONG plDestination, LONG lExchange, LONG lCompared)
{
	LONG lRet = *plDestination;
	if (*plDestination == lCompared)
	{
		*plDestination = lExchange;
	}
	return lRet;
}


