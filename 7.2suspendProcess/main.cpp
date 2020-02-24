
#include <Windows.h>
#include <TlHelp32.h>

#include "../CmmHdr.h"

void suspendProcess(DWORD dwProcessID, BOOL fSuspend)
{
	//得到系统中的线程列表
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);
	if (INVALID_HANDLE_VALUE == hSnapshot )
	{
		CloseHandle(hSnapshot);
		return;
	}

	//遍历线程列表
	THREADENTRY32 te = { sizeof(te) };
	BOOL fOK = Thread32First(hSnapshot, &te);
	for (; fOK; fOK = Thread32Next(hSnapshot, &te))
	{
		//在要求的进程中是这个线程么
		if (te.th32OwnerProcessID != dwProcessID)
		{
			continue;
		}

		HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
		if (NULL == hThread)
		{
			continue;
		}
		//挂起或恢复线程
		if (fSuspend)
		{
			SuspendThread(hThread);
		}
		else
		{
			ResumeThread(hThread);
		}
		CloseHandle(hThread);
	}
	CloseHandle(hSnapshot);
}