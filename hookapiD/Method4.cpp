//	Project: HookApi
//	File:    Method4.cpp
//	Author:  Paul Bludov
//	Date:    09/05/2001
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

#include <detours.h>

//@//////////////////////////////////////////////////////////////////////////
//	Method4: Inject code to original handler

LRESULT CMainDlg::OnMethod4(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PBYTE pbTrampoline = DetourFunction(PBYTE(::MessageBeep), PBYTE(MyMessageBeep));

	if (pbTrampoline)
	{
		::MessageBeep(m_uType);

		DetourRemove(pbTrampoline, PBYTE(MyMessageBeep));
	}
	else
		MessageBox(_T("Failed to detour ::MessageBeep()"), NULL, MB_OK | MB_ICONHAND);

	return 0;
}


//@//////////////////////////////////////////////////////////////////////////
//	End Of File Method4.cpp
