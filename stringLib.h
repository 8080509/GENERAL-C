#pragma once

#include "debugControl.h"
#include "commonDebug.h"

#include "commonHeader.h"
#include <string.h>

//string datatype which simply includes a len field to help optimize
typedef struct _string {
	char *data;
	size_t len;
} string;

//creates a new string equal to the concantenation of the two given strings
string stringJoin(string a, string b);

//extends the given string a, by the given string b
//given strings shoud not overlap!
//a must have its own buffer!
//if these are not met, use stringJoin instead
int stringExtend(string *a, string b);

//copies the given string str
//a new buffer is allocated for this purpose
//useful for storing strings from a temp buffer
string stringCopy(string str);

//frees the data field in the given string
void stringDel(string str);

//creates a string from the given buffer
//only delete this if the origial buffer may be freed!
//mainly intended for construction from string literals
string stringFromBuff(char *buff);

#define STRING_FROM_LITERAL(str) { .data = str, .len = sizeof(str) - 1}

//like stringFromBuff, but only so long as characters are not from ctrl
string stringFromBuffcspn(char *buff, const char *ctrl);

//like stringFromBuff, but only so long as characters are from ctrl
string stringFromBuffspn(char *buff, const char *ctrl);

//computes the hash of the given string
hash_t stringHash(string *str);

int stringEq(string *a, string *b);

//uses a simple "text0|text1|text2|text3|..." pattern
//if str == "textN..." then stringMatch(str, pat) will return N
//advances str past the matched segment
//
//EXAMPLE: 
//str = "Hello World!"
//match(str, "Heaven|Hell|") -> 1
//str -> "o World!"
int stringMatch(string *str, string pat);

//gets the first index of the character chr in the string str
int stringIndex(string str, char chr);

//checks if the given character occurs in the given string
int stringContains(string str, char chr);

//counts the number of occurances of the given character in the given string
uint stringCount(string str, char chr);

//advances the string past the characters in control
string stringSkipPast(string *str, char *control);

//advances the string to the first character in str present in control
string stringSkipTo(string *str, char *control);
