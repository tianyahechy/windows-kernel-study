#include "CmmHdr.h"
#include <Windows.h>
#include <tchar.h>

#define DELAYLOADLIBAPI extern "C" __declspec(dllexport)
#include "DelayLoadLib.h"

int fnLib()
{
	return 321;
}

int fnLib2()
{
	return 123;
}