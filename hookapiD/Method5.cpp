//	Project: HookApi
//	File:    Method5.cpp
//	Author:  Paul Bludov
//	Date:    10/12/2001
//
//	Description:
//		NONE
//
//	Update History:
//		NONE
//
//@//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"

//@//////////////////////////////////////////////////////////////////////////
//	Method5: Inject break to original handler

static DWORD_PTR m_dwFunction;

static LONG WINAPI MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	if (pExceptionInfo->ContextRecord->Eip != m_dwFunction)
		return EXCEPTION_CONTINUE_SEARCH;

	// Continue execution from MyMessageBeep
	pExceptionInfo->ContextRecord->Eip = (DWORD_PTR)MyMessageBeep;
	return EXCEPTION_CONTINUE_EXECUTION;
}

LRESULT CMainDlg::OnMethod5(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
#ifdef _DEBUG
	if (::IsDebuggerPresent())
	{
		MessageBox(_T("Not available while a debugger present"), _T("HookApi"));
		return 0;
	}
#endif

	m_dwFunction = (DWORD_PTR)::GetProcAddress(::GetModuleHandle("USER32.dll"), "MessageBeep");
	BYTE nSavedByte = *(LPBYTE)m_dwFunction;
	LPTOP_LEVEL_EXCEPTION_FILTER pOldFilter = ::SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	const BYTE nInt3 = 0xCC;
	// Inject int 3
	HRESULT hr = WriteProtectedMemory(LPVOID(m_dwFunction), &nInt3, sizeof(const BYTE));
	if (SUCCEEDED(hr))
	{
		::MessageBeep(m_uType);

		// Restore function
		hr = WriteProtectedMemory(LPVOID(m_dwFunction), &nSavedByte, sizeof(BYTE));
	}

	::SetUnhandledExceptionFilter(pOldFilter);

	return 0;
}

//@//////////////////////////////////////////////////////////////////////////
//	End Of File Method5.cpp
