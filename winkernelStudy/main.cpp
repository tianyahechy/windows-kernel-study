#include "CmmHdr.h"
#include <windowsx.h>
#include <tchar.h>
#include "resource.h"

#define ESM_POKECODEANDLOOKUP	(WM_USER + 100)
const TCHAR g_szAppName[] = TEXT("Error Show");

BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	Edit_LimitText(GetDlgItem(hwnd, IDC_ERRORCODE), 5);
	SendMessage(hwnd, ESM_POKECODEANDLOOKUP, lParam, 0);
	return TRUE;
}

void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDCANCEL:
		EndDialog(hwnd, id);

	case IDC_ALWAYSONTOP:
		SetWindowPos(hwnd, IsDlgButtonChecked(hwnd, IDC_ALWAYSONTOP) ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		break;

	case IDC_ERRORCODE:
		EnableWindow(GetDlgItem(hwnd, IDOK), Edit_GetTextLength(hwndCtl) > 0);
		break;

	case IDOK:
	{
				 DWORD dwError = GetDlgItemInt(hwnd, IDC_ERRORCODE, NULL, FALSE);
				 HLOCAL hLocal = NULL;
				 BOOL fOk = FormatMessage(
					 FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
					 NULL, dwError, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
					 (PTSTR)& hLocal, 0, NULL);
				 if (!fOk)
				 {
					 HMODULE hDLL = LoadLibraryEx(TEXT("netmsg.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES);
					 if (hDLL != NULL)
					 {
						 FormatMessage(
							 FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM,
							 hDLL, dwError, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
							 (PTSTR)& hLocal, 0, NULL);
						 FreeLibrary(hDLL);
					 }

				 }
				 if (hLocal != NULL)
				 {
					 SetDlgItemText(hwnd, IDC_ERRORTEXT, (PCTSTR)LocalLock(hLocal));
					 LocalFree(hLocal);
				 }
				 else
				 {
					 SetDlgItemText(hwnd, IDC_ERRORTEXT, TEXT("ERROR NUMBER NOT FOUND!"));
				 }
	}
		break;
	default:
		break;
	}
}

INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
		chHANDLE_DLGMSG(hwnd, WM_COMMAND, Dlg_OnCommand);

	case ESM_POKECODEANDLOOKUP:
		SetDlgItemInt(hwnd, IDC_ERRORCODE, (UINT)wParam, FALSE);
		FORWARD_WM_COMMAND(hwnd, IDOK, GetDlgItem(hwnd, IDOK), BN_CLICKED, PostMessage);
		SetForegroundWindow(hwnd);
		break;
	
	}
	return FALSE;
}

int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR pszCmdLine, int)
{
	HWND hwnd = FindWindow(TEXT("#32770"), TEXT("Error Show"));
	if (IsWindow(hwnd) )
	{
		SendMessage(hwnd, ESM_POKECODEANDLOOKUP, _ttoi(pszCmdLine), 0);
	}
	else
	{
		DialogBoxParam(hInstExe, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Dlg_Proc, _ttoi(pszCmdLine));
	}
	return 0;
}