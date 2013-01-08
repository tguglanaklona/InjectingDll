//Source.cpp
#include <windows.h>
#include <math.h>
#include "SampleDll.h"

extern "C" _declspec(dllexport) int SymbolCounter(Class& cl)
{
	char* lpSource = cl.s;
	char lpMask = cl.b;
	int size = cl.c;
	double r = cl.r;

	int number = 0;
	if( lpSource == NULL )
		return -1;
	else
	{
		for(int i = 0; i < size; i++ )
		{
			if(lpSource[i] == lpMask)
				number++;
		}
	}
	return number;
}

BOOL Class::NoArgs (int i)
{
	int a = 3;
	int b = 4;
	double c = a*a+b*b;
	c  = c - 1;
	c = double(c) + 1;
	a = i = b;
	return functionX(i);
}

extern "C" _declspec(dllexport) int SymbolCount(char* lpSource, char lpMask, int size, double r)
{
	Class cl;
	cl.s = lpSource;
	cl.b = lpMask;
	cl.c = size;
	cl.r = r;
	return SymbolCounter(cl);
}