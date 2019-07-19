#include "../cmmhdr.h"
#include <windowsx.h>
#include <Windows.h>
#include <sddl.h>
#include <tchar.h>
#include <strsafe.h>
#include <securitybaseapi.h>
#include "resource.h"

HWND g_hDlg;
HANDLE g_hSingleton = NULL;
HANDLE g_hBoundary = NULL;
HANDLE g_hNamespace = NULL;

BOOL g_bNamespaceOpened = FALSE;
PCTSTR g_szBoundary = TEXT("Boundary");
PCTSTR g_szNamespace = TEXT("Namespace");

#define DETAILS_CTRL GetDlgItem(g_hDlg,IDC_EDIT_DETAILS)
void AddText(PCTSTR pszFormat, ...)
{
	va_list argList;
	va_start(argList, pszFormat);
	TCHAR sz[20 * 1024];
	Edit_GetText(DETAILS_CTRL, sz, _countof(sz));
	_vstprintf_s(_tcschr(sz, TEXT('\0')), _countof(sz) - _tcslen(sz), pszFormat, argList);
	Edit_SetText(DETAILS_CTRL, sz);
	va_end(argList);

}

void Dlg_OnCommand(HWND hwnd, int id, HWND hWndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDOK:
	case IDCANCEL:
		EndDialog(hwnd, id);
	default:
		break;
	}
}

void CheckInstances()
{
	//创建边界标识符
	g_hBoundary = CreateBoundaryDescriptorW(g_szBoundary, 0);
	BYTE localAdminSID[SECURITY_MAX_SID_SIZE] = SECURITY_NT_AUTHORITY;
	PSID pLocalAdminSID;
	//if (!CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, pLocalAdminSID, &cbSID))
	if (!AllocateAndInitializeSid( 
		(PSID_IDENTIFIER_AUTHORITY)localAdminSID ,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&pLocalAdminSID))
	{
		AddText(TEXT("AddSIDtoBoundaryDescriptor failed!:%u\r\n"), GetLastError());
		return;
	}

	if (!AddSIDToBoundaryDescriptor(&g_hBoundary, pLocalAdminSID))
	{
		AddText(TEXT("AddSIDToBoundaryDescriptor failed: %u\r\n"), GetLastError());
		return;
	}
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	if (!ConvertStringSecurityDescriptorToSecurityDescriptor(TEXT("D:(A;;GA;;;BA)"),
		SDDL_REVISION_1,
		&sa.lpSecurityDescriptor,
		NULL))
	{
		AddText(TEXT("Security Descriptor creation failed!:%u\r\n"), GetLastError());
		return;
	}

	//创建专有命名空间
	g_hNamespace = CreatePrivateNamespaceW(&sa, g_hBoundary, g_szBoundary);
	LocalFree(sa.lpSecurityDescriptor);

	DWORD dwLastError = GetLastError();
	if (g_hNamespace == NULL)
	{
		if (dwLastError == ERROR_ACCESS_DENIED)
		{
			AddText(TEXT("Access denied when creating the namespace.\r\n*"));
			AddText(TEXT("YOU must be adnimistrator\r\n\r\n"));
			return;
		}
		else
		{
			if (dwLastError == ERROR_ALREADY_EXISTS)
			{
				AddText(TEXT("CreatePrivateSpace failed:%u\r\n"), dwLastError);
				g_hNamespace = OpenPrivateNamespaceW(g_hBoundary, g_szNamespace);
				if (NULL == g_hNamespace)
				{
					AddText(TEXT("and openPrivateNamespace failed!%u\r\n"), dwLastError);
					return;
				}
				else
				{
					g_bNamespaceOpened = TRUE;
					AddText(TEXT(" but OpenPrivateNamespace success!\r\n\r\n"));
				}
			}
			else
			{
				AddText(TEXT("Unexpected error occured %u\r\n"), dwLastError);
				return;
			}

		}
	}

	TCHAR szMutexName[64];
	StringCchPrintf(szMutexName, _countof(szMutexName), TEXT("%s\\%s"), g_szNamespace, TEXT("Singleton"));
	g_hSingleton = CreateMutex(NULL, FALSE, szMutexName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AddText(TEXT("Unexpected error occured:%u\r\n\r\n"), dwLastError);
		AddText(TEXT("-->Impossible to access application features.\r\n"));
	}
	else
	{
		AddText(TEXT("First instance of singleton:\r\n"));
		AddText(TEXT("-->Access application features now.\r\n"));
	}
}

BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//chSETDLGICONS(hwnd, IDI_SINGLETON);
	g_hDlg = hwnd;
	CheckInstances();
	return TRUE;
}

INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		chHANDLE_DLGMSG(hwnd, WM_COMMAND, Dlg_OnCommand);
		chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);

	}
	return FALSE;
}

int APIENTRY _tWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow
	)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_SINGLETON), NULL, Dlg_Proc);
	if (g_hSingleton != NULL)
	{
		CloseHandle(g_hSingleton);
	}
	if (g_hNamespace != NULL)
	{
		if (g_bNamespaceOpened)
		{
			ClosePrivateNamespace(g_hNamespace, 0);
		}
		else
		{
			ClosePrivateNamespace(g_hNamespace, PRIVATE_NAMESPACE_FLAG_DESTROY);
		}
	}

	if (NULL != g_hBoundary)
	{
		DeleteBoundaryDescriptor(g_hBoundary);
	}
	return 0;
	DWORD dwProcessIdT;
	HANDLE hObjInProcessS = CreateMutex(NULL, FALSE, NULL);
	HANDLE hProcessT = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessIdT);
	HANDLE hObjInProcessT;
	DuplicateHandle(GetCurrentProcess(), hObjInProcessS, hProcessT, &hObjInProcessT, 0, FALSE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hProcessT);
	CloseHandle(hObjInProcessT);

	HANDLE hFileMapRW = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 10240, NULL);
	HANDLE hFileMapRO;
	DuplicateHandle(GetCurrentProcess(), hFileMapRW, GetCurrentProcess(), &hFileMapRO, FILE_MAP_READ, FALSE, 0);
	CloseHandle(hFileMapRO);
	CloseHandle(hFileMapRW);
}