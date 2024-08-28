#include "hash.h"

/*
	hashFunction1: The first hash function you will use - the sum of the ASCII values is computed
	param1: str - the string to hash
	pre: str is not NULL
	return: the sum of the ASCII values of the string is returned
	post: none
	HINT - you can treat chars like ints
	HINT - return the absolute value - negative hashes don't work
*/
int hashFunction1(char *str) {
	/* FIX ME */
    assert(str != NULL);

    int sum = 0;
    int i;
    for(i = 0; i < strlen(str); i++) {
        sum += str[i];
    }
	return abs(sum);
}


/*
	hashFunction2: The second hash function you will use - not easy to explain how it is hashed
	param1: str - the string to hash
	pre: str is not NULL
	return: the hash is returned
	post: none
    HINT - you can treat chars like ints
    HINT - return the absolute value - negative hashes don't work
*/
int hashFunction2(char *str) {
	assert(str != NULL);
	int i;
	int h = 0;
	for (i = 0; str[i] != '\0'; i++) {
		h += (i+1) * str[i];
	}
	return abs(h);
}

/*
	hashValue: returns the hash of the val - hash algorithm is specified
	param1: val - the string to hash
	param2: hashNum - the number of the hashing function (1 or 2 in this program)
	pre: val is not NULL
	return: the hashed value is returned
	post: none
*/
int _hashValue(char *val, int hashNum) {
	if(hashNum == 1) {
		return hashFunction1(val);
	} else {
		return hashFunction2(val);
	}
}

/*
	initMap: Initialize the hash table
	param1: h - the hash map to initialize
	param2: tableSize - the size of the hash map
	pre: h is not NULL
	pre: tableSize is greater than 0
	post: tableSize has been set to tableSize
	post: count is set to 0
	post: hashID is set to ID
	post: memory for array has been allocated
	post: all hash links (buckets) are initialized to NULL
*/
void _initMap(struct hashTable *h, int tableSize, int ID) {
	h->tableSize = tableSize;
	h->count = 0;
	h->hashID = ID;
	h->table = malloc(sizeof(struct hashLink*)*tableSize);

	int i = 0;
	while(i < tableSize) {
        h->table[i] = NULL;
        i++;
	}
}

/*
	createMap: allocate memory and initialize a hash table
	param1: tableSize - the capacity of the table
	pre: tableSize > 0
	return: newly created hash map of size, tableSize
	post: memory for the hash map has been created
	post: hash table of size tableSize has been initialized
*/
struct hashTable *createMap(int tableSize, int ID) {
	assert(tableSize > 0);

	struct hashTable *h;
	h = malloc(sizeof(struct hashTable));
	assert(h != 0); /* ensure that memory was successfully allocated */

	_initMap(h, tableSize, ID);
	return h;
}


 /*
	insertMap: insert the key/value into a hash map - no duplicate keys are permitted
	param1: h - the hash map
	param2: k - the key to insert - in this case, the word
	param3: v - the value to insert - in this case, the number of occurrences - it is just a "1" in pt 1
	pre: h is not NULL
	post: map contains the key/value
	post: count has been incremented
    HINT: - You are not going to pass tests for a while
    HINT - If the bucket is not empty, then add to the hashLink to the back of the linked list
    HINT - Use strcmp to compare c-strings
 */
void insertMap(struct hashTable *h, KeyType k, ValueType v) {
	/* hash the key */
	int hash = _hashValue(k, h->hashID);
	hash = hash % h->tableSize;

    /* debugging information */
	printf("KEY: %s HASH: %d val:%d \n", k, hash, v);

    	/*
	Insert the key value pair
	You need to allocate memory for a new link if the key is new

	Duplicate values - if a duplicate key is found, leave the old key and insert the new value in the link - don't increment count
	You can't have two hash links for the word 'bobcat', but you can have many links in the same bucket
	If there is a collision, then traverse the linked list of hash links
	*/

	/* FIX ME */

    if(h -> table[hash] == NULL) { /*bucket is empty*/
        struct hashLink *newLink = malloc(sizeof(struct hashLink));
        newLink -> value = v;
        newLink -> key = k;
        newLink -> next = NULL;
        h -> table[hash] = newLink;
        h -> count++;
    } else { /*bucket is not empty*/
        struct hashLink *curr = h -> table[hash];
        while(curr -> next != NULL && strcmp(curr -> key, k) != 0) {
            curr = curr -> next;
        }
        if(strcmp(curr -> key, k) == 0) { /*duplicate key*/
            curr -> value = v;
        } else { /*non-duplicate key*/
            struct hashLink *newLink = malloc(sizeof(struct hashLink));
            newLink -> value = v;
            newLink -> key = k;
            newLink -> next = NULL;
            curr -> next = newLink;
            h -> count++;
        }
    }

    /* Resize table if necessary */
	if(tableLoad(h) > LOAD_FACTOR_THRESHOLD) {
        _reSizeTable(h, h->tableSize*2);
	}
}


