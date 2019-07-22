
#include <Windows.h>
#include <tchar.h>

extern "C" const IMAGE_DOS_HEADER __ImageBase;
void DumpModule()
{
	HMODULE hModule = GetModuleHandle(NULL);
	_tprintf(TEXT("with GetModuleHandle(NULL) = 0x%x\r\n"), hModule);
	_tprintf(TEXT("with __ImageBase = 0x%x\r\n"), (HINSTANCE)& __ImageBase);
	hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (PCTSTR) D)
}

int _tmain(int argc, TCHAR* argv[])
{
	DumpModule();
	return 0;
}