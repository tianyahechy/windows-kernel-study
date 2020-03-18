
#include <Windows.h>

int main()
{
	PLOADED_IMAGE
	HANDLE theHandle = GetCurrentProcess();
	PDWORD_PTR pMask = 0;
	PDWORD_PTR sMask = 0;
	GetProcessAffinityMask(theHandle, pMask, sMask);
	return 0;
}