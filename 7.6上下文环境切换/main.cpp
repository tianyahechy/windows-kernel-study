#include <Windows.h>

int main()
{
	CONTEXT theContext;
	HANDLE hThread = GetCurrentThread();
	//�����߳�
	theContext.ContextFlags = CONTEXT_CONTROL;
	GetThreadContext(hThread, &theContext);

	theContext.R12 = 0x00010000;
	//����������
	SetThreadContext(hThread, &theContext);
	ResumeThread(hThread);
	return 0;
}