#include "process.h"

#include <psapi.h>
#include <Dbghelp.h>

void shortName(LPWSTR strToChange)
{
	std::wstring path(strToChange);
	std::wstring filename;

	size_t pos = path.find_last_of(L"\\");
	if (pos != std::string::npos)
		filename.assign(path.begin() + pos + 1, path.end());
	else
		filename = path;

	lstrcpyW(strToChange, filename.data());
}

HANDLE processHandle(LPWSTR szVictimProcess)
{
	DWORD aProcesses[1024], cbNeeded, nProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return NULL;
    nProcesses = cbNeeded / sizeof(DWORD);

	HANDLE ProcHandle = 0;
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	for (i = 0; i < nProcesses; i++)
	{
		ProcHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, aProcesses[i]);
		
		if (NULL != ProcHandle) 
		{
			HMODULE hMod = 0;
			GetModuleBaseName(ProcHandle, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR)); // Get the process name
			if (0 == lstrcmpi(szVictimProcess, szProcessName)){
				return ProcHandle;
			}
		}

		CloseHandle(ProcHandle);
	}

	return 0;
}

LPVOID imageBase(LPWSTR szVictimProcess, LPWSTR szVictim)
{
	//находим процесс szVictimProcess
	DWORD aProcesses[1024], cbNeeded, nProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return NULL;
    nProcesses = cbNeeded / sizeof(DWORD);

	HANDLE ProcHandle = 0;
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	for (i = 0; i < nProcesses; i++)
	{
		ProcHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, aProcesses[i]);
		
		if (NULL != ProcHandle) 
		{
			HMODULE hMod[1024];
			if ( EnumProcessModules(ProcHandle, hMod, sizeof(hMod), &cbNeeded) )
			{
				GetModuleBaseName(ProcHandle, hMod[0], szProcessName, sizeof(szProcessName)/sizeof(TCHAR)); // Get the process name
				if (0 == lstrcmpi(szVictimProcess, szProcessName))
				{
					//находим модуль szVictim
					DWORD nModules = cbNeeded / sizeof(HMODULE);
					TCHAR szModName[MAX_PATH];
					for (unsigned int j = 0; j < nModules; j++)
					{
						if (GetModuleFileNameEx(ProcHandle, hMod[j], szModName, sizeof(szModName))) // Get the module name
						{
							shortName(szModName);
							if (0 == lstrcmpi(szModName, szVictim)) 
							{
								MODULEINFO info;
								GetModuleInformation(ProcHandle, hMod[j], &info, sizeof(info));
								CloseHandle(ProcHandle);
								return info.lpBaseOfDll;

								//Equal To:
								//CloseHandle(ProcHandle);
								//return hMod[j];
								
								//Debug:
								//LPSTR string = new char[256];
								//wsprintf(string,"\t%s (0x%08X)\n", szModName, hMod[j]);
							}
						}
					}
					break;
				}
			}
		}
		
		CloseHandle(ProcHandle);
	}

	return NULL; 
}