/*
	_reSizeTable: Resizes the hash map to the size, newCap. Rehashes all of the current keys.
	param1: hashTable - the map
	param2: newCap - the new capacity
	pre: h is not NULL
	pre: newCap is > 0
	post: memory for a new hash table has been created
	post: new hash table has cap newCap
    post: all keys have been re-hashed and inserted into new hash table
	post: all the old hash links have been freed
	HINT - use insertMap to re-hash the keys from old to new
*/
void _reSizeTable(struct hashTable *h, int newCap) {
	assert(h != NULL);
	assert(newCap > 0);

	printf("***************************\n");
	printf("RESIZE\n");
	printf("***************************\n");

	struct hashLink **temp = h->table; /* pointer to the old table */
	int tempSize = h->tableSize; /* size of the old table */
	struct hashLink *curr, *del; /* Used to free the old hash links and iterate through them */

	h->table = malloc(sizeof(struct hashLink*)*newCap); /* new larger table */
	_initMap(h, newCap, h->hashID);

	/* re-hash links in new table*/
	/* FIX ME */
    int i;
    for(i = 0; i < tempSize; i++) {
        curr = temp[i];
        while(curr != NULL) {
            del = curr;
            insertMap(h, curr -> key, curr -> value);
            curr = curr -> next;
            free(del);
            del = 0;
        }
    }
}


/*
	containsKey: determines whether the key is contained in the map
	param1: h - the map
	param2: k - the key to search for
	pre: h is not null
	post: none
	return: return 0 is not found, otherwise return 1
	HINT: Hash the key and check to see if it is in the table
	HINT: This is where you might start passing some tests
	HINT - Use strcmp to compare c-strings
*/
int containsKey(struct hashTable *h, KeyType k) {
	/* FIX ME */
    assert(h != NULL);

    int hash = _hashValue(k, h->hashID);
	hash = hash % h->tableSize;

    struct hashLink *curr = h -> table[hash];
    while(curr != NULL) {
        if(strcmp(curr -> key, k) == 0) {
            return 1;
        } else {
            curr = curr -> next;
        }
    }
    return 0;
}

/*
	removeKey: find the hash link for the key and remove it
	param1: h - the map
	param2: k - the key to remove
	pre: h is not NULL
	post: key has been removed from the table
	post: if unable to find the key, then print an appropriate message
	post: count has been decremented
	post: memory for the hash link has been freed
	HINT: Check if the key is the first in the chain - this is a special case
	HINT - Use strcmp to compare c-strings
 */
void removeKey(struct hashTable *h, KeyType k) {
    /* FIX ME */
    assert(h != NULL);

	if(containsKey(h, k) == 0) {
        printf("Key Not Found\n");
	} else {
	    int hash = _hashValue(k, h->hashID);
        hash = hash % h->tableSize;

        struct hashLink *curr = h -> table[hash];
        struct hashLink *prev = curr;
        struct hashLink *temp = curr;

        if(strcmp(curr -> key, k) == 0) {
            if(curr -> next != NULL) { /*first thing in list, but not only*/
                temp = curr -> next;
                h -> table[hash] = temp;
                free(curr);
                curr = 0;
            } else { /*first and only thing*/
                h -> table[hash] = NULL;
                free(curr);
                curr = 0;
            }
        } else {
            while(strcmp(curr -> key, k) != 0) {
                prev = curr;
                curr = curr -> next;
            }
            if(curr -> next != NULL) { /*not first and not last*/
                temp = curr -> next;
                prev -> next = temp;
                free(curr);
                curr = 0;
            } else { /*not first, last*/
                prev -> next = NULL;
                free(curr);
                curr = 0;
            }
        }
        h -> count--;
	}
}

/*
	valAtKey: return the value stored at the key
	param1: h - the map
	param2: k - the key to search for
	return: return the value found at the key - return 0 if not found
	pre: h is not null
	pre: k is in the hash table
	post: none
	HINT - Use strcmp to compare c-strings
*/
ValueType valAtKey(struct hashTable *h, KeyType k) {
	/* FIX ME */
    assert(h != NULL);
    assert(containsKey(h, k) == 1);


    if(containsKey(h, k) == 1) {
        int hash = _hashValue(k, h->hashID);
        hash = hash % h->tableSize;
        struct hashLink *curr = h -> table[hash];
        return curr -> value;
    } else {
        printf("Key Not Found\n");
        return 0;
    }
}


