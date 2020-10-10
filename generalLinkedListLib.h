
#include <stdlib.h>

//typedef void *void_ptr_t;

//#define LIST_TYPE void_ptr_t

#ifndef LIST_TYPE

#error LIST_TYPE macro not defined!

#endif // !LIST_TYPE


// MACRO DEFINITIONS

#define THREE_TOKEN_MERGE(pre, mid, post) pre ## mid ## post

#define THREE_TOKEN_MERGE_EVAL_1(pre, mid, post) THREE_TOKEN_MERGE(pre, mid, post)

#define GLL_TM(pre, post) THREE_TOKEN_MERGE_EVAL_1(pre, LIST_TYPE, post)

// TYPE DEFINITIONS

typedef unsigned int uint;


typedef struct GLL_TM(_linked_, _listNode) {
	struct GLL_TM(_linked_, _listNode) *next;
	LIST_TYPE obj;
} GLL_TM(linked_, _listNode);

#define GLL_NODE GLL_TM(linked_, _listNode)


typedef struct GLL_TM(_, _list) {
	struct GLL_TM(_linked_, _listNode) *node;
} GLL_TM(, _list);

#define GLL_LIST GLL_TM(, _list)


typedef int(*GLL_TM(, _checkFunction))(LIST_TYPE, LIST_TYPE);

#define GLL_CHK_FUN GLL_TM(, _checkFunction)


typedef struct GLL_TM(_, _insertableMergeSorter) {
	GLL_LIST subLists[32];
	GLL_CHK_FUN le;
} GLL_TM(, _insertableMergeSorter);

#define GLL_MERGE_SORTER GLL_TM(, _insertableMergeSorter)

// MEMORY MANAGEMENT

#define GLL_NODE_RECYCLING_STORAGE GLL_TM(, _listNodeRecyclingStorage)

GLL_NODE *GLL_NODE_RECYCLING_STORAGE = NULL;

#define GLL_NEW_NODE GLL_TM(new_, _listNode)

GLL_NODE *GLL_NEW_NODE(void) {
	//return malloc(sizeof(listNode));
	if (!GLL_NODE_RECYCLING_STORAGE) return (GLL_NODE *)malloc(sizeof(GLL_NODE));
	GLL_NODE *out = GLL_NODE_RECYCLING_STORAGE;
	GLL_NODE_RECYCLING_STORAGE = out->next;
	return out;
};

#define GLL_DEL_NODE GLL_TM(del_, _listNode)

void GLL_DEL_NODE(GLL_NODE *node) {
	//free(node); return;
	node->next = GLL_NODE_RECYCLING_STORAGE;
	GLL_NODE_RECYCLING_STORAGE = node;
};

void GLL_TM(clear_, _listNodeRecyclingStorage)(void) {
	GLL_NODE *target = GLL_NODE_RECYCLING_STORAGE;
	while (target) {
		GLL_NODE *temp = target->next;
		free(target);
		target = temp;
	};
	GLL_NODE_RECYCLING_STORAGE = NULL;
};

// MAIN FUNCTIONS

int GLL_TM(, _listPrepend)(GLL_LIST *list, LIST_TYPE obj) {
	GLL_NODE *node = GLL_NEW_NODE();
	if (!node)
		return 1;
	node->obj = obj;
	if (!list->node) {
		list->node = node;
		node->next = node;
		return 0;
	};
	node->next = list->node->next;
	list->node->next = node;
	return 0;
};

int GLL_TM(, _listAppend)(GLL_LIST *list, LIST_TYPE obj) {
	GLL_NODE *node = GLL_NEW_NODE();
	if (!node)
		return 1;
	node->obj = obj;
	if (list->node) {
		list->node = node;
		node->next = node;
		return 0;
	};
	node->next = list->node->next;
	list->node->next = node;
	list->node = node; //append and prepend are exactly the same save this step.
	return 0;
};

LIST_TYPE GLL_TM(, _listPop)(GLL_LIST *list) {
	if (!list->node)
		return (LIST_TYPE){ 0 };
	GLL_NODE *node = list->node->next;
	if (list->node != node)
		list->node->next = node->next;
	else
		list->node = NULL;
	LIST_TYPE obj = node->obj;
	GLL_DEL_NODE(node);
	return obj;
};

