#include <Windows.h>
#include <tchar.h>
#include <string>
#include <windowsx.h>
#include <strsafe.h>
void DumpEnvStrings()
{
	PTSTR pEnvBlock = GetEnvironmentStrings();
	TCHAR szName[MAX_PATH];
	TCHAR szValue[MAX_PATH];
	PTSTR pszCurrent = pEnvBlock;
	HRESULT hr = S_OK;
	PCTSTR pszPos = NULL;
	int current = 0;
	while (pszCurrent != NULL)
	{
		if (*pszCurrent != TEXT('='))
		{
			pszPos = _tcschr(pszCurrent, TEXT('='));
			pszPos++;
			size_t cbNameLength = (size_t)pszPos - (size_t)pszCurrent - sizeof(TCHAR);
			hr = StringCbCopyN(szName, MAX_PATH, pszCurrent, cbNameLength);
			if (FAILED(hr))
			{
				break;
			}
			hr = StringCchCopyN(szValue, MAX_PATH, pszPos, _tcslen(pszPos) + 1);
			if (SUCCEEDED(hr))
			{
				_tprintf(TEXT("[%u] %s=%s\r\n"), current, szName, szValue);
			}
			else if (hr == STRSAFE_E_INSUFFICIENT_BUFFER)
			{
				_tprintf(TEXT("[%u] %s=%s..\r\n"), current, szName, szValue);
			}
			else
			{
				_tprintf(TEXT("[%u] %s = ???\r\n"), current, szName, szValue);
			}
			break;
		}
		else
		{
			_tprintf(TEXT("[%u] %s\r\n"), current, pszCurrent);
		}
		current++;
		while (*pszCurrent != TEXT('\0'))
		{
			pszCurrent++;
		}
		pszCurrent++;
		if (*pszCurrent == TEXT('\0'))
		{
			break;
		}
	}

	FreeEnvironmentStrings(pEnvBlock);
}
//获得进程完整命令行
int main()
{
	DumpEnvStrings();
	return 0;
}