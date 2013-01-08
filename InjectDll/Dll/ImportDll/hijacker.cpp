#include "stdafx.h"
#include "hijacker.h"

int MySymbolCounter(unsigned char* str)
{
	FILE* pf = fopen("MySymbolCounter.txt","w+");

	for (int i=0;i<100;i++)
		fprintf(pf,"0x%X:  0x%X \n",str+i,*(str+i));

	MessageBox(NULL,L"get it!",L"get",MB_OK);

	fclose(pf);
	system("MySymbolCounter.txt");
	return 10;
}

BOOL MyNoArgs(int a)
{
	FILE* pf = fopen("MyNoArgs.txt","w+");
	fprintf(pf,"%d\n",a);
	fclose(pf);
	system("MyNoArgs.txt");
	return TRUE;
}