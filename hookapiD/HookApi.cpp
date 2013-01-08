// HookApi.cpp : main source file for HookApi.exe
//

#include "stdafx.h"

#include "maindlg.h"

CAppModule _Module;

//@//////////////////////////////////////////////////////////////////////////
//	Utility

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

	char aaa[256];
	DWORD ss = GetLastError();

	wsprintf(aaa, "ss %d pDest 0x%08X, pSrc 0x%08X, dwSize %d", ss, pDest, pSrc, dwSize);
	MessageBox(NULL, aaa, NULL, MB_OK);

	return HRESULT_FROM_WIN32(ss);//GetLastError());
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	CMainDlg dlgMain;

	int nRet = dlgMain.DoModal(::GetDesktopWindow());

	_Module.Term();

	return nRet;
}
