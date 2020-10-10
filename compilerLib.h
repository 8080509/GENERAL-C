#pragma once

#include "stringLib.h"
#include <stdint.h>

typedef struct _characterSpecs {
	string special;
	string whitespace;
	string breaking;
} characterSpecs;

typedef struct _strTokenIter {
	struct _characterSpecs *cSpecs;
	uint line;
	string str;
} strTokenIter;

typedef struct _fileTokenIter {
	struct _strTokenIter strTokIter;
	FILE *file;
	char *buffer;
	size_t buffLen;
	uchr eof : 1;
	uchr err : 7;
} fileTokenIter;

enum compilerErrors {
	COMP_ERR_NONE,
	COMP_ERR_FILEERR,
	COMP_ERR_BUFFLIM,
	COMP_ERR_BADBUFF,
	COMP_ERR_BADFILE,
};

void cSpecInit(characterSpecs *cSpec, char *special, char *whitespace);

void cSpecManualInit(characterSpecs *cSpec, char *special, char *whitespace, char *breaking);

#define CSPEC_FROM_LITERALS(spec, white) {.special = STRING_FROM_LITERAL(spec), .whitespace = STRING_FROM_LITERAL(white), .breaking = STRING_FROM_LITERAL(white spec)}

void cSpecClear(characterSpecs *cSpec);

void strTokenIterInit(strTokenIter *tokIter, characterSpecs *cSpec, string str, uint line);

void fileTokenIterInit(fileTokenIter *tokIter, characterSpecs *cSpec, FILE *file, char *buffer, size_t buffLen, uint line);

void fileTokenIterStringInit(fileTokenIter * tokIter, characterSpecs * cSpecs, string str, uint line);

string strTokenIterNext(strTokenIter *tokIter);

string fileTokenIterNext(fileTokenIter *tokIter);

int fileTokenCopyUntil(fileTokenIter *tokIter, string end, string *out);
