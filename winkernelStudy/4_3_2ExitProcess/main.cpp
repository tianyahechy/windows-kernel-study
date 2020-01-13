#include <Windows.h>
#include <stdio.h>

class CSomeObj
{
public:
	CSomeObj()
	{
		printf("constructor\r\n");
	}
	~CSomeObj()
	{
		printf("Destructor\r\n");
	}
};

CSomeObj g_GlobalObj;
void main()
{
	CSomeObj LocalObj;
	
}