#include <Windows.h>

//��ý�������������
int main()
{
	int nNumArg = -1;
	PWSTR * ppArgV = CommandLineToArgvW(GetCommandLineW(), &nNumArg);
	HeapFree(GetProcessHeap(), 0, ppArgV);
	return 0;
}