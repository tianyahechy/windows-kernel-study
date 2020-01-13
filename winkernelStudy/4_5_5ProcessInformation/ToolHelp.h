#pragma once
#include "../CmmHdr.h"
#include <TlHelp32.h>
#include <tchar.h>

class CToolhelp
{
private:
	HANDLE m_hSnapShot;

public:
	CToolhelp(DWORD dwFlags = 0, DWORD dwProcessID = 0);
	~CToolhelp();
	BOOL CreateSnapShot(DWORD dwFlags, DWORD dwProcessID = 0);
	BOOL ProcessFirst(PPROCESSENTRY32 ppe) const;
	BOOL ProcessNext(PPROCESSENTRY32 ppe) const;
	BOOL ProcessFind(DWORD dwProcessId, PPROCESSENTRY32 ppe) const;

	BOOL ModuleFirst(PMODULEENTRY32 pme) const;
	BOOL ModuleNext(PMODULEENTRY32 pme) const;
	BOOL ModuleFind(PVOID pvBaseAddr, PMODULEENTRY32 pme) const;
	BOOL ModuleFind(PTSTR pszModeName, PMODULEENTRY32 pme) const;

	BOOL ThreadFirst(PTHREADENTRY32 pte) const;
	BOOL ThreadNext(PTHREADENTRY32 pte) const;

	BOOL HeapListFirst(PHEAPLIST32 phl) const;
	BOOL HeapListNext(PHEAPLIST32 phl) const;
	int HowManyHeaps() const;

	BOOL HeapFirst(PHEAPENTRY32 phe, DWORD dwProcessID, UINT_PTR dwHeapID) const;
	BOOL HeapNext(PHEAPENTRY32 phe) const;
	int HowManyBlocksInHeap(DWORD dwProcessID, DWORD dwHeapId) const;
	BOOL IsAHeap(HANDLE hProcess, PVOID pvBlock, PDWORD pdwFlags) const;

public:
	static BOOL EnableDebugPrivilege(BOOL fEnable = TRUE);
	static BOOL ReadProcessMemory(DWORD dwProcessID, LPCVOID pvBaseAddress,
		PVOID pvBuffer, DWORD cbRead, PDWORD pdwNumberOfBytesRead = NULL);
};
