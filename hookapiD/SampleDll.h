//MyDll.h
#ifndef MYDLLH
#define MYDLLH

#pragma comment(lib,"DllToImport.lib") //implicit linking
#include "DllToImport.h" //implicit linking

extern "C" _declspec(dllexport) int SymbolCounter(const char *, char, int);
extern "C" _declspec(dllexport) BOOL NoArgs (int);

#endif