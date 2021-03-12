#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jansson.h>
#include <stdbool.h>

#include "decode.h"



//in this code we have to decode a message by having a json file 
//which contains the keys and values we need to decode
//first we read the library
//then we read from a text message in a text file
//what we write in another text file is an array of 4-bit binary numbers
//!! it would be safer to have just one text file open at a time because it is platform dependant

/*int main(int argc, char * argv[]){
		
	json_t *json;
    json_error_t error;
	size_t size = 2000;
	char words[size][40];
	next pointersToNext[size];
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
	
	//now we have all the information in the two arrays words and pointersToNext, we encode 4-bit binary numbers
	//suppose we have the number of these numbers
    
	
	return 0;
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
char* decodetxt(size_t size,char words[size][40],next pointersToNext[size]){
	
	FILE * fp = fopen("TweetDownload.txt", "r"); 
	//if (fp == NULL) return 1; 
	char c; 
	int count = 0,countext = 0;//countext is the the number of words in the file
	//count is the number of chars in each word that we count
	char *str1;
	str1 = (char*)malloc(40*sizeof(char));
	char message[2000][40];
	while((c = fgetc(fp)) != EOF  && countext < 2000) //getting characters
	{ 
		if(c == ' ' || c == '\n') //this is the end of the word
		{ 
			//printf("\n");
			memcpy(message[countext],str1,strlen(str1));
			free(str1);
			str1 = (char*)malloc(40*sizeof(char));
			countext++;
			count = 0;
		} 
		else 
		{ 
			//printf("%c",c);
			strncat(str1, &c, 1);
			count++;//to have the length of the word
		} 
	} 
	fclose(fp); 
	//now we have all the words in the array message and we need to use the library to decode it into an array
	//of 4-bits, we do the inverse of encode
	
	/*for(int i= 0;i < countext;i++){
		printf("%s \n", message[i]);
	}
	printf("\n \n \n \n");
	for(int i = 0;i < size;i++){
		printf("%s \n", words[i]);
	}*/
	int numPacket = -1;
	int order;
	int counter = 0;
	bool start = true;//if we are at the beggining of a message this variable is true.
	bool decodeEnable = false;//this variable is true if we are in the right packet and we decode this packet.
	char* decodedPacket = (char*)malloc(sizeof("a"));//this is what we return
	memcpy(decodedPacket,"a",sizeof("a"));
	char* lastword = (char*)malloc(40*sizeof(char));
	int arrayofints[countext];
	int decodeLength = 0;
	for(int i = 0;i < countext;i++){//i_th 4-bit number to encode
		if(strchr(message[i],'/') == NULL){
			if(decodeEnable){
				if(start){
					//we search in the array words
					int j = 0;
					while(j < size && strcmp(message[0],words[j])!=0){
						j++;
					}
					memcpy(lastword,words[j],strlen(words[j]));
					//now we know that this j is actually the value of the binary number
					arrayofints[counter] = j;
					decodeLength++;
					//printf("the real one %d \n" , j);
					start = false;
				}else{
					//here we have two cases : 1.the last word is actually in the list
					//the last word was not on the list
					//in the first case we look at its next words array
					//in the second case we look at the array words
					
					int j = 0;
					while(j < size && strcmp(lastword,words[j])!=0){//size is the size of the library
						j++;
					}
					
					if(j == size){
						//it means that we have not found the last word in the words and hence :
						int k = 0;
						while(k < size && strcmp(message[i],words[k])!=0){//size is the size of the library
							k++;
						}
						free(lastword);
						lastword = (char*)malloc(40*sizeof(char));
						memcpy(lastword,words[k],strlen(words[k]));
						arrayofints[counter] = k;
						decodeLength++;
						//printf("real one : %d \n" , k);
					}else{
						//printf("lastword is %s message[i] is %s words[j] is %s\n", lastword, message[i],words[j]);
						//we need to look the list of its nextwords
						int k = 0;
						while(k < 64 && strcmp(message[i],(pointersToNext[j].nextWords)[k])!=0){
							k++;
						}
						free(lastword);
						lastword = (char*)malloc(40*sizeof(char));
						memcpy(lastword,message[i],strlen(message[i]));
						arrayofints[counter] = k;
						decodeLength++;
						//printf("the message : %s  nextwords : %s  real one %d \n" ,message[i] ,(pointersToNext[j].nextWords)[k],k);
					}
				}
				
				char str[12];
				sprintf(str, "%d", arrayofints[counter]);
				decodedPacket = (char*) realloc(decodedPacket,strlen(decodedPacket)+13);
				strcat(decodedPacket,str);
				counter++;
			}
		}else{
			if(numPacket == -1){
				char* slash = strchr(message[i],'/');// slash points to the slash position. numPacket/order
				*(slash) = '\0';
				slash = slash+1;
				order = atoi(slash);
				numPacket = atoi(message[i]);
			}else{
				char* slash = strchr(message[i],'/');// slash points to the slash position. numPacket/order
				*(slash) = '\0';
				slash = slash+1;
				int ordernew = atoi(slash);
				if(ordernew > order+1)
					printf("we have lost a part of the encoded message over twitter");
				order = ordernew;
				if(numPacket != atoi(message[i])){
					//this means that we have started to read the message associated with a different packet
					//so the process of reading should continue until reaching the good packet number
					decodeEnable = false;
				}else{
					decodeEnable = true;
				}
			}
			
			start = true;
		}
	}

	/*for(int i = 0;i < countext;i++){
		
		printf("%d \n",arrayofints[i]);
	}*/
	
	FILE *fptr;
    fptr = fopen("decoded.txt", "w"); 
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }
	printf("\n");
	
	for(int i = 0; i < decodeLength; i++){
		/*int value = arrayoffourbits[i];
		int devide = 8;
		int i = 0;*/
		char str[12];
		sprintf(str, "%d", arrayofints[i]);
		/*while(devide!=0){
			if(value/devide == 1){
				str[i] = '1';
			}else{
				str[i] = '0';
			}
			value = value%devide;
			i++;
			devide = devide/2;
		}*/	
		fprintf(fptr, "%s", str);
		fprintf(fptr, "%s", " ");
		printf("  %s   ", str);
		//printf("\n");
	}
	printf("\n");
	printf("%s \n",decodedPacket+1);
	printf("%s \n",decodedPacket);
	return (decodedPacket+1);
}