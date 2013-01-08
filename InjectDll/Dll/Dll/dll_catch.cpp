#include <windows.h>
#include <tchar.h>
#include "import.h"
#define addr_ 0xEE6674

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, PVOID fImpLoad) 
{
	LPSTR moduleToHijack = "SampleDll.dll";//abstractgametablescreen.dll";	  //модуль процесса в котором определена функция
	LPSTR functionToHijack = "SymbolCounter";//OnReceiveGameMove"; //функция
	LPVOID pXFunction = MySymbolCounter;	  //функция для замены

	static LPVOID pOldHandler = NULL;
	
	if (fdwReason == DLL_PROCESS_ATTACH) 
	{
		HRESULT hr = ApiHijackImports(::GetModuleHandle(NULL), moduleToHijack, functionToHijack, pXFunction, &pOldHandler);
		//HRESULT hr = WriteProtectedMemory((LPVOID)addr_, &pXFunction, sizeof(LPVOID));
		if (!SUCCEEDED(hr))
			MessageBox(NULL, L"Failed to hijack imports", NULL, MB_OK | MB_ICONHAND);
	}
	else if (fdwReason == DLL_PROCESS_DETACH) 
	{
		// Restore old handler
		HRESULT hr = ApiHijackImports(::GetModuleHandle(NULL), moduleToHijack, functionToHijack, pOldHandler, NULL);
		if (!SUCCEEDED(hr))
			MessageBox(NULL, L"Failed to hijack imports", NULL, MB_OK | MB_ICONHAND);
	}
	else
	{
	}

	return(TRUE);
}

/*
		//MessageBoxA(NULL,"Attach",NULL,MB_OK);
		char szBuf[MAX_PATH * 100] = { 0 };
		PBYTE pb = NULL;
		MEMORY_BASIC_INFORMATION mbi;
		while (VirtualQuery(pb, &mbi, sizeof(mbi)) == sizeof(mbi)) 
		{
			int nLen;
			char szModName[MAX_PATH];
			if (mbi.State == MEM_FREE)
				mbi.AllocationBase = mbi.BaseAddress;
			if ((mbi.AllocationBase == hinstDll) ||
				(mbi.AllocationBase != mbi.BaseAddress) ||
				(mbi.AllocationBase == NULL)) {
					nLen = 0;
			} else {
				nLen = GetModuleFileNameA((HINSTANCE) mbi.AllocationBase,
					szModName, sizeof(szModName));
			}
			if (nLen > 0) {
				wsprintfA(strchr(szBuf, 0), "\n%p-%s",
					mbi.AllocationBase, szModName);
			}
			pb += mbi.RegionSize;
		}
		MessageBoxA(NULL,(&szBuf[1]),NULL,MB_OK);
*/

/*
#include <windows.h>
#include <Tlhelp32.h>
#include <ImageHlp.h>
#include <iostream>
#include <fstream>

#pragma comment(lib,"DbgHelp.lib")

void GetCurrentProcessName(ULONG ID ,LPSTR szExeName);
void ReplaceIATEntryInOneMod(PCSTR pszCalleeModName, PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller);
void INIT ();
int WINAPI MyMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType);

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
switch( fdwReason ){
case DLL_PROCESS_ATTACH:
INIT();
break;
case DLL_THREAD_ATTACH:

break;
case DLL_THREAD_DETACH:

break;
case DLL_PROCESS_DETACH:

break;    
}
return TRUE;  
}

void INIT ()
{
char s[50];
PROC MessageBOX = GetProcAddress(GetModuleHandle(TEXT("User32")), "MessageBoxA");
GetCurrentProcessName(GetCurrentProcessId(),s);

//char[] to LPCWSTR
WCHAR result[50];
MultiByteToWideChar(CP_ACP, 0, s, 1, result, sizeof(result)/sizeof(WCHAR));  

HMODULE hmodCaller = GetModuleHandle(result);
PROC m = NULL; LPVOID box = (LPVOID)m;
box = MyMessageBox;
ReplaceIATEntryInOneMod("USER32.DLL", MessageBOX, m, hmodCaller);
return;
}

void GetCurrentProcessName(ULONG ID, LPSTR szExeName)
{
PROCESSENTRY32 Pc = { sizeof(PROCESSENTRY32) };
HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
if(Process32First(hSnapshot, &Pc))
{
do
{
if(Pc.th32ProcessID==ID)
{
//char[] to LPCWSTR
WCHAR result[50];
MultiByteToWideChar(CP_ACP, 0, szExeName, 1, result, sizeof(result)/sizeof(WCHAR));  

lstrcpy(result, Pc.szExeFile);
return ;
}
}
while(Process32Next(hSnapshot, &Pc));
}
}



void ReplaceIATEntryInOneMod(PCSTR pszCalleeModName, PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller)
{
ULONG ulSize;
ULONG tmp=0;
PIMAGE_IMPORT_DESCRIPTOR pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR) ImageDirectoryEntryToData(hmodCaller, TRUE,IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);
if (pImportDesc == NULL)
return;
for (; pImportDesc->Name; pImportDesc++)
{
PSTR pszModName =  (PSTR)((PBYTE) hmodCaller + pImportDesc->Name);
if (lstrcmpiA(pszModName, pszCalleeModName) == 0)
break;
}
if (pImportDesc->Name == 0)
return;
PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA) ((PBYTE) hmodCaller + pImportDesc->FirstThunk);
for (; pThunk->u1.Function; pThunk++)
{

PROC* ppfn = (PROC*) &pThunk->u1.Function;
BOOL fFound = (*ppfn == pfnCurrent);
if (fFound)
{
VirtualProtect(hmodCaller,0x16000,PAGE_READWRITE,&tmp);
WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew, 4, 0 );
VirtualProtect(hmodCaller,0x16000,tmp,&tmp);
return;
}
}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI MyMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType)
{
MessageBox(0,L"1",L"2",0);
return 0;
}
*/