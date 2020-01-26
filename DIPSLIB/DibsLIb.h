# if !defined(DIPSLIBAPI)
#define DIPSLIBAPI	__declspec(dllimport)
#endif

DIPSLIBAPI BOOL WINAPI SetDIPSHook(DWORD dwThreadId);