#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jansson.h>
#include <math.h>

#include "encode.h"

//we have a list of 4-bit binary numbers and we want to encode it using the json library
/*
int main(int argc, char * argv[]){
*/		
	/*json_t *json;
    json_error_t error;
	size_t size = 2000;
	char words[size][40];
	next pointersToNext[size];
	
	size = readfromjson(size,words,pointersToNext);*/
    /*json = json_load_file("json_dump_file.json", 0, &error);
	if(json_is_array(json)){
		size = json_array_size(json);
		for(int i=0;i < size;i++){
			json_t *jsontmp = json_array_get(json,i);//in the jsontmp we have the i_th json cell that constaints
			//the word and the array nextwords
			
			json_t *word = json_object_get(jsontmp,"mainWord");
			json_t *nextwords = json_object_get(jsontmp,"nextWords");
			memcpy(words[i],json_string_value(word),json_string_length(word));
			//words[i] = json_string_value(word);
			size_t index;
			json_t *value;

			json_array_foreach(nextwords, index, value) {//values are the next words that we have to convert
			//to string and put them back into the next structure "pointersToNext"
			
				const char *str = json_string_value(value);
				memcpy((pointersToNext[i].nextWords)[index],str,strlen(str));
				//(pointersToNext[i].nextWords)[index] = str;
			}
		}
	}*/
	
	//now we have all the information in the two arrays words and pointersToNext, we encode 4-bit binary numbers
	//suppose we have the number of these numbers
    /*FILE *fptr;
    fptr = fopen("message.txt", "w"); 
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }
	for(int i = 0;i < size;i++){
		printf("library : %s and %d \n", words[i],size);
	}//100111101110011
	char binaryarray[10][4] = {"1001","1111","0111","0011","0001","1010","1110","1100","1000","0101"};
	char* str1 = (char*)malloc(40*sizeof(char));
	memcpy(str1,"",strlen(""));
	for(int i = 0;i < 10;i++){//i_th 4-bit number to encode
		int a = binvalue(binaryarray[i]);//temp
		if(strcmp(str1, "")==0){
			fprintf(fptr, "%s", words[a]);
			fprintf(fptr, "%s", " ");
			printf("%d with the trsing %s \n", a, words[a]);
			printf("1111 \n");
			memcpy(str1,words[a],strlen(words[a]));
		}else{
			int j = 0;
			while(j<size && strcmp(str1,words[j])!=0){//str1 is the last word we have printed in the text file
				j++;
			}
			
			//printf("this is j : %d \n", j);
			if(j == size){//if the last word was not in our library => simply print the words[binvalue[binaryarray[i]]]
				fprintf(fptr, "%s",words[a]);
				fprintf(fptr, "%s", " ");
				printf("%d with the string %s \n", a, words[a]);
				printf("22222 \n");
				free(str1);
				str1 = (char*)malloc(40*sizeof(char));
				memcpy(str1,words[a],strlen(words[a]));
				//printf("the string %s in %d \n", words[a],a);
				//str1 = words[a];
			}else{
				fprintf(fptr, "%s", (pointersToNext[j].nextWords)[a]);//we print the correspong pointersToNext
				fprintf(fptr, "%s", " ");
				//printf("%d with the string %s \n", a , (pointersToNext[j].nextWords)[a]);
				//printf("333333 \n");
				free(str1);
				str1 = (char*)malloc(40*sizeof(char));
				memcpy(str1,(pointersToNext[j].nextWords)[a],strlen((pointersToNext[j].nextWords)[a]));
				//str1 = (pointersToNext[j].nextWords)[a];
			}
		}
	}
	 
    fclose(fptr);*/
/*
    return 0;
}
*/

