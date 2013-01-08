#include <string>

#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>

void shortName(LPWSTR strToChange);
LPVOID imageBase(LPWSTR szVictimProcess, LPWSTR szVictim);
HANDLE processHandle(LPWSTR szVictimProcess);