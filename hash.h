#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define KeyType char *
#define ValueType int
#define TYPE double
#define LOAD_FACTOR_THRESHOLD .75

struct hashLink {
   KeyType key; /* The key is what you use to look up a hashLink */
   ValueType value; /* The value stored with the hashLink, an int in our case */
   struct hashLink * next; /* Notice how these are like linked list nodes */
};

struct hashTable {
    struct hashLink **table; /* Array of pointers to hashLinks */
    int tableSize; /* Number of buckets in the table */
    int count; /* Number of hash links */
    int hashID; /* ID of the hashing algorithm */
};

/* Hash functions */
int hashFunction1(char *str);
int hashFunction2(char *str);
int _hashValue(char *key, int hashNum);

/* Hash Map Functions */
void _initMap (struct hashTable * h, int tableSize, int ID);
struct hashTable *createMap (int tableSize, int ID);
void _reSizeTable(struct hashTable *h, int newCap);
void insertMap (struct hashTable * h, KeyType k, ValueType v);
int containsKey (struct hashTable * h, KeyType k);
void removeKey (struct hashTable * h, KeyType k);
ValueType valAtKey(struct hashTable *h, KeyType k);
int getFullBuckets(struct hashTable *h);
int getEmptyBuckets(struct hashTable *h);
int getLinkCount(struct hashTable *h);
int getMapSize(struct hashTable *h);
void printMap(struct hashTable * h);
float tableLoad(struct hashTable *h);
void concordance(struct hashTable *h, char *word);
void _freeMap (struct hashTable * h);
void deleteMap (struct hashTable * h);

void assertTrue(int predicate, char *message);
char* getWord(FILE *file);