/*
	getEmptyBuckets: returns the number of empty buckets in the table, these are buckets which have no hashlinks in them
	param1: h - the hash table
	pre: h is not null
	post: no changes to the table
	return: the number of empty buckets in the table
	HINT - you need to check the hash map for NULLs
*/
int getEmptyBuckets(struct hashTable *h) {
	/* FIX ME */
    assert(h != NULL);

	int emptyBuckets = 0;
    int i;
    for(i = 0; i < h -> tableSize; i++) {
        if(h -> table[i] == NULL) {
            emptyBuckets++;
        } else {
            continue;
        }
    }
	return emptyBuckets;
}


/*
	getFullBuckets: returns the number of hashLinks in the map
	param1: h - the map
	pre: h is not NULL
	return: the bucketCount of the hash table
	post: none
	HINT: this is the number of non-empty buckets in the table
*/
int getFullBuckets(struct hashTable *h) {
	/* FIX ME */
	assert(h != NULL);

	int fullBuckets = 0;
    int i;
    for(i = 0; i < h -> tableSize; i++) {
        if(h -> table[i] != NULL) {
            fullBuckets++;
        } else {
            continue;
        }
    }
	return fullBuckets;
}

/*
	getLinkCount: returns the number of hashLinks in the map
	param1: h - the map
	pre: h is not NULL
    return: the number of hash links in the table
	post: none
*/
int getLinkCount(struct hashTable *h) {
	/* FIX ME */
    assert(h != NULL);

    struct hashLink *curr = malloc(sizeof(struct hashLink));
    int numLinks = 0;
    int i;
    for(i = 0; i < h -> tableSize; i++) {
        if(h -> table[i] != NULL) {
            curr = h -> table[i];
            while(curr != NULL) {
                numLinks++;
                curr = curr -> next;
            }
        }
    }
	return numLinks;
}


/*
	getMapSize: returns the size of the map
	param1: h - the hash map
	pre: - h is not null
	return: return the tableSize
	post: no changes to the map
*/
int getMapSize(struct hashTable *h) {
	/* FIX ME */
	assert(h != NULL);

	return h -> tableSize;
}


/*
	tableLoad: determine the load of the table
	there are a couple ways you can calculate this - we want full buckets / total buckets
	we are not concerned with the number of hash links - we are assuming a reasonable hash
	param1: h - the hash table
	pre: h is not null
	post: no changes to the table
	return: returns the ratio of: full buckets / table size
	HINT - use getFullBuckets() and getMapSize()
	HINT - cast one of the operands to a float, otherwise you have integer division
*/
float tableLoad(struct hashTable *h) {
	/* FIX ME */
	assert(h != NULL);

    float tableLoad = ((float)getFullBuckets(h)) / getMapSize(h);

	return tableLoad;
}


/*
	printMap: prints the contents of the hash map
	param1: h - the map
	pre: h is not NULL
	return: contents of the hash map are displayed via standard output - see format below
	post: map has been printed to standard output
	bucket x: key (value), key (value, ....
	Ex. Bucket 1: cat (1), dog (1), the (5)...
*/
void printMap(struct hashTable *h) {
	/* FIX ME */
    assert(h != NULL);

    struct hashLink *curr;
    int i;
    for(i = 0; i < h -> tableSize; i++) {
        printf("Bucket %i: ", i);
        curr = h -> table[i];
        while(curr != NULL) {
            printf("%s (%i), ", curr -> key, curr -> value);
            curr = curr -> next;
        }
        printf("\n");
    }
}


/*
	freeMap: deallocate buckets and the hash map
	param1: h - the hash map
	pre: - h is not null
	post: memory used by the hash links has been freed - use _freeMap
	post: memory used by the hash table has been freed
*/
void deleteMap(struct hashTable *h) {
	assert(h != NULL);

	_freeMap(h);/* free all memory used by the buckets */
	free(h->table);/* free the array of hash link pointers */
}


/*
	freeMap: free all memory used by the buckets
	param1: h - the hash map
	pre: - h is not null
	post: memory used by the hash buckets has been freed
*/
void _freeMap (struct hashTable *h) {
	/* FIX ME */
    assert(h != NULL);

    int i;
    for(i = 0; i < h -> tableSize; i++) {
        free(h -> table[i]);
        h -> table[i] = 0;
    }
}


/*
    concordance: create a hash table which contains word frequencies
    param1: h - the hash table
    param2: word - the current word to hash
    pre: h is not NULL
    post: key and value is now updated
    if the key already exists, add 1 to the value
    HINT - this is almost a normal insert, the difference is that the value is not always 1
    HINT: If key exists, then increment the value
    HINT - use valAtKey() and insertMap()
*/
void concordance(struct hashTable *h, char *word) {
    /* FIX ME */
    assert(h != NULL);

    int val = 0;
    if(containsKey(h, word) == 1) {
      val = valAtKey(h, word);
    }
    insertMap(h, word, val++);
}
