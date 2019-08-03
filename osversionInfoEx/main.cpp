#include <Windows.h>

int main()
{
	OSVERSIONINFOEX osVer = { 0 };
	osVer.dwOSVersionInfoSize = sizeof(osVer);
	osVer.dwMajorVersion = 5;
	osVer.dwMinorVersion = 0;
	osVer.dwPlatformId = VER_PLATFORM_WIN32_NT;

	DWORDLONG dwlConditionMask = 0;
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_PLATFORMID, VER_EQUAL);

	bool bWin2000 = VerifyVersionInfo(&osVer, VER_MAJORVERSION | VER_MINORVERSION | VER_PLATFORMID, dwlConditionMask);

	return 0;
}