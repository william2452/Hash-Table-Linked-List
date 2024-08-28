#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hash.h"

/*
	assertTrue: testing function
	param1: predicate - result of the test function
	param2: message - the output message
	pre: none
	post: none
 */
void assertTrue(int predicate, char *message) {
	printf("%s: ", message);
	if (predicate)
		printf("Pass \n");
	else
		printf("Fail \n");
}

/*
	getWord: function takes a FILE pointer and returns a string which is the next word in the file
	words are defined to be characters, apostrophes or numbers separated by periods, spaces or newlines
	param1: file - the file to parse
	pre: none
	post: no changes to the file
	return: current word of file is returned. NULL is returned at the end of the file
 */
char* getWord(FILE *file) {
	int length = 0;
	int maxLength = 16;
	char character;

	char *word = malloc(sizeof (char) *maxLength);
	assert(word != NULL);

	while((character = fgetc(file)) != EOF) {
		if((length+1) > maxLength) {
			maxLength *= 2;
			word = (char*)realloc(word, maxLength);
		}
		if((character >= '0' && character <= '9') || (character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z') || character == 39) {
			word[length] = character;
			length++;
		} else if(length > 0) {
			break;
		}
	}

	if(length == 0)	{
		free(word);
		return NULL;
	}
	word[length] = '\0';
	return word;
}


int main () {
	char *fn; /* File name */
	struct hashTable *hash1, *hash2;
	FILE *filePtr;

	fn = "text1.txt";/* the file name and path */
	printf("Opening file: %s \n", fn);
	filePtr = fopen(fn, "r");
	hash1 = createMap(40, 1);

    char *word;
    while((word = getWord(filePtr))) {
        if(*word == '\0') {
            break;
        }
        insertMap(hash1, word, 1);
    }

    printf("--------------- Testing hashFunction1 --------------- \n");

    assertTrue((hashFunction1("abc") == 294), "Hash 'abc' == 294");

    printf("--------------- Testing contains --------------- \n");

    assertTrue(containsKey(hash1, "it") == 1, "Search for 'it'");
    assertTrue(containsKey(hash1, "comparison") == 1, "Search for 'comparison'");
    assertTrue(containsKey(hash1, "period") == 1, "Search for 'period'");
    assertTrue(containsKey(hash1, "despair") == 1, "Search for 'despair'");
    assertTrue(containsKey(hash1, "deriop") == 0, "Search for 'deriop'");
    assertTrue(containsKey(hash1, "yuck") == 0, "Search for 'yuck'");

    printf("\n--------------- Testing table stats --------------- \n");

    assertTrue(hash1->tableSize == 40, "Test table size (40)");
    assertTrue(getFullBuckets(hash1) == 30, "Test full buckets (30)");
    assertTrue(getEmptyBuckets(hash1) == 10, "Test empty buckets (10)");
    assertTrue(getLinkCount(hash1) == 59, "Test link count (59)");

    printf("--------------- Testing remove --------------- \n");

    assertTrue(containsKey(hash1, "despair") == 1, "Search for 'despair'");
    removeKey(hash1, "despair");
    assertTrue(containsKey(hash1, "despair") == 0, "Search for 'despair' after remove");
    assertTrue(containsKey(hash1, "yuck") == 0, "Search for 'yuck'");
    removeKey(hash1, "yuck"); /* Should print some type of 'not found' message */

    printf("--------------- Printing hash table --------------- \n");
	printMap(hash1);

    printf("Delete Hash Table 1\n");
    deleteMap(hash1);

    printf("Closing file\n");
    fclose(filePtr);

    printf("--------------- Testing Round 2 - Same text file - new hash --------------- \n");

    fn = "text1.txt";/* the file name and path*/
	printf("Opening file: %s \n", fn);
	filePtr = fopen(fn, "r");
	hash2 = createMap(40, 2);

    while((word = getWord(filePtr))) {
        if(*word == '\0') {
            break;
        }
        insertMap(hash2, word, 1);
    }

    printf("--------------- Testing table stats 2 --------------- \n");

    assertTrue(hash2->tableSize == 80, "Test table size (80)");
    assertTrue(getFullBuckets(hash2) == 38, "Test full buckets (38)");
    assertTrue(getEmptyBuckets(hash2) == 42, "Test empty buckets (42)");
    assertTrue(getLinkCount(hash2) == 59, "Test link count (59)");


    printf("--------------- Testing remove --------------- \n");

    assertTrue(containsKey(hash2, "yuck") == 0, "Search for 'yuck'");
    removeKey(hash2, "yuck"); /*Should print some type of 'not found' message*/
    assertTrue(containsKey(hash2, "despair") == 1, "Search for 'despair'");
    removeKey(hash2, "despair");
    assertTrue(containsKey(hash2, "despair") == 0, "Search for 'despair' after remove");
    assertTrue(containsKey(hash2, "belief") == 1, "Search for 'belief'");
    removeKey(hash2, "belief");
    assertTrue(containsKey(hash2, "belief") == 0, "Search for 'belief' after remove");
    assertTrue(containsKey(hash2, "degree") == 1, "Search for 'degree'");
    removeKey(hash2, "degree");
    assertTrue(containsKey(hash2, "degree") == 0, "Search for 'degree' after remove");

    printf("Delete Hash Table 2\n");
    deleteMap(hash2);

    printf("Closing file\n");
    fclose(filePtr);

    /*UNCOMMENT AFTER YOU GET THE MAP WORKING*/
    

    printf("--------------- Testing table stats 2 --------------- \n");

	struct hashTable *concord;

    fn = "text2.txt";
	printf("Opening file: %s \n", fn);
	filePtr = fopen(fn, "r");
	concord = createMap(10, 2);


	//instead of calling insert, we call concordance
    while((word = getWord(filePtr)) != '\0') {
        concordance(concord, word);
    }

    printf("--------------- Concordance table stats --------------- \n");

    printf("table size: %d \n", concord->tableSize);
    printf("full buckets: %d \n", getFullBuckets(concord));
    printf("empty buckets: %d \n", getEmptyBuckets(concord));
    printf("link count: %d \n", getLinkCount(concord));

    



	return 0;
}
