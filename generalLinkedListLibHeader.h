
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

//#define GLL_NODE_RECYCLING_STORAGE GLL_TM(, _listNodeRecyclingStorage)

//GLL_NODE *GLL_NODE_RECYCLING_STORAGE = NULL;

#define GLL_NEW_NODE GLL_TM(new_, _listNode)

GLL_NODE *GLL_NEW_NODE(void);

#define GLL_DEL_NODE GLL_TM(del_, _listNode)

void GLL_DEL_NODE(GLL_NODE *node);

void GLL_TM(clear_, _listNodeRecyclingStorage)(void);

// MAIN FUNCTIONS

int GLL_TM(, _listPrepend)(GLL_LIST *list, LIST_TYPE obj);

int GLL_TM(, _listAppend)(GLL_LIST *list, LIST_TYPE obj);

LIST_TYPE GLL_TM(, _listPop)(GLL_LIST *list);

void GLL_TM(, _listJoin)(GLL_LIST *start, GLL_LIST *end);

void GLL_TM(, _listClear)(GLL_LIST *list);

int GLL_TM(, _listDel)(GLL_LIST *list, LIST_TYPE obj, GLL_CHK_FUN eq);

LIST_TYPE GLL_TM(, _listStart)(GLL_LIST *list);

LIST_TYPE GLL_TM(, _listEnd)(GLL_LIST *list);

// MERGE SORTER FUNCTIONS

GLL_MERGE_SORTER GLL_TM(, _mergeSorter)(GLL_CHK_FUN le);

void GLL_TM(, _mergeSorterClear)(GLL_MERGE_SORTER *block);

GLL_LIST GLL_TM(, _listSortedMerge)(GLL_LIST *lower, GLL_LIST *upper, GLL_CHK_FUN le);

int GLL_TM(, _mergeSorterInsert)(GLL_MERGE_SORTER *block, LIST_TYPE item);

GLL_LIST GLL_TM(, _mergeSorterGetRes)(GLL_MERGE_SORTER *block);
