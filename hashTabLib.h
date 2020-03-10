#pragma once

#include <stdlib.h>
#include "commonHeader.h"

typedef unsigned int hash_t;

typedef hash_t (*hashFunction)(void *);

typedef int(*eqChkFunction)(void *, void *);

//hash table root
//hashFunc - pointer to the hash function in use
//eqCheck - pointer to the equality check function
//tab - pointer to the actual table of hashNode0 structs
//tabSize - the size of the table
typedef struct _hashTab {
	hashFunction hashFunc;
	eqChkFunction eqCheck;
	struct _hashNode0 **tab;
	size_t tabSize;
	size_t count;
} hashTab;

//hash table subnode 0
//next - next subnode 0 with same hash % tabSize
//sub - pointer to a child sequence of subnode 1's
//hash - full value of the hash associated with this node
typedef struct _hashNode0 {
	struct _hashNode0 *next;
	struct _hashNode1 *sub;
	hash_t hash;
} hashNode0;

//hash table subnode 1
//next - next subnode 1 with same hash
//obj - pointer to the object associated with this node
typedef struct _hashNode1 {
	struct _hashNode1 *next;
	void *obj;
} hashNode1;

typedef struct _hashTabIterator {
	struct _hashTab *hTab;
	size_t tabInd;
	struct _hashNode0 *node0;
	struct _hashNode1 *node1;
} hashTabIterator;

int hashTabInit(hashTab *hTab, hashFunction hashFunc, eqChkFunction eqCheck, size_t tabSize);

int hashTabAdd(hashTab *hTab, void *obj);

int hashTabCheck(hashTab *hTab, void *obj);

void hashTabClear(hashTab *hTab);