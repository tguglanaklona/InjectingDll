#include "import.h"
#include <psapi.h>
#include <string>
#include <Dbghelp.h>

HRESULT WriteProtectedMemory(LPVOID pDest, LPCVOID pSrc, DWORD dwSize)
{
	// Make it writable
	DWORD dwOldProtect = 0;
	if (::VirtualProtect(pDest, dwSize, PAGE_READWRITE, &dwOldProtect))
	{
		::MoveMemory(pDest, pSrc, dwSize);

		// Restore protection
		::VirtualProtect(pDest, dwSize, dwOldProtect, &dwOldProtect);
		return S_OK;
	}

	wchar_t aaa[256];
	DWORD ss = GetLastError();

	wsprintf(aaa, L"ss %d pDest 0x%08X, pSrc 0x%08X, dwSize %d", ss, pDest, pSrc, dwSize);
	MessageBox(NULL, aaa, NULL, MB_OK);

	return HRESULT_FROM_WIN32(ss);//GetLastError());
}

HRESULT ApiHijackImports(
				HMODULE hModule,	//хэндл на эту программу (метод импорта), в ней замещающая процедура
				LPSTR szVictim,
				LPSTR szEntry,
				LPVOID pHijacker,	//замещающая процедура
				LPVOID *ppOrig
				)
{
	FILE* pf = fopen("importlog.txt","a+");

	// Check args
	if (::IsBadStringPtrA(szVictim, -1) ||
		(!IS_INTRESOURCE(szEntry) && ::IsBadStringPtrA(szEntry, -1)) ||
		::IsBadCodePtr(FARPROC(pHijacker)))
	{
		fclose(pf); return E_INVALIDARG;
	}

	PIMAGE_DOS_HEADER pDosHeader = PIMAGE_DOS_HEADER(hModule); //DOS заголовок PE файла (этой программы)

	if (::IsBadReadPtr(pDosHeader, sizeof(IMAGE_DOS_HEADER)) ||
		IMAGE_DOS_SIGNATURE != pDosHeader->e_magic)
	{
		fclose(pf); return E_INVALIDARG;
	}

	PIMAGE_NT_HEADERS pNTHeaders = 
		MakePtr(PIMAGE_NT_HEADERS, hModule, pDosHeader->e_lfanew); //PE заголовок (этой программы)

	if (::IsBadReadPtr(pNTHeaders, sizeof(IMAGE_NT_HEADERS)) ||
		IMAGE_NT_SIGNATURE != pNTHeaders->Signature)
	{
		fclose(pf); return E_INVALIDARG;
	}

	HRESULT hr = E_UNEXPECTED;

	// Locate the victim
	IMAGE_DATA_DIRECTORY& impDir = 
		pNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]; //элемент директории данных опционального заголовка (структура): символы импорта
	PIMAGE_IMPORT_DESCRIPTOR pImpDesc = 
		MakePtr(PIMAGE_IMPORT_DESCRIPTOR, hModule, impDir.VirtualAddress), //адрес начала таблицы импорта (массив структур IMAGE_IMPORT_DESCRIPTOR) 
			pEnd = pImpDesc + impDir.Size / sizeof(IMAGE_IMPORT_DESCRIPTOR) - 1; //адрес конца таблицы

	while(pImpDesc < pEnd)
	{
		char *p; //debug
		p=MakePtr(char*,hModule,pImpDesc->Name);
		
		if (0 == ::lstrcmpiA(MakePtr(LPSTR, hModule, pImpDesc->Name), szVictim)) //в таблице импорта szVictim
		{
			if (0 == pImpDesc->OriginalFirstThunk)
			{
				// no import names table
				fclose(pf); return E_UNEXPECTED;
			}

			// Locate the entry
		    PIMAGE_THUNK_DATA pNamesTable =
				MakePtr(PIMAGE_THUNK_DATA, hModule, pImpDesc->OriginalFirstThunk);

			if (IS_INTRESOURCE(szEntry))
			{
				// By ordinal
				while(pNamesTable->u1.AddressOfData)
				{
					if (IMAGE_SNAP_BY_ORDINAL(pNamesTable->u1.Ordinal) &&
						WORD(szEntry) == IMAGE_ORDINAL(pNamesTable->u1.Ordinal))
					{
						hr = S_OK;
						break;
					}
					pNamesTable++;
				}
			}
			else
			{
				// By name
				while(pNamesTable->u1.AddressOfData)
				{
					if (!IMAGE_SNAP_BY_ORDINAL(pNamesTable->u1.Ordinal))
					{
						PIMAGE_IMPORT_BY_NAME pName = MakePtr(PIMAGE_IMPORT_BY_NAME,
											hModule, pNamesTable->u1.AddressOfData);

						if (0 == ::lstrcmpiA(LPSTR(pName->Name), szEntry))
						{
							hr = S_OK;
							break;
						}
					}
					pNamesTable++;
				}
			}

			if (SUCCEEDED(hr))
			{
				// Get address
				LPVOID *pProc = MakePtr(LPVOID *, pNamesTable,
							pImpDesc->FirstThunk - pImpDesc->OriginalFirstThunk);
				//pImpDesc->FirstThunk - выводится как Address в проге pe_class - import_descriptor_array->FirstThunk

				// Save original handler
				if (ppOrig)
					*ppOrig = *pProc;

				// write to write-protected memory
				fprintf(pf, "0x%X 0x%X 0x%X 0x%X\n", pProc, pNamesTable, pImpDesc->FirstThunk, pImpDesc->OriginalFirstThunk);
				fclose(pf); return WriteProtectedMemory(pProc, &pHijacker, sizeof(LPVOID));
			}
			break;
		}
		pImpDesc++;
	}

	fclose(pf);
	return hr;
}

//int MySymbolCounter(const unsigned char *a)
//int MySymbolCounter(const char *a, char b, int c, double r)
int MySymbolCounter(mystruct* str)
{
	FILE* pf = fopen("MySymbolCounter.txt","w+");
	//fprintf(pf,"%s, %c, %d, %f\n",str->a,str->b,str->c,str->r);
	fprintf(pf,"%d\n",str->in1);
	fclose(pf);
	system("MySymbolCounter.txt");
	return 10;
}

int
WINAPI
MyMessageBoxA(
    __in_opt HWND hWnd,
    __in_opt LPCSTR lpText,
    __in_opt LPCSTR lpCaption,
    __in UINT uType)
{
	//exit(0);

	FILE* pf = fopen("MessageBoxInput.txt","w+");
	fprintf(pf,"hWnd = 0x%x\n",hWnd);
	fprintf(pf,"lpText = %s\n",lpText);
	fprintf(pf,"lpCaption = %s\n",lpCaption);
	fprintf(pf,"uType = %d\n",uType);
	fclose(pf);
	system("MessageBoxInput.txt");

	return ::MessageBeep(2);
}


