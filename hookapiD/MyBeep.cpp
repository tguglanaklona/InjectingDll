//	Project: HookApi
//	File:    MyBeep.cpp
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
#include "resource.h"

#pragma comment(lib, "winmm.lib")

//@//////////////////////////////////////////////////////////////////////////
// My custom beep routine

BOOL WINAPI MyMessageBeep ( IN UINT uType)
{
	UINT uResID = 0;
	switch(uType)
	{
		case MB_ICONASTERISK:
			uResID = IDR_ASTERISK;
			break;
		case MB_ICONEXCLAMATION:
			uResID = IDR_EXCLAMATION;
			break;
		case MB_ICONHAND:
			uResID = IDR_HAND;
			break;
		case MB_ICONQUESTION:
			uResID = IDR_QUESTION;
			break;
		case MB_OK:
			uResID = IDR_DEFAULT;
			break;
		default:
			uResID = IDR_BEEP;
			break;
	}

	return ::PlaySound(MAKEINTRESOURCE(uResID), 
			_Module.GetResourceInstance(), SND_RESOURCE | SND_ASYNC);
}

int
WINAPI
MyHereMessageBoxA(
    __in_opt HWND hWnd,
    __in_opt LPCSTR lpText,
    __in_opt LPCSTR lpCaption,
    __in UINT uType)
{
	exit(0);
	return MyMessageBeep(0);
}

BOOL MyNoArgs (int i)
{
	UINT uResID = 0;
	switch(i)
	{
		case MB_ICONASTERISK:
			uResID = IDR_ASTERISK;
			break;
		case MB_ICONEXCLAMATION:
			uResID = IDR_EXCLAMATION;
			break;
		case MB_ICONHAND:
			uResID = IDR_HAND;
			break;
		case MB_ICONQUESTION:
			uResID = IDR_QUESTION;
			break;
		case MB_OK:
			uResID = IDR_DEFAULT;
			break;
		default:
			uResID = IDR_BEEP;
			break;
	}

	return ::PlaySound(MAKEINTRESOURCE(uResID), 
			_Module.GetResourceInstance(), SND_RESOURCE | SND_ASYNC);
}
//@//////////////////////////////////////////////////////////////////////////
//	End Of File MyBeep.cpp

