#include "../CmmHdr.h"
#include <windowsx.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <stdarg.h>
#include <stdio.h>
#include "ToolHelp.h"

void AddText(HWND hwnd, PCTSTR pszFormat, ...)
{
	va_list argList;
	va_start(argList, pszFormat);

	TCHAR sz[20 * 1024];
	Edit_GetText(hwnd, sz, chDIMOF(sz));
	_vstprintf(_tcschr(sz, 0), pszFormat, argList);
	Edit_SetText(hwnd, sz);
	va_end(argList);
}

void Dlg_PopulateProcessList(HWND hwnd)
{
	HWND hwndList = GetDlgItem(hwnd, IDC_PROCESSMODULELIST);
	SetWindowRedraw(hwndList, FALSE);
	ComboBox_ResetContent(hwndList);

	CToolhelp theProcesses(TH32CS_SNAPPROCESS);
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk = theProcesses.ProcessFirst(&pe);
	for (; fOk; fOk = theProcesses.ProcessNext(&pe))
	{
		TCHAR sz[1024];
		PCTSTR pszExeFile = _tcsrchr(pe.szExeFile, TEXT('\\'));
		if (NULL == pszExeFile)
		{
			pszExeFile = pe.szExeFile;
		}
		else
		{
			pszExeFile++;
		}
		wsprintf(sz, TEXT("%s (0x%08X)"), pszExeFile, pe.th32ProcessID);
		int n = ComboBox_AddString(hwndList, sz);
		ComboBox_SetItemData(hwndList, n, pe.th32ProcessID);
	}
	ComboBox_SetCurSel(hwndList, 0);
	FORWARD_WM_COMMAND(hwnd, IDC_PROCESSMODULELIST, hwndList, CBN_SELCHANGE, SendMessage);
	SetWindowRedraw(hwndList, TRUE);
	InvalidateRect(hwndList, NULL, FALSE);
}

PVOID GetModulePreferredBaseAddr(DWORD dwProcessId, PVOID pvModuleRemote)
{
	PVOID pvModulePreferedBaseAddr = NULL;
	IMAGE_DOS_HEADER idh;
	IMAGE_NT_HEADERS inth;

	Toolhelp32ReadProcessMemory(dwProcessId, pvModuleRemote, &idh, sizeof(idh), NULL);
	if (idh.e_magic == IMAGE_DOS_SIGNATURE)
	{
		Toolhelp32ReadProcessMemory(dwProcessId, (PBYTE)pvModuleRemote + idh.e_lfanew, &inth, sizeof(inth), NULL);
		if (IMAGE_NT_SIGNATURE == inth.Signature )
		{
			pvModulePreferedBaseAddr = (PVOID)inth.OptionalHeader.ImageBase;
		}
	}
	return pvModulePreferedBaseAddr;
}

void ShowProcessInfo(HWND hWnd, DWORD dwProcessID)
{
	SetWindowText(hWnd, TEXT(""));
	CToolhelp th(TH32CS_SNAPALL, dwProcessID);
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk = th.ProcessFirst(&pe);
	for (; fOk; fOk = th.ProcessNext(&pe))
	{
		if (pe.th32ProcessID == dwProcessID)
		{
			AddText(hWnd, TEXT("FileName: %s\r\n"), pe.szExeFile);
			AddText(hWnd, TEXT(" PID = 0x%08X, ParentPID = 0x%08X, ") TEXT("ProrityClass = %d, Threads = %d, Heaps = %d\r\n"),
				pe.th32ProcessID, pe.th32ParentProcessID, pe.pcPriClassBase, pe.cntThreads, th.HowManyHeaps());
			break;
		}
	}
	const int cchAddress = sizeof(PVOID) * 2;
	AddText(hWnd, TEXT("\r\nModules Information : \r\n") 
		TEXT(" Usage %-*s (%-*s) %8s Module \r\n"), 
		cchAddress, TEXT("BaseAddr"),
		cchAddress, TEXT("ImagAddr"), TEXT("Size"));
}