#include "ToolHelp.h"

CToolhelp::CToolhelp(DWORD dwFlags, DWORD dwProcessID)
{
	m_hSnapShot = INVALID_HANDLE_VALUE;
	CreateSnapShot(dwFlags, dwProcessID);
}

CToolhelp::~CToolhelp()
{
	if (m_hSnapShot != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSnapShot);
	}
}

BOOL CToolhelp::CreateSnapShot(DWORD dwFlags, DWORD dwProcessID)
{
	if (m_hSnapShot != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSnapShot);
	}
	if ( 0 == dwFlags )
	{
		m_hSnapShot = INVALID_HANDLE_VALUE;
	}
	else
	{
		m_hSnapShot = CreateToolhelp32Snapshot(dwFlags, dwProcessID);
	}
	return (m_hSnapShot != INVALID_HANDLE_VALUE);
}

BOOL CToolhelp::ProcessFirst(PPROCESSENTRY32 ppe) const
{
	BOOL fOk = Process32First(m_hSnapShot, ppe);
	if (fOk && (ppe->th32ProcessID == 0))
	{
		fOk = ProcessNext(ppe);
	}
	return fOk;
}

BOOL CToolhelp::ProcessNext(PPROCESSENTRY32 ppe) const
{
	BOOL fOk = Process32Next(m_hSnapShot, ppe);
	if (fOk && (ppe->th32ProcessID == 0))
	{
		fOk = ProcessNext(ppe);
	}
	return fOk;
}

BOOL CToolhelp::ProcessFind(DWORD dwProcessId, PPROCESSENTRY32 ppe) const
{
	BOOL fFound = FALSE;
	for (BOOL fOk = ProcessFirst(ppe); fOk ; fOk = ProcessNext(ppe))
	{
		fFound = (ppe->th32ProcessID == dwProcessId);
		if (fFound )
		{
			break;
		}
	}
	return fFound;
}

BOOL CToolhelp::ModuleFirst(PMODULEENTRY32 pme) const
{
	return (Module32First(m_hSnapShot, pme));
}

BOOL CToolhelp::ModuleNext(PMODULEENTRY32 pme) const
{
	return (Module32Next(m_hSnapShot, pme));
}

BOOL CToolhelp::ModuleFind(PVOID pvBaseAddr, PMODULEENTRY32 pme) const
{
	BOOL fFound = FALSE;
	for (BOOL fOk = ModuleFirst(pme); fOk; fOk = ModuleNext(pme))
	{
		fFound = (pme->modBaseAddr == pvBaseAddr);
		if (fFound)
		{
			break;
		}
	}
	return fFound;
}

BOOL CToolhelp::ModuleFind(PTSTR pszModeName, PMODULEENTRY32 pme) const
{
	BOOL fFound = FALSE;
	for (BOOL fOK = ModuleFirst(pme); fOK ; fOK = ModuleNext(pme))
	{
		fFound = (lstrcmpi(pme->szModule, pszModeName) == 0) ||
			(lstrcmpi(pme->szExePath, pszModeName) == 0);
		if (fFound)
		{
			break;
		}
	}
	return fFound;
}

BOOL CToolhelp::ThreadFirst(PTHREADENTRY32 pte) const
{
	return Thread32First(m_hSnapShot, pte );
}

BOOL CToolhelp::ThreadNext(PTHREADENTRY32 pte) const
{
	return Thread32Next(m_hSnapShot, pte);
}

BOOL CToolhelp::HeapListFirst(PHEAPLIST32 phl) const
{
	return Heap32ListFirst(m_hSnapShot, phl);
}

BOOL CToolhelp::HeapListNext(PHEAPLIST32 phl) const
{
	return Heap32ListNext(m_hSnapShot, phl);
}

int CToolhelp::HowManyHeaps() const
{
	int nHowManyHeaps = 0;
	HEAPLIST32 hl = { sizeof(hl) };
	for (BOOL fOK = HeapListFirst(&hl); fOK; fOK = HeapListNext(&hl))
	{
		nHowManyHeaps++;
	}
	return nHowManyHeaps;
}

BOOL CToolhelp::HeapFirst(PHEAPENTRY32 phe, DWORD dwProcessID, UINT_PTR dwHeapID) const
{
	return Heap32First(phe, dwProcessID, dwHeapID);
}

BOOL CToolhelp::HeapNext(PHEAPENTRY32 phe) const
{
	return Heap32Next(phe);
}

int CToolhelp::HowManyBlocksInHeap(DWORD dwProcessID, DWORD dwHeapId) const
{
	int nHowManyBlocksInHeap = 0;
	HEAPENTRY32 he = { sizeof(he) };
	BOOL fOk = HeapFirst(&he, dwProcessID, dwHeapId);
	for (; fOk; fOk = HeapNext(&he))
	{
		nHowManyBlocksInHeap++;
	}
	return nHowManyBlocksInHeap;
}

BOOL CToolhelp::IsAHeap(HANDLE hProcess, PVOID pvBlock, PDWORD pdwFlags) const
{
	HEAPLIST32 hl = { sizeof(hl) };
	for (BOOL fOKHL = HeapListFirst(&hl); fOKHL; fOKHL = HeapListNext(&hl))
	{
		HEAPENTRY32 he = { sizeof(he) };
		BOOL fOKHE = HeapFirst(&he, hl.th32ProcessID, hl.th32HeapID);
		for (; fOKHE; fOKHE = HeapNext(&he))
		{
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQueryEx(hProcess, (PVOID)he.dwAddress, &mbi, sizeof(mbi));
			if (chINRANGE(mbi.AllocationBase, pvBlock, (PBYTE) mbi.AllocationBase + mbi.RegionSize )))
			{
				*pdwFlags = hl.dwFlags;
				return TRUE;
			}
		}
	}
	return FALSE;
}