/*int binvalue (char binary[4]){
	int sum = 0;
	if(binary[3]=='1')
		sum += 1;
	
	if(binary[2]=='1')
		sum += 2;
	
	if(binary[1]=='1')
		sum += 4;
	
	if(binary[0]=='1')
		sum += 8;
	
	return sum;
}*/
int readfromjson(size_t size,char words[size][40],next pointersToNext[size]){
	json_t *json;
    json_error_t error;
    json = json_load_file("json_dump_file.json", 0, &error);
	if(json_is_array(json)){
		size = json_array_size(json);
		for(int i=0;i < size;i++){
			json_t *jsontmp = json_array_get(json,i);//in the jsontmp we have the i_th json cell that constaints
			//the word and the array nextwords
			
			json_t *word = json_object_get(jsontmp,"mainWord");
			json_t *nextwords = json_object_get(jsontmp,"nextWords");
			memcpy(words[i],json_string_value(word),json_string_length(word));
			//words[i] = json_string_value(word);
			size_t index;
			json_t *value;

			json_array_foreach(nextwords, index, value) {//values are the next words that we have to convert
			//to string and put them back into the next structure "pointersToNext"
			
				const char *str = json_string_value(value);
				memcpy((pointersToNext[i].nextWords)[index],str,strlen(str));
				//(pointersToNext[i].nextWords)[index] = str;
			}
		}
	}

	return size;
}

//the following function makes a text file of just a part of a captured packet
//the arguments we need are : array words and nextwords as library and also their size, a pointer to integer temp and the
//length of the the integer we want to encode, a number to indicate the part of the the message in the ordering
//number of the packet

//in this function we write the encoded message into a text file

void writencode(size_t size,char words[size][40],next pointersToNext[size],int order,int numPacket,int length,int *temp){
	
	FILE *fptr;
    fptr = fopen("message.txt", "w"); 
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }
	/*for(int i = 0;i < size;i++){
		printf("library : %s and %d \n", words[i],size);
	}*/
	//char binaryarray[10][4] = {"1001","1111","0111","0011","0001","1010","1110","1100","1000","0101"};
	char* strindicator1 = (char*) malloc(2*sizeof(char));
	sprintf(strindicator1, "%d", numPacket);
	char* strindicator2 = (char*) malloc(4*sizeof(char));
	sprintf(strindicator2, "%d" , order);//we need to add the numPacket/order but they are numbers.
	char* result = (char*) malloc(strlen(strindicator1)+strlen(strindicator2)+2);
	result = concat(strindicator1,"/");
	result = concat(result,strindicator2);
	fprintf(fptr, "%s", result);//adding the numPacket and order at the beggining of the text file
	fprintf(fptr, "%s", " ");
	char* str1 = (char*)malloc(40*sizeof(char));
	memcpy(str1,"",strlen(""));
	for(int i = 0;i < length;i++){//i_th 4-bit number to encode
		int a = *(temp+i);//*temp
		if(strcmp(str1, "")==0){
			fprintf(fptr, "%s", words[a]);
			fprintf(fptr, "%s", " ");
			/*printf("%d with the trsing %s \n", a, words[a]);
			printf("1111 \n");*/
			memcpy(str1,words[a],strlen(words[a]));
		}else{
			int j = 0;
			while(j<size && strcmp(str1,words[j])!=0){//str1 is the last word we have printed in the text file
				j++;
			}
			
			//printf("this is j : %d \n", j);
			if(j == size){//if the last word was not in our library => simply print the words[binvalue[binaryarray[i]]]
				fprintf(fptr, "%s",words[a]);
				fprintf(fptr, "%s", " ");
				/*printf("%d with the string %s \n", a, words[a]);
				printf("22222 \n");*/
				free(str1);
				str1 = (char*)malloc(40*sizeof(char));
				memcpy(str1,words[a],strlen(words[a]));
				//printf("the string %s in %d \n", words[a],a);
				//str1 = words[a];
			}else{
				fprintf(fptr, "%s", (pointersToNext[j].nextWords)[a]);//we print the correspong pointersToNext
				fprintf(fptr, "%s", " ");
				//printf("%d with the string %s \n", a , (pointersToNext[j].nextWords)[a]);
				//printf("333333 \n");
				free(str1);
				str1 = (char*)malloc(40*sizeof(char));
				memcpy(str1,(pointersToNext[j].nextWords)[a],strlen((pointersToNext[j].nextWords)[a]));
				//str1 = (pointersToNext[j].nextWords)[a];
			}
		}
	}
	fprintf(fptr, "%s", result);//adding the numPacket and order at the end of the text file
	fprintf(fptr, "%s", " ");
	free(str1);
	free(strindicator1);
	free(strindicator2);
	free(result);
    fclose(fptr);
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
