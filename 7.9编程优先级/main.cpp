#include <Windows.h>
#include <stdio.h>

DWORD WINAPI ThreadFunc(LPVOID p)
{
	printf("�������̣߳� pid = %d\n", GetCurrentThreadId());   //������߳�pid
	return 0;
}
int main()
{
	 HANDLE theProcess = GetCurrentProcess();
	 DWORD processPri =  GetPriorityClass(theProcess);
	 PBOOL theBool = NULL;;
	 GetProcessPriorityBoost(theProcess, theBool);
	 SetProcessPriorityBoost(theProcess, true);
	 SetPriorityClass(theProcess, IDLE_PRIORITY_CLASS);
	 processPri = GetPriorityClass(theProcess);

	 DWORD dwThreadID = 0;
	 HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, NULL, CREATE_SUSPENDED, &dwThreadID);
	 int prid1 = GetThreadPriority(hThread);
	 SetThreadPriority(hThread, THREAD_PRIORITY_IDLE);
	 int prid2 = GetThreadPriority(hThread);
	 ResumeThread(hThread);
	 CloseHandle(hThread);
	return 0;
}