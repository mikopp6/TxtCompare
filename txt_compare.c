/*  txt_compare
	This program compares two text files.
	All the words of the first file are read and
	saved to a global 2d char-array hash-table.
	If reading and saving the first file is
	succesful, the program then starts reading
	a second file, and prints the 50 unique words
	in that file not found in the first file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WORD_SIZE 101
#define HASHTABLE_SIZE 49999

char hashTable[HASHTABLE_SIZE][WORD_SIZE];

int readFile(int fileNum);
int addWord(char *wordBuffer);
unsigned long hashAlgorithm(char *wordBuffer);

/*  Function main:
	The function calls the readFile function
	first with fileNum 1, and fileNum 2. After
	that the program exits. Nothing else is
	done on the main function.
*/
int main(){
	
	int fileNum;
	printf("Start of compare.\n");
	
	for(fileNum = 1; fileNum < 3; fileNum++){
		if(readFile(fileNum) == 0){
			printf("Error opening file %d", fileNum);
			return -1;
		}
	}
	return 0;
}


/*  Function readFile:
	The function receives the current filenumber that
	is being read. Return value 1 means that reading
	the file was succesful, and value 0 means that
	something went wrong during the file reading process.
	First the function asks the use for a filename, and
	tries to open that file. Every character from the file is
	read, and if the character is a letter or a apostrophe, 
	the Word value is changed to 1. The function continues
	to read characters to the current word, until the word
	ends, and adds that word to the table using the addWord
	function. Then it starts looking for a new word. This loop
	runs until the end of the file has been reached, or until
	file 2 unique count reaches 50.
*/
int readFile(int fileNum){
	
	char filename[WORD_SIZE];
	char wordBuffer[WORD_SIZE];
	char charBuffer;
	FILE *fp;
	int word;
	int i = 0;
	int unique = 0;
	int totalWords = 0;
	
	
	printf("Input file %d filename:", fileNum);
	fgets(filename, WORD_SIZE, stdin);
	if(filename[strlen(filename) -1] == '\n'){
        filename[strlen(filename) -1] = '\0';
    }else{
        while(fgetc(stdin) != '\n');
    }
	strcat(filename, ".txt");
	fp = fopen(filename, "r");
	if(fp == NULL){
		return 0;
	}
	printf("Success opening file!\n");
	if(fileNum == 2){
		printf("The 50 first unique words in file 2 not found in file 1:\n");
	}
	
	while((charBuffer = fgetc(fp)) != EOF){
		if(isalpha(charBuffer) || charBuffer == 39){
            wordBuffer[i] = tolower(charBuffer);
            word = 1;
            i++;
			if(i==WORD_SIZE){
				printf("%s\n",wordBuffer);
			}
        }else if(word == 1){
            wordBuffer[i] = '\0';
            switch(addWord(wordBuffer)){
			case 1:
				unique++;
				if(fileNum == 2){
					printf("%d. %s\n", unique, wordBuffer);
				}
				break;
			case 2:
				break;
			case 0:
				return 0;
			}
			totalWords++;
            word = 0;
            i = 0;
        }else{
            word = 0;
        }
		if(unique == 50 && fileNum == 2){
			fclose(fp);
			return 1;
		}
    }
	fclose(fp);
	if(fileNum == 1){
		printf("Total number of words in file %s: %d\n", filename, totalWords);
		printf("Number of unique words: %d\n", unique);
		return 1;
	}
	return 1;
}


/*  Function addWord:
	The function receives a pointer to the word
	that we are trying to add to the hash-table.
	Return value 1 means that the added word was
	unique, value 2 means that the word was already
	in the table and value 0 means that the table
	is full.
	If a collision happens, we use linear probing by
	adding 1 to the hash-value until a free spot is
	found. At every collision the value of 'i' is 
	increased. If 'i' reaches the table size, 
	the table has run out of room and the program stops.

*/
int addWord(char *wordBuffer){
	
	unsigned long hash = hashAlgorithm(wordBuffer);
	int i = 0;
	
	do{
		if(strlen(hashTable[hash]) == 0){
			strcpy(hashTable[hash], wordBuffer);
			return 1;
		}else if(strcmp(hashTable[hash], wordBuffer) == 0){
			return 2;
		}else{
			hash++;
			i++;
			if(hash == HASHTABLE_SIZE){
				hash = 0;
			}	
		}
		if(i == HASHTABLE_SIZE){
			printf("Table full\n");
			return 0;
		}
	}while(1);
}


/*  Function hashAlgorithm:
    The function receives a pointer to a word,
	and returns the words hash-value as an unsigned long.
	The algorithm itself is a very simple multiplying
	hash algorithm. It takes a character one by one,
	multiplies the placement number of the character
	with the prime 31, and adds the Ascii-value of the
	character. Finally we take modulo based on the
	table size.
*/
unsigned long hashAlgorithm(char *wordBuffer){
	
	unsigned long hash;
	int prime = 31;

    for (hash = 0; *wordBuffer != '\0'; wordBuffer++){
        hash = *wordBuffer + prime*hash;
	}
	hash = hash%HASHTABLE_SIZE;
    return hash;
}
