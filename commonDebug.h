
#ifdef DEBUG
#define IF_DEBUG(x) x
#else
#define IF_DEBUG(x)
#endif

#ifdef HEAP_DBG
#define IF_HEAP_DBG(x) x
#else
#define IF_HEAP_DBG(x)
#endif

#ifdef HEAP_DBG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif