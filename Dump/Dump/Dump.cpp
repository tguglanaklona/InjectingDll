// Dump.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>

#define MAX_DUMPS 0x10 // макс. кол-во поддерживаемых дампов

int _tmain(int argc, char** argv)
{
	int c = 6;
	char* v[6] = {"Play89_0.dmp","Play89_1.dmp","Play89_2.dmp","Play89_3.dmp","Play89_4.dmp","Play89_5.dmp"};
	FILE* flog = fopen("log.txt", "w+");

	for (int j=0;j<c-1;j++)
	{
		int pos = 0; int a, b, flag; unsigned char ch[MAX_DUMPS]; FILE* f[MAX_DUMPS];
		for (a = 0; a < c; a++)
			ch[a] = 0;

		// открываем файлы
		for (a = j; a <= j+1; a++)
			if (!(f[a] = fopen(v[a], "r+")))
				return fprintf(flog,"-err: cant open %s\n", v[a]);

		bool end = false; bool match = true;
		fprintf(flog,"\n difference of "); for (a = j; a <= j+1; a++) fprintf(flog,"\t%s", v[a]);
		while (!end)
		{
			// чтение €чеек из первых двух дампов
			for (a = j; a <= j+1; a++){
				if (!fread(&ch[a], 1, 1, f[a])) {end = true; break;}
			}
			if (end) break;
			pos++;

			// если €чейки двух дампов не совпадают, выводим их
			if (memcmp(&ch[j],&ch[j+1],1)!=0) 
			{
				match = false;
				for (fprintf(flog,"\n%08Xh:", pos - 1), a = j; a <= j+1; a++){
					fprintf(flog,"\t%02Xh", ch[a]);
				}
			}

		} 
		if (match)
			fprintf(flog,"\n NO difference.");

		fclose(f[j]); fclose(f[j+1]);
	}



	fclose(flog);

	return 0;
}

/*
int _tmain(int argc, char** argv)
{
	int c = 6;
	char* v[6] = {"Play89_0.dmp","Play89_1.dmp","Play89_2.dmp","Play89_3.dmp","Play89_4.dmp","Play89_5.dmp"};
	FILE* flog = fopen("log.txt", "w+");

	// объ€вл€ем переменные
	int pos = 0; int a, b, flag; unsigned char ch[MAX_DUMPS]; FILE* f[MAX_DUMPS];
	for (a = 0; a < c; a++)
		ch[a] = 0;

	// открываем все файлы
	for (a = 0; a < c; a++)
		if (!(f[a] = fopen(v[a], "rb")))
			return fprintf(flog,"-err: cant open %s\n", v[a]);

	// печать имен файлов
	fprintf(flog,"raw offset"); for (a = 1; a < c; a++) fprintf(flog,"\t%s", v[a]);

	while (1)
	{
		// чтение очередной €чейки из каждого файла
		for (a = 0; a < c; a++){
			if (!fread(&ch[a], 1, 1, f[a])) {fclose(flog); return 0;}
		}
		pos++;

		// если €чейки двух первых дампов совпадают, они отбрасываютс€
		if (ch[0] - ch[1]) continue;

		// поиск совпадающих €чеек во втором и всех последущих дампах
		// (такие €чеки отбрасываютс€ как "левые" и нафиг не нужные)
		for (a = flag = 1; a < c; a++)
			for (b = a; b < c; b++) if ((a - b) && (ch[a] == ch[b])) flag = 0; 

		//оставл€ем €чейки, значени€ которых в 3 и 5 дампах совпадают
		if (flag){
			if (memcmp(&ch[3],&ch[5],1)!=0) flag = 0;
		}

		// печать "правильных" €чеек
		if (flag){
			for (fprintf(flog,"\n%08Xh:", pos - 1), a = 1; a < c; a++){
				fprintf(flog,"\t%02Xh", ch[a]);
			}
		}
	} fprintf(flog,"\n");

	fclose(flog);

	return 0;
}
*/