void GLL_TM(, _listJoin)(GLL_LIST *start, GLL_LIST *end) {
	GLL_NODE *temp;
	temp = start->node->next;
	start->node->next = end->node->next;
	end->node->next = temp;
	end->node = NULL;
};

void GLL_TM(, _listClear)(GLL_LIST *list) {
	if (!list->node)
		return;
	GLL_NODE *node = list->node->next;
	list->node->next = NULL;
	while (node) {
		GLL_NODE *nNode = node->next;
		GLL_DEL_NODE(node);
		node = nNode;
	};
	list->node = NULL;
};

int GLL_TM(, _listDel)(GLL_LIST *list, LIST_TYPE obj, GLL_CHK_FUN eq) {
	if (!list->node)
		return 1;
	GLL_NODE **tgt = &list->node;
	while ((*tgt)->next != list->node) {
		tgt = &(*tgt)->next;
		if (eq((*tgt)->obj, obj)) goto found;

	};
	//special case check for if the last node in the list contains the target value
	if (eq(list->node->obj, obj)) {
		list->node = *tgt;
		tgt = &(*tgt)->next;
		goto found;
	};
	return 1;
found:
	{
		GLL_NODE *temp = (*tgt)->next;
		GLL_DEL_NODE(*tgt);
		if (tgt != &(*tgt)->next)
			*tgt = temp;
		else
			list->node = NULL;
	};
	return 0;
};

LIST_TYPE GLL_TM(, _listStart)(GLL_LIST *list) {
	if (!list->node) return (LIST_TYPE) { 0 };
	return list->node->next->obj;
};

LIST_TYPE GLL_TM(, _listEnd)(GLL_LIST *list) {
	if (!list->node) return (LIST_TYPE) { 0 };
	return list->node->obj;
};

// MERGE SORTER FUNCTIONS

GLL_MERGE_SORTER GLL_TM(, _mergeSorter)(GLL_CHK_FUN le) {
	GLL_MERGE_SORTER block = { 0 };
	block.le = le;
	return block;
};

void GLL_TM(, _mergeSorterClear)(GLL_MERGE_SORTER *block) {
	for (uint i = 0; i < 32; i++) {
		GLL_TM(, _listClear)(block->subLists + i);
	};
};

GLL_LIST GLL_TM(, _listSortedMerge)(GLL_LIST *lower, GLL_LIST *upper, GLL_CHK_FUN le) {
	GLL_LIST out;
	if (!lower->node) {
		out = *upper;
		upper->node = NULL;
		return out;
	};
	if (!upper->node) {
		out = *lower;
		lower->node = NULL;
		return out;
	};
	GLL_NODE *lTgt = lower->node->next;
	GLL_NODE *uTgt = upper->node->next;
	GLL_NODE *lTgtEnd = lower->node;
	GLL_NODE *uTgtEnd = upper->node;
	lTgtEnd->next = NULL;
	uTgtEnd->next = NULL;
	lower->node = NULL;
	upper->node = NULL;
	GLL_NODE *start;
	GLL_NODE **end = &start;
	while (lTgt && uTgt) {
		if (le(lTgt->obj, uTgt->obj)) {
			*end = lTgt;
			end = &lTgt->next;
			lTgt = lTgt->next;
		}
		else {
			*end = uTgt;
			end = &uTgt->next;
			uTgt = uTgt->next;
		};
	};
	if (lTgt) {
		*end = lTgt;
		lTgtEnd->next = start;
		out.node = lTgtEnd;
	}
	else {
		*end = uTgt;
		uTgtEnd->next = start;
		out.node = uTgtEnd;
	};
	return out;
};

int GLL_TM(, _mergeSorterInsert)(GLL_MERGE_SORTER *block, LIST_TYPE item) {
	GLL_LIST *data = block->subLists;
	GLL_LIST temp = { NULL };
	if (GLL_TM(, _listAppend)(&temp, item)) return 1;
	while (data->node) {
		temp = GLL_TM(, _listSortedMerge)(data++, &temp, block->le);
	};
	*data = temp;
	return 0;
};

GLL_LIST GLL_TM(, _mergeSorterGetRes)(GLL_MERGE_SORTER *block) {
	GLL_LIST res = { NULL };
	for (uint i = 0; i < 32; i++) {
		res = GLL_TM(, _listSortedMerge)(block->subLists + i, &res, block->le);
	};
	return res;
};
