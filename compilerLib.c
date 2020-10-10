
#include <stdio.h>

#include "compilerLib.h"

void cSpecInit(characterSpecs *cSpec, char *special, char *whitespace) {
	cSpec->special = stringFromBuff(special);
	cSpec->whitespace = stringFromBuff(whitespace);
	cSpec->breaking = stringJoin(cSpec->whitespace, cSpec->special);
};

void cSpecManualInit(characterSpecs *cSpec, char *special, char *whitespace, char *breaking) {
	cSpec->special = stringFromBuff(special);
	cSpec->whitespace = stringFromBuff(whitespace);
	cSpec->breaking = stringFromBuff(breaking);
};

void cSpecClear(characterSpecs *cSpec) {
	stringDel(cSpec->breaking);
};

void strTokenIterInit(strTokenIter *tokIter, characterSpecs *cSpec, string str, uint line) {
	tokIter->cSpecs = cSpec;
	tokIter->line = line;
	tokIter->str = str;
	//stringSkipPast(&tokIter->str, tokIter->cSpecs->whitespace.data);
};

void fileTokenIterInit(fileTokenIter *tokIter, characterSpecs *cSpec, FILE *file, char *buffer, size_t buffLen, uint line) {
	tokIter->eof = 0;
	tokIter->err = 0;
	tokIter->buffer = buffer;
	tokIter->buffLen = buffLen;
	tokIter->file = file;
	tokIter->strTokIter.line = line;
	tokIter->strTokIter.cSpecs = cSpec;
	if (buffer) buffer[buffLen - 1] = '\0';
	tokIter->strTokIter.str.data = buffer + buffLen - 1;
	tokIter->strTokIter.str.len = 0;
	if (!file) tokIter->err = COMP_ERR_BADFILE;
	if (!buffer) tokIter->err = COMP_ERR_BADBUFF;
};

void fileTokenIterStringInit(fileTokenIter *tokIter, characterSpecs *cSpecs, string str, uint line) {
	tokIter->eof = 0;
	tokIter->err = 0;
	tokIter->buffer = NULL;
	tokIter->buffLen = 0;
	tokIter->file = NULL;
	tokIter->strTokIter.line = line;
	tokIter->strTokIter.cSpecs = cSpecs;
	tokIter->strTokIter.str = str;
	if (!str.data) tokIter->err = COMP_ERR_BADBUFF;
};

string strTokenIterNext(strTokenIter *tokIter) {
	char *start = tokIter->str.data;
	string temp = stringSkipPast(&tokIter->str, tokIter->cSpecs->whitespace.data);
	tokIter->line += stringCount(temp, '\n');
	if (!tokIter->str.len) return (string) { .data = NULL, .len = 0 };
	string out = stringFromBuffcspn(tokIter->str.data, tokIter->cSpecs->breaking.data);
	if (!out.len) out.len = 1;
	assert(out.len <= tokIter->str.len);
	tokIter->str.data += out.len;
	tokIter->str.len -= out.len;
	tokIter->line += stringCount(out, '\n'); //generally don't expect this... but it is a valid possiblility if newline is not a breaking character.
	return out;
};

string fileTokenIterNext(fileTokenIter *tokIter) {

	if (tokIter->eof || tokIter->err) return (string) { .data = NULL, .len = 0 }; // if the token iterator hit eof or has an error, reading is locked

	string out;
	while (1) {
		out = strTokenIterNext(&tokIter->strTokIter); //try reading from the buffer

		if (out.len == tokIter->buffLen - 1) { // use the whole buffer for one token, the buffer is too small
			tokIter->err = COMP_ERR_BUFFLIM; // may add handling if the buffer may be reallocated
			return (string) { .data = NULL, .len = 0 };
		};

		if (*tokIter->strTokIter.str.data) return out; //if the breaking character is not the null terminator, just return out.

		//if the breaking character is null, and we don't have a file, just output the token and set eof.
		if (!tokIter->file) {
			tokIter->eof = 1;
			return out;
		};

		if (out.data) {

			//if the breaking character is null, but we recived a special character, just output it
			if (stringContains(tokIter->strTokIter.cSpecs->special, *out.data)) return out;

			// if the breaking character was null and out was not a special character, it may be an incomplete token, copy the leftover token.
			out.data = memmove(
				tokIter->buffer,
				out.data,
				out.len
			);

			tokIter->strTokIter.line -= stringCount(out, '\n'); // roll back the line number since out has been put back into the buffer.
		};

		//if out.data == NULL, we reached the end of the loaded string
		assert(tokIter->strTokIter.str.len == 0);
		if (!fgets(tokIter->buffer + out.len, tokIter->buffLen - out.len, tokIter->file)) {

			if (feof(tokIter->file)) { // on eof,
				tokIter->eof = 1; // set eof flag, and
				return out; // it is possible we have a token at the end of the file - return this token
			};

			if (ferror(tokIter->file)) { // on read error, set error flag and return NULL string.
				tokIter->err = COMP_ERR_FILEERR;
				return (string) { .data = NULL, .len = 0 };
			};

			assert(0); // how did we get here?

		};

		tokIter->strTokIter.str = stringFromBuff(tokIter->buffer); // we've read new data, so reset str
		//out = strTokenIterNext(&tokIter->strTokIter); // try reading the buffer again
		//if (out.len == tokIter->buffLen - 1) { // if we still use the whole buffer for one token, the buffer is too small
		//	tokIter->err = COMP_ERR_BUFFLIM; // may add handling if the buffer may be reallocated
		//	return (string) { .data = NULL, .len = 0 };
		//};
	};

	return out;
};

int fileTokenCopyUntil(fileTokenIter *tokIter, string end, string *out) {
	
	string token;
	string separator = { .data = tokIter->strTokIter.cSpecs->whitespace.data,.len = 1 };
	while ((token = fileTokenIterNext(tokIter)), (!stringEq(&token, &end))) {
		if (!token.data) return 1;
		if (stringExtend(out, token) ||
			stringExtend(out, separator)) {
			return 2;
		};
	};
	if (!token.data && tokIter->err) return 1;
	return 0;
};