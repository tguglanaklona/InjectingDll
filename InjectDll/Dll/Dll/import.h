#include <windows.h>
#include <tchar.h>

#define MakePtr(cast, base, offset) (cast)((DWORD_PTR)(base) + (DWORD_PTR)(offset))

int
WINAPI
MyMessageBoxA(
    __in_opt HWND hWnd,
    __in_opt LPCSTR lpText,
    __in_opt LPCSTR lpCaption,
    __in UINT uType);

struct mystruct
{
	int in1;
	unsigned char* ch2;
	//char *a; char b; int c; double r;
};

//int MySymbolCounter(const char *a, char b, int c, double r=0);
int MySymbolCounter(mystruct* str);

HRESULT ApiHijackImports(
				HMODULE hModule,	
				LPSTR szVictim,
				LPSTR szEntry,
				LPVOID pHijacker,	
				LPVOID *ppOrig
				);

HRESULT WriteProtectedMemory(LPVOID pDest, LPCVOID pSrc, DWORD dwSize);