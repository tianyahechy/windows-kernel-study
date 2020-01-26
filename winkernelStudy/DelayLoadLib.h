#ifndef DELAYLOADLIBAPI
#define DELAYLOADLIBAPI	extern "C" __declspec(dllimport)
#endif

DELAYLOADLIBAPI int fnLib();
DELAYLOADLIBAPI int fnLib2();