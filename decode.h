#ifndef URL_H
#define URL_H

#include <stdio.h>
#include <stdlib.h>

typedef struct next
{
	//the array of size 64 of words
	char nextWords[64][40];
	
} next;

//char* convertbinary(int value);
char* decodetxt(size_t size,char words[size][40],next pointersToNext[size]);
int readfromjson(size_t size,char words[size][40],next pointersToNext[size]);

#endif