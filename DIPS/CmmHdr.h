#pragma once
#define _WIN32_WINNT	0x0500

#ifndef _M_IX86
#define UNICODE
#endif

#ifdef UNICODE
#define _UNICODE
#endif

#include <Windows.h>
#pragma warning(pop)
#pragma warning(push, 4)

#ifndef WT_EXECUTEINPERSISTENTTHREAD
#pragma message("Yor are not using the latest platform sdk header/library")
#pragma message("files, this may prevent the project from building correctly.")
#endif

#pragma warning(disable:4001)
#pragma warning(disable:4100)
#pragma warning(disable:4699)
#pragma warning(disable:4710)
#pragma warning(disable:4514)
#pragma warning(disable:4512)
#pragma warning(disable:4996)
#define chSTR2(x)	#x
#define chSTR(x)	chSTR2(x)
#define chMSG(desc)	message(__FILE__ "(" chSTR(__LINE__) "):" #desc)

#define chINRANGE(low, Num, High ) (((low)<= (Num)) && (( Num) <= (High))
#define chDIMOF(Array) (sizeof(Array) / sizeof(Array[0]))

typedef unsigned(__stdcall * PTHREAD_START) (void*);
#define chBEGINTHREADEX(psa, cbStack, pfnStartAddr, \
	pvParam, fdwCreate, pdwThreadId)				\
	((HANDLE)_beginthreadex(\
	(void*)(psa), \
	(unsigned)(cbStack), \
	(PTHREAD_START)(pfnStartAddr), \
	(void*)(pvParam), \
	(unsigned)(fdwCreate), \
	(unsigned*)(pdwThreadId)))

#ifdef _X86_
#define DebugBreak()		_asm{int 3}
#endif

#define MAKESOFTWAREEXCEPTION(Severity, Facility, Exception )	\
	((DWORD) (													\
				(Severity )				|						\
				(1 << 29)				|						\
				(0 << 28)				|						\
				(Facility << 16)		|						\
				(Exception << 0) ))

inline void chMB(PCSTR s)
{
	char szTMP[128];
	GetModuleFileNameA(NULL, szTMP, chDIMOF(szTMP));
	MessageBoxA(GetActiveWindow(), s, szTMP, MB_OK);
}

inline void chFAIL(PSTR szMsg)
{
	chMB(szMsg);
	DebugBreak();
}

inline void chASSERTFAIL(LPCSTR file, int line, PCSTR expr)
{
	char sz[128];
	wsprintfA(sz, "File %s, line %d: %s", file, line, expr);
	chFAIL(sz);
}

#ifdef _DEBUG
#define chASSERT(x)	if( !(x)) chASSERTFAIL(__FILE__, __LINE__, #x)
#else
#define chASSERT(x)
#endif

#ifdef _DEBUG
#define chVERIFY(x) chASSERT(x)
#else
#define chVERIFY(x)	(x)
#endif

#define chHANDLE_DLGMSG(hwnd, message, fn)					\
	case (message) : return (SetDlgMsgResult(hwnd, uMsg,	\
		HANDLE_##message((hwnd), (wParam), (lParam), (fn) )))

inline void chSETDLGICONS(HWND hwnd, int idi)
{
	SendMessage(hwnd, WM_SETICON, TRUE, (LPARAM)LoadIcon((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		MAKEINTRESOURCE(idi)));
}

inline void chWindows9xNotAllowed()
{
	OSVERSIONINFO vi = { sizeof(vi) };
	GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		chMB("this application requires features not present in windows9x");
		ExitProcess(0);
	}
}

inline void chWindows2000Required()
{
	OSVERSIONINFO vi = { sizeof(vi) };
	GetVersionEx(&vi);
	if ((vi.dwPlatformId != VER_PLATFORM_WIN32_NT) && ( vi.dwMajorVersion < 5))
	{
		chMB("this application requires features present in windows2000");
		ExitProcess(0);
	}
}

#ifdef UNICODE
class CUnicodeSupported
{
public:
	CUnicodeSupported()
	{
		if (GetWindowsDirectoryW(NULL, 0) <= 0 )
		{
			chMB("This application requires an os that supports unicode.");
			ExitProcess(0);
		}
	}
};
static CUnicodeSupported g_UnicodeSupported;
#endif

#pragma comment(linker, "/subsystem:Windows")