#include <windows.h>
#include <Imagehlp.h>
#include <Tlhelp32.h>
#include <string.h>
#include <iostream>

#include "process.h"

// implicit linking needed

BOOL WINAPI InjectLibW(DWORD dwProcessId, PCWSTR pszLibFile)
{
	BOOL fOk = FALSE; 
	HANDLE hProcess = NULL, hThread = NULL;
	PWSTR pszLibFileRemote = NULL;
	__try 
	{
		hProcess = OpenProcess(	
			PROCESS_CREATE_THREAD | 
			PROCESS_VM_OPERATION | 
			PROCESS_VM_WRITE, 
			FALSE, dwProcessId);
		if (hProcess == NULL) __leave;

		//строка с именем dll - выделяем ей память в процессе и загоняем
		int cb = (1 + lstrlenW(pszLibFile)) * sizeof(WCHAR);
		pszLibFileRemote = (PWSTR) VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
		if (pszLibFileRemote == NULL) __leave;
		if (!WriteProcessMemory(hProcess, pszLibFileRemote, (PVOID) pszLibFile, cb, NULL)) __leave;

		//загружаем dll (LoadLibrary) в удаленный процесс
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
		if (pfnThreadRtn == NULL) __leave;
		hThread = CreateRemoteThread(hProcess, NULL, 0, pfnThreadRtn, pszLibFileRemote, 0, NULL);
		if (hThread == NULL) __leave;

		WaitForSingleObject(hThread, INFINITE);
		fOk = TRUE;
	}
	__finally 
	{ 
		if (pszLibFileRemote != NULL)
			VirtualFreeEx(hProcess, pszLibFileRemote, 0, MEM_RELEASE);
		if (hThread != NULL)
			CloseHandle(hThread);
		if (hProcess != NULL)
			CloseHandle(hProcess);
	}
	return(fOk);
}

BOOL WINAPI EjectLibW(DWORD dwProcessId, PCWSTR pszLibFile) 
{
	BOOL fOk = FALSE; 
	HANDLE hthSnapshot = NULL;
	HANDLE hProcess = NULL, hThread = NULL;
	__try 
	{
		//нужен будет base address на pszLibFile, берем его HMODULE (MODULEENTRY32W)
		hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
		if (hthSnapshot == NULL) __leave;
		MODULEENTRY32W me = { sizeof(me) };
		BOOL fFound = FALSE;
		BOOL fMoreMods = Module32FirstW(hthSnapshot, &me);
		for (; fMoreMods; fMoreMods = Module32NextW(hthSnapshot, &me)) 
		{
			fFound = (lstrcmpiW(me.szModule, pszLibFile) == 0) || (lstrcmpiW(me.szExePath, pszLibFile) == 0);
			if (fFound) break;
		}
		if (!fFound) __leave;
		
		hProcess = OpenProcess(
			PROCESS_CREATE_THREAD |
			PROCESS_VM_OPERATION, 
			FALSE, dwProcessId);
		if (hProcess == NULL) __leave;
		
		//выгружаем dll (FreeLibrary) из удаленного процесса
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "FreeLibrary");
		if (pfnThreadRtn == NULL) __leave;
		hThread = CreateRemoteThread(hProcess, NULL, 0, pfnThreadRtn, me.modBaseAddr, 0, NULL);
		if (hThread == NULL) __leave;
		
		WaitForSingleObject(hThread, INFINITE);
		fOk = TRUE; 
	}
	__finally 
	{ 
		if (hthSnapshot != NULL)
			CloseHandle(hthSnapshot);
		if (hThread != NULL)
			CloseHandle(hThread);
		if (hProcess != NULL)
			CloseHandle(hProcess);
	}
	return(fOk);
}

int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) 
{
	//процесс
	HANDLE handle = processHandle(L"DllProject.exe"); //куда внедряем
	DWORD dwProcessId = GetProcessId(handle);
	CloseHandle(handle);

	//dll
	TCHAR szLibFile[MAX_PATH];
	//GetModuleFileName(NULL, szLibFile, sizeof(szLibFile));
	//_tcscpy(_tcsrchr(szLibFile, TEXT('\\')) + 1, TEXT("Dll.dll"));
	_tcscpy(szLibFile, TEXT("C:\\Projects\\InjectDll\\Dll\\Debug\\ImportDll.dll")); //что внедряем

	//инжектируем
	if (InjectLibW(dwProcessId, szLibFile)) 
	{
		MessageBox(NULL,L"DLL Injection/Ejection successful. Pause!",NULL,MB_OK); 
		EjectLibW(dwProcessId, szLibFile);
	} 
	else 
	{
		MessageBox(NULL,L"DLL Injection/Ejection failed.",NULL,MB_OK);
	}
}

//the same as processHandle
HANDLE GetProcessHandle(LPCWSTR szExeName)
{
	PROCESSENTRY32 Pc = { sizeof(PROCESSENTRY32) };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (Process32First(hSnapshot, &Pc))
	{
		do
		{
			if(!lstrcmp(Pc.szExeFile, szExeName))
			{
				return OpenProcess(PROCESS_ALL_ACCESS, TRUE, Pc.th32ProcessID);
			}
		}
		while(Process32Next(hSnapshot, &Pc));
	}
	return NULL;
}