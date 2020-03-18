#include <Windows.h>

DWORD WINAPI ChildThread(PVOID pvParam)
{
	HANDLE hThreadParent = (HANDLE)pvParam;

	FILETIME ftCreationTime, ftExitTime, ftKernelTime, ftUserTime;
	GetThreadTimes(hThreadParent, &ftCreationTime, &ftExitTime, &ftKernelTime, &ftUserTime);
	CloseHandle(hThreadParent);
	return 0;
}

DWORD WINAPI ParentThread(PVOID pvParam)
{
	HANDLE hThreadParent;
	DuplicateHandle(
		GetCurrentProcess(),
		GetCurrentThread(),
		GetCurrentProcess(),
		&hThreadParent,
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS
	);
	CreateThread(NULL, 0, ChildThread, (PVOID)hThreadParent, 0, NULL);
}

int main()
{
	HANDLE theProcess = GetCurrentProcess();
	HANDLE theThread = GetCurrentThread();

	DWORD theProcessID = GetCurrentProcessId();
	DWORD theThreadID = GetCurrentThreadId();
	return 0;
}