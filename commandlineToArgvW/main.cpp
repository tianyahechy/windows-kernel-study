#include <Windows.h>

//获得进程完整命令行
int main()
{
	int nNumArg = -1;
	PWSTR * ppArgV = CommandLineToArgvW(GetCommandLineW(), &nNumArg);
	HeapFree(GetProcessHeap(), 0, ppArgV);
	return 0;
}