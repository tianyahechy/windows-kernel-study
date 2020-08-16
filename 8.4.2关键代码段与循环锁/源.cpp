
#include <Windows.h>

int main()
{
	CRITICAL_SECTION gs;
	int dwSpinCount = 4000;
	InitializeCriticalSectionAndSpinCount(&gs, dwSpinCount);
	SetCriticalSectionSpinCount(&gs, dwSpinCount);
	return 0;
}