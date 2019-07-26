#include <Windows.h>
#include <tchar.h>
#include <string>
#include <windowsx.h>
#include <strsafe.h>

void PrintEnvironmentVariable(PCTSTR pszVariableName)
{
	PTSTR pszValue = NULL;
	DWORD dwResult = GetEnvironmentVariable(pszVariableName, pszValue, 0);
	if (0 != dwResult)
	{
		//�����ڴ����������
		DWORD size = dwResult * sizeof(TCHAR);
		pszValue = (PTSTR)malloc(size);
		GetEnvironmentVariable(pszVariableName, pszValue, size);
		_tprintf(TEXT("%s = %s\n"), pszVariableName, pszValue);
		free(pszValue);
	}
	else
	{
		_tprintf(TEXT("'%s' = <unknown value>\n"), pszVariableName);
	}
}
void DumpEnvVariables(PTSTR pEnvBlock[])
{
	int current = 0;
	PTSTR* pElement = (PTSTR*)pEnvBlock;
	PTSTR pCurrent = NULL;
	while (pElement != NULL)
	{
		pCurrent = (PTSTR)(*pElement);
		//û�и���Ļ���������
		if (NULL == pCurrent)
		{
			pElement = NULL;
		}
		else
		{
			_tprintf(TEXT("[%u] %s\r\n"), current, pCurrent);
			current++;
			pElement++;
		}
	}
}
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
//��ý�������������
int main()
{
	DumpEnvStrings();
	return 0;
}