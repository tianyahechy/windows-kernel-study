
#include <Windows.h>
#include <TlHelp32.h>

#include "../CmmHdr.h"

void suspendProcess(DWORD dwProcessID, BOOL fSuspend)
{
	//�õ�ϵͳ�е��߳��б�
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);
	if (INVALID_HANDLE_VALUE == hSnapshot )
	{
		CloseHandle(hSnapshot);
		return;
	}

	//�����߳��б�
	THREADENTRY32 te = { sizeof(te) };
	BOOL fOK = Thread32First(hSnapshot, &te);
	for (; fOK; fOK = Thread32Next(hSnapshot, &te))
	{
		//��Ҫ��Ľ�����������߳�ô
		if (te.th32OwnerProcessID != dwProcessID)
		{
			continue;
		}

		HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
		if (NULL == hThread)
		{
			continue;
		}
		//�����ָ��߳�
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