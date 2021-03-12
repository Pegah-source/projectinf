#ifndef URL_H
#define URL_H

#include <stddef.h>

typedef struct next
{
	//the array of size 64 of words
	char nextWords[64][40];
	
} next ;

int binvalue (char binary[4]);

int readfromjson(size_t size,char words[size][40],next pointersToNext[size]);
void writencode(size_t size,char words[size][40],next pointersToNext[size],int order,int numPacket,int length,int *temp);
char* concat(const char *s1, const char *s2);

#endif