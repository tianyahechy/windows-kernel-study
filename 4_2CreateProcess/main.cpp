
#include <Windows.h>
int main()
{
	//½á¹¹Ìå
	STARTUPINFO si = { sizeof(si) };
	SECURITY_ATTRIBUTES saProcess, saThread;
	PROCESS_INFORMATION piProcessB, piProcessC;
	TCHAR szPath[MAX_PATH];
	saProcess.nLength = sizeof(saProcess);
	saProcess.lpSecurityDescriptor = NULL;
	saProcess.bInheritHandle = TRUE;
	saThread.nLength = sizeof(saThread);
	saThread.lpSecurityDescriptor = NULL;
	saThread.bInheritHandle = FALSE;
	wcscpy_s(szPath, _countof(szPath), TEXT("ProcessB"));
	DWORD dwExitCode;
	BOOL fSuccess = CreateProcess(NULL, szPath, &saProcess, &saThread, FALSE, 0, NULL, NULL, &si, &piProcessB);
	if (fSuccess)
	{
		CloseHandle(piProcessB.hThread);
		WaitForSingleObject(piProcessB.hProcess, INFINITE);
		GetExitCodeProcess(piProcessB.hProcess, &dwExitCode);
		CloseHandle(piProcessB.hProcess);
	}


	wcscpy_s(szPath, _countof(szPath), TEXT("ProcessC"));
	fSuccess = CreateProcess(NULL, szPath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &piProcessC);
	if (fSuccess)
	{
		CloseHandle(piProcessC.hThread);
		WaitForSingleObject(piProcessC.hProcess, INFINITE);
		GetExitCodeProcess(piProcessC.hProcess, &dwExitCode);
		CloseHandle(piProcessC.hProcess);
	}
	return 0;
}