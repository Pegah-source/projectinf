#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jansson.h>
#include <stdbool.h>

#include "decode.c"
#include "decode.h"


int main(int argc, char * argv[]){
	size_t size = 2000;
	char words[size][40];
	next pointersToNext[size];
	
	
	size = readfromjson(size,words,pointersToNext);
	
	
	char* recu = decodetxt(size,words,pointersToNext);
	printf("%s",recu);
	
	return 0;
}
