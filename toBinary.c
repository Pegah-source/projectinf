#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
int toBinary(char *c, unsigned int size);

int main(){
    unsigned char a[] = {25,2,4,5,7,8,9,5,2,3,4,1};
    if (toBinary(a,sizeof(a)) == 1){
        perror("Error writing to file");
        exit(EXIT_FAILURE);
    }


    FILE *rptr = fopen("test.bin", "rb");
    if (rptr == NULL){
        perror("open file error");
    }
    unsigned int b[sizeof(a)];
    bzero(b,sizeof(a));
//    int *b = malloc(5*sizeof(char));
    int c = fread(b, 11, sizeof(char), rptr);
    printf("%d Bytes", c);
    for (int i = 0; i<sizeof(a); i++){
        printf("%d", b[i]);
    }
    fclose(rptr);
*/
//}

int toBinary(char *c, unsigned int size){
    FILE *fptr = fopen("test.bin", "wb");
    fwrite(c, size, 1, fptr);
	fclose(fptr);
    return 0;
}
