#include <Windows.h>

#define CACHE_ALIGN	32
#ifdef _X86_
#define CACHE_ALIGN	32
#endif // _X86_

#ifdef _ALPHA_
#define CACHE_ALIGN	64
#endif // _ALPHA_

#define CACHE_PAD(Name,BytesSoFar) BYTE Name[CACHE_ALIGN - ( ( BytesSoFar) % CACHE_ALIGN)]

struct CUSTINFO
{
	DWORD dwCustomerID;
	char szName[100];

	CACHE_PAD(bPad1, sizeof(DWORD) + 100);
	int nBalanceDue;
	FILETIME ftLastOrderDate;
	CACHE_PAD(bPad2, sizeof(int) + sizeof(FILETIME));
};


