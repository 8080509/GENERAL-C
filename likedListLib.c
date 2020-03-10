
#include "linkedListLib.h"

static void callConstr(listHead *list, void *obj) {
	if (list->template)
		list->template->constructor(obj);
};

static void callDestr(listHead *list, void *obj) {
	if (list->template)
		list->template->destructor(obj);
};

static listNode *newListNode() {
	return malloc(sizeof(listNode));
};

static void delListNode(listNode *node) {
	free(node);
};

int listPrepend(listHead *list, void *obj) {
	listNode *node = newListNode();
	if (!node)
		return 1;
	if (!list->start)
		list->end = &(node->next);
	node->next = list->start;
	list->start = node;
	node->obj = obj;
	return 0;
};

int listAppend(listHead *list, void *obj) {
	listNode *node = newListNode();
	if (!node)
		return 1;
	node->next = NULL;
	*list->end = node;
	list->end = &(node->next);
	node->obj = obj;
	return 0;
};

//reverses the action of listPrepend;
void *listPop(listHead *list) {
	listNode *node = list->start;
	list->start = list->start->next;
	void *obj = node->obj;
	delListNode(node);
	return obj;
};

listHead *newList(listTemplate *template) {
	listHead *list = malloc(sizeof(listHead));
	if (!list)
		return NULL;
	list->template = template;
	list->start = NULL;
	list->end = &(list->start);
	return list;
};

void initListTemplate(listTemplate *template, handler constructor, handler destructor) {
	template->constructor = constructor;
	template->destructor = destructor;
};

void listMerge(listHead *main, listHead *extension) {
	*main->end = extension->start;
	main->end = extension->end;
	free(extension);
};

int sListPrepend(simpleList *list, void *obj) {
	listNode *node = newListNode();
	if (!node)
		return 1;
	node->obj = obj;
	if (!*list) {
		*list = node;
		node->next = node;
		return 0;
	};
	node->next = (*list)->next;
	(*list)->next = node;
	return 0;
};

int sListAppend(simpleList *list, void *obj) {
	listNode *node = newListNode();
	if (!node)
		return 1;
	node->obj = obj;
	if (!*list) {
		*list = node;
		node->next = node;
		return 0;
	};
	node->next = (*list)->next;
	(*list)->next = node;
	*list = node; //append and prepend are exactly the same save this step.
	return 0;
};

void *sListPop(simpleList *list) {
	listNode *node = (*list)->next;
	(*list)->next = node->next;
	void *obj = node->obj;
	delListNode(node);
	return obj;
};

void sListJoin(simpleList *start, simpleList *end) {
	listNode *temp;
	temp = (*start)->next;
	(*start)->next = (*end)->next;
	(*end)->next = temp;
	*end = NULL;
};