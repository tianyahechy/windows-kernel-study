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
	ua_tcscpy_s(szPath, _countof(szPath), TEXT("ProcessB"));
	CreateProcess(NULL, szPath, &saProcess, &saThread, FALSE, 0, NULL, NULL, &si, &piProcessB);

	ua_tcscpy_s(szPath, _countof(szPath), TEXT("ProcessC"));
	CreateProcess(NULL, szPath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &piProcessC);

	return 0;
}