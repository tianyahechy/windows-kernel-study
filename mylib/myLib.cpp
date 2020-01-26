#include <Windows.h>

#define MYLIBAPI extern "C" __declspec( dllexport)
#include "myLIB.h"

int g_nResult;
int add(int nLeft, int nRight)
{
	g_nResult = nLeft + nRight;
	return g_nResult;
}
int main()
{
	DWORD dwTlsIndex = TlsAlloc();

	TlsSetValue(dwTlsIndex, (PVOID)12345);
	TlsFree(dwTlsIndex);
	dwTlsIndex = TlsAlloc();
	PVOID pvSomeValue = TlsGetValue(dwTlsIndex);
	return 0;
}
