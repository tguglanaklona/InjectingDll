// ImportDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ImportDll.h"
#include "hijacker.h"

LPVOID* searchFuncAddress(const HMODULE& hCurrent, const char* argv, LPCSTR szVictimModule, LPCSTR szEntry);
HRESULT writeProtectedMemory(LPVOID pDest, LPCVOID pSrc, DWORD dwSize);

FILE* pf = NULL;

char* argv = "C:\\Projects\\DllProject\\DllProject\\Debug\\DllProject.exe"; 
LPSTR szVictimModule = "SampleDll.dll";
LPSTR szEntry = "SymbolCounter";
LPVOID pHijacker = MySymbolCounter;
LPVOID pOldHandler = NULL;

bool OnDllProcessAttach()
{
	pf = fopen("log_importdll.txt","w+");
	fprintf(pf, "%s\n", "Begin");

	LPVOID* pProc = searchFuncAddress(::GetModuleHandle(NULL), argv, szVictimModule, szEntry);
	if (!pProc) {
		fprintf(pf, "%s\n", "Error searchFuncAddress");
		return false;
	}

	pOldHandler = *pProc;

	HRESULT hr = writeProtectedMemory(pProc, &pHijacker, sizeof(LPVOID));
	if (!SUCCEEDED(hr)) {
		fprintf(pf, "%s\n", "Error writeProtectedMemory");
		return false;
	}

	fprintf(pf, "%s\n", "End");
	fclose(pf); 
	return true;
}

bool OnDllProcessDetach()
{
	// Restore old handler
	LPVOID* pProc = searchFuncAddress(::GetModuleHandle(NULL), argv, szVictimModule, szEntry);
	if (!pProc) return false;

	HRESULT hr = writeProtectedMemory(pProc, &pOldHandler, sizeof(LPVOID));
	if (!SUCCEEDED(hr)) return false;

	return true;
}

LPVOID* searchFuncAddress(const HMODULE& hCurrent, const char* argv, LPCSTR szVictimModule, LPCSTR szEntry) 
{
	std::ifstream pefile;
	pefile.open(argv, std::ios::in | std::ios::binary);
	if(!pefile.is_open())
	{
		std::cout << "Can't open file" << std::endl;
		return 0;
	}

	try
	{
		pe32 executable(pefile);
		if(executable.has_imports())
		{
			//адрес начала таблицы импорта (pImpDesc)
			const IMAGE_IMPORT_DESCRIPTOR* import_descriptor_array = reinterpret_cast<const IMAGE_IMPORT_DESCRIPTOR*>(executable.section_data_from_rva(executable.directory_rva(IMAGE_DIRECTORY_ENTRY_IMPORT)));

			while(import_descriptor_array->Characteristics) //по таблице импорта (по модулям)
			{
				if (strcmp(szVictimModule, executable.section_data_from_rva(import_descriptor_array->Name))==0) //если в таблице импорта szVictim
				{
					std::cout << "DLL Name: " << executable.section_data_from_rva(import_descriptor_array->Name) << std::endl;
					std::cout << "Import TimeDateStamp: 0x" << std::hex << import_descriptor_array->TimeDateStamp << std::endl;

					const DWORD* import_address_table = reinterpret_cast<const DWORD*>(executable.section_data_from_rva(import_descriptor_array->FirstThunk));
					const DWORD* import_lookup_table = import_descriptor_array->OriginalFirstThunk == 0 ? import_address_table : reinterpret_cast<const DWORD*>(executable.section_data_from_rva(import_descriptor_array->OriginalFirstThunk)); //(pNamesTable)?

					DWORD address_table = import_descriptor_array->FirstThunk;

					std::string name;
					WORD hint;

					std::cout << std::endl << " hint | name/ordinal                |  address" << std::endl;

					if(import_lookup_table != 0 && import_address_table != 0)
					{
						while(true)
						{
							DWORD address = *import_address_table++;

							if(!address)
								break;

							DWORD lookup = *import_lookup_table++;

							if(IMAGE_SNAP_BY_ORDINAL32(lookup)) //lookup (pNamesTable->u1.Ordinal)
							{
								std::stringstream stream;
								stream << "#" << IMAGE_ORDINAL32(lookup);
								name = stream.str();
								hint = 0;
							}
							else
							{
								name = executable.section_data_from_rva(lookup + 2); //lookup+2 (pNamesTable->Name)
								hint = *reinterpret_cast<const WORD*>(executable.section_data_from_rva(lookup));

								bool OK = false;
								if (name.compare(szEntry)==0){
									fprintf (pf,"%s %s",name.data(),"OK full\n"); 
									OK = true;
								}
								else if (name.find(szEntry)!=std::string::npos){
									fprintf (pf,"%s %s",name.data(),"OK\n"); 
									OK = true;
								}
								else {
									fprintf (pf,"%s %s",name.data(),"NO\n"); 
									OK = false;
								}

								if (OK)
								{
									PIMAGE_THUNK_DATA pNamesTable = (PIMAGE_THUNK_DATA)((DWORD_PTR)(hCurrent) + (DWORD_PTR)(import_descriptor_array->OriginalFirstThunk));
									return (LPVOID *)((DWORD_PTR)(pNamesTable) + (DWORD_PTR)(import_descriptor_array->FirstThunk - import_descriptor_array->OriginalFirstThunk));

									//TO DEBUG:
									//LPSTR string = new char[256];
									//wsprintfA(string, "hCurrent 0x%X, address_table 0x%X, import_descriptor_array->FirstThunk 0x%X, import_descriptor_array->OriginalFirstThunk 0x%X pProc 0x%X, pNamesTable 0x%X", hCurrent, address_table, import_descriptor_array->FirstThunk, import_descriptor_array->OriginalFirstThunk, pProc, pNamesTable);
									//fprintf(pf,"%s ",string);
								}
								
							}

							std::cout << std::dec << "[" << std::setfill('0') << std::setw(4) << hint << "]"
								<< " " << std::left << std::setfill(' ') << std::setw(30) << name
								<< ":0x" << std::hex << std::right << std::setfill('0') << std::setw(8) << address_table
								<< std::endl;

							address_table += 4;
						}
					}

					std::cout << "==========" << std::endl << std::endl;
				}//if

				import_descriptor_array++; //по таблице импорта (по массиву структур IMAGE_IMPORT_DESCRIPTOR)
			}
		}
	}
	catch(const pe_exception& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	return NULL;
}

HRESULT writeProtectedMemory(LPVOID pDest, LPCVOID pSrc, DWORD dwSize)
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
