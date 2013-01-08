//	Project: HookApi
//	File:    Method1.cpp
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

//@//////////////////////////////////////////////////////////////////////////
//	Method1: preprocessor define

#define MessageBeep(uType) MyMessageBeep(uType)

LRESULT CMainDlg::OnMethod1(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return ::MessageBeep(m_uType);
}


//@//////////////////////////////////////////////////////////////////////////
//	End Of File Method1.cpp
