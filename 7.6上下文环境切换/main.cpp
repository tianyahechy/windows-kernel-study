#include <Windows.h>

int main()
{
	CONTEXT theContext;
	HANDLE hThread = GetCurrentThread();
	//挂起线程
	theContext.ContextFlags = CONTEXT_CONTROL;
	GetThreadContext(hThread, &theContext);

	theContext.R12 = 0x00010000;
	//设置上下文
	SetThreadContext(hThread, &theContext);
	ResumeThread(hThread);
	return 0;
}