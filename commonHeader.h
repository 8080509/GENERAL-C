#pragma once

#include <stdlib.h>
#include <assert.h>

typedef void(*handler)(void *);

typedef unsigned char uchr;

typedef unsigned int uint;

typedef uintptr_t hash_t;

typedef struct _buffer {
	size_t stride;
	size_t len;
	void *data;
} buffer;