
#include <Windows.h>
#
void ReplaceIAEntryInOnMod(
	PCSTR pszCalleeModName, 
	PROC pfnCurrent,
	PROC pfnNew,
	HMODULE hmodCaller)
{
	ULONG ulSize;
	PIMAGE_IMPORT_DESCRIPTOR pImPortDesc = (PIMAGE_IMPORT_DESCRIPTOR)
		ImageDirectoryEntryToData(hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

	if (NULL == pImPortDesc)
	{
		return;
	}

	for (; pImPortDesc->Name; pImPortDesc++)
	{
		PSTR pszModName = (PSTR)((PBYTE)hmodCaller + pImPortDesc->Name);
		if (lstrcmpiA(pszModName, pszCalleeModName) == 0 )
		{
			break;
		}
	}

	if (pImPortDesc->Name == 0 )
	{
		return;
	}

	PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((PBYTE)hmodCaller + pImPortDesc->FirstThunk);

	for (; pThunk->u1.Function; pThunk++)
	{
		PROC * ppfn = (PROC*)&pThunk->u1.Function;
		BOOL fFound = (*ppfn == pfnCurrent);

		if (fFound)
		{
			WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew, sizeof(pfnNew), NULL);
			return;
		}
	}

}