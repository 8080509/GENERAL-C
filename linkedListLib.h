#pragma once

#include "commonHeader.h"
#include <stdlib.h>

typedef struct _listTemplate {
	handler constructor; //callback for when the linked list creates a reference to an object
	handler destructor; //callback for when the linked list deletes its reference to an object
} listTemplate;

typedef struct _listHead {
	struct _listTemplate *template; //pointer to the template so the constructor and destructor can be reached - no const/dest if NULL
	struct _listNode *start; //pointer to the first node of the array
	struct _listNode **end; //pointer to the last node of the array
} listHead;

typedef struct _listNode {
	struct _listNode *next;
	void *obj;
} listNode;

int listPrepend(listHead *list, void *obj);

int listAppend(listHead *list, void *obj);

void *listPop(listHead *list);

listHead *newList(listTemplate *template);

void initListTemplate(listTemplate *template, handler constructor, handler destructor);

//a simpler list type, not featuring constructors & destructors, only containing a pointer to an item in the list
//NOTE:  if iterating over list items, note that the list format differs slightly from expectation
typedef struct _listNode *simpleList;

int sListPrepend(simpleList *list, void *obj);

int sListAppend(simpleList *list, void *obj);

void *sListPop(simpleList *list);

simpleList *sListCopy(simpleList *list);

void sListDel(simpleList *list);