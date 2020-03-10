
#include "hashTabLib.h"

int hashTabInit(hashTab *hTab, hashFunction hashFunc, eqChkFunction eqCheck, size_t tabSize) {
	hashNode0 **data = calloc(tabSize, sizeof(hashNode0*));
	if (!data)
		return 1;
	//hTab->hashFunc = hashFunc;
	//hTab->eqCheck = eqCheck;
	//hTab->tab = data;
	//hTab->tabSize = tabSize;
	*hTab = (hashTab){ hashFunc, eqCheck, data, tabSize, 0 };
	return 0;
};

//locates the first pointer to a hashNode0 which is either null or has the desired hash
static hashNode0  **hashTabFind0(hashTab *hTab, hash_t hash) {
	hashNode0 **tgt = hTab->tab + (hash % hTab->tabSize);
	while (*tgt && (*tgt)->hash != hash) {
		tgt = &((*tgt)->next);
	};
	return tgt;
};

//given a hashNode0, locates the first pointer to a hashNode1 which is either null or equal to given object
static hashNode1 **hashTabFind1(hashNode0 *node, eqChkFunction eqCheck, void *obj) {
	hashNode1 **tgt = &(node->sub);
	while (*tgt && !eqCheck((*tgt)->obj, obj)) {
		tgt = &((*tgt)->next);
	};
	return tgt;
};

//adds an object to an existing hash table
//returns:
//	0 if new object is added
//	1 if object was already present
//	2 on failure - no action
int hashTabAdd(hashTab *hTab, void *obj) {
	hash_t hash = hTab->hashFunc(obj);
	hashNode0 **tgt0 = hashTabFind0(hTab, hash);
	hashNode1 **tgt1;
	hashNode0 *node0;
	hashNode1 *node1;
	if (*tgt0) {
		tgt1 = hashTabFind1(*tgt0, hTab->eqCheck, obj);
		node0 = NULL;
	} else {
		node0 = malloc(sizeof(hashNode0));
		if (!node0)
			return 2; //failure!
		node0->hash = hash;
		node0->next = NULL;
		node0->sub = NULL;
		tgt1 = &(node0->sub);
	};
	if (*tgt1) {
		(*tgt1)->obj = obj; //replace object?
		return 1; //object already present;
	};
	node1 = malloc(sizeof(hashNode1));
	if (!node1) {
		free(node0);
		return 2; //failure!
	};
	if(!*tgt0)
		*tgt0 = node0;
	*tgt1 = node1;
	node1->obj = obj;
	node1->next = NULL;
	hTab->count++;
	return 0; //successfully added object;
};

//checks if the given object is in the hash table
//	0 if the object is absent
//	1 if the object is present
int hashTabCheck(hashTab *hTab, void *obj) {
	hash_t hash = hTab->hashFunc(obj);
	hashNode0 **tgt0 = hashTabFind0(hTab, hash);
	hashNode1 **tgt1;
	if (!*tgt0)
		return 0; //no object of that hash present
	tgt1 = hashTabFind1(*tgt0, hTab->eqCheck, obj);
	if (!*tgt1)
		return 0; //while another of the same hash is present, that specific object is not;
	return 1; //object present;
};

void hashTabClear(hashTab *hTab) {
	if (!hTab)
		return;
	hashNode0 **end = hTab->tab + hTab->tabSize;
	for (hashNode0 **temp = hTab->tab; temp != end; temp++)
		hashNode0Del(*temp);
	free(hTab->tab);
	hTab->tab = NULL;
	hTab->tabSize = 0;
	hTab->count = 0;
};

static void hashNode0Del(hashNode0 *node0) {
	hashNode0 *tgt;
	while (node0) {
		hashNode1Del(node0->sub);
		tgt = node0;
		node0 = node0->next;
		free(tgt);
	};
};

//may add a related feature to this which automatically frees all objects referenced by the table.

static void hashNode1Del(hashNode1 *node1) {
	hashNode1 *tgt;
	while (node1) {
		tgt = node1;
		node1 = node1->next;
		free(tgt);
	};
};

void hashTabIterInit(hashTabIterator *hTabIter, hashTab *hTab) {
	hTabIter->hTab = hTab;
	hTabIter->tabInd = 0;
	hTabIter->node0 = NULL;
	hTabIter->node1 = NULL;
};

void *hashTabIterNext(hashTabIterator *hTabIter) {
	if (!hTabIter->node1) {
		while (!hTabIter->node0) {
			if (hTabIter->tabInd == hTabIter->hTab->tabSize)
				return NULL;
			hTabIter->node0 = hTabIter->hTab->tab[(hTabIter->tabInd)++];
		};
		hTabIter->node1 = hTabIter->node0->sub; //note that the sub field of a node0 should never be NULL;
		hTabIter->node0 = hTabIter->node0->next;
	};
	void *out = hTabIter->node1->obj;
	hTabIter->node1 = hTabIter->node1->next;
	return out;
};

//transfers the contents of hashTab 'old' into hashTab 'new'
void hashTabTransfer(hashTab *new, hashTab *old) {
	new->hashFunc = old->hashFunc;
	new->eqCheck = old->eqCheck;
	new->count = old->count;
	hashNode0 **tabEnd = old->tab + old->tabSize;
	for (hashNode0 **temp = old->tab; temp != tabEnd; temp++) {
		hashNode0 *node0 = *temp;
		*temp = NULL;
		while (node0) {
			hashNode0 *node0t = node0->next;
			hashNode0 **loc = new->tab + (node0->hash % new->tabSize);
			node0->next = *loc;
			*loc = node0;
			node0 = node0t;
		};
	};
};

int hashTabResize(hashTab *hTab, size_t newSize) {
	hashTab orig = *hTab;
	if (hashTabInit(hTab, NULL, NULL, newSize))
		return 1;
	hashTabTransfer(hTab, &orig);
	free(orig.tab);
	return 0;
};

int hashTabEqCheck(hashTab *hTab0, hashTab *hTab1) {
	if (hTab0->count != hTab1->count)
		return 0;
	hashTabIterator hTabIter;
	hashTabIterInit(&hTabIter, hTab0);
	while (1) {
		void *obj = hashTabIterNext(&hTabIter);
		if (!obj)
			return 1;
		if (!hashTabCheck(hTab1, obj))
			return 0;
	};
};