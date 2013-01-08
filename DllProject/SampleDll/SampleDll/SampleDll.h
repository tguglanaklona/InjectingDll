//MyDll.h
#ifndef MYDLLH
#define MYDLLH

#pragma comment(lib,"C:\\Projects\\DllProject\\DllToImport\\Release\\DllToImport.lib") //implicit linking
#include "C:\\Projects\\DllProject\\DllToImport\\DllToImport\\DllToImport.h" //implicit linking

class __declspec(dllexport) Class 
{
public:
	Class(){};
	~Class(){};

	int c; 
	char * s;
	char b; 
	
	double r;

	BOOL NoArgs (int);
};

extern "C" _declspec(dllexport) int SymbolCounter(Class& cl);
//extern "C" _declspec(dllexport) BOOL NoArgs (int);
extern "C" _declspec(dllexport) int SymbolCount(char* lpSource, char lpMask, int size, double r);

#endif